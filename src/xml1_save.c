/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

#include <stdio.h>
#include <stdarg.h>

#define RL_MODE PROTO /* explicitly set type of inclusion */
#include <reslib.h>

#define RL_XML1_DOCUMENT_HEADER "<?xml version=\"1.0\"?>"

#define RL_XML1_INDENT_SPACES (2)
#define RL_XML1_INDENT_TEMPLATE "\n%*s"
#define RL_XML1_INDENT "  "

#define RL_XML1_ATTR_CHARP " %s=\"%s\""
#define RL_XML1_ATTR_INT " %s=\"%" SCNd32 "\""

#define RL_XML1_OPEN_TAG_START "<%s"
#define RL_XML1_OPEN_TAG_END ">%s"
#define RL_XML1_OPEN_EMPTY_TAG_END "/>"
#define RL_XML1_CLOSE_TAG "</%s>"

/**
 * Public function. Save scheduler. Save any object as a string.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors
 * @return stringified representation of object
 */
char *
xml1_save (rl_ra_rl_ptrdes_t * ptrs)
{
  rl_rarray_t rl_ra_str = { .data = RL_STRDUP (RL_XML1_DOCUMENT_HEADER), .size = sizeof (RL_XML1_DOCUMENT_HEADER), .alloc_size = sizeof (RL_XML1_DOCUMENT_HEADER), .ext = NULL, };
  int idx = 0;

  if (NULL == rl_ra_str.data)
    return (NULL);
  
  while (idx >= 0)
    {
      int level = 0;
      int empty_tag = !0;
      char * content = NULL;
      rl_fd_t * fdp = &ptrs->ra.data[idx].fd;

      /* route saving handler */
      if ((fdp->rl_type_ext >= 0) && (fdp->rl_type_ext < RL_MAX_TYPES)
	  && rl_io_ext_handlers[fdp->rl_type_ext].save.xml)
	content = rl_io_ext_handlers[fdp->rl_type_ext].save.xml (idx, ptrs);
      else if ((fdp->rl_type >= 0) && (fdp->rl_type < RL_MAX_TYPES)
	       && rl_io_handlers[fdp->rl_type].save.xml)
	content = rl_io_handlers[fdp->rl_type].save.xml (idx, ptrs);
      else
	RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_UNSUPPORTED_NODE_TYPE, fdp->rl_type_ext, fdp->rl_type);

      if (content)
	{
	  level = RL_LIMIT_LEVEL (ptrs->ra.data[idx].level);
	  empty_tag = (ptrs->ra.data[idx].first_child < 0) && (0 == content[0]);
	  if (rl_ra_printf (&rl_ra_str, RL_XML1_INDENT_TEMPLATE RL_XML1_OPEN_TAG_START, level * RL_XML1_INDENT_SPACES, "", ptrs->ra.data[idx].fd.name) < 0)
	    return (NULL);
	  if (ptrs->ra.data[idx].flags & RL_PDF_IS_NULL)
	    if (rl_ra_printf (&rl_ra_str, RL_XML1_ATTR_CHARP, RL_ISNULL, "true") < 0)
	      return (NULL);
	  if (ptrs->ra.data[idx].ref_idx >= 0)
	    if (rl_ra_printf (&rl_ra_str, RL_XML1_ATTR_INT, RL_REF, ptrs->ra.data[ptrs->ra.data[idx].ref_idx].idx) < 0)
	      return (NULL);
	  if (ptrs->ra.data[idx].flags & RL_PDF_IS_REFERENCED)
	    if (rl_ra_printf (&rl_ra_str, RL_XML1_ATTR_INT, RL_REF_IDX, ptrs->ra.data[idx].idx) < 0)
	      return (NULL);
	  if (empty_tag)
	    {
	      if (rl_ra_printf (&rl_ra_str, RL_XML1_OPEN_EMPTY_TAG_END) < 0)
		return (NULL);
	    }
	  else
	    {
	      if (rl_ra_printf (&rl_ra_str, RL_XML1_OPEN_TAG_END, content) < 0)
		return (NULL);
	    }
	  RL_FREE (content);
	}
      
      if (ptrs->ra.data[idx].first_child >= 0)
	idx = ptrs->ra.data[idx].first_child;
      else
	{
	  if (!empty_tag)
	    if (rl_ra_printf (&rl_ra_str, RL_XML1_CLOSE_TAG, ptrs->ra.data[idx].fd.name) < 0)
	      return (NULL);
	  while ((ptrs->ra.data[idx].next < 0) && (ptrs->ra.data[idx].parent >= 0))
	    {
	      idx = ptrs->ra.data[idx].parent;
	      level = RL_LIMIT_LEVEL (ptrs->ra.data[idx].level);
	      if (rl_ra_printf (&rl_ra_str, RL_XML1_INDENT_TEMPLATE RL_XML1_CLOSE_TAG, level * RL_XML1_INDENT_SPACES, "", ptrs->ra.data[idx].fd.name) < 0)
		return (NULL);
	    }
	  idx = ptrs->ra.data[idx].next;
	}
    }
  if (rl_ra_printf (&rl_ra_str, "\n") < 0)
    return (NULL);
  return (rl_ra_str.data);
}

