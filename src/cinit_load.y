/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

%code top {
#include <stdio.h>
#include <stdbool.h>

#define MR_CINIT_DEBUG 0
/* Pass the argument to yyparse through to yylex. */
#define MR_CINIT_LTYPE mr_token_lloc_t
#define MR_LOAD (mr_cinit_get_extra (scanner))
#define mr_cinit_error MR_PARSE_ERROR

#include <metaresc.h>
#include <mr_ic.h>
#include <mr_stringify.h>
#include <lexer.h>
#include <mr_value.h>
#include <cinit_load.tab.h>
#define YYSTYPE MR_CINIT_STYPE
#define YYLTYPE MR_CINIT_LTYPE
#include <cinit_load.lex.h>

}

%code {

static void
cinit_unquote_str (mr_substr_t * substr, char * dst)
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
	  if (c > 0)
	    dst[length++] = c;
	  else if (1 == sscanf (&substr->str[i], "%o%n", &c, &size))
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

%define api.prefix {mr_cinit_}
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
    mr_idx_t ivalue;
  } id_ivalue;
}

/* Bison declarations.  */
%token <id_ivalue> TOK_CINIT_ID_IVALUE
%token <value> TOK_CINIT_NUMBER
%token <string> TOK_CINIT_FIELD_PREFIX TOK_CINIT_FIELD_CAST TOK_CINIT_STRING TOK_CINIT_CHAR TOK_CINIT_ID
%token TOK_CINIT_NULL TOK_CINIT_DOT TOK_CINIT_ASSIGN TOK_CINIT_LBRACE TOK_CINIT_RBRACE TOK_CINIT_LPAREN TOK_CINIT_RPAREN TOK_CINIT_COMMA TOK_CINIT_ERROR

%left TOK_CINIT_BIT_OR TOK_CINIT_BIT_AND TOK_CINIT_BIT_XOR
%left TOK_CINIT_PLUS TOK_CINIT_MINUS
%left TOK_CINIT_MUL TOK_CINIT_DIV TOK_CINIT_MOD
%precedence NEG 
%type <value> expr

%start cinit

%% /* The grammar follows.  */

cinit: start_node cinit_stmt {
  mr_load_t * mr_load = MR_LOAD;
  mr_load->parent = mr_load->ptrs->ra[mr_load->parent].parent;
  (void)mr_cinit_nerrs; /* workaround compiler warning variable: 'mr_cinit_nerrs' set but not used */
 }

start_node: { 
  mr_load_t * mr_load = MR_LOAD; 
  mr_idx_t idx = mr_add_ptr_to_list (mr_load->ptrs);
  if (MR_NULL_IDX == idx)
    { YYERROR; }
  mr_add_child (mr_load->ptrs, mr_load->parent, idx);
  mr_load->parent = idx;
}

cinit_stmt:
casted_value
| TOK_CINIT_ID_IVALUE cinit_stmt {
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

casted_value:
value
| TOK_CINIT_FIELD_CAST value {
  mr_load_t * mr_load = MR_LOAD;
  char * type = $1.str;
  type[$1.length] = 0;

  if (strcmp (type, "void") != 0)
    {
      mr_td_t * tdp = mr_get_td_by_name_internal (type);
      if (NULL == tdp)
	{
	  MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_NO_TYPE_DESCRIPTOR, type);
	  YYERROR;
	}
      else
	{
	  mr_load->ptrs->ra[mr_load->parent].flags |= MR_IS_TYPED;
	  mr_load->ptrs->ra[mr_load->parent].fdp = &tdp->mr_ptr_fd; /* for mr_ptr_t union we get name of union member from type cast */
	}
    }
 }

