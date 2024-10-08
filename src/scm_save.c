/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <metaresc.h>
#include <mr_save.h>
#include <mr_stringify.h>

#define MR_SCM_FALSE "nil"
#define MR_SCM_TRUE "t"

#define MR_SCM_INDENT_SPACES (2)
#define MR_SCM_INDENT_TEMPLATE "\n%*s"

#define MR_SCM_NAMED_FIELD "(%s . "
#define MR_SCM_ATTR_INT ";(%s . %" SCNu32 ")\n"

#define MR_SCM_BITMASK_OR_DELIMITER " | "

#define SCM_SAVE_COMPLEX(TYPE)						\
  static int scm_save_complex_ ## TYPE (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes) \
  {									\
    int count = 0;							\
    count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '"'));	\
    count += TRY_CATCH_THROW (mr_ra_printf_complex_ ## TYPE (mr_ra_str, ptrdes, " + ")); \
    count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '"'));	\
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
  int count = 0;
  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '"'));
  count += TRY_CATCH_THROW (mr_ra_printf_bitmask (mr_ra_str, ptrdes, MR_SCM_BITMASK_OR_DELIMITER));
  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '"'));
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
    case MR_TYPE_INT8: return (mr_ra_printf_int8_t (mr_ra_str, &_ptrdes));
    case MR_TYPE_UINT8: return (mr_ra_printf_uint8_t (mr_ra_str, &_ptrdes));
    case MR_TYPE_INT16: return (mr_ra_printf_int16_t (mr_ra_str, &_ptrdes));
    case MR_TYPE_UINT16: return (mr_ra_printf_uint16_t (mr_ra_str, &_ptrdes));
    case MR_TYPE_INT32: return (mr_ra_printf_int32_t (mr_ra_str, &_ptrdes));
    case MR_TYPE_UINT32: return (mr_ra_printf_uint32_t (mr_ra_str, &_ptrdes));
    case MR_TYPE_INT64: return (mr_ra_printf_int64_t (mr_ra_str, &_ptrdes));
    case MR_TYPE_UINT64: return (mr_ra_printf_uint64_t (mr_ra_str, &_ptrdes));
    case MR_TYPE_INT128: return (mr_ra_printf_int128_t (mr_ra_str, &_ptrdes));
    case MR_TYPE_UINT128: return (mr_ra_printf_uint128_t (mr_ra_str, &_ptrdes));
    default:
      break;
    }
  return (0);
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
  if (ptrdes->flags & MR_IS_NULL)
    return (mr_ra_append_string (mr_ra_str, MR_SCM_FALSE));

  const char * func_str = mr_serialize_func (*(void**)ptrdes->data.ptr);
  if (NULL == func_str)
    return (mr_ra_printf (mr_ra_str, "#x%llx", (long long unsigned int)*(uintptr_t*)ptrdes->data.ptr));

  int count = 0;
  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '"'));
  count += TRY_CATCH_THROW (mr_ra_append_string (mr_ra_str, (char*)func_str));
  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '"'));
  return (count);
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
  char buffer[ptrdes->MR_SIZE + 1];
  strncpy (buffer, ptrdes->data.ptr, ptrdes->MR_SIZE);
  buffer[ptrdes->MR_SIZE] = 0;
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
  if (ptrdes->flags & (MR_IS_NULL | MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE))
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
  if (ptrdes->flags & (MR_IS_NULL | MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE))
    return (mr_ra_append_string (mr_ra_str, MR_SCM_FALSE));
  return (0);
}

static int
scm_printf_compaund (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  if (ptrdes->first_child == MR_NULL_IDX)
    return (mr_ra_append_string (mr_ra_str, "()"));
  return (0);
}

static int
scm_printf_void (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (0);
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
    [MR_TYPE_STRUCT] = scm_printf_compaund,
    [MR_TYPE_FUNC] = scm_save_func,
    [MR_TYPE_FUNC_TYPE] = scm_save_func,
    [MR_TYPE_ARRAY] = scm_printf_compaund,
    [MR_TYPE_POINTER] = scm_save_pointer,
    [MR_TYPE_UNION] = scm_printf_compaund,
    [MR_TYPE_ANON_UNION] = scm_printf_compaund,
    [MR_TYPE_NAMED_ANON_UNION] = scm_printf_compaund,
  };

