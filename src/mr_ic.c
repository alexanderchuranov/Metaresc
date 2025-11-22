/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

#include <string.h>

#include <metaresc.h>
#include <mr_hsort.h>
#include <mr_btree.h>
#include <mr_ic.h>

#undef MR_SAVE
#define MR_SAVE(TDP, S_PTR) ({						\
      void * __ptr__ = (void*)S_PTR;					\
      mr_fd_t __fd__;							\
      memset (&__fd__, 0, sizeof (__fd__));				\
      __fd__.non_persistent = true;					\
      __fd__.stype.tdp = TDP;						\
      __fd__.name = __fd__.stype.tdp->type;				\
      __fd__.stype.type = __fd__.stype.tdp->type.str;			\
      __fd__.stype.size = __fd__.stype.tdp->size;			\
      __fd__.stype.mr_type = __fd__.stype.tdp->mr_type;			\
      mr_save (__ptr__, &__fd__);					\
    })

mr_hash_value_t
mr_generic_hash (const mr_ptr_t x, const void * context)
{
  mr_td_t * tdp = (mr_td_t*)context;
  return (MR_HASH_STRUCT (tdp, x.ptr));
}

int
mr_generic_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  mr_td_t * tdp = (mr_td_t*)context;
  return (MR_CMP_STRUCTS (tdp, x.ptr, y.ptr));
}

mr_status_t
mr_generic_sort (void * data, size_t count, char * key_type)
{
  mr_td_t * tdp = mr_get_td_by_name (key_type);
  
  if ((NULL == tdp) || (0 == tdp->size))
    return (MR_FAILURE);
  
  mr_hsort (data, count, tdp->size, mr_generic_cmp, tdp);
  return (MR_SUCCESS);
}

#define MR_TYPE_CMP_FUNC_NAME(TYPE) mr_ ## TYPE ## _cmp
#define MR_TYPE_CMP_FUNC(TYPE)						\
  static int MR_TYPE_CMP_FUNC_NAME (TYPE)				\
       (const mr_ptr_t x, const mr_ptr_t y, const void * context)	\
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

/* ----------------------- MR_IC_UNSORTED_ARRAY ----------------------- */

mr_ptr_t *
mr_ic_unsorted_array_add (mr_ic_t * ic, mr_ptr_t key)
{
  mr_ptr_t * find = mr_ic_find (ic, key);
  if (find != NULL)
    return (find);

  mr_ic_t dst_ic;
  mr_status_t status = mr_ic_sorted_array_new (&dst_ic, ic->compar_fn, ic->key_type, &ic->context);
  if (MR_SUCCESS != status)
    return (NULL);
      
  status = mr_ic_index (&dst_ic, ic->rarray.ra, ic->rarray.size);
  if (MR_SUCCESS != status)
    return (NULL);
      
  *ic = dst_ic;
  return (mr_ic_add (ic, key));
}

mr_status_t
mr_ic_unsorted_array_del (mr_ic_t * ic, mr_ptr_t key)
{
  mr_ptr_t * find = mr_ic_find (ic, key);
  if (NULL == find)
    return (MR_FAILURE);
  
  mr_ic_t dst_ic;
  mr_status_t status = mr_ic_sorted_array_new (&dst_ic, ic->compar_fn, ic->key_type, &ic->context);
  if (MR_SUCCESS != status)
    return (status);
      
  status = mr_ic_index (&dst_ic, ic->rarray.ra, ic->rarray.size);
  if (MR_SUCCESS != status)
    return (status);
      
  *ic = dst_ic;
  return (mr_ic_del (ic, key));
}

mr_ptr_t *
mr_ic_unsorted_array_find (mr_ic_t * ic, mr_ptr_t key)
{
  unsigned i, count = ic->rarray.size / sizeof (ic->rarray.ra[0]);
  for (i = 0; i < count; ++i)
    if (0 == ic->compar_fn (key, ic->rarray.ra[i], ic->context.data.ptr))
      return (&ic->rarray.ra[i]);
  return (NULL);
}

