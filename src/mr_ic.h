/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */
#ifndef _MR_IC_H_
#define _MR_IC_H_

#include <metaresc.h>

extern mr_ptr_t * mr_ic_unsorted_array_add (mr_ic_t * ic, mr_ptr_t key);
extern mr_status_t mr_ic_unsorted_array_del (mr_ic_t * ic, mr_ptr_t key);
extern mr_ptr_t * mr_ic_unsorted_array_find (mr_ic_t * ic, mr_ptr_t key);
extern mr_status_t mr_ic_unsorted_array_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context);
extern mr_status_t mr_ic_unsorted_array_index (mr_ic_t * ic, mr_ic_rarray_t * rarray);
extern void mr_ic_unsorted_array_free (mr_ic_t * ic);
extern mr_status_t mr_ic_unsorted_array_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type, mr_res_t * context);

extern mr_ptr_t * mr_ic_sorted_array_add (mr_ic_t * ic, mr_ptr_t key);
extern mr_status_t mr_ic_sorted_array_del (mr_ic_t * ic, mr_ptr_t key);
extern mr_ptr_t * mr_ic_sorted_array_find (mr_ic_t * ic, mr_ptr_t key);
extern mr_status_t mr_ic_sorted_array_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn);
extern mr_status_t mr_ic_sorted_array_index (mr_ic_t * ic, mr_ic_rarray_t * rarray);
extern void mr_ic_sorted_array_free (mr_ic_t * ic);
extern mr_status_t mr_ic_sorted_array_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type, mr_res_t * context);

extern mr_ptr_t * mr_ic_hash_next_add (mr_ic_t * ic, mr_ptr_t key);
extern mr_status_t mr_ic_hash_next_del (mr_ic_t * ic, mr_ptr_t key);
extern mr_ptr_t * mr_ic_hash_next_find (mr_ic_t * ic, mr_ptr_t key);
extern mr_status_t mr_ic_hash_next_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context);
extern mr_status_t mr_ic_hash_next_index (mr_ic_t * ic, mr_ic_rarray_t * rarray);
extern void mr_ic_hash_next_free (mr_ic_t * ic);
extern mr_status_t mr_ic_hash_next_new (mr_ic_t * ic, mr_hash_fn_t hash_fn, mr_compar_fn_t compar_fn, char * key_type, mr_res_t * context);

extern mr_ptr_t * mr_ic_static_array_add (mr_ic_t * ic, mr_ptr_t key);
extern mr_status_t mr_ic_static_array_del (mr_ic_t * ic, mr_ptr_t key);
extern mr_ptr_t * mr_ic_static_array_find (mr_ic_t * ic, mr_ptr_t key);
extern mr_status_t mr_ic_static_array_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context);
extern mr_status_t mr_ic_static_array_index (mr_ic_t * ic, mr_ic_rarray_t * rarray);
extern void mr_ic_static_array_free (mr_ic_t * ic);
extern mr_status_t mr_ic_static_array_new (mr_ic_t * ic, mr_hash_fn_t hash_fn, mr_compar_fn_t compar_fn, char * key_type, mr_res_t * context);

extern mr_ptr_t * mr_ic_rbtree_add (mr_ic_t * ic, mr_ptr_t key);
extern mr_status_t mr_ic_rbtree_del (mr_ic_t * ic, mr_ptr_t key);
extern mr_ptr_t * mr_ic_rbtree_find (mr_ic_t * ic, mr_ptr_t key);
extern mr_status_t mr_ic_rbtree_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context);
extern mr_status_t mr_ic_rbtree_index (mr_ic_t * ic, mr_ic_rarray_t * rarray);
extern void mr_ic_rbtree_free (mr_ic_t * ic);
extern mr_status_t mr_ic_rbtree_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type, mr_res_t * context);

extern mr_ptr_t * mr_ic_avltree_add (mr_ic_t * ic, mr_ptr_t key);
extern mr_status_t mr_ic_avltree_del (mr_ic_t * ic, mr_ptr_t key);
extern mr_ptr_t * mr_ic_avltree_find (mr_ic_t * ic, mr_ptr_t key);
extern mr_status_t mr_ic_avltree_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context);
extern mr_status_t mr_ic_avltree_index (mr_ic_t * ic, mr_ic_rarray_t * rarray);
extern void mr_ic_avltree_free (mr_ic_t * ic);
extern mr_status_t mr_ic_avltree_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type, mr_res_t * context);

/* ----------------------- MR_IC_* ----------------------- */

static inline mr_status_t
mr_ic_del (mr_ic_t * ic, mr_ptr_t key)
{
  if ((NULL == ic) || (NULL == ic->virt_func) || (NULL == ic->virt_func->del))
    return (MR_FAILURE);
  return (ic->virt_func->del (ic, key));
}

static inline mr_ptr_t *
mr_ic_find (mr_ic_t * ic, mr_ptr_t key)
{
  if ((NULL == ic) || (NULL == ic->virt_func) || (NULL == ic->virt_func->find))
    return (NULL);
  return (ic->virt_func->find (ic, key));
}

static inline mr_ptr_t *
mr_ic_add (mr_ic_t * ic, mr_ptr_t key)
{
  if ((NULL == ic) || (NULL == ic->virt_func) || (NULL == ic->virt_func->add))
    return (NULL);
  return (ic->virt_func->add (ic, key));
}

static inline mr_status_t
mr_ic_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, __const void * context)
{
  if ((NULL == ic) || (NULL == ic->virt_func) || (NULL == ic->virt_func->foreach) || (NULL == visit_fn))
    return (MR_FAILURE);
  return (ic->virt_func->foreach (ic, visit_fn, context));
}

static inline void
mr_ic_free (mr_ic_t * ic)
{
  if ((NULL == ic) || (NULL == ic->virt_func) || (NULL == ic->virt_func->free))
    return;
  ic->virt_func->free (ic);
}

static inline mr_status_t
mr_ic_index (mr_ic_t * ic, mr_ic_rarray_t * rarray)
{
  if ((NULL == ic) || (NULL == ic->virt_func) || (NULL == ic->virt_func->index) || (NULL == rarray))
    return (MR_FAILURE);
  mr_ic_free (ic);
  return (ic->virt_func->index (ic, rarray));
}

static inline mr_status_t
mr_ic_new (mr_ic_t * ic, mr_hash_fn_t hash_fn, mr_compar_fn_t compar_fn, char * key_type, mr_ic_type_t mr_ic_type, mr_res_t * context)
{
  memset (ic, 0, sizeof (*ic));
  switch (mr_ic_type)
    {
    case MR_IC_UNSORTED_ARRAY:
      return (mr_ic_unsorted_array_new (ic, compar_fn, key_type, context));
    case MR_IC_SORTED_ARRAY:
      return (mr_ic_sorted_array_new (ic, compar_fn, key_type, context));
    case MR_IC_HASH_NEXT:
      return (mr_ic_hash_next_new (ic, hash_fn, compar_fn, key_type, context));
    case MR_IC_STATIC_ARRAY:
      return (mr_ic_static_array_new (ic, hash_fn, compar_fn, key_type, context));
    case MR_IC_RBTREE:
      return (mr_ic_rbtree_new (ic, compar_fn, key_type, context));
    case MR_IC_AVLTREE:
      return (mr_ic_avltree_new (ic, compar_fn, key_type, context));
    default:
      return (MR_FAILURE);
    }
}

#endif /* _MR_IC_H_ */
