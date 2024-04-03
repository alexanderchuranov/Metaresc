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
  mr_value_t mr_value;

  if (MR_SUCCESS != mr_ptrdes_to_mr_value (&mr_value, ptrdes))
    return (MR_FAILURE);
  if (MR_SUCCESS != mr_value_cast (MR_VT_INT, &mr_value))
    return (MR_FAILURE);

  switch (ptrdes->mr_type)
    {
#define CASE_SET_VALUE_BY_TYPE(TYPE) CASE_SET_VALUE_BY_TYPE_ (TYPE, MR_TYPE_DETECT (TYPE))
#define CASE_SET_VALUE_BY_TYPE_(TYPE, MR_TYPE) case MR_TYPE: *(TYPE*)ptrdes->data.ptr = mr_value.vt_int; break;
      MR_FOREACH (CASE_SET_VALUE_BY_TYPE, bool, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t);
      CASE_SET_VALUE_BY_TYPE_ (mr_intmax_t, MR_TYPE_INT128);
      CASE_SET_VALUE_BY_TYPE_ (mr_uintmax_t, MR_TYPE_UINT128);
      
    case MR_TYPE_ENUM:
      switch (ptrdes->fdp->stype.size)
	{
#define CASE_SET_VALUE_BY_SIZE(TYPE) case sizeof (TYPE): *(TYPE*)ptrdes->data.ptr = mr_value.vt_int; break;
	  MR_FOREACH (CASE_SET_VALUE_BY_SIZE, uint8_t, uint16_t, uint32_t, uint64_t);
#ifdef HAVE_INT128
	  MR_FOREACH (CASE_SET_VALUE_BY_SIZE, mr_uint128_t);
#endif /* HAVE_INT128 */

	default:
	  if (ptrdes->fdp)
	    memcpy (ptrdes->data.ptr, &mr_value.vt_int, MR_MIN (ptrdes->fdp->stype.size, sizeof (mr_value.vt_int)));
	  break;
	}
      break;
    default:
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_TARGET_TYPE, ptrdes->mr_type);
      return (MR_FAILURE);
    }
  return (MR_SUCCESS);
}

static mr_status_t
mr_get_func (void ** dst, char * func_name)
{
  void * func_addr = NULL;
#ifdef HAVE_LIBDL
  func_addr = dlsym (RTLD_DEFAULT, func_name);
#endif /* HAVE_LIBDL */
  if (func_addr != NULL)
    *dst = func_addr;
  else
    {
      char * end = NULL;
      mr_uintmax_t ptr = mr_strtouintmax (func_name, &end, 0);

      while (isspace (*end))
	++end;
      if (*end != 0)
	{
	  MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_FUNC, func_name);
	  return (MR_FAILURE);
	}

      *dst = (void*)(uintptr_t)ptr;
    }

  return (MR_SUCCESS);
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
    switch (ptrdes->value_type)
      {
      case MR_VT_INT:
	*(void**)ptrdes->data.ptr = (void*)(uintptr_t)ptrdes->load_params.vt_int;
	break;
      case MR_VT_ID:
      case MR_VT_SUBSTR:
	{
	  char buf[1 << 6];
	  int length = MR_MIN (ptrdes->load_params.vt_substr.length, sizeof (buf) - 1);

	  buf[0] = 0;
	  buf[length] = 0;
	  if (ptrdes->load_params.vt_substr.str)
	    memcpy (buf, ptrdes->load_params.vt_substr.str, length);

	  if (ptrdes->load_params.vt_substr.length <= sizeof (buf) - 1)
	    status = mr_get_func (ptrdes->data.ptr, buf);
	  else
	    {
	      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_FUNC, buf);
	      status = MR_FAILURE;
	    }
	}
	break;
      case MR_VT_STRING:
	if (NULL == ptrdes->load_params.vt_string)
	  {
	    MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
	    status = MR_FAILURE;
	  }
	else
	  status = mr_get_func (ptrdes->data.ptr, ptrdes->load_params.vt_string);
	break;
      default:
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_TARGET_TYPE, ptrdes->value_type);
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
  mr_value_t mr_value;

  if (MR_SUCCESS != mr_ptrdes_to_mr_value (&mr_value, ptrdes))
    return (MR_FAILURE);
  if (MR_SUCCESS != mr_value_cast (MR_VT_INT, &mr_value))
    return (MR_FAILURE);

  value = mr_value.vt_int;
  return (mr_load_bitfield_value (ptrdes, &value));
}

