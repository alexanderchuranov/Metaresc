/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

%option 8bit reentrant bison-bridge bison-locations
%option warn nounput never-interactive noyywrap
%option extra-type="mr_load_t*"
%option prefix="mr_xml1_"
%{
#define __USE_GNU
#include <string.h>
#include <ctype.h>

#include <metaresc.h>
#include <lexer.h>
#include <xml1_load.tab.h>

%}

ID	[_[:alpha:]][_[:alnum:]]*(:[_[:alpha:]][_[:alnum:]]*)?
%%

[[:space:]]+  { return (TOK_XML_WS); }

{ID} { mr_set_lval (&yylval->string, yytext, NULL); return (TOK_XML_ID); }

[[:space:]]*"="[[:space:]]* { return (TOK_XML_ASSIGN); }

"\""[^\"]*"\"" {
  yylval->string.substr.data = &yytext[1];
  yylval->string.substr.size = strlen (yytext) - 2;
  yylval->string.unquote = NULL;
  return (TOK_XML_PROP_VALUE);
}

"<?xml"  { return (TOK_XML_DOC_OPEN_TAG); }
[[:space:]]*"?>"[^<]* { return (TOK_XML_DOC_CLOSE_TAG); }

"<"{ID}  { mr_set_lval (&yylval->string, &yytext[1], NULL); return (TOK_XML_OPEN_TAG); }
"</"{ID} { mr_set_lval (&yylval->string, &yytext[2], NULL); return (TOK_XML_CLOSE_TAG); }
[[:space:]]*"/>"[^<]* { mr_set_lval (&yylval->string, strchr (yytext, '>') + 1, NULL); return (TOK_XML_CLOSE_EMPTY_TAG); }
[[:space:]]*">"[^<]* { mr_set_lval (&yylval->string, strchr (yytext, '>') + 1, xml_unquote_string); return (TOK_XML_CONTENT); }

.         { return (TOK_XML_ERROR); }
%%

