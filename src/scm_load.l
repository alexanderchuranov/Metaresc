/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

%option 8bit reentrant bison-bridge bison-locations
%option warn nounput never-interactive noyywrap
%option yylineno
%option extra-type="rl_load_t*"
%option prefix="rl_scm_"
%{
#include <string.h>
#include <ctype.h>
   
#include <reslib.h>
#include <lexer.h>  
#include <scm_load.tab.h>

#define RL_SCM_BIT_OR "bit-or"
   
   static char * unquote (char*, int);
   static char * encode_char (char*, int);
   static char * encode_bitmask (char*, int);
%}

WS	[[:space:]]*
ID	[_[:alpha:]][_[:alnum:]]*
INT_NUMBER [+-]?[[:digit:]]+
HEX_NUMBER (0x|0X)[[:xdigit:]]+
NUMBER {INT_NUMBER}|{HEX_NUMBER}
NUMBER_OR_ID {NUMBER}|{ID}
%%

[[:space:]]+  { return (TOK_SCM_WS); }

{NUMBER} { rl_set_lval (&yylval->string, yytext, NULL); return (TOK_SCM_VALUE); }

[+-]?[Nn][Aa][Nn] |
[+-]?[Ii][Nn][Ff] |
[+-]?[[:digit:]]+"."[[:digit:]]*  |
[+-]?[[:digit:]]+[eE][+-]?[[:digit:]]* |
[+-]?[[:digit:]]+"."[[:digit:]]*[eE][+-]?[[:digit:]]* { rl_set_lval (&yylval->string, yytext, NULL); return (TOK_SCM_VALUE); }

#f { rl_set_lval (&yylval->string, NULL, NULL); return (TOK_SCM_VALUE); }

{ID} { rl_set_lval (&yylval->string, yytext, NULL); return (TOK_SCM_ID); }

"("{WS}"bit-or"{WS}{NUMBER_OR_ID}([[:space:]]+{NUMBER_OR_ID})*{WS}")" { rl_set_lval (&yylval->string, yytext, encode_bitmask); return (TOK_SCM_VALUE); }

"\""([^\"\\]|\\.)*"\"" { rl_set_lval (&yylval->string, yytext, unquote); return (TOK_SCM_VALUE); }

"#\\". { rl_set_lval (&yylval->string, &yytext[2], NULL); return (TOK_SCM_VALUE); }

"#\\x"[[:digit:]]{2} { rl_set_lval (&yylval->string, &yytext[3], encode_char); return (TOK_SCM_VALUE); }

";"+{WS}"("{WS}{ID}[[:space:]]+[[:digit:]]+{WS}")"{WS} {
  rl_get_id (&yylval->id_ivalue.id, strchr (yytext, '(') + 1);
  sscanf (&yylval->id_ivalue.id.substr.data[yylval->id_ivalue.id.substr.size], "%d", &yylval->id_ivalue.ivalue);
  return (TOK_SCM_ID_IVALUE); }

";"[^\n]*{WS}			  { }

"("{WS}       { return (TOK_SCM_LPARENTHESIS); }

{WS}")"       { return (TOK_SCM_RPARENTHESIS); }

"#"           { return (TOK_SCM_HASH); }

[[:space:]]+"."[[:space:]]+         { return (TOK_SCM_DOT); }

.             { return (TOK_SCM_ERROR); }

%%

static char * unquote (char * str, int length)
{
  int length_ = 0;
  char * str_;
  int i;
#define ESC_CHAR_MAP_SIZE (256)
  static int map[ESC_CHAR_MAP_SIZE] = {
    [0 ... ESC_CHAR_MAP_SIZE - 1] = -1,
    [(unsigned char)'f'] = (unsigned char)'\f',
    [(unsigned char)'n'] = (unsigned char)'\n',
    [(unsigned char)'r'] = (unsigned char)'\r',
    [(unsigned char)'t'] = (unsigned char)'\t',
    [(unsigned char)'v'] = (unsigned char)'\v',
    [(unsigned char)'\''] = (unsigned char)'\'',
    [(unsigned char)'\"'] = (unsigned char)'\"',
    [(unsigned char)'\\'] = (unsigned char)'\\',
  };

  for (i = 1; i < length - 1; ++i) /* we need to skip quotes at the begging and end */
    {
      if ('\\' == str[i])
	{
	  ++i;
	  if (('x' == str[i]) && isalnum (str[i + 1]) && isalnum (str[i + 2]) && (';' == str[i + 3]))
	    i += 3;
	}
      ++length_;
    }
  str_ = RL_MALLOC (length_ + 1);
  if (NULL == str_)
    {
      RL_MESSAGE (RL_LL_FATAL, RL_MESSAGE_OUT_OF_MEMORY);
      return (NULL);
    }
  length_ = 0;
  for (i = 1; i < length - 1; ++i)
    {
      if ('\\' == str[i])
	{
	  int c = map[(unsigned char)str[++i]];
	  if (c > 0)
	    str_[length_++] = c;
	  else if (1 == sscanf (&str[i], "x%x;", &c))
	    {
	      i += 3;
	      str_[length_++] = c;
	    }
	  else
	    str_[length_++] = str[i];
	}
      else
	str_[length_++] = str[i];
    }
  str_[length_] = 0;
  return (str_);
}

static void
tokenize (char * str, void (*handler) (char*, int), int length)
{
  char * end = strchr (str, ')');
  if (NULL == end)
    end = &str[length];
  while (str < end)
    {
      char * tok;
      while (isspace (*str))
	str++;
      tok = str;
      while (!isspace (*tok) && (')' != *tok))
	tok++;
      if (tok != str)
	handler (str, tok - str);
      str = tok;
    }
}

static char *
encode_bitmask (char * str, int length)
{
  int size = 0;
  int tok_count = 0;
  char * bitmask;
  char * end;

  void calc_size (char * tok, int tok_size) { size += tok_size; ++tok_count; }
  void append (char * tok, int tok_size)
  {
    if (tok_count++)
      {
	memcpy (end, RL_BITMASK_OR_DELIMITER, sizeof (RL_BITMASK_OR_DELIMITER) - 1);
	end += sizeof (RL_BITMASK_OR_DELIMITER) - 1;
      }
    memcpy (end, tok, tok_size);
    end += tok_size;
  }
  
  str = strstr (str, RL_SCM_BIT_OR);
  if (NULL == str)
    return (NULL);
  str += sizeof (RL_SCM_BIT_OR) - 1;

  tokenize (str, calc_size, length);

  bitmask = RL_MALLOC (size + (tok_count - 1) * (sizeof (RL_BITMASK_OR_DELIMITER) - 1) + 1);
  if (NULL == bitmask)
    {
      RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_OUT_OF_MEMORY);
      return (NULL);
    }
  end = bitmask;
  tok_count = 0;
  
  tokenize (str, append, length);
  *end = 0;
  return (bitmask);
}

static char *
encode_char (char * str, int length)
{
  char str_[] = "\\000";
  int code;
  sscanf (str, "%x", &code);
  sprintf (str_, "\\%03o", code);
  return (RL_STRDUP (str_));
}

