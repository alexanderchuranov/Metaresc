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
%}

%name-prefix="rl_xml1_"
%pure-parser
%locations
 /* generate include-file with symbols and types */
%defines

 /* a more advanced semantic type */
%union {
  char * string;
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

%start xml

%% /* The grammar follows. */

xml: TOK_XML_DOC_OPEN_TAG doc_properties TOK_XML_DOC_CLOSE_TAG tag

doc_properties: | doc_properties TOK_XML_WS TOK_XML_ID TOK_XML_ASSIGN TOK_XML_PROP_VALUE {
  char * prop_name = $3;
  char * prop_value = $5;
  if (prop_name)
    RL_FREE (prop_name);
  if (prop_value)
    RL_FREE (prop_value);
 }

tag: start_tag TOK_XML_OPEN_TAG properties TOK_XML_CLOSE_EMPTY_TAG {
  rl_load_t * rl_load = RL_LOAD;
  char * tag_open_name = $2;
  char * tag_tail = $4;
  char * tail = tag_tail;
  int is_garbage = 0;

  if (tail)
    {
      while (isspace (*tail))
	++tail;
      is_garbage = *tail;
    }
  tail = tag_open_name ? strchr (tag_open_name, ':') : NULL;
  if (tail)
    {
      rl_load->ptrs->ra.data[rl_load->parent].fd.name = RL_STRDUP (tail + 1);
      RL_FREE (tag_open_name);
    }
  else
    rl_load->ptrs->ra.data[rl_load->parent].fd.name = tag_open_name;
  rl_load->ptrs->ra.data[rl_load->parent].value = RL_STRDUP ("");
  rl_load->parent = rl_load->ptrs->ra.data[rl_load->parent].parent;
  if (tag_tail)
    RL_FREE (tag_tail);
  if (is_garbage)
    {
      rl_xml1_error ("Unexpected charecters after closing tag!");
      YYERROR;
    }  
}
| start_tag TOK_XML_OPEN_TAG properties TOK_XML_CONTENT nested_tags TOK_XML_CLOSE_TAG TOK_XML_CONTENT {
  rl_load_t * rl_load = RL_LOAD;
  char * tag_open_name = $2;
  char * tag_content = $4;
  char * tag_close_name = $6;
  char * tag_tail = $7;
  int tags_mismatch = tag_open_name && tag_close_name ? strcmp (tag_open_name, tag_close_name) : 0;
  char * tail = tag_tail;
  int is_garbage = 0;

  if (tail)
    {
      while (isspace (*tail))
	++tail;
      is_garbage = *tail;
    }
  tail = tag_open_name ? strchr (tag_open_name, ':') : NULL;
  if (tail)
    {
      rl_load->ptrs->ra.data[rl_load->parent].fd.name = RL_STRDUP (tail + 1);
      RL_FREE (tag_open_name);
    }
  else
    rl_load->ptrs->ra.data[rl_load->parent].fd.name = tag_open_name;
  rl_load->ptrs->ra.data[rl_load->parent].value = tag_content;
  rl_load->parent = rl_load->ptrs->ra.data[rl_load->parent].parent;
  if (tag_close_name)
    RL_FREE (tag_close_name);
  if (tag_tail)
    RL_FREE (tag_tail);
  if (tags_mismatch)
    {
      rl_xml1_error ("Open and close tags names do not match!");
      YYERROR;
    }
  if (is_garbage)
    {
      rl_xml1_error ("Unexpected charecters after closing tag!");
      YYERROR;
    }
 }

start_tag: { RL_LOAD->parent = rl_parse_add_node (RL_LOAD); }

nested_tags: | nested_tags tag

properties: | properties TOK_XML_WS TOK_XML_ID TOK_XML_ASSIGN TOK_XML_PROP_VALUE {
  rl_load_t * rl_load = RL_LOAD;
  char * prop_name = $3;
  char * prop_value = $5;
  char * error = NULL;
  if (prop_name && prop_value)
    {
      if (0 == strcmp (prop_name, RL_REF_IDX))
	{
	  if (1 != sscanf (prop_value, "%" SCNd32, &rl_load->ptrs->ra.data[rl_load->parent].idx))
	    error = "Can't read " RL_REF_IDX " property.";
	}
      else if (0 == strcmp (prop_name, RL_REF))
	{
	  if (1 != sscanf (prop_value, "%" SCNd32, &rl_load->ptrs->ra.data[rl_load->parent].ref_idx))
	    error = "Can't read " RL_REF " property.";
	}
      else if (0 == strcmp (prop_name, RL_REF_CONTENT))
	{
	  if (1 != sscanf (prop_value, "%" SCNd32, &rl_load->ptrs->ra.data[rl_load->parent].ref_idx))
	    error = "Can't read " RL_REF " property.";
	  else
	    rl_load->ptrs->ra.data[rl_load->parent].flags |= RL_PDF_CONTENT_REFERENCE;
	}
      else if (0 == strcmp (prop_name, RL_RARRAY_SIZE))
	{
	  if (1 != sscanf (prop_value, "%" SCNd32, &rl_load->ptrs->ra.data[rl_load->parent].rarray_size))
	    error = "Can't read " RL_RARRAY_SIZE " property.";
	}
      else if (0 == strcmp (prop_name, RL_ISNULL))
	{
	  rl_load->ptrs->ra.data[rl_load->parent].flags |= RL_PDF_IS_NULL;
	}
    }
  else
    RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_OUT_OF_MEMORY);
  if (prop_name)
    RL_FREE (prop_name);
  if (prop_value)
    RL_FREE (prop_value);
  if (error)
    {
      rl_xml1_error (error);
      YYERROR;
    }
 }

%%

RL_LOAD_FUNC (xml1);
