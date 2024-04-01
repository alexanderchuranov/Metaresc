/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

%option 8bit reentrant bison-bridge bison-locations
%option warn nounput never-interactive noyywrap
%option yylineno
%option extra-type="mr_load_t*"
%option prefix="mr_scm_"
%{
#include <string.h>
#include <ctype.h>

#include <metaresc.h>
#include <flt_values.h>
#include <lexer.h>
#define MR_SCM_LTYPE mr_token_lloc_t
#include <scm_load.tab.h>
#define YYSTYPE MR_SCM_STYPE
#define YYLTYPE MR_SCM_LTYPE

%}

WS	[[:space:]]+
ID	[_[:alpha:]][_[:alnum:]]*
INT_NUMBER [+-]?[[:digit:]]+
HEX_NUMBER [+-]?#(x|X)[[:xdigit:]]+
OCT_NUMBER [+-]?#(o|O)[[0-7]]+
FLOAT_NAN [+-]?[Nn][Aa][Nn]
FLOAT_INF [+-]?[Ii][Nn][Ff]
FLOAT_NUMBER {FLOAT_NAN}|{FLOAT_INF}|{INT_NUMBER}("."[[:digit:]]*)?([eE][+-]?{INT_NUMBER})?
%%

{WS}  { return (TOK_SCM_WS); }

{INT_NUMBER} {
  yylval->value.value_type = MR_VT_INT;
  yylval->value.vt_int = mr_strtouintmax (yytext, NULL, 10);
  return (TOK_SCM_NUMBER);
}

{HEX_NUMBER} {
  char * hash = strchr (yytext, '#');
  if (NULL == hash)
    return (TOK_SCM_ERROR);

  hash[0] = '0';
  yylval->value.vt_int = mr_strtouintmax (yytext, NULL, 16);
  yylval->value.value_type = MR_VT_INT;
  return (TOK_SCM_NUMBER);
}

{OCT_NUMBER} {
  char * hash = strchr (yytext, '#');
  if (NULL == hash)
    return (TOK_SCM_ERROR);

  hash[0] = '0';
  hash[1] = '0';
  yylval->value.vt_int = mr_strtouintmax (yytext, NULL, 8);
  yylval->value.value_type = MR_VT_INT;
  return (TOK_SCM_NUMBER);
}

{FLOAT_NUMBER} {
  yylval->value.value_type = MR_VT_FLOAT;
  yylval->value.vt_float = strtold (yytext, NULL);
  return (TOK_SCM_NUMBER);
}

"nil" { return (TOK_SCM_FALSE); }
"t" {
  yylval->value.value_type = MR_VT_INT;
  yylval->value.vt_int = true;
  return (TOK_SCM_NUMBER);
}

"\""([^\"\\]|\\.)*"\"" {
  yylval->string.str = yytext;
  yylval->string.length = strlen (yytext);
  return (TOK_SCM_STRING);
}

"#\\". {
  yylval->value.value_type = MR_VT_CHAR;
  yylval->value.vt_char = yytext[2];
  return (TOK_SCM_CHAR);
}

"#\\x"[[:xdigit:]]{2} {
  int code;
  sscanf (yytext, "#\\x%x", &code);
  yylval->value.value_type = MR_VT_CHAR;
  yylval->value.vt_char = code;
  return (TOK_SCM_CHAR);
}

";"+{WS}*"("{WS}*{ID}{WS}*\.{WS}*[[:digit:]]+{WS}*")"{WS}* {
  mr_get_id (&yylval->id_ivalue.id, strchr (yytext, '(') + 1);
  sscanf (strchr (yytext, '.') + 1, "%d", &yylval->id_ivalue.ivalue);
  return (TOK_SCM_ID_IVALUE);
}

";"[^\n]*{WS}* { }

"("{WS}*       { return (TOK_SCM_LPARENTHESIS); }
{WS}*")"       { return (TOK_SCM_RPARENTHESIS); }

{ID} {
  yylval->string.str = yytext;
  yylval->string.length = strlen (yytext);
  return (TOK_SCM_ID);
}

{WS}+"."{WS}+         { return (TOK_SCM_DOT); }

.             { return (TOK_SCM_ERROR); }

%%
