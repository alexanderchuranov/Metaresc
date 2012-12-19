#ifndef _MR_LOAD_H_
#define _MR_LOAD_H_

#include <string.h>
#include <metaresc.h>

int mr_load_rarray_type (mr_fd_t *, int (*) (void *, mr_td_t *), void *);

#endif /* _MR_LOAD_H_ */