mr_status_t
mr_ic_unsorted_array_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, void * context)
{
  unsigned i, count = ic->rarray.size / sizeof (ic->rarray.ra[0]);
  for (i = 0; i < count; ++i)
    if (MR_SUCCESS != visit_fn (ic->rarray.ra[i], context))
      return (MR_FAILURE);
  return (MR_SUCCESS);
}

mr_status_t
mr_ic_unsorted_array_index (mr_ic_t * ic, mr_ptr_t * rarray, size_t size)
{
  ic->items_count = size / sizeof (rarray[0]);
  ic->rarray.ra = rarray;
  ic->rarray.size = ic->items_count * sizeof (ic->rarray.ra[0]);
  ic->rarray.alloc_size = -1;
  return (MR_SUCCESS);
}

void
mr_ic_unsorted_array_free (mr_ic_t * ic)
{
  ic->rarray.ra = NULL;
  ic->rarray.size = ic->rarray.alloc_size = 0;
  ic->items_count = 0;
}

mr_status_t
mr_ic_unsorted_array_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type, mr_res_t * context)
{
  static mr_ic_virt_func_t virt_func = {
    .add = mr_ic_unsorted_array_add,
    .del = mr_ic_unsorted_array_del,
    .find = mr_ic_unsorted_array_find,
    .foreach = mr_ic_unsorted_array_foreach,
    .index = mr_ic_unsorted_array_index,
    .free = mr_ic_unsorted_array_free,
  };
  mr_res_t generic_context = {
    .type = "mr_td_t",
    .mr_size = sizeof (mr_td_t),
  };
  
  if (NULL == ic)
    return (MR_FAILURE);

  if ((compar_fn == NULL) && (context == NULL))
    {
      mr_td_t * tdp = mr_get_td_by_name_internal (key_type);
      if (tdp)
	{
	  generic_context.data.ptr = tdp;
	  compar_fn = mr_generic_cmp;
	  context = &generic_context;
	}
    }

  if (NULL == compar_fn)
    return (MR_FAILURE);
  
  if (context)
    ic->context = *context;
  else
    memset (&ic->context, 0, sizeof (ic->context));
  
  ic->ic_type = MR_IC_UNSORTED_ARRAY;
  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->virt_func = &virt_func;
  ic->items_count = 0;
  
  ic->rarray.ra = NULL;
  ic->rarray.size = ic->rarray.alloc_size = 0;

  return (MR_SUCCESS);
}

/* ----------------------- MR_IC_SORTED_ARRAY ----------------------- */

int
mr_ic_sorted_array_find_idx (mr_ptr_t key, mr_ic_rarray_t * rarray, mr_compar_fn_t compar_fn, void * context, unsigned * idx)
{
  unsigned down = 0;
  unsigned up = rarray->size / sizeof (rarray->ra[0]);
  int diff = !0;

  while ((up != down) && (diff != 0))
    {
      unsigned mid = (down + up) >> 1;
      diff = compar_fn (key, rarray->ra[mid], context);
      if (diff <= 0)
	up = mid;
      else
	down = mid + 1;
    }
  *idx = up;
  return (diff);
}

mr_ptr_t *
mr_ic_sorted_array_add (mr_ic_t * ic, mr_ptr_t key)
{
  unsigned idx;
  if (mr_ic_sorted_array_find_idx (key, &ic->rarray, ic->compar_fn, ic->context.data.ptr, &idx) == 0)
    return (&ic->rarray.ra[idx]);

  mr_ptr_t * add = mr_rarray_allocate_element ((void*)&ic->rarray.ra, &ic->rarray.size, &ic->rarray.alloc_size, sizeof (ic->rarray.ra[0]));
  if (NULL == add)
    return (NULL);

  memmove (&ic->rarray.ra[idx + 1], &ic->rarray.ra[idx], ic->rarray.size - (idx + 1) * sizeof (ic->rarray.ra[0]));
  ic->rarray.ra[idx] = key;
  ic->items_count = ic->rarray.size / sizeof (ic->rarray.ra[0]);

  return (&ic->rarray.ra[idx]);
}

