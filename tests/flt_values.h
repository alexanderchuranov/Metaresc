#ifndef _FLT_VALUES_H_
#define _FLT_VALUES_H_

#include <math.h>
#include <metaresc.h>

#ifdef HAVE_CONFIG_H
#include <mr_config.h>
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_VALUES_H
#include <values.h>
#endif /* HAVE_VALUES_H */

#define MR_PI 3.14159265358979323846L
#define MR_E 2.7182818284590452354L

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

#define LD_NAN __builtin_nanl ("0")
#define LD_LDBL_MAX __builtin_huge_vall ()
#define LD_LDBL_MIN LDBL_MIN

TYPEDEF_ENUM (mr_sign_t, (MR_PLUS, = 0), (MR_MINUS, = 1))

#define IEEE_754_HALF_MANTISSA (10)
#define IEEE_754_HALF_EXPONENT (5)

TYPEDEF_STRUCT (ieee_754_half_t, ATTRIBUTES (__attribute__ ((packed))),
		(uint32_t, mantissa, : IEEE_754_HALF_MANTISSA),
		(uint32_t, exponent, : IEEE_754_HALF_EXPONENT),
		(mr_sign_t, sign, : 1))

TYPEDEF_STRUCT (ieee_754_half_nan_t, ATTRIBUTES (__attribute__ ((packed))),
		(uint32_t, mantissa, : IEEE_754_HALF_MANTISSA - 1),
		(bool, quiet_nan, : 1),
		(uint32_t, exponent, : IEEE_754_HALF_EXPONENT),
		(mr_sign_t, sign, : 1))

TYPEDEF_ENUM (ieee_754_half_nan_enum_t, (IEEE_754_HALF_NAN_ENUM_T, = (1 << IEEE_754_HALF_EXPONENT) - 1, "ieee_754_half_nan"))

TYPEDEF_UNION (ieee_half_t, ATTRIBUTES (__attribute__ ((packed))),
	       ANON_UNION ( , __attribute__ ((packed))),
	       (ieee_754_half_t, ieee_half_float),
	       (ieee_754_half_nan_t, ieee_754_half_nan),
	       END_ANON_UNION ("is_nan"),
	       (ieee_754_half_nan_enum_t, is_nan, : IEEE_754_HALF_EXPONENT, ,
			 .offset = IEEE_754_HALF_MANTISSA / __CHAR_BIT__,
			 .bitfield_param = {
			     .bitfield = NULL, .size = 0,
			     .width = IEEE_754_HALF_EXPONENT,
			     .shift = IEEE_754_HALF_MANTISSA % __CHAR_BIT__,
			     .initialized = true, }
			 ),
	       uint16_t uint16,
	       )

#define IEEE_754_FLOAT_MANTISSA (23)
#define IEEE_754_FLOAT_EXPONENT (8)

TYPEDEF_STRUCT (ieee_754_float_t,
		(uint32_t, mantissa, : IEEE_754_FLOAT_MANTISSA),
		(uint32_t, exponent, : IEEE_754_FLOAT_EXPONENT),
		(mr_sign_t, sign, : 1))

TYPEDEF_STRUCT (ieee_754_float_nan_t,
		(uint32_t, mantissa, : IEEE_754_FLOAT_MANTISSA - 1),
		(bool, quiet_nan, : 1),
		(uint32_t, exponent, : IEEE_754_FLOAT_EXPONENT),
		(mr_sign_t, sign, : 1))

TYPEDEF_ENUM (ieee_754_float_nan_enum_t, (IEEE_754_FLOAT_NAN_ENUM_T, = (1 << IEEE_754_FLOAT_EXPONENT) - 1, "ieee_754_float_nan"))

TYPEDEF_UNION (ieee_float_t,
	       ANON_UNION (),
	       (ieee_754_float_t, ieee_754_float),
	       (ieee_754_float_nan_t, ieee_754_float_nan),
	       END_ANON_UNION ("is_nan"),
	       (ieee_754_float_nan_enum_t, is_nan, : IEEE_754_FLOAT_EXPONENT, ,
			 .offset = IEEE_754_FLOAT_MANTISSA / __CHAR_BIT__,
			 .bitfield_param = {
			     .bitfield = NULL, .size = 0,
			     .width = IEEE_754_FLOAT_EXPONENT,
			     .shift = IEEE_754_FLOAT_MANTISSA % __CHAR_BIT__,
			     .initialized = true, }
			 ),
	       float _float,
	       uint32_t uint32,
	       )

