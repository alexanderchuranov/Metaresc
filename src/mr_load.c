/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

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
#include <mr_load.h>
#include <mr_value.h>
#include <lexer.h>

TYPEDEF_FUNC (mr_status_t, mr_load_handler_t, (int /* idx */, mr_load_data_t * /* mr_load_data */))

/**
 * Post load references setting. If node was marked as references
 * it should be substitude with actual pointer. This substition
 * can't be made during structure loading because of forward references.
 * @param mr_load_data structures that holds context of loading
 * @return Status.
 */
static mr_status_t
mr_set_crossrefs (mr_load_data_t * mr_load_data)
{
  int i;
  int count = mr_load_data->ptrs.size / sizeof (mr_load_data->ptrs.ra[0]);
  int max_idx = -1;
  int * table;
  mr_status_t status = MR_SUCCESS;

  for (i = 0; i < count; ++i)
    if (mr_load_data->ptrs.ra[i].idx > max_idx)
      max_idx = mr_load_data->ptrs.ra[i].idx;

  if (max_idx < 0)
    return (MR_SUCCESS);

  table = MR_MALLOC (sizeof (table[0]) * (max_idx + 1));
  if (NULL == table)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (MR_FAILURE);
    }

  for (i = 0; i <= max_idx; ++i)
    table[i] = -1;
  for (i = 0; i < count; ++i)
    if (mr_load_data->ptrs.ra[i].idx >= 0)
      {
	/* checking indexes collision */
	if (table[mr_load_data->ptrs.ra[i].idx] >= 0)
	  {
	    MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_IDS_COLLISION, MR_REF_IDX, mr_load_data->ptrs.ra[i].idx);
	    status = MR_FAILURE;
	  }
	table[mr_load_data->ptrs.ra[i].idx] = i;
      }

  /* set all cross refernces */
  for (i = 0; i < count; ++i)
    if (mr_load_data->ptrs.ra[i].ref_idx >= 0)
      {
	int idx = table[mr_load_data->ptrs.ra[i].ref_idx];
	if (idx < 0)
	  {
	    MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNDEFINED_REF_IDX, MR_REF_IDX, mr_load_data->ptrs.ra[i].ref_idx);
	    status = MR_FAILURE;
	  }
	else
	  {
	    void * data = mr_load_data->ptrs.ra[idx].data.ptr;

	    if ((MR_TYPE_POINTER != mr_load_data->ptrs.ra[i].fd.mr_type) &&
		(MR_TYPE_STRING != mr_load_data->ptrs.ra[i].fd.mr_type))
	      {
		MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_TARGET_TYPE, mr_load_data->ptrs.ra[i].fd.mr_type);
		status = MR_FAILURE;
		continue;
	      }

	    if (mr_load_data->ptrs.ra[i].flags.is_content_reference)
	      {
		if (NULL == data)
		  {
		    MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
		    status = MR_FAILURE;
		  }
		else
		  data = *(void**)data;
	      }

	    if (NULL == mr_load_data->ptrs.ra[i].data.ptr)
	      {
		MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
		status = MR_FAILURE;
	      }
	    else
	      *(void**)mr_load_data->ptrs.ra[i].data.ptr = data;
	  }
      }
  MR_FREE (table);
  return (status);
}

/**
 * MR_NONE load handler (dummy)
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read
 */