mr_status_t
mr_ic_sorted_array_del (mr_ic_t * ic, mr_ptr_t key)
{
  unsigned idx;
  if (mr_ic_sorted_array_find_idx (key, &ic->rarray, ic->compar_fn, ic->context.data.ptr, &idx) != 0)
    return (MR_FAILURE);
   
  ic->rarray.size -= sizeof (ic->rarray.ra[0]);
  ic->items_count = ic->rarray.size / sizeof (ic->rarray.ra[0]);
  memmove (&ic->rarray.ra[idx], &ic->rarray.ra[idx + 1], (ic->items_count - idx) * sizeof (ic->rarray.ra[0]));
  return (MR_SUCCESS);
}

mr_ptr_t *
mr_ic_sorted_array_find (mr_ic_t * ic, mr_ptr_t key)
{
  unsigned idx;
  return ((mr_ic_sorted_array_find_idx (key, &ic->rarray, ic->compar_fn, ic->context.data.ptr, &idx) == 0) ?
	  &ic->rarray.ra[idx] : NULL);
}

TYPEDEF_STRUCT (mr_sort_context_t,
		(mr_ptr_t *, rarray, , "input array for indexing"),
		(mr_compar_fn_t, compar_fn, , "sorting function"),
		(typeof (((mr_ic_t*)0)->context.data.ptr), context_data_ptr, , "context pointer for sorting function"),
		);

static int
mr_sort_key_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const mr_sort_context_t * mr_sort_context = context;
  const mr_ptr_t * _x = x.ptr;
  const mr_ptr_t * _y = y.ptr;
  /* First compare values */
  int diff = mr_sort_context->compar_fn (mr_sort_context->rarray[_x->uintptr],
					 mr_sort_context->rarray[_y->uintptr],
					 mr_sort_context->context_data_ptr);
  if (diff)
    return (diff);
  /* If values are equal sort by indexes in input array */
  return ((_x->uintptr > _y->uintptr) - (_x->uintptr < _y->uintptr));
}

mr_status_t
mr_ic_sorted_array_index (mr_ic_t * ic, mr_ptr_t * rarray, size_t size)
{
  ic->items_count = 0;
  ic->rarray.size = 0;
  unsigned items_count = size / sizeof (rarray[0]);
  if (items_count > 0)
    {
      mr_ptr_t * add = mr_rarray_allocate_element ((void*)&ic->rarray.ra, &ic->rarray.size, &ic->rarray.alloc_size, items_count * sizeof (ic->rarray.ra[0]));
      if (NULL == add)
	return (MR_FAILURE);

      /*
	Heap sort is not a stable sort. As a result elements with equal keys might come out in an arbitrary order.
	To achieve stable sorting here we will sort array of indexes in original array. After that will make deduplication
	and finally replace indexes on values from the input array.
       */
      unsigned i;
      for (i = 0; i < items_count; ++i)
	ic->rarray.ra[i].uintptr = i;
      mr_sort_context_t mr_sort_context = {
	.rarray = rarray,
	.compar_fn = ic->compar_fn,
	.context_data_ptr = ic->context.data.ptr,
      };

      mr_hsort (ic->rarray.ra, items_count, sizeof (ic->rarray.ra[0]), mr_sort_key_cmp, &mr_sort_context);

      /* Deduplicate equal elements and replace indexes on values from the input array */
      unsigned src, dst = 0;
      mr_ptr_t prev = rarray[ic->rarray.ra[0].uintptr];
      ic->rarray.ra[dst++] = prev;
      for (src = 1; src < items_count; ++src)
	{
	  mr_ptr_t next = rarray[ic->rarray.ra[src].uintptr];
	  if (ic->compar_fn (next, prev, ic->context.data.ptr) != 0)
	    ic->rarray.ra[dst++] = next;
	  prev = next;
	}
      
      ic->items_count = dst; /* after deduplication actual number of elements might be lower */
      ic->rarray.size = ic->items_count * sizeof (ic->rarray.ra[0]); /* used array size */
    }
  return (MR_SUCCESS);
}

void
mr_ic_sorted_array_free (mr_ic_t * ic)
{
  if (NULL != ic->rarray.ra)
    MR_FREE (ic->rarray.ra);
  ic->rarray.ra = NULL;
  ic->rarray.size = ic->rarray.alloc_size = 0;
  ic->items_count = 0;
}

