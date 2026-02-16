#include <metaresc.h>
#include <mr_hsort.h>

static int
mr_elements_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  return (mr_cmp_structs (*(mr_ptrdes_t**)x.ptr, *(mr_ptrdes_t**)y.ptr));
}

mr_status_t
mr_generic_sort (void * data, size_t count, char * key_type)
{
  mr_td_t * tdp = mr_get_td_by_name (key_type);

  if ((NULL == tdp) || (0 == tdp->size))
    return (MR_FAILURE);

  size_t i;
  mr_status_t status = MR_SUCCESS;
  mr_ptrdes_t ** array = MR_CALLOC (count, sizeof (array[0]));

  if (NULL == array)
    return (MR_FAILURE);

  for (i = 0; i < count; ++i)
    {
      array[i] = MR_SAVE_STR_TYPED (key_type, (uint8_t*)data + i * tdp->size);
      if (array[i] == NULL)
	{
	  status = MR_FAILURE;
	  goto cleanup;
	}
    }

  uint8_t * copy = MR_CALLOC (count, tdp->size);
  if (copy == NULL)
    {
      status = MR_FAILURE;
      goto cleanup;
    }

  mr_hsort (array, count, sizeof (array[0]), mr_elements_cmp, NULL);

  for (i = 0; i < count; ++i)
    memcpy (copy + i * tdp->size, array[i][1].data.ptr, tdp->size);

  memcpy (data, copy, count * tdp->size);
  MR_FREE (copy);

 cleanup:
  for (i = 0; i < count; ++i)
    if (array[i] != NULL)
      MR_FREE (array[i]);
  MR_FREE (array);

  return (status);
}

#define MR_TYPE_CMP_FUNC_NAME(TYPE) mr_ ## TYPE ## _cmp
#define MR_TYPE_CMP_FUNC(TYPE)						\
  static int MR_TYPE_CMP_FUNC_NAME (TYPE)				\
    (const mr_ptr_t x, const mr_ptr_t y, const void * context)		\
  {									\
    TYPE * _x = x.ptr;							\
    TYPE * _y = y.ptr;							\
    return ((*_x > *_y) - (*_x < *_y));					\
  }

typedef void * void_ptr;

#define MR_BASIC_TYPES char, bool, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, mr_int128_t, mr_uint128_t, float, double, long_double_t

MR_FOREACH (MR_TYPE_CMP_FUNC, void_ptr, MR_BASIC_TYPES);

static int MR_TYPE_CMP_FUNC_NAME (string_t) (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  string_t * _x = x.ptr;
  string_t * _y = y.ptr;
  if (*_x && *_y)
    return (strcmp (*_x, *_y));
  return ((*_x > *_y) - (*_x < *_y));
}

static int MR_TYPE_CMP_FUNC_NAME (char_array) (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  char * _x = x.ptr;
  char * _y = y.ptr;
  return (strcmp (_x, _y));
}

static mr_compar_fn_t
mr_type_compar_fn (mr_type_t mr_type)
{
  static mr_compar_fn_t types_compar_fn[MR_TYPE_LAST] =
    {
#define MR_TYPE_CMP_FUNC_ENTRY(TYPE) [MR_TYPE_DETECT (TYPE)] = MR_TYPE_CMP_FUNC_NAME (TYPE),
      [MR_TYPE_CHAR_ARRAY] = MR_TYPE_CMP_FUNC_NAME (char_array),
      [MR_TYPE_FUNC] = MR_TYPE_CMP_FUNC_NAME (void_ptr),
      [MR_TYPE_FUNC_TYPE] = MR_TYPE_CMP_FUNC_NAME (void_ptr),
      MR_FOREACH (MR_TYPE_CMP_FUNC_ENTRY, string_t, MR_BASIC_TYPES)
    };

  if ((mr_type >= 0) && (mr_type < MR_TYPE_LAST))
    return (types_compar_fn[mr_type]);
  return (NULL);
}

