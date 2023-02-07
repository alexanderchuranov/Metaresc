/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */
#ifndef _MR_BTREE_H_
#define _MR_BTREE_H_

#include <metaresc.h>

extern void mr_tree_init (mr_tree_t * tree);
extern void mr_tree_free (mr_tree_t * tree);
extern mr_status_t mr_tree_reserve (mr_tree_t * tree, unsigned items_count, bool reset);
extern mr_status_t mr_tree_walk (mr_tree_t * tree, mr_visit_fn_t visit_fn, const void * context);
extern void mr_tree_find (mr_ptr_t key, mr_tree_t * tree, mr_compar_fn_t compar_fn, void * context, mr_tree_traverse_t * traverse);
extern void mr_tree_next (mr_tree_t * tree, mr_tree_traverse_t * traverse, mr_child_idx_t next);

extern mr_ptr_t * mr_rbtree_add (mr_ptr_t key, mr_tree_t * tree, mr_compar_fn_t compar_fn, void * context);
extern mr_status_t mr_rbtree_del (mr_ptr_t key, mr_tree_t * tree, mr_compar_fn_t compar_fn, void * context);
extern bool mr_rbtree_is_valid (mr_tree_t * tree, mr_compar_fn_t cmp, void * context);

extern mr_ptr_t * mr_avltree_add (mr_ptr_t key, mr_tree_t * tree, mr_compar_fn_t compar_fn, void * context);
extern mr_status_t mr_avltree_del (mr_ptr_t key, mr_tree_t * tree, mr_compar_fn_t compar_fn, void * context);
extern bool mr_avltree_is_valid (mr_tree_t * tree, mr_compar_fn_t cmp, void * context);

#endif /* _MR_BTREE_H_ */
