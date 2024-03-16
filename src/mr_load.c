/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */
#ifdef HAVE_CONFIG_H
#include <mr_config.h>
#endif /* HAVE_CONFIG_H */
#include <stdio.h>
#include <string.h>
#ifdef HAVE_DLFCN_H
#define __USE_GNU
#include <dlfcn.h>
#endif /* HAVE_DLFCN_H */

#include <metaresc.h>
#include <mr_stringify.h>
#include <mr_ic.h>
#include <mr_save.h>
#include <mr_load.h>
#include <mr_value.h>
#include <flt_values.h>
#include <lexer.h>

TYPEDEF_STRUCT (mr_load_node_context_t,
		int idx,
		(mr_ra_ptrdes_t *, ptrs)
		);

TYPEDEF_FUNC (mr_status_t, mr_load_handler_t, (int /* idx */, mr_ra_ptrdes_t * /* ptrs */));

/**
 * Post load references setting. If node was marked as references
 * it should be substitute with actual pointer. This substition
 * can't be made during structure loading because of forward references.
 * @param mr_load_data structures that holds context of loading
 * @return Status.
 */
static mr_status_t
mr_set_crossrefs (mr_ra_ptrdes_t * ptrs)
{
  int i;
  int count = ptrs->size / sizeof (ptrs->ra[0]);
  int max_idx = 0;
  int * table;

  for (i = 0; i < count; ++i)
    if (ptrs->ra[i].idx > max_idx)
      max_idx = ptrs->ra[i].idx;

  table = MR_CALLOC (max_idx + 1, sizeof (table[0]));
  if (NULL == table)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (MR_FAILURE);
    }

  for (i = 0; i <= max_idx; ++i)
    table[i] = -1;
  for (i = 0; i < count; ++i)
    if (ptrs->ra[i].idx >= 0)
      {
	/* checking indexes collision */
	if (table[ptrs->ra[i].idx] >= 0)
	  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_IDS_COLLISION, MR_REF_IDX, ptrs->ra[i].idx);
	table[ptrs->ra[i].idx] = i;
      }

  /* set all cross refernces */
  for (i = 0; i < count; ++i)
    if (ptrs->ra[i].ref_idx >= 0)
      {
	if (ptrs->ra[i].ref_idx > max_idx)
	  {
	    MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNDEFINED_REF_IDX, MR_REF_IDX, ptrs->ra[i].ref_idx);
	    continue;
	  }
	
	int idx = table[ptrs->ra[i].ref_idx];
	if (idx < 0)
	  {
	    MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNDEFINED_REF_IDX, MR_REF_IDX, ptrs->ra[i].ref_idx);
	    continue;
	  }

	void * data = ptrs->ra[idx].data.ptr;

	if ((MR_TYPE_POINTER != ptrs->ra[i].mr_type) &&
	    (MR_TYPE_STRING != ptrs->ra[i].mr_type))
	  {
	    MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_TARGET_TYPE, ptrs->ra[i].mr_type);
	    continue;
	  }

	if (ptrs->ra[i].flags.is_content_reference)
	  {
	    if (NULL == data)
	      {
		MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
	      }
	    else
	      data = *(void**)data;
	  }

	if (NULL == ptrs->ra[i].data.ptr)
	  {
	    MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
	    continue;
	  }

	*(void**)ptrs->ra[i].data.ptr = data;
      }
  
  MR_FREE (table);

  return (MR_SUCCESS);
}

/**
 * MR_NONE load handler (dummy)
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read
 */
static mr_status_t
mr_load_none (int idx, mr_ra_ptrdes_t * ptrs)
{
  return (MR_SUCCESS);
}

/**
 * MR_INTEGER load handler
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read (0 - failure, !0 - success)
 */
