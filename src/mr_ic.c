/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <string.h>

#include <metaresc.h>
#include <mr_hsort.h>
#include <mr_btree.h>
#include <mr_ic.h>

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
      
  status = mr_ic_index (&dst_ic, &ic->rarray);
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
      
  status = mr_ic_index (&dst_ic, &ic->rarray);
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
mr_ic_unsorted_array_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context)
{
  unsigned i, count = ic->rarray.size / sizeof (ic->rarray.ra[0]);
  for (i = 0; i < count; ++i)
    if (MR_SUCCESS != visit_fn (ic->rarray.ra[i], context))
      return (MR_FAILURE);
  return (MR_SUCCESS);
}

mr_status_t
mr_ic_unsorted_array_index (mr_ic_t * ic, mr_ic_rarray_t * rarray)
{
  ic->rarray = *rarray;
  ic->items_count = rarray->size / sizeof (rarray->ra[0]);
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
  
  if (NULL == ic)
    return (MR_FAILURE);

  ic->ic_type = MR_IC_UNSORTED_ARRAY;
  if (context)
    ic->context = *context;
  else
    memset (&ic->context, 0, sizeof (ic->context));
  
  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->virt_func = &virt_func;
  ic->rarray.ra = NULL;
  ic->rarray.size = ic->rarray.alloc_size = 0;
  ic->items_count = 0;

  return (MR_SUCCESS);
}

/* ----------------------- MR_IC_SORTED_ARRAY ----------------------- */

static inline int
mr_ic_sorted_array_find_idx (mr_ic_t * ic, mr_ptr_t key, unsigned * idx)
{
  unsigned down = 0;
  unsigned up = ic->rarray.size / sizeof (ic->rarray.ra[0]);
  int diff = !0;

  while ((up != down) && (diff != 0))
    {
      unsigned mid = (down + up) >> 1;
      diff = ic->compar_fn (key, ic->rarray.ra[mid], ic->context.data.ptr);
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
  if (mr_ic_sorted_array_find_idx (ic, key, &idx) == 0)
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
  if (mr_ic_sorted_array_find_idx (ic, key, &idx) != 0)
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
  return ((mr_ic_sorted_array_find_idx (ic, key, &idx) == 0) ? &ic->rarray.ra[idx] : NULL);
}

static int
mr_sort_key_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const mr_ic_t * ic = context;
  const mr_ptr_t * x_ = x.ptr;
  const mr_ptr_t * y_ = y.ptr;
  return (ic->compar_fn (*x_, *y_, ic->context.data.ptr));
}

