#ifndef _MR_LOAD_H_
#define _MR_LOAD_H_

#include <metaresc.h>

extern mr_status_t mr_load_integer (mr_idx_t, mr_ra_ptrdes_t *);
extern mr_status_t mr_load_func (mr_idx_t, mr_ra_ptrdes_t *);
extern void mr_pointer_set_size (mr_idx_t, mr_ra_ptrdes_t *);

#endif /* _MR_LOAD_H_ */
