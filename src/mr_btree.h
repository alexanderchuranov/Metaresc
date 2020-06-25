/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */
#ifndef _MR_BTREE_H_
#define _MR_BTREE_H_

#include <metaresc.h>

#define MR_PATH_SIZE (sizeof (mr_tree_node_idx_t) * __CHAR_BIT__ << 1)

extern void mr_tree_init (mr_tree_t * tree);
extern void mr_tree_free (mr_tree_t * tree);
extern mr_status_t mr_tree_walk (mr_tree_t * tree, mr_visit_fn_t visit_fn, const void * context);
extern mr_tree_find_res_t mr_tree_find (mr_ptr_t key, mr_tree_t * tree, mr_compar_fn_t compar_fn, void * context, mr_tree_path_t * path);

extern mr_ptr_t * mr_rbtree_add (mr_ptr_t key, mr_tree_t * tree, mr_compar_fn_t compar_fn, void * context);
extern mr_status_t mr_rbtree_del (mr_ptr_t key, mr_tree_t * tree, mr_compar_fn_t compar_fn, void * context);
extern bool mr_rbtree_is_valid (mr_tree_t * tree, mr_compar_fn_t cmp, void * context);

extern mr_ptr_t * mr_avltree_add (mr_ptr_t key, mr_tree_t * tree, mr_compar_fn_t compar_fn, void * context);
extern mr_status_t mr_avltree_del (mr_ptr_t key, mr_tree_t * tree, mr_compar_fn_t compar_fn, void * context);
extern bool mr_avltree_is_valid (mr_tree_t * tree, mr_compar_fn_t cmp, void * context);

#endif /* _MR_BTREE_H_ */