static mr_status_t
mr_load_none (int idx, mr_load_data_t * mr_load_data)
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
mr_load_integer (int idx, mr_load_data_t * mr_load_data)
{
  mr_ptrdes_t * ptrdes = &mr_load_data->ptrs.ra[idx];

  if (MR_SUCCESS != mr_value_cast (MR_VT_INT, &ptrdes->mr_value))
    return (MR_FAILURE);

  switch (ptrdes->fd.mr_type)
    {
    case MR_TYPE_BOOL:
      *(bool*)ptrdes->data.ptr = ptrdes->mr_value.vt_int;
      break;
    case MR_TYPE_INT8:
      *(int8_t*)ptrdes->data.ptr = ptrdes->mr_value.vt_int;
      break;
    case MR_TYPE_UINT8:
      *(uint8_t*)ptrdes->data.ptr = ptrdes->mr_value.vt_int;
      break;
    case MR_TYPE_INT16:
      *(int16_t*)ptrdes->data.ptr = ptrdes->mr_value.vt_int;
      break;
    case MR_TYPE_UINT16:
      *(uint16_t*)ptrdes->data.ptr = ptrdes->mr_value.vt_int;
      break;
    case MR_TYPE_INT32:
      *(int32_t*)ptrdes->data.ptr = ptrdes->mr_value.vt_int;
      break;
    case MR_TYPE_UINT32:
      *(uint32_t*)ptrdes->data.ptr = ptrdes->mr_value.vt_int;
      break;
    case MR_TYPE_INT64:
      *(int64_t*)ptrdes->data.ptr = ptrdes->mr_value.vt_int;
      break;
    case MR_TYPE_UINT64:
      *(uint64_t*)ptrdes->data.ptr = ptrdes->mr_value.vt_int;
      break;

    case MR_TYPE_ENUM:
    case MR_TYPE_BITMASK:
      switch (ptrdes->fd.size)
	{
	case sizeof (uint8_t):
	  *(uint8_t*)ptrdes->data.ptr = ptrdes->mr_value.vt_int;
	  break;
	case sizeof (uint16_t):
	  *(uint16_t*)ptrdes->data.ptr = ptrdes->mr_value.vt_int;
	  break;
	case sizeof (uint32_t):
	  *(uint32_t*)ptrdes->data.ptr = ptrdes->mr_value.vt_int;
	  break;
	case sizeof (uint64_t):
	  *(uint64_t*)ptrdes->data.ptr = ptrdes->mr_value.vt_int;
	  break;
	default:
	  memcpy (ptrdes->data.ptr, &ptrdes->mr_value.vt_int,
		  MR_MIN (ptrdes->fd.size, sizeof (ptrdes->mr_value.vt_int)));
	  break;
	}
      break;
    default:
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_TARGET_TYPE, ptrdes->fd.mr_type);
      return (MR_FAILURE);
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
mr_load_func (int idx, mr_load_data_t * mr_load_data)
{
  mr_ptrdes_t * ptrdes = &mr_load_data->ptrs.ra[idx];
  *(void**)ptrdes->data.ptr = NULL;
  switch (ptrdes->mr_value.value_type)
    {
    case MR_VT_INT:
      *(void**)ptrdes->data.ptr = (void*)(long)ptrdes->mr_value.vt_int;
      break;
    case MR_VT_STRING:
      *(void**)ptrdes->data.ptr = ptrdes->mr_value.vt_string;
      ptrdes->mr_value.vt_string = NULL;
      break;
    case MR_VT_ID:
    case MR_VT_UNKNOWN:
      if (NULL == ptrdes->mr_value.vt_string)
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
      else
	{
	  if (isdigit (ptrdes->mr_value.vt_string[0]))
	    {
	      if (MR_SUCCESS != mr_value_cast (MR_VT_INT, &ptrdes->mr_value))
		return (MR_FAILURE);
	      *(void**)ptrdes->data.ptr = (void*)(long)ptrdes->mr_value.vt_int;
	    }
#ifdef HAVE_LIBDL
	  else
	    *(void**)ptrdes->data.ptr = dlsym (RTLD_DEFAULT, ptrdes->mr_value.vt_string);
#endif /* HAVE_LIBDL */
	}
      break;
    default:
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_TARGET_TYPE, ptrdes->mr_value.value_type);
      return (MR_FAILURE);
    }
  return (MR_SUCCESS);
}

/**
 * MR_BITFIELD load handler. Load int from string and save it to
 * bit shifted field.
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read
 */
static mr_status_t
mr_load_bitfield (int idx, mr_load_data_t * mr_load_data)
{
  mr_ptrdes_t * ptrdes = &mr_load_data->ptrs.ra[idx];
  uint64_t value;

  if (MR_SUCCESS != mr_value_cast (MR_VT_INT, &ptrdes->mr_value))
    return (MR_FAILURE);

  value = ptrdes->mr_value.vt_int;
  return (mr_load_bitfield_value (ptrdes, &value));
}

