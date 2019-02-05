/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#include <metaresc.h>
#include <mr_stringify.h>

#define MR_CINIT_NULL "NULL"
#define MR_JSON_NULL "null"
#define MR_CINIT_INDENT_SPACES (2)
#define MR_CINIT_INDENT_TEMPLATE "%*s"

#define MR_CINIT_FIELDS_DELIMITER ",\n"
#define MR_CINIT_NAMED_FIELD_TEMPLATE ".%s = "
#define MR_JSON_NAMED_FIELD_TEMPLATE "\"%s\" : "
#define MR_CINIT_ATTR_INT "/* %s = %" SCNd32 " */ "

#define CINIT_QUOTE_CHAR_PATTERN "\\%03o"

static int
cinit_printf_char (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  unsigned char c = *(unsigned char*)ptrdes->data.ptr;
  char mapped = mr_esc_char_map[c];
  int count = 0;

  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '\''));
  if (c == '\'')
    mapped = c;
  if (mapped)
    {
      count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '\\'));
      count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, mapped));
    }
  else if (isprint (c))
    count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, c));
  else
    count += TRY_CATCH_THROW (mr_ra_printf (mr_ra_str, CINIT_QUOTE_CHAR_PATTERN, c));
  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '\''));
  return (count);
}

static int
cinit_printf_complex_float (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf_complex_float (mr_ra_str, ptrdes, " + "));
}

static int
cinit_printf_complex_double (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf_complex_double (mr_ra_str, ptrdes, " + "));
}

static int
cinit_printf_complex_long_double_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf_complex_long_double_t (mr_ra_str, ptrdes, " + "));
}

static int
cinit_printf_char_array (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf_quote_string (mr_ra_str, ptrdes->data.ptr, CINIT_QUOTE_CHAR_PATTERN));
}

static int
cinit_printf_string (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf_quote_string (mr_ra_str, *(char**)ptrdes->data.ptr, CINIT_QUOTE_CHAR_PATTERN));
}

static int
cinit_printf_struct (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  ptrdes->res.data.string = "}";
  ptrdes->res.type = "char";
  ptrdes->res.MR_SIZE = 0;
  return (mr_ra_printf (mr_ra_str, "{\n"));
}

static int
cinit_printf_func (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  const char * func_str = mr_serialize_func (*(void**)ptrdes->data.ptr);

  if (isdigit (func_str[0])) /* pointer serialized as int */
    {
      char * type = (MR_TYPE_FUNC == ptrdes->fd.mr_type) ? MR_VOIDP_T_STR : ptrdes->fd.type;
      return (mr_ra_printf (mr_ra_str, "(%s)%s", type, func_str));
    }
  else
    return (mr_ra_printf (mr_ra_str, "%s", func_str));
}

static int
cinit_printf_pointer (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  ptrdes->res.data.string = "}";
  ptrdes->res.type = "char";
  ptrdes->res.MR_SIZE = 0;
  return (mr_ra_printf (mr_ra_str, "(%s[]){\n", ptrdes->fd.type));
}

static int
cinit_printf_anon_union (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  ptrdes->res.data.string = "}";
  ptrdes->res.type = "char";
  ptrdes->res.MR_SIZE = 0;
  return (mr_ra_printf (mr_ra_str, "\"\", {\n"));
}

