/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <metaresc.h>
#include <mr_stringify.h>

#define MR_SCM_FALSE "#f"
#define MR_SCM_TRUE "#t"

#define MR_SCM_INDENT_SPACES (2)
#define MR_SCM_INDENT_TEMPLATE "\n%*s"

#define MR_SCM_NAMED_FIELD_START "("
#define MR_SCM_NAMED_FIELD_END " . %s)"
#define MR_SCM_COMPAUND_START "("
#define MR_SCM_COMPAUND_END ")"
#define MR_SCM_ARRAY_PREFIX "#"
#define MR_SCM_ATTR_INT ";(%s %d)"

#define MR_SCM_BITMASK_OR_DELIMITER " "
#define MR_SCM_BITMASK_PREFIX "(logior "

#define MR_SCM_UNNAMED_FIELDS (0)
#define MR_SCM_NAMED_FIELDS (!MR_SCM_UNNAMED_FIELDS)

static int scm_named_fields[MR_TYPE_LAST] = {
  [MR_TYPE_STRUCT] = MR_SCM_NAMED_FIELDS,
  [MR_TYPE_UNION] = MR_SCM_NAMED_FIELDS,
  [MR_TYPE_ANON_UNION] = MR_SCM_NAMED_FIELDS,
  [MR_TYPE_NAMED_ANON_UNION] = MR_SCM_NAMED_FIELDS,
};

#define SCM_SAVE_COMPLEX(TYPE)						\
  static int scm_save_complex_ ## TYPE (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes) \
  {									\
    int count = 0;							\
    count += TRY_CATCH_THROW (mr_ra_printf (mr_ra_str, "(+ "));		\
    count += TRY_CATCH_THROW (mr_ra_printf_complex_ ## TYPE (mr_ra_str, ptrdes, " ")); \
    count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, ')'));	\
    return (count);							\
  }

SCM_SAVE_COMPLEX (float)
SCM_SAVE_COMPLEX (double)
SCM_SAVE_COMPLEX (long_double_t)

static int
scm_printf_bool (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (*(bool*)ptrdes->data.ptr ?
	  mr_ra_printf (mr_ra_str, MR_SCM_TRUE) :
	  mr_ra_printf (mr_ra_str, MR_SCM_FALSE));
}

static int
scm_printf_bitfield (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  if (MR_TYPE_BOOL == ptrdes->fd.mr_type_aux)
    {
      mr_ptrdes_t _ptrdes = *ptrdes;;
      uint64_t value;

      mr_save_bitfield_value (ptrdes, &value);
      _ptrdes.data.ptr = &value;
      return (scm_printf_bool (mr_ra_str, &_ptrdes));
    }

  return (mr_ra_printf_bitfield (mr_ra_str, ptrdes));
}  

static int
scm_printf_bitmask (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  mr_td_t * tdp = mr_get_td_by_name (ptrdes->fd.type);
  if ((NULL == tdp) || (MR_TYPE_ENUM != tdp->mr_type) || (!tdp->param.enum_param.is_bitmask))
    return (mr_ra_printf_enum (mr_ra_str, ptrdes));

  int tokens = 0;
  int i, fields_count = tdp->fields_size / sizeof (tdp->fields[0]);
  int64_t value = mr_get_enum_value (tdp, ptrdes->data.ptr);

  if (0 == value)
    return (mr_ra_printf_enum (mr_ra_str, ptrdes));

  for (i = 0; i < fields_count; ++i)
    if (value & tdp->fields[i].fdp->param.enum_value._unsigned)
      {
	++tokens;
	value ^= tdp->fields[i].fdp->param.enum_value._unsigned;
	if (0 == value)
	  break;
      }

  if (0 != value)
    ++tokens;

  if (1 == tokens)
    return (mr_ra_printf_enum (mr_ra_str, ptrdes));

  int count = 0;
  count += TRY_CATCH_THROW (mr_ra_append_string (mr_ra_str, MR_SCM_BITMASK_PREFIX));
  count += TRY_CATCH_THROW (mr_ra_printf_bitmask (mr_ra_str, ptrdes, MR_SCM_BITMASK_OR_DELIMITER));
  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, ')'));
  return (count);
}

/**
 * MR_TYPE_FUNC & MR_TYPE_FUNC_TYPE type saving handler.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors
 * @return stringified function name
 */
static int
scm_save_func (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  if (true == ptrdes->flags.is_null)
    return (mr_ra_printf (mr_ra_str, MR_SCM_FALSE));
  const char * func_str = mr_serialize_func (*(void**)ptrdes->data.ptr);
  if (func_str)
    return (mr_ra_printf (mr_ra_str, "%s", func_str));
  else
    return (mr_ra_printf (mr_ra_str, "%p", *(void**)ptrdes->data.ptr));
}

