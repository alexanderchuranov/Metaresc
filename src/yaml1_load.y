/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

%code top {
#include <stdio.h>
#include <stdbool.h>

#define MR_YAML1_DEBUG 0
/* Pass the argument to yyparse through to yylex. */
#define MR_YAML1_LTYPE mr_token_lloc_t
#define MR_LOAD (mr_yaml1_get_extra (scanner))
#define mr_yaml1_error MR_PARSE_ERROR

#include <metaresc.h>
#include <mr_stringify.h>
#include <lexer.h>
#include <mr_value.h>
#include <yaml1_load.tab.h>
#define YYSTYPE MR_YAML1_STYPE
#define YYLTYPE MR_YAML1_LTYPE
#include <yaml1_load.lex.h>

}

%code {
void
yaml_unquote_str (mr_substr_t * substr, char * dst)
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
	  else if (1 == sscanf (&substr->str[i], "x%02x%n", &c, &size))
	    {
	      i += size - 1;
	      dst[length++] = c;
	    }
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

%define api.prefix {mr_yaml1_}
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

/* Bison declarations. */
%token <value> TOK_YAML_NUMBER
%token <string> TOK_YAML_STRING
%token TOK_YAML_SEMICOLON TOK_YAML_LBRACE TOK_YAML_RBRACE TOK_YAML_LBRACKET TOK_YAML_RBRACKET TOK_YAML_COMMA TOK_YAML_NULL TOK_YAML_ERROR

%start yaml

%% /* The grammar follows. */

yaml: element

element: push_node value pop_node

pop_node: {
  mr_load_t * mr_load = MR_LOAD;
  mr_load->parent = mr_load->ptrs->ra[mr_load->parent].parent;
}

push_node: {
  mr_load_t * mr_load = MR_LOAD;
  mr_load->parent = mr_parse_add_node (mr_load);
  if (mr_load->parent < 0)
  {
    YYERROR;
  }
  mr_load->ptrs->ra[mr_load->parent].idx = mr_load->parent;
}

value: object | array
| TOK_YAML_STRING {
  mr_load_t * mr_load = MR_LOAD;
  mr_load->ptrs->ra[mr_load->parent].load_params.mr_value.value_type = MR_VT_QUOTED_SUBSTR;
  mr_load->ptrs->ra[mr_load->parent].load_params.mr_value.vt_quoted_substr.substr.str = &mr_load->str[$1.str - mr_load->buf];
  mr_load->ptrs->ra[mr_load->parent].load_params.mr_value.vt_quoted_substr.substr.length = $1.length;
  mr_load->ptrs->ra[mr_load->parent].load_params.mr_value.vt_quoted_substr.unquote = yaml_unquote_str;
  }
| TOK_YAML_NUMBER {
  mr_load_t * mr_load = MR_LOAD;
  mr_load->ptrs->ra[mr_load->parent].load_params.mr_value = $1;
  }
| TOK_YAML_NULL {
  mr_load_t * mr_load = MR_LOAD;
  mr_load->ptrs->ra[mr_load->parent].flags.is_null = true;
  mr_load->ptrs->ra[mr_load->parent].load_params.mr_value.value_type = MR_VT_INT;
  mr_load->ptrs->ra[mr_load->parent].load_params.mr_value.vt_int = 0;
  }

object: TOK_YAML_LBRACE TOK_YAML_RBRACE
| TOK_YAML_LBRACE members TOK_YAML_RBRACE

members: member | member TOK_YAML_COMMA members

member: TOK_YAML_STRING TOK_YAML_SEMICOLON element {
  mr_load_t * mr_load = MR_LOAD;
  int idx = mr_load->ptrs->ra[mr_load->parent].last_child;
  mr_load->ptrs->ra[idx].name = mr_get_static_field_name_from_substring (&$1);
}

array: TOK_YAML_LBRACKET TOK_YAML_RBRACKET
| TOK_YAML_LBRACKET elements TOK_YAML_RBRACKET

elements: element | element TOK_YAML_COMMA elements

%%

MR_LOAD_FUNC (yaml1);