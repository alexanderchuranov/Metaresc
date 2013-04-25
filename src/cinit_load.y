/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

%{
#include <stdio.h>
#include <stdbool.h>

/* Pass the argument to yyparse through to yylex. */
#define YYPARSE_PARAM scanner
#define YYLEX_PARAM YYPARSE_PARAM
#define MR_LOAD (mr_cinit_get_extra (YYPARSE_PARAM))
#define mr_cinit_error(ERROR) MR_PARSE_ERROR (ERROR, YYPARSE_PARAM, cinit)

#include <metaresc.h>
#include <lexer.h>
#include <mr_value.h>
#include <cinit_load.tab.h>
#include <cinit_load.lex.h>

static char *
unquote_str (mr_substr_t * substr)
{
  int length_ = 0;
  char * str_;
  int i;
  static int map[1 << CHAR_BIT] = {
    [0 ... (1 << CHAR_BIT) - 1] = -1,
    [(unsigned char)'f'] = (unsigned char)'\f',
    [(unsigned char)'n'] = (unsigned char)'\n',
    [(unsigned char)'r'] = (unsigned char)'\r',
    [(unsigned char)'t'] = (unsigned char)'\t',
    [(unsigned char)'v'] = (unsigned char)'\v',
    [(unsigned char)'\''] = (unsigned char)'\'',
    [(unsigned char)'\"'] = (unsigned char)'\"',
    [(unsigned char)'\\'] = (unsigned char)'\\',
  };

  if (NULL == substr->str)
    return (NULL);

  for (i = 1; i < substr->length - 1;)
    {
      if ('\\' == substr->str[i++])
	{
	  int c = map[(unsigned char)substr->str[i]];
	  if (c > 0)
	    ++i;
	  else
	    {
	      int count = 3;
	      while ((substr->str[i] >= '0') && (substr->str[i] < '8') && (--count >= 0))
		++i;
	    }
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
	  else if (1 == sscanf (&substr->str[i], "%o", &c))
	    {
	      int count = 3;
	      while ((substr->str[i] >= '0') && (substr->str[i] < '8') && (--count >= 0))
		++i;
	      --i;
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

%}

%name-prefix="mr_cinit_"
%pure-parser
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
%token TOK_CINIT_LBRACE TOK_CINIT_RBRACE TOK_CINIT_LPAREN TOK_CINIT_RPAREN TOK_CINIT_LBRACKET TOK_CINIT_RBRACKET TOK_CINIT_COMMA TOK_CINIT_ERROR

%left TOK_CINIT_BIT_OR TOK_CINIT_BIT_AND TOK_CINIT_BIT_XOR
%left TOK_CINIT_PLUS TOK_CINIT_MINUS
%left TOK_CINIT_MUL TOK_CINIT_DIV TOK_CINIT_MOD
%left NEG
%type <value> expr

%start cinit

%% /* The grammar follows.  */

cinit: start_node cinit_stmt { mr_load_t * mr_load = MR_LOAD; mr_load->parent = mr_load->ptrs->ra.data[mr_load->parent].parent; }

start_node: { mr_load_t * mr_load = MR_LOAD; mr_load->parent = mr_parse_add_node (mr_load); }

cinit_stmt:
casted_value
| TOK_CINIT_ID_IVALUE cinit_stmt {
  mr_load_t * mr_load = MR_LOAD;
  if ($1.id.str && $1.id.length)
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

casted_value:
value
| TOK_CINIT_FIELD_CAST value {
  mr_load_t * mr_load = MR_LOAD;
  mr_load->ptrs->ra.data[mr_load->parent].fd.type = strndup ($1.str, $1.length);
}

value:
compaund
| expr { mr_load_t * mr_load = MR_LOAD; mr_load->ptrs->ra.data[mr_load->parent].mr_value = $1; }
| TOK_CINIT_STRING {
  mr_load_t * mr_load = MR_LOAD;
  mr_load->ptrs->ra.data[mr_load->parent].mr_value.vt_string = unquote_str (&$1);
  mr_load->ptrs->ra.data[mr_load->parent].mr_value.value_type = MR_VT_STRING;
  }
| TOK_CINIT_CHAR {
  mr_load_t * mr_load = MR_LOAD;
  char * vt_char_str = unquote_str (&$1);
  char vt_char = 0;
  if (vt_char_str)
    {
      int vt_char_str_length = strlen (vt_char_str);
      vt_char = vt_char_str[0];
      MR_FREE (vt_char_str);
      if (vt_char_str_length > 1)
	yyerror ("unexpected characters at char definition");
    }
  mr_load->ptrs->ra.data[mr_load->parent].mr_value.vt_char = vt_char;
  mr_load->ptrs->ra.data[mr_load->parent].mr_value.value_type = MR_VT_CHAR;
  }

expr:
TOK_CINIT_NUMBER { $$ = $1; }
| TOK_CINIT_ID {
  $$.vt_string = strndup ($1.str, $1.length);
  $$.value_type = MR_VT_ID;
  }
| expr TOK_CINIT_PLUS expr { mr_value_add (&$$, &$1, &$3); }
| expr TOK_CINIT_MINUS expr { mr_value_sub (&$$, &$1, &$3); }
| expr TOK_CINIT_MUL expr { mr_value_mul (&$$, &$1, &$3); }
| expr TOK_CINIT_DIV expr { mr_value_div (&$$, &$1, &$3); }
| expr TOK_CINIT_MOD expr { mr_value_mod (&$$, &$1, &$3); }
| expr TOK_CINIT_BIT_OR expr { mr_value_bit_or (&$$, &$1, &$3); }
| expr TOK_CINIT_BIT_AND expr { mr_value_bit_and (&$$, &$1, &$3); }
| expr TOK_CINIT_BIT_XOR expr { mr_value_bit_xor (&$$, &$1, &$3); }
| TOK_CINIT_MINUS expr %prec NEG { $$ = $2; mr_value_neg (&$$); }
| TOK_CINIT_LPAREN expr TOK_CINIT_RPAREN { $$ = $2; }

compaund:
TOK_CINIT_LBRACE list TOK_CINIT_RBRACE
| TOK_CINIT_LBRACKET list TOK_CINIT_RBRACKET

list: | nonempty_list | nonempty_list TOK_CINIT_COMMA

nonempty_list: list_element | nonempty_list TOK_CINIT_COMMA list_element

list_element: cinit
| TOK_CINIT_FIELD_PREFIX cinit { mr_load_t * mr_load = MR_LOAD; mr_load->ptrs->ra.data[mr_load->ptrs->ra.data[mr_load->parent].last_child].fd.name.str = strndup ($1.str, $1.length); }

%%

MR_LOAD_FUNC (cinit);
