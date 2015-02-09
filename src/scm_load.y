/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

%code top {
#include <stdio.h>

/* Pass the argument to yyparse through to yylex. */
#define MR_SCM_LTYPE mr_token_lloc_t
#define MR_LOAD (mr_scm_get_extra (scanner))
#define mr_scm_error MR_PARSE_ERROR

#include <metaresc.h>
#include <lexer.h>
#include <mr_value.h>
#include <scm_load.tab.h>
#define YYSTYPE MR_SCM_STYPE
#define YYLTYPE MR_SCM_LTYPE
#include <scm_load.lex.h>
#undef YYSTYPE
#undef YYLTYPE
}

%code {
  char * unquote_str (mr_substr_t * substr)
  {
    int length_ = 0;
    char * str_;
    int i;
#define ESC_CHAR_MAP_SIZE (256)
    static int map[ESC_CHAR_MAP_SIZE] = {
      [0 ... ESC_CHAR_MAP_SIZE - 1] = -1,
      [(unsigned char)'f'] = (unsigned char)'\f',
      [(unsigned char)'n'] = (unsigned char)'\n',
      [(unsigned char)'r'] = (unsigned char)'\r',
      [(unsigned char)'t'] = (unsigned char)'\t',
      [(unsigned char)'v'] = (unsigned char)'\v',
      [(unsigned char)'\''] = (unsigned char)'\'',
      [(unsigned char)'\"'] = (unsigned char)'\"',
      [(unsigned char)'\\'] = (unsigned char)'\\',
    };

    for (i = 1; i < substr->length - 1; ++i) /* we need to skip quotes at the begging and end */
      {
	if ('\\' == substr->str[i])
	  {
	    ++i;
	    if (('x' == substr->str[i]) && isalnum (substr->str[i + 1])
		&& isalnum (substr->str[i + 2]) && (';' == substr->str[i + 3]))
	      i += 3;
	  }
	++length_;
      }
    str_ = MR_MALLOC (length_ + 1);
    if (NULL == str_)
      {
	MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	return (NULL);
      }
    length_ = 0;
    for (i = 1; i < substr->length - 1; ++i)
      {
	if ('\\' == substr->str[i])
	  {
	    int c = map[(unsigned char)substr->str[++i]];
	    if (c > 0)
	      str_[length_++] = c;
	    else if (1 == sscanf (&substr->str[i], "x%x;", &c))
	      {
		i += 3;
		str_[length_++] = c;
	      }
	    else
	      str_[length_++] = substr->str[i];
	  }
	else
	  str_[length_++] = substr->str[i];
      }
    str_[length_] = 0;
    return (str_);
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
%token TOK_SCM_WS TOK_SCM_LPARENTHESIS TOK_SCM_RPARENTHESIS TOK_SCM_DOT TOK_SCM_HASH TOK_SCM_ERROR TOK_SCM_PLUS TOK_SCM_MINUS TOK_SCM_MUL TOK_SCM_DIV TOK_SCM_MOD TOK_SCM_BIT_OR TOK_SCM_BIT_AND TOK_SCM_BIT_XOR

%type <value> expr plus_list minus_list mul_list div_list bit_or_list bit_and_list bit_xor_list

%start ws_scm_ws

%% /* The grammar follows.  */

ws_scm_ws: ws scm ws

scm: start_node scm_stmt { mr_load_t * mr_load = MR_LOAD; mr_load->parent = mr_load->ptrs->ra[mr_load->parent].parent; }

start_node: { mr_load_t * mr_load = MR_LOAD; mr_load->parent = mr_parse_add_node (mr_load); }

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
	  mr_load->ptrs->ra[mr_load->parent].flags.is_content_reference = TRUE;
	}
      else if (0 == mr_substrcmp (MR_REF_IDX, &$1.id))
	mr_load->ptrs->ra[mr_load->parent].idx = $1.ivalue;
    }
}

value:
compaund
| named_node
| TOK_SCM_HASH compaund
| TOK_SCM_CHAR { mr_load_t * mr_load = MR_LOAD; mr_load->ptrs->ra[mr_load->parent].mr_value = $1; }
| expr { mr_load_t * mr_load = MR_LOAD; mr_load->ptrs->ra[mr_load->parent].mr_value = $1; }
| TOK_SCM_STRING {
  mr_load_t * mr_load = MR_LOAD;
  mr_load->ptrs->ra[mr_load->parent].mr_value.vt_string = unquote_str (&$1);
  mr_load->ptrs->ra[mr_load->parent].mr_value.value_type = MR_VT_STRING;
}

expr:
TOK_SCM_NUMBER { $$ = $1; }
| TOK_SCM_ID {
  $$.vt_string = strndup ($1.str, $1.length);
  $$.value_type = MR_VT_ID;
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
  char * name = strndup ($4.str, $4.length);
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
  mr_load->ptrs->ra[child].fd.name.str = name;
  mr_load->parent = child;
}

list: | nonempty_list

nonempty_list: scm | nonempty_list TOK_SCM_WS scm

ws: | TOK_SCM_WS

%%

MR_LOAD_FUNC (scm);
