/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */
#ifndef _MR_IC_H_
#define _MR_IC_H_

#include <metaresc.h>

extern mr_ptr_t * mr_ic_add (mr_ic_t *, mr_ptr_t, const void *);
extern mr_ptr_t * mr_ic_find (mr_ic_t *, mr_ptr_t, const void *);
extern int mr_ic_index (mr_ic_t *, mr_ic_rarray_t *, const void *);
extern void mr_ic_free (mr_ic_t *, const void *);

extern int mr_ic_none_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char *key_type);
extern int mr_ic_rbtree_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char *key_type, void * context);
extern int mr_ic_hash_new (mr_ic_t * ic, mr_hash_fn_t hash_fn, mr_compar_fn_t compar_fn, char * key_type, void * context);

extern mr_ptr_t * mr_ic_none_find (mr_ic_t * ic, mr_ptr_t key, const void * context);
extern int mr_ic_none_index (mr_ic_t * ic, mr_ic_rarray_t * rarray, const void * context);
extern mr_ptr_t * mr_ic_hash_add (mr_ic_t * ic, mr_ptr_t key, const void * context);
extern mr_ptr_t * mr_ic_hash_find (mr_ic_t * ic, mr_ptr_t key, const void * context);
extern int mr_ic_hash_index (mr_ic_t * ic, mr_ic_rarray_t * rarray, const void * context);
extern void mr_ic_hash_free (mr_ic_t * ic, const void * context);
extern void mr_ic_rbtree_free (mr_ic_t * ic, const void * context);
extern mr_ptr_t * mr_ic_rbtree_add (mr_ic_t * ic, mr_ptr_t key, const void * context);
extern mr_ptr_t * mr_ic_rbtree_find (mr_ic_t * ic, mr_ptr_t key, const void * context);

#endif /* _MR_IC_H_ */
