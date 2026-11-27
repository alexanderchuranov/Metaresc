/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */
#ifndef _MR_IC_H_
#define _MR_IC_H_

#include <metaresc.h>

extern mr_ptr_t * mr_ic_unsorted_array_add (mr_ic_t * ic, mr_ptr_t key);
extern mr_status_t mr_ic_unsorted_array_del (mr_ic_t * ic, mr_ptr_t key);
extern mr_ptr_t * mr_ic_unsorted_array_find (mr_ic_t * ic, mr_ptr_t key);
extern mr_status_t mr_ic_unsorted_array_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, void * context);
extern mr_status_t mr_ic_unsorted_array_index (mr_ic_t * ic, mr_ptr_t * rarray, size_t size);
extern void mr_ic_unsorted_array_free (mr_ic_t * ic);
extern mr_status_t mr_ic_unsorted_array_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type, mr_res_t * context);

extern int mr_ic_sorted_array_find_idx (mr_ptr_t key, mr_ic_rarray_t * rarray, mr_compar_fn_t compar_fn, void * context, unsigned * idx);
extern mr_ptr_t * mr_ic_sorted_array_add (mr_ic_t * ic, mr_ptr_t key);
extern mr_status_t mr_ic_sorted_array_del (mr_ic_t * ic, mr_ptr_t key);
extern mr_ptr_t * mr_ic_sorted_array_find (mr_ic_t * ic, mr_ptr_t key);
extern mr_status_t mr_ic_sorted_array_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn);
extern mr_status_t mr_ic_sorted_array_index (mr_ic_t * ic, mr_ptr_t * rarray, size_t size);
extern void mr_ic_sorted_array_free (mr_ic_t * ic);
extern mr_status_t mr_ic_sorted_array_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type, mr_res_t * context);

extern mr_ptr_t * mr_ic_hash_add (mr_ic_t * ic, mr_ptr_t key);
extern mr_status_t mr_ic_hash_del (mr_ic_t * ic, mr_ptr_t key);
extern mr_ptr_t * mr_ic_hash_find (mr_ic_t * ic, mr_ptr_t key);
extern mr_status_t mr_ic_hash_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, void * context);
extern mr_status_t mr_ic_hash_index (mr_ic_t * ic, mr_ptr_t * rarray, size_t size);
extern void mr_ic_hash_free (mr_ic_t * ic);
extern mr_status_t mr_ic_hash_new (mr_ic_t * ic, mr_hash_fn_t hash_fn, mr_compar_fn_t compar_fn, char * key_type, mr_res_t * context);

extern mr_ptr_t * mr_ic_static_array_add (mr_ic_t * ic, mr_ptr_t key);
extern mr_status_t mr_ic_static_array_del (mr_ic_t * ic, mr_ptr_t key);
extern mr_ptr_t * mr_ic_static_array_find (mr_ic_t * ic, mr_ptr_t key);
extern mr_status_t mr_ic_static_array_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, void * context);
extern mr_status_t mr_ic_static_array_index (mr_ic_t * ic, mr_ptr_t * rarray, size_t size);
extern void mr_ic_static_array_free (mr_ic_t * ic);
extern mr_status_t mr_ic_static_array_new (mr_ic_t * ic, mr_hash_fn_t hash_fn, mr_compar_fn_t compar_fn, char * key_type, mr_res_t * context);

extern mr_ptr_t * mr_ic_rbtree_add (mr_ic_t * ic, mr_ptr_t key);
extern mr_status_t mr_ic_rbtree_del (mr_ic_t * ic, mr_ptr_t key);
extern mr_ptr_t * mr_ic_rbtree_find (mr_ic_t * ic, mr_ptr_t key);
extern mr_status_t mr_ic_rbtree_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, void * context);
extern mr_status_t mr_ic_rbtree_index (mr_ic_t * ic, mr_ptr_t * rarray, size_t size);
extern void mr_ic_rbtree_free (mr_ic_t * ic);
extern mr_status_t mr_ic_rbtree_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type, mr_res_t * context);