static mr_status_t
mr_load_float (int idx, mr_load_data_t * mr_load_data)
{
  mr_ptrdes_t * ptrdes = &mr_load_data->ptrs.ra[idx];
  if (MR_SUCCESS != mr_value_cast (MR_VT_FLOAT, &ptrdes->mr_value))
    return (MR_FAILURE);

  switch (ptrdes->fd.mr_type)
    {
    case MR_TYPE_FLOAT:
      *(float*)ptrdes->data.ptr = ptrdes->mr_value.vt_float;
      break;
    case MR_TYPE_DOUBLE:
      *(double*)ptrdes->data.ptr = ptrdes->mr_value.vt_float;
      break;
    case MR_TYPE_LONG_DOUBLE:
      *(long double*)ptrdes->data.ptr = ptrdes->mr_value.vt_float;
      break;
    default:
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
      return (MR_FAILURE);
    }
  return (MR_SUCCESS);
}

static mr_status_t
mr_load_complex (int idx, mr_load_data_t * mr_load_data)
{
  mr_ptrdes_t * ptrdes = &mr_load_data->ptrs.ra[idx];
  if (MR_SUCCESS != mr_value_cast (MR_VT_COMPLEX, &ptrdes->mr_value))
    return (MR_FAILURE);

  switch (ptrdes->fd.mr_type)
    {
    case MR_TYPE_COMPLEX_FLOAT:
      *(complex float*)ptrdes->data.ptr = ptrdes->mr_value.vt_complex;
      break;
    case MR_TYPE_COMPLEX_DOUBLE:
      *(complex double*)ptrdes->data.ptr = ptrdes->mr_value.vt_complex;
      break;
    case MR_TYPE_COMPLEX_LONG_DOUBLE:
      *(complex long double*)ptrdes->data.ptr = ptrdes->mr_value.vt_complex;
      break;
    default:
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
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
mr_load_char (int idx, mr_load_data_t * mr_load_data)
{
  mr_status_t status = MR_SUCCESS;
  mr_ptrdes_t * ptrdes = &mr_load_data->ptrs.ra[idx];
  
  switch (ptrdes->mr_value.value_type)
    {
    case MR_VT_UNKNOWN:
      if (NULL == ptrdes->mr_value.vt_string)
	status = MR_FAILURE;
      else if (strlen (ptrdes->mr_value.vt_string) <= 1)
	*(char*)ptrdes->data.ptr = ptrdes->mr_value.vt_string[0];
      else
	{
	  int32_t code = 0;
	  int size = 0;
	  if (1 != sscanf (ptrdes->mr_value.vt_string, CINIT_CHAR_QUOTE "%n", &code, &size))
	    status = MR_FAILURE;
	  else
	    {
	      while (isspace (ptrdes->mr_value.vt_string[size]))
		++size;
	      if (0 == ptrdes->mr_value.vt_string[size])
		*(char*)ptrdes->data.ptr = code;
	      else
		status = MR_FAILURE;
	    }
	}
      
      if (MR_FAILURE == status)
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_CHAR, ptrdes->mr_value.vt_string);
      
      break;
      
    case MR_VT_CHAR:
      *(char*)ptrdes->data.ptr = ptrdes->mr_value.vt_char;
      break;
      
    default:
      if (MR_SUCCESS == mr_value_cast (MR_VT_INT, &ptrdes->mr_value))
	*(char*)ptrdes->data.ptr = ptrdes->mr_value.vt_int;
      else
	status = MR_FAILURE;
      break;
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
mr_load_string (int idx, mr_load_data_t * mr_load_data)
{
  mr_ptrdes_t * ptrdes = &mr_load_data->ptrs.ra[idx];
  if ((true == ptrdes->flags.is_null) || (ptrdes->ref_idx >= 0))
    *(char**)ptrdes->data.ptr = NULL;
  else
    {
      switch (ptrdes->mr_value.value_type)
	{
	case MR_VT_STRING:
	case MR_VT_UNKNOWN:
	  *(char**)ptrdes->data.ptr = ptrdes->mr_value.vt_string;
	  ptrdes->mr_value.vt_string = NULL;
	  break;
	case MR_VT_INT:
	  *(char**)ptrdes->data.ptr = (void*)(long)ptrdes->mr_value.vt_int;
	  break;
	default:
	  return (MR_FAILURE);
	}
    }
  return (MR_SUCCESS);
}

/**
 * MR_CHAR_ARRAY load handler.
 * Save string in place (truncate string if needed).
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read
 */
static mr_status_t
mr_load_char_array (int idx, mr_load_data_t * mr_load_data)
{
  mr_ptrdes_t * ptrdes = &mr_load_data->ptrs.ra[idx];
  int max_size = ptrdes->fd.size;
  mr_status_t status = MR_FAILURE;
  
  *(char*)ptrdes->data.ptr = 0;
  if ((MR_VT_STRING == ptrdes->mr_value.value_type) ||
      (MR_VT_UNKNOWN == ptrdes->mr_value.value_type))
    {
      char * str = ptrdes->mr_value.vt_string;
      if (NULL != str)
	{
	  int str_len = strlen (str) + 1;
	  status = MR_SUCCESS;
	  if (str_len > max_size)
	    {
	      if ((ptrdes->parent >= 0) &&
		  (MR_TYPE_POINTER == mr_load_data->ptrs.ra[ptrdes->parent].fd.mr_type))
		{
		  void * data = MR_REALLOC (ptrdes->data.ptr, str_len);
		  if (NULL == data)
		    {
		      if (ptrdes->data.ptr != NULL)
			MR_FREE (ptrdes->data.ptr);
		      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
		      status = MR_FAILURE;
		    }
		  
		  *(void**)mr_load_data->ptrs.ra[ptrdes->parent].data.ptr = ptrdes->data.ptr = data;
		}
	      else
		{
		  str[max_size - 1] = 0;
		  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_STRING_TRUNCATED);
		}
	    }
		  
	  if (ptrdes->data.ptr != NULL)
	    strcpy (ptrdes->data.ptr, str);
	}
    }
  else
    MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_WRONG_RESULT_TYPE);
  return (status);
}

static mr_fd_t *
mr_load_struct_next_field (mr_td_t * tdp, mr_fd_t * fdp)
{
  int i, count = tdp->fields_size / sizeof (tdp->fields[0]);

  for (i = 0; i < count; ++i)
    if (NULL == fdp)
      return (tdp->fields[i].fdp);
    else if (tdp->fields[i].fdp == fdp)
      fdp = NULL;

  return (NULL);
}

/**
 * MR_STRUCT load handler.
 * Save content of subnodes to structure fileds.
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @param tdp type descriptor
 * @return Status of read
 */
static mr_status_t
mr_load_struct_inner (int idx, mr_load_data_t * mr_load_data, mr_td_t * tdp)
{
  char * data = mr_load_data->ptrs.ra[idx].data.ptr;
  int first_child = mr_load_data->ptrs.ra[idx].first_child;
  mr_fd_t * fdp = NULL;

  /* get pointer on structure descriptor */
  if (NULL == tdp)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_NO_TYPE_DESCRIPTOR, mr_load_data->ptrs.ra[idx].fd.type);
      return (MR_FAILURE);
    }

  /* for C init style we can get union descriptor only from type cast */
  if ((0 == strcmp (tdp->type.str, "mr_ptr_t")) && (first_child >= 0) &&
      mr_load_data->ptrs.ra[first_child].fd.type && (NULL == mr_load_data->ptrs.ra[first_child].name_ss.str))
    {
      mr_load_data->ptrs.ra[first_child].name_ss.str = mr_load_data->ptrs.ra[first_child].fd.type;
      mr_load_data->ptrs.ra[first_child].name_ss.length = strlen (mr_load_data->ptrs.ra[first_child].fd.type);
    }

  /* loop on all subnodes */
  for (idx = first_child; idx >= 0; idx = mr_load_data->ptrs.ra[idx].next)
    {
      mr_substr_t * substr = &mr_load_data->ptrs.ra[idx].name_ss;
      char name[substr->length + 1];
      memcpy (name, substr->str, substr->length);
      name[substr->length] = 0;
      
      if (NULL == substr->str)
	fdp = mr_load_struct_next_field (tdp, fdp);
      else
	fdp = mr_get_fd_by_name (tdp, name);

      if (NULL == fdp)
	{
	  MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNKNOWN_SUBNODE, tdp->type.str, name);
	  return (MR_FAILURE);
	}

      /* recursively load subnode */
      if (MR_SUCCESS != mr_load (&data[fdp->offset], fdp, idx, mr_load_data))
	return (MR_FAILURE);
    }
  return (MR_SUCCESS);
}

