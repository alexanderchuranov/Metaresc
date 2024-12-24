/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

#include <stdio.h>
#ifndef __USE_BSD
#define __USE_BSD
#endif /* __USE_BSD */
#include <string.h> /* strncasecmp */
#include <stdarg.h>

#include <metaresc.h>
#include <mr_stringify.h>
#include <mr_save.h>

#define MR_XML1_DOCUMENT_HEADER "<?xml version=\"1.0\"?>"

#define MR_XML1_INDENT_SPACES (2)
#define MR_XML1_INDENT_TEMPLATE "\n%*s"

#define MR_XML1_ATTR_CHARP " %s=\"%s\""
#define MR_XML1_ATTR_INT " %s=\"%" SCNu32 "\""

#define MR_XML1_OPEN_TAG_START "<%s"
#define MR_XML1_OPEN_EMPTY_TAG_END "/>"

#define XML_QUOTE_CHAR_PATTERN "&#x%02X;"
#define ESC_SIZE (sizeof (XML_QUOTE_CHAR_PATTERN))
#define ESC_CHAR_MAP_SIZE (1 << __CHAR_BIT__)
static char * esc_char_map[MR_ESC_CHAR_MAP_SIZE] = {
  [(unsigned char)'&'] = "&amp;",
  [(unsigned char)'<'] = "&lt;",
  [(unsigned char)'>'] = "&gt;",
  [(unsigned char)'"'] = "&quot;",
};

TYPEDEF_STRUCT (mr_xml_esc_t,
		(char *, esc_seq, , "escape sequence"),
		(int, length, , "length of escape sequence"),
		(char, symbol, , "escaped symbol"),
		)

/**
 * XML unquote function. Replace XML special characters aliases on a source characters.
 * @param str input string
 * @param length length of the input string
 * @return XML unquoted string
 */
void
xml_unquote_string (mr_substr_t * substr, char * dst)
{
  typeof (substr->length) i, length = 0;
  static bool inited = false;
  static mr_xml_esc_t esc_seq_hash[MR_ESC_CHAR_MAP_SIZE];
  
  if (!inited)
    {
      memset (esc_seq_hash, 0, sizeof (esc_seq_hash));
      for (i = 0; i < sizeof (esc_seq_hash) / sizeof (esc_seq_hash[0]); ++i)
	if (esc_char_map[i])
	  {
	    int length = strlen (esc_char_map[i]);
	    unsigned char mapped = esc_char_map[i][1];
	    esc_seq_hash[mapped].esc_seq = esc_char_map[i];
	    esc_seq_hash[mapped].symbol = i;
	    esc_seq_hash[mapped].length = length;
	  }
      inited = true;
    }

  for (i = 0; i < substr->length; ++i)
    if (substr->str[i] != '&')
      dst[length++] = substr->str[i];
    else
      {
	char esc[ESC_SIZE + 1];
	strncpy (esc, &substr->str[i], sizeof (esc) - 1);
	char * semicolon = strchr (esc, ';');
	if (semicolon)
	  semicolon[1] = 0;
	else
	  esc[sizeof (esc) - 1] = 0;
	
	if ('#' == substr->str[i + 1])
	  {
	    int32_t code = 0;
	    int size = 0;
	    if (1 != sscanf (&substr->str[i], XML_QUOTE_CHAR_PATTERN "%n", &code, &size))
	      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_WRONG_XML_ESC, esc);
	    else
	      {
		i += size - 1; /* one more +1 in the loop */
		dst[length++] = code;
	      }
	  }
	else
	  {
	    bool found = false;
	    unsigned char hash = tolower (substr->str[i + 1]);
	    if (esc_seq_hash[hash].esc_seq)
	      if (0 == strncasecmp (&substr->str[i + 2],
				    &esc_seq_hash[hash].esc_seq[2],
				    esc_seq_hash[hash].length - 2))
		{
		  dst[length++] = esc_seq_hash[hash].symbol;
		  i += esc_seq_hash[hash].length - 1; /* one more increase in the loop */
		  found = true;
		}

	    if (!found)
	      {
		MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNKNOWN_XML_ESC, esc);
		dst[length++] = substr->str[i];
	      }
	  }
      }
  dst[length] = 0;
}

