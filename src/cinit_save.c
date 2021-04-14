/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#include <metaresc.h>
#include <mr_stringify.h>

#define COMPLEX_REAL_IMAG_DELIMITER " + "
#define MR_CINIT_NULL "NULL"
#define MR_JSON_NULL "null"
#define MR_CINIT_INDENT_SPACES (2)
#define MR_CINIT_INDENT_TEMPLATE "%*s"

#define MR_CINIT_FIELDS_DELIMITER ","
#define MR_CINIT_NAMED_FIELD_TEMPLATE ".%s = "
#define MR_JSON_NAMED_FIELD_TEMPLATE "\"%s\" : "
#define MR_CINIT_ATTR_INT "/* %s = %" SCNd32 " */ "

#define CINIT_QUOTE_CHAR_PATTERN "\\%03o"
#define CINIT_BITMASK_DELIMITER " | "

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
  return (mr_ra_printf_complex_float (mr_ra_str, ptrdes, COMPLEX_REAL_IMAG_DELIMITER));
}

static int
cinit_printf_complex_double (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf_complex_double (mr_ra_str, ptrdes, COMPLEX_REAL_IMAG_DELIMITER));
}

static int
cinit_printf_complex_long_double_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf_complex_long_double_t (mr_ra_str, ptrdes, COMPLEX_REAL_IMAG_DELIMITER));
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
cinit_printf_bitmask (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf_bitmask (mr_ra_str, ptrdes, CINIT_BITMASK_DELIMITER));
}

static int
cinit_printf_struct (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  ptrdes->res.data.string = "}";
  ptrdes->res.type = "string";
  ptrdes->res.MR_SIZE = 0;
  return (mr_ra_append_string (mr_ra_str, "{\n"));
}

static int
cinit_printf_func (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  const char * func_str = mr_serialize_func (*(void**)ptrdes->data.ptr);

  if (func_str) /* pointer serialized as name */
    return (mr_ra_append_string (mr_ra_str, (char*)func_str));
  else
    {
      char * type = (MR_TYPE_FUNC == ptrdes->mr_type) ? MR_VOIDP_T_STR : ptrdes->type;
      return (mr_ra_printf (mr_ra_str, "(%s)%p", type, *(void**)ptrdes->data.ptr));
    }
}

static int
cinit_printf_pointer (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  ptrdes->res.data.string = "}";
  ptrdes->res.type = "string";
  ptrdes->res.MR_SIZE = 0;
  return (mr_ra_printf (mr_ra_str, "(%s[]){\n", ptrdes->type));
}

static int
cinit_printf_anon_union (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  ptrdes->res.data.string = "}";
  ptrdes->res.type = "string";
  ptrdes->res.MR_SIZE = 0;
  return (mr_ra_append_string (mr_ra_str, "\"\", {\n"));
}

static mr_ra_printf_t cinit_save_tbl[MR_TYPE_LAST] = {
  [MR_TYPE_NONE] = mr_ra_printf_void,
  [MR_TYPE_VOID] = mr_ra_printf_void,
  [MR_TYPE_ENUM] = cinit_printf_bitmask,
  [MR_TYPE_BITFIELD] = mr_ra_printf_bitfield,
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
  ptrdes->res.type = "string";
  ptrdes->res.MR_SIZE = 0;
  return (mr_ra_append_string (mr_ra_str, "[\n"));
}

static mr_ra_printf_t json_save_tbl[MR_TYPE_LAST] = {
  [MR_TYPE_NONE] = mr_ra_printf_void,
  [MR_TYPE_VOID] = mr_ra_printf_void,
  [MR_TYPE_ENUM] = cinit_printf_bitmask,
  [MR_TYPE_BITFIELD] = mr_ra_printf_bitfield,
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
  [MR_TYPE_POINTER] = json_printf_array,
  [MR_TYPE_UNION] = cinit_printf_struct,
  [MR_TYPE_ANON_UNION] = cinit_printf_struct,
  [MR_TYPE_NAMED_ANON_UNION] = cinit_printf_struct,
};

