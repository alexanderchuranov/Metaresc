#ifndef _MR_SAVE_H_
#define _MR_SAVE_H_

#include <metaresc.h>

#define MR_ONE_SHIFT(SHIFT) | (1UL << (SHIFT))

extern mr_status_t mr_renumber_node (mr_ra_ptrdes_t * ptrs, int idx, int level, mr_dfs_order_t order, void * context);
extern void mr_assign_int (mr_ptrdes_t * dst, mr_ptrdes_t * src);
extern void mr_pointer_get_size_ptrdes (mr_ptrdes_t * ptrdes, int idx, mr_ra_ptrdes_t * ptrs);

#endif /* _MR_SAVE_H_ */