#define COMPLEX_REAL_IMAG_DELIMITER " + "

static int
xml_ra_printf_quote_char (mr_rarray_t * mr_ra_str, unsigned char c)
{
  char * mapped = esc_char_map[c];
  int count = 0;

  if (mapped)
    count += TRY_CATCH_THROW (mr_ra_append_string (mr_ra_str, mapped));
  else if (isprint (c))
    count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, c));
  else
    count += TRY_CATCH_THROW (mr_ra_printf (mr_ra_str, XML_QUOTE_CHAR_PATTERN, c));
  return (count);
}

static int
xml_ra_printf_char (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (xml_ra_printf_quote_char (mr_ra_str, *ptrdes->data.string));
}

static int
xml_ra_printf_quote_string (mr_rarray_t * mr_ra_str, char * str)
{
  int count = 0;
  while (*str)
    count += TRY_CATCH_THROW (xml_ra_printf_quote_char (mr_ra_str, *str++));
  return (count);
}

static int
xml_ra_printf_complex_float (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf_complex_float (mr_ra_str, ptrdes, COMPLEX_REAL_IMAG_DELIMITER));
}

static int
xml_ra_printf_complex_double (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf_complex_double (mr_ra_str, ptrdes, COMPLEX_REAL_IMAG_DELIMITER));
}

static int
xml_ra_printf_complex_long_double_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf_complex_long_double_t (mr_ra_str, ptrdes, COMPLEX_REAL_IMAG_DELIMITER));
}

#define MR_XML_BITMASK_DELIMITER " | "

static int
xml_ra_printf_bitmask (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf_bitmask (mr_ra_str, ptrdes, MR_XML_BITMASK_DELIMITER));
}

static int
xml_ra_printf_bitfield (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf_bitfield (mr_ra_str, ptrdes, MR_XML_BITMASK_DELIMITER));
}

static int
xml_ra_printf_char_array (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  char buffer[ptrdes->MR_SIZE + 1];
  strncpy (buffer, ptrdes->data.ptr, ptrdes->MR_SIZE);
  buffer[ptrdes->MR_SIZE] = 0;
  return (xml_ra_printf_quote_string (mr_ra_str, buffer));
}

static int
xml_ra_printf_string (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (xml_ra_printf_quote_string (mr_ra_str, *(char**)ptrdes->data.ptr));
}

static int
xml_ra_printf_func (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  const char * func_str = mr_serialize_func (*(void**)ptrdes->data.ptr);
  if (func_str)
    return (mr_ra_append_string (mr_ra_str, (char*)func_str));
  else
    return (mr_ra_printf (mr_ra_str, "0x%llx", (unsigned long long int)*(uintptr_t*)ptrdes->data.ptr));
}

/**
 * Init IO handlers Table
 */