mr_status_t
mr_load_integer (int idx, mr_ra_ptrdes_t * ptrs)
{
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  if (MR_SUCCESS != mr_value_cast (MR_VT_INT, &ptrdes->load_params.mr_value))
    return (MR_FAILURE);

  switch (ptrdes->mr_type)
    {
#define CASE_SET_VALUE_BY_TYPE(TYPE) CASE_SET_VALUE_BY_TYPE_ (TYPE, MR_TYPE_DETECT (TYPE))
#define CASE_SET_VALUE_BY_TYPE_(TYPE, MR_TYPE) case MR_TYPE: *(TYPE*)ptrdes->data.ptr = ptrdes->load_params.mr_value.vt_int; break;
      MR_FOREACH (CASE_SET_VALUE_BY_TYPE, bool, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t);
      CASE_SET_VALUE_BY_TYPE_ (mr_intmax_t, MR_TYPE_INT128);
      CASE_SET_VALUE_BY_TYPE_ (mr_uintmax_t, MR_TYPE_UINT128);
      
    case MR_TYPE_ENUM:
      switch (ptrdes->fdp->size)
	{
#define CASE_SET_VALUE_BY_SIZE(TYPE) case sizeof (TYPE): *(TYPE*)ptrdes->data.ptr = ptrdes->load_params.mr_value.vt_int; break;
	  MR_FOREACH (CASE_SET_VALUE_BY_SIZE, uint8_t, uint16_t, uint32_t, uint64_t);
#ifdef HAVE_INT128
	  MR_FOREACH (CASE_SET_VALUE_BY_SIZE, mr_uint128_t);
#endif /* HAVE_INT128 */

	default:
	  {
	    __typeof__ (ptrdes->load_params.mr_value.vt_int) vt_int = ptrdes->load_params.mr_value.vt_int;
	    memcpy (ptrdes->data.ptr, &vt_int, MR_MIN (ptrdes->fdp->size, sizeof (vt_int)));
	    break;
	  }
	}
      break;
    default:
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_TARGET_TYPE, ptrdes->mr_type);
      return (MR_FAILURE);
    }
  return (MR_SUCCESS);
}

static mr_status_t
mr_get_func_wrapper (char * func_name, void * dst)
{
  if (NULL == func_name)
    return (MR_SUCCESS);

  void * func_addr = NULL;
#ifdef HAVE_LIBDL
  func_addr = dlsym (RTLD_DEFAULT, func_name);
#endif /* HAVE_LIBDL */
  if (func_addr != NULL)
    *(void**)dst = func_addr;
  else
    {
      char * end = NULL;
      mr_uintmax_t ptr = mr_strtouintmax (func_name, &end, 0);

      while (isspace (*end))
	++end;
      if (*end != 0)
	{
	  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_READ_FUNC, func_name);
	  return (MR_FAILURE);
	}

      *(void**)dst = (void*)(uintptr_t)ptr;
    }

  return (MR_SUCCESS);
}

#ifndef BUF_SIZE_ON_STACK
#define BUF_SIZE_ON_STACK (1 << 12)
#endif /* BUF_SIZE_ON_STACK */

mr_status_t
mr_process_quoted_str (mr_quoted_substr_t * quoted_substr, mr_process_quoted_str_t process_quoted_str, void * arg)
{
  if (NULL == quoted_substr->substr.str)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
      return (MR_FAILURE);
    }

  mr_status_t status;

  if ((NULL == quoted_substr->unquote) &&
      (0 == quoted_substr->substr.str[quoted_substr->substr.length]))
    status = process_quoted_str (quoted_substr->substr.str, arg);
  else
    {
      char * dst = NULL;
      int size = quoted_substr->substr.length + 1;
      char buf[BUF_SIZE_ON_STACK];
      
      if (size >= BUF_SIZE_ON_STACK)
	dst = MR_CALLOC (size, sizeof (*dst));
      else
	dst = buf;

      if (NULL == dst)
	{
	  MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	  return (MR_FAILURE);
	}
      
      if (NULL == quoted_substr->unquote)
	{
	  memcpy (dst, quoted_substr->substr.str, quoted_substr->substr.length);
	  dst[quoted_substr->substr.length] = 0;
	}
      else
	quoted_substr->unquote (&quoted_substr->substr, dst);
      status = process_quoted_str (dst, arg);

      if (size >= BUF_SIZE_ON_STACK)
	MR_FREE (dst);
    }

  return (status);
}