/**
 * MR_STRUCT load handler. Wrapper over mr_load_struct_inner.
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read
 */
static mr_status_t
mr_load_struct (int idx, mr_load_data_t * mr_load_data)
{
  return (mr_load_struct_inner (idx, mr_load_data, mr_get_td_by_name (mr_load_data->ptrs.ra[idx].fd.type)));
}

/**
 * MR_ARRAY load handler.
 * Save content of subnodes to array elements.
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read
 */
static mr_status_t
mr_load_array (int idx, mr_load_data_t * mr_load_data)
{
  char * data = mr_load_data->ptrs.ra[idx].data.ptr;
  mr_fd_t fd_ = mr_load_data->ptrs.ra[idx].fd;
  int row_count = fd_.param.array_param.row_count;
  int count = fd_.param.array_param.count;
  int i = 0;

  if (1 == fd_.param.array_param.row_count)
    fd_.mr_type = fd_.mr_type_aux; /* prepare copy of filed descriptor for array elements loading */
  else
    {
      fd_.param.array_param.count = row_count;
      fd_.param.array_param.row_count = 1;
    }

  /* loop on subnodes */
  for (idx = mr_load_data->ptrs.ra[idx].first_child; idx >= 0; idx = mr_load_data->ptrs.ra[idx].next)
    {
      /* check if array index is in range */
      if ((i < 0) || (i >= count))
	{
	  mr_substr_t * substr = &mr_load_data->ptrs.ra[idx].name_ss;
	  char name[substr->length + 1];
	  memcpy (name, substr->str, substr->length);
	  name[substr->length] = 0;
	  MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_RANGE_CHECK, name);
	  return (MR_FAILURE);
	}
      /* load recursively */
      if (MR_SUCCESS != mr_load (&data[i * fd_.size], &fd_, idx, mr_load_data))
	return (MR_FAILURE);
      i += row_count;
    }
  return (MR_SUCCESS);
}

