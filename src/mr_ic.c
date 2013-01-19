/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

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
      (0 != strcmp (MR_IC_NONE_TYPE_T, rarray->ra.ptr_type)))
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
      rarray_->ra.ptr_type = MR_IC_NONE_TYPE_T;
      ic->ext.ptr = rarray = rarray_;
    }
  
  add = mr_rarray_append ((mr_rarray_t*)&rarray->ra, sizeof (rarray->ra.data[0]));

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

int
mr_ic_none_index (mr_ic_t * ic, mr_ic_rarray_t * rarray, const void * context)
{
  mr_ic_none_free (ic, context);
  ic->ext.ptr = rarray;
  return (EXIT_SUCCESS);
}
  
void
mr_ic_none_free (mr_ic_t * ic, const void * context)
{
  mr_ic_rarray_t * rarray = ic->ext.ptr;
  
  ic->ext.ptr = NULL;

  if ((NULL == rarray) || (NULL == rarray->ra.ptr_type) ||
      (0 != strcmp (MR_IC_NONE_TYPE_T, rarray->ra.ptr_type)))
    return;

  if (NULL != rarray->ra.data)
    MR_FREE (rarray->ra.data);
  MR_FREE (rarray);
}

int
mr_ic_none_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type)
{
  if ((NULL == ic) || (NULL == compar_fn))
    return (EXIT_FAILURE);
  
  ic->ic_type = MR_IC_NONE;
  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->add = mr_ic_none_add;
  ic->find = mr_ic_none_find;
  ic->index = mr_ic_none_index;
  ic->free = mr_ic_none_free;
  ic->ext.ptr = NULL;
  return (EXIT_SUCCESS);
}

mr_ptr_t *
mr_ic_sorted_array_add (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ic_rarray_t * rarray = ic->ext.ptr;
  mr_ptr_t * add;
  int i;

  if (NULL == rarray)
    return (NULL);

  add = mr_rarray_append ((mr_rarray_t*)&rarray->ra, sizeof (rarray->ra.data[0]));
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

int
mr_ic_sorted_array_index (mr_ic_t * ic, mr_ic_rarray_t * rarray_, const void * context)
{
  mr_ic_rarray_t * rarray = ic->ext.ptr;
  mr_sort_key_t mr_sort_key = { .context = context, .ic = ic, };
  
  if (NULL == rarray)
    return (EXIT_FAILURE);

  if (NULL != rarray->ra.data)
    MR_FREE (rarray->ra.data);

  rarray->ra.size = rarray->ra.alloc_size = rarray_->ra.size;
  rarray->ra.data = MR_MALLOC (rarray->ra.alloc_size);
  if (NULL == rarray->ra.data)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      rarray->ra.size = rarray->ra.alloc_size = 0;
      return (EXIT_FAILURE);
    }

  memcpy (rarray->ra.data, rarray_->ra.data, rarray->ra.size);
  hsort (rarray->ra.data, rarray->ra.size / sizeof (rarray->ra.data[0]), sizeof (rarray->ra.data[0]), mr_sort_key_cmp, &mr_sort_key);
  return (EXIT_SUCCESS);
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

int
mr_ic_sorted_array_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type)
{
  mr_ic_rarray_t * rarray;
  
  if ((NULL == ic) || (NULL == compar_fn))
    return (EXIT_FAILURE);

  rarray = MR_MALLOC (sizeof (*rarray));
  if (NULL == rarray)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (EXIT_FAILURE);
    }
  memset (rarray, 0, sizeof (*rarray));
  rarray->ra.data = NULL;
  rarray->ra.size = rarray->ra.alloc_size = 0;
  
  ic->ic_type = MR_IC_SORTED_ARRAY;
  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->add = mr_ic_sorted_array_add;
  ic->find = mr_ic_sorted_array_find;
  ic->index = mr_ic_sorted_array_index;
  ic->free = mr_ic_sorted_array_free;
  ic->ext.ptr = rarray;
  return (EXIT_SUCCESS);
}

#define MR_HASH_TABLE_SIZE_MULT (1.3)

static void
dummy_free_fn (mr_ptr_t key, const void * context)
{
}