/**
 * MR_FUNC load handler
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read (0 - failure, !0 - success)
 */
mr_status_t
mr_load_func (int idx, mr_ra_ptrdes_t * ptrs)
{
  mr_status_t status = MR_SUCCESS;
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];

  *(void**)ptrdes->data.ptr = NULL;
  if (!ptrdes->flags.is_null)
    switch (ptrdes->load_params.mr_value.value_type)
      {
      case MR_VT_INT:
	*(void**)ptrdes->data.ptr = (void*)(uintptr_t)ptrdes->load_params.mr_value.vt_int;
	break;
      case MR_VT_ID:
      case MR_VT_QUOTED_SUBSTR:
	status = mr_process_quoted_str (&ptrdes->load_params.mr_value.vt_quoted_substr, mr_get_func_wrapper, ptrdes->data.ptr);
	break;
      case MR_VT_STRING:
	status = mr_get_func_wrapper (ptrdes->load_params.mr_value.vt_string, ptrdes->data.ptr);
	break;
      default:
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_TARGET_TYPE, ptrdes->load_params.mr_value.value_type);
	status = MR_FAILURE;
	break;
      }
  return (status);
}

/**
 * MR_BITFIELD load handler. Load int from string and save it to
 * bit shifted field.
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read
 */
static mr_status_t
mr_load_bitfield (int idx, mr_ra_ptrdes_t * ptrs)
{
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  mr_uintmax_t value;

  if (MR_SUCCESS != mr_value_cast (MR_VT_INT, &ptrdes->load_params.mr_value))
    return (MR_FAILURE);

  value = ptrdes->load_params.mr_value.vt_int;
  return (mr_load_bitfield_value (ptrdes, &value));
}

static mr_status_t
mr_load_float (int idx, mr_ra_ptrdes_t * ptrs)
{
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  if (MR_SUCCESS != mr_value_cast (MR_VT_FLOAT, &ptrdes->load_params.mr_value))
    return (MR_FAILURE);

  switch (ptrdes->mr_type)
    {
#define CASE_SET_FLOAT_BY_TYPE(TYPE) case MR_TYPE_DETECT (TYPE): *(TYPE*)ptrdes->data.ptr = ptrdes->load_params.mr_value.vt_float; break;
      MR_FOREACH (CASE_SET_FLOAT_BY_TYPE, float, double, long double);

    default:
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_TARGET_TYPE, ptrdes->load_params.mr_value.value_type);
      return (MR_FAILURE);
    }
  return (MR_SUCCESS);
}

static mr_status_t
mr_load_complex (int idx, mr_ra_ptrdes_t * ptrs)
{
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  if (MR_SUCCESS != mr_value_cast (MR_VT_COMPLEX, &ptrdes->load_params.mr_value))
    return (MR_FAILURE);

  complex_long_double_t cld = MR_CLD_UNPACK (ptrdes->load_params.mr_value.vt_complex);
  switch (ptrdes->mr_type)
    {
#define CASE_SET_COMPLEX_BY_TYPE(TYPE) case MR_TYPE_DETECT (TYPE): *(TYPE*)ptrdes->data.ptr = cld; break;
      MR_FOREACH (CASE_SET_COMPLEX_BY_TYPE, complex_float_t, complex_double_t, complex_long_double_t);

    default:
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_TARGET_TYPE, ptrdes->load_params.mr_value.value_type);
      return (MR_FAILURE);
    }
  return (MR_SUCCESS);
}

static mr_status_t
mr_get_char (char * src, void * dst)
{
  if (NULL == src)
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
      return (MR_FAILURE);
    }
  if (src[0] && src[1])
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_READ_CHAR, src);
      return (MR_FAILURE);
    }    
  
  *(char*)dst = *src;
  return (MR_SUCCESS);
}

