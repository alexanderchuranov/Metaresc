/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <stdio.h>
#include <stdbool.h>

#include <metaresc.h>
#include <mr_save.h>
#include <mr_stringify.h>

#define COMPLEX_REAL_IMAG_DELIMITER " + "
#define JSON_INDENT_SPACES (2)
#define JSON_INDENT_TEMPLATE "%*s"
#define JSON_NULL "null"

#define JSON_QUOTE_CHAR_PATTERN "\\u%04x"
#define JSON_BITMASK_DELIMITER " | "

static int
json_printf_char (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  unsigned char c = *(unsigned char*)ptrdes->data.ptr;
  char mapped = mr_esc_char_map[c];
  int count = 0;

  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '"'));
  if (c == '"')
    mapped = c;
  if (mapped)
    {
      count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '\\'));
      count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, mapped));
    }
  else if (isprint (c))
    count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, c));
  else
    count += TRY_CATCH_THROW (mr_ra_printf (mr_ra_str, JSON_QUOTE_CHAR_PATTERN, c));
  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '"'));
  return (count);
}

static int
json_printf_complex_float (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  int count = 0;

  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '"'));
  count += TRY_CATCH_THROW (mr_ra_printf_complex_float (mr_ra_str, ptrdes, COMPLEX_REAL_IMAG_DELIMITER));
  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '"'));
  
  return (count);
}

static int
json_printf_complex_double (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  int count = 0;

  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '"'));
  count += TRY_CATCH_THROW (mr_ra_printf_complex_double (mr_ra_str, ptrdes, COMPLEX_REAL_IMAG_DELIMITER));
  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '"'));
  
  return (count);
}

static int
json_printf_complex_long_double_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  int count = 0;

  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '"'));
  count += TRY_CATCH_THROW (mr_ra_printf_complex_long_double_t (mr_ra_str, ptrdes, COMPLEX_REAL_IMAG_DELIMITER));
  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '"'));
  
  return (count);
}

static int
json_printf_char_array (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf_quote_string (mr_ra_str, ptrdes->data.ptr, JSON_QUOTE_CHAR_PATTERN));
}

static int
json_printf_string (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf_quote_string (mr_ra_str, *(char**)ptrdes->data.ptr, JSON_QUOTE_CHAR_PATTERN));
}

static int
json_printf_bitmask (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  int count = 0;

  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '"'));
  count += TRY_CATCH_THROW (mr_ra_printf_bitmask (mr_ra_str, ptrdes, JSON_BITMASK_DELIMITER));
  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '"'));
  
  return (count);
}

static int
json_printf_bitfield (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  if (MR_TYPE_ENUM == ptrdes->mr_type_aux)
    {
      mr_ptrdes_t _ptrdes = *ptrdes;
      uint64_t value;

      mr_save_bitfield_value (ptrdes, &value);
      _ptrdes.data.ptr = &value;
      return (json_printf_bitmask (mr_ra_str, &_ptrdes));
    }

  return (mr_ra_printf_bitfield (mr_ra_str, ptrdes, JSON_BITMASK_DELIMITER));
}

static int
json_printf_struct (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  ptrdes->res.data.string = "}";
  ptrdes->res.type = "string";
  ptrdes->res.MR_SIZE = 0;
  return (mr_ra_append_string (mr_ra_str, "{\n"));
}

static int
json_printf_func (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  const char * func_str = mr_serialize_func (*(void**)ptrdes->data.ptr);

  if (func_str) /* pointer serialized as name */
    return (mr_ra_printf (mr_ra_str, "\"%s\"", func_str));
  else
    return (mr_ra_printf (mr_ra_str, "\"%p\"", *(void**)ptrdes->data.ptr));
}

static int
json_printf_array (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  ptrdes->res.data.string = "]";
  ptrdes->res.type = "string";
  ptrdes->res.MR_SIZE = 0;
  return (mr_ra_append_string (mr_ra_str, "[\n"));
}

static int
json_printf_void (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  if (ptrdes->non_persistent)
    return (mr_ra_append_string (mr_ra_str, JSON_NULL));

  switch (ptrdes->fdp->mr_type_class)
    {
    case MR_RECORD_TYPE_CLASS:
    case MR_UNION_TYPE_CLASS:
    case MR_ARRAY_TYPE_CLASS:
      return (mr_ra_append_string (mr_ra_str, "{}"));
    case MR_POINTER_TYPE_CLASS:
      return (mr_ra_append_string (mr_ra_str, JSON_NULL));
    default:
      return (mr_ra_append_char (mr_ra_str, '0'));
    }
}

