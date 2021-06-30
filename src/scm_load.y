/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

%code top {
#include <stdio.h>

#define MR_SCM_DEBUG 0
/* Pass the argument to yyparse through to yylex. */
#define MR_SCM_LTYPE mr_token_lloc_t
#define MR_LOAD (mr_scm_get_extra (scanner))
#define mr_scm_error MR_PARSE_ERROR

#include <metaresc.h>
#include <mr_stringify.h>
#include <lexer.h>
#include <mr_value.h>
#include <scm_load.tab.h>
#define YYSTYPE MR_SCM_STYPE
#define YYLTYPE MR_SCM_LTYPE
#include <scm_load.lex.h>

#define SIZEOF_QUOTE_CHAR (sizeof ("\"") - sizeof (""))

}

%code {
  void scm_unquote_str (mr_substr_t * substr, char * dst)
  {
    int size, length;
    int i;

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
	    if (c)
	      dst[length++] = c;
	    else if (1 == sscanf (&substr->str[i], "x%x%n;", &c, &size))
	      {
		i += size;
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

%define api.prefix {mr_scm_}
%define api.pure full
%param {void * scanner}
%locations
 /* generate include-file with symbols and types */
%defines

 /* a more advanced semantic type */
%union {
  mr_value_t value;
  mr_substr_t string;
  struct {
    mr_substr_t id;
    int ivalue;
  } id_ivalue;
}

/* Bison declarations.  */
%token <string> TOK_SCM_VALUE TOK_SCM_ID TOK_SCM_STRING
%token <value> TOK_SCM_CHAR TOK_SCM_NUMBER
%token <id_ivalue> TOK_SCM_ID_IVALUE
%token TOK_SCM_FALSE TOK_SCM_WS TOK_SCM_LPARENTHESIS TOK_SCM_RPARENTHESIS TOK_SCM_DOT TOK_SCM_HASH TOK_SCM_ERROR TOK_SCM_PLUS TOK_SCM_MINUS TOK_SCM_MUL TOK_SCM_DIV TOK_SCM_MOD TOK_SCM_BIT_OR TOK_SCM_BIT_AND TOK_SCM_BIT_XOR

%type <value> expr plus_list minus_list mul_list div_list bit_or_list bit_and_list bit_xor_list

%start ws_scm_ws

%% /* The grammar follows.  */

ws_scm_ws: ws scm ws

scm: start_node scm_stmt { mr_load_t * mr_load = MR_LOAD; mr_load->parent = mr_load->ptrs->ra[mr_load->parent].parent; }

start_node: { 
  mr_load_t * mr_load = MR_LOAD; 
  mr_load->parent = mr_parse_add_node (mr_load); 
  if (mr_load->parent < 0)
    {
      YYERROR;
    }
}

scm_stmt:
value
| TOK_SCM_ID_IVALUE scm_stmt {
  mr_load_t * mr_load = MR_LOAD;
  if ($1.id.str && $1.id.length)
    {
      if (0 == mr_substrcmp (MR_REF, &$1.id))
	mr_load->ptrs->ra[mr_load->parent].ref_idx = $1.ivalue;
      else if (0 == mr_substrcmp (MR_REF_CONTENT, &$1.id))
	{
	  mr_load->ptrs->ra[mr_load->parent].ref_idx = $1.ivalue;
	  mr_load->ptrs->ra[mr_load->parent].flags.is_content_reference = true;
	}
      else if (0 == mr_substrcmp (MR_REF_IDX, &$1.id))
	mr_load->ptrs->ra[mr_load->parent].idx = $1.ivalue;
    }
}

value:
compaund
| named_node
| TOK_SCM_HASH compaund
| TOK_SCM_CHAR { mr_load_t * mr_load = MR_LOAD; mr_load->ptrs->ra[mr_load->parent].load_params.mr_value = $1; }
| expr { mr_load_t * mr_load = MR_LOAD; mr_load->ptrs->ra[mr_load->parent].load_params.mr_value = $1; }
| TOK_SCM_STRING {
  mr_load_t * mr_load = MR_LOAD;
  mr_load->ptrs->ra[mr_load->parent].load_params.mr_value.vt_quoted_substr.substr.str = &mr_load->str[$1.str - mr_load->buf + SIZEOF_QUOTE_CHAR];
  mr_load->ptrs->ra[mr_load->parent].load_params.mr_value.vt_quoted_substr.substr.length = $1.length - 2 * SIZEOF_QUOTE_CHAR;
  mr_load->ptrs->ra[mr_load->parent].load_params.mr_value.vt_quoted_substr.unquote = scm_unquote_str;
  mr_load->ptrs->ra[mr_load->parent].load_params.mr_value.value_type = MR_VT_QUOTED_SUBSTR;
}
| TOK_SCM_FALSE {
  mr_load_t * mr_load = MR_LOAD;
  mr_load->ptrs->ra[mr_load->parent].flags.is_null = true;
  mr_load->ptrs->ra[mr_load->parent].load_params.mr_value.value_type = MR_VT_INT;
  mr_load->ptrs->ra[mr_load->parent].load_params.mr_value.vt_int = false;
  }

expr:
TOK_SCM_NUMBER { $$ = $1; }
| TOK_SCM_ID {
  mr_load_t * mr_load = MR_LOAD;
  $$.value_type = MR_VT_ID;
  $$.vt_quoted_substr.substr.str = &mr_load->str[$1.str - mr_load->buf];
  $$.vt_quoted_substr.substr.length = $1.length;
  $$.vt_quoted_substr.unquote = NULL;
  }
| TOK_SCM_LPARENTHESIS TOK_SCM_PLUS ws plus_list TOK_SCM_RPARENTHESIS { $$ = $4; }
| TOK_SCM_LPARENTHESIS TOK_SCM_MINUS ws minus_list TOK_SCM_RPARENTHESIS { $$ = $4; }
| TOK_SCM_LPARENTHESIS TOK_SCM_MUL ws mul_list TOK_SCM_RPARENTHESIS { $$ = $4; }
| TOK_SCM_LPARENTHESIS TOK_SCM_DIV ws expr TOK_SCM_WS div_list TOK_SCM_RPARENTHESIS { mr_value_div (&$$, &$4, &$6); }
| TOK_SCM_LPARENTHESIS TOK_SCM_MOD ws expr TOK_SCM_WS expr TOK_SCM_RPARENTHESIS { mr_value_mod (&$$, &$4, &$6); }
| TOK_SCM_LPARENTHESIS TOK_SCM_BIT_OR ws bit_or_list TOK_SCM_RPARENTHESIS { $$ = $4; }
| TOK_SCM_LPARENTHESIS TOK_SCM_BIT_AND ws bit_and_list TOK_SCM_RPARENTHESIS { $$ = $4; }
| TOK_SCM_LPARENTHESIS TOK_SCM_BIT_XOR ws bit_xor_list TOK_SCM_RPARENTHESIS { $$ = $4; }

plus_list: { $$.value_type = MR_VT_INT; $$.vt_int = 0; }
| expr TOK_SCM_WS plus_list { mr_value_add (&$$, &$1, &$3); }
| expr { $$ = $1; }

minus_list: { $$.value_type = MR_VT_INT; $$.vt_int = 0; }
| expr TOK_SCM_WS plus_list { mr_value_sub (&$$, &$1, &$3); }
| expr { $$ = $1; mr_value_neg (&$$); }

mul_list: { $$.value_type = MR_VT_INT; $$.vt_int = 1; }
| expr TOK_SCM_WS mul_list { mr_value_mul (&$$, &$1, &$3); }
| expr { $$ = $1; }

div_list: expr { $$ = $1; }
| expr TOK_SCM_WS mul_list { mr_value_mul (&$$, &$1, &$3); }

bit_or_list: { $$.value_type = MR_VT_INT; $$.vt_int = 0; }
| expr TOK_SCM_WS bit_or_list { mr_value_bit_or (&$$, &$1, &$3); }
| expr { $$ = $1; }

bit_and_list: { $$.value_type = MR_VT_INT; $$.vt_int = 0; }
| expr TOK_SCM_WS bit_and_list { mr_value_bit_and (&$$, &$1, &$3); }
| expr { $$ = $1; }

bit_xor_list: { $$.value_type = MR_VT_INT; $$.vt_int = 0; }
| expr TOK_SCM_WS bit_xor_list { mr_value_bit_xor (&$$, &$1, &$3); }
| expr { $$ = $1; }

compaund: TOK_SCM_LPARENTHESIS list TOK_SCM_RPARENTHESIS

named_node: TOK_SCM_LPARENTHESIS scm TOK_SCM_DOT TOK_SCM_ID TOK_SCM_RPARENTHESIS {
  mr_load_t * mr_load = MR_LOAD;
  int self = mr_load->parent;
  int parent = mr_load->ptrs->ra[self].parent;
  int prev = mr_load->ptrs->ra[self].prev;
  int child = mr_load->ptrs->ra[self].first_child;
  if (parent >= 0)
    {
      if (mr_load->ptrs->ra[parent].first_child == self)
	mr_load->ptrs->ra[parent].first_child = child;
      if (mr_load->ptrs->ra[parent].last_child == self)
	mr_load->ptrs->ra[parent].last_child = child;
    }
  if (prev >= 0)
    mr_load->ptrs->ra[prev].next = child;
  mr_load->ptrs->ra[child].prev = prev;

  mr_load->ptrs->ra[child].parent = parent;
  mr_load->ptrs->ra[child].name = mr_get_static_field_name_from_substring (&$4);

  mr_load->parent = child;
}

list: | nonempty_list

nonempty_list: scm | nonempty_list TOK_SCM_WS scm

ws: | TOK_SCM_WS

%%

MR_LOAD_FUNC (scm);
