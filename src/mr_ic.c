/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <setjmp.h>
#include <string.h>
#include <sys/types.h>

#include <metaresc.h>
#include <mr_tsearch.h>
#include <mr_hsort.h>
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
  *ic = dst_ic;

  if (MR_SUCCESS != status)
    return (NULL);
      
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
  *ic = dst_ic;

  if (MR_SUCCESS != status)
    return (status);
      
  return (mr_ic_del (ic, key));
}

mr_ptr_t *
mr_ic_unsorted_array_find (mr_ic_t * ic, mr_ptr_t key)
{
  int i, count = ic->rarray.size / sizeof (ic->rarray.ra[0]);
  for (i = 0; i < count; ++i)
    if (0 == ic->compar_fn (key, ic->rarray.ra[i], ic->context.data.ptr))
      return (&ic->rarray.ra[i]);
  return (NULL);
}

mr_status_t
mr_ic_unsorted_array_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context)
{
  int i, count = ic->rarray.size / sizeof (ic->rarray.ra[0]);
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

/* ----------------------- MR_IC_RBTREE ----------------------- */

static void
dummy_free_fn (mr_ptr_t key, const void * context)
{
}

void
mr_ic_rbtree_free (mr_ic_t * ic)
{
  mr_tdestroy (ic->tree, dummy_free_fn, NULL);
  ic->tree = NULL;
  ic->items_count = 0;
}

mr_ptr_t *
mr_ic_rbtree_add (mr_ic_t * ic, mr_ptr_t key)
{
  mr_ptr_t * add = mr_tsearch (key, &ic->tree, ic->compar_fn, ic->context.data.ptr);
  if (NULL == add)
    MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
  else
    ++ic->items_count;
  return (add);
}

mr_status_t
mr_ic_rbtree_del (mr_ic_t * ic, mr_ptr_t key)
{
  mr_ptr_t * find = mr_ic_find (ic, key);
  if (find == NULL)
    return (MR_FAILURE);
  mr_tdelete (key, &ic->tree, ic->compar_fn, ic->context.data.ptr);
  --ic->items_count;
  return (MR_SUCCESS);
}

mr_ptr_t *
mr_ic_rbtree_find (mr_ic_t * ic, mr_ptr_t key)
{
  return (mr_tfind (key, &ic->tree, ic->compar_fn, ic->context.data.ptr));
}

TYPEDEF_STRUCT (mr_ic_rbtree_foreach_context_t,
		(mr_visit_fn_t, visit_fn),
		(const void *, context),
		(jmp_buf, jmp_buf))

static void
visit_node (const mr_red_black_tree_node_t * node, mr_rb_visit_order_t order, int level, const void * context)
{
  mr_ic_rbtree_foreach_context_t * mr_ic_rbtree_foreach_context = (void*)context;
  if ((MR_RB_VISIT_POSTORDER == order) || (MR_RB_VISIT_LEAF == order))
    if (MR_SUCCESS != mr_ic_rbtree_foreach_context->visit_fn (node->key, mr_ic_rbtree_foreach_context->context))
      longjmp (mr_ic_rbtree_foreach_context->jmp_buf, !0);
}

mr_status_t
mr_ic_rbtree_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context)
{
  mr_ic_rbtree_foreach_context_t mr_ic_rbtree_foreach_context = {
    .visit_fn = visit_fn,
    .context = context,
  };

  if (NULL == visit_fn)
    return (MR_FAILURE);

  if (0 != setjmp (mr_ic_rbtree_foreach_context.jmp_buf))
    return (MR_FAILURE);

  mr_twalk (ic->tree, visit_node, &mr_ic_rbtree_foreach_context);
  return (MR_SUCCESS);
}

mr_status_t
mr_ic_rbtree_index (mr_ic_t * ic, mr_ic_rarray_t * rarray)
{
  int i;
  mr_ic_rbtree_free (ic);
  for (i = rarray->size / sizeof (rarray->ra[0]) - 1; i >= 0; --i)
    if (NULL == mr_ic_rbtree_add (ic, rarray->ra[i]))
      return (MR_FAILURE);
  return (MR_SUCCESS);
}

