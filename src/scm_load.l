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

WS	[[:space:]]*
ID	[_[:alpha:]][_[:alnum:]]*
INT_NUMBER [+-]?[[:digit:]]+
HEX_NUMBER [+-]?#(x|X)[[:xdigit:]]+
OCT_NUMBER [+-]?#(o|O)[[0-7]]+
FLOAT_NAN [+-]?[Nn][Aa][Nn]
FLOAT_INF [+-]?[Ii][Nn][Ff]
FLOAT_NUMBER {FLOAT_NAN}|{FLOAT_INF}|{INT_NUMBER}("."[[:digit:]]*)?([eE][+-]?{INT_NUMBER})?
IMAGENARY_NUMBER {INT_NUMBER}("."[[:digit:]]*)?([eE][+-]?{INT_NUMBER})?[Ii]
%%

[[:space:]]+  { return (TOK_SCM_WS); }

{INT_NUMBER} { yylval->value.value_type = MR_VT_INT; yylval->value.vt_int = mr_strtouintmax (yytext, NULL, 10); return (TOK_SCM_NUMBER); }
{HEX_NUMBER} { yylval->value.value_type = MR_VT_INT; yylval->value.vt_int = mr_strtouintmax (&yytext[2], NULL, 16); return (TOK_SCM_NUMBER); }
{OCT_NUMBER} { yylval->value.value_type = MR_VT_INT; yylval->value.vt_int = mr_strtouintmax (&yytext[2], NULL, 8); return (TOK_SCM_NUMBER); }
{FLOAT_NUMBER} { yylval->value.value_type = MR_VT_FLOAT; yylval->value.vt_float = strtold (yytext, NULL); return (TOK_SCM_NUMBER); }
{IMAGENARY_NUMBER} {
  yylval->value.value_type = MR_VT_COMPLEX;
  yylval->value.vt_complex = MR_CLD_PACK (I * strtold (yytext, NULL));
  return (TOK_SCM_NUMBER);
}
[Ii] {
  yylval->value.value_type = MR_VT_COMPLEX;
  yylval->value.vt_complex = MR_CLD_PACK (I);
  return (TOK_SCM_NUMBER);
}

#f { return (TOK_SCM_FALSE); }
#t { yylval->value.value_type = MR_VT_INT; yylval->value.vt_int = true; return (TOK_SCM_NUMBER); }

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

";"+{WS}"("{WS}{ID}[[:space:]]+[[:digit:]]+{WS}")"{WS} {
  mr_get_id (&yylval->id_ivalue.id, strchr (yytext, '(') + 1);
  sscanf (&yylval->id_ivalue.id.str[yylval->id_ivalue.id.length], "%d", &yylval->id_ivalue.ivalue);
  return (TOK_SCM_ID_IVALUE);
					       }

";"[^\n]*{WS} { }

"("{WS}       { return (TOK_SCM_LPARENTHESIS); }

{WS}")"       { return (TOK_SCM_RPARENTHESIS); }

"#"           { return (TOK_SCM_HASH); }
"+"           { return (TOK_SCM_PLUS); }
"-"           { return (TOK_SCM_MINUS); }
"*"           { return (TOK_SCM_MUL); }
"/"           { return (TOK_SCM_DIV); } 
"%"           { return (TOK_SCM_MOD); } 
"logior"      { return (TOK_SCM_BIT_OR); } 
"logand"      { return (TOK_SCM_BIT_AND); }
"logxor"      { return (TOK_SCM_BIT_XOR); } 

{ID} {
  yylval->string.str = yytext;
  yylval->string.length = strlen (yytext);
  return (TOK_SCM_ID);
}

[[:space:]]+"."[[:space:]]+         { return (TOK_SCM_DOT); }

.             { return (TOK_SCM_ERROR); }

%%
