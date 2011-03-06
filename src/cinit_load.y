/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

%{
#include <stdio.h>

#define RL_MODE PROTO /* explicitly set type of inclusion */
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
  int ivalue;
  char * string;
}

/* Bison declarations.  */
%token <string> TOK_CINIT_IVALUE
%token <string> TOK_CINIT_DVALUE
%token <string> TOK_CINIT_ID
%token <string> TOK_CINIT_BITMASK
%token <string> TOK_CINIT_STRING
%token <string> TOK_CINIT_CHAR
%token <ivalue> TOK_CINIT_RL_REF
%token <ivalue> TOK_CINIT_RL_REF_IDX
%token <string> TOK_CINIT_FIELD_PREFIX
%token <string> TOK_CINIT_FIELD_CAST
%token TOK_CINIT_NULL
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
| TOK_CINIT_RL_REF cinit_stmt { rl_load_t * rl_load = RL_LOAD; rl_load->ptrs->ra.data[rl_load->parent].ref_idx = $1; }
| TOK_CINIT_RL_REF_IDX cinit_stmt { rl_load_t * rl_load = RL_LOAD; rl_load->ptrs->ra.data[rl_load->parent].idx = $1; }

value:
compaund
| TOK_CINIT_FIELD_CAST compaund { rl_load_t * rl_load = RL_LOAD; rl_load->ptrs->ra.data[rl_load->parent].fd.type = $1; }
| TOK_CINIT_IVALUE { rl_load_t * rl_load = RL_LOAD; rl_load->ptrs->ra.data[rl_load->parent].value = $1; }
| TOK_CINIT_DVALUE { rl_load_t * rl_load = RL_LOAD; rl_load->ptrs->ra.data[rl_load->parent].value = $1; }
| TOK_CINIT_STRING { rl_load_t * rl_load = RL_LOAD; rl_load->ptrs->ra.data[rl_load->parent].value = $1; }
| TOK_CINIT_CHAR { rl_load_t * rl_load = RL_LOAD; rl_load->ptrs->ra.data[rl_load->parent].value = $1; }
| TOK_CINIT_NULL { rl_load_t * rl_load = RL_LOAD; rl_load->ptrs->ra.data[rl_load->parent].value = NULL; }
| TOK_CINIT_BITMASK { rl_load_t * rl_load = RL_LOAD; rl_load->ptrs->ra.data[rl_load->parent].value = $1; } 
| TOK_CINIT_ID { rl_load_t * rl_load = RL_LOAD; rl_load->ptrs->ra.data[rl_load->parent].value = $1; }

compaund: TOK_CINIT_LBRACE list TOK_CINIT_RBRACE 
| TOK_CINIT_LBRACKET list TOK_CINIT_RBRACKET

list: | nonempty_list | nonempty_list TOK_CINIT_COMMA

nonempty_list: list_element | nonempty_list TOK_CINIT_COMMA list_element

list_element: cinit
| TOK_CINIT_FIELD_PREFIX cinit { rl_load_t * rl_load = RL_LOAD; rl_load->ptrs->ra.data[rl_load->ptrs->ra.data[rl_load->parent].last_child].fd.name = $1; }

%%

RL_LOAD_FUNC (cinit);