static mr_ra_printf_t xml_ra_printf_tbl[MR_TYPE_LAST] =
  {
    [MR_TYPE_STRING] = xml_ra_printf_string,
    [MR_TYPE_NONE] = mr_ra_printf_void,
    [MR_TYPE_VOID] = mr_ra_printf_void,
    [MR_TYPE_ENUM] = xml_ra_printf_bitmask,
    [MR_TYPE_BITFIELD] = xml_ra_printf_bitfield,
    [MR_TYPE_BOOL] = mr_ra_printf_bool,
    [MR_TYPE_INT8] = mr_ra_printf_int8_t,
    [MR_TYPE_UINT8] = mr_ra_printf_uint8_t,
    [MR_TYPE_INT16] = mr_ra_printf_int16_t,
    [MR_TYPE_UINT16] = mr_ra_printf_uint16_t,
    [MR_TYPE_INT32] = mr_ra_printf_int32_t,
    [MR_TYPE_UINT32] = mr_ra_printf_uint32_t,
    [MR_TYPE_INT64] = mr_ra_printf_int64_t,
    [MR_TYPE_UINT64] = mr_ra_printf_uint64_t,
    [MR_TYPE_INT128] = mr_ra_printf_int128_t,
    [MR_TYPE_UINT128] = mr_ra_printf_uint128_t,
    [MR_TYPE_FLOAT] = mr_ra_printf_float,
    [MR_TYPE_COMPLEX_FLOAT] = xml_ra_printf_complex_float,
    [MR_TYPE_DOUBLE] = mr_ra_printf_double,
    [MR_TYPE_COMPLEX_DOUBLE] = xml_ra_printf_complex_double,
    [MR_TYPE_LONG_DOUBLE] = mr_ra_printf_long_double_t,
    [MR_TYPE_COMPLEX_LONG_DOUBLE] = xml_ra_printf_complex_long_double_t,
    [MR_TYPE_CHAR] = xml_ra_printf_char,
    [MR_TYPE_CHAR_ARRAY] = xml_ra_printf_char_array,
    [MR_TYPE_STRUCT] = mr_ra_printf_void,
    [MR_TYPE_FUNC] = xml_ra_printf_func,
    [MR_TYPE_FUNC_TYPE] = xml_ra_printf_func,
    [MR_TYPE_ARRAY] = mr_ra_printf_void,
    [MR_TYPE_POINTER] = mr_ra_printf_void,
    [MR_TYPE_UNION] = mr_ra_printf_void,
    [MR_TYPE_ANON_UNION] = mr_ra_printf_void,
    [MR_TYPE_NAMED_ANON_UNION] = mr_ra_printf_void,
  };

static mr_status_t
xml1_pre_print_node (mr_ra_ptrdes_t * ptrs, mr_idx_t idx, int level, mr_rarray_t * mr_ra_str)
{
  bool empty_tag = true;

  /* route saving handler */
  mr_ra_printf_t save_handler = NULL;
  if ((ptrs->ra[idx].mr_type >= 0) && (ptrs->ra[idx].mr_type < MR_TYPE_LAST))
    save_handler = xml_ra_printf_tbl[ptrs->ra[idx].mr_type];
  
  if (NULL == save_handler)
    {
      save_handler = mr_ra_printf_void;
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNSUPPORTED_NODE_TYPE, ptrs->ra[idx].mr_type);
    }

  memset (&ptrs->ra[idx].res, 0, sizeof (ptrs->ra[idx].res));

  char * name = ptrs->ra[idx].fdp ? ptrs->ra[idx].fdp->name.str : MR_DEFAULT_NODE_NAME;
  if (mr_ra_printf (mr_ra_str, MR_XML1_INDENT_TEMPLATE MR_XML1_OPEN_TAG_START,
		    MR_LIMIT_LEVEL (level) * MR_XML1_INDENT_SPACES, "", name) < 0)
    return (MR_FAILURE);
  
  if (ptrs->ra[idx].flags & MR_IS_REFERENCE)
    if (mr_ra_printf (mr_ra_str, MR_XML1_ATTR_INT, MR_REF, (uint32_t)ptrs->ra[ptrs->ra[idx].first_child].idx) < 0)
      return (MR_FAILURE);

  if (ptrs->ra[idx].flags & MR_IS_CONTENT_REFERENCE)
    if (mr_ra_printf (mr_ra_str, MR_XML1_ATTR_INT, MR_REF_CONTENT, (uint32_t)ptrs->ra[ptrs->ra[idx].first_child].idx) < 0)
      return (MR_FAILURE);
  
  if (ptrs->ra[idx].flags & MR_IS_REFERENCED)
    if (mr_ra_printf (mr_ra_str, MR_XML1_ATTR_INT, MR_REF_IDX, (uint32_t)ptrs->ra[idx].idx) < 0)
      return (MR_FAILURE);
  
  if (ptrs->ra[idx].flags & MR_IS_NULL)
    if (mr_ra_printf (mr_ra_str, MR_XML1_ATTR_CHARP, MR_ISNULL, MR_ISNULL_VALUE) < 0)
      return (MR_FAILURE);

  if (ptrs->ra[idx].flags & (MR_IS_NULL | MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE))
    empty_tag = true;
  else
    {
      if (mr_ra_append_char (mr_ra_str, '>') < 0)
	return (MR_FAILURE);
      int count = save_handler (mr_ra_str, &ptrs->ra[idx]);
      empty_tag = (ptrs->ra[idx].first_child == MR_NULL_IDX) && (0 == count);
      if (empty_tag)
	mr_ra_str->data.string[--mr_ra_str->MR_SIZE] = 0;
    }
      
  if (empty_tag)
    {
      if (mr_ra_append_string (mr_ra_str, MR_XML1_OPEN_EMPTY_TAG_END) < 0)
	return (MR_FAILURE);
    }

  ptrs->ra[idx].res.data.intptr = empty_tag;
  ptrs->ra[idx].res.type = "intptr";

  return (MR_SUCCESS);
}

