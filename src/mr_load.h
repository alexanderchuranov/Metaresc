#ifndef _MR_LOAD_H_
#define _MR_LOAD_H_

#include <string.h>
#include <metaresc.h>

extern int mr_load_rarray_type (mr_fd_t *, int (*) (void *, mr_td_t *), void *);
extern int mr_load_enum (int, mr_load_data_t *);
extern int mr_load_bitfield (int, mr_load_data_t *);
extern int mr_load_func (int, mr_load_data_t *);


#endif /* _MR_LOAD_H_ */
