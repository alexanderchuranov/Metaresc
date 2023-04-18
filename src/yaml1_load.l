/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

%option 8bit reentrant bison-bridge bison-locations
%option warn nounput never-interactive noyywrap
%option extra-type="mr_load_t*"
%option prefix="mr_yaml_"
%{
#include <string.h>
#include <complex.h>
#include <math.h>
#include <ctype.h>

#include <metaresc.h>
#include <flt_values.h>
#include <lexer.h>
#define MR_YAML_LTYPE mr_token_lloc_t
#include <yaml_load.tab.h>
#define YYSTYPE MR_YAML_STYPE
#define YYLTYPE MR_YAML_LTYPE

%}

WS           [[:space:]]+
ID           [[:alpha:]][[:alnum:]]*
INT_NUMBER   [+-]?[[:digit:]]+
FLOAT_NAN    [+-]?[Nn][Aa][Nn]
FLOAT_INF    [+-]?[Ii][Nn][Ff]
FLOAT_NUMBER {FLOAT_NAN}|{FLOAT_INF}|{INT_NUMBER}("."[[:digit:]]*)?([eE]{INT_NUMBER})?
%%

"null"  { return (TOK_YAML_NULL); }
"false" { yylval->value.value_type = MR_VT_INT; yylval->value.vt_int = false; return (TOK_YAML_NUMBER); }
"true"  { yylval->value.value_type = MR_VT_INT; yylval->value.vt_int = true; return (TOK_YAML_NUMBER); }

{INT_NUMBER}   { yylval->value.value_type = MR_VT_INT; yylval->value.vt_int = mr_strtouintmax (yytext, NULL, 0); return (TOK_YAML_NUMBER); }
{FLOAT_NUMBER} { yylval->value.value_type = MR_VT_FLOAT; yylval->value.vt_float = strtold (yytext, NULL); return (TOK_YAML_NUMBER); }

"""([^"\]|\.)*""" {
  yylval->string.str = &yytext[1];
  yylval->string.length = strlen (yytext) - 2 * sizeof (yytext[0]);
  return (TOK_YAML_STRING);
}

{WS}      { /* eat up whitespace */ }
":"       { return (TOK_YAML_COLON); }
"{"       { return (TOK_YAML_LBRACE); }
"}"       { return (TOK_YAML_RBRACE); }
"["       { return (TOK_YAML_LBRACKET); }
"]"       { return (TOK_YAML_RBRACKET); }
","       { return (TOK_YAML_COMMA); }
.         { return (TOK_YAML_ERROR); }
%%

