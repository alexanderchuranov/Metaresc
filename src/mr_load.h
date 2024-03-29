#ifndef _MR_LOAD_H_
#define _MR_LOAD_H_

#include <metaresc.h>

extern mr_status_t mr_load_integer (int, mr_ra_ptrdes_t * ptrs);
extern mr_status_t mr_load_func (int, mr_ra_ptrdes_t * ptrs);
extern void mr_pointer_set_size (int idx, mr_ra_ptrdes_t * ptrs);

#endif /* _MR_LOAD_H_ */
