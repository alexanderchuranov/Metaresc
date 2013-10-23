/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <setjmp.h>
#include <string.h>

#include <metaresc.h>
#include <mr_tsearch.h>
#include <mr_hsort.h>
#include <mr_ic.h>

/* ----------------------- MR_IC_NONE ----------------------- */

mr_ptr_t *
mr_ic_none_add (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ic_rarray_t * rarray = ic->rarray;
  mr_ptr_t * add;

  if ((rarray == NULL) ||
      (rarray->ra.size + sizeof (rarray->ra.data[0]) > rarray->ra.alloc_size))
    if ((NULL == rarray->ra.ptr_type) ||
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
	ic->rarray = rarray = rarray_;
      }

  add = mr_rarray_append ((mr_rarray_t*)(void*)&rarray->ra, sizeof (rarray->ra.data[0]));

  if (NULL == add)
    return (NULL);

  *add = key;
  return (add);
}

mr_status_t
mr_ic_none_del (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ic_rarray_t * rarray = ic->rarray;
  mr_ptr_t * find = mr_ic_find (ic, key, context);
  if (NULL == find)
    return (MR_FAILURE);
  memmove (find, &find[1], (char*)rarray->ra.data + rarray->ra.size - (char*)find - sizeof (rarray->ra.data[0]));
  rarray->ra.size -= sizeof (rarray->ra.data[0]);
  return (MR_SUCCESS);
}

