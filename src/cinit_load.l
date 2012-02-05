/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

%option 8bit reentrant bison-bridge bison-locations 
%option warn nounput never-interactive noyywrap
%option extra-type="mr_load_t*"
%option prefix="mr_cinit_"
%{
#include <string.h>
#include <ctype.h>
   
#include <reslib.h>
#include <lexer.h>
#include <cinit_load.tab.h>

   static char * unquote (char*, int);
   static char * unquote_char (char*, int);
   static void get_type (mr_substr_t*, char*);

%}

%x in_comment

WS	[[:space:]]*
ID	[_[:alpha:]][_[:alnum:]]*
INT_NUMBER [+-]?[[:digit:]]+
HEX_NUMBER (0x|0X)[[:xdigit:]]+
NUMBER ({INT_NUMBER}|{HEX_NUMBER})
NUMBER_OR_ID {NUMBER}|{ID}
%%

{WS}  { /* eat up whitespace */ }

{NUMBER} { mr_set_lval (&yylval->string, yytext, NULL); return (TOK_CINIT_VALUE); }

[+-]?[Nn][Aa][Nn] |
[+-]?[Ii][Nn][Ff] |
[+-]?[[:digit:]]+"."[[:digit:]]*  |
[+-]?[[:digit:]]+[eE][+-]?[[:digit:]]* |
[+-]?[[:digit:]]+"."[[:digit:]]*[eE][+-]?[[:digit:]]* { mr_set_lval (&yylval->string, yytext, NULL); return (TOK_CINIT_VALUE); }

"NULL" { mr_set_lval (&yylval->string, NULL, NULL); return (TOK_CINIT_VALUE); }

{ID} { mr_set_lval (&yylval->string, yytext, NULL); return (TOK_CINIT_VALUE); }

"."{WS}{ID}{WS}"=" { mr_get_id (&yylval->string, &yytext[1]); return (TOK_CINIT_FIELD_PREFIX); }

"\""{ID}"\""{WS}":" { mr_get_id (&yylval->string, &yytext[1]); return (TOK_CINIT_FIELD_PREFIX); }

"("{WS}({ID}{WS})*{ID}{WS}\*?{WS}("["{WS}"]")?{WS}")" { get_type (&yylval->string, &yytext[1]); return (TOK_CINIT_FIELD_CAST); }

"/*"{WS}"("{WS}({ID}{WS})*{ID}{WS}\*?{WS}"["{WS}"]"{WS}")"{WS}"*/" { get_type (&yylval->string, strchr (yytext, '(') + 1); return (TOK_CINIT_FIELD_CAST); }

({NUMBER_OR_ID}{WS}"|"{WS})*{NUMBER_OR_ID} { mr_set_lval (&yylval->string, yytext, NULL); return (TOK_CINIT_VALUE); }

"\""([^\"\\]|\\.)*"\"" { mr_set_lval (&yylval->string, yytext, unquote); return (TOK_CINIT_VALUE); }

"\'"([^\'\\]|\\.)*"\'" { mr_set_lval (&yylval->string, yytext, unquote_char); return (TOK_CINIT_VALUE); }

"/*"{WS}{ID}{WS}"="{WS}[[:digit:]]+{WS}"*/" { mr_get_id (&yylval->id_ivalue.id, &yytext[2]); sscanf (strchr (yytext, '=') + 1, "%d", &yylval->id_ivalue.ivalue); return (TOK_CINIT_ID_IVALUE); }

"/*"			  { BEGIN(in_comment); }
<in_comment>[^*\n]*        /* eat anything that's not a '*' */
<in_comment>"*"+[^*/\n]*   /* eat up '*'s not followed by '/'s */
<in_comment>\n             
<in_comment>"*"+"/"       { BEGIN(INITIAL); }

"{"       { return (TOK_CINIT_LBRACE); }
"}"       { return (TOK_CINIT_RBRACE); }
"["       { return (TOK_CINIT_LBRACKET); }
"]"       { return (TOK_CINIT_RBRACKET); }
","       { return (TOK_CINIT_COMMA); }
.         { return (TOK_CINIT_ERROR); }
%%

#define ESC_CHAR_MAP_SIZE (1 << 8)
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

static char * unquote (char * str, int length)
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

static char * unquote_char (char * str, int length)
{
  char * str_ = (char*)MR_MALLOC (length - 1);

  if (NULL == str_)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (NULL);
    }
  ++str;
  if (('\\' == str[0]) && (map[(unsigned char)str[1]] > 0))
    {
      str_[0] = map[(unsigned char)str[1]];
      str_[1] = 0;
    }
  else
    {
      memcpy (str_, str, length - 2);
      str_[length - 2] = 0;
    }
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