static mr_ra_printf_t cinit_save_tbl[] = {
  [MR_TYPE_NONE] = mr_ra_printf_void,
  [MR_TYPE_VOID] = mr_ra_printf_void,
  [MR_TYPE_ENUM] = mr_ra_printf_enum,
  [MR_TYPE_BITFIELD] = mr_ra_printf_bitfield,
  [MR_TYPE_BITMASK] = mr_ra_printf_enum,
  [MR_TYPE_BOOL] = mr_ra_printf_bool,
  [MR_TYPE_INT8] = mr_ra_printf_int8_t,
  [MR_TYPE_UINT8] = mr_ra_printf_uint8_t,
  [MR_TYPE_INT16] = mr_ra_printf_int16_t,
  [MR_TYPE_UINT16] = mr_ra_printf_uint16_t,
  [MR_TYPE_INT32] = mr_ra_printf_int32_t,
  [MR_TYPE_UINT32] = mr_ra_printf_uint32_t,
  [MR_TYPE_INT64] = mr_ra_printf_int64_t,
  [MR_TYPE_UINT64] = mr_ra_printf_uint64_t,
  [MR_TYPE_FLOAT] = mr_ra_printf_float,
  [MR_TYPE_COMPLEX_FLOAT] = cinit_printf_complex_float,
  [MR_TYPE_DOUBLE] = mr_ra_printf_double,
  [MR_TYPE_COMPLEX_DOUBLE] = cinit_printf_complex_double,
  [MR_TYPE_LONG_DOUBLE] = mr_ra_printf_long_double_t,
  [MR_TYPE_COMPLEX_LONG_DOUBLE] = cinit_printf_complex_long_double_t,
  [MR_TYPE_CHAR] = cinit_printf_char,
  [MR_TYPE_CHAR_ARRAY] = cinit_printf_char_array,
  [MR_TYPE_STRING] = cinit_printf_string,
  [MR_TYPE_STRUCT] = cinit_printf_struct,
  [MR_TYPE_FUNC] = cinit_printf_func,
  [MR_TYPE_FUNC_TYPE] = cinit_printf_func,
  [MR_TYPE_ARRAY] = cinit_printf_struct,
  [MR_TYPE_POINTER] = cinit_printf_pointer,
  [MR_TYPE_UNION] = cinit_printf_struct,
  [MR_TYPE_ANON_UNION] = cinit_printf_struct,
  [MR_TYPE_NAMED_ANON_UNION] = cinit_printf_anon_union,
};

static int
json_printf_array (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  ptrdes->res.data.string = "]";
  ptrdes->res.type = "char";
  ptrdes->res.MR_SIZE = 0;
  return (mr_ra_printf (mr_ra_str, "[\n"));
}

static int
json_printf_pointer (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  ptrdes->res.data.string = "]";
  ptrdes->res.type = "char";
  ptrdes->res.MR_SIZE = 0;
  return (mr_ra_printf (mr_ra_str, "/* (%s[]) */ [\n", ptrdes->fd.type));
}

static mr_ra_printf_t json_save_tbl[] = {
  [MR_TYPE_NONE] = mr_ra_printf_void,
  [MR_TYPE_VOID] = mr_ra_printf_void,
  [MR_TYPE_ENUM] = mr_ra_printf_enum,
  [MR_TYPE_BITFIELD] = mr_ra_printf_bitfield,
  [MR_TYPE_BITMASK] = mr_ra_printf_enum,
  [MR_TYPE_BOOL] = mr_ra_printf_bool,
  [MR_TYPE_INT8] = mr_ra_printf_int8_t,
  [MR_TYPE_UINT8] = mr_ra_printf_uint8_t,
  [MR_TYPE_INT16] = mr_ra_printf_int16_t,
  [MR_TYPE_UINT16] = mr_ra_printf_uint16_t,
  [MR_TYPE_INT32] = mr_ra_printf_int32_t,
  [MR_TYPE_UINT32] = mr_ra_printf_uint32_t,
  [MR_TYPE_INT64] = mr_ra_printf_int64_t,
  [MR_TYPE_UINT64] = mr_ra_printf_uint64_t,
  [MR_TYPE_FLOAT] = mr_ra_printf_float,
  [MR_TYPE_COMPLEX_FLOAT] = cinit_printf_complex_float,
  [MR_TYPE_DOUBLE] = mr_ra_printf_double,
  [MR_TYPE_COMPLEX_DOUBLE] = cinit_printf_complex_double,
  [MR_TYPE_LONG_DOUBLE] = mr_ra_printf_long_double_t,
  [MR_TYPE_COMPLEX_LONG_DOUBLE] = cinit_printf_complex_long_double_t,
  [MR_TYPE_CHAR] = cinit_printf_char,
  [MR_TYPE_CHAR_ARRAY] = cinit_printf_char_array,
  [MR_TYPE_STRING] = cinit_printf_string,
  [MR_TYPE_STRUCT] = cinit_printf_struct,
  [MR_TYPE_FUNC] = cinit_printf_func,
  [MR_TYPE_FUNC_TYPE] = cinit_printf_func,
  [MR_TYPE_ARRAY] = json_printf_array,
  [MR_TYPE_POINTER] = json_printf_pointer,
  [MR_TYPE_UNION] = cinit_printf_struct,
  [MR_TYPE_ANON_UNION] = cinit_printf_struct,
  [MR_TYPE_NAMED_ANON_UNION] = cinit_printf_struct,
};