extern mr_ptr_t * mr_ic_avltree_add (mr_ic_t * ic, mr_ptr_t key);
extern mr_status_t mr_ic_avltree_del (mr_ic_t * ic, mr_ptr_t key);
extern mr_ptr_t * mr_ic_avltree_find (mr_ic_t * ic, mr_ptr_t key);
extern mr_status_t mr_ic_avltree_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, void * context);
extern mr_status_t mr_ic_avltree_index (mr_ic_t * ic, mr_ptr_t * rarray, size_t size);
extern void mr_ic_avltree_free (mr_ic_t * ic);
extern mr_status_t mr_ic_avltree_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type, mr_res_t * context);

/* ----------------------- MR_IC_* ----------------------- */

/**
 * Deletes a key from an indexed collection.
 * @param ic indexed collection
 * @param key key to delete
 * @return status, failure if the key is not found
 */
static inline mr_status_t
mr_ic_del (mr_ic_t * ic, mr_ptr_t key)
{
  if ((NULL == ic) || (NULL == ic->virt_func) || (NULL == ic->virt_func->del))
    return (MR_FAILURE);
  return (ic->virt_func->del (ic, key));
}

/**
 * Finds a key in an indexed collection.
 * @param ic indexed collection
 * @param key key to look for
 * @return pointer on the found key or NULL
 */
static inline mr_ptr_t *
mr_ic_find (mr_ic_t * ic, mr_ptr_t key)
{
  if ((NULL == ic) || (NULL == ic->virt_func) || (NULL == ic->virt_func->find))
    return (NULL);
  return (ic->virt_func->find (ic, key));
}

/**
 * Adds a key to an indexed collection, unless an equal key is already there.
 * @param ic indexed collection
 * @param key key to add
 * @return pointer on the added or already present key, NULL on error
 */
static inline mr_ptr_t *
mr_ic_add (mr_ic_t * ic, mr_ptr_t key)
{
  if ((NULL == ic) || (NULL == ic->virt_func) || (NULL == ic->virt_func->add))
    return (NULL);
  return (ic->virt_func->add (ic, key));
}

/**
 * Calls a function for each key of an indexed collection.
 * @param ic indexed collection
 * @param visit_fn function to call
 * @param context context for the function
 * @return status, failure if a call failed
 */
static inline mr_status_t
mr_ic_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, void * context)
{
  if ((NULL == ic) || (NULL == ic->virt_func) || (NULL == ic->virt_func->foreach) || (NULL == visit_fn))
    return (MR_FAILURE);
  return (ic->virt_func->foreach (ic, visit_fn, context));
}

/**
 * Frees the memory of an indexed collection. The keys are not freed.
 * @param ic indexed collection, may be NULL
 */
static inline void
mr_ic_free (mr_ic_t * ic)
{
  if ((NULL == ic) || (NULL == ic->virt_func) || (NULL == ic->virt_func->free))
    return;
  ic->virt_func->free (ic);
}

/**
 * Fills an indexed collection from an array of keys.
 * @param ic indexed collection
 * @param rarray array of keys
 * @param size size of the array in bytes
 * @return status
 */
static inline mr_status_t
mr_ic_index (mr_ic_t * ic, mr_ptr_t * rarray, size_t size)
{
  if ((NULL == ic) || (NULL == ic->virt_func) || (NULL == ic->virt_func->index))
    return (MR_FAILURE);
  return (ic->virt_func->index (ic, rarray, size));
}

/**
 * Initializes an indexed collection of a given kind. Without hash and
 * comparison functions and context the generic functions of the key type are
 * used.
 * @param ic indexed collection
 * @param hash_fn hash function, used by hash tables only
 * @param compar_fn comparison function
 * @param key_type type name of the keys
 * @param mr_ic_type kind of the collection
 * @param context context for the hash and comparison functions, may be NULL
 * @return status
 */
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
    case MR_IC_HASH:
      return (mr_ic_hash_new (ic, hash_fn, compar_fn, key_type, context));
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