/**
 * MR_CHAR load handler. Handles nonprint characters in octal format.
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read
 */
static mr_status_t
mr_load_char (int idx, mr_ra_ptrdes_t * ptrs)
{
  mr_status_t status = MR_SUCCESS;
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  
  switch (ptrdes->load_params.mr_value.value_type)
    {
    case MR_VT_QUOTED_SUBSTR:
      status = mr_process_quoted_str (&ptrdes->load_params.mr_value.vt_quoted_substr, mr_get_char, ptrdes->data.ptr);
      break;

    case MR_VT_CHAR:
      *(char*)ptrdes->data.ptr = ptrdes->load_params.mr_value.vt_char;
      break;
      
    case MR_VT_STRING:
      if (NULL == ptrdes->load_params.mr_value.vt_string)
	{
	  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
	  status = MR_FAILURE;
	  break;
	}

      if (strlen (ptrdes->load_params.mr_value.vt_string) == sizeof (char))
	{
	  *(char*)ptrdes->data.ptr = ptrdes->load_params.mr_value.vt_string[0];
	  break;
	}
      __attribute__ ((fallthrough));

    default:
      if (MR_SUCCESS == mr_value_cast (MR_VT_INT, &ptrdes->load_params.mr_value))
	*(char*)ptrdes->data.ptr = ptrdes->load_params.mr_value.vt_int;
      else
	status = MR_FAILURE;
      break;
    }

  return (status);
}

static mr_status_t
mr_get_str (char * src, void * dst)
{
  *(char**)dst = mr_strdup (src);
  return ((NULL == *(char**)dst) ? MR_FAILURE : MR_SUCCESS);
}

/**
 * MR_STRING load handler. Allocate memory for a string.
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read
 */
static mr_status_t
mr_load_string (int idx, mr_ra_ptrdes_t * ptrs)
{
  mr_status_t status = MR_SUCCESS;
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  
  *(char**)ptrdes->data.ptr = NULL;
  if (!ptrdes->flags.is_null && (ptrdes->ref_idx < 0))
    {
      switch (ptrdes->load_params.mr_value.value_type)
	{
	case MR_VT_QUOTED_SUBSTR:
	  status = mr_process_quoted_str (&ptrdes->load_params.mr_value.vt_quoted_substr, mr_get_str, ptrdes->data.ptr);
	  break;
	  
	case MR_VT_STRING:
	  *(char**)ptrdes->data.ptr = ptrdes->load_params.mr_value.vt_string;
	  ptrdes->load_params.mr_value.vt_string = NULL;
	  break;

	case MR_VT_INT:
	  if (ptrdes->load_params.mr_value.vt_int >= 0)
	    ptrdes->ref_idx = ptrdes->load_params.mr_value.vt_int;
	  else
	    {
	      ptrdes->ref_idx = -ptrdes->load_params.mr_value.vt_int;
	      ptrdes->flags.is_content_reference = true;
	    }
	  break;
	  
	default:
	  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_TARGET_TYPE, ptrdes->load_params.mr_value.value_type);
	  status = MR_FAILURE;
	  break;
	}
    }
  return (status);
}

