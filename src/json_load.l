/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

%option 8bit reentrant bison-bridge bison-locations
%option warn nounput never-interactive noyywrap
%option extra-type="mr_load_t*"
%option prefix="mr_json_"
%{
#include <string.h>
#include <complex.h>
#include <math.h>
#include <ctype.h>

#include <metaresc.h>
#include <lexer.h>
#define MR_JSON_LTYPE mr_token_lloc_t
#include <json_load.tab.h>
#define YYSTYPE MR_JSON_STYPE
#define YYLTYPE MR_JSON_LTYPE

%}

WS	[[:space:]]+
ID	[_[:alpha:]][_[:alnum:]]*
INT_NUMBER [+-]?[[:digit:]]+
FLOAT_NAN [+-]?[Nn][Aa][Nn]
FLOAT_INF [+-]?[Ii][Nn][Ff]
FLOAT_NUMBER {FLOAT_NAN}|{FLOAT_INF}|{INT_NUMBER}("."[[:digit:]]*)?([eE]{INT_NUMBER})?
%%

"null" { return (TOK_JSON_NULL); }
"false" { yylval->value.value_type = MR_VT_INTMAX; yylval->value.vt_intmax = false; return (TOK_JSON_NUMBER); }
"true" { yylval->value.value_type = MR_VT_INTMAX; yylval->value.vt_intmax = true; return (TOK_JSON_NUMBER); }

{INT_NUMBER} { yylval->value.value_type = MR_VT_INTMAX; yylval->value.vt_intmax = mr_strtouintmax (yytext, NULL, 0); return (TOK_JSON_NUMBER); }
{FLOAT_NUMBER} { yylval->value.value_type = MR_VT_LONG_DOUBLE; yylval->value.vt_long_double = strtold (yytext, NULL); return (TOK_JSON_NUMBER); }

"\""([^\"\\]|\\.)*"\"" {
  yylval->string.str = &yytext[1];
  yylval->string.length = strlen (yytext) - 2 * sizeof (yytext[0]);
  return (TOK_JSON_STRING);
}

{WS}      { /* eat up whitespace */ }
":"       { return (TOK_JSON_SEMICOLON); }
"{"       { return (TOK_JSON_LBRACE); }
"}"       { return (TOK_JSON_RBRACE); }
"["       { return (TOK_JSON_LBRACKET); }
"]"       { return (TOK_JSON_RBRACKET); }
","       { return (TOK_JSON_COMMA); }
.         { return (TOK_JSON_ERROR); }
%%


