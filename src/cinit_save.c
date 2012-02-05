/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <stdio.h>
#include <stdarg.h>
#include <metaresc.h>

#define MR_CINIT_NULL "NULL"
#define MR_CINIT_INDENT_SPACES (2)
#define MR_CINIT_INDENT_TEMPLATE "%*s"
#define MR_CINIT_INDENT "  "

#define MR_CINIT_TYPE_SIZE_TEMPLATE "%1$" SCNd32
#define MR_CINIT_TYPE_NAME_TEMPLATE "%2$s"
#define MR_JSON_TYPE_NAME_TEMPLATE MR_CINIT_TYPE_NAME_TEMPLATE
#define MR_CINIT_FIELDS_DELIMITER ",\n"
#define MR_CINIT_NAMED_FIELD_TEMPLATE ".%s = "
#define MR_JSON_NAMED_FIELD_TEMPLATE "\"%s\" : "
#define MR_CINIT_ATTR_INT "/* %s = %" SCNd32 " */ "

#define MR_CINIT_UNNAMED_FIELDS (0)
#define MR_CINIT_NAMED_FIELDS (!MR_CINIT_UNNAMED_FIELDS)

static int cinit_named_node[MR_MAX_TYPES] = {
  [0 ... MR_MAX_TYPES - 1] = MR_CINIT_UNNAMED_FIELDS,
  [MR_TYPE_STRUCT] = MR_CINIT_NAMED_FIELDS,
  [MR_TYPE_UNION] = MR_CINIT_NAMED_FIELDS,
  [MR_TYPE_ANON_UNION] = MR_CINIT_NAMED_FIELDS,
};

/**
 * Public function. Save scheduler. Save any object as a string.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors
 * @return stringified representation of object
 */
static char *
cinit_json_save (mr_ra_mr_ptrdes_t * ptrs, char * named_field_template, int (*node_handler) (mr_fd_t*, int, mr_ra_mr_ptrdes_t*, mr_save_type_data_t*))
{
  mr_rarray_t mr_ra_str = { .data = MR_STRDUP (""), .size = sizeof (""), .alloc_size = sizeof (""), .ext = { NULL }, };
  int idx = 0;

  if (NULL == mr_ra_str.data)
    return (NULL);
  
  while (idx >= 0)
    {
      int level = 0;
      int skip_node;
      mr_fd_t * fdp = &ptrs->ra.data[idx].fd;
      mr_save_type_data_t save_data = { .prefix = NULL, .content = NULL, .suffix = NULL, };

      /* route saving handler */
      skip_node = node_handler (fdp, idx, ptrs, &save_data);
      if (!skip_node)
	{
	  int named_node = MR_CINIT_UNNAMED_FIELDS;
	  level = MR_LIMIT_LEVEL (ptrs->ra.data[idx].level);
	  if (ptrs->ra.data[idx].level > 0)
	    if (mr_ra_printf (&mr_ra_str, MR_CINIT_INDENT_TEMPLATE, level * MR_CINIT_INDENT_SPACES, "") < 0)
	      return (NULL);
	  if ((ptrs->ra.data[idx].parent >= 0) && (MR_TYPE_EXT_NONE == ptrs->ra.data[ptrs->ra.data[idx].parent].fd.mr_type_ext)
	      && strcmp ("mr_ptr_t", ptrs->ra.data[ptrs->ra.data[idx].parent].fd.type)) /* ugly hack for synthetic type. mr_ptr_t members should be unnamed */
	    named_node = cinit_named_node[ptrs->ra.data[ptrs->ra.data[idx].parent].fd.mr_type];
	  if (MR_CINIT_NAMED_FIELDS == named_node)
	    if (mr_ra_printf (&mr_ra_str, named_field_template, ptrs->ra.data[idx].fd.name) < 0)
	      return (NULL);
	  if (ptrs->ra.data[idx].ref_idx >= 0)
	    if (mr_ra_printf (&mr_ra_str, MR_CINIT_ATTR_INT,
			      (ptrs->ra.data[idx].flags & MR_PDF_CONTENT_REFERENCE) ? MR_REF_CONTENT : MR_REF,
			      ptrs->ra.data[ptrs->ra.data[idx].ref_idx].idx) < 0)
	      return (NULL);
	  if (ptrs->ra.data[idx].flags & MR_PDF_IS_REFERENCED)
	    if (mr_ra_printf (&mr_ra_str, MR_CINIT_ATTR_INT, MR_REF_IDX, ptrs->ra.data[idx].idx) < 0)
	      return (NULL);
	  if (save_data.prefix)
	    if (mr_ra_printf (&mr_ra_str, save_data.prefix, ptrs->ra.data[idx].fd.size, ptrs->ra.data[idx].fd.type) < 0)
	      return (NULL);
	  if (save_data.content)
	    {
	      if (mr_ra_printf (&mr_ra_str, "%s", save_data.content) < 0)
		return (NULL);
	      MR_FREE (save_data.content);
	    }
	  ptrs->ra.data[idx].ext.ptr = save_data.suffix;
	  ptrs->ra.data[idx].ptr_type = "string_t";
	}
      
      if (ptrs->ra.data[idx].first_child >= 0)
	idx = ptrs->ra.data[idx].first_child;
      else
	{
	  if (save_data.suffix)
	    {
	      level = MR_LIMIT_LEVEL (ptrs->ra.data[idx].level);
	      if (mr_ra_printf (&mr_ra_str, MR_CINIT_INDENT_TEMPLATE, level * MR_CINIT_INDENT_SPACES, "") < 0)
		return (NULL);
	      if (mr_ra_printf (&mr_ra_str, "%s", save_data.suffix) < 0)
		return (NULL);
	    }
	  if (!skip_node && (idx != 0))
	    if (mr_ra_printf (&mr_ra_str, MR_CINIT_FIELDS_DELIMITER) < 0)
	      return (NULL);
	  while ((ptrs->ra.data[idx].next < 0) && (ptrs->ra.data[idx].parent >= 0))
	    {
	      idx = ptrs->ra.data[idx].parent;
	      if (ptrs->ra.data[idx].ext.ptr)
		{
		  level = MR_LIMIT_LEVEL (ptrs->ra.data[idx].level);
		  if (mr_ra_printf (&mr_ra_str, MR_CINIT_INDENT_TEMPLATE, level * MR_CINIT_INDENT_SPACES, "") < 0)
		    return (NULL);
		  if (mr_ra_printf (&mr_ra_str, "%s", (char*)ptrs->ra.data[idx].ext.ptr) < 0)
		    return (NULL);
		  if (idx != 0)
		    if (mr_ra_printf (&mr_ra_str, MR_CINIT_FIELDS_DELIMITER) < 0)
		      return (NULL);
		}
	    }
	  idx = ptrs->ra.data[idx].next;
	}
    }
  
  return (mr_ra_str.data);
}

