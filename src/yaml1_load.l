/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

%option 8bit reentrant bison-bridge bison-locations
%option warn nounput never-interactive noyywrap
%option extra-type="mr_load_t*"
%option prefix="mr_yaml1_"
%{
#include <string.h>
#include <complex.h>
#include <math.h>
#include <ctype.h>

#include <metaresc.h>
#include <flt_values.h>
#include <lexer.h>
#define MR_YAML1_LTYPE mr_token_lloc_t
#include <yaml1_load.tab.h>
#define YYSTYPE MR_YAML1_STYPE
#define YYLTYPE MR_YAML1_LTYPE

%}

WS	[[:space:]]+
ID	[_[:alpha:]][_[:alnum:]]*
INT_NUMBER [+-]?[[:digit:]]+
FLOAT_NAN [+-]?[Nn][Aa][Nn]
FLOAT_INF [+-]?[Ii][Nn][Ff]
FLOAT_NUMBER {FLOAT_NAN}|{FLOAT_INF}|{INT_NUMBER}("."[[:digit:]]*)?([eE]{INT_NUMBER})?
%%

"null" { return (TOK_YAML1_NULL); }
"false" { yylval->value.value_type = MR_VT_INT; yylval->value.vt_int = false; return (TOK_YAML1_NUMBER); }
"true" { yylval->value.value_type = MR_VT_INT; yylval->value.vt_int = true; return (TOK_YAML1_NUMBER); }

{INT_NUMBER} { yylval->value.value_type = MR_VT_INT; yylval->value.vt_int = mr_strtouintmax (yytext, NULL, 0); return (TOK_YAML1_NUMBER); }
{FLOAT_NUMBER} { yylval->value.value_type = MR_VT_FLOAT; yylval->value.vt_float = strtold (yytext, NULL); return (TOK_YAML1_NUMBER); }

"\""([^\"\\]|\\.)*"\"" {
  yylval->string.str = &yytext[1];
  yylval->string.length = strlen (yytext) - 2 * sizeof (yytext[0]);
  return (TOK_YAML1_STRING);
}

{WS}      { /* eat up whitespace */ }
":"       { return (TOK_YAML1_SEMICOLON); }
"{"       { return (TOK_YAML1_LBRACE); }
"}"       { return (TOK_YAML1_RBRACE); }
"["       { return (TOK_YAML1_LBRACKET); }
"]"       { return (TOK_YAML1_RBRACKET); }
","       { return (TOK_YAML1_COMMA); }
.         { return (TOK_YAML1_ERROR); }
%%


