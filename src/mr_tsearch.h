#ifndef _MR_TSEARCH_H_
#define _MR_TSEARCH_H_

#include <metaresc.h>

#ifndef MR_COMPAR_FN_T
# define MR_COMPAR_FN_T
typedef int (*mr_compar_fn_t) (__const mr_ptr_t, __const mr_ptr_t, __const void *);
#endif

#ifndef MR_ACTION_FN_T
# define MR_ACTION_FN_T
typedef void (*mr_action_fn_t) (__const void *__nodep, mr_rb_visit_order_t __value,
				int __level);
#endif

/* Callback type for function to free a tree node.  If the keys are atomic
   data this function should do nothing.  */
#ifndef MR_FREE_FN_T
# define MR_FREE_FN_T
typedef void (*mr_free_fn_t) (void *__nodep);
#endif

/* Search for an entry matching the given KEY in the tree pointed to
   by *ROOTP and insert a new element if not found.  */
extern void *mr_tsearch (__const mr_ptr_t __key, mr_red_black_tree_node_t **__rootp,
			 mr_compar_fn_t __compar, __const void * __context);

/* Search for an entry matching the given KEY in the tree pointed to
   by *ROOTP.  If no matching entry is available return NULL.  */
extern void *mr_tfind (__const mr_ptr_t __key, mr_red_black_tree_node_t *__const *__rootp,
		       mr_compar_fn_t __compar, __const void * __context);

/* Remove the element matching KEY from the tree pointed to by *ROOTP.  */
extern void *mr_tdelete (__const mr_ptr_t __key,
			 mr_red_black_tree_node_t **__restrict __rootp,
			 mr_compar_fn_t __compar,
			 __const void * __context);

/* Walk through the whole tree and call the ACTION callback for every node
   or leaf.  */
extern void mr_twalk (__const mr_red_black_tree_node_t *__root, mr_action_fn_t __action);

/* Destroy the whole tree, call FREEFCT for each node or leaf.  */
extern void mr_tdestroy (mr_red_black_tree_node_t *__root, mr_free_fn_t __freefct);

#endif /* _MR_TSEARCH_H_ */
