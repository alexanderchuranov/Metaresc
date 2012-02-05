#ifndef _RL_LOAD_H_
#define _RL_LOAD_H_

#include <string.h>
#include <metaresc.h>

#define MR_LOAD_RARRAY(ACTION)						\
  ({									\
    mr_td_t * _tdp_ = mr_get_td_by_name ("mr_rarray_t");		\
    int _status_ = 0;							\
    if (NULL == _tdp_)							\
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_NO_TYPE_DESCRIPTOR, "mr_rarray_t"); \
    else								\
      {									\
	mr_td_t _td_ = *_tdp_;						\
	int fields_count = _td_.fields.size / sizeof (_td_.fields.data[0]); \
	int lookup_by_name_count = _td_.lookup_by_name.size / sizeof (_td_.lookup_by_name.data[0]); \
	mr_fd_t fields[fields_count];					\
	mr_fd_ptr_t lookup_by_name[lookup_by_name_count];		\
	int _i_;							\
	memcpy (fields, _td_.fields.data, _td_.fields.size);		\
	memset (lookup_by_name, 0, _td_.lookup_by_name.size);		\
	for (_i_ = 0; _i_ < fields_count; ++_i_)			\
	  lookup_by_name[fields[_i_].hash_value % lookup_by_name_count].fdp = &fields[_i_]; \
	_td_.lookup_by_name.data = lookup_by_name;			\
	_td_.fields.data = fields;					\
	_status_ = ACTION (_td_);					\
      }									\
    _status_;								\
  })

#endif /* _MR_LOAD_H_ */
