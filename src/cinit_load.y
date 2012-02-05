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
#define MR_LOAD (mr_cinit_get_extra (YYPARSE_PARAM))
#define mr_cinit_error(ERROR) MR_PARSE_ERROR (ERROR, YYPARSE_PARAM, cinit)

%}

%name-prefix="mr_cinit_"
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
%token <id_ivalue> TOK_CINIT_ID_IVALUE
%token <string> TOK_CINIT_VALUE
%token <string> TOK_CINIT_FIELD_PREFIX
%token <string> TOK_CINIT_FIELD_CAST
%token TOK_CINIT_LBRACE
%token TOK_CINIT_RBRACE
%token TOK_CINIT_LBRACKET
%token TOK_CINIT_RBRACKET
%token TOK_CINIT_COMMA
%token TOK_CINIT_ERROR

%start cinit

%% /* The grammar follows.  */

cinit: start_node cinit_stmt { mr_load_t * mr_load = MR_LOAD; mr_load->parent = mr_load->ptrs->ra.data[mr_load->parent].parent; }

start_node: { mr_load_t * mr_load = MR_LOAD; mr_load->parent = mr_parse_add_node (mr_load); }

cinit_stmt:
value
| TOK_CINIT_ID_IVALUE cinit_stmt {
  mr_load_t * mr_load = MR_LOAD;
  if ($1.id.substr.data && $1.id.substr.size)
    {
      if (0 == mr_substrcmp (MR_REF, &$1.id))
	mr_load->ptrs->ra.data[mr_load->parent].ref_idx = $1.ivalue;
      else if (0 == mr_substrcmp (MR_REF_CONTENT, &$1.id))
	{
	  mr_load->ptrs->ra.data[mr_load->parent].ref_idx = $1.ivalue;
	  mr_load->ptrs->ra.data[mr_load->parent].flags |= MR_PDF_CONTENT_REFERENCE;
	}
      else if (0 == mr_substrcmp (MR_REF_IDX, &$1.id))
	mr_load->ptrs->ra.data[mr_load->parent].idx = $1.ivalue;
    }
}

value:
compaund
| TOK_CINIT_FIELD_CAST compaund { mr_load_t * mr_load = MR_LOAD; mr_load->ptrs->ra.data[mr_load->parent].fd.type = mr_unquote (&$1); }
| TOK_CINIT_VALUE { mr_load_t * mr_load = MR_LOAD; mr_load->ptrs->ra.data[mr_load->parent].value = mr_unquote (&$1); }

compaund: TOK_CINIT_LBRACE list TOK_CINIT_RBRACE 
| TOK_CINIT_LBRACKET list TOK_CINIT_RBRACKET

list: | nonempty_list | nonempty_list TOK_CINIT_COMMA

nonempty_list: list_element | nonempty_list TOK_CINIT_COMMA list_element

list_element: cinit
| TOK_CINIT_FIELD_PREFIX cinit { mr_load_t * mr_load = MR_LOAD; mr_load->ptrs->ra.data[mr_load->ptrs->ra.data[mr_load->parent].last_child].fd.name = mr_unquote (&$1); }

%%

MR_LOAD_FUNC (cinit);