static mr_status_t
mr_get_char_array (char * str, void * dst)
{
  mr_load_node_context_t * load_node_context = dst;
  mr_ptrdes_t * ptrdes = &load_node_context->ptrs->ra[load_node_context->idx];
  int max_size = ptrdes->fdp->size;
  mr_status_t status = MR_SUCCESS;

  if (NULL == str)
    return (MR_FAILURE);
  
  int str_len = strlen (str);
  if ((str_len >= max_size) && (ptrdes->parent >= 0))
    if (MR_TYPE_POINTER == load_node_context->ptrs->ra[ptrdes->parent].mr_type)
      {
	void * data = MR_REALLOC (ptrdes->data.ptr, str_len + 1);
	if (NULL == data)
	  {
	    if (ptrdes->data.ptr != NULL)
	      MR_FREE (ptrdes->data.ptr);
	    MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	    status = MR_FAILURE;
	  }
		  
	*(void**)load_node_context->ptrs->ra[ptrdes->parent].data.ptr = ptrdes->data.ptr = data;
	max_size = str_len + 1;
      }

  if (str_len > max_size)
    {
      str_len = max_size;
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_STRING_TRUNCATED);
    }
		  
  if (ptrdes->data.ptr != NULL)
    {
      memcpy (ptrdes->data.string, str, str_len);
      if (str_len < max_size)
	ptrdes->data.string[str_len] = 0;
    }

  return (status);
}  

/**
 * MR_CHAR_ARRAY load handler.
 * Save string in place (truncate string if needed).
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read
 */
static mr_status_t
mr_load_char_array (int idx, mr_ra_ptrdes_t * ptrs)
{
  mr_load_node_context_t load_node_context = {
    .idx = idx,
    .ptrs = ptrs,
  };
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  mr_status_t status = MR_SUCCESS;

  if (ptrdes->fdp->size > 0)
    *(char*)ptrdes->data.ptr = 0;
  
  switch (ptrdes->load_params.mr_value.value_type)
    {
    case MR_VT_QUOTED_SUBSTR:
      status = mr_process_quoted_str (&ptrdes->load_params.mr_value.vt_quoted_substr, mr_get_char_array, &load_node_context);
      break;
    case MR_VT_STRING:
      status = mr_get_char_array (ptrdes->load_params.mr_value.vt_string, &load_node_context);
      break;
    default:
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_TARGET_TYPE, ptrdes->load_params.mr_value.value_type);
      status = MR_FAILURE;
      break;
    }
  
  return (status);
}

static mr_fd_t *
mr_load_struct_next_field (mr_td_t * tdp, mr_fd_t * fdp)
{
  int i, count = tdp->param.struct_param.fields_size / sizeof (tdp->param.struct_param.fields[0]);

  for (i = 0; i < count; ++i)
    if (NULL == fdp)
      return (tdp->param.struct_param.fields[i]);
    else if (tdp->param.struct_param.fields[i] == fdp)
      fdp = NULL;

  return (NULL);
}

/**
 * MR_STRUCT load handler.
 * Save content of subnodes to structure fileds.
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read
 */
static mr_status_t
mr_load_struct (int idx, mr_ra_ptrdes_t * ptrs)
{
  mr_fd_t * fdp = NULL;
  char * data = ptrs->ra[idx].data.ptr;
  int first_child = ptrs->ra[idx].first_child;
  mr_td_t * tdp = ptrs->ra[idx].tdp;
  mr_status_t status = MR_SUCCESS;

  /* get pointer on structure descriptor */
  if (NULL == tdp)
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_NO_TYPE_DESCRIPTOR, "unknown");
      return (MR_FAILURE);
    }

  /* loop on all subnodes */
  for (idx = first_child; (MR_SUCCESS == status) && (idx >= 0); idx = ptrs->ra[idx].next)
    {
      if (NULL == ptrs->ra[idx].name)
	fdp = mr_load_struct_next_field (tdp, fdp);
      else
	fdp = mr_get_fd_by_name (tdp, ptrs->ra[idx].name);

      if (NULL == fdp)
	{
	  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNKNOWN_SUBNODE, tdp->type.str, ptrs->ra[idx].name);
	  status = MR_FAILURE;
	  continue;
	}

      /* recursively load subnode */
      status = mr_load (&data[fdp->offset], fdp, idx, ptrs);
    }
  return (status);
}

/**
 * MR_ARRAY load handler.
 * Save content of subnodes to array elements.
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read
 */