mr_status_t
mr_basic_types_sort (void * data, size_t count, char * key_type, mr_type_t mr_type, size_t element_size)
{
  if (MR_TYPE_NONE == mr_type)
    return (mr_generic_sort (data, count, key_type));
  else
    {
      mr_compar_fn_t compar_fn = mr_type_compar_fn (mr_type);
      if (NULL == compar_fn)
	return (MR_FAILURE);
      mr_hsort (data, count, element_size, compar_fn, NULL);
    }
  return (MR_SUCCESS);
}

/**
 * Recursively free all allocated memory. Needs to be done from bottom to top.
 * @param ptrs resizable array with serialized data
 * @return status
 */
mr_status_t
mr_free_recursively (mr_ptrdes_t * ptrs)
{
  mr_conf_init ();
  
  if (NULL == ptrs)
    return (MR_FAILURE);

  mr_idx_t i;
  mr_status_t status = MR_SUCCESS;
  for (i = 1; i < ptrs[0].next; ++i)
    {
      ptrs[i].vt_ptr = NULL;

      if ((ptrs[i].idx != MR_NULL_IDX) && !(ptrs[i].flags & (MR_IS_NULL | MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE)) &&
	  ((MR_TYPE_POINTER == ptrs[i].mr_type) || (MR_TYPE_STRING == ptrs[i].mr_type)))
	{
	  if (NULL == ptrs[i].data.ptr)
	    {
	      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
	      status = MR_FAILURE;
	    }
	  else
	    ptrs[i].vt_ptr = *(void**)ptrs[i].data.ptr;
	}
    }

  for (i = 1; i < ptrs[0].next; ++i)
    if (ptrs[i].vt_ptr)
      MR_FREE (ptrs[i].vt_ptr);

  return (status);
}

static mr_status_t
calc_relative_addr (mr_ptrdes_t * ptrs, mr_idx_t idx, int level, mr_dfs_order_t order, void * context)
{
  if (MR_DFS_PRE_ORDER != order)
    return (MR_SUCCESS);

  /* if new address is not set yet, then it could be calculated as relative address from the parent node */
  if (NULL == ptrs[idx].vt_ptr)
    {
      mr_idx_t parent = ptrs[idx].parent;
      ptrs[idx].vt_ptr = &((char*)ptrs[parent].vt_ptr)[ptrs[idx].data.ptr - ptrs[parent].data.ptr];
    }
  return (MR_SUCCESS);
}

/**
 * Recursively copy
 * @param ptrs resizable array with serialized data
 * @return status
 */
