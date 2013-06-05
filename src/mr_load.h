#ifndef _MR_LOAD_H_
#define _MR_LOAD_H_

#include <metaresc.h>

extern mr_status_t mr_load_rarray_type (mr_fd_t *, mr_status_t (*) (mr_td_t *, void *), void *);
extern mr_status_t mr_load_integer (int, mr_load_data_t *);
extern mr_status_t mr_load_func (int, mr_load_data_t *);

#endif /* _MR_LOAD_H_ */
