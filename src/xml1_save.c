/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <stdio.h>
#include <stdarg.h>

#include <metaresc.h>

#define MR_XML1_DOCUMENT_HEADER "<?xml version=\"1.0\"?>"

#define MR_XML1_INDENT_SPACES (2)
#define MR_XML1_INDENT_TEMPLATE "\n%*s"
#define MR_XML1_INDENT "  "

#define MR_XML1_ATTR_CHARP " %s=\"%s\""
#define MR_XML1_ATTR_INT " %s=\"%" SCNd32 "\""

#define MR_XML1_OPEN_TAG_START "<%s"
#define MR_XML1_OPEN_TAG_END ">%s"
#define MR_XML1_OPEN_EMPTY_TAG_END "/>"
#define MR_XML1_CLOSE_TAG "</%s>"

/**
 * Public function. Save scheduler. Save any object as a string.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors
 * @return stringified representation of object
 */
char *
xml1_save (mr_ra_mr_ptrdes_t * ptrs)
{
  mr_rarray_t mr_ra_str = { .data = MR_STRDUP (MR_XML1_DOCUMENT_HEADER), .size = sizeof (MR_XML1_DOCUMENT_HEADER), .alloc_size = sizeof (MR_XML1_DOCUMENT_HEADER), .ext = { NULL }, };
  int idx = 0;

  if (NULL == mr_ra_str.data)
    return (NULL);
  
  while (idx >= 0)
    {
      int level = 0;
      int empty_tag = !0;
      char * content = NULL;
      mr_fd_t * fdp = &ptrs->ra.data[idx].fd;

      /* route saving handler */
      if ((fdp->mr_type_ext >= 0) && (fdp->mr_type_ext < MR_MAX_TYPES)
	  && mr_conf.io_ext_handlers[fdp->mr_type_ext].save.xml1)
	content = mr_conf.io_ext_handlers[fdp->mr_type_ext].save.xml1 (idx, ptrs);
      else if ((fdp->mr_type >= 0) && (fdp->mr_type < MR_MAX_TYPES)
	       && mr_conf.io_handlers[fdp->mr_type].save.xml1)
	content = mr_conf.io_handlers[fdp->mr_type].save.xml1 (idx, ptrs);
      else
	MR_MESSAGE_UNSUPPORTED_NODE_TYPE_ (fdp);    	  

      level = MR_LIMIT_LEVEL (ptrs->ra.data[idx].level);
      empty_tag = (ptrs->ra.data[idx].first_child < 0) && ((NULL == content) || (0 == content[0]));
      if (mr_ra_printf (&mr_ra_str, MR_XML1_INDENT_TEMPLATE MR_XML1_OPEN_TAG_START, level * MR_XML1_INDENT_SPACES, "", ptrs->ra.data[idx].fd.name) < 0)
	return (NULL);
      if (ptrs->ra.data[idx].ref_idx >= 0)
	if (mr_ra_printf (&mr_ra_str, MR_XML1_ATTR_INT,
			  (ptrs->ra.data[idx].flags & MR_PDF_CONTENT_REFERENCE) ? MR_REF_CONTENT : MR_REF,
			  ptrs->ra.data[ptrs->ra.data[idx].ref_idx].idx) < 0)
	  return (NULL);
      if (ptrs->ra.data[idx].flags & MR_PDF_IS_REFERENCED)
	if (mr_ra_printf (&mr_ra_str, MR_XML1_ATTR_INT, MR_REF_IDX, ptrs->ra.data[idx].idx) < 0)
	  return (NULL);
      if (ptrs->ra.data[idx].flags & MR_PDF_IS_NULL)
	if (mr_ra_printf (&mr_ra_str, MR_XML1_ATTR_CHARP, MR_ISNULL, MR_ISNULL_VALUE) < 0)
	  return (NULL);
	  
      if (empty_tag)
	{
	  if (mr_ra_printf (&mr_ra_str, MR_XML1_OPEN_EMPTY_TAG_END) < 0)
	    return (NULL);
	}
      else
	{
	  if (mr_ra_printf (&mr_ra_str, MR_XML1_OPEN_TAG_END, content ? content : "") < 0)
	    return (NULL);
	}
      if (content)
	MR_FREE (content);
      
      if (ptrs->ra.data[idx].first_child >= 0)
	idx = ptrs->ra.data[idx].first_child;
      else
	{
	  if (!empty_tag)
	    if (mr_ra_printf (&mr_ra_str, MR_XML1_CLOSE_TAG, ptrs->ra.data[idx].fd.name) < 0)
	      return (NULL);
	  while ((ptrs->ra.data[idx].next < 0) && (ptrs->ra.data[idx].parent >= 0))
	    {
	      idx = ptrs->ra.data[idx].parent;
	      level = MR_LIMIT_LEVEL (ptrs->ra.data[idx].level);
	      if (mr_ra_printf (&mr_ra_str, MR_XML1_INDENT_TEMPLATE MR_XML1_CLOSE_TAG, level * MR_XML1_INDENT_SPACES, "", ptrs->ra.data[idx].fd.name) < 0)
		return (NULL);
	    }
	  idx = ptrs->ra.data[idx].next;
	}
    }
  if (mr_ra_printf (&mr_ra_str, "\n") < 0)
    return (NULL);
  return (mr_ra_str.data);
}

/**
 * MR_NONE type saving handler.
 * @param node a pointer on XML node
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors 
 */
static char *
xml_save_none (int idx, mr_ra_mr_ptrdes_t * ptrs)
{
  return (NULL);
}

/**
 * MR_XXX type saving handler. Saves ptrs->ra.data[idx] as string into newly allocaeted XML node.
 * @param node a pointer on XML node
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors 
 */
