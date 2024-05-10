/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

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

#if MR_SCM_DEBUG
  __attribute__ ((constructor)) void init_scm_debug (void) { mr_scm_debug = 1; }
#endif /* MR_SCM_DEBUG */
}

%code {
  static void
    scm_unquote_str (mr_substr_t * substr, char * dst)
  {
    typeof (substr->length) i, length;
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
	    int size;
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
%token TOK_SCM_FALSE TOK_SCM_WS TOK_SCM_LPARENTHESIS TOK_SCM_RPARENTHESIS TOK_SCM_LBRACKETS TOK_SCM_RBRACKETS TOK_SCM_DOT TOK_SCM_ERROR

%start ws_scm_ws

%% /* The grammar follows.  */

ws_scm_ws: ws scm ws

scm: start_node scm_stmt {
  mr_load_t * mr_load = MR_LOAD;
  mr_load->parent = mr_load->ptrs->ra[mr_load->parent].parent;
}

start_node: { 
  mr_load_t * mr_load = MR_LOAD; 
  mr_idx_t idx = mr_add_ptr_to_list (mr_load->ptrs);
  if (MR_NULL_IDX == idx)
    { YYERROR; }
  mr_add_child (mr_load->ptrs, mr_load->parent, idx);
  mr_load->parent = idx;
}

scm_stmt:
value
| TOK_SCM_ID_IVALUE scm_stmt {
  mr_load_t * mr_load = MR_LOAD;
  if ($1.id.str && $1.id.length)
    {
      if (0 == mr_substrcmp (MR_REF, &$1.id))
	{
	  mr_load->ptrs->ra[mr_load->parent].first_child = $1.ivalue;
	  mr_load->ptrs->ra[mr_load->parent].flags |= MR_IS_REFERENCE;
	}
      else if (0 == mr_substrcmp (MR_REF_CONTENT, &$1.id))
	{
	  mr_load->ptrs->ra[mr_load->parent].first_child = $1.ivalue;
	  mr_load->ptrs->ra[mr_load->parent].flags |= MR_IS_CONTENT_REFERENCE;
	}
      else if (0 == mr_substrcmp (MR_REF_IDX, &$1.id))
	{
	  mr_load->ptrs->ra[mr_load->parent].idx = $1.ivalue;
	  mr_load->ptrs->ra[mr_load->parent].flags |= MR_IS_REFERENCED;
	}
    }
}

value:
compaund
| named_node
| TOK_SCM_CHAR {
  mr_load_t * mr_load = MR_LOAD;
  mr_status_t status = mr_value_to_mr_ptrdes (&mr_load->ptrs->ra[mr_load->parent], &$1);
  if (MR_SUCCESS != status)
    { YYERROR; }
}
| TOK_SCM_NUMBER {
  mr_load_t * mr_load = MR_LOAD;
  mr_status_t status = mr_value_to_mr_ptrdes (&mr_load->ptrs->ra[mr_load->parent], &$1);
  if (MR_SUCCESS != status)
    { YYERROR; }
}
| TOK_SCM_STRING {
  mr_load_t * mr_load = MR_LOAD;
  if (memchr ($1.str, '\\', $1.length))
    {
      char * buf = MR_CALLOC (1, $1.length + sizeof (char));
      if (NULL == buf)
	{
	  MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	  YYERROR;
	}
      scm_unquote_str (&$1, buf);
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
| TOK_SCM_FALSE {
  mr_load_t * mr_load = MR_LOAD;
  mr_load->ptrs->ra[mr_load->parent].flags |= MR_IS_NULL;
  mr_load->ptrs->ra[mr_load->parent].value_type = MR_VT_INT;
  memset (&mr_load->ptrs->ra[mr_load->parent].load_params.vt_int, 0, sizeof (mr_load->ptrs->ra[mr_load->parent].load_params.vt_int));
  }

compaund: TOK_SCM_LPARENTHESIS list TOK_SCM_RPARENTHESIS

list: | scm ws list

named_node: TOK_SCM_LPARENTHESIS TOK_SCM_ID TOK_SCM_DOT value TOK_SCM_RPARENTHESIS {
  mr_load_t * mr_load = MR_LOAD;
  $2.str[$2.length] = 0;
  mr_load->ptrs->ra[mr_load->parent].fdp = mr_get_any_fd_by_name ($2.str, NULL);
  if (NULL == mr_load->ptrs->ra[mr_load->parent].fdp)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNKNOWN_FIELD_NAME, $2.str);
      YYERROR;
    }
}

ws: | TOK_SCM_WS

%%

MR_LOAD_FUNC (scm);