static mr_status_t
mr_load_array (int idx, mr_ra_ptrdes_t * ptrs)
{
  char * data = ptrs->ra[idx].data.ptr;
  mr_fd_t fd_ = *ptrs->ra[idx].fdp;
  mr_status_t status = MR_SUCCESS;
  int i;

  fd_.non_persistent = true;
  fd_.unnamed = true;
  fd_.offset = 0;
  fd_.size = mr_type_size (fd_.mr_type_aux);
  if (fd_.size == 0)
    fd_.size = fd_.tdp ? fd_.tdp->size : 0;
  if (fd_.size == 0)
    return (MR_FAILURE);

  if (fd_.param.array_param.dim.dim[0].is_last)
    {
      fd_.mr_type = fd_.mr_type_aux; /* prepare copy of filed descriptor for array elements loading */
      fd_.mr_type_aux = fd_.mr_type_ptr;
    }
  else
    for (i = 0; i < sizeof (fd_.param.array_param.dim.dim) / sizeof (fd_.param.array_param.dim.dim[0]) - 1; ++i)
      {
	fd_.param.array_param.dim.dim[i] = fd_.param.array_param.dim.dim[i + 1];
	fd_.size *= fd_.param.array_param.dim.dim[i].count;
	if (fd_.param.array_param.dim.dim[i].is_last)
	  break;
      }

  /* loop on subnodes */
  i = 0;
  for (idx = ptrs->ra[idx].first_child; (MR_SUCCESS == status) && (idx >= 0); idx = ptrs->ra[idx].next)
    {
      /* check if array index is in range */
      if (i >= fd_.param.array_param.dim.dim[0].count)
	{
	  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_RANGE_CHECK, fd_.name.str);
	  return (MR_FAILURE);
	}
      /* load recursively */
      status = mr_load (&data[i++ * fd_.size], &fd_, idx, ptrs);
    }
  return (status);
}

void
mr_pointer_set_size (int idx, mr_ra_ptrdes_t * ptrs)
{
  mr_ptrdes_t src, dst;
  mr_pointer_get_size_ptrdes (&dst, idx, ptrs);
      
  if (dst.data.ptr != NULL)
    {
      src.data.ptr = &ptrs->ra[idx].MR_SIZE;
      src.mr_type = MR_TYPE_DETECT (__typeof__ (ptrs->ra[idx].MR_SIZE));
      mr_assign_int (&dst, &src);
    }
}

/**
 * Loads element into newly allocate memory.
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read (0 - failure, !0 - success)
 */
static mr_status_t
mr_load_pointer_postponed (int idx, mr_ra_ptrdes_t * ptrs)
{
  mr_status_t status = MR_SUCCESS;
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  char ** data = ptrs->ra[idx].data.ptr;
  int count = 0;
  int node;

  if (NULL == data)
    return (MR_FAILURE);

  *data = NULL; /* default initializer */
  if (ptrdes->flags.is_null)
    return (MR_SUCCESS);
  
  if (MR_VT_INT == ptrdes->load_params.mr_value.value_type)
    {
      if (ptrdes->load_params.mr_value.vt_int >= 0)
	ptrdes->ref_idx = ptrdes->load_params.mr_value.vt_int;
      else
	{
	  ptrdes->ref_idx = -ptrdes->load_params.mr_value.vt_int;
	  ptrdes->flags.is_content_reference = true;
	}
    }

  if (ptrdes->ref_idx >= 0)
    return (MR_SUCCESS);

  for (node = ptrs->ra[idx].first_child; node >= 0; node = ptrs->ra[node].next)
    ++count;
  if (0 == count)
    return (MR_SUCCESS);

  mr_fd_t fd_;
  memset (&fd_, 0, sizeof (fd_));
  fd_.non_persistent = true;
  fd_.unnamed = true;
  fd_.mr_type = ptrs->ra[idx].mr_type_aux;
  fd_.mr_type_aux = ptrs->ra[idx].tdp ? ptrs->ra[idx].tdp->mr_type : MR_TYPE_VOID;
  fd_.name.str = ptrs->ra[idx].name;
  fd_.tdp = ptrs->ra[idx].tdp;
  fd_.size = mr_type_size (fd_.mr_type);
  if (fd_.size == 0)
    fd_.size = fd_.tdp ? fd_.tdp->size : 0;
  if (fd_.size == 0)
    return (MR_FAILURE);

  /* allocate memory */
  *data = MR_CALLOC (count, fd_.size);
  if (NULL == *data)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (MR_FAILURE);
    }
  
  /* load recursively */
  count = 0;
  for (node = ptrs->ra[idx].first_child; (MR_SUCCESS == status) && (node >= 0); node = ptrs->ra[node].next)
    status = mr_load (*data + count++ * fd_.size, &fd_, node, ptrs);

  ptrs->ra[idx].MR_SIZE = count * fd_.size;
  mr_pointer_set_size (idx, ptrs);
  
  return (status);
}

