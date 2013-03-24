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

#define mr_set_lval(...)
   
   char * unquote (char*, int);
   char * encode_char (char*, int);
%}

WS	[[:space:]]*
ID	[_[:alpha:]][_[:alnum:]]*
INT_NUMBER [+-]?[[:digit:]]+
HEX_NUMBER (0x|0X)[[:xdigit:]]+
NUMBER {INT_NUMBER}|{HEX_NUMBER}
FLOAT_NAN [+-]?[Nn][Aa][Nn]
FLOAT_INF [+-]?[Ii][Nn][Ff]
FLOAT_NUMBER {FLOAT_NAN}|{FLOAT_INF}|{INT_NUMBER}("."[[:digit:]]*)?([eE][+-]?{INT_NUMBER})?
IMAGENARY_NUMBER {INT_NUMBER}("."[[:digit:]]*)?([eE][+-]?{INT_NUMBER})?"i"
%%

[[:space:]]+  { return (TOK_SCM_WS); }

{NUMBER}|{FLOAT_NUMBER}|{IMAGENARY_NUMBER} { mr_set_lval (&yylval->string, yytext, NULL); return (TOK_SCM_VALUE); }

#f { mr_set_lval (&yylval->string, NULL, NULL); return (TOK_SCM_VALUE); }

{ID} { mr_set_lval (&yylval->string, yytext, NULL); return (TOK_SCM_ID); }

"\""([^\"\\]|\\.)*"\"" { mr_set_lval (&yylval->string, yytext, unquote); return (TOK_SCM_VALUE); }

"#\\". { mr_set_lval (&yylval->string, &yytext[2], NULL); return (TOK_SCM_VALUE); }

"#\\x"[[:xdigit:]]{2} { mr_set_lval (&yylval->string, &yytext[3], encode_char); return (TOK_SCM_VALUE); }

";"+{WS}"("{WS}{ID}[[:space:]]+[[:digit:]]+{WS}")"{WS} {
  mr_get_id (&yylval->id_ivalue.id, strchr (yytext, '(') + 1);
  sscanf (&yylval->id_ivalue.id.str[yylval->id_ivalue.id.length], "%d", &yylval->id_ivalue.ivalue);
  return (TOK_SCM_ID_IVALUE); }

";"[^\n]*{WS}			  { }

"("{WS}       { return (TOK_SCM_LPARENTHESIS); }

{WS}")"       { return (TOK_SCM_RPARENTHESIS); }

"#"           { return (TOK_SCM_HASH); }

[[:space:]]+"."[[:space:]]+         { return (TOK_SCM_DOT); }

.             { return (TOK_SCM_ERROR); }

%%

char * unquote (char * str, int length)
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

char *
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

