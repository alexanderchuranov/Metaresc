/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <metaresc.h>
#include <mr_stringify.h>

TYPEDEF_FUNC (int, scm_save_handler_t, (mr_rarray_t *, mr_ptrdes_t *))

#define MR_SCM_EMPTY "()"
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

#define MR_SCM_UNNAMED_FIELDS (0)
#define MR_SCM_NAMED_FIELDS (!MR_SCM_UNNAMED_FIELDS)

static int scm_named_fields[MR_TYPE_LAST] = {
  [0 ... MR_TYPE_LAST - 1] = MR_SCM_UNNAMED_FIELDS,
  [MR_TYPE_STRUCT] = MR_SCM_NAMED_FIELDS,
  [MR_TYPE_UNION] = MR_SCM_NAMED_FIELDS,
  [MR_TYPE_ANON_UNION] = MR_SCM_NAMED_FIELDS,
  [MR_TYPE_NAMED_ANON_UNION] = MR_SCM_NAMED_FIELDS,
};

/**
 * MR_NONE type saving handler.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors
 * @return stringified int value
 */
static int
scm_save_none (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (0);
}

#define MR_SCM_BITMASK_OR_DELIMITER " "
#define MR_SCM_BITMASK_TEMPLATE "(logior %s)"
#define MR_SCM_COMPLEX_FLOAT_TEMPLATE "(+ %s %si)"

static int
scm_save_float (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf (mr_ra_str, "%.8g", *(float *)ptrdes->data.ptr));
}

static int
scm_save_double (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf (mr_ra_str, "%.17g", *(double *)ptrdes->data.ptr));
}

static int
scm_save_long_double_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf (mr_ra_str, "%.20Lg", *(long double *)ptrdes->data.ptr));
}

static int
scm_save_complex_float (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf (mr_ra_str, "(+ %.8g %.8gi)",
			__real__ *(complex float *)ptrdes->data.ptr,
			__imag__ *(complex float *)ptrdes->data.ptr));
}

static int
scm_save_complex_double (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf (mr_ra_str, "(+ %.17g %.17gi)",
			__real__ *(complex double *)ptrdes->data.ptr,
			__imag__ *(complex double *)ptrdes->data.ptr));
}

static int
scm_save_complex_long_double_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf (mr_ra_str, "(+ %.20Lg %.20Lgi)",
			__real__ *(complex long double *)ptrdes->data.ptr,
			__imag__ *(complex long double *)ptrdes->data.ptr));
}

static int
scm_save_int8_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf (mr_ra_str, "%" SCNi8, *(int8_t *)ptrdes->data.ptr));
}

static int
scm_save_uint8_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf (mr_ra_str, "%" SCNu8, *(uint8_t *)ptrdes->data.ptr));
}

static int
scm_save_int16_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf (mr_ra_str, "%" SCNi16, *(int16_t *)ptrdes->data.ptr));
}

static int
scm_save_uint16_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf (mr_ra_str, "%" SCNu16, *(uint16_t *)ptrdes->data.ptr));
}

static int
scm_save_int32_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf (mr_ra_str, "%" SCNi32, *(int32_t *)ptrdes->data.ptr));
}

static int
scm_save_uint32_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf (mr_ra_str, "%" SCNu32, *(uint32_t *)ptrdes->data.ptr));
}

static int
scm_save_int64_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf (mr_ra_str, "%" SCNi64, *(int64_t *)ptrdes->data.ptr));
}

static int
scm_save_uint64_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (mr_ra_printf (mr_ra_str, "%" SCNu64, *(uint64_t *)ptrdes->data.ptr));
}

