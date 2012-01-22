/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

#include <stdio.h>
#include <stdarg.h>
#include <reslib.h>

#define RL_CINIT_NULL "NULL"
#define RL_CINIT_INDENT_SPACES (2)
#define RL_CINIT_INDENT_TEMPLATE "%*s"
#define RL_CINIT_INDENT "  "

#define RL_CINIT_TYPE_SIZE_TEMPLATE "%1$" SCNd32
#define RL_CINIT_TYPE_NAME_TEMPLATE "%2$s"
#define RL_JSON_TYPE_NAME_TEMPLATE RL_CINIT_TYPE_NAME_TEMPLATE
#define RL_CINIT_FIELDS_DELIMITER ",\n"
#define RL_CINIT_NAMED_FIELD_TEMPLATE ".%s = "
#define RL_JSON_NAMED_FIELD_TEMPLATE "\"%s\" : "
#define RL_CINIT_ATTR_INT "/* %s = %" SCNd32 " */ "

#define RL_CINIT_UNNAMED_FIELDS (0)
#define RL_CINIT_NAMED_FIELDS (!RL_CINIT_UNNAMED_FIELDS)

static int cinit_named_node[RL_MAX_TYPES] = {
  [0 ... RL_MAX_TYPES - 1] = RL_CINIT_UNNAMED_FIELDS,
  [RL_TYPE_STRUCT] = RL_CINIT_NAMED_FIELDS,
  [RL_TYPE_UNION] = RL_CINIT_NAMED_FIELDS,
  [RL_TYPE_ANON_UNION] = RL_CINIT_NAMED_FIELDS,
};

/**
 * Public function. Save scheduler. Save any object as a string.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors
 * @return stringified representation of object
 */
