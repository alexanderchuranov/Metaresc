/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <stdio.h>
#include <stdbool.h>

#include <metaresc.h>
#include <mr_stringify.h>

#define COMPLEX_REAL_IMAG_DELIMITER " + "
#define MR_JSON_INDENT_SPACES (2)
#define MR_JSON_INDENT_TEMPLATE "%*s"

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

  return (mr_ra_printf_bitfield (mr_ra_str, ptrdes));
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

static mr_ra_printf_t json_save_tbl[MR_TYPE_LAST] = {
  [MR_TYPE_NONE] = mr_ra_printf_void,
  [MR_TYPE_VOID] = mr_ra_printf_void,
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

char *
jsonx_save (mr_ra_ptrdes_t * ptrs)
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
      if ((ptrs->ra[idx].mr_type < MR_TYPE_LAST) && json_save_tbl[ptrs->ra[idx].mr_type])
	save_handler = json_save_tbl[ptrs->ra[idx].mr_type];
      else
	MR_MESSAGE_UNSUPPORTED_NODE_TYPE_ (ptrs->ra[idx].fdp);

      if (mr_ra_printf (&mr_ra_str, MR_JSON_INDENT_TEMPLATE, MR_LIMIT_LEVEL (level) * MR_JSON_INDENT_SPACES, "") < 0)
	return (NULL);

      if ((false == ptrs->ra[idx].unnamed) ||
	  (MR_TYPE_ANON_UNION == ptrs->ra[idx].mr_type) ||
	  (MR_TYPE_POINTER == ptrs->ra[idx].mr_type))
	{
	  if (mr_ra_append_char (&mr_ra_str, '"') < 0)
	    return (NULL);
	  if (mr_ra_append_string (&mr_ra_str, ptrs->ra[idx].name) < 0)
	    return (NULL);
	  if (mr_ra_append_string (&mr_ra_str, "\" : ") < 0)
	    return (NULL);
	}

      if (ptrs->ra[idx].ref_idx >= 0)
	{
	  if (ptrs->ra[idx].flags.is_content_reference)
	    {
	      if (mr_ra_printf (&mr_ra_str, "-%" SCNd32, ptrs->ra[ptrs->ra[idx].ref_idx].idx) < 0)
		return (NULL);
	    }
	  else
	    {
	      if (mr_ra_printf (&mr_ra_str, "%" SCNd32, ptrs->ra[ptrs->ra[idx].ref_idx].idx) < 0)
		return (NULL);
	    }
	}
      else if (true == ptrs->ra[idx].flags.is_null)
	{
	  if (mr_ra_append_string (&mr_ra_str, "null") < 0)
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
	    if (mr_ra_append_char (&mr_ra_str, ',') < 0)
	      return (NULL);
	  if (mr_ra_append_char (&mr_ra_str, '\n') < 0)
	    return (NULL);
	  
	  while ((ptrs->ra[idx].next < 0) && (ptrs->ra[idx].parent >= 0))
	    {
	      if (ptrs->ra[idx].res.data.string)
		{
		  if (mr_ra_printf (&mr_ra_str, MR_JSON_INDENT_TEMPLATE, MR_LIMIT_LEVEL (level) * MR_JSON_INDENT_SPACES + 1, ptrs->ra[idx].res.data.string) < 0)
		    return (NULL);
		  if (ptrs->ra[idx].next >= 0)
		    if (mr_ra_append_char (&mr_ra_str, ',') < 0)
		      return (NULL);
		  if (mr_ra_append_char (&mr_ra_str, '\n') < 0)
		    return (NULL);
		}
	      --level;
	      idx = ptrs->ra[idx].parent;
	    }
	  
	  if (ptrs->ra[idx].res.data.string)
	    {
	      if (mr_ra_printf (&mr_ra_str, MR_JSON_INDENT_TEMPLATE, MR_LIMIT_LEVEL (level) * MR_JSON_INDENT_SPACES + 1, ptrs->ra[idx].res.data.string) < 0)
		return (NULL);
	      if (ptrs->ra[idx].next >= 0)
		if (mr_ra_append_char (&mr_ra_str, ',') < 0)
		  return (NULL);
	      if (mr_ra_append_char (&mr_ra_str, '\n') < 0)
		return (NULL);
	    }
	  
	  idx = ptrs->ra[idx].next;
	}
    }

  return (mr_ra_str.data.string);
}