/**
 * MR_ANON_UNION load handler.
 * Load anonymous union
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read
 */
static mr_status_t
mr_load_anon_union (int idx, mr_ra_ptrdes_t * ptrs)
{
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];

  /*
    Anonimous unions in C init style saved as named field folowed by union itself. Named field has type of zero length static string and must be inited by empty string. Here is an example.
    .anon_union_79 = "", {
    .union_float = 3.1415927410,
    },
  */
  if ((ptrdes->first_child < 0) && /* if node has no childs, then it is C init style anonumous union */
      (MR_VT_QUOTED_SUBSTR == ptrdes->load_params.mr_value.value_type)
      && (0 == ptrdes->load_params.mr_value.vt_quoted_substr.substr.length) && /* content must be an empty string */
      (ptrdes->name != NULL) && /* node must have a name */
      (ptrdes->next >= 0))
    if (NULL == ptrs->ra[ptrdes->next].name) /* there should be a next node without name */
      {
	ptrs->ra[ptrdes->next].name = ptrdes->name;
	return (MR_SUCCESS); /* now next node has a name and will be loaded by top level procedure */
      }
  return (mr_load_struct (idx, ptrs));
}

/**
 * Init IO handlers Table
 */
static mr_load_handler_t mr_load_handler[MR_TYPE_LAST] =
  {
    [MR_TYPE_NONE] = mr_load_none,
    [MR_TYPE_VOID] = mr_load_none,
    [MR_TYPE_ENUM] = mr_load_integer,
    [MR_TYPE_BITFIELD] = mr_load_bitfield,
    [MR_TYPE_BOOL] = mr_load_integer,
    [MR_TYPE_INT8] = mr_load_integer,
    [MR_TYPE_UINT8] = mr_load_integer,
    [MR_TYPE_INT16] = mr_load_integer,
    [MR_TYPE_UINT16] = mr_load_integer,
    [MR_TYPE_INT32] = mr_load_integer,
    [MR_TYPE_UINT32] = mr_load_integer,
    [MR_TYPE_INT64] = mr_load_integer,
    [MR_TYPE_UINT64] = mr_load_integer,
    [MR_TYPE_INT128] = mr_load_integer,
    [MR_TYPE_UINT128] = mr_load_integer,
    [MR_TYPE_FLOAT] = mr_load_float,
    [MR_TYPE_COMPLEX_FLOAT] = mr_load_complex,
    [MR_TYPE_DOUBLE] = mr_load_float,
    [MR_TYPE_COMPLEX_DOUBLE] = mr_load_complex,
    [MR_TYPE_LONG_DOUBLE] = mr_load_float,
    [MR_TYPE_COMPLEX_LONG_DOUBLE] = mr_load_complex,
    [MR_TYPE_CHAR] = mr_load_char,
    [MR_TYPE_CHAR_ARRAY] = mr_load_char_array,
    [MR_TYPE_STRING] = mr_load_string,
    [MR_TYPE_STRUCT] = mr_load_struct,
    [MR_TYPE_FUNC] = mr_load_func,
    [MR_TYPE_FUNC_TYPE] = mr_load_func,
    [MR_TYPE_ARRAY] = mr_load_array,
    [MR_TYPE_POINTER] = mr_load_none,
    [MR_TYPE_UNION] = mr_load_struct,
    [MR_TYPE_ANON_UNION] = mr_load_struct,
    [MR_TYPE_NAMED_ANON_UNION] = mr_load_anon_union,
  };

