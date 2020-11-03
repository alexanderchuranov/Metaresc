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
#define LD_LDBL_MAX ((ieee_long_double_t){ { .ieee_854_long_double = { .mantissa1 = -1, .mantissa0 = -1, .exponent = -2, .sign = PLUS, }, }, }).long_double
#define LD_LDBL_MIN ((ieee_long_double_t){ { .ieee_854_long_double = { .mantissa1 = 0, .mantissa0 = (1 << 31), .exponent = 1, .sign = PLUS, }, }, }).long_double

TYPEDEF_ENUM (sign_t, (PLUS, = 0), (MINUS, = 1))

TYPEDEF_STRUCT (ieee_754_float_t,
		BITFIELD (unsigned int, mantissa, :23),
		BITFIELD (unsigned int, exponent, :8),
		BITFIELD (sign_t, sign, :1))

TYPEDEF_STRUCT (ieee_754_float_nan_t,
		BITFIELD (unsigned int, mantissa, :22),
		BITFIELD (bool, quiet_nan, :1),
		BITFIELD (unsigned int, exponent, :8),
		BITFIELD (sign_t, sign, :1))

TYPEDEF_ENUM (ieee_754_float_nan_enum_t, (IEEE_754_FLOAT_NAN_ENUM_T, = (1 << 8) - 1, "ieee_754_float_nan"))

TYPEDEF_UNION (ieee_float_t,
	       ANON_UNION (),
	       (ieee_754_float_t, ieee_754_float),
	       (ieee_754_float_nan_t, ieee_754_float_nan),
	       END_ANON_UNION ("is_nan"),
	       BITFIELD (ieee_754_float_nan_enum_t, is_nan, :8, , .offset = 23 / __CHAR_BIT__, .param = { .bitfield_param = { .width = 8, .shift = 23 % __CHAR_BIT__, .size = 0, }, } ),
	       float _float,
	       uint32_t uint32,
	       )

TYPEDEF_STRUCT (ieee_754_double_t,
		BITFIELD (unsigned int, mantissa1, :32),
		BITFIELD (unsigned int, mantissa0, :20),
		BITFIELD (unsigned int, exponent, :11),
		BITFIELD (sign_t, sign, :1))

TYPEDEF_STRUCT (ieee_754_double_nan_t,
		BITFIELD (unsigned int, mantissa1, :32),
		BITFIELD (unsigned int, mantissa0, :19),
		BITFIELD (bool, quiet_nan, :1),
		BITFIELD (unsigned int, exponent, :11),
		BITFIELD (sign_t, sign, :1))

TYPEDEF_ENUM (ieee_754_double_nan_enum_t, (IEEE_754_DOUBLE_NAN_ENUM_T, = (1 << 11) - 1, "ieee_754_double_nan"))
TYPEDEF_UNION (ieee_double_t,
	       ANON_UNION (),
	       (ieee_754_double_t, ieee_754_double),
	       (ieee_754_double_nan_t, ieee_754_double_nan),
	       END_ANON_UNION ("is_nan"),
	       BITFIELD (ieee_754_double_nan_enum_t, is_nan, :11, , .offset = (32 + 19) / 8, .param = { .bitfield_param = { .width = 11, .shift = (32 + 19) % 8, .size = 0, }, } ),
	       double _double,
	       uint64_t uint64,
	       )

TYPEDEF_STRUCT (ieee_854_long_double_t,
		BITFIELD (unsigned int, mantissa1, :32),
		BITFIELD (unsigned int, mantissa0, :32),
		BITFIELD (unsigned int, exponent, :15),
		BITFIELD (sign_t, sign, :1),
		VOID (unsigned int, empty, :16))

TYPEDEF_STRUCT (ieee_854_long_double_nan_t,
		BITFIELD (unsigned int, mantissa1, :32),
		BITFIELD (unsigned int, mantissa0, :30),
		BITFIELD (bool, quiet_nan, :1),
		BITFIELD (unsigned int, one, :1),
		BITFIELD (unsigned int, exponent, :15),
		BITFIELD (sign_t, sign, :1),
		VOID (unsigned int, empty, :16))

TYPEDEF_ENUM (ieee_854_long_double_nan_enum_t,
	      (IEEE_854_LONG_DOUBLE_NAN_ENUM_T, = (1 << 15) - 1, "ieee_854_long_double_nan"),
	      (IEEE_854_LONG_DOUBLE_NAN_ZERO_ONE_ENUM_T, = ((1 << 15) - 1) ^ (1 << 1), "ieee_854_long_double_nan"),
	      )

TYPEDEF_UNION (ieee_long_double_t,
	       ANON_UNION (),
	       (ieee_854_long_double_t, ieee_854_long_double),
	       (ieee_854_long_double_nan_t, ieee_854_long_double_nan),
	       END_ANON_UNION ("is_nan"),
	       BITFIELD (ieee_854_long_double_nan_enum_t, is_nan, :15, , .offset = (32 + 30) / 8, .param = { .bitfield_param = { .width = 15, .shift = (32 + 30) % 8, .size = 0, }, } ),
	       long double long_double,
	       )

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