static char *
cinit_json_save (mr_ra_ptrdes_t * ptrs, mr_ra_printf_t * printf_tbl, char * named_field_tmplt, char * null)
{
  mr_rarray_t mr_ra_str = {
    .data = { mr_strdup ("") },
    .MR_SIZE = sizeof (""),
    .type = "string",
    .alloc_size = sizeof (""),
  };
  int idx = 0;
  int level = 0;

  if (NULL == mr_ra_str.data.string)
    return (NULL);

  ptrs->ptrdes_type = MR_PD_CUSTOM;

  while (idx >= 0)
    {
      memset (&ptrs->ra[idx].res, 0, sizeof (ptrs->ra[idx].res));
      
      mr_ra_printf_t save_handler = mr_ra_printf_void;
      if ((ptrs->ra[idx].mr_type < MR_TYPE_LAST) && printf_tbl[ptrs->ra[idx].mr_type])
	save_handler = printf_tbl[ptrs->ra[idx].mr_type];
      else
	MR_MESSAGE_UNSUPPORTED_NODE_TYPE_ (ptrs->ra[idx].fdp);

      if (mr_ra_printf (&mr_ra_str, MR_CINIT_INDENT_TEMPLATE, MR_LIMIT_LEVEL (level) * MR_CINIT_INDENT_SPACES, "") < 0)
	return (NULL);

      if (false == ptrs->ra[idx].unnamed)
	if (mr_ra_printf (&mr_ra_str, named_field_tmplt, ptrs->ra[idx].name) < 0)
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
	{
	  ++level;
	  idx = ptrs->ra[idx].first_child;
	}
      else
	{
	  if (ptrs->ra[idx].next >= 0)
	    if (mr_ra_append_string (&mr_ra_str, MR_CINIT_FIELDS_DELIMITER) < 0)
	      return (NULL);
	  if (mr_ra_append_string (&mr_ra_str, "\n") < 0)
	    return (NULL);
	  
	  while ((ptrs->ra[idx].next < 0) && (ptrs->ra[idx].parent >= 0))
	    {
	      if (ptrs->ra[idx].res.data.string)
		{
		  if (mr_ra_printf (&mr_ra_str, MR_CINIT_INDENT_TEMPLATE, MR_LIMIT_LEVEL (level) * MR_CINIT_INDENT_SPACES + 1, ptrs->ra[idx].res.data.string) < 0)
		    return (NULL);
		  if (ptrs->ra[idx].next >= 0)
		    if (mr_ra_append_string (&mr_ra_str, MR_CINIT_FIELDS_DELIMITER) < 0)
		      return (NULL);
		  if (mr_ra_append_string (&mr_ra_str, "\n") < 0)
		    return (NULL);
		}
	      --level;
	      idx = ptrs->ra[idx].parent;
	    }
	  
	  if (ptrs->ra[idx].res.data.string)
	    {
	      if (mr_ra_printf (&mr_ra_str, MR_CINIT_INDENT_TEMPLATE, MR_LIMIT_LEVEL (level) * MR_CINIT_INDENT_SPACES + 1, ptrs->ra[idx].res.data.string) < 0)
		return (NULL);
	      if (ptrs->ra[idx].next >= 0)
		if (mr_ra_append_string (&mr_ra_str, MR_CINIT_FIELDS_DELIMITER) < 0)
		  return (NULL);
	      if (mr_ra_append_string (&mr_ra_str, "\n") < 0)
		return (NULL);
	    }
	  
	  idx = ptrs->ra[idx].next;
	}
    }

  return (mr_ra_str.data.string);
}

char *
cinit_save (mr_ra_ptrdes_t * ptrs)
{
  return (cinit_json_save (ptrs, cinit_save_tbl, MR_CINIT_NAMED_FIELD_TEMPLATE, MR_CINIT_NULL));
}

char *
json_save (mr_ra_ptrdes_t * ptrs)
{
  int i;
  for (i = ptrs->size / sizeof (ptrs->ra[0]) - 1; i >= 0; --i)
    if ((MR_TYPE_ANON_UNION == ptrs->ra[i].mr_type) ||
	(MR_TYPE_POINTER == ptrs->ra[i].mr_type))
      ptrs->ra[i].unnamed = false;
  return (cinit_json_save (ptrs, json_save_tbl, MR_JSON_NAMED_FIELD_TEMPLATE, MR_JSON_NULL));
}