mr_status_t
mr_copy_recursively (mr_ptrdes_t * ptrs, void * dst)
{
  mr_conf_init ();

  if ((NULL == ptrs) || (NULL == dst))
    return (MR_FAILURE);

  mr_idx_t i;
  for (i = 1; i < ptrs[0].next; ++i)
    {
      ptrs[i].vt_ptr = NULL;
      ptrs[i].value_type = MR_VT_PTR;
    }

  /* copy first level struct */
  memcpy (dst, ptrs[1].data.ptr, ptrs[1].MR_SIZE);
  ptrs[1].vt_ptr = dst;

  /* NB index 1 is excluded */
  for (i = 2; i < ptrs[0].next; ++i)
    /*
      process nodes that are in final save graph (ptrs[i].idx != MR_NULL_IDX)
      and are not references on other nodes and not a NULL pointer
    */
    if ((ptrs[i].idx != MR_NULL_IDX) && !(ptrs[i].flags & (MR_IS_NULL | MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE))
	&& (ptrs[i].mr_type == MR_TYPE_POINTER))
      {
	mr_idx_t idx;
	char * copy;
	typeof (ptrs[i].MR_SIZE) size = ptrs[i].MR_SIZE;

	if (ptrs[i].first_child == MR_NULL_IDX)
	  {
	    mr_td_t * tdp = ptrs[i].fdp ? ptrs[i].fdp->stype.tdp : NULL;
	    char * name = ptrs[i].fdp ? ptrs[i].fdp->name.str : MR_DEFAULT_NODE_NAME;
	    MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_POINTER_NODE_CHILD_MISSING,
			tdp ? tdp->type.str : "unknown", name);
	    goto failure;
	  }
	    
	if (size < 0)
	  {
	    MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_WRONG_SIZE_FOR_DYNAMIC_ARRAY, size);
	    goto failure;
	  }
	    
	copy = MR_CALLOC (1, size);
	if (NULL == copy)
	  {
	    MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	    goto failure;
	  }

	/* copy data from source */
	memcpy (copy, *(void**)ptrs[i].data.ptr, size);
	/* go thru all childs and calculate their addresses in newly allocated chunk */
	for (idx = ptrs[i].first_child; idx != MR_NULL_IDX; idx = ptrs[idx].next)
	  ptrs[idx].vt_ptr = &copy[ptrs[idx].data.intptr - *(intptr_t*)ptrs[i].data.ptr];
      }

  /* depth search thru the graph and calculate new addresses for all nodes */
  mr_ptrs_dfs (ptrs, calc_relative_addr, NULL);

  for (i = 2; i < ptrs[0].next; ++i)
    if ((ptrs[i].idx != MR_NULL_IDX) && !(ptrs[i].flags & (MR_IS_NULL | MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE))
	&& (ptrs[i].mr_type == MR_TYPE_STRING) && (ptrs[i].vt_ptr != NULL))
      {
	*(char**)ptrs[i].vt_ptr = mr_strdup (*(char**)ptrs[i].data.ptr);
	if (NULL == *(char**)ptrs[i].vt_ptr)
	  {
	    for (--i; i > 1; --i)
	      if ((MR_TYPE_STRING == ptrs[i].mr_type) && !(ptrs[i].flags & (MR_IS_NULL | MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE))
		  && (ptrs[i].vt_ptr != NULL) && (*(char**)ptrs[i].vt_ptr != NULL))
		MR_FREE (*(char**)ptrs[i].vt_ptr);
	    goto failure;
	  }
      }

  /* now we should update pointers in a copy */
  for (i = 2; i < ptrs[0].next; ++i)
    if (ptrs[i].idx != MR_NULL_IDX) /* skip invalid nodes */
      switch (ptrs[i].mr_type)
	{
	case MR_TYPE_STRING:
	  /* update pointer in the copy */
	  if (!(ptrs[i].flags & MR_IS_NULL))
	    {
	      if (ptrs[i].flags & MR_IS_REFERENCE)
		*(void**)ptrs[i].vt_ptr = ptrs[ptrs[i].first_child].vt_ptr;
	      else if (ptrs[i].flags & MR_IS_CONTENT_REFERENCE)
		*(void**)ptrs[i].vt_ptr = *(void**)ptrs[ptrs[i].first_child].vt_ptr;
	    }
	  break;

	case MR_TYPE_POINTER:
	  /* update pointer in the copy */
	  *(void**)ptrs[i].vt_ptr = ptrs[ptrs[i].first_child].vt_ptr;
	  break;

	default:
	  break;
	}

  return (MR_SUCCESS);

 failure:
  for (i = 2; i < ptrs[0].next; ++i)
    if ((MR_TYPE_POINTER == ptrs[i].mr_type) && !(ptrs[i].flags & (MR_IS_NULL | MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE)) &&
	(ptrs[i].first_child != MR_NULL_IDX) && (ptrs[ptrs[i].first_child].vt_ptr != NULL))
      MR_FREE (ptrs[ptrs[i].first_child].vt_ptr);

  return (MR_FAILURE);
}