char *
cinit_save (mr_ra_mr_ptrdes_t * _ptrs_)
{
  int node_handler (mr_fd_t * fdp, int idx, mr_ra_mr_ptrdes_t * ptrs, mr_save_type_data_t * save_data)
  {
    int skip_node = 0;
    if ((fdp->mr_type_ext >= 0) && (fdp->mr_type_ext < MR_MAX_TYPES) && mr_conf.io_ext_handlers[fdp->mr_type_ext].save.cinit)
      skip_node = mr_conf.io_ext_handlers[fdp->mr_type_ext].save.cinit (idx, ptrs, save_data);
    else if ((fdp->mr_type >= 0) && (fdp->mr_type < MR_MAX_TYPES) && mr_conf.io_handlers[fdp->mr_type].save.cinit)
      skip_node = mr_conf.io_handlers[fdp->mr_type].save.cinit (idx, ptrs, save_data);
    else
      MR_MESSAGE_UNSUPPORTED_NODE_TYPE_ (fdp);
    return (skip_node);
  }
  
  return (cinit_json_save (_ptrs_, MR_CINIT_NAMED_FIELD_TEMPLATE, node_handler));
}

char *
json_save (mr_ra_mr_ptrdes_t * _ptrs_)
{
  int node_handler (mr_fd_t * fdp, int idx, mr_ra_mr_ptrdes_t * ptrs, mr_save_type_data_t * save_data)
  {
    int skip_node = 0;
    if ((fdp->mr_type_ext >= 0) && (fdp->mr_type_ext < MR_MAX_TYPES) && mr_conf.io_ext_handlers[fdp->mr_type_ext].save.json)
      skip_node = mr_conf.io_ext_handlers[fdp->mr_type_ext].save.json (idx, ptrs, save_data);
    else if ((fdp->mr_type >= 0) && (fdp->mr_type < MR_MAX_TYPES) && mr_conf.io_handlers[fdp->mr_type].save.json)
      skip_node = mr_conf.io_handlers[fdp->mr_type].save.json (idx, ptrs, save_data);
    else
      MR_MESSAGE_UNSUPPORTED_NODE_TYPE_ (fdp);
    return (skip_node);
  }
  
  return (cinit_json_save (_ptrs_, MR_JSON_NAMED_FIELD_TEMPLATE, node_handler));
}