mr_status_t
mr_ic_rbtree_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type, mr_res_t * context)
{
  static mr_ic_virt_func_t virt_func = {
    .add = mr_ic_rbtree_add,
    .del = mr_ic_rbtree_del,
    .find = mr_ic_rbtree_find,
    .foreach = mr_ic_rbtree_foreach,
    .index = mr_ic_rbtree_index,
    .free = mr_ic_rbtree_free,
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
  ic->tree = NULL;
  ic->items_count = 0;

  return (MR_SUCCESS);
}

/* ----------------------- MR_IC_SORTED_ARRAY ----------------------- */

static int
mr_ic_sorted_array_find_idx (mr_ic_t * ic, mr_ptr_t key, int * diff)
{
  int down = 0;
  int up = ic->rarray.size / sizeof (ic->rarray.ra[0]);

  *diff = !0;

  while (up != down)
    {
      int mid = (down + up) >> 1;
      *diff = ic->compar_fn (key, ic->rarray.ra[mid], ic->context.data.ptr);
      if (0 == *diff)
	return (mid);
      if (*diff < 0)
	up = mid;
      else
	down = mid + 1;
    }
  return (down);
}

mr_ptr_t *
mr_ic_sorted_array_add (mr_ic_t * ic, mr_ptr_t key)
{
  mr_ptr_t * add;
  int diff = !0;
  int idx = mr_ic_sorted_array_find_idx (ic, key, &diff);
  if (0 == diff)
    return (&ic->rarray.ra[idx]);

  add = mr_rarray_allocate_element ((void*)&ic->rarray.ra, &ic->rarray.size, &ic->rarray.alloc_size, sizeof (ic->rarray.ra[0]));
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
  mr_ptr_t * find = mr_ic_find (ic, key);
  if (NULL == find)
    return (MR_FAILURE);
   
  ptrdiff_t offset = (char*)find - (char*)ic->rarray.ra;
  memmove (find, &find[1], ic->rarray.size - offset);
  ic->rarray.size -= sizeof (ic->rarray.ra[0]);
  ic->items_count = ic->rarray.size / sizeof (ic->rarray.ra[0]);
  return (MR_SUCCESS);
}

mr_ptr_t *
mr_ic_sorted_array_find (mr_ic_t * ic, mr_ptr_t key)
{
  int diff = !0;
  int idx = mr_ic_sorted_array_find_idx (ic, key, &diff);
  if (0 == diff)
    return (&ic->rarray.ra[idx]);
  else
    return (NULL);
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
  mr_ic_sorted_array_free (ic);

  if (0 == rarray->size)
    return (MR_SUCCESS);
  
  ic->rarray.ra = MR_MALLOC (rarray->size);
  if (NULL == ic->rarray.ra)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (MR_FAILURE);
    }

  ic->rarray.size = ic->rarray.alloc_size = rarray->size;
  memcpy (ic->rarray.ra, rarray->ra, rarray->size);
  hsort (ic->rarray.ra, ic->rarray.size / sizeof (ic->rarray.ra[0]), sizeof (ic->rarray.ra[0]), mr_sort_key_cmp, ic);
  ic->items_count = ic->rarray.size / sizeof (ic->rarray.ra[0]);

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

static mr_ptr_t zero = { .long_int_t = 0, };

#define MR_HASH_TABLE_SIZE_MULT (1.61803398875)

void
mr_ic_hash_next_free (mr_ic_t * ic)
{
  if (NULL != ic->hash_next.hash_table)
    MR_FREE (ic->hash_next.hash_table);
  ic->hash_next.hash_table = NULL;
  ic->hash_next.size = 0;
  ic->hash_next.zero_key = false;
  ic->items_count = 0;
}

static inline int
mr_ic_hash_get_backet (mr_ic_t * ic, mr_ptr_t key)
{
  mr_hash_value_t hash_value = ic->hash_next.hash_fn (key, ic->context.data.ptr);
  int hash_size = ic->hash_next.size / sizeof (ic->hash_next.hash_table[0]);
  if (hash_size <= 0)
    return (-1);
  return (hash_value % hash_size);
}

static mr_ptr_t *
mr_ic_hash_next_index_add (mr_ic_t * ic, mr_ptr_t key, int bucket)
{
  int i, count = ic->hash_next.size / sizeof (ic->hash_next.hash_table[0]);

  if (bucket < 0)
    return (NULL);
  
  if (0 == key.long_int_t)
    {
      ic->hash_next.zero_key = true;
      return (&zero);
    }

  for (i = bucket; ;)
    {
      if (0 == ic->hash_next.hash_table[i].long_int_t)
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
  MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_HASH_TABLE_ERROR);
  return (NULL);
}

static mr_ptr_t *
mr_ic_hash_add_inner (mr_ic_t * ic, mr_ptr_t key)
{
  int bucket = mr_ic_hash_get_backet (ic, key);
  if (bucket < 0)
    return (NULL);

  mr_ptr_t * add = mr_ic_hash_next_index_add (ic, key, bucket);
  if ((add != NULL) && (add->ptr == key.ptr))
    ++ic->items_count;

  return (add);
}

static mr_status_t
mr_ic_hash_index_visitor (mr_ptr_t key, const void * context)
{
  mr_ic_t * dst_ic = (void*)context;
  mr_ptr_t * add = mr_ic_hash_add_inner (dst_ic, key);
  return ((NULL == add) ? MR_FAILURE : MR_SUCCESS);
}

static mr_status_t
mr_ic_hash_index_inner (mr_ic_t * src_ic, mr_ic_t * dst_ic, int items_count)
{
  
  dst_ic->items_count = items_count;

  if (0 == dst_ic->items_count)
    return (MR_SUCCESS);

  dst_ic->hash_next.size = ((int)(dst_ic->items_count * 2 * MR_HASH_TABLE_SIZE_MULT)) * sizeof (dst_ic->hash_next.hash_table[0]);
  dst_ic->hash_next.hash_table = MR_MALLOC (dst_ic->hash_next.size);
  if (NULL == dst_ic->hash_next.hash_table)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (MR_FAILURE);
    }

  memset (dst_ic->hash_next.hash_table, 0, dst_ic->hash_next.size);

  dst_ic->items_count = 0;
  return (mr_ic_foreach (src_ic, mr_ic_hash_index_visitor, dst_ic));
}

