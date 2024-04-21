#ifndef _MR_SAVE_H_
#define _MR_SAVE_H_

#include <metaresc.h>

#define MR_ONE_SHIFT(SHIFT) | (1ULL << (SHIFT))
#define MR_STRUCT_TYPES (0 MR_FOREACH (MR_ONE_SHIFT, MR_TYPE_STRUCT, MR_TYPE_UNION, MR_TYPE_ANON_UNION, MR_TYPE_NAMED_ANON_UNION))
#define MR_TYPED_TYPES (0 MR_FOREACH (MR_ONE_SHIFT, MR_TYPE_STRUCT, MR_TYPE_ENUM, MR_TYPE_UNION, MR_TYPE_ANON_UNION, MR_TYPE_NAMED_ANON_UNION))

extern void mr_assign_int (mr_ptrdes_t * dst, mr_ptrdes_t * src);
extern void mr_pointer_get_size_ptrdes (mr_ptrdes_t * ptrdes, mr_idx_t idx, mr_ra_ptrdes_t * ptrs);

#endif /* _MR_SAVE_H_ */