static char *
cinit_json_save (rl_ra_rl_ptrdes_t * ptrs, char * named_field_template, int (*node_handler) (rl_fd_t*, int, rl_ra_rl_ptrdes_t*, rl_save_type_data_t*))
{
  rl_rarray_t rl_ra_str = { .data = RL_STRDUP (""), .size = sizeof (""), .alloc_size = sizeof (""), .ext = { NULL }, };
  int idx = 0;

  if (NULL == rl_ra_str.data)
    return (NULL);
  
  while (idx >= 0)
    {
      int level = 0;
      int skip_node;
      rl_fd_t * fdp = &ptrs->ra.data[idx].fd;
      rl_save_type_data_t save_data = { .prefix = NULL, .content = NULL, .suffix = NULL, };

      /* route saving handler */
      skip_node = node_handler (fdp, idx, ptrs, &save_data);
      if (!skip_node)
	{
	  int named_node = RL_CINIT_UNNAMED_FIELDS;
	  level = RL_LIMIT_LEVEL (ptrs->ra.data[idx].level);
	  if (ptrs->ra.data[idx].level > 0)
	    if (rl_ra_printf (&rl_ra_str, RL_CINIT_INDENT_TEMPLATE, level * RL_CINIT_INDENT_SPACES, "") < 0)
	      return (NULL);
	  if ((ptrs->ra.data[idx].parent >= 0) && (RL_TYPE_EXT_NONE == ptrs->ra.data[ptrs->ra.data[idx].parent].fd.rl_type_ext)
	      && strcmp ("rl_ptr_t", ptrs->ra.data[ptrs->ra.data[idx].parent].fd.type)) /* ugly hack for synthetic type. rl_ptr_t members should be unnamed */
	    named_node = cinit_named_node[ptrs->ra.data[ptrs->ra.data[idx].parent].fd.rl_type];
	  if (RL_CINIT_NAMED_FIELDS == named_node)
	    if (rl_ra_printf (&rl_ra_str, named_field_template, ptrs->ra.data[idx].fd.name) < 0)
	      return (NULL);
	  if (ptrs->ra.data[idx].ref_idx >= 0)
	    if (rl_ra_printf (&rl_ra_str, RL_CINIT_ATTR_INT,
			      (ptrs->ra.data[idx].flags & RL_PDF_CONTENT_REFERENCE) ? RL_REF_CONTENT : RL_REF,
			      ptrs->ra.data[ptrs->ra.data[idx].ref_idx].idx) < 0)
	      return (NULL);
	  if (ptrs->ra.data[idx].flags & RL_PDF_IS_REFERENCED)
	    if (rl_ra_printf (&rl_ra_str, RL_CINIT_ATTR_INT, RL_REF_IDX, ptrs->ra.data[idx].idx) < 0)
	      return (NULL);
	  if (save_data.prefix)
	    if (rl_ra_printf (&rl_ra_str, save_data.prefix, ptrs->ra.data[idx].fd.size, ptrs->ra.data[idx].fd.type) < 0)
	      return (NULL);
	  if (save_data.content)
	    {
	      if (rl_ra_printf (&rl_ra_str, "%s", save_data.content) < 0)
		return (NULL);
	      RL_FREE (save_data.content);
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
	      level = RL_LIMIT_LEVEL (ptrs->ra.data[idx].level);
	      if (rl_ra_printf (&rl_ra_str, RL_CINIT_INDENT_TEMPLATE, level * RL_CINIT_INDENT_SPACES, "") < 0)
		return (NULL);
	      if (rl_ra_printf (&rl_ra_str, "%s", save_data.suffix) < 0)
		return (NULL);
	    }
	  if (!skip_node && (idx != 0))
	    if (rl_ra_printf (&rl_ra_str, RL_CINIT_FIELDS_DELIMITER) < 0)
	      return (NULL);
	  while ((ptrs->ra.data[idx].next < 0) && (ptrs->ra.data[idx].parent >= 0))
	    {
	      idx = ptrs->ra.data[idx].parent;
	      if (ptrs->ra.data[idx].ext.ptr)
		{
		  level = RL_LIMIT_LEVEL (ptrs->ra.data[idx].level);
		  if (rl_ra_printf (&rl_ra_str, RL_CINIT_INDENT_TEMPLATE, level * RL_CINIT_INDENT_SPACES, "") < 0)
		    return (NULL);
		  if (rl_ra_printf (&rl_ra_str, "%s", (char*)ptrs->ra.data[idx].ext.ptr) < 0)
		    return (NULL);
		  if (idx != 0)
		    if (rl_ra_printf (&rl_ra_str, RL_CINIT_FIELDS_DELIMITER) < 0)
		      return (NULL);
		}
	    }
	  idx = ptrs->ra.data[idx].next;
	}
    }
  
  return (rl_ra_str.data);
}

char *
cinit_save (rl_ra_rl_ptrdes_t * _ptrs_)
{
  int node_handler (rl_fd_t * fdp, int idx, rl_ra_rl_ptrdes_t * ptrs, rl_save_type_data_t * save_data)
  {
    int skip_node = 0;
    if ((fdp->rl_type_ext >= 0) && (fdp->rl_type_ext < RL_MAX_TYPES) && rl_conf.io_ext_handlers[fdp->rl_type_ext].save.cinit)
      skip_node = rl_conf.io_ext_handlers[fdp->rl_type_ext].save.cinit (idx, ptrs, save_data);
    else if ((fdp->rl_type >= 0) && (fdp->rl_type < RL_MAX_TYPES) && rl_conf.io_handlers[fdp->rl_type].save.cinit)
      skip_node = rl_conf.io_handlers[fdp->rl_type].save.cinit (idx, ptrs, save_data);
    else
      RL_MESSAGE_UNSUPPORTED_NODE_TYPE_ (fdp);
    return (skip_node);
  }
  
  return (cinit_json_save (_ptrs_, RL_CINIT_NAMED_FIELD_TEMPLATE, node_handler));
}