static mr_status_t
mr_load_float (int idx, mr_ra_ptrdes_t * ptrs)
{
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  mr_value_t mr_value;

  if (MR_SUCCESS != mr_ptrdes_to_mr_value (&mr_value, ptrdes))
    return (MR_FAILURE);
  if (MR_SUCCESS != mr_value_cast (MR_VT_FLOAT, &mr_value))
    return (MR_FAILURE);

  switch (ptrdes->mr_type)
    {
#define CASE_SET_FLOAT_BY_TYPE(TYPE) case MR_TYPE_DETECT (TYPE): *(TYPE*)ptrdes->data.ptr = mr_value.vt_float; break;
      MR_FOREACH (CASE_SET_FLOAT_BY_TYPE, float, double, long double);

    default:
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_TARGET_TYPE, mr_value.value_type);
      return (MR_FAILURE);
    }
  return (MR_SUCCESS);
}

static mr_status_t
mr_load_complex (int idx, mr_ra_ptrdes_t * ptrs)
{
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  mr_value_t mr_value;

  if (MR_SUCCESS != mr_ptrdes_to_mr_value (&mr_value, ptrdes))
    return (MR_FAILURE);
  if (MR_SUCCESS != mr_value_cast (MR_VT_COMPLEX, &mr_value))
    return (MR_FAILURE);

  switch (ptrdes->mr_type)
    {
#define CASE_SET_COMPLEX_BY_TYPE(TYPE) case MR_TYPE_DETECT (TYPE): *(TYPE*)ptrdes->data.ptr = mr_value.vt_complex; break;
      MR_FOREACH (CASE_SET_COMPLEX_BY_TYPE, complex_float_t, complex_double_t, complex_long_double_t);

    default:
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_TARGET_TYPE, mr_value.value_type);
      return (MR_FAILURE);
    }
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
  
  switch (ptrdes->value_type)
    {
    case MR_VT_SUBSTR:
      if (ptrdes->load_params.vt_substr.length == sizeof (char))
	*(char*)ptrdes->data.ptr = ptrdes->load_params.vt_substr.str[0];
      else
	{
	  char buf[1 << 8];
	  int length = MR_MIN (ptrdes->load_params.vt_substr.length, sizeof (buf) - 1);

	  buf[0] = 0;
	  buf[length] = 0;
	  if (ptrdes->load_params.vt_substr.str)
	    memcpy (buf, ptrdes->load_params.vt_substr.str, length);

	  MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_CHAR, buf);
	}
      break;

    case MR_VT_CHAR:
      *(char*)ptrdes->data.ptr = ptrdes->load_params.vt_char;
      break;
      
    case MR_VT_STRING:
      if (NULL == ptrdes->load_params.vt_string)
	{
	  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
	  status = MR_FAILURE;
	  break;
	}

      if (strlen (ptrdes->load_params.vt_string) == sizeof (char))
	{
	  *(char*)ptrdes->data.ptr = ptrdes->load_params.vt_string[0];
	  break;
	}
      __attribute__ ((fallthrough));

    default:
      {
	mr_value_t mr_value;
	if (MR_SUCCESS != mr_ptrdes_to_mr_value (&mr_value, ptrdes))
	  return (MR_FAILURE);
	if (MR_SUCCESS != mr_value_cast (MR_VT_INT, &mr_value))
	  return (MR_FAILURE);
	*(char*)ptrdes->data.ptr = mr_value.vt_int;
	break;
      }
    }

  return (status);
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
      switch (ptrdes->value_type)
	{
	case MR_VT_SUBSTR:
	  *(char**)ptrdes->data.ptr = MR_CALLOC (1, ptrdes->load_params.vt_substr.length + sizeof (char));
	  if (*(char**)ptrdes->data.ptr)
	    memcpy (*(char**)ptrdes->data.ptr, ptrdes->load_params.vt_substr.str, ptrdes->load_params.vt_substr.length);
	  else
	    {
	      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	      status = MR_FAILURE;
	    }
	  break;
	  
	case MR_VT_STRING:
	  *(char**)ptrdes->data.ptr = ptrdes->load_params.vt_string;
	  ptrdes->load_params.vt_string = NULL;
	  break;

	case MR_VT_INT:
	  if (ptrdes->load_params.vt_int >= 0)
	    ptrdes->ref_idx = ptrdes->load_params.vt_int;
	  else
	    {
	      ptrdes->ref_idx = -ptrdes->load_params.vt_int;
	      ptrdes->flags.is_content_reference = true;
	    }
	  break;
	  
	default:
	  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_TARGET_TYPE, ptrdes->value_type);
	  status = MR_FAILURE;
	  break;
	}
    }
  return (status);
}