static int
scm_save_enum (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  mr_td_t * tdp = mr_get_td_by_name (ptrdes->fd.type); /* look up for type descriptor */
  /* check whether type descriptor was found */
  if (NULL == tdp)
    MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_NO_TYPE_DESCRIPTOR, ptrdes->fd.type);
  else if (MR_TYPE_ENUM != tdp->mr_type)
    MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_TYPE_NOT_ENUM, ptrdes->fd.type);
  else
    {
      int64_t value = mr_get_enum_value (tdp, ptrdes->data.ptr);
      mr_fd_t * fdp = mr_get_enum_by_value (tdp, value);
      if (fdp && fdp->name.str)
	return (mr_ra_printf (mr_ra_str, "%s", fdp->name.str));
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_SAVE_ENUM, value, tdp->type.str, ptrdes->fd.name.str);
      ptrdes->fd.size = tdp->size_effective;
    }
  /* save as integer otherwise */
  switch (ptrdes->fd.size)
    {
    case sizeof (uint8_t): return (mr_ra_printf (mr_ra_str, "%" SCNu8, *(uint8_t *)ptrdes->data.ptr));
    case sizeof (uint16_t): return (mr_ra_printf (mr_ra_str, "%" SCNu16, *(uint16_t *)ptrdes->data.ptr));
    case sizeof (uint32_t): return (mr_ra_printf (mr_ra_str, "%" SCNu32, *(uint32_t *)ptrdes->data.ptr));
    case sizeof (uint64_t): return (mr_ra_printf (mr_ra_str, "%" SCNu64, *(uint64_t *)ptrdes->data.ptr));
    }
  
  return (-1);
}

static int
scm_save_bool (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  if (*(bool*)ptrdes->data.ptr)
    return (mr_ra_printf (mr_ra_str, MR_SCM_TRUE));
  else
    return (mr_ra_printf (mr_ra_str, MR_SCM_FALSE));
}

static int
scm_save_bitfield (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  mr_ptrdes_t ptrdes_;
  uint64_t value;

  mr_save_bitfield_value (ptrdes, &value);
  ptrdes_ = *ptrdes;
  ptrdes_.data.ptr = &value;

  switch (ptrdes->fd.mr_type_aux)
    {
    case MR_TYPE_BOOL: return (scm_save_bool (mr_ra_str, &ptrdes_));
    case MR_TYPE_INT8: return (scm_save_int8_t (mr_ra_str, &ptrdes_));
    case MR_TYPE_UINT8: return (scm_save_uint8_t (mr_ra_str, &ptrdes_));
    case MR_TYPE_INT16: return (scm_save_int16_t (mr_ra_str, &ptrdes_));
    case MR_TYPE_UINT16: return (scm_save_uint16_t (mr_ra_str, &ptrdes_));
    case MR_TYPE_INT32: return (scm_save_int32_t (mr_ra_str, &ptrdes_));
    case MR_TYPE_UINT32: return (scm_save_uint32_t (mr_ra_str, &ptrdes_));
    case MR_TYPE_INT64: return (scm_save_int64_t (mr_ra_str, &ptrdes_));
    case MR_TYPE_UINT64: return (scm_save_uint64_t (mr_ra_str, &ptrdes_));
    case MR_TYPE_ENUM: return (scm_save_enum (mr_ra_str, &ptrdes_));
    default: break;
    }
  return (-1);
}  

/**
 * MR_BITMASK type saving handler. Look up type descriptor and save as
 * stringified bitmask value or as integer otherwise.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors
 * @return stringified enum value
 */
static int
scm_save_bitmask (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (scm_save_enum (mr_ra_str, ptrdes));
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

  void * func = *(void**)ptrdes->data.ptr;
#ifdef HAVE_LIBDL
  Dl_info info;
  memset (&info, 0, sizeof (info));
  if (0 != dladdr (func, &info))
    {
      if (info.dli_sname && (func == info.dli_saddr)) /* found some non-null name and address matches */
	{
	  void * func_ = dlsym (RTLD_DEFAULT, info.dli_sname); /* try backward resolve. MAC OS X could resolve static functions, but can't make backward resolution */
	  if (func_ == func)
	    return (mr_ra_printf (mr_ra_str, "%s", info.dli_sname));
	}
    }
#endif /* HAVE_LIBDL */
  return (mr_ra_printf (mr_ra_str, "%p", func));
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

static int
mr_ra_append_char (mr_rarray_t * mr_ra_str, char c)
{
  if ((0 == mr_ra_str->MR_SIZE) || (NULL == mr_ra_str->data.ptr))
    goto free_mr_ra;

  char * tail = mr_rarray_append (mr_ra_str, sizeof (c));

  if (NULL == tail)
    goto free_mr_ra;

  tail[-1] = c;
  tail[0] = 0;
  return (1);

 free_mr_ra:
  if (mr_ra_str->data.ptr)
    MR_FREE (mr_ra_str->data.ptr);
  mr_ra_str->data.ptr = NULL;
  mr_ra_str->MR_SIZE = mr_ra_str->alloc_size = 0;
  return (-1);
}

/**
 * Quote string.
 * @param mr_ra_str output buffer
 * @param str string pointer
 * @return number of outputed chars
 */
#define TRY_CATCH_THROW(ADD) ({			\
      int added = ADD;				\
      if (added < 0)				\
	return (added);				\
      added;					\
    })

