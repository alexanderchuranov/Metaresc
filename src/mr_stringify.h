#ifndef _MR_STRINGIFY_H_
#define  _MR_STRINGIFY_H_

#include <metaresc.h>

extern char * mr_output_format_int8_t (mr_ptrdes_t*);
extern char * mr_output_format_uint8_t (mr_ptrdes_t*);
extern char * mr_output_format_int16_t (mr_ptrdes_t*);
extern char * mr_output_format_uint16_t (mr_ptrdes_t*);
extern char * mr_output_format_int32_t (mr_ptrdes_t*);
extern char * mr_output_format_uint32_t (mr_ptrdes_t*);
extern char * mr_output_format_int64_t (mr_ptrdes_t*);
extern char * mr_output_format_uint64_t (mr_ptrdes_t*);
extern char * mr_output_format_float (mr_ptrdes_t*);
extern char * mr_output_format_double (mr_ptrdes_t*);
extern char * mr_output_format_long_double_t (mr_ptrdes_t*);

#endif /* _MR_STRINGIFY_H_ */
