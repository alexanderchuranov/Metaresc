#include <metaresc.h>
#include <mr_tsearch.h>
#include <mr_ic.h>

mr_ptr_t *
mr_ic_none_find (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  int i, count = ic->collection.size / sizeof (ic->collection.data[0]);
  for (i = 0; i < count; ++i)
    if (0 == ic->compar_fn (key, ic->collection.data[i], context))
      return (&ic->collection.data[i]);
  return (NULL);
}

int
mr_ic_none_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type)
{
  if ((NULL == ic) || (NULL == compar_fn))
    return (!0);
  ic->ic_type = MR_IC_NONE;
  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->index = NULL;
  ic->add = NULL;
  ic->find = mr_ic_none_find;
  ic->free = NULL;
  ic->ext.ptr = NULL;
  return (0);
}

#define MR_HASH_TABLE_SIZE_MULT (2)

static void
dummy_free_fn (mr_ptr_t key, const void * context)
{
}

static inline void
mr_ic_hash_free_inner (mr_ic_hash_t * index, const void * context)
{
  if (index->index.data)
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
  return (mr_tsearch (key, &index->index.data[hash_value % hash_size].root, ic->compar_fn, context));
}

int
mr_ic_hash_index (mr_ic_t * ic, const void * context)
{
  mr_ic_hash_t * index = ic->ext.ptr;
  int i, count = ic->collection.size / sizeof (ic->collection.data[0]);
  
  if ((NULL == index) || (NULL == index->hash_fn) || (NULL == ic->compar_fn))
    return (!0);

  mr_ic_hash_free_inner (index, context);
  
  index->index.size = 2 * count * MR_HASH_TABLE_SIZE_MULT * sizeof (index->index.data[0]);
  index->index.data = MR_MALLOC (index->index.size);
  if (NULL == index->index.data)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (!0);
    }
  memset (index->index.data, 0, index->index.size);

  for (i = 0; i < count; ++i)
    if (NULL == mr_ic_hash_add_inner (ic, ic->collection.data[i], context))
      {
	MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	return (!0);
      }
  
  return (0);
}

mr_ptr_t *
mr_ic_hash_add (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ic_hash_t * index = ic->ext.ptr;
  
  if ((NULL == index) || (NULL == index->hash_fn) || (NULL == ic->compar_fn))
    return (NULL);
  
  if (index->index.size <= ic->collection.size * MR_HASH_TABLE_SIZE_MULT)
    mr_ic_hash_index (ic, context);
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
      return (mr_tfind (key, &index->index.data[hash_value % hash_size].root, ic->compar_fn, context));
    }
}

int
mr_ic_hash_new (mr_ic_t * ic, mr_hash_fn_t hash_fn, mr_compar_fn_t compar_fn, char * key_type, void * context)
{
  mr_ic_hash_t * index;
  if ((NULL == ic) || (NULL == compar_fn) || (NULL == hash_fn))
    return (!0);
  index = MR_MALLOC (sizeof (*index));
  if (NULL == index)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (!0);
    }
  memset (index, 0, sizeof (*index));
  index->hash_fn = hash_fn;
  
  ic->ic_type = MR_IC_HASH;
  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->index = mr_ic_hash_index;
  ic->add = mr_ic_hash_add;
  ic->find = mr_ic_hash_find;
  ic->free = mr_ic_hash_free;
  ic->ext.ptr = index;

  return (mr_ic_hash_index (ic, context));
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

int
mr_ic_rbtree_index (mr_ic_t * ic, const void * context)
{
  int i, count = ic->collection.size / sizeof (ic->collection.data[0]);

  mr_ic_rbtree_free (ic, context);

  for (i = 0; i < count; ++i)
    if (NULL == mr_ic_rbtree_add (ic, ic->collection.data[i], context))
      {
	MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	return (!0);
      }
  
  return (0);
}

mr_ptr_t *
mr_ic_rbtree_find (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  return (mr_tfind (key, (mr_red_black_tree_node_t**)&ic->ext.ptr, ic->compar_fn, context));
}

int
mr_ic_rbtree_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type, void * context)
{
  if ((NULL == ic) || (NULL == compar_fn))
    return (!0);

  ic->ic_type = MR_IC_RBTREE;
  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->index = mr_ic_rbtree_index;
  ic->add = mr_ic_rbtree_add;
  ic->find = mr_ic_rbtree_find;
  ic->free = mr_ic_rbtree_free;
  ic->ext.ptr = NULL;

  return (mr_ic_rbtree_index (ic, context));
}

/**
 * Type descriptors collection iterator.
 * @param ic indexed collection
 * @param visit_fn function for type descriptors processing
 * @param context auxiliary arguments
 * @return flag that cycle was not completed
 */
int
mr_ic_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context)
{
  int count, i;
  if (NULL == ic)
    return (!0);
  
  count = ic->collection.size / sizeof (ic->collection.data[0]);
  for (i = 0; i < count; ++i)
    if (visit_fn (ic->collection.data[i], context))
      return (!0);
  return (0);
}

mr_ptr_t *
mr_ic_add (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ptr_t * new_element;
  if (NULL == ic)
    return (NULL);
  
  new_element = mr_rarray_append ((mr_rarray_t*)&ic->collection, sizeof (ic->collection.data[0]));
  if (NULL == new_element)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (NULL);
    }
  *new_element = key;
  
  if (ic->add)
    return (ic->add (ic, key, context));
  return (new_element);
}

int
mr_ic_index (mr_ic_t * ic, const void * context)
{
  if (NULL == ic)
    return (!0);
  if (ic->index)
    return (ic->index (ic, context));
  return (0);
}

mr_ptr_t *
mr_ic_find (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  if ((NULL == ic) || (NULL == ic->compar_fn))
    return (NULL);
  if (ic->find)
    return (ic->find (ic, key, context));
  return (mr_ic_none_find (ic, key, context));
}

void
mr_ic_free (mr_ic_t * ic, const void * context)
{
  if (NULL == ic)
    return;
  if (ic->free)
    ic->free (ic, context);
}