/**
 * MR_NONE type saving handler.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors
 * @return stringified int value
 */
static int
cinit_save_none (int idx, mr_ra_mr_ptrdes_t * ptrs, mr_save_type_data_t * data)
{
  return (!0);
}

/**
 * MR_XXX type saving handler. Make a string from *(XXX_t*)data.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors 
 */
#define CINIT_SAVE_TYPE(TYPE, EXT...) static int cinit_save_ ## TYPE (int idx, mr_ra_mr_ptrdes_t * ptrs, mr_save_type_data_t * data) { data->content = mr_stringify_ ## TYPE (&ptrs->ra.data[idx] EXT); return (0); }

CINIT_SAVE_TYPE (int8);
CINIT_SAVE_TYPE (uint8);
CINIT_SAVE_TYPE (int16);
CINIT_SAVE_TYPE (uint16);
CINIT_SAVE_TYPE (int32);
CINIT_SAVE_TYPE (uint32);
CINIT_SAVE_TYPE (int64);
CINIT_SAVE_TYPE (uint64);
CINIT_SAVE_TYPE (enum);
CINIT_SAVE_TYPE (float);
CINIT_SAVE_TYPE (double);
CINIT_SAVE_TYPE (long_double_t);
CINIT_SAVE_TYPE (bitfield);
CINIT_SAVE_TYPE (bitmask, , MR_BITMASK_OR_DELIMITER);

#define ESC_CHAR_MAP_SIZE (1 << 8)
static int map[ESC_CHAR_MAP_SIZE] = {
  [0 ... ESC_CHAR_MAP_SIZE - 1] = -1,
  [(unsigned char)'\f'] = (unsigned char)'f',
  [(unsigned char)'\n'] = (unsigned char)'n',
  [(unsigned char)'\r'] = (unsigned char)'r',
  [(unsigned char)'\t'] = (unsigned char)'t',
  [(unsigned char)'\v'] = (unsigned char)'v',
  [(unsigned char)'\\'] = (unsigned char)'\\',
};

/**
 * Quote string.
 * @param str string pointer
 * @return quoted string
 */
static char *
cinit_quote_string (char * str, char quote)
{
  int length = 0;
  char * str_;
  char * ptr;
  
  for (ptr = str; *ptr; ++ptr)
    {
      if ((quote == *ptr) || (map[(unsigned char)*ptr] >= 0))
	length += 2;
      else if (isprint (*ptr))
	length += 1;
      else
	length += 4;
    }
  str_ = MR_MALLOC (length + 3);
  if (NULL == str_)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (NULL);
    }
  length = 0;
  str_[length++] = quote;
  for (ptr = str; *ptr; ++ptr)
    {
      if (quote == *ptr)
	{
	  str_[length++] = '\\';
	  str_[length++] = *ptr;
	}
      else if (map[(unsigned char)*ptr] >= 0)
	{
	  str_[length++] = '\\';
	  str_[length++] = map[(unsigned char)*ptr];
	}      
      else if (isprint (*ptr))
	str_[length++] = *ptr;
      else
	{
	  str_[length++] = '\\';
	  sprintf (&str_[length], "%03o", (int)(unsigned char)*ptr);
	  length += 3;
	}
    }
  str_[length++] = quote;
  str_[length++] = 0;
  return (str_);
}

/**
 * MR_CHAR type saving handler. Stringify char.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors 
 * @return stringified float value
 */
static int
cinit_save_char (int idx, mr_ra_mr_ptrdes_t * ptrs, mr_save_type_data_t * data)
{
  char str[] = " ";
  str[0] = *(char*)ptrs->ra.data[idx].data;
  if (0 == str[0])
    data->content = MR_STRDUP ("'\\000'");
  else
    data->content = cinit_quote_string (str, '\'');
  return (0);
}

/**
 * MR_CHAR_ARRAY type saving handler. Save char array.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors 
 * @return char array value
 */
static int
cinit_save_char_array (int idx, mr_ra_mr_ptrdes_t * ptrs, mr_save_type_data_t * data)
{
  data->content = cinit_quote_string (ptrs->ra.data[idx].data, '"');
  return (0);
}

/**
 * MR_STRING type saving handler. Save string.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors 
 * @return string value
 */
static int
cinit_save_string (int idx, mr_ra_mr_ptrdes_t * ptrs, mr_save_type_data_t * data)
{
  char * str = *(char**)ptrs->ra.data[idx].data;
  if ((ptrs->ra.data[idx].flags & MR_PDF_IS_NULL) || (ptrs->ra.data[idx].ref_idx >= 0))
    data->content = MR_STRDUP (MR_CINIT_NULL);
  else
    data->content = cinit_quote_string (str, '"');
  return (0);
}

