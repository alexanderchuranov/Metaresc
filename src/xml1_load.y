/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

%{
#include <stdio.h>

/* Pass the argument to yyparse through to yylex. */
#define YYPARSE_PARAM scanner
#define YYLEX_PARAM YYPARSE_PARAM
#define MR_LOAD (mr_xml1_get_extra (YYPARSE_PARAM))
#define mr_xml1_error(ERROR) MR_PARSE_ERROR (ERROR, YYPARSE_PARAM, xml1)

#include <metaresc.h>
#include <lexer.h>
#include <xml1_load.tab.h>
#include <xml1_load.lex.h>

  static inline int tail_is_not_blank (mr_substr_t * substr, int offset)
  {
    if (offset > substr->length)
      return (!0);
    for ( ; offset < substr->length; ++offset)
      if (!isspace (substr->str[offset]))
	break;
    return (offset < substr->length);
  }
%}

%name-prefix="mr_xml1_"
%pure-parser
%locations
 /* generate include-file with symbols and types */
%defines

 /* a more advanced semantic type */
%union {
  mr_substr_t string;
}

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
      mr_xml1_error ("Unexpected charecters after closing tag!");
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
  mr_load->ptrs->ra.data[mr_load->parent].fd.name.str = strndup ($2.str, $2.length);
  mr_load->ptrs->ra.data[mr_load->parent].mr_value.value_type = MR_VT_UNKNOWN;
  mr_load->ptrs->ra.data[mr_load->parent].mr_value.vt_string.str = NULL;
  mr_load->ptrs->ra.data[mr_load->parent].mr_value.vt_string.length = 0;
  mr_load->parent = mr_load->ptrs->ra.data[mr_load->parent].parent;
}
| start_tag TOK_XML_OPEN_TAG properties TOK_XML_CONTENT nested_tags TOK_XML_CLOSE_TAG TOK_XML_CONTENT {
  mr_load_t * mr_load = MR_LOAD;
  int i;

  if (($2.length != $6.length) || (0 != strncmp ($2.str, $6.str, $2.length)))
    {
      mr_xml1_error ("Open and close tags names do not match!");
      YYERROR;
    }

  for (i = 0; i < $7.length; ++i)
    if (!isspace ($7.str[i]))
      break;
  if (i < $7.length)
    {
      mr_xml1_error ("Unexpected charecters after closing tag!");
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
  mr_load->ptrs->ra.data[mr_load->parent].fd.name.str = strndup ($2.str, $2.length);
  mr_load->ptrs->ra.data[mr_load->parent].mr_value.value_type = MR_VT_UNKNOWN;
  mr_load->ptrs->ra.data[mr_load->parent].mr_value.vt_string = $4;
  mr_load->parent = mr_load->ptrs->ra.data[mr_load->parent].parent;
 }

start_tag: { MR_LOAD->parent = mr_parse_add_node (MR_LOAD); }

nested_tags: | nested_tags tag

properties: | properties TOK_XML_WS TOK_XML_ID TOK_XML_ASSIGN TOK_XML_PROP_VALUE {
  mr_load_t * mr_load = MR_LOAD;
  char * error = NULL;
  int offset = 0;

  if (0 == mr_substrcmp (MR_REF_IDX, &$3))
    {
      if ((1 != sscanf ($5.str, "%" SCNd32 "%n", &mr_load->ptrs->ra.data[mr_load->parent].idx, &offset)) || tail_is_not_blank (&$5, offset))
	error = "Can't read " MR_REF_IDX " property.";
    }
  else if (0 == mr_substrcmp (MR_REF, &$3))
    {
      if ((1 != sscanf ($5.str, "%" SCNd32 "%n", &mr_load->ptrs->ra.data[mr_load->parent].ref_idx, &offset)) || tail_is_not_blank (&$5, offset))
	error = "Can't read " MR_REF " property.";
    }
  else if (0 == mr_substrcmp (MR_REF_CONTENT, &$3))
    {
      if ((1 != sscanf ($5.str, "%" SCNd32 "%n", &mr_load->ptrs->ra.data[mr_load->parent].ref_idx, &offset)) || tail_is_not_blank (&$5, offset))
	error = "Can't read " MR_REF " property.";
      else
	mr_load->ptrs->ra.data[mr_load->parent].flags.is_content_reference = MR_TRUE;
    }
  else if (0 == mr_substrcmp (MR_ISNULL, &$3))
    {
      mr_load->ptrs->ra.data[mr_load->parent].flags.is_null = MR_TRUE;
    }

  if (error)
    {
      mr_xml1_error (error);
      YYERROR;
    }
 }

%%

MR_LOAD_FUNC (xml1);