static int
scm_quote_string (mr_rarray_t * mr_ra_str, char * str)
{
  char * ptr;
  int count = 0;

  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '"'));
  
  for (ptr = str; *ptr; ++ptr)
    {
      if (('"' == *ptr) || ('\\' == *ptr))
	count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '\\'));
      
      if (isprint (*ptr))
	count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, *ptr));
      else
	count += TRY_CATCH_THROW (mr_ra_printf (mr_ra_str, "\\x%02x;", *(unsigned char *)ptr));
    }
  
  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '"'));
  return (count);
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
  return (scm_quote_string (mr_ra_str, ptrdes->data.ptr));
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
    return (scm_quote_string (mr_ra_str, str));
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
static scm_save_handler_t scm_save_handler[] =
  {
    [MR_TYPE_NONE] = scm_save_none,
    [MR_TYPE_VOID] = scm_save_none,
    [MR_TYPE_ENUM] = scm_save_enum,
    [MR_TYPE_BITFIELD] = scm_save_bitfield,
    [MR_TYPE_BITMASK] = scm_save_bitmask,
    [MR_TYPE_BOOL] = scm_save_bool,
    [MR_TYPE_INT8] = scm_save_int8_t,
    [MR_TYPE_UINT8] = scm_save_uint8_t,
    [MR_TYPE_INT16] = scm_save_int16_t,
    [MR_TYPE_UINT16] = scm_save_uint16_t,
    [MR_TYPE_INT32] = scm_save_int32_t,
    [MR_TYPE_UINT32] = scm_save_uint32_t,
    [MR_TYPE_INT64] = scm_save_int64_t,
    [MR_TYPE_UINT64] = scm_save_uint64_t,
    [MR_TYPE_FLOAT] = scm_save_float,
    [MR_TYPE_COMPLEX_FLOAT] = scm_save_complex_float,
    [MR_TYPE_DOUBLE] = scm_save_double,
    [MR_TYPE_COMPLEX_DOUBLE] = scm_save_complex_double,
    [MR_TYPE_LONG_DOUBLE] = scm_save_long_double_t,
    [MR_TYPE_COMPLEX_LONG_DOUBLE] = scm_save_complex_long_double_t,
    [MR_TYPE_CHAR] = scm_save_char,
    [MR_TYPE_CHAR_ARRAY] = scm_save_char_array,
    [MR_TYPE_STRING] = scm_save_string,
    [MR_TYPE_STRUCT] = scm_save_none,
    [MR_TYPE_FUNC] = scm_save_func,
    [MR_TYPE_FUNC_TYPE] = scm_save_func,
    [MR_TYPE_ARRAY] = scm_save_none,
    [MR_TYPE_POINTER] = scm_save_pointer,
    [MR_TYPE_UNION] = scm_save_none,
    [MR_TYPE_ANON_UNION] = scm_save_none,
    [MR_TYPE_NAMED_ANON_UNION] = scm_save_none,
  };

/**
 * Public function. Save scheduler. Save any object as a string.
 * @param ptrs resizeable array with pointers descriptors
 * @return stringified representation of object
 */
char *
scm_save (mr_ra_ptrdes_t * ptrs)
{
  mr_rarray_t mr_ra_str = { .data = { mr_strdup ("") }, .MR_SIZE = sizeof (""), .type = "char", .alloc_size = sizeof (""), };
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
	  scm_save_handler_t save_handler = scm_save_none;
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
