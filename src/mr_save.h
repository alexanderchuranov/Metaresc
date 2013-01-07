#ifndef _MR_SAVE_H_
#define _MR_SAVE_H_

#include <metaresc.h>

TYPEDEF_FUNC (int, mr_ptrdes_processor_t, (mr_ra_mr_ptrdes_t * /* ptrs */, int /* idx */, void * /* context */))

extern int mr_ptrs_ds (mr_ra_mr_ptrdes_t * ptrs, mr_ptrdes_processor_t processor, void * context);
extern int mr_renumber_node (mr_ra_mr_ptrdes_t * ptrs, int idx, void * );

#endif /* _MR_SAVE_H_ */
