/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <setjmp.h>
#include <string.h>

#include <metaresc.h>
#include <mr_tsearch.h>
#include <mr_hsort.h>
#include <mr_ic.h>

mr_ptr_t *
mr_ic_none_add (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ic_rarray_t * rarray = ic->ext.ptr;
  mr_ptr_t * add;

  if ((NULL == rarray) || (NULL == rarray->ra.ptr_type) ||
      (0 != strcmp (MR_IC_NONE_TYPE_T_STR, rarray->ra.ptr_type)))
    {
      mr_ic_rarray_t * rarray_ = MR_MALLOC (sizeof (*rarray));
      if (NULL == rarray_)
	{
	  MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	  return (NULL);
	}

      if (NULL == rarray)
	rarray_->ra.size = 0;
      else
	rarray_->ra.size = rarray->ra.size;

      rarray_->ra.alloc_size = rarray_->ra.size + sizeof (rarray_->ra.data[0]);
      rarray_->ra.data = MR_MALLOC (rarray_->ra.alloc_size);
      if (NULL == rarray_->ra.data)
	{
	  MR_FREE (rarray_);
	  MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	  return (NULL);
	}

      if (NULL != rarray)
	memcpy (rarray_->ra.data, rarray->ra.data, rarray_->ra.size);

      rarray_->ra.ext.ptr = NULL;
      rarray_->ra.ptr_type = MR_IC_NONE_TYPE_T_STR;
      ic->ext.ptr = rarray = rarray_;
    }

  add = mr_rarray_append ((mr_rarray_t*)(void*)&rarray->ra, sizeof (rarray->ra.data[0]));

  if (NULL == add)
    return (NULL);

  *add = key;
  return (add);
}

mr_ptr_t *
mr_ic_none_find (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ic_rarray_t * rarray = ic->ext.ptr;
  int i, count;

  if (NULL == rarray)
    return (NULL);

  count = rarray->ra.size / sizeof (rarray->ra.data[0]);
  for (i = 0; i < count; ++i)
    if (0 == ic->compar_fn (key, rarray->ra.data[i], context))
      return (&rarray->ra.data[i]);
  return (NULL);
}

mr_status_t
mr_ic_none_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context)
{
  mr_ic_rarray_t * rarray = ic->ext.ptr;
  int i, count;

  if ((NULL == rarray) || (NULL == visit_fn))
    return (MR_FAILURE);

  count = rarray->ra.size / sizeof (rarray->ra.data[0]);
  for (i = 0; i < count; ++i)
    if (MR_SUCCESS != visit_fn (rarray->ra.data[i], context))
      return (MR_FAILURE);
  return (MR_SUCCESS);
}

mr_status_t
mr_ic_none_index (mr_ic_t * ic, mr_ic_rarray_t * rarray, const void * context)
{
  mr_ic_rarray_t * copy;
  mr_ic_none_free (ic, context);

  if ((NULL == rarray) || (NULL == rarray->ra.ptr_type) ||
      (0 != strcmp (MR_IC_NONE_TYPE_T_STR, rarray->ra.ptr_type)))
    {
      ic->ext.ptr = rarray;
      return (MR_SUCCESS);
    }

  copy = MR_MALLOC (sizeof (*copy));
  if (NULL == copy)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (MR_FAILURE);
    }

  *copy = *rarray;
  copy->ra.ext.ptr = NULL;
  copy->ra.data = MR_MALLOC (copy->ra.size);
  if (NULL == copy->ra.data)
    {
      MR_FREE (copy);
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (MR_FAILURE);
    }

  memcpy (copy->ra.data, rarray->ra.data, copy->ra.size);
  ic->ext.ptr = copy;
  return (MR_SUCCESS);
}

void
mr_ic_none_free (mr_ic_t * ic, const void * context)
{
  mr_ic_rarray_t * rarray = ic->ext.ptr;

  ic->ext.ptr = NULL;

  if ((NULL == rarray) || (NULL == rarray->ra.ptr_type) ||
      (0 != strcmp (MR_IC_NONE_TYPE_T_STR, rarray->ra.ptr_type)))
    return;

  if (NULL != rarray->ra.data)
    MR_FREE (rarray->ra.data);
  MR_FREE (rarray);
}

