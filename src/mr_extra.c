#include <metaresc.h>
#include <flt_values.h>

/**
 * Recursively free all allocated memory. Needs to be done from bottom to top.
 * @param ptrs resizable array with serialized data
 * @return status
 */
mr_status_t
mr_free_recursively (mr_ra_ptrdes_t * ptrs)
{
  int i;
  mr_status_t status = MR_SUCCESS;

  mr_conf_init ();
  
  if (NULL == ptrs)
    return (MR_FAILURE);
  if (NULL == ptrs->ra)
    return (MR_FAILURE);

  ptrs->ptrdes_type = MR_PD_CUSTOM;

  for (i = ptrs->size / sizeof (ptrs->ra[0]) - 1; i >= 0; --i)
    {
      mr_ptrdes_t * ptrdes = &ptrs->ra[i];
      ptrdes->res.data.ptr = NULL;
      ptrdes->res.type = NULL;
      ptrdes->res.MR_SIZE = 0;

      if ((ptrdes->ref_idx < 0) && (ptrdes->idx >= 0) && !ptrdes->flags.is_null &&
	  ((MR_TYPE_POINTER == ptrdes->mr_type) || (MR_TYPE_STRING == ptrdes->mr_type)))
	{
	  if (NULL == ptrdes->data.ptr)
	    {
	      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
	      status = MR_FAILURE;
	    }
	  else
	    ptrdes->res.data.ptr = *(void**)ptrdes->data.ptr;
	}
    }

  for (i = ptrs->size / sizeof (ptrs->ra[0]) - 1; i >= 0; --i)
    if (ptrs->ra[i].res.data.ptr)
      MR_FREE (ptrs->ra[i].res.data.ptr);

  return (status);
}

static mr_status_t
calc_relative_addr (mr_ra_ptrdes_t * ptrs, int idx, int level, mr_dfs_order_t order, void * context)
{
  if (MR_DFS_PRE_ORDER != order)
    return (MR_SUCCESS);

  /* is new address is not set yet, then it could be calculated as relative address from the parent node */
  if (NULL == ptrs->ra[idx].res.data.ptr)
    {
      int parent = ptrs->ra[idx].parent;
      ptrs->ra[idx].res.data.ptr = &((char*)ptrs->ra[parent].res.data.ptr)[ptrs->ra[idx].data.ptr - ptrs->ra[parent].data.ptr];
    }
  return (MR_SUCCESS);
}

/**
 * Recursively copy
 * @param ptrs resizable array with serialized data
 * @return status
 */