static mr_status_t
xml1_post_print_node (mr_ra_ptrdes_t * ptrs, mr_idx_t idx, int level, mr_rarray_t * mr_ra_str)
{
  if ((ptrs->ra[idx].first_child != MR_NULL_IDX) && !(ptrs->ra[idx].flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE)))
    if (mr_ra_printf (mr_ra_str, MR_XML1_INDENT_TEMPLATE, MR_LIMIT_LEVEL (level) * MR_XML1_INDENT_SPACES, "") < 0)
      return (MR_FAILURE);

  if (!ptrs->ra[idx].res.data.intptr)
    {
      if (mr_ra_append_string (mr_ra_str, "</") < 0)
	return (MR_FAILURE);
      char * name = ptrs->ra[idx].fdp ? ptrs->ra[idx].fdp->name.str : MR_DEFAULT_NODE_NAME;
      if (mr_ra_append_string (mr_ra_str, name) < 0)
	return (MR_FAILURE);
      if (mr_ra_append_char (mr_ra_str, '>') < 0)
	return (MR_FAILURE);
    }

  return (MR_SUCCESS);
}

static mr_status_t
xml1_print_node (mr_ra_ptrdes_t * ptrs, mr_idx_t idx, int level, mr_dfs_order_t order, void * context)
{
  mr_rarray_t * mr_ra_str = context;

  switch (order)
    {
    case MR_DFS_PRE_ORDER:
      return (xml1_pre_print_node (ptrs, idx, level, mr_ra_str));
    case MR_DFS_POST_ORDER:
      return (xml1_post_print_node (ptrs, idx, level, mr_ra_str));
    default:
      return (MR_FAILURE);
    }
}

/**
 * Public function. Save scheduler. Save any object as a string.
 * @param ptrs resizeable array with pointers descriptors
 * @return stringified representation of object
 */
char *
mr_xml1_save (mr_ra_ptrdes_t * ptrs)
{
  mr_rarray_t mr_ra_str = {
    .data = { mr_strdup (MR_XML1_DOCUMENT_HEADER) },
    .MR_SIZE = sizeof (MR_XML1_DOCUMENT_HEADER),
    .type = "string",
    .alloc_size = sizeof (MR_XML1_DOCUMENT_HEADER),
  };

  if (NULL == mr_ra_str.data.ptr)
    return (NULL);

  mr_ptrs_dfs (ptrs, xml1_print_node, &mr_ra_str);

  if (mr_ra_append_char (&mr_ra_str, '\n') < 0)
    return (NULL);

  return (mr_ra_str.data.ptr);
}

#ifdef HAVE_LIBXML2