char *
json_save (rl_ra_rl_ptrdes_t * _ptrs_)
{
  int node_handler (rl_fd_t * fdp, int idx, rl_ra_rl_ptrdes_t * ptrs, rl_save_type_data_t * save_data)
  {
    int skip_node = 0;
    if ((fdp->rl_type_ext >= 0) && (fdp->rl_type_ext < RL_MAX_TYPES) && rl_conf.io_ext_handlers[fdp->rl_type_ext].save.json)
      skip_node = rl_conf.io_ext_handlers[fdp->rl_type_ext].save.json (idx, ptrs, save_data);
    else if ((fdp->rl_type >= 0) && (fdp->rl_type < RL_MAX_TYPES) && rl_conf.io_handlers[fdp->rl_type].save.json)
      skip_node = rl_conf.io_handlers[fdp->rl_type].save.json (idx, ptrs, save_data);
    else
      RL_MESSAGE_UNSUPPORTED_NODE_TYPE_ (fdp);
    return (skip_node);
  }
  
  return (cinit_json_save (_ptrs_, RL_JSON_NAMED_FIELD_TEMPLATE, node_handler));
}

/**
 * RL_NONE type saving handler.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors
 * @return stringified int value
 */
static int
cinit_save_none (int idx, rl_ra_rl_ptrdes_t * ptrs, rl_save_type_data_t * data)
{
  return (!0);
}

/**
 * RL_XXX type saving handler. Make a string from *(XXX_t*)data.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors 
 */
