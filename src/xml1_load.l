/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

%option 8bit reentrant bison-bridge bison-locations 
%option warn nounput never-interactive noyywrap
%option extra-type="rl_load_t*"
%option prefix="rl_xml1_"
%{
#define __USE_GNU
#include <string.h>
#include <ctype.h>
   
#define RL_MODE PROTO /* explicitly set type of inclusion */
#include <reslib.h>
#include <lexer.h>
#include <xml1_load.tab.h>

%}

ID	[_[:alpha:]][_[:alnum:]]*(:[_[:alpha:]][_[:alnum:]]*)?
%%

[[:space:]]+  { return (TOK_XML_WS); }

{ID} { yylval->string = RL_STRDUP (yytext); return (TOK_XML_ID); }

[[:space:]]*"="[[:space:]]* { return (TOK_XML_ASSIGN); }

"\""[^\"]*"\"" { yylval->string = RL_STRDUP (&yytext[1]); yylval->string[strlen (yylval->string) - 1] = 0; return (TOK_XML_PROP_VALUE); }

"<?xml"  { return (TOK_XML_DOC_OPEN_TAG); }
[[:space:]]*"?>"[^<]* { return (TOK_XML_DOC_CLOSE_TAG); }

"<"{ID}  { yylval->string = RL_STRDUP (&yytext[1]); return (TOK_XML_OPEN_TAG); }
"</"{ID} { yylval->string = RL_STRDUP (&yytext[2]); return (TOK_XML_CLOSE_TAG); }
[[:space:]]*"/>"[^<]* { yylval->string = xml_unquote_string (strchr (yytext, '>') + 1); return (TOK_XML_CLOSE_EMPTY_TAG); }
[[:space:]]*">"[^<]* { yylval->string = xml_unquote_string (strchr (yytext, '>') + 1); return (TOK_XML_CONTENT); }

.         { return (yytext[0]); }
%%