mr_status_t
mr_ic_hash_next_index (mr_ic_t * ic, mr_ic_rarray_t * rarray)
{
  mr_status_t status;
  mr_ic_t ic_unsorted_array;

  if (NULL == rarray)
    return (MR_FAILURE);

  mr_ic_unsorted_array_new (&ic_unsorted_array, ic->compar_fn, ic->key_type, &ic->context);
  mr_ic_unsorted_array_index (&ic_unsorted_array, rarray);
  status = mr_ic_hash_index_inner (&ic_unsorted_array, ic, rarray->size / sizeof (rarray->ra[0]));
  mr_ic_free (&ic_unsorted_array);
  return (status);
}

mr_ptr_t *
mr_ic_hash_next_add (mr_ic_t * ic, mr_ptr_t key)
{
  mr_ptr_t * find = mr_ic_find (ic, key);
  if (find != NULL)
    return (find);

  if (ic->hash_next.size / sizeof (ic->hash_next.hash_table[0]) <= (ic->items_count + 1) * MR_HASH_TABLE_SIZE_MULT)
    {
      mr_ic_t dst_ic;
      if (MR_SUCCESS != mr_ic_new (&dst_ic, ic->hash_next.hash_fn, ic->compar_fn, ic->key_type, ic->ic_type, &ic->context))
	return (NULL);

      if (MR_SUCCESS != mr_ic_hash_index_inner (ic, &dst_ic, ic->items_count + 1))
	return (NULL);

      mr_ic_free (ic);
      *ic = dst_ic;
    }
  return (mr_ic_hash_add_inner (ic, key));
}