static mr_status_t
xml2_save_node (mr_ra_ptrdes_t * ptrs, mr_idx_t idx, int level, mr_dfs_order_t order, void * context)
{
  if (MR_DFS_PRE_ORDER != order)
    return (MR_SUCCESS);

  mr_rarray_t * mr_ra_str = context;
  mr_idx_t parent = ptrs->ra[idx].parent;
  char number[MR_INT_TO_STRING_BUF_SIZE];

  /* route saving handler */
  mr_ra_printf_t save_handler = NULL;
  if ((ptrs->ra[idx].mr_type >= 0) && (ptrs->ra[idx].mr_type < MR_TYPE_LAST))
    save_handler = xml_ra_printf_tbl[ptrs->ra[idx].mr_type];
  
  if (NULL == save_handler)
    {
      save_handler = mr_ra_printf_void;
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNSUPPORTED_NODE_TYPE, ptrs->ra[idx].mr_type);
    }

  mr_ra_str->MR_SIZE = sizeof ("");
  mr_ra_str->data.string[0] = 0;
  
  if (!(ptrs->ra[idx].flags & (MR_IS_NULL | MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE)))
    if (save_handler (mr_ra_str, &ptrs->ra[idx]) < 0)
      return (MR_FAILURE);

  char * name = ptrs->ra[idx].fdp ? ptrs->ra[idx].fdp->name.str : MR_DEFAULT_NODE_NAME;
  xmlNodePtr node = xmlNewNode (NULL, BAD_CAST name);

  if (NULL == node)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (MR_FAILURE);
    }

  ptrs->ra[idx].res.data.ptr = node;
  ptrs->ra[idx].res.type = "xmlNode";
  node->_private = (void*)(intptr_t)idx;

  if (mr_ra_str->data.string[0])
    xmlNodeSetContent (node, BAD_CAST mr_ra_str->data.string);
  
  if (ptrs->ra[idx].flags & MR_IS_REFERENCE)
    {
      /* set REF_IDX property */
      snprintf (number, sizeof (number), "%" SCNd32, (uint32_t)ptrs->ra[ptrs->ra[idx].first_child].idx);
      xmlSetProp (node, BAD_CAST MR_REF, BAD_CAST number);
    }
  if (ptrs->ra[idx].flags & MR_IS_CONTENT_REFERENCE)
    {
      /* set REF_CONTENT property */
      snprintf (number, sizeof (number), "%" SCNd32, (uint32_t)ptrs->ra[ptrs->ra[idx].first_child].idx);
      xmlSetProp (node, BAD_CAST MR_REF_CONTENT, BAD_CAST number);
    }
  if (ptrs->ra[idx].flags & MR_IS_REFERENCED)
    {
      /* set IDX property */
      snprintf (number, sizeof (number), "%" SCNd32, (uint32_t)ptrs->ra[idx].idx);
      xmlSetProp (node, BAD_CAST MR_REF_IDX, BAD_CAST number);
    }
  if (ptrs->ra[idx].flags & MR_IS_NULL)
    xmlSetProp (node, BAD_CAST MR_ISNULL, BAD_CAST MR_ISNULL_VALUE);

  if (parent != MR_NULL_IDX)
    xmlAddChild (ptrs->ra[parent].res.data.ptr, node);

  return (MR_SUCCESS);
}

/**
 * Public function. Save scheduler. Save any object as XML node.
 * @param ptrs resizeable array with pointers descriptors
 * @return XML document
 */
xmlDocPtr
mr_xml2_save (mr_ra_ptrdes_t * ptrs)
{
  mr_rarray_t mr_ra_str = {
    .data = { mr_strdup ("") },
    .MR_SIZE = sizeof (""),
    .type = "string",
    .alloc_size = sizeof (""),
  };

  if (NULL == mr_ra_str.data.ptr)
    return (NULL);

  xmlDocPtr doc = xmlNewDoc (BAD_CAST "1.0");

  if (NULL == doc)
    MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_XML_SAVE_FAILED);
  else
    {
      ptrs->ptrdes_type = MR_PD_CUSTOM;
      ptrs->res.data.ptr = doc;
      ptrs->res.type = NULL;

      mr_ptrs_dfs (ptrs, xml2_save_node, &mr_ra_str);
      
      if (ptrs->size >= 2 * sizeof (ptrs->ra[0]))
	if (NULL != ptrs->ra[1].res.data.ptr)
	  xmlDocSetRootElement (doc, ptrs->ra[1].res.data.ptr);
    }

  if (mr_ra_str.data.ptr)
    MR_FREE (mr_ra_str.data.ptr);
  
  return (doc);
}
#endif /* HAVE_LIBXML2 */