mr_status_t
mr_ic_sorted_array_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type, mr_res_t * context)
{
  static mr_ic_virt_func_t virt_func = {
    .add = mr_ic_sorted_array_add,
    .del = mr_ic_sorted_array_del,
    .find = mr_ic_sorted_array_find,
    .foreach = mr_ic_unsorted_array_foreach,
    .index = mr_ic_sorted_array_index,
    .free = mr_ic_sorted_array_free,
  };
  mr_res_t generic_context = {
    .type = "mr_td_t",
    .mr_size = sizeof (mr_td_t),
  };
  
  if (NULL == ic)
    return (MR_FAILURE);

  if ((compar_fn == NULL) && (context == NULL))
    {
      mr_td_t * tdp = mr_get_td_by_name_internal (key_type);
      if (tdp)
	{
	  generic_context.data.ptr = tdp;
	  compar_fn = mr_generic_cmp;
	  context = &generic_context;
	}
    }
  
  if (NULL == compar_fn)
    return (MR_FAILURE);

  if (context)
    ic->context = *context;
  else
    memset (&ic->context, 0, sizeof (ic->context));

  ic->ic_type = MR_IC_SORTED_ARRAY;
  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->virt_func = &virt_func;
  ic->items_count = 0;

  ic->rarray.ra = NULL;
  ic->rarray.size = ic->rarray.alloc_size = 0;
  
  return (MR_SUCCESS);
}

/* ----------------------- MR_IC_HASH ----------------------- */

void
mr_ic_hash_free (mr_ic_t * ic)
{
  if (NULL != ic->hash.hash_table)
    MR_FREE (ic->hash.hash_table);
  ic->hash.hash_table = NULL;
  ic->hash.count = 0;
  ic->hash.zero_key = false;
  ic->items_count = 0;
}

static mr_ptr_t *
mr_ic_hash_index_add (mr_ic_t * ic, mr_ptr_t key)
{
  if (ic->hash.count >= 2)
    {
      unsigned count = ic->hash.count - 1;
      
      if (0 == key.intptr)
	{
	  ++ic->items_count;
	  ic->hash.zero_key = true;
	  ic->hash.hash_table[count].intptr = 0;
	  return (&ic->hash.hash_table[count]);
	}

      unsigned i, bucket = ic->hash.hash_fn (key, ic->context.data.ptr) % count;

      for (i = bucket; ;)
	{
	  if (0 == ic->hash.hash_table[i].intptr)
	    {
	      ++ic->items_count;
	      ic->hash.hash_table[i] = key;
	      return (&ic->hash.hash_table[i]);
	    }
	  if (0 == ic->compar_fn (key, ic->hash.hash_table[i], ic->context.data.ptr))
	    return (&ic->hash.hash_table[i]);
	  if (++i >= count)
	    i = 0;
	  if (i == bucket)
	    break;
	}
    }
  MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_HASH_TABLE_ERROR);
  return (NULL);
}

static mr_status_t
mr_ic_hash_index_visitor (mr_ptr_t key, void * context)
{
  mr_ic_t * dst_ic = context;
  mr_ptr_t * add = mr_ic_hash_index_add (dst_ic, key);
  return ((NULL == add) ? MR_FAILURE : MR_SUCCESS);
}

static mr_status_t
mr_ic_hash_reindex (mr_ic_t * dst_ic, mr_ic_t * src_ic)
{
  dst_ic->hash.zero_key = false;
  dst_ic->items_count = 0;

  if (0 != src_ic->items_count)
    {
      unsigned count = (src_ic->items_count << 2) + 6; /* 6 is a magic constant found in a manual performance test */

      if (dst_ic->hash.count < count)
	{
	  dst_ic->hash.count = count;
	  typeof (dst_ic->hash.hash_table) hash_table =
	    MR_REALLOC (dst_ic->hash.hash_table, dst_ic->hash.count * sizeof (dst_ic->hash.hash_table[0]));
	  if (NULL == hash_table)
	    {
	      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	      mr_ic_hash_free (dst_ic);
	      return (MR_FAILURE);
	    }
	  dst_ic->hash.hash_table = hash_table;
	}
    }

  if ((NULL != dst_ic->hash.hash_table) && (0 != dst_ic->hash.count))
    memset (dst_ic->hash.hash_table, 0, dst_ic->hash.count * sizeof (dst_ic->hash.hash_table[0]));
  return (mr_ic_foreach (src_ic, mr_ic_hash_index_visitor, dst_ic));
}