/**
 * Public function. Load router. Load any object from internal representation graph.
 * @param data pointer on place to save data
 * @param fdp filed descriptor
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read
 */
mr_status_t
mr_load (void * data, mr_fd_t * fdp, int idx, mr_ra_ptrdes_t * ptrs)
{
  mr_status_t status = MR_FAILURE;

  if (NULL == data)
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_NULL_POINTER);
      return (MR_FAILURE);
    }

  if ((idx < 0) || (idx >= ptrs->size / sizeof (ptrs->ra[0])))
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_SAVE_IDX_RANGE_CHECK);
      return (MR_FAILURE);
    }

  if (ptrs->ra[idx].name && fdp->name.str && !fdp->unnamed)
    if (0 != strcmp (fdp->name.str, ptrs->ra[idx].name))
      {
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_NODE_NAME_MISSMATCH, fdp->name.str, ptrs->ra[idx].name);
	return (MR_FAILURE);
      }

  if (ptrs->ra[idx].tdp && fdp->tdp)
    if (ptrs->ra[idx].tdp != fdp->tdp)
      {
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_NODE_TYPE_MISSMATCH, fdp->name.str, fdp->type, ptrs->ra[idx].tdp->type.str);
	return (MR_FAILURE);
      }

  ptrs->ra[idx].data.ptr = data;
  ptrs->ra[idx].fdp = fdp;
  ptrs->ra[idx].non_persistent = fdp->non_persistent;
  ptrs->ra[idx].mr_size = fdp->size;
  ptrs->ra[idx].mr_type = fdp->mr_type;
  ptrs->ra[idx].mr_type_aux = fdp->mr_type_aux;
  ptrs->ra[idx].tdp = fdp->tdp;
  ptrs->ra[idx].name = fdp->name.str;

  /* route loading */
  mr_load_handler_t load_handler = NULL;
  if ((fdp->mr_type >= 0) && (fdp->mr_type < MR_TYPE_LAST))
    load_handler = mr_load_handler[fdp->mr_type];

  if (load_handler != NULL)
    status = load_handler (idx, ptrs);
  else
    MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNSUPPORTED_NODE_TYPE, fdp->mr_type);

  /* set cross references at the upper level */
  if (0 == idx)
    {
      int i, count = ptrs->size / sizeof (ptrs->ra[0]);

      for (i = 0; (MR_SUCCESS == status) && (i < count); ++i)
	if (MR_TYPE_POINTER == ptrs->ra[i].mr_type)
	  status = mr_load_pointer_postponed (i, ptrs);
      
      if (MR_SUCCESS == status)
	status = mr_set_crossrefs (ptrs);

      if (MR_SUCCESS != status)
	for (i = count - 1; i >= 0; --i)
	  if (((MR_TYPE_POINTER == ptrs->ra[i].mr_type) || (MR_TYPE_STRING == ptrs->ra[i].mr_type))
	      && *(void**)ptrs->ra[i].data.ptr)
	    {
	      MR_FREE (*(void**)ptrs->ra[i].data.ptr);
	      *(void**)ptrs->ra[i].data.ptr = NULL;
	    }

      for (i = 0; i < count; ++i)
	if (MR_VT_STRING == ptrs->ra[i].load_params.mr_value.value_type)
	  if (ptrs->ra[i].load_params.mr_value.vt_string)
	    {
	      MR_FREE (ptrs->ra[i].load_params.mr_value.vt_string);
	      ptrs->ra[i].load_params.mr_value.vt_string = NULL;
	    }
    }

  return (status);
}
