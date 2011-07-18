/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

%option 8bit reentrant bison-bridge bison-locations 
%option warn nounput never-interactive noyywrap
%option extra-type="rl_load_t*"
%option prefix="rl_cinit_"
%{
#include <string.h>
#include <ctype.h>
   
#define RL_MODE PROTO /* explicitly set type of inclusion */
#include <reslib.h>
#include <lexer.h>
#include <cinit_load.tab.h>

   static char * unquote (char*);
   static char * unquote_char (char*);
   static char * get_type (char*);
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

{NUMBER} { yylval->string = RL_STRDUP (yytext); return (TOK_CINIT_VALUE); }

[+-]?[Nn][Aa][Nn] |
[+-]?[Ii][Nn][Ff] |
[+-]?[[:digit:]]+"."[[:digit:]]*  |
[+-]?[[:digit:]]+[eE][+-]?[[:digit:]]* |
[+-]?[[:digit:]]+"."[[:digit:]]*[eE][+-]?[[:digit:]]* { yylval->string = RL_STRDUP (yytext); return (TOK_CINIT_VALUE); }

"NULL" { yylval->string = NULL; return (TOK_CINIT_VALUE); }

{ID} { yylval->string = RL_STRDUP (yytext); return (TOK_CINIT_VALUE); }

"."{WS}{ID}{WS}"=" { yylval->string = rl_get_id (&yytext[1]); return (TOK_CINIT_FIELD_PREFIX); }

{ID}{WS}":" { yylval->string = rl_get_id (yytext); return (TOK_CINIT_FIELD_PREFIX); }

"("({ID}{WS})*{ID}{WS}"["{WS}"]"{WS}")"  { yylval->string = get_type (&yytext[1]); return (TOK_CINIT_FIELD_CAST); }

({NUMBER_OR_ID}{WS}"|"{WS})*{NUMBER_OR_ID} { yylval->string = RL_STRDUP (yytext); return (TOK_CINIT_VALUE); }

"\""([^\"\\]|\\.)*"\"" { yylval->string = unquote (yytext); return (TOK_CINIT_VALUE); }

"\'"([^\'\\]|\\.)*"\'" { yylval->string = unquote_char (yytext); return (TOK_CINIT_VALUE); }

"/*"{WS}{ID}{WS}"="{WS}[[:digit:]]+{WS}"*/" { yylval->id_ivalue.id = rl_get_id (&yytext[2]); sscanf (strchr (yytext, '=') + 1, "%d", &yylval->id_ivalue.ivalue); return (TOK_CINIT_ID_IVALUE); }

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
.         { return (yytext[0]); }
%%

#define ESC_CHAR_MAP_SIZE (256)
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

static char * unquote (char * str)
{
  int length = strlen (str) - 1;
  int length_ = 0;
  char * str_;
  int i;

  for (i = 1; i < length;)
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
  str_ = RL_MALLOC (length_ + 1);
  if (NULL == str_)
    {
      RL_MESSAGE (RL_LL_FATAL, RL_MESSAGE_OUT_OF_MEMORY);
      return (NULL);
    }

  length_ = 0;
  for (i = 1; i < length; ++i)
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

static char * unquote_char (char * str)
{
  int length;
  char * str_;
  ++str;
  if (('\\' == str[0]) && (map[(unsigned char)str[1]] > 0))
    {
      str[1] = map[(unsigned char)str[1]];
      ++str;
    }
  length = strlen (str);
  str_ = (char*)RL_MALLOC (length);
  if (NULL == str_)
    {
      RL_MESSAGE (RL_LL_FATAL, RL_MESSAGE_OUT_OF_MEMORY);
      return (NULL);
    }
  memcpy (str_, str, length - 1);
  str_[length - 1] = 0;
  return (str_);
}

static char * get_type (char * start)
{
  int size;
  char * id;
  char * stop;
  while (isspace (*start)) ++start;
  stop = strchr (start, '[');
  if (stop)
    while (isspace (stop[-1])) --stop;
  else
    stop = strchr (start, 0);
  size = stop - start;
  id = RL_MALLOC (size + 1);
  if (id)
    {
      memcpy (id, start, size);
      id[size] = 0;
    }
  return (id);
}