mr_status_t
mr_ic_hash_index (mr_ic_t * ic, mr_ptr_t * rarray, size_t size)
{
  mr_ic_t ic_unsorted_array;
  mr_status_t status = mr_ic_unsorted_array_new (&ic_unsorted_array, ic->compar_fn, ic->key_type, &ic->context);
  if (MR_SUCCESS != status)
    return (status);
  
  status = mr_ic_index (&ic_unsorted_array, rarray, size);
  if (MR_SUCCESS != status)
    return (status);
  
  status = mr_ic_hash_reindex (ic, &ic_unsorted_array);
  mr_ic_free (&ic_unsorted_array);

  return (status);
}

mr_ptr_t *
mr_ic_hash_add (mr_ic_t * ic, mr_ptr_t key)
{
  mr_ptr_t * find = mr_ic_find (ic, key);
  if (find != NULL)
    return (find);
  
  if (ic->items_count >= ic->hash.count / 2)
    {
      mr_ic_t dst_ic;
      if (MR_SUCCESS != mr_ic_hash_new (&dst_ic, ic->hash.hash_fn, ic->compar_fn, ic->key_type, &ic->context))
	return (NULL);
      ++ic->items_count;
      if (MR_SUCCESS != mr_ic_hash_reindex (&dst_ic, ic))
	return (NULL);

      mr_ic_free (ic);
      *ic = dst_ic;
    }
  return (mr_ic_hash_index_add (ic, key));
}

mr_status_t
mr_ic_hash_del (mr_ic_t * ic, mr_ptr_t key)
{
  mr_ptr_t * find = mr_ic_find (ic, key);
  if (NULL == find)
    return (MR_FAILURE);

  --ic->items_count;
  unsigned count = ic->hash.count - 1;
  unsigned bucket = find - ic->hash.hash_table;
  
  if (bucket == count)
    ic->hash.zero_key = false; /* release zero element */
  else
    {
      unsigned i = bucket;

      ic->hash.hash_table[i].intptr = 0;
      for (;;) /* need to re-index all elements in sequential blocks after deleted element */
	{
	  if (++i >= count)
	    i = 0;

	  if (0 == ic->hash.hash_table[i].intptr)
	    break;

	  if (i == bucket)
	    {
	      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_HASH_TABLE_ERROR);
	      return (MR_FAILURE);
	    }

	  mr_ptr_t key = ic->hash.hash_table[i];
	  ic->hash.hash_table[i].intptr = 0;
	  --ic->items_count;
	  
	  if (mr_ic_hash_index_add (ic, key) == NULL)
	    return (MR_FAILURE);
	}
    }

  return (MR_SUCCESS);
}

mr_ptr_t *
mr_ic_hash_find (mr_ic_t * ic, mr_ptr_t key)
{
  if (ic->hash.count < 2)
    return (NULL);
  
  unsigned count = ic->hash.count - 1;
  unsigned i, bucket = ic->hash.hash_fn (key, ic->context.data.ptr) % count;

  for (i = bucket; ;)
    {
      if (0 == ic->hash.hash_table[i].intptr)
	break;
      if (0 == ic->compar_fn (key, ic->hash.hash_table[i], ic->context.data.ptr))
	return (&ic->hash.hash_table[i]);
      if (++i >= count)
	i = 0;
      if (i == bucket)
	break;
    }
  
  if (ic->hash.zero_key &&
      (0 == ic->compar_fn (key, ic->hash.hash_table[count], ic->context.data.ptr)))
    return (&ic->hash.hash_table[count]);

  return (NULL);
}