mr_status_t
mr_ic_none_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type)
{
  if ((NULL == ic) || (NULL == compar_fn))
    return (MR_FAILURE);

  ic->ic_type = MR_IC_NONE;
  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->add = mr_ic_none_add;
  ic->find = mr_ic_none_find;
  ic->foreach = mr_ic_none_foreach;
  ic->index = mr_ic_none_index;
  ic->free = mr_ic_none_free;
  ic->ext.ptr = NULL;
  return (MR_SUCCESS);
}

mr_ptr_t *
mr_ic_sorted_array_add (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ic_rarray_t * rarray = ic->ext.ptr;
  mr_ptr_t * add;
  int i;

  if (NULL == rarray)
    return (NULL);

  add = mr_rarray_append ((mr_rarray_t*)(void*)&rarray->ra, sizeof (rarray->ra.data[0]));
  if (NULL == add)
    return (NULL);

  for (i = rarray->ra.size / sizeof (rarray->ra.data[0]) - 1; i > 0; --i)
    {
      rarray->ra.data[i] = rarray->ra.data[i - 1];
      if (ic->compar_fn (key, rarray->ra.data[i], context) >= 0)
	break;
    }
  rarray->ra.data[i] = key;

  return (&rarray->ra.data[i]);
}

mr_ptr_t *
mr_ic_sorted_array_find (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ic_rarray_t * rarray = ic->ext.ptr;
  int up, down;

  if (NULL == rarray)
    return (NULL);

  down = 0;
  up = rarray->ra.size / sizeof (rarray->ra.data[0]);

  while (up != down)
    {
      int mid = (down + up) >> 1;
      int diff = ic->compar_fn (key, rarray->ra.data[mid], context);
      if (0 == diff)
	return (&rarray->ra.data[mid]);
      if (diff < 0)
	up = mid;
      else
	down = mid + 1;
    }
  return (NULL);
}

TYPEDEF_STRUCT (mr_sort_key_t,
		(const void *, context),
		(mr_ic_t *, ic))

static int
mr_sort_key_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const mr_sort_key_t * mr_sort_key = context;
  const mr_ptr_t * x_ = x.ptr;
  const mr_ptr_t * y_ = y.ptr;
  return (mr_sort_key->ic->compar_fn (*x_, *y_, mr_sort_key->context));
}

mr_status_t
mr_ic_sorted_array_index (mr_ic_t * ic, mr_ic_rarray_t * rarray_, const void * context)
{
  mr_ic_rarray_t * rarray = ic->ext.ptr;
  mr_sort_key_t mr_sort_key = { .context = context, .ic = ic, };

  if (NULL == rarray)
    return (MR_FAILURE);

  if (NULL != rarray->ra.data)
    MR_FREE (rarray->ra.data);

  rarray->ra.size = rarray->ra.alloc_size = rarray_->ra.size;
  rarray->ra.data = MR_MALLOC (rarray->ra.alloc_size);
  if (NULL == rarray->ra.data)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      rarray->ra.size = rarray->ra.alloc_size = 0;
      return (MR_FAILURE);
    }

  memcpy (rarray->ra.data, rarray_->ra.data, rarray->ra.size);
  hsort (rarray->ra.data, rarray->ra.size / sizeof (rarray->ra.data[0]), sizeof (rarray->ra.data[0]), mr_sort_key_cmp, &mr_sort_key);
  return (MR_SUCCESS);
}

void
mr_ic_sorted_array_free (mr_ic_t * ic, const void * context)
{
  mr_ic_rarray_t * rarray = ic->ext.ptr;

  if (NULL == rarray)
    return;

  if (NULL != rarray->ra.data)
    MR_FREE (rarray->ra.data);
  MR_FREE (rarray);
  ic->ext.ptr = NULL;
}

