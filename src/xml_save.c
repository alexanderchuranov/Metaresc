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
/* a reference of the largest code point, for messages about references */
#define ESC_SIZE (sizeof ("&#x10FFFF;"))
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
                );

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
        /* the escape sequence for the messages, strncpy does not terminate a copy of the full size */
        char esc[ESC_SIZE + 1];
        strncpy (esc, &substr->str[i], sizeof (esc) - 1);
        esc[sizeof (esc) - 1] = 0;
        char * semicolon = strchr (esc, ';');
        if (semicolon)
          semicolon[1] = 0;

        if ('#' == substr->str[i + 1])
          {
            int32_t code = 0;
            int size = 0;
            /*
              sscanf returns 1 once the code is read, but sets size only if the closing ';' matches too.
              Without the size the loop would not advance and would write past the end of dst.
            */
            /* a reference has a hexadecimal or a decimal code point, up to U+10FFFF */
            int scanned = ('x' == substr->str[i + 2]) ?
              sscanf (&substr->str[i], "&#x%6X;%n", &code, &size) :
              sscanf (&substr->str[i], "&#%7d;%n", &code, &size);
            if ((1 != scanned) || (size <= 0))
              {
                MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_WRONG_XML_ESC, esc);
                dst[length++] = substr->str[i];
              }
            else
              {
                i += size - 1; /* one more +1 in the loop */
                /* a reference is a Unicode code point, written in UTF-8; it is not longer than the reference */
                length += mr_utf8_encode (code, &dst[length]);
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

/**
 * Appends a character with XML escapes. Unprintable characters are
 * appended as numeric character references.
 * @param mr_ra_str output string
 * @param c character
 * @return number of appended characters or -1 on error
 */
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

/**
 * Appends a char with XML escapes.
 * @param mr_ra_str output string
 * @param ptrdes node descriptor
 * @return number of appended characters or -1 on error
 */
static int
xml_ra_printf_char (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (xml_ra_printf_quote_char (mr_ra_str, *ptrdes->data.string));
}

/**
 * Appends characters with XML escapes.
 * @param mr_ra_str output string
 * @param str characters, not necessarily zero terminated
 * @param length number of characters
 * @return number of appended characters or -1 on error
 */
static int
xml_ra_printf_quote_chars (mr_rarray_t * mr_ra_str, char * str, size_t length)
{
  int count = 0;
  size_t i;
  for (i = 0; i < length; ++i)
    {
      /* XML is UTF-8, so a character beyond ASCII is written as is; a byte that is not a part of it is escaped */
      size_t sequence = mr_utf8_sequence_length (&str[i], length - i);
      if (sequence > 0)
        {
          for ( ; sequence > 1; --sequence)
            count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, str[i++]));
          count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, str[i]));
        }
      else
        count += TRY_CATCH_THROW (xml_ra_printf_quote_char (mr_ra_str, str[i]));
    }
  return (count);
}

/**
 * Appends a string with XML escapes.
 * @param mr_ra_str output string
 * @param str string
 * @return number of appended characters or -1 on error
 */
static int
xml_ra_printf_quote_string (mr_rarray_t * mr_ra_str, char * str)
{
  return (xml_ra_printf_quote_chars (mr_ra_str, str, strlen (str)));
}

/**
 * Appends a float complex value as 'real + imag I'.
 * @param mr_ra_str output string
 * @param ptrdes node descriptor
 * @return number of appended characters or -1 on error
 */
static int
xml_ra_printf_complex_float (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf_complex_float (mr_ra_str, ptrdes, COMPLEX_REAL_IMAG_DELIMITER));
}

/**
 * Appends a double complex value as 'real + imag I'.
 * @param mr_ra_str output string
 * @param ptrdes node descriptor
 * @return number of appended characters or -1 on error
 */
static int
xml_ra_printf_complex_double (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf_complex_double (mr_ra_str, ptrdes, COMPLEX_REAL_IMAG_DELIMITER));
}