mr_hash_value_t
mr_hash_block (const void * block, mr_size_t size)
{
  const uint8_t * _block = block;
  mr_hash_value_t hash_value = 0;
  while (size > sizeof (hash_value))
    {
      /* pointer block might be unaligned to a size of mr_hash_value_t.
	 This makes sanitized tests fail. To avoid that we copy block to a properly aligned variable bafore use. */
      mr_hash_value_t word;
      __builtin_memcpy (&word, _block, sizeof (word));
      _block += sizeof (word);
      hash_value ^= (hash_value << 1) + word;
      size -= sizeof (hash_value);
    }
  
  if (size > 0)
    {
      mr_hash_value_t last = 0; /* make sure that the rest of hashing block is zeroed */
      __builtin_memcpy (&last, _block, size);
      hash_value ^= (hash_value << 1) + last;
    }
  
  return (hash_value);
}

/**
 * Default hash function.
 * @param str a pointer on null terminated string
 * @return Hash function value.
 */
mr_hash_value_t
mr_hash_str (const char * str)
{
  return (str ? mr_hash_block (str, strlen (str)) : 1);
}

static mr_status_t
node_hash (mr_ptrdes_t * ptrs, mr_idx_t idx, int level, mr_dfs_order_t order, void * context)
{
  if (MR_DFS_POST_ORDER != order)
    return (MR_SUCCESS);

  mr_ptrdes_t * ptrdes = &ptrs[idx];

  switch (ptrdes->mr_type)
    {
    case MR_TYPE_STRING:
      ptrdes->vt_uintptr = mr_hash_str (*(char**)ptrdes->data.ptr);
      break;
	  
    case MR_TYPE_CHAR_ARRAY:
      ptrdes->vt_uintptr = mr_hash_str ((char*)ptrdes->data.ptr);
      break;
	  
    case MR_TYPE_BOOL:
      ptrdes->vt_uintptr = *(bool*)ptrdes->data.ptr;
      break;

#define CASE_MR_TYPE_HASH(TYPE)					\
      case MR_TYPE_DETECT (TYPE):				\
	ptrdes->vt_uintptr =				\
	  mr_hash_block (ptrdes->data.ptr, sizeof (TYPE));	\
	break;

#define CASE_MR_FLOAT_TYPE_HASH(TYPE)					\
      case MR_TYPE_DETECT (TYPE):					\
	ptrdes->vt_uintptr = __builtin_isnan (*(TYPE*)ptrdes->data.ptr) ? -1 : mr_hash_block (ptrdes->data.ptr, sizeof (TYPE)); \
	break;

#define CASE_MR_COMPLEX_FLOAT_TYPE_HASH(TYPE)				\
      case MR_TYPE_DETECT (TYPE):					\
	ptrdes->vt_uintptr = (__builtin_isnan (__real__ *(TYPE*)ptrdes->data.ptr) || __builtin_isnan (__imag__ *(TYPE*)ptrdes->data.ptr)) ? -1 : mr_hash_block (ptrdes->data.ptr, sizeof (TYPE)); \
	break;

      MR_FOREACH (CASE_MR_TYPE_HASH, char, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t, mr_uint128_t, mr_int128_t);
      MR_FOREACH (CASE_MR_FLOAT_TYPE_HASH, float, double);
      MR_FOREACH (CASE_MR_COMPLEX_FLOAT_TYPE_HASH, complex_float_t, complex_double_t);
	  
    case MR_TYPE_LONG_DOUBLE:
      ptrdes->vt_uintptr = __builtin_isnan (*(long_double_t*)ptrdes->data.ptr) ? -1 : mr_hash_block (ptrdes->data.ptr, MR_SIZEOF_LONG_DOUBLE);
      break;

    case MR_TYPE_COMPLEX_LONG_DOUBLE:
      {
	long double * real = &__real__ *(complex long double *)ptrdes->data.ptr;
	long double * imag = &__imag__ *(complex long double *)ptrdes->data.ptr;
	ptrdes->vt_uintptr =
	  (__builtin_isnan (*real) || __builtin_isnan (*imag)) ? -1 :
	  mr_hash_block (real, MR_SIZEOF_LONG_DOUBLE) + mr_hash_block (imag, MR_SIZEOF_LONG_DOUBLE);
      }
      break;
	  
    case MR_TYPE_STRUCT:
    case MR_TYPE_ARRAY:
    case MR_TYPE_POINTER:
    case MR_TYPE_UNION:
    case MR_TYPE_ANON_UNION:
    case MR_TYPE_NAMED_ANON_UNION:
      {
	mr_idx_t child;
	mr_hash_value_t hash_value = 0;
	for (child = ptrdes->first_child; child != MR_NULL_IDX; child = ptrs[child].next)
	  hash_value = hash_value * 3 + ptrs[child].vt_uintptr + 1;
	ptrdes->vt_uintptr = hash_value;
	break;
      }
	  
    case MR_TYPE_ENUM:
      {
	mr_td_t * tdp = ptrdes->fdp ? ptrdes->fdp->stype.tdp : NULL;
	if (NULL == tdp)
	  break;
	if (tdp->mr_type != MR_TYPE_ENUM)
	  break;
	ptrdes->vt_uintptr = mr_hash_block (ptrdes->data.ptr, tdp->param.enum_param.size_effective);
	break;
      }
	  
    case MR_TYPE_BITFIELD:
      {
	mr_uintmax_t value;
	mr_save_bitfield_value (ptrdes, &value);
	ptrdes->vt_uintptr = mr_hash_block (&value, sizeof (value));
	break;
      }
	  
    case MR_TYPE_FUNC_TYPE:
    case MR_TYPE_FUNC:
      ptrdes->vt_uintptr = mr_hash_block (ptrdes->data.ptr, sizeof (void *));
      break;

    case MR_TYPE_NONE:
    case MR_TYPE_VOID:
    case MR_TYPE_END_ANON_UNION:
    case MR_TYPE_LAST:
      break;
    }

  return (MR_SUCCESS);
}