mr_status_t
mr_ic_hash_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, void * context)
{
  if (ic->hash.count < 2)
    return (MR_SUCCESS);
  
  unsigned i, count = ic->hash.count - 1;
  
  if (ic->hash.zero_key)
    if (MR_SUCCESS != visit_fn (ic->hash.hash_table[count], context))
      return (MR_FAILURE);

  for (i = 0; i < count; ++i)
    if (0 != ic->hash.hash_table[i].intptr)
      if (MR_SUCCESS != visit_fn (ic->hash.hash_table[i], context))
	return (MR_FAILURE);
  return (MR_SUCCESS);
}

mr_status_t
mr_ic_hash_new (mr_ic_t * ic, mr_hash_fn_t hash_fn, mr_compar_fn_t compar_fn, char * key_type, mr_res_t * context)
{
  static mr_ic_virt_func_t virt_func = {
    .add = mr_ic_hash_add,
    .del = mr_ic_hash_del,
    .find = mr_ic_hash_find,
    .foreach = mr_ic_hash_foreach,
    .index = mr_ic_hash_index,
    .free = mr_ic_hash_free,
  };
  mr_res_t generic_context = {
    .type = "mr_td_t",
    .mr_size = sizeof (mr_td_t),
  };

  if ((compar_fn == NULL) && (context == NULL) && (NULL == hash_fn))
    {
      mr_td_t * tdp = mr_get_td_by_name_internal (key_type);
      if (tdp)
	{
	  generic_context.data.ptr = tdp;
	  hash_fn = mr_generic_hash;
	  compar_fn = mr_generic_cmp;
	  context = &generic_context;
	}
    }
  
  if ((NULL == ic) || (NULL == compar_fn) || (NULL == hash_fn))
    return (MR_FAILURE);

  if (context)
    ic->context = *context;
  else
    memset (&ic->context, 0, sizeof (ic->context));

  ic->ic_type = MR_IC_HASH;
  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->virt_func = &virt_func;
  ic->items_count = 0;

  memset (&ic->hash, 0, sizeof (ic->hash));
  ic->hash.hash_fn = hash_fn;

  return (MR_SUCCESS);
}

/* ----------------------- MR_IC_STATIC_ARRAY ----------------------- */

mr_ptr_t *
mr_ic_static_array_add (mr_ic_t * ic, mr_ptr_t key)
{
  mr_ptr_t * find = mr_ic_find (ic, key);
  if (NULL != find)
    return (find);

  if (ic->items_count > sizeof (ic->static_array) / sizeof (ic->static_array[0]))
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_NUMBER_OF_ITEMS, ic->items_count);
      ic->items_count = 0;
    }

  if (ic->items_count == sizeof (ic->static_array) / sizeof (ic->static_array[0]))
    {
      mr_ic_t dst_ic;
      mr_status_t status = mr_ic_sorted_array_new (&dst_ic, ic->compar_fn, ic->key_type, &ic->context);
      if (MR_SUCCESS != status)
	return (NULL);

      status = mr_ic_index (&dst_ic, ic->static_array, sizeof (ic->static_array));
      if (MR_SUCCESS != status)
	return (NULL);

      *ic = dst_ic;
      return (mr_ic_add (ic, key));
    }

  mr_ptr_t * add = &ic->static_array[ic->items_count++];
  *add = key;
  return (add);
}

mr_status_t
mr_ic_static_array_del (mr_ic_t * ic, mr_ptr_t key)
{
  mr_ptr_t * find = mr_ic_find (ic, key);
  if (NULL == find)
    return (MR_FAILURE);

  ptrdiff_t offset = (char*)find - (char*)&ic->static_array[0];
  --ic->items_count;
  memmove (find, &find[1], ic->items_count * sizeof (ic->static_array[0]) - offset);
  ic->static_array[ic->items_count].intptr = 0;
  return (MR_SUCCESS);
}

mr_ptr_t *
mr_ic_static_array_find (mr_ic_t * ic, mr_ptr_t key)
{
  int i;
  for (i = 0; i < ic->items_count; ++i)
    if (0 == ic->compar_fn (key, ic->static_array[i], ic->context.data.ptr))
      return (&ic->static_array[i]);
  return (NULL);
}

mr_status_t
mr_ic_static_array_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, void * context)
{
  unsigned i;
  for (i = 0; i < ic->items_count; ++i)
    if (MR_SUCCESS != visit_fn (ic->static_array[i], context))
      return (MR_FAILURE);
  return (MR_SUCCESS);
}