/**
 * Appends a long double complex value as 'real + imag I'.
 * @param mr_ra_str output string
 * @param ptrdes node descriptor
 * @return number of appended characters or -1 on error
 */
static int
xml_ra_printf_complex_long_double_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf_complex_long_double_t (mr_ra_str, ptrdes, COMPLEX_REAL_IMAG_DELIMITER));
}

#define MR_XML_BITMASK_DELIMITER " | "

/**
 * Appends a bitmask enum as names joined with the XML bitmask delimiter.
 * @param mr_ra_str output string
 * @param ptrdes node descriptor
 * @return number of appended characters or -1 on error
 */
static int
xml_ra_printf_bitmask (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf_bitmask (mr_ra_str, ptrdes, MR_XML_BITMASK_DELIMITER));
}

/**
 * Appends the value of a bit-field, bitmask enums as names.
 * @param mr_ra_str output string
 * @param ptrdes node descriptor
 * @return number of appended characters or -1 on error
 */
static int
xml_ra_printf_bitfield (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf_bitfield (mr_ra_str, ptrdes, MR_XML_BITMASK_DELIMITER));
}

/**
 * Appends a char array with XML escapes. The array may be not zero
 * terminated.
 * @param mr_ra_str output string
 * @param ptrdes node descriptor
 * @return number of appended characters or -1 on error
 */
static int
xml_ra_printf_char_array (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  /* a char array may be filled up to its size without a terminating zero */
  return (xml_ra_printf_quote_chars (mr_ra_str, ptrdes->data.ptr, strnlen (ptrdes->data.ptr, ptrdes->MR_SIZE)));
}

/**
 * Appends a string with XML escapes.
 * @param mr_ra_str output string
 * @param ptrdes node descriptor
 * @return number of appended characters or -1 on error
 */
static int
xml_ra_printf_string (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (xml_ra_printf_quote_string (mr_ra_str, *(char**)ptrdes->data.ptr));
}

/**
 * Appends a function pointer by name or as an address.
 * @param mr_ra_str output string
 * @param ptrdes node descriptor
 * @return number of appended characters or -1 on error
 */
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

/**
 * Prints the opening tag of a node with reference and NULL attributes, and
 * the value. Nodes without a value and children are printed as empty tags;
 * the flag is stored in the node for the post-order pass.
 * @param ptrs array with node descriptors
 * @param idx index of the node
 * @param level depth of the node
 * @param mr_ra_str output string
 * @return status
 */
