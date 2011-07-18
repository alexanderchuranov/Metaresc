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
   
#define RL_MODE PROTO /* explicitly set type of inclusion */
#include <reslib.h>
#include <lexer.h>  
#include <scm_load.tab.h>

#define RL_SCM_BIT_OR "bit-or"
   
   static char * unquote (char*);
   static char * encode_char (char*);
   static char * encode_bitmask (char*);
%}

WS	[[:space:]]*
ID	[_[:alpha:]][_[:alnum:]]*
INT_NUMBER [+-]?[[:digit:]]+
HEX_NUMBER (0x|0X)[[:xdigit:]]+
NUMBER {INT_NUMBER}|{HEX_NUMBER}
NUMBER_OR_ID {NUMBER}|{ID}
%%

[[:space:]]+  { return (TOK_SCM_WS); }

{NUMBER} { yylval->string = RL_STRDUP (yytext); return (TOK_SCM_VALUE); }

[+-]?[Nn][Aa][Nn] |
[+-]?[Ii][Nn][Ff] |
[+-]?[[:digit:]]+"."[[:digit:]]*  |
[+-]?[[:digit:]]+[eE][+-]?[[:digit:]]* |
[+-]?[[:digit:]]+"."[[:digit:]]*[eE][+-]?[[:digit:]]* { yylval->string = RL_STRDUP (yytext); return (TOK_SCM_VALUE); }

#f { yylval->string = NULL; return (TOK_SCM_VALUE); }

{ID} { yylval->string = RL_STRDUP (yytext); return (TOK_SCM_ID); }

"("{WS}"bit-or"{WS}{NUMBER_OR_ID}([[:space:]]+{NUMBER_OR_ID})*{WS}")" { yylval->string = encode_bitmask (yytext); return (TOK_SCM_VALUE); }

"\""([^\"\\]|\\.)*"\"" { yylval->string = unquote (yytext); return (TOK_SCM_VALUE); }

"#\\". { yylval->string = RL_STRDUP (&yytext[2]); return (TOK_SCM_VALUE); }

"#\\x"[[:digit:]]{2} { yylval->string = encode_char (&yytext[3]); return (TOK_SCM_VALUE); }

";"+{WS}"("{WS}{ID}[[:space:]]+[[:digit:]]+{WS}")"{WS} {
  char * ptr = strchr (yytext, '(') + 1;
  yylval->id_ivalue.id = rl_get_id (ptr);
  while (isspace (*ptr)) ++ptr;
  while (isalnum (*ptr) || ('_' == *ptr)) ++ptr;
  sscanf (ptr, "%d", &yylval->id_ivalue.ivalue);
  return (TOK_SCM_ID_IVALUE); }

";"[^\n]*{WS}			  { }

"("{WS}       { return (TOK_SCM_LPARENTHESIS); }

{WS}")"       { return (TOK_SCM_RPARENTHESIS); }

"#"           { return (TOK_SCM_HASH); }

[[:space:]]+"."[[:space:]]+         { return (TOK_SCM_DOT); }

.             { return (yytext[0]); }

%%

static char * unquote (char * str)
{
  int length = strlen (str) - 1;
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

  for (i = 1; i < length; ++i)
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
  for (i = 1; i < length; ++i)
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
tokenize (char * str, void (*handler) (char*, int))
{
  char * end = strchr (str, ')');
  if (NULL == end)
    end = &str[strlen (str)];
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
encode_bitmask (char * str)
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

  tokenize (str, calc_size);

  bitmask = RL_MALLOC (size + (tok_count - 1) * (sizeof (RL_BITMASK_OR_DELIMITER) - 1) + 1);
  if (NULL == bitmask)
    {
      RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_OUT_OF_MEMORY);
      return (NULL);
    }
  end = bitmask;
  tok_count = 0;
  
  tokenize (str, append);
  *end = 0;
  return (bitmask);
}

static char *
encode_char (char * str)
{
  char str_[] = "\\000";
  int code;
  sscanf (str, "%x", &code);
  sprintf (str_, "\\%03o", code);
  return (RL_STRDUP (str_));
}

