#ifndef _MR_STRINGIFY_H_
#define _MR_STRINGIFY_H_

#include <metaresc.h>

#define TRY_CATCH_THROW(ADD) ({			\
      int added = ADD;				\
      if (added < 0)				\
	return (added);				\
      added;					\
    })

extern int mr_ra_printf_void (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
extern int mr_ra_printf_float (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
extern int mr_ra_printf_bool (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
extern int mr_ra_printf_double (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
extern int mr_ra_printf_long_double_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
extern int mr_ra_printf_complex_float (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes, char * delimiter);
extern int mr_ra_printf_complex_double (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes, char * delimiter);
extern int mr_ra_printf_complex_long_double_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes, char * delimiter);
extern int mr_ra_printf_int8_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
extern int mr_ra_printf_uint8_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
extern int mr_ra_printf_int16_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
extern int mr_ra_printf_uint16_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
extern int mr_ra_printf_int32_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
extern int mr_ra_printf_uint32_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
extern int mr_ra_printf_int64_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
extern int mr_ra_printf_uint64_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
extern int mr_ra_printf_enum (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
extern int mr_ra_printf_bitfield (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
extern int mr_ra_printf_quote_string (mr_rarray_t * mr_ra_str, char * str, char * char_pattern);
extern int mr_ra_printf_bitmask (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes, char * delimiter);
extern const char * mr_serialize_func (void * func);
extern int mr_ra_append_char (mr_rarray_t * mr_ra_str, char c);
extern int mr_ra_append_string (mr_rarray_t * mr_ra_str, char * str);

#define MR_ESC_CHAR_MAP_SIZE (1 << __CHAR_BIT__)
extern char mr_esc_char_map[MR_ESC_CHAR_MAP_SIZE];

#endif /* _MR_STRINGIFY_H_ */