/**
 * MR_TYPE_EXT_POINTER load handler. Initiated as postponed call thru mr_load_pointer via stack.
 * Loads element into newly allocate memory.
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read (0 - failure, !0 - success)
 */
static mr_status_t
mr_load_pointer_postponed (int idx, mr_load_data_t * mr_load_data)
{
  char ** data = mr_load_data->ptrs.ra[idx].data.ptr;
  mr_fd_t fd_ = mr_load_data->ptrs.ra[idx].fd;
  int count = 0;
  int node;

  for (node = mr_load_data->ptrs.ra[idx].first_child; node >= 0; node = mr_load_data->ptrs.ra[node].next)
    ++count;
  if (0 == count)
    return (MR_SUCCESS);
  
  fd_.mr_type = fd_.mr_type_aux;
  /* allocate memory */
  *data = MR_MALLOC (count * fd_.size);
  if (NULL == *data)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (MR_FAILURE);
    }
  memset (*data, 0, count * fd_.size);
  /* load recursively */
  count = 0;
  for (node = mr_load_data->ptrs.ra[idx].first_child; node >= 0; node = mr_load_data->ptrs.ra[node].next)
    if (MR_SUCCESS != mr_load (*data + count++ * fd_.size, &fd_, node, mr_load_data))
      return (MR_FAILURE);

  mr_load_data->ptrs.ra[idx].MR_SIZE = count * fd_.size;
  mr_pointer_set_size (idx, &mr_load_data->ptrs);
  
  return (MR_SUCCESS);
}

/**
 * MR_TYPE_EXT_POINTER load handler. Schedule element postponed loading via stack.
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read
 */
static mr_status_t
mr_load_pointer (int idx, mr_load_data_t * mr_load_data)
{
  void ** data = mr_load_data->ptrs.ra[idx].data.ptr;
  /* default initializer */
  *data = NULL;
  if (mr_load_data->ptrs.ra[idx].ref_idx >= 0)
    return (MR_SUCCESS);
  if (mr_load_data->ptrs.ra[idx].first_child < 0)
    return (MR_SUCCESS);
  /* check whether pointer should have offsprings or not */
  if ((MR_TYPE_NONE != mr_load_data->ptrs.ra[idx].fd.mr_type) && (MR_TYPE_VOID != mr_load_data->ptrs.ra[idx].fd.mr_type))
    {
      int * idx_ = mr_rarray_allocate_element ((void**)&mr_load_data->mr_ra_idx,
					       &mr_load_data->mr_ra_idx_size, &mr_load_data->mr_ra_idx_alloc_size,
					       sizeof (mr_load_data->mr_ra_idx[0]));
      if (NULL == idx_)
	return (MR_FAILURE);
      *idx_ = idx;
      return (MR_SUCCESS);
    }

  return (MR_SUCCESS);
}