mr_status_t
mr_ic_sorted_array_index (mr_ic_t * ic, mr_ic_rarray_t * rarray)
{
  unsigned items_count = rarray->size / sizeof (rarray->ra[0]);
  if (items_count > 0)
    {
      ic->rarray.ra = MR_CALLOC (items_count, sizeof (rarray->ra[0]));
      if (NULL == ic->rarray.ra)
	{
	  MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	  return (MR_FAILURE);
	}

      memcpy (ic->rarray.ra, rarray->ra, items_count * sizeof (rarray->ra[0]));
      mr_hsort (ic->rarray.ra, items_count, sizeof (ic->rarray.ra[0]), mr_sort_key_cmp, ic);
      
      unsigned src, dst = 0;
      ic->rarray.ra[dst++] = ic->rarray.ra[0];
      for (src = 1; src < items_count; ++src)
	if (ic->compar_fn (ic->rarray.ra[src], ic->rarray.ra[src - 1], ic->context.data.ptr) != 0)
	  ic->rarray.ra[dst++] = ic->rarray.ra[src];
      
      ic->items_count = dst;
      ic->rarray.size = ic->rarray.alloc_size = ic->items_count * sizeof (ic->rarray.ra[0]);
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
  
  if ((NULL == ic) || (NULL == compar_fn))
    return (MR_FAILURE);

  ic->rarray.ra = NULL;
  ic->rarray.size = ic->rarray.alloc_size = 0;

  ic->ic_type = MR_IC_SORTED_ARRAY;
  if (context)
    ic->context = *context;
  else
    memset (&ic->context, 0, sizeof (ic->context));
  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->virt_func = &virt_func;
  ic->items_count = 0;
  
  return (MR_SUCCESS);
}

/* ----------------------- MR_IC_HASH_NEXT ----------------------- */

#define MR_HASH_TABLE_SIZE_MULT (1.61803398875) /* golden ratio */
#define MR_HASH_MULT_FIXEDPOINT ((sizeof (int) - 1) * __CHAR_BIT__)

void
mr_ic_hash_next_free (mr_ic_t * ic)
{
  if (NULL != ic->hash_next.hash_table)
    MR_FREE (ic->hash_next.hash_table);
  ic->hash_next.hash_table = NULL;
  ic->hash_next.size = 0;
  ic->hash_next.resize_count = 0;
  ic->hash_next.zero_key = false;
  ic->items_count = 0;
}

static mr_ptr_t *
mr_ic_hash_next_index_add (mr_ic_t * ic, mr_ptr_t key)
{
  if (ic->hash_next.size >= 2 * sizeof (ic->hash_next.hash_table[0]))
    {
      unsigned count = ic->hash_next.size / sizeof (ic->hash_next.hash_table[0]) - 1;
      
      if (0 == key.intptr_t)
	{
	  ic->hash_next.zero_key = true;
	  ic->hash_next.hash_table[count].intptr_t = 0;
	  return (&ic->hash_next.hash_table[count]);
	}

      unsigned i, bucket = ic->hash_next.hash_fn (key, ic->context.data.ptr) % count;

      for (i = bucket; ;)
	{
	  if (0 == ic->hash_next.hash_table[i].intptr_t)
	    {
	      ic->hash_next.hash_table[i] = key;
	      return (&ic->hash_next.hash_table[i]);
	    }
	  if (0 == ic->compar_fn (key, ic->hash_next.hash_table[i], ic->context.data.ptr))
	    return (&ic->hash_next.hash_table[i]);
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
mr_ic_hash_index_visitor (mr_ptr_t key, const void * context)
{
  mr_ic_t * dst_ic = (void*)context;
  mr_ptr_t * add = mr_ic_hash_next_index_add (dst_ic, key);
  return ((NULL == add) ? MR_FAILURE : MR_SUCCESS);
}

static mr_status_t
mr_ic_hash_reindex (mr_ic_t * src_ic, mr_ic_t * dst_ic, unsigned items_count)
{
  if (MR_IC_HASH_NEXT != dst_ic->ic_type)
    return (EXIT_FAILURE);
  
  mr_ic_hash_next_free (dst_ic);

  if (0 == items_count)
    return (MR_SUCCESS);

  unsigned count = 1 + (((long long)((2LL << MR_HASH_MULT_FIXEDPOINT) * MR_HASH_TABLE_SIZE_MULT) * items_count) >> MR_HASH_MULT_FIXEDPOINT);
  dst_ic->hash_next.hash_table = MR_CALLOC (count, sizeof (dst_ic->hash_next.hash_table[0]));
  if (NULL == dst_ic->hash_next.hash_table)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (MR_FAILURE);
    }
  dst_ic->hash_next.resize_count = count >> 1;
  dst_ic->hash_next.size = count * sizeof (dst_ic->hash_next.hash_table[0]);
  dst_ic->items_count = items_count;
  return (mr_ic_foreach (src_ic, mr_ic_hash_index_visitor, dst_ic));
}

static mr_status_t
mr_ic_hash_count_visitor (mr_ptr_t key, const void * context)
{
  mr_ic_t * ic = (void*)context;
  ++ic->items_count;
  return (MR_SUCCESS);
}

mr_status_t
mr_ic_hash_next_index (mr_ic_t * ic, mr_ic_rarray_t * rarray)
{
  mr_ic_t ic_unsorted_array;
  mr_status_t status = mr_ic_unsorted_array_new (&ic_unsorted_array, ic->compar_fn, ic->key_type, &ic->context);
  if (MR_SUCCESS != status)
    return (status);
  
  status = mr_ic_index (&ic_unsorted_array, rarray);
  if (MR_SUCCESS != status)
    return (status);
  
  status = mr_ic_hash_reindex (&ic_unsorted_array, ic, rarray->size / sizeof (rarray->ra[0]));
  mr_ic_free (&ic_unsorted_array);

  /* there might be duplicates in input data, so we need to count number of unique elemenets */
  ic->items_count = 0;
  mr_ic_foreach (ic, mr_ic_hash_count_visitor, ic);
  
  return (status);
}

mr_ptr_t *
mr_ic_hash_next_add (mr_ic_t * ic, mr_ptr_t key)
{
  mr_ptr_t * find = mr_ic_find (ic, key);
  if (find != NULL)
    return (find);

  if (++ic->items_count >= ic->hash_next.resize_count)
    {
      mr_ic_t dst_ic;
      if (MR_SUCCESS != mr_ic_new (&dst_ic, ic->hash_next.hash_fn, ic->compar_fn, ic->key_type, ic->ic_type, &ic->context))
	return (NULL);

      if (MR_SUCCESS != mr_ic_hash_reindex (ic, &dst_ic, ic->items_count))
	return (NULL);

      mr_ic_free (ic);
      *ic = dst_ic;
    }
  return (mr_ic_hash_next_index_add (ic, key));
}

mr_status_t
mr_ic_hash_next_del (mr_ic_t * ic, mr_ptr_t key)
{
  mr_ptr_t * find = mr_ic_find (ic, key);
  if (NULL == find)
    return (MR_FAILURE);

  --ic->items_count;
  unsigned count = ic->hash_next.size / sizeof (ic->hash_next.hash_table[0]) - 1;
  
  if (0 == key.intptr_t)
    ic->hash_next.zero_key = false; /* release zero element */
  else
    {
      unsigned i, start_bucket = find - ic->hash_next.hash_table;

      find->intptr_t = 0;
      for (i = start_bucket; ;) /* need to re-index all elements in sequential blocks after deleted element */
	{
	  if (++i >= count)
	    i = 0;

	  if (0 == ic->hash_next.hash_table[i].intptr_t)
	    break;

	  if (i == start_bucket)
	    {
	      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_HASH_TABLE_ERROR);
	      return (MR_FAILURE);
	    }

	  mr_ptr_t key = ic->hash_next.hash_table[i];
	  ic->hash_next.hash_table[i].intptr_t = 0;
	  
	  if (mr_ic_hash_next_index_add (ic, key) == NULL)
	    return (MR_FAILURE);
	}
    }

  return (MR_SUCCESS);
}

mr_ptr_t *
mr_ic_hash_next_find (mr_ic_t * ic, mr_ptr_t key)
{
  if (ic->hash_next.size < 2 * sizeof (ic->hash_next.hash_table[0]))
    return (NULL);
  
  unsigned count = ic->hash_next.size / sizeof (ic->hash_next.hash_table[0]) - 1;
  unsigned i, bucket = ic->hash_next.hash_fn (key, ic->context.data.ptr) % count;

  for (i = bucket; ;)
    {
      if (0 == ic->hash_next.hash_table[i].intptr_t)
	break;
      if (0 == ic->compar_fn (key, ic->hash_next.hash_table[i], ic->context.data.ptr))
	return (&ic->hash_next.hash_table[i]);
      if (++i >= count)
	i = 0;
      if (i == bucket)
	break;
    }
  
  if (ic->hash_next.zero_key &&
      (0 == ic->compar_fn (key, ic->hash_next.hash_table[count], ic->context.data.ptr)))
    return (&ic->hash_next.hash_table[count]);

  return (NULL);
}

mr_status_t
mr_ic_hash_next_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context)
{
  if (ic->hash_next.size < 2 * sizeof (ic->hash_next.hash_table[0]))
    return (MR_SUCCESS);
  
  unsigned i, count = ic->hash_next.size / sizeof (ic->hash_next.hash_table[0]) - 1;
  
  if (ic->hash_next.zero_key)
    if (MR_SUCCESS != visit_fn (ic->hash_next.hash_table[count], context))
      return (MR_FAILURE);

  for (i = 0; i < count; ++i)
    if (0 != ic->hash_next.hash_table[i].intptr_t)
      if (MR_SUCCESS != visit_fn (ic->hash_next.hash_table[i], context))
	return (MR_FAILURE);
  return (MR_SUCCESS);
}

mr_status_t
mr_ic_hash_next_new (mr_ic_t * ic, mr_hash_fn_t hash_fn, mr_compar_fn_t compar_fn, char * key_type, mr_res_t * context)
{
  static mr_ic_virt_func_t virt_func = {
    .add = mr_ic_hash_next_add,
    .del = mr_ic_hash_next_del,
    .find = mr_ic_hash_next_find,
    .foreach = mr_ic_hash_next_foreach,
    .index = mr_ic_hash_next_index,
    .free = mr_ic_hash_next_free,
  };

  if ((NULL == ic) || (NULL == compar_fn) || (NULL == hash_fn))
    return (MR_FAILURE);

  ic->ic_type = MR_IC_HASH_NEXT;
  if (context)
    ic->context = *context;
  else
    memset (&ic->context, 0, sizeof (ic->context));

  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->virt_func = &virt_func;
  ic->items_count = 0;

  memset (&ic->hash_next, 0, sizeof (ic->hash_next));
  ic->hash_next.hash_fn = hash_fn;

  return (MR_SUCCESS);
}

/* ----------------------- MR_IC_STATIC_ARRAY ----------------------- */

mr_ptr_t *
mr_ic_static_array_add (mr_ic_t * ic, mr_ptr_t key)
{
  mr_ptr_t * find = mr_ic_find (ic, key);
  if (NULL != find)
    return (find);

  if (ic->items_count > sizeof (ic->static_array.static_array) / sizeof (ic->static_array.static_array[0]))
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_NUMBER_OF_ITEMS, ic->items_count);
      ic->items_count = 0;
    }

  if (ic->items_count == sizeof (ic->static_array.static_array) / sizeof (ic->static_array.static_array[0]))
    {
      mr_ic_t dst_ic;
      mr_status_t status = mr_ic_sorted_array_new (&dst_ic, ic->compar_fn, ic->key_type, &ic->context);
      if (MR_SUCCESS != status)
	return (NULL);

      mr_ic_rarray_t rarray;
      rarray.ra = &ic->static_array.static_array[0];
      rarray.size = sizeof (ic->static_array.static_array);
      rarray.alloc_size = -1;

      status = mr_ic_index (&dst_ic, &rarray);
      if (MR_SUCCESS != status)
	return (NULL);

      *ic = dst_ic;
      return (mr_ic_add (ic, key));
    }

  mr_ptr_t * add = &ic->static_array.static_array[ic->items_count++];
  *add = key;
  return (add);
}

