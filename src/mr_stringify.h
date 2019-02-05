#ifndef _MR_STRINGIFY_H_
#define _MR_STRINGIFY_H_

#include <metaresc.h>

#define CINIT_COMPLEX_FLOAT_QUOTE "%s + %si"
#define CINIT_CHAR_QUOTE "\\%03o"

#define MR_OUTPUT_FORMAT_COMPLEX(NAME_SUFFIX, TYPE_NAME, MR_TYPE, TMPL)	\
  char * mr_output_format_complex_ ## NAME_SUFFIX (mr_ptrdes_t * ptrdes) { \
    char str[2 * MR_FLOAT_TO_STRING_BUF_SIZE] = "";			\
    mr_ptrdes_t real_ptrdes = *ptrdes;					\
    mr_ptrdes_t imag_ptrdes = *ptrdes;					\
    TYPE_NAME real = __real__ *(complex TYPE_NAME *)ptrdes->data.ptr;	\
    TYPE_NAME imag = __imag__ *(complex TYPE_NAME *)ptrdes->data.ptr;	\
    real_ptrdes.data.ptr = &real;					\
    real_ptrdes.fd.mr_type = MR_TYPE;					\
    imag_ptrdes.data.ptr = &imag;					\
    imag_ptrdes.fd.mr_type = MR_TYPE;					\
    char * real_str = mr_stringify_ ## NAME_SUFFIX (&real_ptrdes);	\
    char * imag_str = mr_stringify_ ## NAME_SUFFIX (&imag_ptrdes);	\
    if (real_str && imag_str)						\
      sprintf (str, TMPL, real_str, imag_str);				\
    if (real_str)							\
      MR_FREE (real_str);						\
    if (imag_str)							\
      MR_FREE (imag_str);						\
    return (mr_strdup (str));						\
  }

extern char * mr_output_format_bool (mr_ptrdes_t*);
extern char * mr_output_format_int8_t (mr_ptrdes_t*);
extern char * mr_output_format_uint8_t (mr_ptrdes_t*);
extern char * mr_output_format_int16_t (mr_ptrdes_t*);
extern char * mr_output_format_uint16_t (mr_ptrdes_t*);
extern char * mr_output_format_int32_t (mr_ptrdes_t*);
extern char * mr_output_format_uint32_t (mr_ptrdes_t*);
extern char * mr_output_format_int64_t (mr_ptrdes_t*);
extern char * mr_output_format_uint64_t (mr_ptrdes_t*);
extern char * mr_output_format_float (mr_ptrdes_t*);
extern char * mr_output_format_complex_float (mr_ptrdes_t*);
extern char * mr_output_format_double (mr_ptrdes_t*);
extern char * mr_output_format_complex_double (mr_ptrdes_t*);
extern char * mr_output_format_long_double_t (mr_ptrdes_t*);
extern char * mr_output_format_complex_long_double_t (mr_ptrdes_t*);

extern char * mr_stringify_bool (mr_ptrdes_t*);
extern char * mr_stringify_int8_t (mr_ptrdes_t*);
extern char * mr_stringify_uint8_t (mr_ptrdes_t*);
extern char * mr_stringify_int16_t (mr_ptrdes_t*);
extern char * mr_stringify_uint16_t (mr_ptrdes_t*);
extern char * mr_stringify_int32_t (mr_ptrdes_t*);
extern char * mr_stringify_uint32_t (mr_ptrdes_t*);
extern char * mr_stringify_int64_t (mr_ptrdes_t*);
extern char * mr_stringify_uint64_t (mr_ptrdes_t*);
extern char * mr_stringify_float (mr_ptrdes_t*);
extern char * mr_stringify_complex_float (mr_ptrdes_t*);
extern char * mr_stringify_double (mr_ptrdes_t*);
extern char * mr_stringify_complex_double (mr_ptrdes_t*);
extern char * mr_stringify_long_double_t (mr_ptrdes_t*);
extern char * mr_stringify_complex_long_double_t (mr_ptrdes_t*);
extern char * mr_stringify_enum (mr_ptrdes_t*);
extern char * mr_stringify_bitfield (mr_ptrdes_t*);
extern char * mr_stringify_bitmask (mr_ptrdes_t*, char*);
extern char * mr_stringify_func (mr_ptrdes_t*);

TYPEDEF_FUNC (int, mr_ra_printf_t, (mr_rarray_t *, mr_ptrdes_t *))

#define TRY_CATCH_THROW(ADD) ({			\
      int added = ADD;				\
      if (added < 0)				\
	return (added);				\
      added;					\
    })

int mr_ra_printf_void (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
int mr_ra_printf_float (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
int mr_ra_printf_bool (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
int mr_ra_printf_double (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
int mr_ra_printf_long_double_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
int mr_ra_printf_complex_float (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes, char * delimiter);
int mr_ra_printf_complex_double (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes, char * delimiter);
int mr_ra_printf_complex_long_double_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes, char * delimiter);
int mr_ra_printf_int8_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
int mr_ra_printf_uint8_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
int mr_ra_printf_int16_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
int mr_ra_printf_uint16_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
int mr_ra_printf_int32_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
int mr_ra_printf_uint32_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
int mr_ra_printf_int64_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
int mr_ra_printf_uint64_t (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
int mr_ra_printf_enum (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
int mr_ra_printf_bitfield (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes);
int mr_ra_printf_quote_string (mr_rarray_t * mr_ra_str, char * str, char * char_pattern);
const char * mr_serialize_func (void * func);
int mr_ra_append_char (mr_rarray_t * mr_ra_str, char c);
int mr_ra_append_string (mr_rarray_t * mr_ra_str, char * str);

#define MR_ESC_CHAR_MAP_SIZE (1 << __CHAR_BIT__)
extern char mr_esc_char_map[MR_ESC_CHAR_MAP_SIZE];

#endif /* _MR_STRINGIFY_H_ */