mr_status_t
mr_ic_sorted_array_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type)
{
  mr_ic_rarray_t * rarray;

  if ((NULL == ic) || (NULL == compar_fn))
    return (MR_FAILURE);

  rarray = MR_MALLOC (sizeof (*rarray));
  if (NULL == rarray)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (MR_FAILURE);
    }
  memset (rarray, 0, sizeof (*rarray));
  rarray->ra.data = NULL;
  rarray->ra.size = rarray->ra.alloc_size = 0;

  ic->ic_type = MR_IC_SORTED_ARRAY;
  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->add = mr_ic_sorted_array_add;
  ic->find = mr_ic_sorted_array_find;
  ic->foreach = mr_ic_none_foreach;
  ic->index = mr_ic_sorted_array_index;
  ic->free = mr_ic_sorted_array_free;
  ic->ext.ptr = rarray;
  return (MR_SUCCESS);
}

#define MR_HASH_TABLE_SIZE_MULT (1.05)
/* MR_HASH_TABLE_SIZE_MULT: HASH_NEXT ( / cmp matched) ratio, HASH_TREE ( / cmp matched) ratio */
/* 1.5 ( / 1050008 22904.0) 45.84 ( / 1366426 30009.0) 45.53 */
/* 1.4 ( / 1020647 22312.0) 45.74 ( / 1329262 29417.0) 45.18 */
/* 1.3 ( / 1013628 21872.0) 46.34 ( / 1320209 28969.0) 45.57 */
/* 1.2 ( / 1003236 21392.0) 46.89 ( / 1295439 28470.0) 45.50 */
/* 1.1 ( / 1005402 20952.0) 47.98 ( / 1283147 28012.0) 45.80 */
/* 1.05 ( / 984455 20748.0) 47.44 ( / 1278280 27831.0) 45.93 */
/* 1.0 ( / 1012156 20624.0) 49.07 ( / 1278339 27685.0) 46.17 */

void
mr_ic_hash_free (mr_ic_t * ic, const void * context)
{
  mr_ic_hash_t * index = ic->ext.ptr;

  if (NULL == index)
    return;

  index->index_free (ic, context);
  MR_FREE (index);
  ic->ext.ptr = NULL;
}

static int
mr_ic_hash_get_backet (mr_ic_hash_t * index, mr_ptr_t key, const void * context)
{
  mr_hash_value_t hash_value = index->hash_fn (key, context);
  int hash_size = index->index.size / sizeof (index->index.data[0]);
  if (hash_size <= 0)
    return (-1);
  return (hash_value % hash_size);
}

static mr_ptr_t *
mr_ic_hash_add_inner (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ic_hash_t * index = ic->ext.ptr;
  int bucket = mr_ic_hash_get_backet (index, key, context);
  if (bucket < 0)
    return (NULL);
  return (index->index_add (ic, key, context, bucket));
}

TYPEDEF_STRUCT (mr_ic_hash_index_context_t,
		(mr_ic_t *, ic),
		(const void *, context))

static mr_status_t
mr_ic_hash_index_visitor (mr_ptr_t key, const void * context)
{
  const mr_ic_hash_index_context_t * mr_ic_hash_index_context = context;
  mr_ptr_t * add = mr_ic_find (mr_ic_hash_index_context->ic, key, mr_ic_hash_index_context->context);

  if (NULL == add)
    {
      mr_ic_hash_t * index = mr_ic_hash_index_context->ic->ext.ptr;
      ++index->items_count;
      add = mr_ic_hash_add_inner (mr_ic_hash_index_context->ic, key, mr_ic_hash_index_context->context);
    }

  return ((NULL == add) ? MR_FAILURE : MR_SUCCESS);
}

static mr_status_t
mr_ic_hash_index_inner (mr_ic_t * src_ic, mr_ic_t * dst_ic, int items_count, const void * context)
{
  mr_ic_hash_t * index = dst_ic->ext.ptr;
  mr_ic_hash_index_context_t mr_ic_hash_index_context = {
    .ic = dst_ic,
    .context = context,
  };

  index->index_free (dst_ic, context);

  index->items_count = items_count;

  if (0 == index->items_count)
    return (MR_SUCCESS);

  index->index.alloc_size = index->index.size = ((int)(index->items_count * 2 * MR_HASH_TABLE_SIZE_MULT)) * sizeof (index->index.data[0]);
  index->index.data = MR_MALLOC (index->index.alloc_size);
  if (NULL == index->index.data)
    return (MR_FAILURE);

  memset (index->index.data, 0, index->index.alloc_size);

  index->items_count = 0;
  return (mr_ic_foreach (src_ic, mr_ic_hash_index_visitor, &mr_ic_hash_index_context));
}

