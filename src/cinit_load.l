/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

%option 8bit reentrant bison-bridge bison-locations
%option warn nounput never-interactive noyywrap
%option extra-type="mr_load_t*"
%option prefix="mr_cinit_"
%{
#include <string.h>
#include <complex.h>
#include <math.h>
#include <ctype.h>
#ifdef HAVE_DLFCN_H
#define __USE_GNU
#include <dlfcn.h>
#endif /* HAVE_DLFCN_H */

#include <metaresc.h>
#include <lexer.h>
#include <cinit_load.tab.h>

   static char * unquote_str (char*, int);
   static void get_type (mr_substr_t*, char*);
   static void mr_value_id (mr_value_t*, char*);

%}

%x in_comment

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

{WS}  { /* eat up whitespace */ }

"NULL" { mr_set_lval (&yylval->string, NULL, NULL); return (TOK_CINIT_STRING); }
"FALSE" { yylval->value.value_type = MR_VT_INT; yylval->value.vt_int = FALSE; return (TOK_CINIT_NUMBER); }
"TRUE" { yylval->value.value_type = MR_VT_INT; yylval->value.vt_int = TRUE; return (TOK_CINIT_NUMBER); }
"I" { yylval->value.value_type = MR_VT_COMPLEX; yylval->value.vt_complex = I; return (TOK_CINIT_NUMBER); }

{NUMBER} { yylval->value.value_type = MR_VT_INT; yylval->value.vt_int = strtoull (yytext, NULL, 0); return (TOK_CINIT_NUMBER); }
{FLOAT_NUMBER} { yylval->value.value_type = MR_VT_FLOAT; yylval->value.vt_float = strtold (yytext, NULL); return (TOK_CINIT_NUMBER); }
{IMAGENARY_NUMBER} {
  yylval->value.value_type = MR_VT_COMPLEX;
  __real__ yylval->value.vt_complex = 0;
  __imag__ yylval->value.vt_complex = strtold (yytext, NULL);
  return (TOK_CINIT_NUMBER);
}

{ID} { mr_value_id (&yylval->value, yytext); return (TOK_CINIT_NUMBER); }

"\""([^\"\\]|\\.)*"\"" {
  yylval->value.value_type = MR_VT_STRING;
  yylval->value.vt_string.substr.data = yytext;
  yylval->value.vt_string.substr.size = strlen (yytext);
  yylval->value.vt_string.unquote = unquote_str;
  return (TOK_CINIT_STRING);
}

"\'"([^\'\\]|\\.)*"\'" {
  yylval->value.value_type = MR_VT_CHAR;
  yylval->value.vt_string.substr.data = yytext;
  yylval->value.vt_string.substr.size = strlen (yytext);
  yylval->value.vt_string.unquote = unquote_str;
  return (TOK_CINIT_STRING);
}

"."{WS}{ID}{WS}"=" { mr_get_id (&yylval->string, &yytext[1]); return (TOK_CINIT_FIELD_PREFIX); }

"\""{ID}"\""{WS}":" { mr_get_id (&yylval->string, &yytext[1]); return (TOK_CINIT_FIELD_PREFIX); }

"("{WS}({ID}{WS})*{ID}{WS}\*?{WS}("["{WS}"]")?{WS}")" { get_type (&yylval->string, &yytext[1]); return (TOK_CINIT_FIELD_CAST); }

"/*"{WS}"("{WS}({ID}{WS})*{ID}{WS}\*?{WS}"["{WS}"]"{WS}")"{WS}"*/" { get_type (&yylval->string, strchr (yytext, '(') + 1); return (TOK_CINIT_FIELD_CAST); }

"/*"{WS}{ID}{WS}"="{WS}[[:digit:]]+{WS}"*/" { mr_get_id (&yylval->id_ivalue.id, &yytext[2]); sscanf (strchr (yytext, '=') + 1, "%d", &yylval->id_ivalue.ivalue); return (TOK_CINIT_ID_IVALUE); }

"/*"			  { BEGIN(in_comment); }
<in_comment>[^*\n]*        /* eat anything that's not a '*' */
<in_comment>"*"+[^*/\n]*   /* eat up '*'s not followed by '/'s */
<in_comment>\n
<in_comment>"*"+"/"       { BEGIN(INITIAL); }

"+"       { return (TOK_CINIT_PLUS); }
"-"       { return (TOK_CINIT_MINUS); }
"*"       { return (TOK_CINIT_MUL); }
"/"       { return (TOK_CINIT_DIV); }
"%"       { return (TOK_CINIT_MOD); }
"|"       { return (TOK_CINIT_BIT_OR); }
"&"       { return (TOK_CINIT_BIT_AND); }
"^"       { return (TOK_CINIT_BIT_XOR); }

"{"       { return (TOK_CINIT_LBRACE); }
"}"       { return (TOK_CINIT_RBRACE); }
"("       { return (TOK_CINIT_LPAREN); }
")"       { return (TOK_CINIT_RPAREN); }
"["       { return (TOK_CINIT_LBRACKET); }
"]"       { return (TOK_CINIT_RBRACKET); }
","       { return (TOK_CINIT_COMMA); }
.         { return (TOK_CINIT_ERROR); }
%%

#define ESC_CHAR_MAP_SIZE (1 << CHAR_BIT)
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

char *
unquote_str (char * str, int length)
{
  int length_ = 0;
  char * str_;
  int i;

  for (i = 1; i < length - 1;)
    {
      if ('\\' == str[i++])
	{
	  int c = map[(unsigned char)str[i]];
	  if (c > 0)
	    ++i;
	  else
	    {
	      int count = 3;
	      while ((str[i] >= '0') && (str[i] < '8') && (--count >= 0))
		++i;
	    }
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
	  else if (1 == sscanf (&str[i], "%o", &c))
	    {
	      int count = 3;
	      while ((str[i] >= '0') && (str[i] < '8') && (--count >= 0))
		++i;
	      --i;
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

static void get_type (mr_substr_t * substr, char * start)
{
  char * stop;
  while (isspace (*start))
    ++start;
  stop = strchr (start, ')');
  if (stop)
    while (isspace (stop[-1]) || ('[' == stop[-1]) || (']' == stop[-1]))
      --stop;
  else
    stop = strchr (start, 0);
  substr->substr.data = start;
  substr->substr.size = stop - start;
  substr->unquote = NULL;
}

static void mr_value_id (mr_value_t * result, char * id)
{
  mr_fd_t * fdp = mr_get_enum_by_name (id);
  result->value_type = MR_VT_INT;
  result->vt_int = 0;
  if (fdp != NULL)
    result->vt_int = fdp->param.enum_value;
  else
    {
      void * func = NULL;
#ifdef HAVE_LIBDL
      func = dlsym (RTLD_DEFAULT, value);
#endif /* HAVE_LIBDL */
      if (NULL == func)
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNKNOWN_ENUM, id);
      else
	result->vt_int = (long)func;
    }
}