/**
 * MR_CHAR type saving handler. Stringify char.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors
 * @return stringified char
 */
static int
scm_save_char (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  char c = *(char*)ptrdes->data.ptr;
  if (isprint (c))
    return (mr_ra_printf (mr_ra_str, "#\\%c", c));
  
  return (mr_ra_printf (mr_ra_str, "#\\x%02x", (int)(unsigned char)c));
}

/**
 * MR_CHAR_ARRAY type saving handler. Save char array.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors
 * @return char array value
 */
static int
scm_save_char_array (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf_quote_string (mr_ra_str, ptrdes->data.ptr, "\\x%02x;"));
}

/**
 * MR_STRING type saving handler. Save string.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors
 * @return string value
 */
static int
scm_save_string (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  char * str = *(char**)ptrdes->data.ptr;
  if ((NULL == str) || (ptrdes->ref_idx >= 0))
    return (mr_ra_printf (mr_ra_str, MR_SCM_FALSE));
  else
    return (mr_ra_printf_quote_string (mr_ra_str, str, "\\x%02x;"));
}

/**
 * MR_POINTER type saving handler. Save pointer as a string.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors
 * @return stringified pointer
 */
static int
scm_save_pointer (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf (mr_ra_str, MR_SCM_FALSE));
}

/**
 * Init IO handlers Table
 */
static mr_ra_printf_t scm_save_handler[] =
  {
    [MR_TYPE_NONE] = mr_ra_printf_void,
    [MR_TYPE_VOID] = mr_ra_printf_void,
    [MR_TYPE_ENUM] = scm_printf_bitmask,
    [MR_TYPE_BITFIELD] = scm_printf_bitfield,
    [MR_TYPE_BOOL] = scm_printf_bool,
    [MR_TYPE_INT8] = mr_ra_printf_int8_t,
    [MR_TYPE_UINT8] = mr_ra_printf_uint8_t,
    [MR_TYPE_INT16] = mr_ra_printf_int16_t,
    [MR_TYPE_UINT16] = mr_ra_printf_uint16_t,
    [MR_TYPE_INT32] = mr_ra_printf_int32_t,
    [MR_TYPE_UINT32] = mr_ra_printf_uint32_t,
    [MR_TYPE_INT64] = mr_ra_printf_int64_t,
    [MR_TYPE_UINT64] = mr_ra_printf_uint64_t,
    [MR_TYPE_FLOAT] = mr_ra_printf_float,
    [MR_TYPE_COMPLEX_FLOAT] = scm_save_complex_float,
    [MR_TYPE_DOUBLE] = mr_ra_printf_double,
    [MR_TYPE_COMPLEX_DOUBLE] = scm_save_complex_double,
    [MR_TYPE_LONG_DOUBLE] = mr_ra_printf_long_double_t,
    [MR_TYPE_COMPLEX_LONG_DOUBLE] = scm_save_complex_long_double_t,
    [MR_TYPE_CHAR] = scm_save_char,
    [MR_TYPE_CHAR_ARRAY] = scm_save_char_array,
    [MR_TYPE_STRING] = scm_save_string,
    [MR_TYPE_STRUCT] = mr_ra_printf_void,
    [MR_TYPE_FUNC] = scm_save_func,
    [MR_TYPE_FUNC_TYPE] = scm_save_func,
    [MR_TYPE_ARRAY] = mr_ra_printf_void,
    [MR_TYPE_POINTER] = scm_save_pointer,
    [MR_TYPE_UNION] = mr_ra_printf_void,
    [MR_TYPE_ANON_UNION] = mr_ra_printf_void,
    [MR_TYPE_NAMED_ANON_UNION] = mr_ra_printf_void,
  };

/**
 * Public function. Save scheduler. Save any object as a string.
 * @param ptrs resizeable array with pointers descriptors
 * @return stringified representation of object
 */