mr_hash_value_t
mr_hash_struct (mr_ptrdes_t * ptrs)
{
  mr_conf_init ();
  
  if (NULL == ptrs)
    return (0);

  mr_idx_t i;
  for (i = 1; i < ptrs[0].next; ++i)
    {
      ptrs[i].vt_uintptr = 0;
      ptrs[i].value_type = MR_VT_UINTPTR;
    }
  
  mr_ptrs_dfs (ptrs, node_hash, NULL);
  return (ptrs[1].vt_uintptr);
}

int
mr_cmp_structs (mr_ptrdes_t * x, mr_ptrdes_t * y)
{
  mr_conf_init ();
  
  if ((NULL == x) || (NULL == y))
    return ((x > y) - (x < y));

  mr_idx_t i;
  mr_idx_t count_x = x[0].next;
  mr_idx_t count_y = y[0].next;

  int diff = (count_x > count_y) - (count_x < count_y);
  if (diff)
    return (diff);

  for (i = 1; i < count_x; ++i)
    {
      mr_ptrdes_t * x_i = &x[i];
      mr_ptrdes_t * y_i = &y[i];

      diff = (x_i->idx > y_i->idx) - (x_i->idx < y_i->idx);
      if (diff)
	return (diff);

      if (x_i->idx == MR_NULL_IDX)
	continue;
      
      diff = memcmp (x_i, y_i, offsetof (mr_ptrdes_t, data));
      if (diff)
	return (diff);
      
      switch (x_i->mr_type)
	{
	case MR_TYPE_STRING:
	  {
	    char * _x = *(char**)x_i->data.ptr;
	    char * _y = *(char**)y_i->data.ptr;
	    if (_x == _y)
	      break;
	    if ((NULL == _x) || (NULL == _y))
	      return ((_x > _y) - (_x < _y));
	    diff = strcmp (_x, _y);
	    if (diff)
	      return (diff);
	    break;
	  }
	  
	case MR_TYPE_CHAR_ARRAY:
	  diff = strcmp (x_i->data.ptr, y_i->data.ptr);
	  if (diff)
	    return (diff);
	  break;

#define CASE_MR_TYPE_CMP(TYPE)						\
	  case MR_TYPE_DETECT (TYPE):					\
	    diff = (*(TYPE*)x_i->data.ptr > *(TYPE*)y_i->data.ptr) -	\
	      (*(TYPE*)x_i->data.ptr < *(TYPE*)y_i->data.ptr);		\
	    if (diff)							\
	      return (diff);						\
	    break;

#define CASE_MR_TYPE_CMP_FLOAT(TYPE)					\
	  case MR_TYPE_DETECT (TYPE):					\
	    {								\
	      TYPE _x = *(TYPE*)x_i->data.ptr;				\
	      TYPE _y = *(TYPE*)y_i->data.ptr;				\
	      if (!__builtin_isnan (_x) && !__builtin_isnan (_y))	\
		{							\
		  diff = (_x > _y) - (_x < _y);				\
		  if (diff)						\
		    return (diff);					\
		}							\
	      diff = !__builtin_isnan (_x) - !__builtin_isnan (_y);	\
	      if (diff)							\
		return (diff);						\
	    }								\
	    break;

#define CASE_MR_TYPE_CMP_COMPLEX(TYPE)					\
	  case MR_TYPE_DETECT (TYPE):					\
	    {								\
	      TYPE _x = *(TYPE*)x_i->data.ptr;				\
	      TYPE _y = *(TYPE*)y_i->data.ptr;				\
	      bool rx_isnan = __builtin_isnan (__real__ _x);		\
	      bool ix_isnan = __builtin_isnan (__imag__ _x);		\
	      bool ry_isnan = __builtin_isnan (__real__ _y);		\
	      bool iy_isnan = __builtin_isnan (__imag__ _y);		\
	      if (((rx_isnan || ix_isnan)) && ((ry_isnan || iy_isnan)))	\
		continue;						\
	      if (!rx_isnan && !ry_isnan)				\
		{							\
		  diff = (__real__ _x > __real__ _y) - (__real__ _x < __real__ _y); \
		  if (diff)						\
		    return (diff);					\
		}							\
	      else							\
		{							\
		  diff = !rx_isnan - !ry_isnan;				\
		  if (diff)						\
		    return (diff);					\
		}							\
	      if (!ix_isnan && !iy_isnan)				\
		{							\
		  diff = (__imag__ _x > __imag__ _y) - (__imag__ _x < __imag__ _y); \
		  if (diff)						\
		    return (diff);					\
		}							\
	      else							\
		{							\
		  diff = !ix_isnan - !iy_isnan;				\
		  if (diff)						\
		    return (diff);					\
		}							\
	    }								\
	    break;

	  MR_FOREACH (CASE_MR_TYPE_CMP, char, bool, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, mr_int128_t, mr_uint128_t);
	  MR_FOREACH (CASE_MR_TYPE_CMP_FLOAT, float, double, long_double_t);
	  MR_FOREACH (CASE_MR_TYPE_CMP_COMPLEX, complex float, complex double, complex long double);

	case MR_TYPE_ENUM:
	  switch (x_i->mr_type_aux)
	    {
	      MR_FOREACH (CASE_MR_TYPE_CMP, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, mr_int128_t, mr_uint128_t);
	  
	    default:
	      break;
	    }
	  break;
	  
	case MR_TYPE_BITFIELD:
	  {
	    mr_uintmax_t x_value, y_value;
	    mr_save_bitfield_value (x_i, &x_value);
	    mr_save_bitfield_value (y_i, &y_value);
	    diff = (x_value > y_value) - (x_value < y_value);
	    if (diff)
	      return (diff);
	    break;
	  }
	  
	case MR_TYPE_FUNC_TYPE:
	case MR_TYPE_FUNC:
	  diff = (*(void**)x_i->data.ptr > *(void**)y_i->data.ptr) -
	    (*(void**)x_i->data.ptr < *(void**)y_i->data.ptr);
	  if (diff)
	    return (diff);
	  break;

	default:
	  break;
	}
    }
  return (0);
}