mr_status_t
mr_ic_static_array_del (mr_ic_t * ic, mr_ptr_t key)
{
  mr_ptr_t * find = mr_ic_find (ic, key);
  if (NULL == find)
    return (MR_FAILURE);

  ptrdiff_t offset = (char*)find - (char*)&ic->static_array.static_array[0];
  --ic->items_count;
  memmove (find, &find[1], ic->items_count * sizeof (ic->static_array.static_array[0]) - offset);
  ic->static_array.static_array[ic->items_count].intptr_t = 0;
  return (MR_SUCCESS);
}

mr_ptr_t *
mr_ic_static_array_find (mr_ic_t * ic, mr_ptr_t key)
{
  int i;
  for (i = 0; i < ic->items_count; ++i)
    if (0 == ic->compar_fn (key, ic->static_array.static_array[i], ic->context.data.ptr))
      return (&ic->static_array.static_array[i]);
  return (NULL);
}

mr_status_t
mr_ic_static_array_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context)
{
  unsigned i;
  for (i = 0; i < ic->items_count; ++i)
    if (MR_SUCCESS != visit_fn (ic->static_array.static_array[i], context))
      return (MR_FAILURE);
  return (MR_SUCCESS);
}

void
mr_ic_static_array_free (mr_ic_t * ic)
{
  ic->items_count = 0;
  memset (&ic->static_array.static_array, 0, sizeof (ic->static_array.static_array));
}

