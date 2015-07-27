#ifndef _FLT_VALUES_H_
#define _FLT_VALUES_H_

#include <metaresc.h>

#ifdef HAVE_CONFIG_H
#include <mr_config.h>
#endif /* HAVE_CONFIG_H */
#ifdef HAVE_VALUES_H
#include <values.h>
#else  /* HAVE_VALUES_H */
#define FLT_MAX (0x1.fffffep+127)
#define FLT_MIN (0x1p-126)
#define FLT_EPSILON (0x1p-23)
#define DBL_MAX (0x1.fffffffffffffp+1023)
#define DBL_MIN (0x1p-1022)
#define DBL_EPSILON (0x1p-52)
#define LDBL_MAX (0x1.ffffffffffffffffp+16383L)
#define LDBL_MIN (0x1p-16382L)
#define LDBL_EPSILON (0x1p-64)
#endif /* HAVE_VALUES_H */

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif /* M_PI */

#ifndef M_E
#define M_E 2.7182818284590452354
#endif /* M_E */

#define LD_NAN ((ieee_long_double_t){ { .long_double = 0, .ieee_854_long_double_nan = { .quiet_nan = !0, .one = 1, .exponent = -1, }, }, }).long_double
#define LD_LDBL_MAX ((ieee_long_double_t){ { .long_double = 0, .ieee_854_long_double = { .mantissa1 = -1, .mantissa0 = -1, .exponent = -2, .sign = PLUS, }, }, }).long_double
#define LD_LDBL_MIN ((ieee_long_double_t){ { .long_double = 0, .ieee_854_long_double = { .mantissa1 = 0, .mantissa0 = (1 << 31), .exponent = 1, .sign = PLUS, }, }, }).long_double

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

TYPEDEF_ENUM (ieee_754_float_nan_enum_t, (IEEE_754_FLOAT_NAN_ENUM_T, = (1 << 9) - 1, "ieee_754_float_nan"))
TYPEDEF_UNION (ieee_float_t,
	       ANON_UNION (),
	       (ieee_754_float_t, ieee_754_float),
	       (ieee_754_float_nan_t, ieee_754_float_nan),
	       float _float,
	       uint32_t uint32,
	       END_ANON_UNION ("quiet_nan"),
	       BITFIELD (ieee_754_float_nan_enum_t, quiet_nan, :9, , .offset = 22 / 8, .param = { .bitfield_param = { .width = 9, .shift = 22 % 8, .size = 0, }, } ),
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

TYPEDEF_ENUM (ieee_754_double_nan_enum_t, (IEEE_754_DOUBLE_NAN_ENUM_T, = (1 << 12) - 1, "ieee_754_double_nan"))
TYPEDEF_UNION (ieee_double_t,
	       ANON_UNION (),
	       (ieee_754_double_t, ieee_754_double),
	       (ieee_754_double_nan_t, ieee_754_double_nan),
	       double _double,
	       uint64_t uint64,
	       END_ANON_UNION ("quiet_nan"),
	       BITFIELD (ieee_754_double_nan_enum_t, quiet_nan, :12, , .offset = (32 + 19) / 8, .param = { .bitfield_param = { .width = 12, .shift = (32 + 19) % 8, .size = 0, }, } ),
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
	      (IEEE_854_LONG_DOUBLE_NAN_ENUM_T, = (1 << 17) - 1, "ieee_854_long_double_nan"),
	      (IEEE_854_LONG_DOUBLE_NAN_ZERO_ONE_ENUM_T, = ((1 << 17) - 1) ^ (1 << 1), "ieee_854_long_double_nan"),
	      )

TYPEDEF_UNION (ieee_long_double_t,
	       ANON_UNION (),
	       (ieee_854_long_double_t, ieee_854_long_double),
	       (ieee_854_long_double_nan_t, ieee_854_long_double_nan),
	       long double long_double,
	       END_ANON_UNION ("quiet_nan"),
	       BITFIELD (ieee_854_long_double_nan_enum_t, quiet_nan, :17, , .offset = (32 + 30) / 8, .param = { .bitfield_param = { .width = 17, .shift = (32 + 30) % 8, .size = 0, }, } ),
	       )
#endif /* _FLT_VALUES_H_ */