mr_status_t
mr_copy_recursively (mr_ra_ptrdes_t * ptrs, void * dst)
{
  int i;

  mr_conf_init ();

  if ((NULL == ptrs) || (NULL == dst))
    return (MR_FAILURE);
  if (NULL == ptrs->ra)
    return (MR_FAILURE);

  ptrs->ptrdes_type = MR_PD_CUSTOM;

  /* copy first level struct */
  memcpy (dst, ptrs->ra[0].data.ptr, ptrs->ra[0].MR_SIZE);
  ptrs->ra[0].res.data.ptr = dst;

  for (i = ptrs->size / sizeof (ptrs->ra[0]) - 1; i > 0; --i)
    {
      ptrs->ra[i].res.data.ptr = NULL;
      ptrs->ra[i].res.type = NULL;
      ptrs->ra[i].res.MR_SIZE = 0;
    }

  /* NB index 0 is excluded */
  for (i = ptrs->size / sizeof (ptrs->ra[0]) - 1; i > 0; --i)
    /*
      process nodes that are in final save graph (ptrs->ra[i].idx >= 0)
      and are not references on other nodes (ptrs->ra[i].ref_idx < 0)
      and not a NULL pointer
    */
    if ((ptrs->ra[i].idx >= 0) && (ptrs->ra[i].ref_idx < 0) && (true != ptrs->ra[i].flags.is_null))
      switch (ptrs->ra[i].mr_type)
	{
	case MR_TYPE_STRING:
	  if (*(char**)ptrs->ra[i].data.ptr != NULL)
	    {
	      ptrs->ra[i].res.type = mr_strdup (*(char**)ptrs->ra[i].data.ptr);
	      if (NULL == ptrs->ra[i].res.type)
		goto failure;
	    }
	  break;
	    
	case MR_TYPE_POINTER:
	  {
	    int idx;
	    char * copy;
	    ssize_t size = ptrs->ra[i].MR_SIZE;

	    if (ptrs->ra[i].first_child < 0)
	      {
		mr_td_t * tdp = ptrs->ra[i].fdp ? ptrs->ra[i].fdp->stype.tdp : NULL;
		char * name = ptrs->ra[i].fdp ? ptrs->ra[i].fdp->name.str : MR_DEFAULT_NODE_NAME;
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
	    memcpy (copy, *(void**)ptrs->ra[i].data.ptr, size);
	    /* go thru all childs and calculate their addresses in newly allocated chunk */
	    for (idx = ptrs->ra[i].first_child; idx >= 0; idx = ptrs->ra[idx].next)
	      ptrs->ra[idx].res.data.ptr = &copy[(char*)ptrs->ra[idx].data.ptr - *(char**)ptrs->ra[i].data.ptr];
	  }
	  break;
	default:
	  break;
	}

  /* depth search thru the graph and calculate new addresses for all nodes */
  mr_ptrs_dfs (ptrs, calc_relative_addr, NULL);

  /* now we should update pointers in a copy */
  for (i = ptrs->size / sizeof (ptrs->ra[0]) - 1; i > 0; --i)
    if ((ptrs->ra[i].idx >= 0)) /* skip invalid nodes */
      switch (ptrs->ra[i].mr_type)
	{
	case MR_TYPE_STRING:
	  /* update pointer in the copy */
	  if (!ptrs->ra[i].flags.is_null)
	    {
	      if (ptrs->ra[i].ref_idx < 0)
		*(char**)ptrs->ra[i].res.data.ptr = ptrs->ra[i].res.type;
	      else if (ptrs->ra[i].flags.is_content_reference)
		*(void**)ptrs->ra[i].res.data.ptr = ptrs->ra[ptrs->ra[i].ref_idx].res.type;
	      else
		*(void**)ptrs->ra[i].res.data.ptr = ptrs->ra[ptrs->ra[i].ref_idx].res.data.ptr;
	    }
	  break;

	case MR_TYPE_POINTER:
	  /* update pointer in the copy */
	  if (ptrs->ra[i].ref_idx < 0)
	    {
	      if (ptrs->ra[i].first_child >= 0)
		*(void**)ptrs->ra[i].res.data.ptr = ptrs->ra[ptrs->ra[i].first_child].res.data.ptr;
	    }
	  else
	    *(void**)ptrs->ra[i].res.data.ptr = ptrs->ra[ptrs->ra[i].ref_idx].res.data.ptr;
	  break;

	default:
	  break;
	}

  return (MR_SUCCESS);

 failure:
  for (i = ptrs->size / sizeof (ptrs->ra[0]) - 1; i > 0; --i)
    if ((MR_TYPE_STRING == ptrs->ra[i].mr_type) && (ptrs->ra[i].res.type != NULL))
      MR_FREE (ptrs->ra[i].res.type);
    else if ((MR_TYPE_POINTER == ptrs->ra[i].mr_type) &&
	     (ptrs->ra[i].first_child >= 0))
      if (ptrs->ra[ptrs->ra[i].first_child].res.data.ptr != NULL)
	MR_FREE (ptrs->ra[ptrs->ra[i].first_child].res.data.ptr);

  return (MR_FAILURE);
}

mr_hash_value_t
mr_hash_block (void * block, mr_size_t size)
{
  mr_hash_value_t * _block = block;
  mr_hash_value_t hash_value = 0;
  while (size > sizeof (hash_value))
    {
      hash_value ^= (hash_value << 1) + *_block++;
      size -= sizeof (hash_value);
    }
  
  if (size > 0)
    {
      mr_hash_value_t last = 0;
      memcpy (&last, _block, size);
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
mr_hash_str (char * str)
{
  return (str ? mr_hash_block (str, strlen (str)) : 1);
}

static mr_status_t
node_hash (mr_ra_ptrdes_t * ptrs, int idx, int level, mr_dfs_order_t order, void * context)
{
  if (MR_DFS_POST_ORDER != order)
    return (MR_SUCCESS);

  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];

  switch (ptrdes->mr_type)
    {
    case MR_TYPE_STRING:
      ptrdes->res.data.uintptr = mr_hash_str (*(char**)ptrdes->data.ptr);
      break;
	  
    case MR_TYPE_CHAR_ARRAY:
      ptrdes->res.data.uintptr = mr_hash_str ((char*)ptrdes->data.ptr);
      break;
	  
    case MR_TYPE_BOOL:
      ptrdes->res.data.uintptr = *(bool*)ptrdes->data.ptr;
      break;

#define CASE_MR_TYPE_HASH(TYPE)					\
      case MR_TYPE_DETECT (TYPE):				\
	ptrdes->res.data.uintptr =				\
	  mr_hash_block (ptrdes->data.ptr, sizeof (TYPE));	\
	break;

#define CASE_MR_FLOAT_TYPE_HASH(TYPE)					\
      case MR_TYPE_DETECT (TYPE):					\
	ptrdes->res.data.uintptr = MR_ISNAN (*(TYPE*)ptrdes->data.ptr) ? -1 : mr_hash_block (ptrdes->data.ptr, sizeof (TYPE)); \
	break;

#define CASE_MR_COMPLEX_FLOAT_TYPE_HASH(TYPE)				\
      case MR_TYPE_DETECT (TYPE):					\
	ptrdes->res.data.uintptr = (MR_ISNAN (__real__ *(TYPE*)ptrdes->data.ptr) || MR_ISNAN (__imag__ *(TYPE*)ptrdes->data.ptr)) ? -1 : mr_hash_block (ptrdes->data.ptr, sizeof (TYPE)); \
	break;

      MR_FOREACH (CASE_MR_TYPE_HASH, char, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t, mr_uint128_t, mr_int128_t);
      MR_FOREACH (CASE_MR_FLOAT_TYPE_HASH, float, double);
      MR_FOREACH (CASE_MR_COMPLEX_FLOAT_TYPE_HASH, complex_float_t, complex_double_t);
	  
    case MR_TYPE_LONG_DOUBLE:
      ptrdes->res.data.uintptr = MR_ISNAN (*(long_double_t*)ptrdes->data.ptr) ? -1 : mr_hash_block (ptrdes->data.ptr, MR_SIZEOF_LONG_DOUBLE);
      break;

    case MR_TYPE_COMPLEX_LONG_DOUBLE:
      {
	long double * real = &__real__ *(complex long double *)ptrdes->data.ptr;
	long double * imag = &__imag__ *(complex long double *)ptrdes->data.ptr;
	ptrdes->res.data.uintptr =
	  (MR_ISNAN (*real) || MR_ISNAN (*imag)) ? -1 :
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
	int child;
	mr_hash_value_t hash_value = 0;
	for (child = ptrdes->first_child; child >= 0; child = ptrs->ra[child].next)
	  hash_value = hash_value * 3 + ptrs->ra[child].res.data.uintptr + 1;
	ptrdes->res.data.uintptr = hash_value;
	break;
      }
	  
    case MR_TYPE_ENUM:
      {
	mr_td_t * tdp = ptrdes->fdp ? ptrdes->fdp->stype.tdp : NULL;
	if (NULL == tdp)
	  break;
	if (tdp->mr_type != MR_TYPE_ENUM)
	  break;
	ptrdes->res.data.uintptr = mr_hash_block (ptrdes->data.ptr, tdp->param.enum_param.size_effective);
	break;
      }
	  
    case MR_TYPE_BITFIELD:
      {
	mr_uintmax_t value;
	mr_save_bitfield_value (ptrdes, &value);
	ptrdes->res.data.uintptr = mr_hash_block (&value, sizeof (value));
	break;
      }
	  
    case MR_TYPE_FUNC_TYPE:
    case MR_TYPE_FUNC:
      ptrdes->res.data.uintptr = mr_hash_block (ptrdes->data.ptr, sizeof (void *));
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
mr_hash_struct (mr_ra_ptrdes_t * ptrs)
{
  mr_conf_init ();
  
  if (NULL == ptrs)
    return (0);
  if ((NULL == ptrs->ra) || (ptrs->size < sizeof (ptrs->ra[0])))
    return (0);

  int i;
  for (i = ptrs->size / sizeof (ptrs->ra[0]) - 1; i >= 0; --i)
    {
      ptrs->ra[i].res.data.uintptr = 0;
      ptrs->ra[i].res.type = "uintptr";
    }
  
  ptrs->ptrdes_type = MR_PD_CUSTOM;
  mr_ptrs_dfs (ptrs, node_hash, NULL);
  return (ptrs->ra[0].res.data.uintptr);
}

int
mr_cmp_structs (mr_ra_ptrdes_t * x, mr_ra_ptrdes_t * y)
{
  mr_conf_init ();
  
  if ((NULL == x) || (NULL == y))
    return (0);
  if ((NULL == x->ra) || (x->size < sizeof (x->ra[0])) ||
      (NULL == y->ra) || (y->size < sizeof (y->ra[0])))
    return (0);

  int diff = (x->size > y->size) - (x->size < y->size);
  if (diff)
    return (diff);

  unsigned i, count = x->size / sizeof (x->ra[0]);
  for (i = 0; i < count; ++i)
    {
      mr_ptrdes_t * x_i = &x->ra[i];
      mr_ptrdes_t * y_i = &y->ra[i];

      diff = (x_i->idx > y_i->idx) - (x_i->idx < y_i->idx);
      if (diff)
	return (diff);

      if (x_i->idx < 0)
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
	      if (!MR_ISNAN (_x) && !MR_ISNAN (_y))			\
		{							\
		  diff = (_x > _y) - (_x < _y);				\
		  if (diff)						\
		    return (diff);					\
		}							\
	      diff = !MR_ISNAN (_x) - !MR_ISNAN (_y);			\
	      if (diff)							\
		return (diff);						\
	    }								\
	    break;

#define CASE_MR_TYPE_CMP_COMPLEX(TYPE)					\
	  case MR_TYPE_DETECT (TYPE):					\
	    {								\
	      TYPE _x = *(TYPE*)x_i->data.ptr;				\
	      TYPE _y = *(TYPE*)y_i->data.ptr;				\
	      bool rx_isnan = MR_ISNAN (__real__ _x);			\
	      bool ix_isnan = MR_ISNAN (__imag__ _x);			\
	      bool ry_isnan = MR_ISNAN (__real__ _y);			\
	      bool iy_isnan = MR_ISNAN (__imag__ _y);			\
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
