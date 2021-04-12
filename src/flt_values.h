#ifndef _FLT_VALUES_H_
#define _FLT_VALUES_H_

#include <metaresc.h>

#ifdef HAVE_CONFIG_H
#include <mr_config.h>
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_VALUES_H
#include <values.h>
#endif /* HAVE_VALUES_H */

#ifndef FLT_MAX
#define FLT_MAX (0x1.fffffep+127)
#endif /* FLT_MAX */

#ifndef FLT_MIN
#define FLT_MIN (0x1p-126)
#endif /* FLT_MIN */

#ifndef FLT_EPSILON
#define FLT_EPSILON (0x1p-23)
#endif /* FLT_EPSILON */

#ifndef DBL_MAX
#define DBL_MAX (0x1.fffffffffffffp+1023)
#endif /* DBL_MAX */

#ifndef DBL_MIN
#define DBL_MIN (0x1p-1022)
#endif /* DBL_MIN */

#ifndef DBL_EPSILON
#define DBL_EPSILON (0x1p-52)
#endif /* DBL_EPSILON */

#ifndef LDBL_MAX
#define LDBL_MAX (0x1.ffffffffffffffffp+16383L)
#endif /* LDBL_MAX */

#ifndef LDBL_MIN
#define LDBL_MIN (0x1p-16382L)
#endif /* LDBL_MIN */

#ifndef LDBL_EPSILON
#define LDBL_EPSILON (0x1p-64)
#endif /* LDBL_EPSILON */

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif /* M_PI */

#ifndef M_E
#define M_E 2.7182818284590452354
#endif /* M_E */

#define LD_NAN ((ieee_long_double_t){ { .ieee_854_long_double_nan = { .quiet_nan = !0, .one = 1, .exponent = -1, }, }, }).long_double
#define LD_LDBL_MAX ((ieee_long_double_t){ { .ieee_854_long_double = { .mantissa = -1, .exponent = -2, .sign = PLUS, }, }, }).long_double
#define LD_LDBL_MIN ((ieee_long_double_t){ { .ieee_854_long_double = { .mantissa = (1UL << (IEEE_854_LONG_DOUBLE_MANTISSA - 1)), .exponent = 1, .sign = PLUS, }, }, }).long_double

#define MR_SIZEOF_LONG_DOUBLE (sizeof (ieee_854_long_double_t))

#define MR_CLD_PACK(CLD) ({				\
      complex_long_double_t __cld__ = (CLD);		\
      (mr_complex_long_double_t) {			\
	.real = (ieee_long_double_t) {			\
	  .long_double = __real__ __cld__		\
	}.ieee_854_long_double,				\
	    .imag = (ieee_long_double_t) {		\
	  .long_double = __imag__ __cld__		\
	}.ieee_854_long_double,				\
	    };						\
    })

#define MR_CLD_UNPACK(MR_CLD) ({			\
      mr_complex_long_double_t __mr_cld = (MR_CLD);	\
      complex_long_double_t __cld__;			\
      __real__ __cld__ = (ieee_long_double_t) {		\
	.ieee_854_long_double = __mr_cld.real		\
      }.long_double;					\
      __imag__ __cld__ = (ieee_long_double_t) {		\
	.ieee_854_long_double = __mr_cld.imag		\
      }.long_double;					\
      __cld__;						\
    })
  
#define MR_ISNAN(X)							\
  __builtin_choose_expr (__builtin_types_compatible_p (__typeof__ (X), float), \
			 ((ieee_float_t)(float)(X)).ieee_754_float_nan.exponent == IEEE_754_FLOAT_NAN_ENUM_T, \
			 __builtin_choose_expr (__builtin_types_compatible_p (__typeof__ (X), double), \
						((ieee_double_t)(double)(X)).ieee_754_double_nan.exponent == IEEE_754_DOUBLE_NAN_ENUM_T, \
						__builtin_choose_expr (__builtin_types_compatible_p (__typeof__ (X), long double), \
								       ((((ieee_long_double_t)(long double)(X)).ieee_854_long_double_nan.exponent == IEEE_854_LONG_DOUBLE_NAN_ENUM_T) || \
									(((ieee_long_double_t)(long double)(X)).ieee_854_long_double_nan.exponent == IEEE_854_LONG_DOUBLE_NAN_ZERO_ONE_ENUM_T)), \
								       false)))
#endif /* _FLT_VALUES_H_ */