static mr_status_t
xml1_pre_print_node (mr_ptrdes_t * ptrs, mr_idx_t idx, int level, mr_rarray_t * mr_ra_str)
{
  bool empty_tag = true;

  /* route saving handler */
  mr_ra_printf_t save_handler = NULL;
  if ((ptrs[idx].mr_type >= 0) && (ptrs[idx].mr_type < MR_TYPE_LAST))
    save_handler = xml_ra_printf_tbl[ptrs[idx].mr_type];
  
  if (NULL == save_handler)
    {
      save_handler = mr_ra_printf_void;
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNSUPPORTED_NODE_TYPE, ptrs[idx].mr_type);
    }

  char * name = ptrs[idx].fdp ? ptrs[idx].fdp->name.str : MR_DEFAULT_NODE_NAME;
  if (mr_ra_printf (mr_ra_str, MR_XML1_INDENT_TEMPLATE MR_XML1_OPEN_TAG_START,
                    MR_LIMIT_LEVEL (level) * MR_XML1_INDENT_SPACES, "", name) < 0)
    return (MR_FAILURE);
  
  if (ptrs[idx].flags & MR_IS_REFERENCE)
    if (mr_ra_printf (mr_ra_str, MR_XML1_ATTR_INT, MR_REF, (uint32_t)ptrs[ptrs[idx].first_child].idx) < 0)
      return (MR_FAILURE);

  if (ptrs[idx].flags & MR_IS_CONTENT_REFERENCE)
    if (mr_ra_printf (mr_ra_str, MR_XML1_ATTR_INT, MR_REF_CONTENT, (uint32_t)ptrs[ptrs[idx].first_child].idx) < 0)
      return (MR_FAILURE);
  
  if (ptrs[idx].flags & MR_IS_REFERENCED)
    if (mr_ra_printf (mr_ra_str, MR_XML1_ATTR_INT, MR_REF_IDX, (uint32_t)ptrs[idx].idx) < 0)
      return (MR_FAILURE);
  
  if (ptrs[idx].flags & MR_IS_NULL)
    if (mr_ra_printf (mr_ra_str, MR_XML1_ATTR_CHARP, MR_ISNULL, MR_ISNULL_VALUE) < 0)
      return (MR_FAILURE);

  if (ptrs[idx].flags & (MR_IS_NULL | MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE))
    empty_tag = true;
  else
    {
      if (mr_ra_append_char (mr_ra_str, '>') < 0)
        return (MR_FAILURE);
      int count = save_handler (mr_ra_str, &ptrs[idx]);
      empty_tag = (ptrs[idx].first_child == MR_NULL_IDX) && (0 == count);
      if (empty_tag)
        mr_ra_str->data.string[--mr_ra_str->MR_SIZE] = 0;
    }
      
  if (empty_tag)
    {
      if (mr_ra_append_string (mr_ra_str, MR_XML1_OPEN_EMPTY_TAG_END) < 0)
        return (MR_FAILURE);
    }

  ptrs[idx].vt_intptr = empty_tag;
  ptrs[idx].value_type = MR_VT_INTPTR;

  return (MR_SUCCESS);
}

/**
 * Prints the closing tag of a node unless it was printed as an empty tag.
 * @param ptrs array with node descriptors
 * @param idx index of the node
 * @param level depth of the node
 * @param mr_ra_str output string
 * @return status
 */
static mr_status_t
xml1_post_print_node (mr_ptrdes_t * ptrs, mr_idx_t idx, int level, mr_rarray_t * mr_ra_str)
{
  if ((ptrs[idx].first_child != MR_NULL_IDX) && !(ptrs[idx].flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE)))
    if (mr_ra_printf (mr_ra_str, MR_XML1_INDENT_TEMPLATE, MR_LIMIT_LEVEL (level) * MR_XML1_INDENT_SPACES, "") < 0)
      return (MR_FAILURE);

  if (!ptrs[idx].vt_intptr)
    {
      if (mr_ra_append_string (mr_ra_str, "</") < 0)
        return (MR_FAILURE);
      char * name = ptrs[idx].fdp ? ptrs[idx].fdp->name.str : MR_DEFAULT_NODE_NAME;
      if (mr_ra_append_string (mr_ra_str, name) < 0)
        return (MR_FAILURE);
      if (mr_ra_append_char (mr_ra_str, '>') < 0)
        return (MR_FAILURE);
    }

  return (MR_SUCCESS);
}

/**
 * DFS visitor that prints a node as XML text.
 * @param ptrs array with node descriptors
 * @param idx index of the node
 * @param level depth of the node
 * @param order DFS order
 * @param context output string
 * @return status
 */
