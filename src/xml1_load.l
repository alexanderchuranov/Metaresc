/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

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
#define MR_XML1_LTYPE mr_token_lloc_t
#include <xml1_load.tab.h>
#define YYSTYPE MR_XML1_STYPE
#define YYLTYPE MR_XML1_LTYPE

%}

ID	[_[:alpha:]][_[:alnum:]]*(:[_[:alpha:]][_[:alnum:]]*)?
%%

[[:space:]]+  { return (TOK_XML_WS); }

{ID} {
  yylval->string.str = yytext;
  yylval->string.length = strlen (yytext);
  return (TOK_XML_ID);
}

[[:space:]]*"="[[:space:]]* { return (TOK_XML_ASSIGN); }

"\""[^\"]*"\"" {
  yylval->string.str = &yytext[1];
  yylval->string.length = strlen (yytext) - 2;
  return (TOK_XML_PROP_VALUE);
}

"<?xml"  { return (TOK_XML_DOC_OPEN_TAG); }
[[:space:]]*"?>"[^<]* { return (TOK_XML_DOC_CLOSE_TAG); }

"<"{ID}  {
  yylval->string.str = &yytext[1];
  yylval->string.length = strlen (&yytext[1]);
  return (TOK_XML_OPEN_TAG);
}
  
"</"{ID} {
  yylval->string.str = &yytext[2];

  yylval->string.length = strlen (&yytext[2]);
  return (TOK_XML_CLOSE_TAG);
}
  
[[:space:]]*"/>"[^<]* {
  yylval->string.str = strchr (yytext, '>') + 1;
  yylval->string.length = strlen (yylval->string.str);
  return (TOK_XML_CLOSE_EMPTY_TAG);
}

[[:space:]]*">"[^<]* {
  yylval->string.str = strchr (yytext, '>') + 1;
  yylval->string.length = strlen (yylval->string.str);
  return (TOK_XML_CONTENT);
}

.         { return (TOK_XML_ERROR); }
%%