static inline void
mr_ic_hash_free_inner (mr_ic_hash_t * index, const void * context)
{
  if (NULL != index->index.data)
    {
      int i;
      for (i = index->index.size / sizeof (index->index.data[0]) - 1; i >= 0; --i)
	mr_tdestroy (index->index.data[i].root, dummy_free_fn, context);
      MR_FREE (index->index.data);
      index->index.data = NULL;
    }
}

void
mr_ic_hash_free (mr_ic_t * ic, const void * context)
{
  mr_ic_hash_t * index = ic->ext.ptr;
  
  if (NULL == index)
    return;

  mr_ic_hash_free_inner (index, context);
  MR_FREE (index);
  ic->ext.ptr = NULL;
}

static inline mr_ptr_t *
mr_ic_hash_add_inner (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ic_hash_t * index = ic->ext.ptr;
  unsigned int hash_value = index->hash_fn (key, context);
  int hash_size = index->index.size / sizeof (index->index.data[0]);
  if (hash_size <= 0)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_INCORRECT_HASH_SIZE, hash_size);
      return (NULL);
    }
  
  return (mr_tsearch (key, &index->index.data[hash_value % hash_size].root, ic->compar_fn, context));
}

int
mr_ic_hash_index (mr_ic_t * ic, mr_ic_rarray_t * rarray, const void * context)
{
  mr_ic_hash_t * index = ic->ext.ptr;
  
  if ((NULL == index) || (NULL == index->hash_fn) || (NULL == ic->compar_fn) || (NULL == rarray))
    return (EXIT_FAILURE);
  
  mr_ic_hash_free_inner (index, context);

  index->count = rarray->ra.size / sizeof (rarray->ra.data[0]);
  
  if (0 != index->count)
    {
      int i;
      index->index.size = index->index.alloc_size = index->count * 2 * MR_HASH_TABLE_SIZE_MULT * sizeof (index->index.data[0]);
      index->index.data = MR_MALLOC (index->index.alloc_size);
      if (NULL == index->index.data)
	{
	  MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	  return (EXIT_FAILURE);
	}
      memset (index->index.data, 0, index->index.size);

      index->count = 0;
      for (i = rarray->ra.size / sizeof (rarray->ra.data[0]) - 1; i >= 0; --i)
	{
	  mr_ptr_t * key = mr_ic_hash_add_inner (ic, rarray->ra.data[i], context);
	  if (NULL == key)
	    {
	      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	      return (EXIT_FAILURE);
	    }
	  if (0 == memcmp (key, &rarray->ra.data[i], sizeof (*key)))
	    ++index->count;
	}
    }
  return (EXIT_SUCCESS);
}

static void
visit_node (const mr_red_black_tree_node_t * node, mr_rb_visit_order_t order, int level, const void * context)
{
  mr_ic_rarray_t * rarray = (void*)context;
  if ((MR_RB_VISIT_POSTORDER == order) || (MR_RB_VISIT_LEAF == order))
    {
      mr_ptr_t * add = mr_rarray_append ((mr_rarray_t*)&rarray->ra, sizeof (rarray->ra.data[0]));
      if (add)
	*add = node->key;
    }
}

int
mr_ic_hash_reindex (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ic_hash_t * index = ic->ext.ptr;
  mr_ic_rarray_t rarray;
  mr_red_black_tree_node_t node = { .key = key };
  int status;
  int i;
  
  if ((NULL == index) || (NULL == index->hash_fn) || (NULL == ic->compar_fn))
    return (EXIT_FAILURE);

  rarray.ra.size = 0;
  rarray.ra.alloc_size = index->count * sizeof (rarray.ra.data[0]);
  rarray.ra.data = MR_MALLOC (rarray.ra.alloc_size);
  if (NULL == rarray.ra.data)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (EXIT_FAILURE);
    }
  
  for (i = index->index.size / sizeof (index->index.data[0]) - 1; i >= 0; --i)
    mr_twalk (index->index.data[i].root, visit_node, &rarray);
  visit_node (&node, MR_RB_VISIT_LEAF, 0, &rarray);
  
  status = mr_ic_hash_index (ic, &rarray, context);
  MR_FREE (rarray.ra.data);
  
  return (status);
}