mr_status_t
mr_ic_hash_next_del (mr_ic_t * ic, mr_ptr_t key)
{
  mr_ptr_t * find = mr_ic_find (ic, key);

  if (NULL == find)
    return (MR_FAILURE);

  --ic->items_count;
  if (0 == key.long_int_t)
    ic->hash_next.zero_key = false;
  else
    {
      int i, count = ic->hash_next.size / sizeof (ic->hash_next.hash_table[0]);
      
      find->long_int_t = 0;
      for (i = find - ic->hash_next.hash_table; ;) /* need to re-index all elements in sequential block after deleted element */
	{
	  mr_ptr_t mr_ptr;
	  if (++i >= count)
	    i = 0;
	  if (0 == ic->hash_next.hash_table[i].long_int_t)
	    break;
	  mr_ptr = ic->hash_next.hash_table[i];
	  ic->hash_next.hash_table[i].long_int_t = 0;
	  int bucket = mr_ic_hash_get_backet (ic, mr_ptr);
	  mr_ic_hash_next_index_add (ic, mr_ptr, bucket);
	}
    }
  
  return (MR_SUCCESS);
}

mr_ptr_t *
mr_ic_hash_next_find (mr_ic_t * ic, mr_ptr_t key)
{
  int i, bucket, count = ic->hash_next.size / sizeof (ic->hash_next.hash_table[0]);
  
  if (0 == key.long_int_t)
    return (ic->hash_next.zero_key ? &zero : NULL);

  bucket = mr_ic_hash_get_backet (ic, key);
  if (bucket < 0)
    return (NULL);

  for (i = bucket; ;)
    {
      if (0 == ic->hash_next.hash_table[i].long_int_t)
	return (NULL);
      if (0 == ic->compar_fn (key, ic->hash_next.hash_table[i], ic->context.data.ptr))
	return (&ic->hash_next.hash_table[i]);
      if (++i >= count)
	i = 0;
      if (i == bucket)
	break;
    }
  return (NULL);
}

mr_status_t
mr_ic_hash_next_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context)
{
  int i;

  if (ic->hash_next.zero_key)
    if (MR_SUCCESS != visit_fn (zero, context))
      return (MR_FAILURE);

  for (i = ic->hash_next.size / sizeof (ic->hash_next.hash_table[0]) - 1; i >= 0; --i)
    if (0 != ic->hash_next.hash_table[i].long_int_t)
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
  ic->hash_next.hash_table = NULL;
  ic->hash_next.size = 0;
  ic->hash_next.zero_key = false;

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
      mr_status_t status = mr_ic_hash_next_new (&dst_ic, ic->static_array.hash_fn, ic->compar_fn, ic->key_type, &ic->context);
      if (MR_SUCCESS != status)
	return (NULL);
      
      mr_ic_rarray_t rarray;
      rarray.ra = &ic->static_array.static_array[0];
      rarray.size = sizeof (ic->static_array.static_array);
      rarray.alloc_size = -1;
      
      status = mr_ic_index (&dst_ic, &rarray);
      *ic = dst_ic;

      if (MR_SUCCESS != status)
	return (NULL);
      
      return (mr_ic_add (ic, key));
    }

  mr_ptr_t * add = &ic->static_array.static_array[ic->items_count++];
  *add = key;
  return (add);
}

mr_status_t
mr_ic_static_array_del (mr_ic_t * ic, mr_ptr_t key)
{
  ptrdiff_t offset;
  mr_ptr_t * find = mr_ic_find (ic, key);
  if (NULL == find)
    return (MR_FAILURE);
  
  offset = (char*)find - (char*)&ic->static_array.static_array[0];
  
  --ic->items_count;
  memmove (find, &find[1], ic->items_count * sizeof (ic->static_array.static_array[0]) - offset);
  ic->static_array.static_array[ic->items_count].long_int_t = 0;
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
  int i;
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
  
  mr_ic_static_array_free (ic);
  
  if (rarray->size > sizeof (ic->static_array.static_array))
    {
      mr_status_t status = mr_ic_hash_next_new (ic, ic->static_array.hash_fn, ic->compar_fn, ic->key_type, &ic->context);
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
  ic->static_array.hash_fn = hash_fn;

  return (MR_SUCCESS);
}