/**
 * MR_ANON_UNION load handler.
 * Load anonymous union
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read
 */
static mr_status_t
mr_load_anon_union (int idx, mr_load_data_t * mr_load_data)
{
  mr_ptrdes_t * ptrdes = &mr_load_data->ptrs.ra[idx];
  /*
    Anonimous unions in C init style saved as named field folowed by union itself. Named field has type of zero length static string and must be inited by empty string. Here is an example.
    .anon_union_79 = "", {
    .union_float = 3.1415927410,
    },
  */
  if ((ptrdes->first_child < 0) && /* if node has no childs, then it is C init style anonumous union */
      (MR_VT_STRING == ptrdes->mr_value.value_type) && (NULL != ptrdes->mr_value.vt_string)
      && (0 == ptrdes->mr_value.vt_string[0]) && /* content must be an empty string */
      (mr_load_data->ptrs.ra[idx].name_ss.str != NULL) && /* node must have a name */
      (ptrdes->next >= 0) && (NULL == mr_load_data->ptrs.ra[ptrdes->next].name_ss.str)) /* there should be a next node without name */
    {
      mr_load_data->ptrs.ra[ptrdes->next].name_ss = mr_load_data->ptrs.ra[idx].name_ss;
      return (MR_SUCCESS); /* now next node has a name and will be loaded by top level procedure */
    }
  return (mr_load_struct (idx, mr_load_data));
}

/**
 * Cleanup helper. Deallocates all dynamically allocated resources.
 * @param ptrs resizeable array with pointers descriptors
 * @return Status of read
 */
mr_status_t
mr_free_ptrs (mr_ra_ptrdes_t ptrs)
{
  if (ptrs.ra)
    {
      int count = ptrs.size / sizeof (ptrs.ra[0]);
      int i;
      for (i = 0; i < count; ++i)
	{
	  if ((MR_VT_UNKNOWN == ptrs.ra[i].mr_value.value_type)
	      || (MR_VT_STRING == ptrs.ra[i].mr_value.value_type)
	      || (MR_VT_ID == ptrs.ra[i].mr_value.value_type))
	    {
	      if (ptrs.ra[i].mr_value.vt_string)
		MR_FREE (ptrs.ra[i].mr_value.vt_string);
	      ptrs.ra[i].mr_value.vt_string = NULL;
	    }
	}
      MR_FREE (ptrs.ra);
      ptrs.ra = NULL;
      ptrs.size = ptrs.alloc_size = 0;
    }
  return (MR_SUCCESS);
}

/**
 * Init IO handlers Table
 */
