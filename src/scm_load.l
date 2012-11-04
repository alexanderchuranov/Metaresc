/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

%option 8bit reentrant bison-bridge bison-locations
%option warn nounput never-interactive noyywrap
%option yylineno
%option extra-type="mr_load_t*"
%option prefix="mr_scm_"
%{
#include <string.h>
#include <ctype.h>
   
#include <metaresc.h>
#include <lexer.h>  
#include <scm_load.tab.h>

#define MR_SCM_BIT_OR "bit-or"
   
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

{NUMBER} { mr_set_lval (&yylval->string, yytext, NULL); return (TOK_SCM_VALUE); }

[+-]?[Nn][Aa][Nn] |
[+-]?[Ii][Nn][Ff] |
[+-]?[[:digit:]]+"."[[:digit:]]*  |
[+-]?[[:digit:]]+[eE][+-]?[[:digit:]]* |
[+-]?[[:digit:]]+"."[[:digit:]]*[eE][+-]?[[:digit:]]* { mr_set_lval (&yylval->string, yytext, NULL); return (TOK_SCM_VALUE); }

#f { mr_set_lval (&yylval->string, NULL, NULL); return (TOK_SCM_VALUE); }

{ID} { mr_set_lval (&yylval->string, yytext, NULL); return (TOK_SCM_ID); }

"("{WS}"bit-or"{WS}{NUMBER_OR_ID}([[:space:]]+{NUMBER_OR_ID})*{WS}")" { mr_set_lval (&yylval->string, yytext, encode_bitmask); return (TOK_SCM_VALUE); }

"\""([^\"\\]|\\.)*"\"" { mr_set_lval (&yylval->string, yytext, unquote); return (TOK_SCM_VALUE); }

"#\\". { mr_set_lval (&yylval->string, &yytext[2], NULL); return (TOK_SCM_VALUE); }

"#\\x"[[:xdigit:]]{2} { mr_set_lval (&yylval->string, &yytext[3], encode_char); return (TOK_SCM_VALUE); }

";"+{WS}"("{WS}{ID}[[:space:]]+[[:digit:]]+{WS}")"{WS} {
  mr_get_id (&yylval->id_ivalue.id, strchr (yytext, '(') + 1);
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
  str_ = MR_MALLOC (length_ + 1);
  if (NULL == str_)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
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

static int
tokenize (char * str, int length, void (*handler) (char * token, int token_size, int token_count, void * context), void * context)
{
  int tok_count = 0;
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
	handler (str, tok - str, tok_count++, context);
      str = tok;
    }
  return (tok_count);
}

static void
calc_size (char * tok, int tok_size, int tok_count, void * context)
{
  int * size_ptr = context;
  if (tok_count > 0)
    *size_ptr += sizeof (MR_BITMASK_OR_DELIMITER) - 1;
  *size_ptr += tok_size;
}

static void
append (char * tok, int tok_size, int tok_count, void * context)
{
  char ** end_ptr = context;
  if (tok_count > 0)
    {
      memcpy (*end_ptr, MR_BITMASK_OR_DELIMITER, sizeof (MR_BITMASK_OR_DELIMITER) - 1);
      *end_ptr += sizeof (MR_BITMASK_OR_DELIMITER) - 1;
    }
  memcpy (*end_ptr, tok, tok_size);
  *end_ptr += tok_size;
}
  
static char *
encode_bitmask (char * str, int length)
{
  int size = 0;
  char * bitmask;
  char * end;

  str = strstr (str, MR_SCM_BIT_OR);
  if (NULL == str)
    return (NULL);
  str += sizeof (MR_SCM_BIT_OR) - 1;

  tokenize (str, length, calc_size, &size);

  bitmask = MR_MALLOC (size + 1);
  if (NULL == bitmask)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_OUT_OF_MEMORY);
      return (NULL);
    }
  end = bitmask;
  
  tokenize (str, length, append, &end);
  *end = 0;
  return (bitmask);
}

static char *
encode_char (char * str, int length)
{
  char * str_ = MR_MALLOC (2);
  int code;
  sscanf (str, "%x", &code);
  if (str_)
    {
      str_[0] = code;
      str_[1] = 0;
    }
  return (str_);
}

