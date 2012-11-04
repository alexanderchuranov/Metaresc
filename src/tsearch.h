#ifndef _TSEARCH_H_
#define _TSEARCH_H_

/* For use with hsearch(3).  */
#ifndef MR_COMPAR_FN_T
# define MR_COMPAR_FN_T
typedef int (*mr_compar_fn_t) (__const void *, __const void *, __const void *);
#endif

/* The tsearch routines are very interesting. They make many
   assumptions about the compiler.  It assumes that the first field
   in node must be the "key" field, which points to the datum.
   Everything depends on that.  */
/* For tsearch */
typedef enum
{
  preorder,
  postorder,
  endorder,
  leaf
}
VISIT;

#ifndef __ACTION_FN_T
# define __ACTION_FN_T
typedef void (*__action_fn_t) (__const void *__nodep, VISIT __value,
			       int __level);
#endif

/* Search for an entry matching the given KEY in the tree pointed to
   by *ROOTP and insert a new element if not found.  */
extern void *tsearch (__const void *__key, void **__rootp,
		      mr_compar_fn_t __compar, __const void * __context);

/* Search for an entry matching the given KEY in the tree pointed to
   by *ROOTP.  If no matching entry is available return NULL.  */
extern void *tfind (__const void *__key, void *__const *__rootp,
		    mr_compar_fn_t __compar, __const void * __context);

/* Remove the element matching KEY from the tree pointed to by *ROOTP.  */
extern void *tdelete (__const void *__restrict __key,
		      void **__restrict __rootp,
		      mr_compar_fn_t __compar,
		      __const void * __context);

/* Walk through the whole tree and call the ACTION callback for every node
   or leaf.  */
extern void twalk (__const void *__root, __action_fn_t __action);

/* Callback type for function to free a tree node.  If the keys are atomic
   data this function should do nothing.  */
typedef void (*__free_fn_t) (void *__nodep);

/* Destroy the whole tree, call FREEFCT for each node or leaf.  */
extern void tdestroy (void *__root, __free_fn_t __freefct);

#define weak_alias(...)
#define __tsearch tsearch
#define __tfind tfind
#define __tdelete tdelete
#define __twalk twalk
#define __tdestroy tdestroy

/* On some platforms we can make internal function calls (i.e., calls of
   functions not exported) a bit faster by using a different calling
   convention.  */
#ifndef internal_function
#define internal_function	/* empty */
#endif

#include <metaresc.h>

#define node_t MR_TYPEDEF_PREFIX(mr_red_black_tree_node_t)
typedef struct node_t *node;

#endif /* _TSEARCH_H_ */