void
mr_ic_static_array_free (mr_ic_t * ic)
{
  ic->items_count = 0;
  memset (&ic->static_array, 0, sizeof (ic->static_array));
}

mr_status_t
mr_ic_static_array_index (mr_ic_t * ic, mr_ptr_t * rarray, size_t size)
{
  if (size > sizeof (ic->static_array))
    {
      mr_status_t status = mr_ic_sorted_array_new (ic, ic->compar_fn, ic->key_type, &ic->context);
      if (MR_SUCCESS != status)
	return (status);
      return (mr_ic_index (ic, rarray, size));
    }

  size /= sizeof (rarray[0]);
  ic->items_count = 0;
  
  int i;
  for (i = 0; i < size; ++i)
    mr_ic_add (ic, rarray[i]);

  return (MR_SUCCESS);
}

mr_status_t
mr_ic_static_array_new (mr_ic_t * ic, mr_hash_fn_t hash_fn, mr_compar_fn_t compar_fn, char * key_type, mr_res_t * context)
{
  static mr_ic_virt_func_t virt_func = {
    .add = mr_ic_static_array_add,
    .del = mr_ic_static_array_del,
    .find = mr_ic_static_array_find,
    .foreach = mr_ic_static_array_foreach,
    .index = mr_ic_static_array_index,
    .free = mr_ic_static_array_free,
  };
  mr_res_t generic_context = {
    .type = "mr_td_t",
    .mr_size = sizeof (mr_td_t),
  };

  if (NULL == ic)
    return (MR_FAILURE);

  if ((compar_fn == NULL) && (context == NULL))
    {
      mr_td_t * tdp = mr_get_td_by_name_internal (key_type);
      if (tdp)
	{
	  generic_context.data.ptr = tdp;
	  compar_fn = mr_generic_cmp;
	  context = &generic_context;
	}
    }
  
  if (NULL == compar_fn)
    return (MR_FAILURE);

  if (context)
    ic->context = *context;
  else
    memset (&ic->context, 0, sizeof (ic->context));

  ic->ic_type = MR_IC_STATIC_ARRAY;
  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->virt_func = &virt_func;
  ic->items_count = 0;

  memset (&ic->static_array, 0, sizeof (ic->static_array));

  return (MR_SUCCESS);
}

/* ----------------------- MR_IC_RBTREE ----------------------- */

mr_ptr_t *
mr_ic_rbtree_add (mr_ic_t * ic, mr_ptr_t key)
{
  mr_ptr_t * add = mr_rbtree_add (key, &ic->rb, ic->compar_fn, ic->context.data.ptr);
  ic->items_count = ic->rb.size / sizeof (ic->rb.pool[0]) - 1;
  return (add);
}

mr_status_t
mr_ic_rbtree_del (mr_ic_t * ic, mr_ptr_t key)
{
  mr_status_t status = mr_rbtree_del (key, &ic->rb, ic->compar_fn, ic->context.data.ptr);
  ic->items_count = ic->rb.size / sizeof (ic->rb.pool[0]) - 1;
  return (status);
}

mr_ptr_t *
mr_ic_tree_find (mr_ic_t * ic, mr_ptr_t key)
{
  mr_tree_traverse_t traverse;
  mr_tree_find (key, &ic->tree, ic->compar_fn, ic->context.data.ptr, &traverse);
  return (traverse.equal ?
	  &ic->tree.pool[traverse.path[traverse.count - 1].idx].key
	  : NULL);
}

mr_status_t
mr_ic_tree_foreach_unsorted (mr_ic_t * ic, mr_visit_fn_t visit_fn, void * context)
{
  unsigned i;
  if (ic->tree.size >= 2 * sizeof (ic->tree.pool[0]))
    for (i = ic->tree.size / sizeof (ic->tree.pool[0]) - 1; i > 0; --i)
      if (MR_SUCCESS != visit_fn (ic->tree.pool[i].key, context))
	return (MR_FAILURE);
  return (MR_SUCCESS);
}

