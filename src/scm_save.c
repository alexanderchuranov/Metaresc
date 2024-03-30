/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <metaresc.h>
#include <mr_save.h>
#include <mr_stringify.h>

#define MR_SCM_FALSE "#f"
#define MR_SCM_TRUE "#t"

#define MR_SCM_INDENT_SPACES (2)
#define MR_SCM_INDENT_TEMPLATE "\n%*s"

#define MR_SCM_NAMED_FIELD_START "("
#define MR_SCM_COMPAUND_START "("
#define MR_SCM_COMPAUND_END ")"
#define MR_SCM_ARRAY_PREFIX "#"
#define MR_SCM_ATTR_INT ";(%s %d)"

#define MR_SCM_BITMASK_OR_DELIMITER " "
#define MR_SCM_BITMASK_PREFIX "(logior "

#define SCM_SAVE_COMPLEX(TYPE)						\
  static int scm_save_complex_ ## TYPE (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes) \
  {									\
    int count = 0;							\
    count += TRY_CATCH_THROW (mr_ra_append_string (mr_ra_str, "(+ "));	\
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
	  mr_ra_append_string (mr_ra_str, MR_SCM_TRUE) :
	  mr_ra_append_string (mr_ra_str, MR_SCM_FALSE));
}

static int
scm_printf_bitmask (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  mr_td_t * tdp = ptrdes->fdp ? ptrdes->fdp->stype.tdp : NULL;
  if (NULL == tdp)
    return (mr_ra_printf_enum (mr_ra_str, ptrdes));
  if ((MR_TYPE_ENUM != tdp->mr_type) || (!tdp->param.enum_param.is_bitmask))
    return (mr_ra_printf_enum (mr_ra_str, ptrdes));

  int tokens = 0;
  int i, enums_count = tdp->param.enum_param.enums_size / sizeof (tdp->param.enum_param.enums[0]);
  mr_enum_value_type_t value = mr_get_enum_value (tdp, ptrdes->data.ptr);

  if (0 == value)
    return (mr_ra_printf_enum (mr_ra_str, ptrdes));

  for (i = 0; i < enums_count; ++i)
    if (value & tdp->param.enum_param.enums[i]->value._unsigned)
      {
	++tokens;
	value ^= tdp->param.enum_param.enums[i]->value._unsigned;
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

static int
scm_printf_bitfield (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  mr_ptrdes_t _ptrdes = *ptrdes;
  mr_uintmax_t value;

  mr_save_bitfield_value (ptrdes, &value);
  _ptrdes.data.ptr = &value;
  
  switch (ptrdes->mr_type_aux)
    {
    case MR_TYPE_BOOL: return (scm_printf_bool (mr_ra_str, &_ptrdes));
    case MR_TYPE_ENUM: return (scm_printf_bitmask (mr_ra_str, &_ptrdes));
    default: return (mr_ra_printf_bitfield (mr_ra_str, ptrdes, MR_SCM_BITMASK_OR_DELIMITER));
    }
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
    return (mr_ra_append_string (mr_ra_str, MR_SCM_FALSE));
  const char * func_str = mr_serialize_func (*(void**)ptrdes->data.ptr);
  if (func_str)
    return (mr_ra_append_string (mr_ra_str, (char*)func_str));
  else
    return (mr_ra_printf (mr_ra_str, "#x%llx", (long long unsigned int)*(uintptr_t*)ptrdes->data.ptr));
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
  typeof (ptrdes->fdp->stype.size) size = ptrdes->fdp ? ptrdes->fdp->stype.size : 0;
  char buffer[size + 1];
  strncpy (buffer, ptrdes->data.ptr, size);
  buffer[size] = 0;
  return (mr_ra_printf_quote_string (mr_ra_str, buffer, "\\x%02x;"));
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
    return (mr_ra_append_string (mr_ra_str, MR_SCM_FALSE));
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
  return (mr_ra_append_string (mr_ra_str, MR_SCM_FALSE));
}

static int
scm_printf_void (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_append_string (mr_ra_str, MR_SCM_FALSE));
}

/**
 * Init IO handlers Table
 */
static mr_ra_printf_t scm_save_handler[MR_TYPE_LAST] =
  {
    [MR_TYPE_NONE] = mr_ra_printf_void,
    [MR_TYPE_VOID] = scm_printf_void,
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
    [MR_TYPE_INT128] = mr_ra_printf_int128_t,
    [MR_TYPE_UINT128] = mr_ra_printf_uint128_t,
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

static inline bool
scm_is_unnamed (mr_ra_ptrdes_t * ptrs, int idx)
{
  int parent = ptrs->ra[idx].parent;
  if ((MR_TYPE_POINTER == ptrs->ra[idx].mr_type) && (parent >= 0))
    if (MR_TYPE_ARRAY != ptrs->ra[parent].mr_type)
      return (false);
  return (ptrs->ra[idx].flags.unnamed);
}

static mr_status_t
scm_pre_print_node (mr_ra_ptrdes_t * ptrs, int idx, int level, mr_rarray_t * mr_ra_str)
{
  int in_comment = false;
  int limit_level = MR_LIMIT_LEVEL (level);

  memset (&ptrs->ra[idx].res, 0, sizeof (ptrs->ra[idx].res));
  
  if (ptrs->ra[idx].ref_idx >= 0)
    {
      if (mr_ra_printf (mr_ra_str, MR_SCM_INDENT_TEMPLATE MR_SCM_ATTR_INT,
			limit_level * MR_SCM_INDENT_SPACES, "",
			(ptrs->ra[idx].flags.is_content_reference) ? MR_REF_CONTENT : MR_REF,
			ptrs->ra[ptrs->ra[idx].ref_idx].idx) < 0)
	return (MR_FAILURE);

      in_comment = true;
    }
  if (ptrs->ra[idx].flags.is_referenced)
    {
      if (mr_ra_printf (mr_ra_str, MR_SCM_INDENT_TEMPLATE MR_SCM_ATTR_INT,
			limit_level * MR_SCM_INDENT_SPACES, "", MR_REF_IDX, ptrs->ra[idx].idx) < 0)
	return (MR_FAILURE);

      in_comment = true;
    }

  if (ptrs->ra[idx].first_child < 0)
    {
      mr_ra_printf_t save_handler = NULL;
      /* route saving handler */
      if ((ptrs->ra[idx].mr_type >= 0) && (ptrs->ra[idx].mr_type < MR_TYPE_LAST))
	save_handler = scm_save_handler[ptrs->ra[idx].mr_type];
      
      if (NULL == save_handler)
	{
	  save_handler = mr_ra_printf_void;
	  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNSUPPORTED_NODE_TYPE, ptrs->ra[idx].mr_type);
	}

      if (!scm_is_unnamed (ptrs, idx))
	{
	  if (mr_ra_printf (mr_ra_str, MR_SCM_INDENT_TEMPLATE MR_SCM_NAMED_FIELD_START,
			    limit_level * MR_SCM_INDENT_SPACES, "") < 0)
	    return (MR_FAILURE);
	  if (save_handler (mr_ra_str, &ptrs->ra[idx]) < 0)
	    return (MR_FAILURE);
	  if (mr_ra_append_string (mr_ra_str, " . ") < 0)
	    return (MR_FAILURE);
	  char * name = ptrs->ra[idx].fdp ? ptrs->ra[idx].fdp->name.str : MR_DEFAULT_NODE_NAME;
	  if (mr_ra_append_string (mr_ra_str, name) < 0)
	    return (MR_FAILURE);
	  if (mr_ra_append_char (mr_ra_str, ')') < 0)
	    return (MR_FAILURE);
	}
      else
	{
	  if (in_comment)
	    {
	      if (mr_ra_printf (mr_ra_str, MR_SCM_INDENT_TEMPLATE,
				limit_level * MR_SCM_INDENT_SPACES, "") < 0)
		return (MR_FAILURE);
	    }
	  else
	    {
	      if (mr_ra_append_char (mr_ra_str, ' ') < 0)
		return (MR_FAILURE);
	    }
	  if (save_handler (mr_ra_str, &ptrs->ra[idx]) < 0)
	    return (MR_FAILURE);
	}
    }
  else
    {
      if ((idx != 0) || in_comment)
	if (mr_ra_printf (mr_ra_str, MR_SCM_INDENT_TEMPLATE,
			  limit_level * MR_SCM_INDENT_SPACES, "") < 0)
	  return (MR_FAILURE);

      if (!scm_is_unnamed (ptrs, idx))
	if (mr_ra_append_string (mr_ra_str, MR_SCM_NAMED_FIELD_START) < 0)
	  return (MR_FAILURE);

      if (MR_TYPE_ARRAY == ptrs->ra[idx].mr_type)
	if (mr_ra_append_string (mr_ra_str, MR_SCM_ARRAY_PREFIX) < 0)
	  return (MR_FAILURE);

      if (mr_ra_append_string (mr_ra_str, MR_SCM_COMPAUND_START) < 0)
	return (MR_FAILURE);
    }

  return (MR_SUCCESS);
}

static mr_status_t
scm_post_print_node (mr_ra_ptrdes_t * ptrs, int idx, int level, mr_rarray_t * mr_ra_str)
{
  if (ptrs->ra[idx].first_child >= 0)
    if (mr_ra_append_string (mr_ra_str, MR_SCM_COMPAUND_END) < 0)
      return (MR_FAILURE);
  
  if (!scm_is_unnamed (ptrs, idx) && (ptrs->ra[idx].first_child >= 0))
    {
      if (mr_ra_append_string (mr_ra_str, " . ") < 0)
	return (MR_FAILURE);
      char * name = ptrs->ra[idx].fdp ? ptrs->ra[idx].fdp->name.str : MR_DEFAULT_NODE_NAME;
      if (mr_ra_append_string (mr_ra_str, name) < 0)
	return (MR_FAILURE);
      if (mr_ra_append_char (mr_ra_str, ')') < 0)
	return (MR_FAILURE);
    }      

  return (MR_SUCCESS);
}

static mr_status_t
scm_print_node (mr_ra_ptrdes_t * ptrs, int idx, int level, mr_dfs_order_t order, void * context)
{
  mr_rarray_t * mr_ra_str = context;

  switch (order)
    {
    case MR_DFS_PRE_ORDER:
      return (scm_pre_print_node (ptrs, idx, level, mr_ra_str));
    case MR_DFS_POST_ORDER:
      return (scm_post_print_node (ptrs, idx, level, mr_ra_str));
    default:
      return (MR_FAILURE);
    }
}

/**
 * Public function. Save scheduler. Save any object as a string.
 * @param ptrs resizeable array with pointers descriptors
 * @return stringified representation of object
 */
char *
mr_scm_save (mr_ra_ptrdes_t * ptrs)
{
  mr_rarray_t mr_ra_str = {
    .data = { mr_strdup ("") },
    .MR_SIZE = sizeof (""),
    .type = "string",
    .alloc_size = sizeof (""),
  };

  if (NULL == mr_ra_str.data.ptr)
    return (NULL);

  mr_ptrs_dfs (ptrs, scm_print_node, &mr_ra_str);

  return (mr_ra_str.data.ptr);
}