value:
compaund
| expr {
  mr_load_t * mr_load = MR_LOAD;
  mr_status_t status = mr_value_to_mr_ptrdes (&mr_load->ptrs->ra[mr_load->parent], &$1);
  if (MR_SUCCESS != status)
    { YYERROR; }
}
| TOK_CINIT_STRING {
  mr_load_t * mr_load = MR_LOAD;
  if (memchr ($1.str, '\\', $1.length))
    {
      char * buf = MR_CALLOC (1, $1.length + sizeof (char));
      if (NULL == buf)
	{
	  MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	  YYERROR;
	}
      cinit_unquote_str (&$1, buf);
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
| TOK_CINIT_NULL {
  mr_load_t * mr_load = MR_LOAD;
  mr_load->ptrs->ra[mr_load->parent].flags |= MR_IS_NULL;
  mr_load->ptrs->ra[mr_load->parent].value_type = MR_VT_INT;
  memset (&mr_load->ptrs->ra[mr_load->parent].load_params.vt_int, 0, sizeof (mr_load->ptrs->ra[mr_load->parent].load_params.vt_int));
  }
| TOK_CINIT_CHAR {
  mr_load_t * mr_load = MR_LOAD;
  $1.str[$1.length] = 0;
  if ($1.length > sizeof ("\\000"))
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_CHAR, $1.str);
      YYERROR;
    }

  char vt_char_str[$1.length + sizeof (char)];
  vt_char_str[0] = 0;
  cinit_unquote_str (&$1, vt_char_str);
  if (strlen (vt_char_str) > 1)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_CHAR, vt_char_str);
      YYERROR;
    }

  mr_load->ptrs->ra[mr_load->parent].load_params.vt_char = vt_char_str[0];
  mr_load->ptrs->ra[mr_load->parent].value_type = MR_VT_CHAR;
}

expr:
  expr TOK_CINIT_PLUS expr { mr_value_add (&$$, &$1, &$3); }
| expr TOK_CINIT_MINUS expr { mr_value_sub (&$$, &$1, &$3); }
| expr TOK_CINIT_MUL expr { mr_value_mul (&$$, &$1, &$3); }
| expr TOK_CINIT_DIV expr { mr_value_div (&$$, &$1, &$3); }
| expr TOK_CINIT_MOD expr { mr_value_mod (&$$, &$1, &$3); }
| expr TOK_CINIT_BIT_OR expr { mr_value_bit_or (&$$, &$1, &$3); }
| expr TOK_CINIT_BIT_AND expr { mr_value_bit_and (&$$, &$1, &$3); }
| expr TOK_CINIT_BIT_XOR expr { mr_value_bit_xor (&$$, &$1, &$3); }
| TOK_CINIT_MINUS expr %prec NEG { $$ = $2; mr_value_neg (&$$); }
| TOK_CINIT_PLUS expr %prec NEG { $$ = $2; }
| TOK_CINIT_NUMBER { $$ = $1; }
| TOK_CINIT_ID {
  mr_load_t * mr_load = MR_LOAD;
  $$.value_type = MR_VT_ID;
  $$.vt_substr.str = &mr_load->str[$1.str - mr_load->buf];
  $$.vt_substr.length = $1.length;
  }
| TOK_CINIT_LPAREN expr TOK_CINIT_RPAREN { $$ = $2; }

compaund: TOK_CINIT_LBRACE list TOK_CINIT_RBRACE

list: | list_element | list_element TOK_CINIT_COMMA list

list_element: cinit
| TOK_CINIT_DOT TOK_CINIT_ID TOK_CINIT_ASSIGN cinit {
  mr_load_t * mr_load = MR_LOAD;
  mr_idx_t idx = mr_last_child_for_parent (mr_load->ptrs, mr_load->parent);
  if (MR_NULL_IDX == idx)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_INTERNAL_PARSER_ERROR);
      YYERROR;
    }

  mr_td_t * tdp = mr_load->ptrs->ra[idx].fdp ? mr_load->ptrs->ra[idx].fdp->stype.tdp : NULL;
  char * type = tdp ? tdp->type.str : NULL;
  $2.str[$2.length] = 0;
  mr_load->ptrs->ra[idx].fdp = mr_get_any_fd_by_name ($2.str, type);

  if (NULL == mr_load->ptrs->ra[idx].fdp)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNKNOWN_FIELD_NAME, $2.str);
      YYERROR;
    }
}

%%

MR_LOAD_FUNC (cinit);