mr_ptr_t *
mr_ic_hash_add (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ic_hash_t * index = ic->ext.ptr;
  
  if ((NULL == index) || (NULL == index->hash_fn) || (NULL == ic->compar_fn))
    return (NULL);

  if (index->index.size / sizeof (index->index.data[0]) <= ++index->count * MR_HASH_TABLE_SIZE_MULT)
    {
      if (EXIT_SUCCESS != mr_ic_hash_reindex (ic, key, context))
	return (NULL);
    }
  return (mr_ic_hash_add_inner (ic, key, context));
}

mr_ptr_t *
mr_ic_hash_find (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ic_hash_t * index = ic->ext.ptr;

  if ((NULL == index) || (NULL == index->hash_fn))
    return (NULL);
  else
    {
      unsigned int hash_value = index->hash_fn (key, context);
      int hash_size = index->index.size / sizeof (index->index.data[0]);
      if (hash_size <= 0)
	return (NULL);
      return (mr_tfind (key, &index->index.data[hash_value % hash_size].root, ic->compar_fn, context));
    }
}

int
mr_ic_hash_new (mr_ic_t * ic, mr_hash_fn_t hash_fn, mr_compar_fn_t compar_fn, char * key_type)
{
  mr_ic_hash_t * index;

  if ((NULL == ic) || (NULL == compar_fn) || (NULL == hash_fn))
    return (EXIT_FAILURE);
  
  index = MR_MALLOC (sizeof (*index));
  if (NULL == index)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (EXIT_FAILURE);
    }
  
  memset (index, 0, sizeof (*index));
  index->hash_fn = hash_fn;
  
  ic->ic_type = MR_IC_HASH;
  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->add = mr_ic_hash_add;
  ic->find = mr_ic_hash_find;
  ic->index = mr_ic_hash_index;
  ic->free = mr_ic_hash_free;
  ic->ext.ptr = index;

  return (EXIT_SUCCESS);
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
  return (mr_tsearch (key, (mr_red_black_tree_node_t**)&ic->ext.ptr, ic->compar_fn, context));
}

mr_ptr_t *
mr_ic_rbtree_find (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  return (mr_tfind (key, (mr_red_black_tree_node_t**)&ic->ext.ptr, ic->compar_fn, context));
}

int
mr_ic_rbtree_index (mr_ic_t * ic, mr_ic_rarray_t * rarray, const void * context)
{
  int i;
  mr_ic_rbtree_free (ic, context);
  for (i = rarray->ra.size / sizeof (rarray->ra.data[0]) - 1; i >= 0; --i)
    if (NULL == mr_ic_rbtree_add (ic, rarray->ra.data[i], context))
      return (EXIT_FAILURE);
  return (EXIT_SUCCESS);
}

int
mr_ic_rbtree_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type)
{
  if ((NULL == ic) || (NULL == compar_fn))
    return (EXIT_FAILURE);

  ic->ic_type = MR_IC_RBTREE;
  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->add = mr_ic_rbtree_add;
  ic->find = mr_ic_rbtree_find;
  ic->index = mr_ic_rbtree_index;
  ic->free = mr_ic_rbtree_free;
  ic->ext.ptr = NULL;

  return (EXIT_SUCCESS);
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

int
mr_ic_index (mr_ic_t * ic, mr_ic_rarray_t * rarray, const void * context)
{
  int i, count;
  if ((NULL == ic) || (NULL == rarray))
    return (EXIT_FAILURE);
  if (ic->index)
    return (ic->index (ic, rarray, context));
  count = rarray->ra.size / sizeof (rarray->ra.data[0]);
  for (i = 0; i < count; ++i)
    if (NULL == mr_ic_add (ic, rarray->ra.data[i], context))
      return (EXIT_FAILURE);
  return (EXIT_SUCCESS);
}

void
mr_ic_free (mr_ic_t * ic, const void * context)
{
  if (NULL == ic)
    return;
  if (ic->free)
    ic->free (ic, context);
}
