/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

%{
#include <stdio.h>

#include <reslib.h>
#include <lexer.h>
#include <cinit_load.tab.h>
#include <cinit_load.lex.h>

/* Pass the argument to yyparse through to yylex. */
#define YYPARSE_PARAM scanner
#define YYLEX_PARAM YYPARSE_PARAM
#define RL_LOAD (rl_cinit_get_extra (YYPARSE_PARAM))
#define rl_cinit_error(ERROR) RL_PARSE_ERROR (ERROR, YYPARSE_PARAM, cinit)
%}

%name-prefix="rl_cinit_"
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
%token <id_ivalue> TOK_CINIT_ID_IVALUE
%token <string> TOK_CINIT_VALUE
%token <string> TOK_CINIT_FIELD_PREFIX
%token <string> TOK_CINIT_FIELD_CAST
%token TOK_CINIT_LBRACE
%token TOK_CINIT_RBRACE
%token TOK_CINIT_LBRACKET
%token TOK_CINIT_RBRACKET
%token TOK_CINIT_COMMA

%start cinit

%% /* The grammar follows.  */

cinit: start_node cinit_stmt { rl_load_t * rl_load = RL_LOAD; rl_load->parent = rl_load->ptrs->ra.data[rl_load->parent].parent; }

start_node: { rl_load_t * rl_load = RL_LOAD; rl_load->parent = rl_parse_add_node (rl_load); }

cinit_stmt:
value
| TOK_CINIT_ID_IVALUE cinit_stmt {
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
| TOK_CINIT_FIELD_CAST compaund { rl_load_t * rl_load = RL_LOAD; rl_load->ptrs->ra.data[rl_load->parent].fd.type = $1; }
| TOK_CINIT_VALUE { rl_load_t * rl_load = RL_LOAD; rl_load->ptrs->ra.data[rl_load->parent].value = $1; }

compaund: TOK_CINIT_LBRACE list TOK_CINIT_RBRACE 
| TOK_CINIT_LBRACKET list TOK_CINIT_RBRACKET

list: | nonempty_list | nonempty_list TOK_CINIT_COMMA

nonempty_list: list_element | nonempty_list TOK_CINIT_COMMA list_element

list_element: cinit
| TOK_CINIT_FIELD_PREFIX cinit { rl_load_t * rl_load = RL_LOAD; rl_load->ptrs->ra.data[rl_load->ptrs->ra.data[rl_load->parent].last_child].fd.name = $1; }

%%

RL_LOAD_FUNC (cinit);