#define XML_SAVE_TYPE(TYPE, ...)				      \
  static char * xml_save_ ## TYPE (int idx, mr_ra_mr_ptrdes_t * ptrs) \
  {								      \
    return (mr_stringify_ ## TYPE (&ptrs->ra.data[idx] __VA_ARGS__)); \
  }

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
XML_SAVE_TYPE (bitfield);
XML_SAVE_TYPE (bitmask, , MR_BITMASK_OR_DELIMITER);

/**
 * MR_CHAR type saving handler. Stringify char.
 * @param ptrs resizeable array with pointers descriptors 
 * @return stringified float value
 */
static char *
xml_save_char (int idx, mr_ra_mr_ptrdes_t * ptrs)
{
  char str[MR_CHAR_TO_STRING_BUF_SIZE] = " ";
  unsigned char c = *(char*)ptrs->ra.data[idx].data;
  if (isprint (c))
    str[0] = c;
  else
    sprintf (str, "\\%03o", c);
  return (xml_quote_string (str));
}

/**
 * MR_CHAR_ARRAY type saving handler. Save char array as XML node.
 * @param node a pointer on XML node
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors 
 */
static char *
xml_save_char_array (int idx, mr_ra_mr_ptrdes_t * ptrs)
{
  return (xml_quote_string (ptrs->ra.data[idx].data));
}

/**
 * MR_STRING type saving handler. Save string as XML node.
 * @param node a pointer on XML node
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors 
 */
static char *
xml1_save_string (int idx, mr_ra_mr_ptrdes_t * ptrs)
{
  char * str = *(char**)ptrs->ra.data[idx].data;
  if ((NULL == str) || (ptrs->ra.data[idx].ref_idx >= 0))
    return (MR_STRDUP (""));
  else 
    return (xml_quote_string (str));
}

static char *
xml_save_empty (int idx, mr_ra_mr_ptrdes_t * ptrs)
{
  return (MR_STRDUP (""));
}

/**
 * Init IO handlers Table
 */
void mr_init_save_xml (void)
{
  int i;
  for (i = 0; i < MR_MAX_TYPES; ++i)
    {
      mr_conf.io_handlers[i].save.xml = NULL;
      mr_conf.io_ext_handlers[i].save.xml = NULL;
    }
  
  mr_conf.io_handlers[MR_TYPE_NONE].save.xml = xml_save_none;
  mr_conf.io_handlers[MR_TYPE_VOID].save.xml = xml_save_none;
  mr_conf.io_handlers[MR_TYPE_ENUM].save.xml = xml_save_enum;
  mr_conf.io_handlers[MR_TYPE_BITFIELD].save.xml = xml_save_bitfield;
  mr_conf.io_handlers[MR_TYPE_BITMASK].save.xml = xml_save_bitmask;
  mr_conf.io_handlers[MR_TYPE_INT8].save.xml = xml_save_int8;
  mr_conf.io_handlers[MR_TYPE_UINT8].save.xml = xml_save_uint8;
  mr_conf.io_handlers[MR_TYPE_INT16].save.xml = xml_save_int16;
  mr_conf.io_handlers[MR_TYPE_UINT16].save.xml = xml_save_uint16;
  mr_conf.io_handlers[MR_TYPE_INT32].save.xml = xml_save_int32;
  mr_conf.io_handlers[MR_TYPE_UINT32].save.xml = xml_save_uint32;
  mr_conf.io_handlers[MR_TYPE_INT64].save.xml = xml_save_int64;
  mr_conf.io_handlers[MR_TYPE_UINT64].save.xml = xml_save_uint64;
  mr_conf.io_handlers[MR_TYPE_FLOAT].save.xml = xml_save_float;
  mr_conf.io_handlers[MR_TYPE_DOUBLE].save.xml = xml_save_double;
  mr_conf.io_handlers[MR_TYPE_LONG_DOUBLE].save.xml = xml_save_long_double_t;
  mr_conf.io_handlers[MR_TYPE_CHAR].save.xml = xml_save_char;
  mr_conf.io_handlers[MR_TYPE_CHAR_ARRAY].save.xml = xml_save_char_array;
  mr_conf.io_handlers[MR_TYPE_STRUCT].save.xml = xml_save_empty;
  mr_conf.io_handlers[MR_TYPE_FUNC].save.xml = xml_save_none;
  mr_conf.io_handlers[MR_TYPE_FUNC_TYPE].save.xml = xml_save_none;
  mr_conf.io_handlers[MR_TYPE_UNION].save.xml = xml_save_empty;
  mr_conf.io_handlers[MR_TYPE_ANON_UNION].save.xml = xml_save_empty;

  mr_conf.io_ext_handlers[MR_TYPE_EXT_ARRAY].save.xml = xml_save_empty;
  mr_conf.io_ext_handlers[MR_TYPE_EXT_RARRAY_DATA].save.xml = xml_save_empty;
  mr_conf.io_ext_handlers[MR_TYPE_EXT_POINTER].save.xml = xml_save_empty;
}

void __attribute__((constructor))
mr_init_save_xml1 (void)
{
  int i;
  
  mr_init_save_xml ();
  for (i = 0; i < MR_MAX_TYPES; ++i)
    {
      mr_conf.io_handlers[i].save.xml1 = mr_conf.io_handlers[i].save.xml;
      mr_conf.io_ext_handlers[i].save.xml1 = mr_conf.io_ext_handlers[i].save.xml;
    }
    
  mr_conf.io_handlers[MR_TYPE_STRING].save.xml1 = xml1_save_string;
}