mr_ptr_t *
mr_ic_none_find (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ic_rarray_t * rarray = ic->rarray;
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
  mr_ic_rarray_t * rarray = ic->rarray;
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
  mr_ic_none_free (ic);

  if ((NULL == rarray) || (NULL == rarray->ra.ptr_type) ||
      (0 != strcmp (MR_IC_NONE_TYPE_T_STR, rarray->ra.ptr_type)))
    {
      ic->rarray = rarray;
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
  ic->rarray = copy;
  return (MR_SUCCESS);
}

void
mr_ic_none_reset (mr_ic_t * ic)
{
  mr_ic_rarray_t * rarray = ic->rarray;

  if (NULL == rarray)
    return;
  
  if ((NULL != rarray->ra.ptr_type) &&
      (0 == strcmp (MR_IC_NONE_TYPE_T_STR, rarray->ra.ptr_type)))
    rarray->ra.size = 0;
  else
    ic->rarray = NULL;
}

void
mr_ic_none_free (mr_ic_t * ic)
{
  mr_ic_rarray_t * rarray = ic->rarray;

  ic->rarray = NULL;

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
  ic->del = mr_ic_none_del;
  ic->find = mr_ic_none_find;
  ic->foreach = mr_ic_none_foreach;
  ic->index = mr_ic_none_index;
  ic->reset = mr_ic_none_reset;
  ic->free = mr_ic_none_free;
  ic->rarray = NULL;
  return (MR_SUCCESS);
}

/* ----------------------- MR_IC_SORTED_ARRAY ----------------------- */

static void
dummy_free_fn (mr_ptr_t key, const void * context)
{
}

void
mr_ic_rbtree_reset (mr_ic_t * ic)
{
  mr_ic_rbtree_free (ic);
}

void
mr_ic_rbtree_free (mr_ic_t * ic)
{
  mr_tdestroy (ic->tree, dummy_free_fn, NULL);
  ic->tree = NULL;
}

mr_ptr_t *
mr_ic_rbtree_add (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ptr_t * add = mr_tsearch (key, &ic->tree, ic->compar_fn, context);
  if (NULL == add)
    MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
  return (add);
}

mr_status_t
mr_ic_rbtree_del (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ptr_t * find = mr_ic_find (ic, key, context);
  if (find == NULL)
    return (MR_FAILURE);
  mr_tdelete (key, &ic->tree, ic->compar_fn, context);
  return (MR_SUCCESS);
}

mr_ptr_t *
mr_ic_rbtree_find (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  return (mr_tfind (key, &ic->tree, ic->compar_fn, context));
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
mr_ic_rbtree_index (mr_ic_t * ic, mr_ic_rarray_t * rarray, const void * context)
{
  int i;
  mr_ic_rbtree_free (ic);
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
  ic->del = mr_ic_rbtree_del;
  ic->find = mr_ic_rbtree_find;
  ic->foreach = mr_ic_rbtree_foreach;
  ic->index = mr_ic_rbtree_index;
  ic->reset = mr_ic_rbtree_reset;
  ic->free = mr_ic_rbtree_free;
  ic->tree = NULL;

  return (MR_SUCCESS);
}

/* ----------------------- MR_IC_SORTED_ARRAY ----------------------- */

mr_ptr_t *
mr_ic_sorted_array_add (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ic_rarray_t * rarray = ic->rarray;
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

mr_status_t
mr_ic_sorted_array_del (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  return (mr_ic_none_del (ic, key, context));
}

mr_ptr_t *
mr_ic_sorted_array_find (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ic_rarray_t * rarray = ic->rarray;
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
  mr_ic_rarray_t * rarray = ic->rarray;
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
mr_ic_sorted_array_reset (mr_ic_t * ic)
{
  mr_ic_rarray_t * rarray = ic->rarray;

  if (NULL == rarray)
    return;

  rarray->ra.size = 0;
}

void
mr_ic_sorted_array_free (mr_ic_t * ic)
{
  mr_ic_rarray_t * rarray = ic->rarray;

  if (NULL == rarray)
    return;

  if (NULL != rarray->ra.data)
    MR_FREE (rarray->ra.data);
  MR_FREE (rarray);
  ic->rarray = NULL;
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
  ic->del = mr_ic_sorted_array_del;
  ic->find = mr_ic_sorted_array_find;
  ic->foreach = mr_ic_none_foreach;
  ic->index = mr_ic_sorted_array_index;
  ic->reset = mr_ic_sorted_array_reset;
  ic->free = mr_ic_sorted_array_free;
  ic->rarray = rarray;
  return (MR_SUCCESS);
}

/* ----------------------- MR_IC_HASH ----------------------- */

#define MR_HASH_TABLE_SIZE_MULT (1.4)
/* MR_HASH_TABLE_SIZE_MULT: HASH_NEXT ( / cmp matched) ratio, HASH_TREE ( / cmp matched) ratio */
/* 1.5  ( / 24151 15217.0) 1.58 ( / 31483 20344.0) 1.54 */
/* 1.4  ( / 17783 14921.0) 1.19 ( / 22681 20048.0) 1.13 */
/* 1.3  ( / 18459 14701.0) 1.25 ( / 23180 19820.0) 1.16 */
/* 1.2  ( / 38973 14461.0) 2.69 ( / 24962 19561.0) 1.27 */
/* 1.1  ( / 48032 14241.0) 3.37 ( / 29766 19323.0) 1.54 */
/* 1.05 ( / 30735 14139.0) 2.17 ( / 28792 19244.0) 1.49 */
/* 1.0  ( / 62538 14077.0) 4.44 ( / 29668 19160.0) 1.54 */

void
mr_ic_hash_reset (mr_ic_t * ic)
{
  mr_ic_hash_t * hash = ic->hash;

  if (NULL == hash)
    return;

  hash->index_free (ic);
}

void
mr_ic_hash_free (mr_ic_t * ic)
{
  mr_ic_hash_t * hash = ic->hash;

  if (NULL == hash)
    return;

  hash->index_free (ic);
  MR_FREE (hash);
  ic->hash = NULL;
}

static int
mr_ic_hash_get_backet (mr_ic_hash_t * hash, mr_ptr_t key, const void * context)
{
  mr_hash_value_t hash_value = hash->hash_fn (key, context);
  int hash_size = hash->index.size / sizeof (hash->index.data[0]);
  if (hash_size <= 0)
    return (-1);
  return (hash_value % hash_size);
}

static mr_ptr_t *
mr_ic_hash_add_inner (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ic_hash_t * hash = ic->hash;
  int bucket = mr_ic_hash_get_backet (hash, key, context);
  if (bucket < 0)
    return (NULL);
  return (hash->index_add (ic, key, context, bucket));
}

TYPEDEF_STRUCT (mr_ic_hash_index_context_t,
		(mr_ic_t *, ic),
		(const void *, context))

static mr_status_t
mr_ic_hash_index_visitor (mr_ptr_t key, const void * context)
{
  const mr_ic_hash_index_context_t * mr_ic_hash_index_context = context;
  mr_ptr_t * add = mr_ic_hash_add_inner (mr_ic_hash_index_context->ic, key, mr_ic_hash_index_context->context);

  if (NULL != add)
    {
      mr_ic_hash_t * hash = mr_ic_hash_index_context->ic->hash;
      ++hash->items_count;
    }

  return ((NULL == add) ? MR_FAILURE : MR_SUCCESS);
}

static mr_status_t
mr_ic_hash_index_inner (mr_ic_t * src_ic, mr_ic_t * dst_ic, int items_count, const void * context)
{
  mr_ic_hash_t * hash = dst_ic->hash;
  mr_ic_hash_index_context_t mr_ic_hash_index_context = {
    .ic = dst_ic,
    .context = context,
  };

  hash->index_free (dst_ic);

  hash->items_count = items_count;

  if (0 == hash->items_count)
    return (MR_SUCCESS);

  hash->index.alloc_size = hash->index.size = ((int)(hash->items_count * 2 * MR_HASH_TABLE_SIZE_MULT)) * sizeof (hash->index.data[0]);
  hash->index.data = MR_MALLOC (hash->index.alloc_size);
  if (NULL == hash->index.data)
    return (MR_FAILURE);

  memset (hash->index.data, 0, hash->index.alloc_size);

  hash->items_count = 0;
  return (mr_ic_foreach (src_ic, mr_ic_hash_index_visitor, &mr_ic_hash_index_context));
}

mr_status_t
mr_ic_hash_index (mr_ic_t * ic, mr_ic_rarray_t * rarray, const void * context)
{
  mr_status_t status;
  mr_ic_t ic_none;
  mr_ic_hash_t * hash = ic->hash;

  if ((NULL == hash) || (NULL == rarray))
    return (MR_FAILURE);

  mr_ic_none_new (&ic_none, ic->compar_fn, ic->key_type);
  mr_ic_none_index (&ic_none, rarray, context);
  status = mr_ic_hash_index_inner (&ic_none, ic, rarray->ra.size / sizeof (rarray->ra.data[0]), context);
  mr_ic_free (&ic_none);
  return (status);
}

mr_ptr_t *
mr_ic_hash_add (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ic_hash_t * hash = ic->hash;

  if (NULL == hash)
    return (NULL);

  if (hash->index.size / sizeof (hash->index.data[0]) <= ++hash->items_count * MR_HASH_TABLE_SIZE_MULT)
    {
      mr_ic_hash_t ic_hash = *hash;
      mr_ic_t dst_ic = *ic;
      memset (&ic_hash.index, 0, sizeof (ic_hash.index));
      dst_ic.hash = &ic_hash;
      
      if (MR_SUCCESS != mr_ic_hash_index_inner (ic, &dst_ic, hash->items_count, context))
	return (NULL);
      
      hash->index_free (ic);
      *hash = ic_hash;
    }
  return (mr_ic_hash_add_inner (ic, key, context));
}

/* ----------------------- MR_IC_HASH_TREE ----------------------- */

mr_status_t
mr_ic_hash_tree_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context)
{
  mr_ic_hash_t * hash = ic->hash;
  mr_ic_rbtree_foreach_context_t mr_ic_rbtree_foreach_context = {
    .visit_fn = visit_fn,
    .context = context,
  };
  int i;

  if ((NULL == hash) || (NULL == visit_fn))
    return (MR_FAILURE);

  if (0 != setjmp (mr_ic_rbtree_foreach_context.jmp_buf))
    return (MR_FAILURE);

  for (i = hash->index.size / sizeof (hash->index.data[0]) - 1; i >= 0; --i)
    mr_twalk (hash->index.data[i].ptr, visit_node, &mr_ic_rbtree_foreach_context);
  return (MR_SUCCESS);
}

mr_status_t
mr_ic_hash_tree_del (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ic_hash_t * hash = ic->hash;
  mr_ptr_t * find = mr_ic_find (ic, key, context);
  int bucket;

  if (NULL == find)
    return (MR_FAILURE);

  --hash->items_count;
  bucket = mr_ic_hash_get_backet (hash, key, context);
  mr_tdelete (key, (mr_red_black_tree_node_t**)&hash->index.data[bucket].ptr, ic->compar_fn, context);
  return (MR_SUCCESS);
}

mr_ptr_t *
mr_ic_hash_tree_find (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ic_hash_t * hash = ic->hash;
  int bucket;
  
  if (NULL == hash)
    return (NULL);

  bucket = mr_ic_hash_get_backet (hash, key, context);
  if (bucket < 0)
    return (NULL);
  return (mr_tfind (key, (mr_red_black_tree_node_t**)&hash->index.data[bucket].ptr, ic->compar_fn, context));
}

void
mr_ic_hash_tree_index_free (mr_ic_t * ic)
{
  mr_ic_hash_t * hash = ic->hash;
  
  if (NULL != hash->index.data)
    {
      int i;
      for (i = hash->index.size / sizeof (hash->index.data[0]) - 1; i >= 0; --i)
	mr_tdestroy (hash->index.data[i].ptr, dummy_free_fn, NULL);
      MR_FREE (hash->index.data);
    }
  memset (&hash->index, 0, sizeof (hash->index));
}

mr_ptr_t *
mr_ic_hash_tree_index_add (mr_ic_t * ic, mr_ptr_t key, const void * context, int bucket)
{
  mr_ic_hash_t * hash = ic->hash;
  mr_ptr_t * find = mr_tsearch (key, (mr_red_black_tree_node_t**)&hash->index.data[bucket].ptr, ic->compar_fn, context);
  if (NULL == find)
    MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
  return (find);
}

mr_status_t
mr_ic_hash_tree_new (mr_ic_t * ic, mr_hash_fn_t hash_fn, mr_compar_fn_t compar_fn, char * key_type)
{
  mr_ic_hash_t * hash;

  if ((NULL == ic) || (NULL == compar_fn) || (NULL == hash_fn))
    return (MR_FAILURE);

  hash = MR_MALLOC (sizeof (*hash));
  if (NULL == hash)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (MR_FAILURE);
    }

  memset (hash, 0, sizeof (*hash));
  hash->items_count = 0;
  hash->hash_fn = hash_fn;
  hash->index_add = mr_ic_hash_tree_index_add;
  hash->index_free = mr_ic_hash_tree_index_free;
  hash->bucket_type = "mr_red_black_tree_node_t";

  ic->ic_type = MR_IC_HASH_TREE;
  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->add = mr_ic_hash_add;
  ic->del = mr_ic_hash_tree_del;
  ic->find = mr_ic_hash_tree_find;
  ic->foreach = mr_ic_hash_tree_foreach;
  ic->index = mr_ic_hash_index;
  ic->reset = mr_ic_hash_reset;
  ic->free = mr_ic_hash_free;
  ic->hash = hash;

  return (MR_SUCCESS);
}

/* ----------------------- MR_IC_HASH_NEXT ----------------------- */

void
mr_ic_hash_next_index_free (mr_ic_t * ic)
{
  mr_ic_hash_t * hash = ic->hash;
  if (NULL != hash->index.data)
    MR_FREE (hash->index.data);
  memset (&hash->index, 0, sizeof (hash->index));
}

mr_ptr_t *
mr_ic_hash_next_index_add (mr_ic_t * ic, mr_ptr_t key, const void * context, int bucket)
{
  mr_ic_hash_t * hash = ic->hash;
  int i, count = hash->index.size / sizeof (hash->index.data[0]);
  
  if (0 == key.long_int_t)
    {
      hash->index.ptr_type = "has zero";
      return (&hash->index.ext);
    }

  for (i = bucket; ;)
    {
      if (0 == hash->index.data[i].long_int_t)
	{
	  hash->index.data[i] = key;
	  return (&hash->index.data[i]);
	}
      if (++i >= count)
	i = 0;
      if (i == bucket)
	break;
    }
  MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_HASH_TABLE_ERROR);
  return (NULL);
}

mr_status_t
mr_ic_hash_next_del (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ic_hash_t * hash = ic->hash;
  mr_ptr_t * find = mr_ic_find (ic, key, context);

  if (NULL == find)
    return (MR_FAILURE);

  --hash->items_count;
  if (0 == key.long_int_t)
    hash->index.ptr_type = NULL;
  else
    find->long_int_t = 0;
  
  return (MR_SUCCESS);
}

mr_ptr_t *
mr_ic_hash_next_find (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ic_hash_t * hash = ic->hash;
  int i, bucket, count = hash->index.size / sizeof (hash->index.data[0]);
  
  if (0 == key.long_int_t)
    return ((hash->index.ptr_type != NULL) ? &hash->index.ext : NULL);

  bucket = mr_ic_hash_get_backet (hash, key, context);
  if (bucket < 0)
    return (NULL);

  for (i = bucket; ;)
    {
      if (0 == hash->index.data[i].long_int_t)
	return (NULL);
      if (0 == ic->compar_fn (key, hash->index.data[i], context))
	return (&hash->index.data[i]);
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
  mr_ic_hash_t * hash = ic->hash;
  int i;

  if (NULL == hash)
    return (MR_FAILURE);
  
  if (hash->index.ptr_type != NULL)
    if (MR_SUCCESS != visit_fn (hash->index.ext, context))
      return (MR_FAILURE);

  for (i = hash->index.size / sizeof (hash->index.data[0]) - 1; i >= 0; --i)
    if (0 != hash->index.data[i].long_int_t)
      if (MR_SUCCESS != visit_fn (hash->index.data[i], context))
	return (MR_FAILURE);
  return (MR_SUCCESS);
}

mr_status_t
mr_ic_hash_next_new (mr_ic_t * ic, mr_hash_fn_t hash_fn, mr_compar_fn_t compar_fn, char * key_type)
{
  mr_ic_hash_t * hash;
  
  if ((NULL == ic) || (NULL == compar_fn))
    return (MR_FAILURE);

  hash = MR_MALLOC (sizeof (*hash));
  if (NULL == hash)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (MR_FAILURE);
    }
  
  memset (hash, 0, sizeof (*hash));
  hash->items_count = 0;
  hash->hash_fn = hash_fn;
  hash->index_add = mr_ic_hash_next_index_add;
  hash->index_free = mr_ic_hash_next_index_free;
  hash->bucket_type = key_type;
  
  ic->ic_type = MR_IC_HASH_NEXT;
  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->add = mr_ic_hash_add;
  ic->del = mr_ic_hash_next_del;
  ic->find = mr_ic_hash_next_find;
  ic->foreach = mr_ic_hash_next_foreach;
  ic->index = mr_ic_hash_index;
  ic->reset = mr_ic_hash_reset;
  ic->free = mr_ic_hash_free;
  ic->hash = hash;

  return (MR_SUCCESS);
}
