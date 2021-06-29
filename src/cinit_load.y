/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

%code top {
#include <stdio.h>
#include <stdbool.h>

#define MR_CINIT_DEBUG 0
/* Pass the argument to yyparse through to yylex. */
#define MR_CINIT_LTYPE mr_token_lloc_t
#define MR_LOAD (mr_cinit_get_extra (scanner))
#define mr_cinit_error MR_PARSE_ERROR

#include <metaresc.h>
#include <mr_stringify.h>
#include <lexer.h>
#include <mr_value.h>
#include <cinit_load.tab.h>
#define YYSTYPE MR_CINIT_STYPE
#define YYLTYPE MR_CINIT_LTYPE
#include <cinit_load.lex.h>

}

%code {

void
cinit_unquote_str (mr_substr_t * substr, char * dst)
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
    int ivalue;
  } id_ivalue;
}

/* Bison declarations.  */
%token <id_ivalue> TOK_CINIT_ID_IVALUE
%token <value> TOK_CINIT_NUMBER
%token <string> TOK_CINIT_FIELD_PREFIX TOK_CINIT_FIELD_CAST TOK_CINIT_STRING TOK_CINIT_CHAR TOK_CINIT_ID
%token TOK_CINIT_NULL TOK_CINIT_LBRACE TOK_CINIT_RBRACE TOK_CINIT_LPAREN TOK_CINIT_RPAREN TOK_CINIT_COMMA TOK_CINIT_ERROR

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
 }

start_node: { 
  mr_load_t * mr_load = MR_LOAD; 
  mr_load->parent = mr_parse_add_node (mr_load); 
  if (mr_load->parent < 0)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      YYERROR;
    }
}

cinit_stmt:
casted_value
| TOK_CINIT_ID_IVALUE cinit_stmt {
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

casted_value:
value
| TOK_CINIT_FIELD_CAST value {
  mr_load_t * mr_load = MR_LOAD;
  char type[$1.length + 1];
  memcpy (type, $1.str, $1.length);
  type[$1.length] = 0;
  mr_td_t * tdp = mr_get_td_by_name (type);
  if (tdp != NULL)
    {
      mr_load->ptrs->ra[mr_load->parent].type = tdp->type.str;
      /* for mr_ptr_t union we get name of union member from type cast */
      mr_load->ptrs->ra[mr_load->parent].name = tdp->type.str;
    }
 }

value:
compaund
| expr { mr_load_t * mr_load = MR_LOAD; mr_load->ptrs->ra[mr_load->parent].load_params.mr_value = $1; }
| TOK_CINIT_STRING {
  mr_load_t * mr_load = MR_LOAD;
  mr_load->ptrs->ra[mr_load->parent].load_params.mr_value.value_type = MR_VT_QUOTED_SUBSTR;
  mr_load->ptrs->ra[mr_load->parent].load_params.mr_value.vt_quoted_substr.substr.str = &mr_load->str[$1.str - mr_load->buf];
  mr_load->ptrs->ra[mr_load->parent].load_params.mr_value.vt_quoted_substr.substr.length = $1.length;
  mr_load->ptrs->ra[mr_load->parent].load_params.mr_value.vt_quoted_substr.unquote = cinit_unquote_str;
  }
| TOK_CINIT_NULL {
  mr_load_t * mr_load = MR_LOAD;
  mr_load->ptrs->ra[mr_load->parent].flags.is_null = true;
  mr_load->ptrs->ra[mr_load->parent].load_params.mr_value.value_type = MR_VT_INT;
  mr_load->ptrs->ra[mr_load->parent].load_params.mr_value.vt_int = 0;
  }
| TOK_CINIT_CHAR {
  mr_load_t * mr_load = MR_LOAD;
  char vt_char_str[$1.length + sizeof (char)];

  vt_char_str[0] = 0;
  cinit_unquote_str (&$1, vt_char_str);

  if (strlen (vt_char_str) > 1)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_CHAR, vt_char_str);
      YYERROR;
    }
  
  mr_load->ptrs->ra[mr_load->parent].load_params.mr_value.vt_char = vt_char_str[0];
  mr_load->ptrs->ra[mr_load->parent].load_params.mr_value.value_type = MR_VT_CHAR;
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
  $$.vt_quoted_substr.substr.str = &mr_load->str[$1.str - mr_load->buf];
  $$.vt_quoted_substr.substr.length = $1.length;
  $$.vt_quoted_substr.unquote = NULL;
  }
| TOK_CINIT_LPAREN expr TOK_CINIT_RPAREN { $$ = $2; }

compaund: TOK_CINIT_LBRACE list TOK_CINIT_RBRACE

list: | list_element | list_element TOK_CINIT_COMMA list

list_element: cinit
| TOK_CINIT_FIELD_PREFIX cinit {
  mr_load_t * mr_load = MR_LOAD;
  int idx = mr_load->ptrs->ra[mr_load->parent].last_child;
  mr_load->ptrs->ra[idx].name = mr_get_static_field_name_from_substring (&$1);
}

%%

MR_LOAD_FUNC (cinit);