char *
_cinit_json_save (mr_ra_ptrdes_t * ptrs, mr_ra_printf_t * printf_tbl, char * named_field_tmplt, char * null)
{
  mr_rarray_t mr_ra_str = {
    .data = { mr_strdup ("") },
    .MR_SIZE = sizeof (""),
    .type = "char",
    .alloc_size = sizeof (""),
  };
  int idx = 0;

  if (NULL == mr_ra_str.data.string)
    return (NULL);

  while (idx >= 0)
    {
      int level = 0;
      mr_fd_t * fdp = &ptrs->ra[idx].fd;

      /* route saving handler */
      mr_ra_printf_t save_handler = mr_ra_printf_void;
      /* route saving handler */
      if ((fdp->mr_type < MR_TYPE_LAST) && printf_tbl[fdp->mr_type])
	save_handler = printf_tbl[fdp->mr_type];
      else
	MR_MESSAGE_UNSUPPORTED_NODE_TYPE_ (fdp);

      level = MR_LIMIT_LEVEL (ptrs->ra[idx].save_params.level);

      if (ptrs->ra[idx].save_params.level > 0)
	if (mr_ra_printf (&mr_ra_str, MR_CINIT_INDENT_TEMPLATE, level * MR_CINIT_INDENT_SPACES, "") < 0)
	  return (NULL);

      if (false == fdp->unnamed)
	if (mr_ra_printf (&mr_ra_str, named_field_tmplt, ptrs->ra[idx].fd.name.str) < 0)
	  return (NULL);

      if (ptrs->ra[idx].ref_idx >= 0)
	if (mr_ra_printf (&mr_ra_str, MR_CINIT_ATTR_INT,
			  (ptrs->ra[idx].flags.is_content_reference) ? MR_REF_CONTENT : MR_REF,
			  ptrs->ra[ptrs->ra[idx].ref_idx].idx) < 0)
	  return (NULL);

      if (ptrs->ra[idx].flags.is_referenced)
	if (mr_ra_printf (&mr_ra_str, MR_CINIT_ATTR_INT, MR_REF_IDX, ptrs->ra[idx].idx) < 0)
	  return (NULL);

      if ((true == ptrs->ra[idx].flags.is_null) || (ptrs->ra[idx].ref_idx >= 0))
	{
	  if (mr_ra_append_string (&mr_ra_str, null) < 0)
	    return (NULL);
	}
      else if (save_handler (&mr_ra_str, &ptrs->ra[idx]) < 0)
	return (NULL);

      if (ptrs->ra[idx].first_child >= 0)
	idx = ptrs->ra[idx].first_child;
      else
	{
	  if (idx != 0)
	    if (mr_ra_append_string (&mr_ra_str, MR_CINIT_FIELDS_DELIMITER) < 0)
	      return (NULL);
	  
	  while ((ptrs->ra[idx].next < 0) && (ptrs->ra[idx].parent >= 0))
	    {
	      idx = ptrs->ra[idx].parent;
	      if (ptrs->ra[idx].res.data.string)
		{
		  level = MR_LIMIT_LEVEL (ptrs->ra[idx].save_params.level);
		  if (mr_ra_printf (&mr_ra_str, MR_CINIT_INDENT_TEMPLATE, level * MR_CINIT_INDENT_SPACES + 1, ptrs->ra[idx].res.data.string) < 0)
		    return (NULL);
		  if (idx != 0)
		    if (mr_ra_append_string (&mr_ra_str, MR_CINIT_FIELDS_DELIMITER) < 0)
		      return (NULL);
		}
	    }
	  idx = ptrs->ra[idx].next;
	}
    }

  return (mr_ra_str.data.string);
}

char *
cinit_save (mr_ra_ptrdes_t * ptrs)
{
  return (_cinit_json_save (ptrs, cinit_save_tbl, MR_CINIT_NAMED_FIELD_TEMPLATE, MR_CINIT_NULL));
}

char *
json_save (mr_ra_ptrdes_t * ptrs)
{
  int i;
  for (i = ptrs->size / sizeof (ptrs->ra[0]) - 1; i >= 0; --i)
    if ((MR_TYPE_ANON_UNION == ptrs->ra[i].fd.mr_type) ||
	(MR_TYPE_POINTER == ptrs->ra[i].fd.mr_type))
      ptrs->ra[i].fd.unnamed = false;
  return (_cinit_json_save (ptrs, json_save_tbl, MR_JSON_NAMED_FIELD_TEMPLATE, MR_JSON_NULL));
}
