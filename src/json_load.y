/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

%code top {
#include <stdio.h>
#include <stdbool.h>

#define MR_JSON_DEBUG 0
/* Pass the argument to yyparse through to yylex. */
#define MR_JSON_LTYPE mr_token_lloc_t
#define MR_LOAD (mr_json_get_extra (scanner))
#define mr_json_error MR_PARSE_ERROR

#include <metaresc.h>
#include <mr_stringify.h>
#include <lexer.h>
#include <mr_value.h>
#include <json_load.tab.h>
#define YYSTYPE MR_JSON_STYPE
#define YYLTYPE MR_JSON_LTYPE
#include <json_load.lex.h>

}

%code {

static void
json_unquote_str (mr_substr_t * substr, char * dst)
{
  int i, size, length;

  static bool initialized = false;
  static char map[MR_ESC_CHAR_MAP_SIZE];

  if (!initialized)
    {
      memset (map, 0, sizeof (map));
      for (i = 0; i < MR_ESC_CHAR_MAP_SIZE; ++i)
	if (mr_esc_char_map[i])
	  map[(unsigned char)mr_esc_char_map[i]] = i;
      initialized = true;
    }

  if (NULL == substr->str)
    return;

  length = 0;
  for (i = 0; i < substr->length; ++i)
    {
      if ('\\' == substr->str[i])
	{
	  int c = map[(unsigned char)substr->str[++i]];
	  if (c > 0)
	    dst[length++] = c;
	  else if (1 == sscanf (&substr->str[i], "u%04x%n", &c, &size))
	    {
	      i += size - 1;
	      dst[length++] = c;
	    }
	  else
	    dst[length++] = substr->str[i];
	}
      else
	dst[length++] = substr->str[i];
    }
  dst[length] = 0;
}

}

%define api.prefix {mr_json_}
%define api.pure full
%param {void * scanner}
%locations
 /* generate include-file with symbols and types */
%defines

 /* a more advanced semantic type */
%union {
  mr_value_t value;
  mr_substr_t string;
}

/* Bison declarations.  */
%token <value> TOK_JSON_NUMBER
%token <string> TOK_JSON_STRING
%token TOK_JSON_SEMICOLON TOK_JSON_LBRACE TOK_JSON_RBRACE TOK_JSON_LBRACKET TOK_JSON_RBRACKET TOK_JSON_COMMA TOK_JSON_NULL TOK_JSON_ERROR

%start json

%% /* The grammar follows.  */

json: element

element: push_node value pop_node

pop_node: {
  mr_load_t * mr_load = MR_LOAD;
  mr_load->parent = mr_load->ptrs->ra[mr_load->parent].parent;
}

push_node: { 
  mr_load_t * mr_load = MR_LOAD; 
  mr_load->parent = mr_parse_add_node (mr_load); 
  if (mr_load->parent < 0)
    { YYERROR; }
  mr_load->ptrs->ra[mr_load->parent].idx = mr_load->parent;
}

value: object | array
| TOK_JSON_STRING {
  mr_load_t * mr_load = MR_LOAD;
  if (memchr ($1.str, '\\', $1.length))
    {
      char * buf = MR_CALLOC (1, $1.length + sizeof (char));
      if (NULL == buf)
	{
	  MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	  YYERROR;
	}
      json_unquote_str (&$1, buf);
      mr_load->ptrs->ra[mr_load->parent].value_type = MR_VT_STRING;
      mr_load->ptrs->ra[mr_load->parent].load_params.vt_string = buf;
    }
  else
    {
      mr_load->ptrs->ra[mr_load->parent].value_type = MR_VT_SUBSTR;
      mr_load->ptrs->ra[mr_load->parent].load_params.vt_substr.str = &mr_load->str[$1.str - mr_load->buf];
      mr_load->ptrs->ra[mr_load->parent].load_params.vt_substr.length = $1.length;
    }
  }
| TOK_JSON_NUMBER {
  mr_load_t * mr_load = MR_LOAD;
  mr_status_t status = mr_value_to_mr_ptrdes (&mr_load->ptrs->ra[mr_load->parent], &$1);
  if (MR_SUCCESS != status)
    { YYERROR; }
 }
| TOK_JSON_NULL {
  mr_load_t * mr_load = MR_LOAD;
  mr_load->ptrs->ra[mr_load->parent].flags.is_null = true;
  mr_load->ptrs->ra[mr_load->parent].value_type = MR_VT_INT;
  mr_load->ptrs->ra[mr_load->parent].load_params.vt_int = 0;
  }

object: TOK_JSON_LBRACE TOK_JSON_RBRACE
| TOK_JSON_LBRACE members TOK_JSON_RBRACE

members: member | member TOK_JSON_COMMA members

member: TOK_JSON_STRING TOK_JSON_SEMICOLON element {
  mr_load_t * mr_load = MR_LOAD;
  int idx = mr_load->ptrs->ra[mr_load->parent].last_child;
  $1.str[$1.length] = 0;
  mr_load->ptrs->ra[idx].fdp = mr_get_any_fd_by_name ($1.str, NULL);
  if (NULL == mr_load->ptrs->ra[idx].fdp)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNKNOWN_FIELD_NAME, $1.str);
      YYERROR;
    }
}

array: TOK_JSON_LBRACKET TOK_JSON_RBRACKET
| TOK_JSON_LBRACKET elements TOK_JSON_RBRACKET

elements: element | element TOK_JSON_COMMA elements

%%

MR_LOAD_FUNC (json);
