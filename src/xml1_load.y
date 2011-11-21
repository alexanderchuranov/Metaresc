/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

%{
#include <stdio.h>

#include <reslib.h>
#include <lexer.h>
#include <xml1_load.tab.h>
#include <xml1_load.lex.h>

/* Pass the argument to yyparse through to yylex. */
#define YYPARSE_PARAM scanner
#define YYLEX_PARAM YYPARSE_PARAM
#define RL_LOAD (rl_xml1_get_extra (YYPARSE_PARAM))
#define rl_xml1_error(ERROR) RL_PARSE_ERROR (ERROR, YYPARSE_PARAM, xml1)
  
  static inline int tail_is_not_blank (rl_substr_t * substr, int offset)
  {
    if (offset > substr->substr.size)
      return (!0);
    for ( ; offset < substr->substr.size; ++offset)
      if (!isspace (substr->substr.data[offset]))
	break;
    return (offset < substr->substr.size);
  }
%}

%name-prefix="rl_xml1_"
%pure-parser
%locations
 /* generate include-file with symbols and types */
%defines

 /* a more advanced semantic type */
%union {
  rl_substr_t string;
}

/* Bison declarations.  */
%token <string> TOK_XML_OPEN_TAG
%token <string> TOK_XML_CLOSE_TAG
%token <string> TOK_XML_CLOSE_EMPTY_TAG
%token <string> TOK_XML_CONTENT
%token <string> TOK_XML_ID
%token <string> TOK_XML_PROP_VALUE
%token TOK_XML_DOC_OPEN_TAG
%token TOK_XML_DOC_CLOSE_TAG
%token TOK_XML_WS
%token TOK_XML_ASSIGN
%token TOK_XML_ERROR

%start xml

%% /* The grammar follows. */

xml: TOK_XML_DOC_OPEN_TAG doc_properties TOK_XML_DOC_CLOSE_TAG tag

doc_properties: | doc_properties TOK_XML_WS TOK_XML_ID TOK_XML_ASSIGN TOK_XML_PROP_VALUE 

tag: start_tag TOK_XML_OPEN_TAG properties TOK_XML_CLOSE_EMPTY_TAG {
  rl_load_t * rl_load = RL_LOAD;
  int i;
  
  for (i = 0; i < $4.substr.size; ++i)
    if (!isspace ($4.substr.data[i]))
      break;
  if (i < $4.substr.size)
    {
      rl_xml1_error ("Unexpected charecters after closing tag!");
      YYERROR;
    }  

  for (i = 0; i < $2.substr.size; ++i)
    if (':' == $2.substr.data[i])
      break;
  if (i < $2.substr.size)
    {
      $2.substr.size -= i + 1;
      $2.substr.data += i + 1;
    }
  rl_load->ptrs->ra.data[rl_load->parent].fd.name = rl_unquote (&$2);
  rl_load->ptrs->ra.data[rl_load->parent].value = RL_STRDUP ("");
  rl_load->parent = rl_load->ptrs->ra.data[rl_load->parent].parent;
}
| start_tag TOK_XML_OPEN_TAG properties TOK_XML_CONTENT nested_tags TOK_XML_CLOSE_TAG TOK_XML_CONTENT {
  rl_load_t * rl_load = RL_LOAD;
  int i;
  
  if (($2.substr.size != $6.substr.size) || (0 != strncmp ($2.substr.data, $6.substr.data, $2.substr.size)))
    {
      rl_xml1_error ("Open and close tags names do not match!");
      YYERROR;
    }

  for (i = 0; i < $7.substr.size; ++i)
    if (!isspace ($7.substr.data[i]))
      break;
  if (i < $7.substr.size)
    {
      rl_xml1_error ("Unexpected charecters after closing tag!");
      YYERROR;
    }  

  for (i = 0; i < $2.substr.size; ++i)
    if (':' == $2.substr.data[i])
      break;
  if (i < $2.substr.size)
    {
      $2.substr.size -= i + 1;
      $2.substr.data += i + 1;
    }
  rl_load->ptrs->ra.data[rl_load->parent].fd.name = rl_unquote (&$2);
  rl_load->ptrs->ra.data[rl_load->parent].value = rl_unquote (&$4);;
  rl_load->parent = rl_load->ptrs->ra.data[rl_load->parent].parent;
 }

start_tag: { RL_LOAD->parent = rl_parse_add_node (RL_LOAD); }

nested_tags: | nested_tags tag

properties: | properties TOK_XML_WS TOK_XML_ID TOK_XML_ASSIGN TOK_XML_PROP_VALUE {
  rl_load_t * rl_load = RL_LOAD;
  char * error = NULL;
  int offset = 0;
  
  if (0 == rl_substrcmp (RL_REF_IDX, &$3))
    {
      if ((1 != sscanf ($5.substr.data, "%" SCNd32 "%n", &rl_load->ptrs->ra.data[rl_load->parent].idx, &offset)) || tail_is_not_blank (&$5, offset))
	error = "Can't read " RL_REF_IDX " property.";
    }
  else if (0 == rl_substrcmp (RL_REF, &$3))
    {
      if ((1 != sscanf ($5.substr.data, "%" SCNd32 "%n", &rl_load->ptrs->ra.data[rl_load->parent].ref_idx, &offset)) || tail_is_not_blank (&$5, offset))
	error = "Can't read " RL_REF " property.";
    }
  else if (0 == rl_substrcmp (RL_REF_CONTENT, &$3))
    {
      if ((1 != sscanf ($5.substr.data, "%" SCNd32 "%n", &rl_load->ptrs->ra.data[rl_load->parent].ref_idx, &offset)) || tail_is_not_blank (&$5, offset))
	error = "Can't read " RL_REF " property.";
      else
	rl_load->ptrs->ra.data[rl_load->parent].flags |= RL_PDF_CONTENT_REFERENCE;
    }
  else if (0 == rl_substrcmp (RL_RARRAY_SIZE, &$3))
    {
      if ((1 != sscanf ($5.substr.data, "%" SCNd32 "%n", &rl_load->ptrs->ra.data[rl_load->parent].rarray_size, &offset)) || tail_is_not_blank (&$5, offset))
	error = "Can't read " RL_RARRAY_SIZE " property.";
    }
  else if (0 == rl_substrcmp (RL_ISNULL, &$3))
    {
      rl_load->ptrs->ra.data[rl_load->parent].flags |= RL_PDF_IS_NULL;
    }

  if (error)
    {
      rl_xml1_error (error);
      YYERROR;
    }
 }

%%

RL_LOAD_FUNC (xml1);
