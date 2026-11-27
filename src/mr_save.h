#ifndef _MR_SAVE_H_
#define _MR_SAVE_H_

#include <metaresc.h>

#define MR_ONE_SHIFT(SHIFT) | (UINT64_C (1) << (SHIFT))
#define MR_STRUCT_TYPES (0 MR_FOREACH (MR_ONE_SHIFT, MR_TYPE_STRUCT, MR_TYPE_UNION, MR_TYPE_ANON_UNION, MR_TYPE_NAMED_ANON_UNION))
#define MR_UNION_TYPES (0 MR_FOREACH (MR_ONE_SHIFT, MR_TYPE_ANON_UNION, MR_TYPE_NAMED_ANON_UNION, MR_TYPE_UNION))
#define MR_TYPED_TYPES (0 MR_FOREACH (MR_ONE_SHIFT, MR_TYPE_STRUCT, MR_TYPE_ENUM, MR_TYPE_UNION, MR_TYPE_ANON_UNION, MR_TYPE_NAMED_ANON_UNION))

extern void mr_assign_int (mr_ptrdes_t * dst, mr_ptrdes_t * src);
extern bool mr_is_opaque_data (mr_ptrdes_t * ptrdes);
extern void mr_pointer_get_size_ptrdes (mr_ptrdes_t * ptrdes, mr_idx_t idx, mr_ra_ptrdes_t * ptrs);
extern mr_fd_t * mr_union_discriminator (mr_save_data_t * mr_save_data, mr_idx_t node, mr_fd_t * union_fdp);
extern void mr_ud_free (mr_ptrdes_t * ptrdes);
extern int mr_uds_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context);
extern mr_hash_value_t mr_uds_get_hash (mr_ptr_t x, const void * context);

#endif /* _MR_SAVE_H_ */