/**
 * RL_NONE type saving handler.
 * @param node a pointer on XML node
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors 
 */
static char *
xml_save_none (int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  return (NULL);
}

/**
 * RL_XXX type saving handler. Saves ptrs->ra.data[idx] as string into newly allocaeted XML node.
 * @param node a pointer on XML node
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors 
 */
#define XML_SAVE_TYPE(TYPE, EXT...) static char * xml_save_ ## TYPE (int idx, rl_ra_rl_ptrdes_t * ptrs) { return (rl_stringify_ ## TYPE (&ptrs->ra.data[idx] EXT)); }

XML_SAVE_TYPE (int8);
XML_SAVE_TYPE (uint8);
XML_SAVE_TYPE (int16);
XML_SAVE_TYPE (uint16);
XML_SAVE_TYPE (int32);
XML_SAVE_TYPE (uint32);
XML_SAVE_TYPE (int64);
XML_SAVE_TYPE (uint64);
XML_SAVE_TYPE (enum);
XML_SAVE_TYPE (float);
XML_SAVE_TYPE (double);
XML_SAVE_TYPE (long_double_t);
XML_SAVE_TYPE (bitmask, , RL_BITMASK_OR_DELIMITER);

/**
 * RL_CHAR type saving handler. Stringify char.
 * @param ptrs resizeable array with pointers descriptors 
 * @return stringified float value
 */
static char *
xml_save_char (int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  char str[RL_CHAR_TO_STRING_BUF_SIZE] = "";
  char c = *(char*)ptrs->ra.data[idx].data;
  if (isprint (c))
    sprintf (str, "%c", c);
  else
    sprintf (str, "\\%03o", ((unsigned int)((unsigned char)c)));
  return (RL_STRDUP (str));
}

/**
 * RL_CHAR_ARRAY type saving handler. Save char array as XML node.
 * @param node a pointer on XML node
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors 
 */
static char *
xml_save_char_array (int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  return (xml_quote_string (ptrs->ra.data[idx].data));
}

/**
 * RL_STRING type saving handler. Save string as XML node.
 * @param node a pointer on XML node
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors 
 */
static char *
xml_save_string (int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  char * str = *(char**)ptrs->ra.data[idx].data;
  return (NULL == str) ? RL_STRDUP ("") : xml_quote_string (str);
}

static char *
xml_save_empty (int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  return (RL_STRDUP (""));
}

/**
 * Init IO handlers Table
 */
void __attribute__((constructor)) rl_init_save_xml1 (void)
{
  rl_io_handlers[RL_TYPE_VOID].save.xml = xml_save_none;
  rl_io_handlers[RL_TYPE_ENUM].save.xml = xml_save_enum;
  rl_io_handlers[RL_TYPE_BITMASK].save.xml = xml_save_bitmask;
  rl_io_handlers[RL_TYPE_INT8].save.xml = xml_save_int8;
  rl_io_handlers[RL_TYPE_UINT8].save.xml = xml_save_uint8;
  rl_io_handlers[RL_TYPE_INT16].save.xml = xml_save_int16;
  rl_io_handlers[RL_TYPE_UINT16].save.xml = xml_save_uint16;
  rl_io_handlers[RL_TYPE_INT32].save.xml = xml_save_int32;
  rl_io_handlers[RL_TYPE_UINT32].save.xml = xml_save_uint32;
  rl_io_handlers[RL_TYPE_INT64].save.xml = xml_save_int64;
  rl_io_handlers[RL_TYPE_UINT64].save.xml = xml_save_uint64;
  rl_io_handlers[RL_TYPE_FLOAT].save.xml = xml_save_float;
  rl_io_handlers[RL_TYPE_DOUBLE].save.xml = xml_save_double;
  rl_io_handlers[RL_TYPE_LONG_DOUBLE].save.xml = xml_save_long_double_t;
  rl_io_handlers[RL_TYPE_CHAR].save.xml = xml_save_char;
  rl_io_handlers[RL_TYPE_CHAR_ARRAY].save.xml = xml_save_char_array;
  rl_io_handlers[RL_TYPE_STRING].save.xml = xml_save_string;
  rl_io_handlers[RL_TYPE_STRUCT].save.xml = xml_save_empty;
  rl_io_handlers[RL_TYPE_UNION].save.xml = xml_save_empty;
  rl_io_handlers[RL_TYPE_ANON_UNION].save.xml = xml_save_empty;

  rl_io_ext_handlers[RL_TYPE_EXT_ARRAY].save.xml = xml_save_empty;
  rl_io_ext_handlers[RL_TYPE_EXT_RARRAY].save.xml = xml_save_empty;
  rl_io_ext_handlers[RL_TYPE_EXT_POINTER].save.xml = xml_save_empty;
}