static int
cinit_save_struct (int idx, mr_ra_mr_ptrdes_t * ptrs, mr_save_type_data_t * data)
{
  data->prefix = "{\n";
  data->suffix = "}";
  return (0);
}

static int
cinit_save_anon_union (int idx, mr_ra_mr_ptrdes_t * ptrs, mr_save_type_data_t * data)
{
  data->prefix = "\"\", {\n";
  data->suffix = "}";
  return (0);
}

static int
cinit_save_rarray_data (int idx, mr_ra_mr_ptrdes_t * ptrs, mr_save_type_data_t * data)
{
  if ((ptrs->ra.data[idx].flags & MR_PDF_IS_NULL) || (ptrs->ra.data[idx].ref_idx >= 0))
    data->content = MR_STRDUP (MR_CINIT_NULL);
  else
    {
      data->prefix = "(" MR_CINIT_TYPE_NAME_TEMPLATE "[]){\n";
      data->suffix = "}";
    }
  return (0);
}

/**
 * MR_POINTER type saving handler. Save pointer as a string.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors
 * @return stringified pointer
 */
static int
cinit_save_pointer (int idx, mr_ra_mr_ptrdes_t * ptrs, mr_save_type_data_t * data)
{
  if (ptrs->ra.data[idx].first_child < 0)
    data->content = MR_STRDUP (MR_CINIT_NULL);
  else if ((MR_TYPE_CHAR_ARRAY == ptrs->ra.data[idx].fd.mr_type) && (0 == strcmp ("string_t", ptrs->ra.data[idx].fd.type)))
    {
      data->prefix = "(" MR_CINIT_TYPE_NAME_TEMPLATE "){\n";
      data->suffix = "}";
    }    
  else
    {
      data->prefix = "(" MR_CINIT_TYPE_NAME_TEMPLATE "[]){\n";
      data->suffix = "}";
    }
  return (0);
}

static int
json_save_array (int idx, mr_ra_mr_ptrdes_t * ptrs, mr_save_type_data_t * data)
{
  data->prefix = "[\n";
  data->suffix = "]";
  return (0);
}

static int
json_save_rarray_data (int idx, mr_ra_mr_ptrdes_t * ptrs, mr_save_type_data_t * data)
{
  if ((ptrs->ra.data[idx].flags & MR_PDF_IS_NULL) || (ptrs->ra.data[idx].ref_idx >= 0))
    data->content = MR_STRDUP (MR_CINIT_NULL);
  else
    {
      data->prefix = "[\n";
      data->suffix = "]";
    }
  return (0);
}

static int
json_save_pointer (int idx, mr_ra_mr_ptrdes_t * ptrs, mr_save_type_data_t * data)
{
  if (ptrs->ra.data[idx].first_child < 0)
    data->content = MR_STRDUP (MR_CINIT_NULL);
  else
    {
      data->prefix = "/* (" MR_JSON_TYPE_NAME_TEMPLATE "[]) */ [\n";
      data->suffix = "]";
    }
  return (0);
}

/**
 * Init IO handlers Table
 */