char *
scm_save (mr_ra_ptrdes_t * ptrs)
{
  mr_rarray_t mr_ra_str = { .data = { mr_strdup ("") }, .MR_SIZE = sizeof (""), .type = "string", .alloc_size = sizeof (""), };
  int idx = 0;

  if (NULL == mr_ra_str.data.ptr)
    return (NULL);

  while (idx >= 0)
    {
      mr_fd_t * fdp = &ptrs->ra[idx].fd;
      int level = MR_LIMIT_LEVEL (ptrs->ra[idx].save_params.level);
      int named_node = MR_SCM_UNNAMED_FIELDS;
      int parent = ptrs->ra[idx].parent;
      int in_comment = false;

      if (parent >= 0)
	named_node = scm_named_fields[ptrs->ra[parent].fd.mr_type];

      if (ptrs->ra[idx].ref_idx >= 0)
	{
	  if (mr_ra_printf (&mr_ra_str, MR_SCM_INDENT_TEMPLATE MR_SCM_ATTR_INT,
			    level * MR_SCM_INDENT_SPACES, "",
			    (ptrs->ra[idx].flags.is_content_reference) ? MR_REF_CONTENT : MR_REF,
			    ptrs->ra[ptrs->ra[idx].ref_idx].idx) < 0)
	    return (NULL);

	  in_comment = true;
	}
      if (ptrs->ra[idx].flags.is_referenced)
	{
	  if (mr_ra_printf (&mr_ra_str, MR_SCM_INDENT_TEMPLATE MR_SCM_ATTR_INT,
			    level * MR_SCM_INDENT_SPACES, "", MR_REF_IDX, ptrs->ra[idx].idx) < 0)
	    return (NULL);

	  in_comment = true;
	}

      if (ptrs->ra[idx].first_child < 0)
	{
	  mr_ra_printf_t save_handler = mr_ra_printf_void;
	  /* route saving handler */
	  if ((fdp->mr_type < MR_TYPE_LAST) && scm_save_handler[fdp->mr_type])
	    save_handler = scm_save_handler[fdp->mr_type];
	  else
	    MR_MESSAGE_UNSUPPORTED_NODE_TYPE_ (fdp);

	  if (MR_SCM_NAMED_FIELDS == named_node)
	    {
	      if (mr_ra_printf (&mr_ra_str, MR_SCM_INDENT_TEMPLATE MR_SCM_NAMED_FIELD_START, level * MR_SCM_INDENT_SPACES, "") < 0)
		return (NULL);
	      if (save_handler (&mr_ra_str, &ptrs->ra[idx]) < 0)
		return (NULL);
	      if (mr_ra_printf (&mr_ra_str, MR_SCM_NAMED_FIELD_END, ptrs->ra[idx].fd.name.str) < 0)
		return (NULL);
	    }
	  else
	    {
	      if (in_comment)
		{
		  if (mr_ra_printf (&mr_ra_str, MR_SCM_INDENT_TEMPLATE, level * MR_SCM_INDENT_SPACES, "") < 0)
		    return (NULL);
		}
	      else
		{
		  if (mr_ra_append_char (&mr_ra_str, ' ') < 0)
		    return (NULL);
		}
	      if (save_handler (&mr_ra_str, &ptrs->ra[idx]) < 0)
		return (NULL);
	    }
	}
      else
	{
	  if ((idx != 0) || in_comment)
	    if (mr_ra_printf (&mr_ra_str, MR_SCM_INDENT_TEMPLATE, level * MR_SCM_INDENT_SPACES, "") < 0)
	      return (NULL);
	  if (MR_SCM_NAMED_FIELDS == named_node)
	    if (mr_ra_printf (&mr_ra_str, MR_SCM_NAMED_FIELD_START) < 0)
	      return (NULL);
	  if (MR_TYPE_ARRAY == fdp->mr_type)
	    if (mr_ra_printf (&mr_ra_str, MR_SCM_ARRAY_PREFIX) < 0)
	      return (NULL);
	  if (mr_ra_printf (&mr_ra_str, MR_SCM_COMPAUND_START) < 0)
	    return (NULL);
	}

      if (ptrs->ra[idx].first_child >= 0)
	idx = ptrs->ra[idx].first_child;
      else
	{
	  while ((ptrs->ra[idx].next < 0) && (ptrs->ra[idx].parent >= 0))
	    {
	      idx = ptrs->ra[idx].parent;
	      named_node = MR_SCM_UNNAMED_FIELDS;
	      parent = ptrs->ra[idx].parent;

	      if (parent >= 0)
		named_node = scm_named_fields[ptrs->ra[parent].fd.mr_type];

	      if (mr_ra_printf (&mr_ra_str, MR_SCM_COMPAUND_END) < 0)
		return (NULL);
	      if (MR_SCM_NAMED_FIELDS == named_node)
		if (mr_ra_printf (&mr_ra_str, MR_SCM_NAMED_FIELD_END, ptrs->ra[idx].fd.name.str) < 0)
		  return (NULL);
	    }
	  idx = ptrs->ra[idx].next;
	}
    }

  return (mr_ra_str.data.ptr);
}