static mr_status_t
mr_get_char_array (int idx, mr_ra_ptrdes_t * ptrs, char * str, size_t str_len)
{
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  int max_size = ptrdes->fdp ? ptrdes->fdp->stype.size : 0;
  mr_status_t status = MR_SUCCESS;

  if ((str_len >= max_size) && (ptrdes->parent >= 0))
    if (MR_TYPE_POINTER == ptrs->ra[ptrdes->parent].mr_type)
      {
	void * data = MR_REALLOC (ptrdes->data.ptr, str_len + 1);
	if (NULL == data)
	  {
	    if (ptrdes->data.ptr != NULL)
	      MR_FREE (ptrdes->data.ptr);
	    MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	    status = MR_FAILURE;
	  }
		  
	*(void**)ptrs->ra[ptrdes->parent].data.ptr = ptrdes->data.ptr = data;
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
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  mr_status_t status = MR_SUCCESS;

  if (ptrdes->fdp)
    if (ptrdes->fdp->stype.size > 0)
      *(char*)ptrdes->data.ptr = 0;
  
  switch (ptrdes->value_type)
    {
    case MR_VT_SUBSTR:
      status = mr_get_char_array (idx, ptrs, ptrdes->load_params.vt_substr.str, ptrdes->load_params.vt_substr.length);
      break;
    case MR_VT_STRING:
      if (NULL == ptrdes->load_params.vt_string)
	{
	  MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
	  status = MR_FAILURE;
	}
      else
	status = mr_get_char_array (idx, ptrs, ptrdes->load_params.vt_string, strlen (ptrdes->load_params.vt_string));
      break;
    default:
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_TARGET_TYPE, ptrdes->value_type);
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
  mr_td_t * tdp = ptrs->ra[idx].fdp ? ptrs->ra[idx].fdp->stype.tdp : NULL;
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
      char * name = ptrs->ra[idx].fdp ? ptrs->ra[idx].fdp->name.str : NULL;
      mr_fd_t * next_fdp = name ? mr_get_fd_by_name (tdp, name) : NULL;
      fdp = next_fdp ? next_fdp : mr_load_struct_next_field (tdp, fdp);

      if (NULL == fdp)
	{
	  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNKNOWN_SUBNODE, tdp->type.str, name);
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
  fd_.stype.size = mr_type_size (fd_.stype.mr_type_aux);
  if (fd_.stype.size == 0)
    fd_.stype.size = fd_.stype.tdp ? fd_.stype.tdp->size : 0;
  if (fd_.stype.size == 0)
    return (MR_FAILURE);

  if (fd_.stype.dim.size == sizeof (fd_.stype.dim.dim[0]))
    {
      fd_.stype.mr_type = fd_.stype.mr_type_aux; /* prepare copy of filed descriptor for array elements loading */
      fd_.stype.mr_type_aux = fd_.stype.tdp ? fd_.stype.tdp->mr_type : MR_TYPE_VOID;
    }
  else
    {
      fd_.stype.dim.size -= sizeof (fd_.stype.dim.dim[0]);
      int dim_count = fd_.stype.dim.size / sizeof (fd_.stype.dim.dim[0]);
      for (i = 0; i < dim_count; ++i)
	{
	  fd_.stype.dim.dim[i] = fd_.stype.dim.dim[i + 1];
	  fd_.stype.size *= fd_.stype.dim.dim[i];
	}
    }

  /* loop on subnodes */
  i = 0;
  for (idx = ptrs->ra[idx].first_child; (MR_SUCCESS == status) && (idx >= 0); idx = ptrs->ra[idx].next)
    {
      /* check if array index is in range */
      if (i >= fd_.stype.dim.dim[0])
	{
	  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_RANGE_CHECK, fd_.name.str);
	  return (MR_FAILURE);
	}
      /* load recursively */
      status = mr_load (&data[i++ * fd_.stype.size], &fd_, idx, ptrs);
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
  
  if (MR_VT_INT == ptrdes->value_type)
    {
      if (ptrdes->load_params.vt_int >= 0)
	ptrdes->ref_idx = ptrdes->load_params.vt_int;
      else
	{
	  ptrdes->ref_idx = -ptrdes->load_params.vt_int;
	  ptrdes->flags.is_content_reference = true;
	}
    }

  if (ptrdes->ref_idx >= 0)
    return (MR_SUCCESS);

  for (node = ptrs->ra[idx].first_child; node >= 0; node = ptrs->ra[node].next)
    ++count;
  if (0 == count)
    return (MR_SUCCESS);

  mr_td_t * tdp = ptrs->ra[idx].fdp ? ptrs->ra[idx].fdp->stype.tdp : NULL;
  mr_fd_t fd_;
  memset (&fd_, 0, sizeof (fd_));
  fd_.non_persistent = true;
  fd_.unnamed = true;
  fd_.stype.mr_type = ptrs->ra[idx].mr_type_aux;
  fd_.stype.mr_type_aux = tdp ? tdp->mr_type : MR_TYPE_VOID;
  fd_.stype.tdp = tdp;
  fd_.stype.size = mr_type_size (fd_.stype.mr_type);
  if (fd_.stype.size == 0)
    fd_.stype.size = fd_.stype.tdp ? fd_.stype.tdp->size : 0;
  if (fd_.stype.size == 0)
    return (MR_FAILURE);

  /* allocate memory */
  *data = MR_CALLOC (count, fd_.stype.size);
  if (NULL == *data)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (MR_FAILURE);
    }
  
  /* load recursively */
  count = 0;
  for (node = ptrs->ra[idx].first_child; (MR_SUCCESS == status) && (node >= 0); node = ptrs->ra[node].next)
    status = mr_load (*data + count++ * fd_.stype.size, &fd_, node, ptrs);

  ptrs->ra[idx].MR_SIZE = count * fd_.stype.size;
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
      (MR_VT_SUBSTR == ptrdes->value_type)
      && (0 == ptrdes->load_params.vt_substr.length) && /* content must be an empty string */
      (ptrdes->fdp != NULL) && /* node must have a name */
      (ptrdes->next >= 0))
    if (NULL == ptrs->ra[ptrdes->next].fdp) /* there should be a next node without name */
      {
	ptrs->ra[ptrdes->next].fdp = ptrdes->fdp;
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

  if (!fdp->non_persistent && (MR_TYPE_POINTER == fdp->stype.mr_type) && ptrs->ra[idx].fdp)
    if (ptrs->ra[idx].flags.typed && (fdp->stype.tdp != ptrs->ra[idx].fdp->stype.tdp))
      {
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_NODE_TYPE_MISSMATCH, fdp->name.str,
		    fdp->stype.tdp ? fdp->stype.tdp->type.str : "undefined",
		    ptrs->ra[idx].fdp->stype.tdp ? ptrs->ra[idx].fdp->stype.tdp->type.str : "undefined");
	return (MR_FAILURE);
      }

  if (!fdp->unnamed && ptrs->ra[idx].fdp && fdp->name.str)
    if (ptrs->ra[idx].fdp->name.str)
      if (0 != strcmp (fdp->name.str, ptrs->ra[idx].fdp->name.str))
	{
	  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_NODE_NAME_MISSMATCH, fdp->name.str, ptrs->ra[idx].fdp->name.str);
	  return (MR_FAILURE);
	}

  ptrs->ra[idx].data.ptr = data;
  ptrs->ra[idx].fdp = fdp;
  ptrs->ra[idx].mr_size = fdp->stype.size;
  ptrs->ra[idx].mr_type = fdp->stype.mr_type;
  ptrs->ra[idx].mr_type_aux = fdp->stype.mr_type_aux;

  /* route loading */
  mr_load_handler_t load_handler = NULL;
  if ((fdp->stype.mr_type >= 0) && (fdp->stype.mr_type < MR_TYPE_LAST))
    load_handler = mr_load_handler[fdp->stype.mr_type];

  if (load_handler != NULL)
    status = load_handler (idx, ptrs);
  else
    MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNSUPPORTED_NODE_TYPE, fdp->stype.mr_type);

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
	switch (ptrs->ra[i].value_type)
	  {
	  case MR_VT_STRING:
	    if (ptrs->ra[i].load_params.vt_string)
	      {
		MR_FREE (ptrs->ra[i].load_params.vt_string);
		ptrs->ra[i].load_params.vt_string = NULL;
	      }
	    break;
	  case MR_VT_COMPLEX:
	    if (ptrs->ra[i].load_params.vt_complex)
	      {
		MR_FREE (ptrs->ra[i].load_params.vt_complex);
		ptrs->ra[i].load_params.vt_complex = NULL;
	      }
	    break;
	  default:
	    break;
	  }
    }

  if (fdp->non_persistent)
    ptrs->ra[idx].fdp = fdp->stype.tdp ? &fdp->stype.tdp->mr_ptr_fd : NULL;

  return (status);
}
