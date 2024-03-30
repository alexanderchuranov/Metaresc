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
  static inline bool tail_is_not_blank (mr_substr_t * substr, int offset)
  {
    if (offset > substr->length)
      return (true);
    for ( ; offset < substr->length; ++offset)
      if (!isspace (substr->str[offset]))
	break;
    return (offset < substr->length);
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

  mr_load->ptrs->ra[mr_load->parent].fdp = mr_get_any_fd_by_name_substr (&$2, NULL);
  if (NULL == mr_load->ptrs->ra[mr_load->parent].fdp)
    {
      char name[$2.length + 1];
      memcpy (name, $2.str, $2.length);
      name[$2.length] = 0;
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNKNOWN_FIELD_NAME, name);
      YYERROR;
    }

  mr_load->ptrs->ra[mr_load->parent].load_params.mr_value.value_type = MR_VT_QUOTED_SUBSTR;
  mr_load->ptrs->ra[mr_load->parent].load_params.mr_value.vt_quoted_substr.substr.str = "";
  mr_load->ptrs->ra[mr_load->parent].load_params.mr_value.vt_quoted_substr.substr.length = 0;
  mr_load->ptrs->ra[mr_load->parent].load_params.mr_value.vt_quoted_substr.unquote = NULL;
  
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

  mr_load->ptrs->ra[mr_load->parent].fdp = mr_get_any_fd_by_name_substr (&$2, NULL);
  if (NULL == mr_load->ptrs->ra[mr_load->parent].fdp)
    {
      char name[$2.length + 1];
      memcpy (name, $2.str, $2.length);
      name[$2.length] = 0;
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNKNOWN_FIELD_NAME, name);
      YYERROR;
    }
  
  mr_load->ptrs->ra[mr_load->parent].load_params.mr_value.value_type = MR_VT_QUOTED_SUBSTR;
  mr_load->ptrs->ra[mr_load->parent].load_params.mr_value.vt_quoted_substr.substr.str = &mr_load->str[$4.str - mr_load->buf];
  mr_load->ptrs->ra[mr_load->parent].load_params.mr_value.vt_quoted_substr.substr.length = $4.length;
  mr_load->ptrs->ra[mr_load->parent].load_params.mr_value.vt_quoted_substr.unquote = xml_unquote_string;

  mr_load->parent = mr_load->ptrs->ra[mr_load->parent].parent;
 }

start_tag: { 
  mr_load_t * mr_load = MR_LOAD; 
  mr_load->parent = mr_parse_add_node (mr_load); 
  if (mr_load->parent < 0)
    {
      YYERROR;
    }
}

nested_tags: | nested_tags tag

properties: | properties TOK_XML_WS TOK_XML_ID TOK_XML_ASSIGN TOK_XML_PROP_VALUE {
  mr_load_t * mr_load = MR_LOAD;
  char * error = NULL;
  int offset = 0;

  if (0 == mr_substrcmp (MR_REF_IDX, &$3))
    {
      if ((1 != sscanf ($5.str, "%" SCNd32 "%n", &mr_load->ptrs->ra[mr_load->parent].idx, &offset)) || tail_is_not_blank (&$5, offset))
	error = MR_REF_IDX;
    }
  else if (0 == mr_substrcmp (MR_REF, &$3))
    {
      if ((1 != sscanf ($5.str, "%" SCNd32 "%n", &mr_load->ptrs->ra[mr_load->parent].ref_idx, &offset)) || tail_is_not_blank (&$5, offset))
	error = MR_REF;
    }
  else if (0 == mr_substrcmp (MR_REF_CONTENT, &$3))
    {
      if ((1 != sscanf ($5.str, "%" SCNd32 "%n", &mr_load->ptrs->ra[mr_load->parent].ref_idx, &offset)) || tail_is_not_blank (&$5, offset))
	error = MR_REF;
      else
	mr_load->ptrs->ra[mr_load->parent].flags.is_content_reference = true;
    }
  else if (0 == mr_substrcmp (MR_ISNULL, &$3))
    {
      mr_load->ptrs->ra[mr_load->parent].flags.is_null = true;
    }

  if (error)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_CANT_READ_PROPERTY, error);
      YYERROR;
    }
 }

%%

MR_LOAD_FUNC (xml1);
