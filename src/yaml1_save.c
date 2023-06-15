/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

#include <stdio.h>
#include <stdbool.h>

#include <metaresc.h>
#include <mr_save.h>
#include <mr_stringify.h>

#define COMPLEX_REAL_IMAG_DELIMITER " + "
#define YAML1_INDENT_SPACES (2)
#define YAML1_INDENT_TEMPLATE "%*s"
#define YAML1_NULL "null"

#define YAML1_QUOTE_CHAR_PATTERN "\\u%04x"
#define YAML1_BITMASK_DELIMITER " | "

static int
yaml1_printf_char (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
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
    count += TRY_CATCH_THROW (mr_ra_printf (mr_ra_str, YAML1_QUOTE_CHAR_PATTERN, c));
  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '"'));
  return (count);
}

static int
yaml1_printf_complex_float (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  int count = 0;

  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '"'));
  count += TRY_CATCH_THROW (mr_ra_printf_complex_float (mr_ra_str, ptrdes, COMPLEX_REAL_IMAG_DELIMITER));
  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '"'));
  
  return (count);
}

static int
yaml1_printf_complex_double (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  int count = 0;

  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '"'));
  count += TRY_CATCH_THROW (mr_ra_printf_complex_double (mr_ra_str, ptrdes, COMPLEX_REAL_IMAG_DELIMITER));
  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '"'));
  
  return (count);
}

static int
yaml1_printf_complex_long_double_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  int count = 0;

  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '"'));
  count += TRY_CATCH_THROW (mr_ra_printf_complex_long_double_t (mr_ra_str, ptrdes, COMPLEX_REAL_IMAG_DELIMITER));
  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '"'));
  
  return (count);
}

static int
yaml1_printf_char_array (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  typeof (ptrdes->fdp->size) size = ptrdes->non_persistent ? (ptrdes->tdp ? ptrdes->tdp->size : 0) : ptrdes->fdp->size;
  char buffer[size + 1];
  strncpy (buffer, ptrdes->data.ptr, size);
  buffer[size] = 0;
  return (mr_ra_printf_quote_string (mr_ra_str, buffer, YAML1_QUOTE_CHAR_PATTERN));
}

static int
yaml1_printf_string (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf_quote_string (mr_ra_str, *(char**)ptrdes->data.ptr, YAML1_QUOTE_CHAR_PATTERN));
}

static int
yaml1_printf_bitmask (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  int count = 0;

  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '"'));
  count += TRY_CATCH_THROW (mr_ra_printf_bitmask (mr_ra_str, ptrdes, YAML1_BITMASK_DELIMITER));
  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '"'));
  
  return (count);
}

static int
yaml1_printf_bitfield (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  if (MR_TYPE_ENUM == ptrdes->mr_type_aux)
    {
      mr_ptrdes_t _ptrdes = *ptrdes;
      uint64_t value;

      mr_save_bitfield_value (ptrdes, &value);
      _ptrdes.data.ptr = &value;
      return (yaml1_printf_bitmask (mr_ra_str, &_ptrdes));
    }

  return (mr_ra_printf_bitfield (mr_ra_str, ptrdes, YAML1_BITMASK_DELIMITER));
}

static int
yaml1_printf_struct (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  ptrdes->res.data.string = "}";
  ptrdes->res.type = "string";
  ptrdes->res.MR_SIZE = 0;
  if (ptrdes->unnamed && ptrdes->mr_type != MR_TYPE_ANON_UNION)
  {
    return (mr_ra_append_string (mr_ra_str, ""));
  }
  return (mr_ra_append_string (mr_ra_str, "\n"));
}

static int
yaml1_printf_func (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  const char * func_str = mr_serialize_func (*(void**)ptrdes->data.ptr);

  if (func_str) /* pointer serialized as name */
    return (mr_ra_printf (mr_ra_str, "\"%s\"", func_str));
  else
    return (mr_ra_printf (mr_ra_str, "\"0x%llx\"", (unsigned long long int)*(uintptr_t*)ptrdes->data.ptr));
}

static int
yaml1_printf_array (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  ptrdes->res.data.string = "]";
  ptrdes->res.type = "string";
  ptrdes->res.MR_SIZE = 0;
  return (mr_ra_append_string (mr_ra_str, "\n"));
}

static int
yaml1_printf_void (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  if (ptrdes->non_persistent)
    return (mr_ra_append_string (mr_ra_str, YAML1_NULL));

  switch (ptrdes->fdp->mr_type_class)
    {
    case MR_RECORD_TYPE_CLASS:
    case MR_UNION_TYPE_CLASS:
    case MR_ARRAY_TYPE_CLASS:
      return (mr_ra_append_string (mr_ra_str, "{}"));
    case MR_POINTER_TYPE_CLASS:
      return (mr_ra_append_string (mr_ra_str, YAML1_NULL));
    default:
      return (mr_ra_append_char (mr_ra_str, '0'));
    }
}