mr_status_t
mr_ic_hash_index (mr_ic_t * ic, mr_ic_rarray_t * rarray, const void * context)
{
  mr_status_t status;
  mr_ic_t ic_none;
  mr_ic_hash_t * index = ic->ext.ptr;

  if ((NULL == index) || (NULL == rarray))
    return (MR_FAILURE);

  mr_ic_none_new (&ic_none, ic->compar_fn, ic->key_type);
  mr_ic_none_index (&ic_none, rarray, context);
  status = mr_ic_hash_index_inner (&ic_none, ic, rarray->ra.size / sizeof (rarray->ra.data[0]), context);
  mr_ic_free (&ic_none, context);
  return (status);
}

mr_ptr_t *
mr_ic_hash_add (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ic_hash_t * index = ic->ext.ptr;

  if (NULL == index)
    return (NULL);

  if (index->index.size / sizeof (index->index.data[0]) <= ++index->items_count * MR_HASH_TABLE_SIZE_MULT)
    {
      mr_ic_hash_t ic_hash = *index;
      mr_ic_t dst_ic = *ic;
      memset (&ic_hash.index, 0, sizeof (ic_hash.index));
      dst_ic.ext.ptr = &ic_hash;
      
      if (MR_SUCCESS != mr_ic_hash_index_inner (ic, &dst_ic, index->items_count, context))
	return (NULL);
      
      index->index_free (ic, context);
      *index = ic_hash;
    }
  return (mr_ic_hash_add_inner (ic, key, context));
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
mr_ic_hash_tree_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context)
{
  mr_ic_hash_t * index = ic->ext.ptr;
  mr_ic_rbtree_foreach_context_t mr_ic_rbtree_foreach_context = {
    .visit_fn = visit_fn,
    .context = context,
  };
  int i;

  if ((NULL == index) || (NULL == visit_fn))
    return (MR_FAILURE);

  if (0 != setjmp (mr_ic_rbtree_foreach_context.jmp_buf))
    return (MR_FAILURE);

  for (i = index->index.size / sizeof (index->index.data[0]) - 1; i >= 0; --i)
    mr_twalk (index->index.data[i].ptr, visit_node, &mr_ic_rbtree_foreach_context);
  return (MR_SUCCESS);
}

mr_ptr_t *
mr_ic_hash_tree_find (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ic_hash_t * index = ic->ext.ptr;
  int bucket;
  
  if (NULL == index)
    return (NULL);

  bucket = mr_ic_hash_get_backet (index, key, context);
  if (bucket < 0)
    return (NULL);
  return (mr_tfind (key, (mr_red_black_tree_node_t**)&index->index.data[bucket].ptr, ic->compar_fn, context));
}

static void
dummy_free_fn (mr_ptr_t key, const void * context)
{
}

void
mr_ic_hash_tree_index_free (mr_ic_t * ic, const void * context)
{
  mr_ic_hash_t * index = ic->ext.ptr;
  
  if (NULL != index->index.data)
    {
      int i;
      for (i = index->index.size / sizeof (index->index.data[0]) - 1; i >= 0; --i)
	mr_tdestroy (index->index.data[i].ptr, dummy_free_fn, context);
      MR_FREE (index->index.data);
    }
  memset (&index->index, 0, sizeof (index->index));
}

mr_ptr_t *
mr_ic_hash_tree_index_add (mr_ic_t * ic, mr_ptr_t key, const void * context, int bucket)
{
  mr_ic_hash_t * index = ic->ext.ptr;
  mr_ptr_t * find = mr_tsearch (key, (mr_red_black_tree_node_t**)&index->index.data[bucket].ptr, ic->compar_fn, context);
  if (NULL == find)
    MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
  return (find);
}