#define IEEE_754_DOUBLE_MANTISSA (52)
#define IEEE_754_DOUBLE_EXPONENT (11)

TYPEDEF_STRUCT (ieee_754_double_t,
		(uint64_t, mantissa, : IEEE_754_DOUBLE_MANTISSA),
		(uint32_t, exponent, : IEEE_754_DOUBLE_EXPONENT),
		(mr_sign_t, sign, : 1))

TYPEDEF_STRUCT (ieee_754_double_nan_t,
		(uint64_t, mantissa, : IEEE_754_DOUBLE_MANTISSA - 1),
		(bool, quiet_nan, : 1),
		(uint32_t, exponent, : IEEE_754_DOUBLE_EXPONENT),
		(mr_sign_t, sign, : 1))

TYPEDEF_ENUM (ieee_754_double_nan_enum_t, (IEEE_754_DOUBLE_NAN_ENUM_T, = (1 << IEEE_754_DOUBLE_EXPONENT) - 1, "ieee_754_double_nan"))

TYPEDEF_UNION (ieee_double_t,
	       ANON_UNION (),
	       (ieee_754_double_t, ieee_754_double),
	       (ieee_754_double_nan_t, ieee_754_double_nan),
	       END_ANON_UNION ("is_nan"),
	       (ieee_754_double_nan_enum_t, is_nan, : IEEE_754_DOUBLE_EXPONENT, ,
			 .offset = IEEE_754_DOUBLE_MANTISSA / __CHAR_BIT__,
			 .bitfield_param = {
			     .bitfield = NULL, .size = 0,
			     .width = IEEE_754_DOUBLE_EXPONENT,
			     .shift = IEEE_754_DOUBLE_MANTISSA % __CHAR_BIT__,
			     .initialized = true, }
			 ),
	       double _double,
	       uint64_t uint64,
	       )

#define IEEE_854_LONG_DOUBLE_MANTISSA (64)
#define IEEE_854_LONG_DOUBLE_EXPONENT (15)

TYPEDEF_STRUCT (ieee_854_long_double_t, ATTRIBUTES (__attribute__ ((packed))),
		(uint64_t, mantissa),
		(uint32_t, exponent, : IEEE_854_LONG_DOUBLE_EXPONENT),
		(mr_sign_t, sign, : 1),
		)

TYPEDEF_STRUCT (ieee_854_long_double_nan_t, ATTRIBUTES (__attribute__ ((packed))),
		(uint64_t, mantissa, : IEEE_854_LONG_DOUBLE_MANTISSA - 2),
		(bool, quiet_nan, : 1),
		(unsigned int, one, : 1),
		(uint32_t, exponent, : IEEE_854_LONG_DOUBLE_EXPONENT),
		(mr_sign_t, sign, : 1),
		)

TYPEDEF_ENUM (ieee_854_long_double_nan_enum_t,
	      (IEEE_854_LONG_DOUBLE_NAN_ENUM_T, = (1 << IEEE_854_LONG_DOUBLE_EXPONENT) - 1, "ieee_854_long_double_nan"),
	      (IEEE_854_LONG_DOUBLE_NAN_ZERO_ONE_ENUM_T, = ((1 << IEEE_854_LONG_DOUBLE_EXPONENT) - 1) ^ (1 << 1), "ieee_854_long_double_nan"),
	      )

TYPEDEF_UNION (ieee_long_double_t,
	       ANON_UNION (),
	       (ieee_854_long_double_t, ieee_854_long_double),
	       (ieee_854_long_double_nan_t, ieee_854_long_double_nan),
	       END_ANON_UNION ("is_nan"),
	       (ieee_854_long_double_nan_enum_t, is_nan, : IEEE_854_LONG_DOUBLE_EXPONENT, ,
			 .offset = IEEE_854_LONG_DOUBLE_MANTISSA / __CHAR_BIT__,
			 .bitfield_param = {
			     .bitfield = NULL, .size = 0,
			     .width = IEEE_854_LONG_DOUBLE_EXPONENT,
			     .shift = IEEE_854_LONG_DOUBLE_MANTISSA % __CHAR_BIT__,
			     .initialized = true, }
			 ),
	       long double long_double,
	       )

#endif /* _FLT_VALUES_H_ */