mr_status_t mr_ic_static_array_index (mr_ic_t * ic, mr_ic_rarray_t * rarray)
{
  int i;
  
  if (rarray->size > sizeof (ic->static_array.static_array))
    {
      mr_status_t status = mr_ic_unsorted_array_new (ic, ic->compar_fn, ic->key_type, &ic->context);
      if (MR_SUCCESS != status)
	return (status);
      return (mr_ic_index (ic, rarray));
    }
  
  for (i = rarray->size / sizeof (rarray->ra[0]) - 1; i >= 0; --i)
    mr_ic_add (ic, rarray->ra[i]);

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
  
  if ((NULL == ic) || (NULL == compar_fn) || (NULL == hash_fn))
    return (MR_FAILURE);

  ic->ic_type = MR_IC_STATIC_ARRAY;
  if (context)
    ic->context = *context;
  else
    memset (&ic->context, 0, sizeof (ic->context));

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
  return (traverse.equal ? &ic->tree.pool[traverse.path[traverse.size - 1].idx].key : NULL);
}

mr_status_t
mr_ic_tree_foreach_unsorted (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context)
{
  unsigned i;
  if (ic->tree.size >= 2 * sizeof (ic->tree.pool[0]))
    for (i = ic->tree.size / sizeof (ic->tree.pool[0]) - 1; i > 0; --i)
      if (MR_SUCCESS != visit_fn (ic->tree.pool[i].key, context))
	return (MR_FAILURE);
  return (MR_SUCCESS);
}

mr_status_t
mr_ic_tree_foreach_sorted (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context)
{
  return (mr_tree_walk (&ic->tree, visit_fn, context));
}

void
mr_ic_tree_free (mr_ic_t * ic)
{
  ic->items_count = 0;
  mr_tree_free (&ic->tree);
}

mr_status_t mr_ic_tree_index (mr_ic_t * ic, mr_ic_rarray_t * rarray)
{
  int i;
  for (i = rarray->size / sizeof (rarray->ra[0]) - 1; i >= 0; --i)
    if (NULL == mr_ic_add (ic, rarray->ra[i]))
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
  
  if ((NULL == ic) || (NULL == compar_fn))
    return (MR_FAILURE);

  ic->ic_type = MR_IC_RBTREE;
  if (context)
    ic->context = *context;
  else
    memset (&ic->context, 0, sizeof (ic->context));

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
  
  if ((NULL == ic) || (NULL == compar_fn))
    return (MR_FAILURE);

  ic->ic_type = MR_IC_AVLTREE;
  if (context)
    ic->context = *context;
  else
    memset (&ic->context, 0, sizeof (ic->context));

  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->virt_func = &virt_func;
  ic->items_count = 0;

  mr_tree_init (&ic->avl);

  return (MR_SUCCESS);
}
