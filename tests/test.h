/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

#include <reslib.h>

#include <meta.h>

#undef FALSE
#undef TRUE

TYPEDEF_ENUM (boolean_t,
	      ATTRIBUTES (__attribute__ ((packed,aligned(1)))),
	      (FALSE, = 0),
	      (TRUE, = !0),
	      )

TYPEDEF_ENUM (mask_t,
	      ATTRIBUTES (__attribute__ ((packed,aligned(2)))),
	      (NONE, = 0),
	      (READ, = (1 << 0)),
	      (WRITE, = (1 << 1)),
	      (EXEC, = (1 << 2)),
	      )

TYPEDEF_STRUCT (point_t,
		ATTRIBUTES (__attribute__((packed)), "Comment on struct", { &((meta_info_t){.meta_type = SIMPLE, .info = "comment struct",})}, "meta_info_t"),
		FLOAT (x, "Comment", {&((meta_info_t){.meta_type = SIMPLE, .info = "comment field",})}, "meta_info_t"),
		DOUBLE (y, "Comment", {"ext comment"}, "rl_char_array_t"),
		POINTER_STRUCT (sample_t, sample),
		)

TYPEDEF_STRUCT (char_t, CHAR (c))

TYPEDEF_STRUCT
(empty_t,
 (int, _x_, (int, __volatile__ __const__ char*, __volatile __const char * *, volatile const int*, void *, void**,  char*, char**, struct sample_t *, int (*) (struct sample_t*))),
 (str_t, p),
 INT32 _int,
 ANON_UNION (__attribute__((__transparent_union__))),
   INT32 (x),
   (float, y),
 END_ANON_UNION ("xxx"),
 NAMED_ANON_UNION (huh),
   INT32 (x),
   (float, y),
 END_ANON_UNION ("com1"),
 (double, z, [2]),
 (void, e, (void)),
 (const long double volatile* __restrict__ __const const __const__  __volatile__, union_),
 (const struct sample_t *, forward),
 FIELD (int32_t, q, [3], RL_TYPE_INT32, RL_TYPE_EXT_ARRAY, "com"),
 (long double *, w, , "comment"),
 BITFIELD (short, b1, 12),
 BITFIELD (long int, b2, 24),
 )

TYPEDEF_STRUCT
(ieee_float_t,
 BITFIELD (unsigned int, mantissa, 23),
 BITFIELD (int, exponent, 8),
 BITFIELD (unsigned int, negative, 1),
 )

TYPEDEF_UNION
(ieee754_float_t,
 (ieee_float_t, b),
 (float, f),
 )

TYPEDEF_UNION (union_t,
	       ATTRIBUTES (__attribute__((__transparent_union__)), "comment"),
	       UINT32 (union_uint32),
	       FLOAT (union_float),
	       )

TYPEDEF_ENUM (union_enum_discriminator_t,
	      UED_DEFAULT,
	      (UED_INT32, , "union_uint32"),
	      (UED_FLOAT, , "union_float"),
	      )

TYPEDEF_FUNC (msg_handler_x_t, int, (int, int, char*), "comment")
  
#undef RL_TYPE_NAME
#define RL_TYPE_NAME sample_t
RL_TYPEDEF_STRUCT ()
  RL_POINTER (point_t, ptr_fwd)
  RL_POINTER (point_t, ptr_fwd_mismatch)
  RL_STRUCT (point_t, point)
  RL_POINTER (point_t, ptr_bkw)
  RL_POINTER (point_t, ptr_bkw_mismatch)
  RL_POINTER (point_t, ptr_external)
  RL_POINTER (double volatile const, type_renamed)
  RL_POINTER ( long double, typename_with_spaces)
  RL_POINTER (void, pointer_on_pointer)
  RL_INT8 (_int8, "Comment", {(meta_info_t[]){{.format = "0x%02" SCNx8 " "}}}, "meta_info_t")
  RL_UINT8 (_uint8, "Comment", {&((meta_info_t){.format = "0x%02" SCNx8 " "})}, "meta_info_t")
  RL_INT16 (_int16)
  RL_UINT16 (_uint16)
  RL_INT32 (_int32)
  RL_UINT32 (_uint32)
  RL_AUTO (int, _int)
  RL_INT64 (_int64)
  RL_UINT64 (_uint64)
  RL_BITFIELD (mask_t, b1, 3)
  RL_BITFIELD (mask_t, b2, 3)
  RL_BITFIELD (mask_t, b3, 3)
  RL_FLOAT (_float)
  RL_DOUBLE (_double)
  RL_LONG_DOUBLE (ld)
  RL_AUTO (msg_handler_x_t, handler)
  RL_AUTO (long double, _ld, [2])
  RL_AUTO (long_double_t, _ld_, , "RL_AUTO for any type")
  RL_CHAR (_char)
  RL_CHAR_ARRAY (char, union_static_str_discriminator, [16])
  RL_ANON_UNION (__attribute__ ((packed)))
    RL_ARRAY (int32_t, union_uint1, [sizeof (float) / sizeof (int32_t)])
    RL_FLOAT (union_float)
  RL_END_ANON_UNION ("union_static_str_discriminator")
  RL_STRING (union_str_discriminator)
  RL_NAMED_ANON_UNION (union_double_, __attribute__ ((packed)))
    RL_ARRAY (int64_t, union_uint2, [sizeof (double) / sizeof (int64_t)])
    RL_DOUBLE (union_double)
  RL_END_ANON_UNION ("union_str_discriminator")
  RL_ENUM (union_enum_discriminator_t, union_enum_discriminator)
  RL_UNION (union_t, _union_enum, "union_enum_discriminator")
  RL_UINT8 (union_int_discriminator)
  RL_UNION (union_t, _union_int, "union_int_discriminator")
  RL_ENUM (boolean_t, _bool)
  RL_BITMASK (mask_t, mask)
  RL_STRING (string)
  RL_STRING (string_empty)
  RL_STRING (string_zero)
  RL_STRING (string_full)
  RL_CHAR_ARRAY (char, char_array, [16])
  RL_CHAR_ARRAY (str_t, char_array_type)
  RL_ARRAY (char, array, [2])
  RL_ARRAY (char_t, array_, [2])
  RL_ARRAY (int, array2d, [3][2])
  RL_NONE (int, xxx)
  RL_NONE (char_t, array_none, [2])
  RL_RARRAY (char_t, rarray)
  RL_RARRAY (char, rarray_1)
  RL_RARRAY (char, rarray_2)
  RL_RARRAY (char_t, rarray_empty)
  RL_POINTER (void, _void, "void pointer")
  RL_POINTER (sample_t, next, "linked list example")
  RL_POINTER (sample_t, prev)
  RL_POINTER (int8_t, ptr_int8)
  RL_POINTER (uint8_t, ptr_uint8)
  RL_POINTER (char, ptr_char)
  RL_POINTER (char, ptr_char_)
  RL_POINTER (char, ptr_null)
  RL_AUTO (int, arr, [2], "comment")
  RL_NONE (void*, ext_info, , "user extended info", { "one more extra string" })
  RL_NONE (int , res1[0], , "test of memory cleanup")
  RL_NONE (int , res2, [0])
  RL_NONE (int , res3, [], "array with flexible number of elements")
RL_END_STRUCT ("Comment", { "One more comment" }, "rl_char_array_t")

#undef RL_MODE
