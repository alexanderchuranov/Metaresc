/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

%{
#include <stdio.h>

#include <metaresc.h>
#include <lexer.h>
#include <scm_load.tab.h>
#include <scm_load.lex.h>

/* Pass the argument to yyparse through to yylex. */
#define YYPARSE_PARAM scanner
#define YYLEX_PARAM YYPARSE_PARAM
#define MR_LOAD (mr_scm_get_extra (YYPARSE_PARAM))
#define mr_scm_error(ERROR) MR_PARSE_ERROR (ERROR, YYPARSE_PARAM, scm)
%}

%name-prefix="mr_scm_"
%pure-parser
%locations
 /* generate include-file with symbols and types */
%defines

 /* a more advanced semantic type */
%union {
  mr_substr_t string;
  struct {
    mr_substr_t id;
    int ivalue;
  } id_ivalue;
}

/* Bison declarations.  */
%token <string> TOK_SCM_VALUE
%token <string> TOK_SCM_ID
%token <id_ivalue> TOK_SCM_ID_IVALUE
%token TOK_SCM_WS
%token TOK_SCM_LPARENTHESIS
%token TOK_SCM_RPARENTHESIS
%token TOK_SCM_DOT
%token TOK_SCM_HASH
%token TOK_SCM_ERROR

%start ws_scm_ws

%% /* The grammar follows.  */

ws_scm_ws: ws scm ws

scm: start_node scm_stmt { mr_load_t * mr_load = MR_LOAD; mr_load->parent = mr_load->ptrs->ra.data[mr_load->parent].parent; }

start_node: { mr_load_t * mr_load = MR_LOAD; mr_load->parent = mr_parse_add_node (mr_load); }

scm_stmt:
value
| TOK_SCM_ID_IVALUE scm_stmt {
  mr_load_t * mr_load = MR_LOAD;
  if ($1.id.substr.data && $1.id.substr.size)
    {
      if (0 == mr_substrcmp (MR_REF, &$1.id))
	mr_load->ptrs->ra.data[mr_load->parent].ref_idx = $1.ivalue;
      else if (0 == mr_substrcmp (MR_REF_CONTENT, &$1.id))
	{
	  mr_load->ptrs->ra.data[mr_load->parent].ref_idx = $1.ivalue;
	  mr_load->ptrs->ra.data[mr_load->parent].flags.is_content_reference = MR_TRUE;
	}
      else if (0 == mr_substrcmp (MR_REF_IDX, &$1.id))
	mr_load->ptrs->ra.data[mr_load->parent].idx = $1.ivalue;
    }
}

value:
compaund
| named_node 
| TOK_SCM_HASH compaund 
| TOK_SCM_VALUE { mr_load_t * mr_load = MR_LOAD; mr_load->ptrs->ra.data[mr_load->parent].value = mr_unquote (&$1); }
| TOK_SCM_ID { mr_load_t * mr_load = MR_LOAD; mr_load->ptrs->ra.data[mr_load->parent].value = mr_unquote (&$1); }

compaund: TOK_SCM_LPARENTHESIS list TOK_SCM_RPARENTHESIS

named_node: TOK_SCM_LPARENTHESIS scm TOK_SCM_DOT TOK_SCM_ID TOK_SCM_RPARENTHESIS {
  mr_load_t * mr_load = MR_LOAD;
  int self = mr_load->parent;
  int parent = mr_load->ptrs->ra.data[self].parent;
  int prev = mr_load->ptrs->ra.data[self].prev;
  int child = mr_load->ptrs->ra.data[self].first_child;
  char * name = mr_unquote (&$4);
  if (parent >= 0)
    {
      if (mr_load->ptrs->ra.data[parent].first_child == self)
	mr_load->ptrs->ra.data[parent].first_child = child;
      if (mr_load->ptrs->ra.data[parent].last_child == self)
	mr_load->ptrs->ra.data[parent].last_child = child;
    }
  if (prev >= 0)
    mr_load->ptrs->ra.data[prev].next = child;
  mr_load->ptrs->ra.data[child].prev = prev;
  
  mr_load->ptrs->ra.data[child].parent = parent;
  mr_load->ptrs->ra.data[child].fd.name = name;
  mr_load->parent = child;
}

list: | nonempty_list

nonempty_list: scm | nonempty_list TOK_SCM_WS scm

ws: | TOK_SCM_WS

%%

MR_LOAD_FUNC (scm);