mr_status_t
mr_ic_hash_tree_new (mr_ic_t * ic, mr_hash_fn_t hash_fn, mr_compar_fn_t compar_fn, char * key_type)
{
  mr_ic_hash_t * index;

  if ((NULL == ic) || (NULL == compar_fn) || (NULL == hash_fn))
    return (MR_FAILURE);

  index = MR_MALLOC (sizeof (*index));
  if (NULL == index)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (MR_FAILURE);
    }

  memset (index, 0, sizeof (*index));
  index->items_count = 0;
  index->hash_fn = hash_fn;
  index->index_add = mr_ic_hash_tree_index_add;
  index->index_free = mr_ic_hash_tree_index_free;
  index->bucket_type = "mr_red_black_tree_node_t";

  ic->ic_type = MR_IC_HASH_TREE;
  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->add = mr_ic_hash_add;
  ic->find = mr_ic_hash_tree_find;
  ic->foreach = mr_ic_hash_tree_foreach;
  ic->index = mr_ic_hash_index;
  ic->free = mr_ic_hash_free;
  ic->ext.ptr = index;

  return (MR_SUCCESS);
}

void
mr_ic_rbtree_free (mr_ic_t * ic, const void * context)
{
  mr_tdestroy (ic->ext.ptr, dummy_free_fn, context);
  ic->ext.ptr = NULL;
}

mr_ptr_t *
mr_ic_rbtree_add (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ptr_t * add = mr_tsearch (key, (mr_red_black_tree_node_t**)(void*)&ic->ext.ptr, ic->compar_fn, context);
  if (NULL == add)
    MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
  return (add);
}

mr_ptr_t *
mr_ic_rbtree_find (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  return (mr_tfind (key, (mr_red_black_tree_node_t**)(void*)&ic->ext.ptr, ic->compar_fn, context));
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

  mr_twalk (ic->ext.ptr, visit_node, &mr_ic_rbtree_foreach_context);
  return (MR_SUCCESS);
}

mr_status_t
mr_ic_rbtree_index (mr_ic_t * ic, mr_ic_rarray_t * rarray, const void * context)
{
  int i;
  mr_ic_rbtree_free (ic, context);
  for (i = rarray->ra.size / sizeof (rarray->ra.data[0]) - 1; i >= 0; --i)
    if (NULL == mr_ic_rbtree_add (ic, rarray->ra.data[i], context))
      return (MR_FAILURE);
  return (MR_SUCCESS);
}

mr_status_t
mr_ic_rbtree_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type)
{
  if ((NULL == ic) || (NULL == compar_fn))
    return (MR_FAILURE);

  ic->ic_type = MR_IC_RBTREE;
  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->add = mr_ic_rbtree_add;
  ic->find = mr_ic_rbtree_find;
  ic->foreach = mr_ic_rbtree_foreach;
  ic->index = mr_ic_rbtree_index;
  ic->free = mr_ic_rbtree_free;
  ic->ext.ptr = NULL;

  return (MR_SUCCESS);
}

void
mr_ic_hash_next_index_free (mr_ic_t * ic, const void * context)
{
  mr_ic_hash_t * index = ic->ext.ptr;
  if (NULL != index->index.data)
    MR_FREE (index->index.data);
  memset (&index->index, 0, sizeof (index->index));
}

mr_ptr_t *
mr_ic_hash_next_index_add (mr_ic_t * ic, mr_ptr_t key, const void * context, int bucket)
{
  mr_ic_hash_t * index = ic->ext.ptr;
  int i, count = index->index.size / sizeof (index->index.data[0]);
  
  if (0 == key.long_int_t)
    {
      index->index.ptr_type = "has zero";
      return (&index->index.ext);
    }

  for (i = bucket; ;)
    {
      if (0 == index->index.data[i].long_int_t)
	{
	  index->index.data[i] = key;
	  return (&index->index.data[i]);
	}
      if (++i >= count)
	i = 0;
      if (i == bucket)
	break;
    }
  MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_HASH_TABLE_ERROR);
  return (NULL);
}

