/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */
#ifndef _MR_IC_H_
#define _MR_IC_H_

#include <metaresc.h>

extern mr_ptr_t * mr_ic_add (mr_ic_t *, mr_ptr_t, const void *);
extern mr_ptr_t * mr_ic_find (mr_ic_t *, mr_ptr_t, const void *);
extern mr_status_t mr_ic_foreach (mr_ic_t *, mr_visit_fn_t, const void *);
extern mr_status_t mr_ic_index (mr_ic_t *, mr_ic_rarray_t *, const void *);
extern void mr_ic_free (mr_ic_t *, const void *);
extern mr_status_t mr_ic_new (mr_ic_t * ic, mr_hash_fn_t hash_fn, mr_compar_fn_t compar_fn, char * key_type, mr_ic_type_t mr_ic_type);

extern mr_ptr_t * mr_ic_none_add (mr_ic_t * ic, mr_ptr_t key, const void * context);
extern mr_ptr_t * mr_ic_none_find (mr_ic_t * ic, mr_ptr_t key, const void * context);
extern mr_status_t mr_ic_none_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context);
extern mr_status_t mr_ic_none_index (mr_ic_t * ic, mr_ic_rarray_t * rarray, const void * context);
extern void mr_ic_none_free (mr_ic_t * ic, const void * context);
extern mr_status_t mr_ic_none_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type);

extern mr_ptr_t * mr_ic_rbtree_add (mr_ic_t * ic, mr_ptr_t key, const void * context);
extern mr_ptr_t * mr_ic_rbtree_find (mr_ic_t * ic, mr_ptr_t key, const void * context);
extern mr_status_t mr_ic_rbtree_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context);
extern mr_status_t mr_ic_rbtree_index (mr_ic_t * ic, mr_ic_rarray_t * rarray, const void * context);
extern void mr_ic_rbtree_free (mr_ic_t * ic, const void * context);
extern mr_status_t mr_ic_rbtree_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type);

extern mr_ptr_t * mr_ic_sorted_array_add (mr_ic_t * ic, mr_ptr_t key, const void * context);
extern mr_ptr_t * mr_ic_sorted_array_find (mr_ic_t * ic, mr_ptr_t key, const void * context);
extern mr_status_t mr_ic_sorted_array_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context);
extern mr_status_t mr_ic_sorted_array_index (mr_ic_t * ic, mr_ic_rarray_t * rarray, const void * context);
extern void mr_ic_sorted_array_free (mr_ic_t * ic, const void * context);
extern mr_status_t mr_ic_sorted_array_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type);

extern mr_ptr_t * mr_ic_hash_tree_index_add (mr_ic_t * ic, mr_ptr_t key, const void * context, int bucket);
extern void mr_ic_hash_tree_index_free (mr_ic_t * ic, const void * context);
extern mr_ptr_t * mr_ic_hash_add (mr_ic_t * ic, mr_ptr_t key, const void * context);
extern mr_ptr_t * mr_ic_hash_tree_find (mr_ic_t * ic, mr_ptr_t key, const void * context);
extern mr_status_t mr_ic_hash_tree_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context);
extern mr_status_t mr_ic_hash_index (mr_ic_t * ic, mr_ic_rarray_t * rarray, const void * context);
extern void mr_ic_hash_free (mr_ic_t * ic, const void * context);
extern mr_status_t mr_ic_hash_tree_new (mr_ic_t * ic, mr_hash_fn_t hash_fn, mr_compar_fn_t compar_fn, char * key_type);

extern mr_ptr_t * mr_ic_hash_next_index_add (mr_ic_t * ic, mr_ptr_t key, const void * context, int bucket);
extern void mr_ic_hash_next_index_free (mr_ic_t * ic, const void * context);
extern mr_ptr_t * mr_ic_hash_add (mr_ic_t * ic, mr_ptr_t key, const void * context);
extern mr_ptr_t * mr_ic_hash_next_find (mr_ic_t * ic, mr_ptr_t key, const void * context);
extern mr_status_t mr_ic_hash_next_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context);
extern mr_status_t mr_ic_hash_index (mr_ic_t * ic, mr_ic_rarray_t * rarray, const void * context);
extern void mr_ic_hash_free (mr_ic_t * ic, const void * context);
extern mr_status_t mr_ic_hash_next_new (mr_ic_t * ic, mr_hash_fn_t hash_fn, mr_compar_fn_t compar_fn, char * key_type);

#endif /* _MR_IC_H_ */