static mr_load_handler_t mr_load_handler[] =
  {
    [MR_TYPE_NONE] = mr_load_none,
    [MR_TYPE_VOID] = mr_load_none,
    [MR_TYPE_ENUM] = mr_load_integer,
    [MR_TYPE_BITFIELD] = mr_load_bitfield,
    [MR_TYPE_BITMASK] = mr_load_integer,
    [MR_TYPE_BOOL] = mr_load_integer,
    [MR_TYPE_INT8] = mr_load_integer,
    [MR_TYPE_UINT8] = mr_load_integer,
    [MR_TYPE_INT16] = mr_load_integer,
    [MR_TYPE_UINT16] = mr_load_integer,
    [MR_TYPE_INT32] = mr_load_integer,
    [MR_TYPE_UINT32] = mr_load_integer,
    [MR_TYPE_INT64] = mr_load_integer,
    [MR_TYPE_UINT64] = mr_load_integer,
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
    [MR_TYPE_POINTER] = mr_load_pointer,
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
mr_load (void * data, mr_fd_t * fdp, int idx, mr_load_data_t * mr_load_data)
{
  mr_status_t status = MR_FAILURE;

  if (0 == idx)
    {
      mr_load_data->mr_ra_idx = NULL;
      mr_load_data->mr_ra_idx_size = 0;
      mr_load_data->mr_ra_idx_alloc_size = 0;
    }

  if ((idx < 0) || (idx >= mr_load_data->ptrs.size / sizeof (mr_load_data->ptrs.ra[0])))
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_SAVE_IDX_RANGE_CHECK);
      return (MR_FAILURE);
    }

  mr_load_data->ptrs.ra[idx].data.ptr = data;
  if (mr_load_data->ptrs.ra[idx].name_ss.str && fdp->name.str)
    if (0 != mr_substrcmp (fdp->name.str, &mr_load_data->ptrs.ra[idx].name_ss))
      {
	mr_substr_t * substr = &mr_load_data->ptrs.ra[idx].name_ss;
	char name[substr->length + 1];
	memcpy (name, substr->str, substr->length);
	name[substr->length] = 0;
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_NODE_NAME_MISSMATCH, fdp->name.str, name);
	return (MR_FAILURE);
      }

  if (mr_load_data->ptrs.ra[idx].fd.type && fdp->type)
    if (strcmp (fdp->type, mr_load_data->ptrs.ra[idx].fd.type))
      if (!((0 == strcmp (MR_VOIDP_T_STR, mr_load_data->ptrs.ra[idx].fd.type)) &&
	    (('*' == fdp->type[strlen (fdp->type) - 1]) || (MR_TYPE_FUNC == fdp->mr_type))))
	{
	  MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_NODE_TYPE_MISSMATCH, fdp->name.str, fdp->type, mr_load_data->ptrs.ra[idx].fd.type);
	  return (MR_FAILURE);
	}

  if ((NULL == mr_load_data->ptrs.ra[idx].name_ss.str) && (fdp->name.str))
    {
      mr_load_data->ptrs.ra[idx].name_ss.str = fdp->name.str;
      mr_load_data->ptrs.ra[idx].name_ss.length = strlen (fdp->name.str);
    }
  
  if ((NULL == mr_load_data->ptrs.ra[idx].fd.type) && (fdp->type))
    mr_load_data->ptrs.ra[idx].fd.type = fdp->type;

  mr_load_data->ptrs.ra[idx].fd.size = fdp->size;
  mr_load_data->ptrs.ra[idx].fd.mr_type = fdp->mr_type;
  mr_load_data->ptrs.ra[idx].fd.mr_type_aux = fdp->mr_type_aux;
  mr_load_data->ptrs.ra[idx].fd.param = fdp->param;
  mr_load_data->ptrs.ra[idx].fd.meta = fdp->meta;
  mr_load_data->ptrs.ra[idx].fd.res.ptr = fdp->res.ptr;
  mr_load_data->ptrs.ra[idx].fd.res_type = fdp->res_type;

  /* route loading */
  if ((fdp->mr_type < MR_TYPE_LAST) && mr_load_handler[fdp->mr_type])
    status = mr_load_handler[fdp->mr_type] (idx, mr_load_data);
  else
    MR_MESSAGE_UNSUPPORTED_NODE_TYPE_ (fdp);

  /* set cross references at the upper level */
  if (0 == idx)
    {
      if (MR_SUCCESS == status)
	{
	  while (mr_load_data->mr_ra_idx_size > 0)
	    {
	      mr_load_data->mr_ra_idx_size -= sizeof (mr_load_data->mr_ra_idx[0]);
	      status = mr_load_pointer_postponed (mr_load_data->mr_ra_idx[mr_load_data->mr_ra_idx_size / sizeof (mr_load_data->mr_ra_idx[0])],
						  mr_load_data);
	      if (MR_SUCCESS != status)
		break;
	    }
	}
      
      mr_status_t crossref_status = mr_set_crossrefs (mr_load_data);
      if (MR_SUCCESS == status)
	status = crossref_status;
      
      if (mr_load_data->mr_ra_idx != NULL)
	{
	  MR_FREE (mr_load_data->mr_ra_idx);
	  mr_load_data->mr_ra_idx = NULL;
	  mr_load_data->mr_ra_idx_size = 0;
	  mr_load_data->mr_ra_idx_alloc_size = 0;
	}
    }

  return (status);
}