static mr_status_t
xml1_print_node (mr_ptrdes_t * ptrs, mr_idx_t idx, int level, mr_dfs_order_t order, void * context)
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
mr_xml1_save (mr_ptrdes_t * ptrs)
{
  if (ptrs == NULL)
    return (NULL);

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

/**
 * DFS visitor that builds a libxml2 node for a node with its value and
 * reference attributes, and links it to the node of the parent.
 * @param ptrs array with node descriptors
 * @param idx index of the node
 * @param level depth of the node
 * @param order DFS order, only pre-order is handled
 * @param context buffer for formatted values
 * @return status
 */
static mr_status_t
xml2_save_node (mr_ptrdes_t * ptrs, mr_idx_t idx, int level, mr_dfs_order_t order, void * context)
{
  if (MR_DFS_PRE_ORDER != order)
    return (MR_SUCCESS);

  mr_rarray_t * mr_ra_str = context;
  mr_idx_t parent = ptrs[idx].parent;
  char number[MR_INT_TO_STRING_BUF_SIZE];

  /* route saving handler */
  mr_ra_printf_t save_handler = NULL;
  if ((ptrs[idx].mr_type >= 0) && (ptrs[idx].mr_type < MR_TYPE_LAST))
    save_handler = xml_ra_printf_tbl[ptrs[idx].mr_type];
  
  if (NULL == save_handler)
    {
      save_handler = mr_ra_printf_void;
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNSUPPORTED_NODE_TYPE, ptrs[idx].mr_type);
    }

  mr_ra_str->MR_SIZE = sizeof ("");
  mr_ra_str->data.string[0] = 0;
  
  if (!(ptrs[idx].flags & (MR_IS_NULL | MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE)))
    if (save_handler (mr_ra_str, &ptrs[idx]) < 0)
      return (MR_FAILURE);

  char * name = ptrs[idx].fdp ? ptrs[idx].fdp->name.str : MR_DEFAULT_NODE_NAME;
  xmlNodePtr node = xmlNewNode (NULL, BAD_CAST name);

  if (NULL == node)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (MR_FAILURE);
    }

  ptrs[idx].vt_ptr = node;
  ptrs[idx].value_type = MR_VT_PTR;
  node->_private = (void*)(intptr_t)idx;

  if (mr_ra_str->data.string[0])
    xmlNodeSetContent (node, BAD_CAST mr_ra_str->data.string);
  
  if (ptrs[idx].flags & MR_IS_REFERENCE)
    {
      /* set REF property */
      snprintf (number, sizeof (number), "%" SCNu32, (uint32_t)ptrs[ptrs[idx].first_child].idx);
      xmlSetProp (node, BAD_CAST MR_REF, BAD_CAST number);
    }
  if (ptrs[idx].flags & MR_IS_CONTENT_REFERENCE)
    {
      /* set REF_CONTENT property */
      snprintf (number, sizeof (number), "%" SCNu32, (uint32_t)ptrs[ptrs[idx].first_child].idx);
      xmlSetProp (node, BAD_CAST MR_REF_CONTENT, BAD_CAST number);
    }
  if (ptrs[idx].flags & MR_IS_REFERENCED)
    {
      /* set REF_IDX property */
      snprintf (number, sizeof (number), "%" SCNu32, (uint32_t)ptrs[idx].idx);
      xmlSetProp (node, BAD_CAST MR_REF_IDX, BAD_CAST number);
    }
  if (ptrs[idx].flags & MR_IS_NULL)
    xmlSetProp (node, BAD_CAST MR_ISNULL, BAD_CAST MR_ISNULL_VALUE);

  if (parent != MR_NULL_IDX)
    xmlAddChild (ptrs[parent].vt_ptr, node);

  return (MR_SUCCESS);
}

/**
 * Public function. Save scheduler. Save any object as XML node.
 * @param ptrs resizeable array with pointers descriptors
 * @return XML document
 */
xmlDocPtr
mr_xml2_save (mr_ptrdes_t * ptrs)
{
  if (ptrs == NULL)
    return (NULL);

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
      mr_ptrs_dfs (ptrs, xml2_save_node, &mr_ra_str);
      
      if (NULL != ptrs[1].vt_ptr)
        xmlDocSetRootElement (doc, ptrs[1].vt_ptr);
    }

  if (mr_ra_str.data.ptr)
    MR_FREE (mr_ra_str.data.ptr);
  
  return (doc);
}
#endif /* HAVE_LIBXML2 */