static void __attribute__((constructor)) mr_init_save_cinit (void)
{
  mr_conf.io_handlers[MR_TYPE_NONE].save.cinit = cinit_save_none;
  mr_conf.io_handlers[MR_TYPE_VOID].save.cinit = cinit_save_none;
  mr_conf.io_handlers[MR_TYPE_ENUM].save.cinit = cinit_save_enum;
  mr_conf.io_handlers[MR_TYPE_BITFIELD].save.cinit = cinit_save_bitfield;
  mr_conf.io_handlers[MR_TYPE_BITMASK].save.cinit = cinit_save_bitmask;
  mr_conf.io_handlers[MR_TYPE_INT8].save.cinit = cinit_save_int8;
  mr_conf.io_handlers[MR_TYPE_UINT8].save.cinit = cinit_save_uint8;
  mr_conf.io_handlers[MR_TYPE_INT16].save.cinit = cinit_save_int16;
  mr_conf.io_handlers[MR_TYPE_UINT16].save.cinit = cinit_save_uint16;
  mr_conf.io_handlers[MR_TYPE_INT32].save.cinit = cinit_save_int32;
  mr_conf.io_handlers[MR_TYPE_UINT32].save.cinit = cinit_save_uint32;
  mr_conf.io_handlers[MR_TYPE_INT64].save.cinit = cinit_save_int64;
  mr_conf.io_handlers[MR_TYPE_UINT64].save.cinit = cinit_save_uint64;
  mr_conf.io_handlers[MR_TYPE_FLOAT].save.cinit = cinit_save_float;
  mr_conf.io_handlers[MR_TYPE_DOUBLE].save.cinit = cinit_save_double;
  mr_conf.io_handlers[MR_TYPE_LONG_DOUBLE].save.cinit = cinit_save_long_double_t;
  mr_conf.io_handlers[MR_TYPE_CHAR].save.cinit = cinit_save_char;
  mr_conf.io_handlers[MR_TYPE_CHAR_ARRAY].save.cinit = cinit_save_char_array;
  mr_conf.io_handlers[MR_TYPE_STRING].save.cinit = cinit_save_string;
  mr_conf.io_handlers[MR_TYPE_STRUCT].save.cinit = cinit_save_struct;
  mr_conf.io_handlers[MR_TYPE_FUNC].save.cinit = cinit_save_none;
  mr_conf.io_handlers[MR_TYPE_FUNC_TYPE].save.cinit = cinit_save_none;
  mr_conf.io_handlers[MR_TYPE_UNION].save.cinit = cinit_save_struct;
  mr_conf.io_handlers[MR_TYPE_ANON_UNION].save.cinit = cinit_save_anon_union;

  mr_conf.io_ext_handlers[MR_TYPE_EXT_ARRAY].save.cinit = cinit_save_struct;
  mr_conf.io_ext_handlers[MR_TYPE_EXT_RARRAY_DATA].save.cinit = cinit_save_rarray_data;
  mr_conf.io_ext_handlers[MR_TYPE_EXT_POINTER].save.cinit = cinit_save_pointer;
}

static void __attribute__((constructor)) mr_init_save_json (void)
{
  mr_conf.io_handlers[MR_TYPE_NONE].save.json = cinit_save_none;
  mr_conf.io_handlers[MR_TYPE_VOID].save.json = cinit_save_none;
  mr_conf.io_handlers[MR_TYPE_ENUM].save.json = cinit_save_enum;
  mr_conf.io_handlers[MR_TYPE_BITFIELD].save.json = cinit_save_bitfield;
  mr_conf.io_handlers[MR_TYPE_BITMASK].save.json = cinit_save_bitmask;
  mr_conf.io_handlers[MR_TYPE_INT8].save.json = cinit_save_int8;
  mr_conf.io_handlers[MR_TYPE_UINT8].save.json = cinit_save_uint8;
  mr_conf.io_handlers[MR_TYPE_INT16].save.json = cinit_save_int16;
  mr_conf.io_handlers[MR_TYPE_UINT16].save.json = cinit_save_uint16;
  mr_conf.io_handlers[MR_TYPE_INT32].save.json = cinit_save_int32;
  mr_conf.io_handlers[MR_TYPE_UINT32].save.json = cinit_save_uint32;
  mr_conf.io_handlers[MR_TYPE_INT64].save.json = cinit_save_int64;
  mr_conf.io_handlers[MR_TYPE_UINT64].save.json = cinit_save_uint64;
  mr_conf.io_handlers[MR_TYPE_FLOAT].save.json = cinit_save_float;
  mr_conf.io_handlers[MR_TYPE_DOUBLE].save.json = cinit_save_double;
  mr_conf.io_handlers[MR_TYPE_LONG_DOUBLE].save.json = cinit_save_long_double_t;
  mr_conf.io_handlers[MR_TYPE_CHAR].save.json = cinit_save_char;
  mr_conf.io_handlers[MR_TYPE_CHAR_ARRAY].save.json = cinit_save_char_array;
  mr_conf.io_handlers[MR_TYPE_STRING].save.json = cinit_save_string;
  mr_conf.io_handlers[MR_TYPE_STRUCT].save.json = cinit_save_struct;
  mr_conf.io_handlers[MR_TYPE_FUNC].save.json = cinit_save_none;
  mr_conf.io_handlers[MR_TYPE_FUNC_TYPE].save.json = cinit_save_none;
  mr_conf.io_handlers[MR_TYPE_UNION].save.json = cinit_save_struct;
  mr_conf.io_handlers[MR_TYPE_ANON_UNION].save.json = cinit_save_anon_union;

  mr_conf.io_ext_handlers[MR_TYPE_EXT_ARRAY].save.json = json_save_array;
  mr_conf.io_ext_handlers[MR_TYPE_EXT_RARRAY_DATA].save.json = json_save_rarray_data;
  mr_conf.io_ext_handlers[MR_TYPE_EXT_POINTER].save.json = json_save_pointer;
}
