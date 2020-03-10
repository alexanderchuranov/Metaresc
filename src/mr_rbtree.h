/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */
#ifndef _MR_RBTREE_H_
#define _MR_RBTREE_H_

#include <metaresc.h>

extern void mr_rbtree_init (mr_rbtree_t * rbtree);
extern void mr_rbtree_free (mr_rbtree_t * rbtree);
extern unsigned mr_rbtree_find (mr_ptr_t key, mr_rbtree_t * rbtree, mr_compar_fn_t compar_fn, void * context, int * parent_cmp);
extern mr_status_t mr_rbtree_del (mr_ptr_t key, mr_rbtree_t * rbtree, mr_compar_fn_t compar_fn, void * context);
extern mr_ptr_t * mr_rbtree_add (mr_ptr_t key, mr_rbtree_t * rbtree, mr_compar_fn_t compar_fn, void * context);
extern bool mr_rbtree_is_valid (mr_rbtree_t * rbtree, mr_compar_fn_t cmp, void * context);

#endif /* _MR_RBTREE_H_ */