#define CINIT_SAVE_TYPE(TYPE, EXT...) static int cinit_save_ ## TYPE (int idx, rl_ra_rl_ptrdes_t * ptrs, rl_save_type_data_t * data) { data->content = rl_stringify_ ## TYPE (&ptrs->ra.data[idx] EXT); return (0); }

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
CINIT_SAVE_TYPE (bitmask, , RL_BITMASK_OR_DELIMITER);

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
  str_ = RL_MALLOC (length + 3);
  if (NULL == str_)
    {
      RL_MESSAGE (RL_LL_FATAL, RL_MESSAGE_OUT_OF_MEMORY);
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
 * RL_CHAR type saving handler. Stringify char.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors 
 * @return stringified float value
 */
static int
cinit_save_char (int idx, rl_ra_rl_ptrdes_t * ptrs, rl_save_type_data_t * data)
{
  char str[] = " ";
  str[0] = *(char*)ptrs->ra.data[idx].data;
  if (0 == str[0])
    data->content = RL_STRDUP ("'\\000'");
  else
    data->content = cinit_quote_string (str, '\'');
  return (0);
}

/**
 * RL_CHAR_ARRAY type saving handler. Save char array.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors 
 * @return char array value
 */
static int
cinit_save_char_array (int idx, rl_ra_rl_ptrdes_t * ptrs, rl_save_type_data_t * data)
{
  data->content = cinit_quote_string (ptrs->ra.data[idx].data, '"');
  return (0);
}

/**
 * RL_STRING type saving handler. Save string.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors 
 * @return string value
 */
static int
cinit_save_string (int idx, rl_ra_rl_ptrdes_t * ptrs, rl_save_type_data_t * data)
{
  char * str = *(char**)ptrs->ra.data[idx].data;
  if ((ptrs->ra.data[idx].flags & RL_PDF_IS_NULL) || (ptrs->ra.data[idx].ref_idx >= 0))
    data->content = RL_STRDUP (RL_CINIT_NULL);
  else
    data->content = cinit_quote_string (str, '"');
  return (0);
}

static int
cinit_save_struct (int idx, rl_ra_rl_ptrdes_t * ptrs, rl_save_type_data_t * data)
{
  data->prefix = "{\n";
  data->suffix = "}";
  return (0);
}

static int
cinit_save_anon_union (int idx, rl_ra_rl_ptrdes_t * ptrs, rl_save_type_data_t * data)
{
  data->prefix = "\"\", {\n";
  data->suffix = "}";
  return (0);
}

static int
cinit_save_rarray_data (int idx, rl_ra_rl_ptrdes_t * ptrs, rl_save_type_data_t * data)
{
  if ((ptrs->ra.data[idx].flags & RL_PDF_IS_NULL) || (ptrs->ra.data[idx].ref_idx >= 0))
    data->content = RL_STRDUP (RL_CINIT_NULL);
  else
    {
      data->prefix = "(" RL_CINIT_TYPE_NAME_TEMPLATE "[]){\n";
      data->suffix = "}";
    }
  return (0);
}

/**
 * RL_POINTER type saving handler. Save pointer as a string.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors
 * @return stringified pointer
 */
static int
cinit_save_pointer (int idx, rl_ra_rl_ptrdes_t * ptrs, rl_save_type_data_t * data)
{
  if (ptrs->ra.data[idx].first_child < 0)
    data->content = RL_STRDUP (RL_CINIT_NULL);
  else if (0 == strcmp ("string_t", ptrs->ra.data[idx].fd.type))
    {
      data->prefix = "(" RL_CINIT_TYPE_NAME_TEMPLATE "){\n";
      data->suffix = "}";
    }    
  else
    {
      data->prefix = "(" RL_CINIT_TYPE_NAME_TEMPLATE "[]){\n";
      data->suffix = "}";
    }
  return (0);
}

static int
json_save_array (int idx, rl_ra_rl_ptrdes_t * ptrs, rl_save_type_data_t * data)
{
  data->prefix = "[\n";
  data->suffix = "]";
  return (0);
}

static int
json_save_rarray_data (int idx, rl_ra_rl_ptrdes_t * ptrs, rl_save_type_data_t * data)
{
  if ((ptrs->ra.data[idx].flags & RL_PDF_IS_NULL) || (ptrs->ra.data[idx].ref_idx >= 0))
    data->content = RL_STRDUP (RL_CINIT_NULL);
  else
    {
      data->prefix = "[\n";
      data->suffix = "]";
    }
  return (0);
}

static int
json_save_pointer (int idx, rl_ra_rl_ptrdes_t * ptrs, rl_save_type_data_t * data)
{
  if (ptrs->ra.data[idx].first_child < 0)
    data->content = RL_STRDUP (RL_CINIT_NULL);
  else
    {
      data->prefix = "/* (" RL_JSON_TYPE_NAME_TEMPLATE "[]) */ [\n";
      data->suffix = "]";
    }
  return (0);
}

/**
 * Init IO handlers Table
 */
static void __attribute__((constructor)) rl_init_save_cinit (void)
{
  rl_conf.io_handlers[RL_TYPE_NONE].save.cinit = cinit_save_none;
  rl_conf.io_handlers[RL_TYPE_VOID].save.cinit = cinit_save_none;
  rl_conf.io_handlers[RL_TYPE_ENUM].save.cinit = cinit_save_enum;
  rl_conf.io_handlers[RL_TYPE_BITFIELD].save.cinit = cinit_save_bitfield;
  rl_conf.io_handlers[RL_TYPE_BITMASK].save.cinit = cinit_save_bitmask;
  rl_conf.io_handlers[RL_TYPE_INT8].save.cinit = cinit_save_int8;
  rl_conf.io_handlers[RL_TYPE_UINT8].save.cinit = cinit_save_uint8;
  rl_conf.io_handlers[RL_TYPE_INT16].save.cinit = cinit_save_int16;
  rl_conf.io_handlers[RL_TYPE_UINT16].save.cinit = cinit_save_uint16;
  rl_conf.io_handlers[RL_TYPE_INT32].save.cinit = cinit_save_int32;
  rl_conf.io_handlers[RL_TYPE_UINT32].save.cinit = cinit_save_uint32;
  rl_conf.io_handlers[RL_TYPE_INT64].save.cinit = cinit_save_int64;
  rl_conf.io_handlers[RL_TYPE_UINT64].save.cinit = cinit_save_uint64;
  rl_conf.io_handlers[RL_TYPE_FLOAT].save.cinit = cinit_save_float;
  rl_conf.io_handlers[RL_TYPE_DOUBLE].save.cinit = cinit_save_double;
  rl_conf.io_handlers[RL_TYPE_LONG_DOUBLE].save.cinit = cinit_save_long_double_t;
  rl_conf.io_handlers[RL_TYPE_CHAR].save.cinit = cinit_save_char;
  rl_conf.io_handlers[RL_TYPE_CHAR_ARRAY].save.cinit = cinit_save_char_array;
  rl_conf.io_handlers[RL_TYPE_STRING].save.cinit = cinit_save_string;
  rl_conf.io_handlers[RL_TYPE_STRUCT].save.cinit = cinit_save_struct;
  rl_conf.io_handlers[RL_TYPE_FUNC].save.cinit = cinit_save_none;
  rl_conf.io_handlers[RL_TYPE_FUNC_TYPE].save.cinit = cinit_save_none;
  rl_conf.io_handlers[RL_TYPE_UNION].save.cinit = cinit_save_struct;
  rl_conf.io_handlers[RL_TYPE_ANON_UNION].save.cinit = cinit_save_anon_union;

  rl_conf.io_ext_handlers[RL_TYPE_EXT_ARRAY].save.cinit = cinit_save_struct;
  rl_conf.io_ext_handlers[RL_TYPE_EXT_RARRAY_DATA].save.cinit = cinit_save_rarray_data;
  rl_conf.io_ext_handlers[RL_TYPE_EXT_POINTER].save.cinit = cinit_save_pointer;
}

static void __attribute__((constructor)) rl_init_save_json (void)
{
  rl_conf.io_handlers[RL_TYPE_NONE].save.json = cinit_save_none;
  rl_conf.io_handlers[RL_TYPE_VOID].save.json = cinit_save_none;
  rl_conf.io_handlers[RL_TYPE_ENUM].save.json = cinit_save_enum;
  rl_conf.io_handlers[RL_TYPE_BITFIELD].save.json = cinit_save_bitfield;
  rl_conf.io_handlers[RL_TYPE_BITMASK].save.json = cinit_save_bitmask;
  rl_conf.io_handlers[RL_TYPE_INT8].save.json = cinit_save_int8;
  rl_conf.io_handlers[RL_TYPE_UINT8].save.json = cinit_save_uint8;
  rl_conf.io_handlers[RL_TYPE_INT16].save.json = cinit_save_int16;
  rl_conf.io_handlers[RL_TYPE_UINT16].save.json = cinit_save_uint16;
  rl_conf.io_handlers[RL_TYPE_INT32].save.json = cinit_save_int32;
  rl_conf.io_handlers[RL_TYPE_UINT32].save.json = cinit_save_uint32;
  rl_conf.io_handlers[RL_TYPE_INT64].save.json = cinit_save_int64;
  rl_conf.io_handlers[RL_TYPE_UINT64].save.json = cinit_save_uint64;
  rl_conf.io_handlers[RL_TYPE_FLOAT].save.json = cinit_save_float;
  rl_conf.io_handlers[RL_TYPE_DOUBLE].save.json = cinit_save_double;
  rl_conf.io_handlers[RL_TYPE_LONG_DOUBLE].save.json = cinit_save_long_double_t;
  rl_conf.io_handlers[RL_TYPE_CHAR].save.json = cinit_save_char;
  rl_conf.io_handlers[RL_TYPE_CHAR_ARRAY].save.json = cinit_save_char_array;
  rl_conf.io_handlers[RL_TYPE_STRING].save.json = cinit_save_string;
  rl_conf.io_handlers[RL_TYPE_STRUCT].save.json = cinit_save_struct;
  rl_conf.io_handlers[RL_TYPE_FUNC].save.json = cinit_save_none;
  rl_conf.io_handlers[RL_TYPE_FUNC_TYPE].save.json = cinit_save_none;
  rl_conf.io_handlers[RL_TYPE_UNION].save.json = cinit_save_struct;
  rl_conf.io_handlers[RL_TYPE_ANON_UNION].save.json = cinit_save_anon_union;

  rl_conf.io_ext_handlers[RL_TYPE_EXT_ARRAY].save.json = json_save_array;
  rl_conf.io_ext_handlers[RL_TYPE_EXT_RARRAY_DATA].save.json = json_save_rarray_data;
  rl_conf.io_ext_handlers[RL_TYPE_EXT_POINTER].save.json = json_save_pointer;
}
