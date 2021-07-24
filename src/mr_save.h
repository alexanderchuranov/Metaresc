#ifndef _MR_SAVE_H_
#define _MR_SAVE_H_

#include <metaresc.h>

#define MR_ONE_SHIFT(SHIFT) | (1UL << (SHIFT))

#define MR_NON_SERIALIZABLE			\
  (0 MR_FOREACH (MR_ONE_SHIFT,			\
		 MR_TYPE_VOID,			\
		 MR_TYPE_CHAR_ARRAY,		\
		 MR_TYPE_BITFIELD,		\
		 MR_TYPE_FUNC,			\
		 MR_TYPE_FUNC_TYPE,		\
		 MR_TYPE_ARRAY,			\
		 MR_TYPE_POINTER,		\
		 MR_TYPE_UNION,			\
		 MR_TYPE_ANON_UNION,		\
		 MR_TYPE_NAMED_ANON_UNION))

extern mr_status_t mr_renumber_node (mr_ra_ptrdes_t * ptrs, int idx, int level, mr_dfs_order_t order, void * context);
extern void mr_pointer_get_size (ssize_t * size, char * name, int idx, mr_ra_ptrdes_t * ptrs);

#endif /* _MR_SAVE_H_ */
