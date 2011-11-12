/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

%{
#include <stdio.h>

#include <reslib.h>
#include <lexer.h>
#include <scm_load.tab.h>
#include <scm_load.lex.h>

/* Pass the argument to yyparse through to yylex. */
#define YYPARSE_PARAM scanner
#define YYLEX_PARAM YYPARSE_PARAM
#define RL_LOAD (rl_scm_get_extra (YYPARSE_PARAM))
#define rl_scm_error(ERROR) RL_PARSE_ERROR (ERROR, YYPARSE_PARAM, scm)
%}

%name-prefix="rl_scm_"
%pure-parser
%locations
 /* generate include-file with symbols and types */
%defines

 /* a more advanced semantic type */
%union {
  char * string;
  struct {
    char * id;
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

%start ws_scm_ws

%% /* The grammar follows.  */

ws_scm_ws: ws scm ws

scm: start_node scm_stmt { rl_load_t * rl_load = RL_LOAD; rl_load->parent = rl_load->ptrs->ra.data[rl_load->parent].parent; }

start_node: { rl_load_t * rl_load = RL_LOAD; rl_load->parent = rl_parse_add_node (rl_load); }

scm_stmt:
value
| TOK_SCM_ID_IVALUE scm_stmt {
  rl_load_t * rl_load = RL_LOAD;
  if ($1.id)
    {
      if (0 == strcmp (RL_REF, $1.id))
	rl_load->ptrs->ra.data[rl_load->parent].ref_idx = $1.ivalue;
      else if (0 == strcmp (RL_REF_CONTENT, $1.id))
	{
	  rl_load->ptrs->ra.data[rl_load->parent].ref_idx = $1.ivalue;
	  rl_load->ptrs->ra.data[rl_load->parent].flags |= RL_PDF_CONTENT_REFERENCE;
	}
      else if (0 == strcmp (RL_REF_IDX, $1.id))
	rl_load->ptrs->ra.data[rl_load->parent].idx = $1.ivalue;
      else if (0 == strcmp (RL_RARRAY_SIZE, $1.id))
	rl_load->ptrs->ra.data[rl_load->parent].rarray_size = $1.ivalue;
      RL_FREE ($1.id);
    }
}

value:
compaund
| named_node 
| TOK_SCM_HASH compaund 
| TOK_SCM_VALUE { rl_load_t * rl_load = RL_LOAD; rl_load->ptrs->ra.data[rl_load->parent].value = $1; }
| TOK_SCM_ID { rl_load_t * rl_load = RL_LOAD; rl_load->ptrs->ra.data[rl_load->parent].value = $1; }

compaund: TOK_SCM_LPARENTHESIS list TOK_SCM_RPARENTHESIS

named_node: TOK_SCM_LPARENTHESIS scm TOK_SCM_DOT TOK_SCM_ID TOK_SCM_RPARENTHESIS {
  rl_load_t * rl_load = RL_LOAD;
  int self = rl_load->parent;
  int parent = rl_load->ptrs->ra.data[self].parent;
  int prev = rl_load->ptrs->ra.data[self].prev;
  int child = rl_load->ptrs->ra.data[self].first_child;
  char * name = $4;
  if (parent >= 0)
    {
      if (rl_load->ptrs->ra.data[parent].first_child == self)
	rl_load->ptrs->ra.data[parent].first_child = child;
      if (rl_load->ptrs->ra.data[parent].last_child == self)
	rl_load->ptrs->ra.data[parent].last_child = child;
    }
  if (prev >= 0)
    rl_load->ptrs->ra.data[prev].next = child;
  rl_load->ptrs->ra.data[child].prev = prev;
  
  rl_load->ptrs->ra.data[child].parent = parent;
  rl_load->ptrs->ra.data[child].fd.name = name;
  rl_load->parent = child;
}

list: | nonempty_list

nonempty_list: scm | nonempty_list TOK_SCM_WS scm

ws: | TOK_SCM_WS

%%

RL_LOAD_FUNC (scm);