static mr_status_t
scm_pre_print_node (mr_ra_ptrdes_t * ptrs, mr_idx_t idx, int level, mr_rarray_t * mr_ra_str)
{
  int limit_level = MR_LIMIT_LEVEL (level);
  int count = 0;

  if (ptrs->ra[idx].flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE))
    {
      if (mr_ra_str->data.string[mr_ra_str->mr_size - 2] == '\n')
	mr_ra_str->mr_size--;
      count = mr_ra_printf (mr_ra_str, MR_SCM_INDENT_TEMPLATE MR_SCM_ATTR_INT,
			    limit_level * MR_SCM_INDENT_SPACES, "",
			    (ptrs->ra[idx].flags & MR_IS_CONTENT_REFERENCE) ? MR_REF_CONTENT : MR_REF,
			    (uint32_t)ptrs->ra[ptrs->ra[idx].first_child].idx);
      if (count < 0)
	return (MR_FAILURE);
    }

  if (ptrs->ra[idx].flags & MR_IS_REFERENCED)
    {
      if (mr_ra_str->data.string[mr_ra_str->mr_size - 2] == '\n')
	mr_ra_str->mr_size--;
      count = mr_ra_printf (mr_ra_str, MR_SCM_INDENT_TEMPLATE MR_SCM_ATTR_INT,
			    limit_level * MR_SCM_INDENT_SPACES, "",
			    MR_REF_IDX, (uint32_t)ptrs->ra[idx].idx);
      if (count < 0)
	return (MR_FAILURE);
    }

  mr_idx_t parent = ptrs->ra[idx].parent;
  /* members of union mr_ptr_t should be named */
  if ((parent != MR_NULL_IDX) && ptrs->ra[parent].fdp)
    if ((MR_TYPE_UNION == ptrs->ra[parent].mr_type) && ptrs->ra[parent].fdp->stype.tdp)
      if (0 == strcmp (ptrs->ra[parent].fdp->stype.tdp->type.str, "mr_ptr_t"))
	ptrs->ra[idx].flags &= ~MR_IS_UNNAMED;

  if (ptrs->ra[idx].flags & MR_IS_UNNAMED)
    {
      if (mr_ra_str->data.string[mr_ra_str->mr_size - 2] == '\n')
	{
	  mr_ra_str->mr_size--;
	  count = mr_ra_printf (mr_ra_str, MR_SCM_INDENT_TEMPLATE,
				limit_level * MR_SCM_INDENT_SPACES, "");
	}
      else if ((ptrs->ra[idx].parent != MR_NULL_IDX) && (ptrs->ra[ptrs->ra[idx].parent].first_child != idx))
	count = mr_ra_append_char (mr_ra_str, ' ');
    }
  else
    {
      if (mr_ra_str->data.string[mr_ra_str->mr_size - 2] == '\n')
	mr_ra_str->mr_size--;
      count = mr_ra_printf (mr_ra_str, MR_SCM_INDENT_TEMPLATE MR_SCM_NAMED_FIELD,
			    limit_level * MR_SCM_INDENT_SPACES, "", ptrs->ra[idx].fdp->name.str);
    }

  if (count < 0)
    return (MR_FAILURE);

  if ((ptrs->ra[idx].first_child != MR_NULL_IDX) && !(ptrs->ra[idx].flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE)))
    {
      if (mr_ra_str->data.string[mr_ra_str->mr_size - 2] == '\n')
	{
	  mr_ra_str->mr_size--;
	  count = mr_ra_printf (mr_ra_str, MR_SCM_INDENT_TEMPLATE, limit_level * MR_SCM_INDENT_SPACES, "");
	  if (count < 0)
	    return (MR_FAILURE);
	}
      count = mr_ra_append_char (mr_ra_str, '(');
      if (count < 0)
	return (MR_FAILURE);
    }

  if ((ptrs->ra[idx].flags & MR_IS_UNNAMED) && (ptrs->ra[idx].first_child == MR_NULL_IDX) && (mr_ra_str->data.string[mr_ra_str->mr_size - 2] == '\n'))
    {
      mr_ra_str->mr_size--;
      count = mr_ra_printf (mr_ra_str, MR_SCM_INDENT_TEMPLATE, limit_level * MR_SCM_INDENT_SPACES, "");
      if (count < 0)
	return (MR_FAILURE);
    }

  mr_ra_printf_t save_handler = NULL;
  /* route saving handler */
  if ((ptrs->ra[idx].mr_type >= 0) && (ptrs->ra[idx].mr_type < MR_TYPE_LAST))
    save_handler = scm_save_handler[ptrs->ra[idx].mr_type];

  if (NULL == save_handler)
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNSUPPORTED_NODE_TYPE, ptrs->ra[idx].mr_type);
      return (MR_FAILURE);
    }

  count = save_handler (mr_ra_str, &ptrs->ra[idx]);

  return ((count >= 0) ? MR_SUCCESS : MR_FAILURE);
}

static mr_status_t
scm_post_print_node (mr_ra_ptrdes_t * ptrs, mr_idx_t idx, int level, mr_rarray_t * mr_ra_str)
{
  int limit_level = MR_LIMIT_LEVEL (level);
  int count;

  if (!(ptrs->ra[idx].flags & MR_IS_UNNAMED) ||
      ((ptrs->ra[idx].first_child != MR_NULL_IDX) && !(ptrs->ra[idx].flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE))))
    {
      if (mr_ra_str->data.string[mr_ra_str->mr_size - 2] == '\n')
	{
	  mr_ra_str->mr_size--;
	  count = mr_ra_printf (mr_ra_str, MR_SCM_INDENT_TEMPLATE, limit_level * MR_SCM_INDENT_SPACES, "");
	  if (count < 0)
	    return (MR_FAILURE);
	}
      count = mr_ra_printf (mr_ra_str, ")%s\n",
			    (!(ptrs->ra[idx].flags & (MR_IS_UNNAMED | MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE))
			     && (ptrs->ra[idx].first_child != MR_NULL_IDX)) ? ")" : "");
      if (count < 0)
	return (MR_FAILURE);
    }

  return (MR_SUCCESS);
}

static mr_status_t
scm_print_node (mr_ra_ptrdes_t * ptrs, mr_idx_t idx, int level, mr_dfs_order_t order, void * context)
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
    .data.string = mr_strdup ("\n"),
    .MR_SIZE = sizeof ("\n"),
    .type = "string",
    .alloc_size = sizeof ("\n"),
  };

  if (NULL == mr_ra_str.data.string)
    return (NULL);

  mr_ptrs_dfs (ptrs, scm_print_node, &mr_ra_str);

  if (NULL != mr_ra_str.data.string)
    memmove (mr_ra_str.data.string, &mr_ra_str.data.string[1], mr_ra_str.mr_size - 1);
  return (mr_ra_str.data.string);
}
