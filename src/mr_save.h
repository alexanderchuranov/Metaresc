#ifndef _MR_SAVE_H_
#define _MR_SAVE_H_

#include <metaresc.h>

extern mr_status_t mr_renumber_node (mr_ra_ptrdes_t * ptrs, int idx, int level, mr_dfs_order_t order, void * context);
extern void mr_pointer_get_size (ssize_t * size, char * name, int idx, mr_ra_ptrdes_t * ptrs);

#endif /* _MR_SAVE_H_ */