mr_ptr_t *
mr_ic_hash_next_find (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ic_hash_t * index = ic->ext.ptr;
  int i, bucket, count = index->index.size / sizeof (index->index.data[0]);
  
  if (0 == key.long_int_t)
    return ((index->index.ptr_type != NULL) ? &index->index.ext : NULL);

  bucket = mr_ic_hash_get_backet (index, key, context);
  if (bucket < 0)
    return (NULL);

  for (i = bucket; ;)
    {
      if (0 == index->index.data[i].long_int_t)
	return (NULL);
      if (0 == ic->compar_fn (key, index->index.data[i], context))
	return (&index->index.data[i]);
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
  mr_ic_hash_t * index = ic->ext.ptr;
  int i;

  if (NULL == index)
    return (MR_FAILURE);
  
  if (index->index.ptr_type != NULL)
    if (MR_SUCCESS != visit_fn (&index->index.ext, context))
      return (MR_FAILURE);

  for (i = index->index.size / sizeof (index->index.data[0]) - 1; i >= 0; --i)
    if (0 != index->index.data[i].long_int_t)
      if (MR_SUCCESS != visit_fn (index->index.data[i], context))
	return (MR_FAILURE);
  return (MR_SUCCESS);
}

mr_status_t
mr_ic_hash_next_new (mr_ic_t * ic, mr_hash_fn_t hash_fn, mr_compar_fn_t compar_fn, char * key_type)
{
  mr_ic_hash_t * index;
  
  if ((NULL == ic) || (NULL == compar_fn))
    return (MR_FAILURE);

  index = MR_MALLOC (sizeof (*index));
  if (NULL == index)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (MR_FAILURE);
    }
  
  memset (index, 0, sizeof (*index));
  index->items_count = 0;
  index->hash_fn = hash_fn;
  index->index_add = mr_ic_hash_next_index_add;
  index->index_free = mr_ic_hash_next_index_free;
  index->bucket_type = key_type;
  
  ic->ic_type = MR_IC_HASH_NEXT;
  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->add = mr_ic_hash_add;
  ic->find = mr_ic_hash_next_find;
  ic->foreach = mr_ic_hash_next_foreach;
  ic->index = mr_ic_hash_index;
  ic->free = mr_ic_hash_free;
  ic->ext.ptr = index;

  return (MR_SUCCESS);
}

mr_ptr_t *
mr_ic_add (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ptr_t * find = mr_ic_find (ic, key, context);
  if (NULL != find)
    return (find);

  if (NULL == ic)
    return (NULL);

  if (ic->add)
    return (ic->add (ic, key, context));
  return (NULL);
}

mr_ptr_t *
mr_ic_find (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  if ((NULL == ic) || (NULL == ic->compar_fn))
    return (NULL);
  if (ic->find)
    return (ic->find (ic, key, context));
  return (NULL);
}

mr_status_t
mr_ic_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context)
{
  if ((NULL == ic) || (NULL == visit_fn))
    return (MR_FAILURE);
  if (ic->foreach)
    return (ic->foreach (ic, visit_fn, context));
  return (MR_FAILURE);
}

mr_status_t
mr_ic_index (mr_ic_t * ic, mr_ic_rarray_t * rarray, const void * context)
{
  int i, count;
  if ((NULL == ic) || (NULL == rarray))
    return (MR_FAILURE);
  if (ic->index)
    return (ic->index (ic, rarray, context));
  count = rarray->ra.size / sizeof (rarray->ra.data[0]);
  for (i = 0; i < count; ++i)
    if (NULL == mr_ic_add (ic, rarray->ra.data[i], context))
      return (MR_FAILURE);
  return (MR_SUCCESS);
}

void
mr_ic_free (mr_ic_t * ic, const void * context)
{
  if (NULL == ic)
    return;
  if (ic->free)
    ic->free (ic, context);
}

mr_status_t
mr_ic_new (mr_ic_t * ic, mr_hash_fn_t hash_fn, mr_compar_fn_t compar_fn, char * key_type, mr_ic_type_t mr_ic_type)
{
  switch (mr_ic_type)
    {
    case MR_IC_NONE:
      return (mr_ic_none_new (ic, compar_fn, key_type));
    case MR_IC_RBTREE:
      return (mr_ic_rbtree_new (ic, compar_fn, key_type));
    case MR_IC_SORTED_ARRAY:
      return (mr_ic_sorted_array_new (ic, compar_fn, key_type));
    case MR_IC_HASH_TREE:
      return (mr_ic_hash_tree_new (ic, hash_fn, compar_fn, key_type));
    case MR_IC_HASH_NEXT:
      return (mr_ic_hash_next_new (ic, hash_fn, compar_fn, key_type));
    default:
      return (MR_FAILURE);
    }
}
