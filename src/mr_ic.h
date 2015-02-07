/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */
#ifndef _MR_IC_H_
#define _MR_IC_H_

#include <metaresc.h>

extern mr_ptr_t * mr_ic_none_add (mr_ic_t * ic, mr_ptr_t key, const void * context);
extern mr_status_t mr_ic_none_del (mr_ic_t * ic, mr_ptr_t key, const void * context);
extern mr_ptr_t * mr_ic_none_find (mr_ic_t * ic, mr_ptr_t key, const void * context);
extern mr_status_t mr_ic_none_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context);
extern mr_status_t mr_ic_none_index (mr_ic_t * ic, mr_ic_rarray_t * rarray, const void * context);
extern void mr_ic_none_reset (mr_ic_t * ic);
extern void mr_ic_none_free (mr_ic_t * ic);
extern mr_status_t mr_ic_none_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type);

extern mr_ptr_t * mr_ic_rbtree_add (mr_ic_t * ic, mr_ptr_t key, const void * context);
extern mr_status_t mr_ic_rbtree_del (mr_ic_t * ic, mr_ptr_t key, const void * context);
extern mr_ptr_t * mr_ic_rbtree_find (mr_ic_t * ic, mr_ptr_t key, const void * context);
extern mr_status_t mr_ic_rbtree_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context);
extern mr_status_t mr_ic_rbtree_index (mr_ic_t * ic, mr_ic_rarray_t * rarray, const void * context);
extern void mr_ic_rbtree_reset (mr_ic_t * ic);
extern void mr_ic_rbtree_free (mr_ic_t * ic);
extern mr_status_t mr_ic_rbtree_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type);

extern mr_ptr_t * mr_ic_sorted_array_add (mr_ic_t * ic, mr_ptr_t key, const void * context);
extern mr_status_t mr_ic_sorted_array_del (mr_ic_t * ic, mr_ptr_t key, const void * context);
extern mr_ptr_t * mr_ic_sorted_array_find (mr_ic_t * ic, mr_ptr_t key, const void * context);
extern mr_status_t mr_ic_sorted_array_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context);
extern mr_status_t mr_ic_sorted_array_index (mr_ic_t * ic, mr_ic_rarray_t * rarray, const void * context);
extern void mr_ic_sorted_array_reset (mr_ic_t * ic);
extern void mr_ic_sorted_array_free (mr_ic_t * ic);
extern mr_status_t mr_ic_sorted_array_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type);

extern mr_ptr_t * mr_ic_hash_tree_index_add (mr_ic_t * ic, mr_ptr_t key, const void * context, int bucket);
extern void mr_ic_hash_tree_index_free (mr_ic_t * ic);
extern mr_ptr_t * mr_ic_hash_add (mr_ic_t * ic, mr_ptr_t key, const void * context);
extern mr_status_t mr_ic_hash_tree_del (mr_ic_t * ic, mr_ptr_t key, const void * context);
extern mr_ptr_t * mr_ic_hash_tree_find (mr_ic_t * ic, mr_ptr_t key, const void * context);
extern mr_status_t mr_ic_hash_tree_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context);
extern mr_status_t mr_ic_hash_index (mr_ic_t * ic, mr_ic_rarray_t * rarray, const void * context);
extern void mr_ic_hash_reset (mr_ic_t * ic);
extern void mr_ic_hash_free (mr_ic_t * ic);
extern mr_status_t mr_ic_hash_tree_new (mr_ic_t * ic, mr_hash_fn_t hash_fn, mr_compar_fn_t compar_fn, char * key_type);

extern mr_ptr_t * mr_ic_hash_next_index_add (mr_ic_t * ic, mr_ptr_t key, const void * context, int bucket);
extern void mr_ic_hash_next_index_free (mr_ic_t * ic);
extern mr_status_t mr_ic_hash_next_del (mr_ic_t * ic, mr_ptr_t key, const void * context);
extern mr_ptr_t * mr_ic_hash_next_find (mr_ic_t * ic, mr_ptr_t key, const void * context);
extern mr_status_t mr_ic_hash_next_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context);
extern mr_status_t mr_ic_hash_next_new (mr_ic_t * ic, mr_hash_fn_t hash_fn, mr_compar_fn_t compar_fn, char * key_type);

/* ----------------------- MR_IC_* ----------------------- */

static inline mr_status_t
mr_ic_del (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  if (NULL == ic)
    return (MR_FAILURE);
  if (ic->del)
    return (ic->del (ic, key, context));
  return (MR_FAILURE);
}

static inline mr_ptr_t *
mr_ic_find (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  if (NULL == ic)
    return (NULL);
  if (ic->find)
    return (ic->find (ic, key, context));
  return (NULL);
}

static inline mr_ptr_t *
mr_ic_add (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  if (NULL == ic)
    return (NULL);
  if (ic->add)
    return (ic->add (ic, key, context));
  return (NULL);
}

static inline mr_status_t
mr_ic_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context)
{
  if ((NULL == ic) || (NULL == visit_fn))
    return (MR_FAILURE);
  if (ic->foreach)
    return (ic->foreach (ic, visit_fn, context));
  return (MR_FAILURE);
}

static inline mr_status_t
mr_ic_index (mr_ic_t * ic, mr_ic_rarray_t * rarray, const void * context)
{
  int i, count;
  if ((NULL == ic) || (NULL == rarray))
    return (MR_FAILURE);
  if (ic->index)
    return (ic->index (ic, rarray, context));
  count = rarray->MR_SIZE / sizeof (rarray->ra[0]);
  for (i = 0; i < count; ++i)
    if (NULL == mr_ic_add (ic, rarray->ra[i], context))
      return (MR_FAILURE);
  return (MR_SUCCESS);
}

static inline void
mr_ic_reset (mr_ic_t * ic)
{
  if (NULL == ic)
    return;
  if (ic->reset)
    ic->reset (ic);
}

static inline void
mr_ic_free (mr_ic_t * ic)
{
  if (NULL == ic)
    return;
  if (ic->free)
    ic->free (ic);
}

static inline mr_status_t
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

#endif /* _MR_IC_H_ */
