#ifndef _MR_SAVE_H_
#define _MR_SAVE_H_

#include <metaresc.h>

TYPEDEF_ENUM (mr_dfs_order_t,
	      MR_DFS_PRE_ORDER,
	      MR_DFS_POST_ORDER,
	      );

TYPEDEF_FUNC (mr_status_t, mr_ptrdes_processor_t, (mr_ra_ptrdes_t * /* ptrs */, int /* idx */, int /* level */, mr_dfs_order_t /* order */, void * /* context */))

extern mr_status_t mr_ptrs_dfs (mr_ra_ptrdes_t * ptrs, mr_ptrdes_processor_t processor, void * context);
extern mr_status_t mr_renumber_node (mr_ra_ptrdes_t * ptrs, int idx, int level, mr_dfs_order_t order, void * context);
extern void mr_pointer_get_size (ssize_t * size, char * name, int idx, mr_ra_ptrdes_t * ptrs);

#endif /* _MR_SAVE_H_ */
