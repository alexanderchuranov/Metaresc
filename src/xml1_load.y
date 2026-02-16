/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

%code top {
#include <stdio.h>

#define MR_XML1_DEBUG 0
/* Pass the argument to yyparse through to yylex. */
#define MR_XML1_LTYPE mr_token_lloc_t
#define MR_LOAD (mr_xml1_get_extra (scanner))
#define mr_xml1_error MR_PARSE_ERROR

#include <metaresc.h>
#include <lexer.h>
#include <xml1_load.tab.h>
#define YYSTYPE MR_XML1_STYPE
#define YYLTYPE MR_XML1_LTYPE
#include <xml1_load.lex.h>
}

%code {
  static inline mr_status_t load_idx (mr_substr_t * substr, mr_idx_t * idx, mr_ptrdes_flags_t * flags, mr_ptrdes_flags_t flag)
  {
    char * end = &substr->str[substr->length];
    char * tail = NULL;
    *idx = strtoull (substr->str, &tail, 0);
    if (NULL == tail)
      return (MR_FAILURE);
    while ((tail < end) && isspace (*tail))
      ++tail;
    if (tail < end)
      return (MR_FAILURE);
    *flags |= flag;
    return (MR_SUCCESS);
  }
}

 /* a more advanced semantic type */
%union {
  mr_substr_t string;
}

%define api.prefix {mr_xml1_}
%define api.pure full
%param {void * scanner}
%locations

 /* generate include-file with symbols and types */
%defines

/* Bison declarations.  */
%token <string> TOK_XML_OPEN_TAG TOK_XML_CLOSE_TAG TOK_XML_CLOSE_EMPTY_TAG TOK_XML_CONTENT TOK_XML_ID TOK_XML_PROP_VALUE
%token TOK_XML_DOC_OPEN_TAG TOK_XML_DOC_CLOSE_TAG TOK_XML_WS TOK_XML_ASSIGN TOK_XML_ERROR

%start xml

%% /* The grammar follows. */

xml: TOK_XML_DOC_OPEN_TAG doc_properties TOK_XML_DOC_CLOSE_TAG tag

doc_properties: | doc_properties TOK_XML_WS TOK_XML_ID TOK_XML_ASSIGN TOK_XML_PROP_VALUE

tag: start_tag TOK_XML_OPEN_TAG properties TOK_XML_CLOSE_EMPTY_TAG {
  mr_load_t * mr_load = MR_LOAD;
  int i;

  for (i = 0; i < $4.length; ++i)
    if (!isspace ($4.str[i]))
      break;
  if (i < $4.length)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_CHARS_AFTER_CLOSING_TAG);
      YYERROR;
    }

  for (i = 0; i < $2.length; ++i)
    if (':' == $2.str[i])
      break;
  if (i < $2.length)
    {
      $2.length -= i + 1;
      $2.str += i + 1;
    }

  $2.str[$2.length] = 0;
  mr_load->ptrs->ra[mr_load->parent].fdp = mr_get_any_fd_by_name ($2.str, NULL);
  if (NULL == mr_load->ptrs->ra[mr_load->parent].fdp)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNKNOWN_FIELD_NAME, $2.str);
      YYERROR;
    }

  mr_load->ptrs->ra[mr_load->parent].value_type = MR_VT_UNALLOCATED_STRING;
  mr_load->ptrs->ra[mr_load->parent].vt_string = "";
  
  mr_load->parent = mr_load->ptrs->ra[mr_load->parent].parent;
}
| start_tag TOK_XML_OPEN_TAG properties TOK_XML_CONTENT nested_tags TOK_XML_CLOSE_TAG TOK_XML_CONTENT {
  mr_load_t * mr_load = MR_LOAD;
  int i;

  if (($2.length != $6.length) || (0 != strncmp ($2.str, $6.str, $2.length)))
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_TAGS_DONT_MATCH);
      YYERROR;
    }

  for (i = 0; i < $7.length; ++i)
    if (!isspace ($7.str[i]))
      break;
  if (i < $7.length)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_CHARS_AFTER_CLOSING_TAG);
      YYERROR;
    }

  for (i = 0; i < $2.length; ++i)
    if (':' == $2.str[i])
      break;
  if (i < $2.length)
    {
      $2.length -= i + 1;
      $2.str += i + 1;
    }

  $2.str[$2.length] = 0;
  mr_load->ptrs->ra[mr_load->parent].fdp = mr_get_any_fd_by_name ($2.str, NULL);
  if (NULL == mr_load->ptrs->ra[mr_load->parent].fdp)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNKNOWN_FIELD_NAME, $2.str);
      YYERROR;
    }

  if (memchr ($4.str, '&', $4.length))
    {
      char * buf = MR_CALLOC (1, $4.length + sizeof (char));
      if (NULL == buf)
	{
	  MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	  YYERROR;
	}
      xml_unquote_string (&$4, buf);
      mr_load->ptrs->ra[mr_load->parent].value_type = MR_VT_STRING;
      mr_load->ptrs->ra[mr_load->parent].vt_string = buf;
    }
  else
    {
      mr_load->ptrs->ra[mr_load->parent].value_type = MR_VT_SUBSTR_POS;
      mr_load->ptrs->ra[mr_load->parent].vt_substr_pos.offset = $4.str - mr_load->buf;
      mr_load->ptrs->ra[mr_load->parent].vt_substr_pos.length = $4.length;
    }

  mr_load->parent = mr_load->ptrs->ra[mr_load->parent].parent;
 }

start_tag: { 
  mr_load_t * mr_load = MR_LOAD; 
  mr_idx_t idx = mr_add_ptr_to_list (mr_load->ptrs);
  if (MR_NULL_IDX == idx)
    { YYERROR; }
  mr_add_child (mr_load->ptrs, mr_load->parent, idx);
  mr_load->parent = idx;
  (void)mr_xml1_nerrs; /* workaround compiler warning: variable 'mr_xml1_nerrs' set but not used */
}

nested_tags: | nested_tags tag

properties: | properties TOK_XML_WS TOK_XML_ID TOK_XML_ASSIGN TOK_XML_PROP_VALUE {
  mr_load_t * mr_load = MR_LOAD;
  mr_status_t status = MR_SUCCESS;

  if (0 == mr_substrcmp (MR_REF_IDX, &$3))
    status = load_idx (&$5, &mr_load->ptrs->ra[mr_load->parent].idx, &mr_load->ptrs->ra[mr_load->parent].flags, MR_IS_REFERENCED);
  else if (0 == mr_substrcmp (MR_REF, &$3))
    status = load_idx (&$5, &mr_load->ptrs->ra[mr_load->parent].first_child, &mr_load->ptrs->ra[mr_load->parent].flags, MR_IS_REFERENCE);
  else if (0 == mr_substrcmp (MR_REF_CONTENT, &$3))
    status = load_idx (&$5, &mr_load->ptrs->ra[mr_load->parent].first_child, &mr_load->ptrs->ra[mr_load->parent].flags, MR_IS_CONTENT_REFERENCE);
  else if (0 == mr_substrcmp (MR_ISNULL, &$3))
    mr_load->ptrs->ra[mr_load->parent].flags |= MR_IS_NULL;

  if (status != MR_SUCCESS)
    {
      char property[1 << 6];
      int length = MR_MIN ($3.length, sizeof (property) - 1);
      memcpy (property, $3.str, length);
      property[length] = 0;
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_REF, property);
      YYERROR;
    }
 }

%%

MR_LOAD_FUNC (xml1);