static mr_ra_printf_t yaml1_save_tbl[MR_TYPE_LAST] = {
  [MR_TYPE_NONE] = mr_ra_printf_void,
  [MR_TYPE_VOID] = yaml1_printf_void,
  [MR_TYPE_ENUM] = yaml1_printf_bitmask,
  [MR_TYPE_BITFIELD] = yaml1_printf_bitfield,
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
  [MR_TYPE_COMPLEX_FLOAT] = yaml1_printf_complex_float,
  [MR_TYPE_DOUBLE] = mr_ra_printf_double,
  [MR_TYPE_COMPLEX_DOUBLE] = yaml1_printf_complex_double,
  [MR_TYPE_LONG_DOUBLE] = mr_ra_printf_long_double_t,
  [MR_TYPE_COMPLEX_LONG_DOUBLE] = yaml1_printf_complex_long_double_t,
  [MR_TYPE_CHAR] = yaml1_printf_char,
  [MR_TYPE_CHAR_ARRAY] = yaml1_printf_char_array,
  [MR_TYPE_STRING] = yaml1_printf_string,
  [MR_TYPE_STRUCT] = yaml1_printf_struct,
  [MR_TYPE_FUNC] = yaml1_printf_func,
  [MR_TYPE_FUNC_TYPE] = yaml1_printf_func,
  [MR_TYPE_ARRAY] = yaml1_printf_array,
  [MR_TYPE_POINTER] = yaml1_printf_array,
  [MR_TYPE_UNION] = yaml1_printf_struct,
  [MR_TYPE_ANON_UNION] = yaml1_printf_struct,
  [MR_TYPE_NAMED_ANON_UNION] = yaml1_printf_struct,
};

static mr_status_t
yaml1_pre_print_node (mr_ra_ptrdes_t * ptrs, int idx, int level, mr_rarray_t * mr_ra_str)
{
   mr_ra_printf_t save_handler = NULL;
  
  if ((ptrs->ra[idx].mr_type >= 0) && (ptrs->ra[idx].mr_type < MR_TYPE_LAST))
    save_handler = yaml1_save_tbl[ptrs->ra[idx].mr_type];

  if (NULL == save_handler)
    {
      save_handler = mr_ra_printf_void;
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNSUPPORTED_NODE_TYPE, ptrs->ra[idx].mr_type);
    }

  memset (&ptrs->ra[idx].res, 0, sizeof (ptrs->ra[idx].res));
  
  bool unnamed = ptrs->ra[idx].unnamed;
  int parent = ptrs->ra[idx].parent;
  if (unnamed && (parent >= 0))
    if (((0 MR_FOREACH (MR_ONE_SHIFT, MR_TYPE_STRUCT, MR_TYPE_UNION, MR_TYPE_ANON_UNION, MR_TYPE_NAMED_ANON_UNION)) >> ptrs->ra[parent].mr_type) & 1)
      unnamed = false;

  // Indent and add dashes to array entries which are not a struct/union/etc.
  if (unnamed && (parent >= 0) && ptrs->ra[idx].mr_type != MR_TYPE_UNION && \
      ptrs->ra[idx].mr_type != MR_TYPE_STRUCT)
  {
    if (mr_ra_printf (mr_ra_str, YAML1_INDENT_TEMPLATE, MR_LIMIT_LEVEL (level + 1) * YAML1_INDENT_SPACES, "- ") < 0)
      return (MR_FAILURE);
  }

  if (!unnamed)
  {
    // Add a special symbol to start of nested YAML array / structure.
    char* symb = "";
    if (ptrs->ra[parent].unnamed)
    {
      symb = (ptrs->ra[idx].prev < 0)? "- " : "";
    }
    if (mr_ra_printf (mr_ra_str, YAML1_INDENT_TEMPLATE, MR_LIMIT_LEVEL (level) * YAML1_INDENT_SPACES, symb) < 0)
      return (MR_FAILURE);
    if (mr_ra_append_string (mr_ra_str, ptrs->ra[idx].name) < 0)
      return (MR_FAILURE);
    if (mr_ra_append_string (mr_ra_str, ": ") < 0)
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
      if (mr_ra_append_string (mr_ra_str, YAML1_NULL) < 0)
	      return (MR_FAILURE);
    }
  else if (save_handler (mr_ra_str, &ptrs->ra[idx]) < 0)
    return (MR_FAILURE);
  
  return (MR_SUCCESS);
}

static mr_status_t
yaml1_post_print_node (mr_ra_ptrdes_t * ptrs, int idx, int level, mr_rarray_t * mr_ra_str)
{
  if (!ptrs->ra[idx].res.data.string)
  {
    if (mr_ra_append_char (mr_ra_str, '\n') < 0)
      return (MR_FAILURE);
  }
  
  return (MR_SUCCESS);
}

static mr_status_t
yaml1_print_node (mr_ra_ptrdes_t * ptrs, int idx, int level, mr_dfs_order_t order, void * context)
{
  mr_rarray_t * mr_ra_str = context;

  switch (order)
    {
    case MR_DFS_PRE_ORDER:
      return (yaml1_pre_print_node (ptrs, idx, level, mr_ra_str));
    case MR_DFS_POST_ORDER:
      return (yaml1_post_print_node (ptrs, idx, level, mr_ra_str));
    default:
      return (MR_FAILURE);
    }
}

char *
yaml1_save (mr_ra_ptrdes_t * ptrs)
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
  // Add a YAML document identifier.
  mr_ra_append_string (&mr_ra_str, "---\n");
  mr_ptrs_dfs (ptrs, yaml1_print_node, &mr_ra_str);

  return (mr_ra_str.data.string);
}