static mr_ra_printf_t json_save_tbl[MR_TYPE_LAST] = {
  [MR_TYPE_NONE] = mr_ra_printf_void,
  [MR_TYPE_VOID] = json_printf_void,
  [MR_TYPE_ENUM] = json_printf_bitmask,
  [MR_TYPE_BITFIELD] = json_printf_bitfield,
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
  [MR_TYPE_COMPLEX_FLOAT] = json_printf_complex_float,
  [MR_TYPE_DOUBLE] = mr_ra_printf_double,
  [MR_TYPE_COMPLEX_DOUBLE] = json_printf_complex_double,
  [MR_TYPE_LONG_DOUBLE] = mr_ra_printf_long_double_t,
  [MR_TYPE_COMPLEX_LONG_DOUBLE] = json_printf_complex_long_double_t,
  [MR_TYPE_CHAR] = json_printf_char,
  [MR_TYPE_CHAR_ARRAY] = json_printf_char_array,
  [MR_TYPE_STRING] = json_printf_string,
  [MR_TYPE_STRUCT] = json_printf_struct,
  [MR_TYPE_FUNC] = json_printf_func,
  [MR_TYPE_FUNC_TYPE] = json_printf_func,
  [MR_TYPE_ARRAY] = json_printf_array,
  [MR_TYPE_POINTER] = json_printf_array,
  [MR_TYPE_UNION] = json_printf_struct,
  [MR_TYPE_ANON_UNION] = json_printf_struct,
  [MR_TYPE_NAMED_ANON_UNION] = json_printf_struct,
};

static mr_status_t
json_pre_print_node (mr_ra_ptrdes_t * ptrs, int idx, int level, mr_rarray_t * mr_ra_str)
{
  mr_ra_printf_t save_handler = mr_ra_printf_void;
  if ((ptrs->ra[idx].mr_type < MR_TYPE_LAST) && json_save_tbl[ptrs->ra[idx].mr_type])
    save_handler = json_save_tbl[ptrs->ra[idx].mr_type];
  else
    MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNSUPPORTED_NODE_TYPE, ptrs->ra[idx].mr_type);

  memset (&ptrs->ra[idx].res, 0, sizeof (ptrs->ra[idx].res));

  if (mr_ra_printf (mr_ra_str, JSON_INDENT_TEMPLATE, MR_LIMIT_LEVEL (level) * JSON_INDENT_SPACES, "") < 0)
    return (MR_FAILURE);
  
  bool unnamed = ptrs->ra[idx].unnamed;
  int parent = ptrs->ra[idx].parent;
  if (((MR_TYPE_ANON_UNION == ptrs->ra[idx].mr_type) ||
       (MR_TYPE_POINTER == ptrs->ra[idx].mr_type)) &&
      (parent >= 0) && (MR_TYPE_ARRAY != ptrs->ra[parent].mr_type))
    unnamed = false;
  
  if (!unnamed)
    {
      if (mr_ra_append_char (mr_ra_str, '"') < 0)
	return (MR_FAILURE);
      if (mr_ra_append_string (mr_ra_str, ptrs->ra[idx].name) < 0)
	return (MR_FAILURE);
      if (mr_ra_append_string (mr_ra_str, "\" : ") < 0)
	return (MR_FAILURE);
    }

  if (ptrs->ra[idx].ref_idx >= 0)
    {
      if (ptrs->ra[idx].flags.is_content_reference)
	if (mr_ra_append_char (mr_ra_str, '-') < 0)
	  return (MR_FAILURE);
      if (mr_ra_printf (mr_ra_str, "%" SCNu32, ptrs->ra[ptrs->ra[idx].ref_idx].idx) < 0)
	return (MR_FAILURE);
    }
  else if (ptrs->ra[idx].flags.is_null)
    {
      if (mr_ra_append_string (mr_ra_str, JSON_NULL) < 0)
	return (MR_FAILURE);
    }
  else if (save_handler (mr_ra_str, &ptrs->ra[idx]) < 0)
    return (MR_FAILURE);
  
  return (MR_SUCCESS);
}

static mr_status_t
json_post_print_node (mr_ra_ptrdes_t * ptrs, int idx, int level, mr_rarray_t * mr_ra_str)
{
  if (ptrs->ra[idx].res.data.string)
    if (mr_ra_printf (mr_ra_str, JSON_INDENT_TEMPLATE, MR_LIMIT_LEVEL (level) * JSON_INDENT_SPACES + 1, ptrs->ra[idx].res.data.string) < 0)
      return (MR_FAILURE);

  if (ptrs->ra[idx].next >= 0)
    if (mr_ra_append_char (mr_ra_str, ',') < 0)
      return (MR_FAILURE);

  if (mr_ra_append_char (mr_ra_str, '\n') < 0)
    return (MR_FAILURE);

  return (MR_SUCCESS);
}

static mr_status_t
json_print_node (mr_ra_ptrdes_t * ptrs, int idx, int level, mr_dfs_order_t order, void * context)
{
  mr_rarray_t * mr_ra_str = context;

  switch (order)
    {
    case MR_DFS_PRE_ORDER:
      return (json_pre_print_node (ptrs, idx, level, mr_ra_str));
    case MR_DFS_POST_ORDER:
      return (json_post_print_node (ptrs, idx, level, mr_ra_str));
    default:
      return (MR_FAILURE);
    }
}

char *
json_save (mr_ra_ptrdes_t * ptrs)
{
  mr_rarray_t mr_ra_str = {
    .data = { mr_strdup ("") },
    .MR_SIZE = sizeof (""),
    .type = "string",
    .alloc_size = sizeof (""),
  };

  if (NULL == mr_ra_str.data.string)
    return (NULL);

  ptrs->ptrdes_type = MR_PD_CUSTOM;

  mr_ptrs_dfs (ptrs, json_print_node, &mr_ra_str);

  return (mr_ra_str.data.string);
}