mr_status_t
mr_ic_tree_foreach_sorted (mr_ic_t * ic, mr_visit_fn_t visit_fn, void * context)
{
  return (mr_tree_walk (&ic->tree, visit_fn, context));
}

void
mr_ic_tree_free (mr_ic_t * ic)
{
  ic->items_count = 0;
  mr_tree_free (&ic->tree);
}

mr_status_t
mr_ic_tree_index (mr_ic_t * ic, mr_ptr_t * rarray, size_t size)
{
  /* Add elements into the tree in the natuaral order. In case of equal elements only first will be added. */
  int i, count = size / sizeof (rarray[0]);
  mr_tree_reserve (&ic->tree, count, true);
  for (i = 0; i < count; ++i)
    if (NULL == mr_ic_add (ic, rarray[i]))
      return (MR_FAILURE);

  return (MR_SUCCESS);
}

mr_status_t
mr_ic_rbtree_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type, mr_res_t * context)
{
  static mr_ic_virt_func_t virt_func = {
    .add = mr_ic_rbtree_add,
    .del = mr_ic_rbtree_del,
    .find = mr_ic_tree_find,
    .foreach = mr_ic_tree_foreach_unsorted,
    .index = mr_ic_tree_index,
    .free = mr_ic_tree_free,
  };
  mr_res_t generic_context = {
    .type = "mr_td_t",
    .mr_size = sizeof (mr_td_t),
  };
  
  if (NULL == ic)
    return (MR_FAILURE);

  if ((compar_fn == NULL) && (context == NULL))
    {
      mr_td_t * tdp = mr_get_td_by_name_internal (key_type);
      if (tdp)
	{
	  generic_context.data.ptr = tdp;
	  compar_fn = mr_generic_cmp;
	  context = &generic_context;
	}
    }
  
  if (NULL == compar_fn)
    return (MR_FAILURE);

  if (context)
    ic->context = *context;
  else
    memset (&ic->context, 0, sizeof (ic->context));

  ic->ic_type = MR_IC_RBTREE;
  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->virt_func = &virt_func;
  ic->items_count = 0;

  mr_tree_init (&ic->rb);

  return (MR_SUCCESS);
}

/* ----------------------- MR_IC_AVLTREE ----------------------- */

mr_ptr_t *
mr_ic_avltree_add (mr_ic_t * ic, mr_ptr_t key)
{
  mr_ptr_t * add = mr_avltree_add (key, &ic->avl, ic->compar_fn, ic->context.data.ptr);
  ic->items_count = ic->avl.size / sizeof (ic->avl.pool[0]) - 1;
  return (add);
}

mr_status_t
mr_ic_avltree_del (mr_ic_t * ic, mr_ptr_t key)
{
  mr_status_t status = mr_avltree_del (key, &ic->avl, ic->compar_fn, ic->context.data.ptr);
  ic->items_count = ic->avl.size / sizeof (ic->avl.pool[0]) - 1;
  return (status);
}

mr_status_t
mr_ic_avltree_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type, mr_res_t * context)
{
  static mr_ic_virt_func_t virt_func = {
    .add = mr_ic_avltree_add,
    .del = mr_ic_avltree_del,
    .find = mr_ic_tree_find,
    .foreach = mr_ic_tree_foreach_unsorted,
    .index = mr_ic_tree_index,
    .free = mr_ic_tree_free,
  };
  mr_res_t generic_context = {
    .type = "mr_td_t",
    .mr_size = sizeof (mr_td_t),
  };
  
  if (NULL == ic)
    return (MR_FAILURE);

  if ((compar_fn == NULL) && (context == NULL))
    {
      mr_td_t * tdp = mr_get_td_by_name_internal (key_type);
      if (tdp)
	{
	  generic_context.data.ptr = tdp;
	  compar_fn = mr_generic_cmp;
	  context = &generic_context;
	}
    }
  
  if (NULL == compar_fn)
    return (MR_FAILURE);

  if (context)
    ic->context = *context;
  else
    memset (&ic->context, 0, sizeof (ic->context));

  ic->ic_type = MR_IC_AVLTREE;
  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->virt_func = &virt_func;
  ic->items_count = 0;

  mr_tree_init (&ic->avl);

  return (MR_SUCCESS);
}
