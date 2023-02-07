/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

%option 8bit reentrant bison-bridge bison-locations
%option warn nounput never-interactive noyywrap
%option extra-type="mr_load_t*"
%option prefix="mr_cinit_"
%{
#include <string.h>
#include <complex.h>
#include <math.h>
#include <ctype.h>

#include <metaresc.h>
#include <flt_values.h>
#include <lexer.h>
#define MR_CINIT_LTYPE mr_token_lloc_t
#include <cinit_load.tab.h>
#define YYSTYPE MR_CINIT_STYPE
#define YYLTYPE MR_CINIT_LTYPE

static void
get_type (mr_substr_t * substr, char * start)
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
  substr->str = start;
  substr->length = stop - start;
}

%}

%x in_comment

WS	[[:space:]]*
ID	[_[:alpha:]][_[:alnum:]]*
INT_NUMBER [+-]?[[:digit:]]+
HEX_NUMBER [+-]?(0x|0X)[[:xdigit:]]+
OCT_NUMBER [+-]?0[[0-7]]+
NUMBER {INT_NUMBER}|{HEX_NUMBER}|{OCT_NUMBER}
FLOAT_NAN [+-]?[Nn][Aa][Nn]
FLOAT_INF [+-]?[Ii][Nn][Ff]
FLOAT_NUMBER {FLOAT_NAN}|{FLOAT_INF}|{INT_NUMBER}("."[[:digit:]]*)?([eE]{INT_NUMBER})?
IMAGENARY_NUMBER {INT_NUMBER}("."[[:digit:]]*)?([eE]{INT_NUMBER})?[Ii]
%%

{WS}  { /* eat up whitespace */ }

"NULL" { return (TOK_CINIT_NULL); }
"false" { yylval->value.value_type = MR_VT_INT; yylval->value.vt_int = false; return (TOK_CINIT_NUMBER); }
"FALSE" { yylval->value.value_type = MR_VT_INT; yylval->value.vt_int = false; return (TOK_CINIT_NUMBER); }
"true" { yylval->value.value_type = MR_VT_INT; yylval->value.vt_int = true; return (TOK_CINIT_NUMBER); }
"TRUE" { yylval->value.value_type = MR_VT_INT; yylval->value.vt_int = true; return (TOK_CINIT_NUMBER); }
[Ii] { yylval->value.value_type = MR_VT_COMPLEX; yylval->value.vt_complex = MR_CLD_PACK (I); return (TOK_CINIT_NUMBER); }

{NUMBER} { yylval->value.value_type = MR_VT_INT; yylval->value.vt_int = mr_strtouintmax (yytext, NULL, 0); return (TOK_CINIT_NUMBER); }

{FLOAT_NUMBER} { yylval->value.value_type = MR_VT_FLOAT; yylval->value.vt_float = strtold (yytext, NULL); return (TOK_CINIT_NUMBER); }

{IMAGENARY_NUMBER} {
  yylval->value.value_type = MR_VT_COMPLEX;
  yylval->value.vt_complex = MR_CLD_PACK ( I * strtold (yytext, NULL) );
  return (TOK_CINIT_NUMBER);
}

{ID} {
  yylval->string.str = yytext;
  yylval->string.length = strlen (yytext);
  return (TOK_CINIT_ID);
}

"\""([^\"\\]|\\.)*"\"" {
  yylval->string.str = &yytext[1];
  yylval->string.length = strlen (yytext) - 2 * sizeof (yytext[0]);
  return (TOK_CINIT_STRING);
}

"\'"([^\'\\]|\\.)*"\'" {
  yylval->string.str = &yytext[1];
  yylval->string.length = strlen (yytext) - 2 * sizeof (yytext[0]);
  return (TOK_CINIT_CHAR);
}

"("{WS}({ID}{WS})*{ID}{WS}\*?{WS}("["{WS}"]")?{WS}")" { get_type (&yylval->string, &yytext[1]); return (TOK_CINIT_FIELD_CAST); }

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
","       { return (TOK_CINIT_COMMA); }
"."       { return (TOK_CINIT_DOT); }
"="       { return (TOK_CINIT_ASSIGN); }
.         { return (TOK_CINIT_ERROR); }
%%


