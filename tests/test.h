/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

#include <reslib.h>

#include <meta.h>

#undef FALSE
#undef TRUE

#undef RL_TYPE_NAME
#define RL_TYPE_NAME boolean_t
RL_TYPEDEF_ENUM (__attribute__ ((packed,aligned(1))))
  RL_ENUM_DEF (FALSE, = 0)
  RL_ENUM_DEF (TRUE, = !0)
RL_END_ENUM ()

#undef RL_TYPE_NAME
#define RL_TYPE_NAME mask_t
RL_TYPEDEF_ENUM (__attribute__ ((packed,aligned(2))))
  RL_ENUM_DEF (NONE, = 0)
  RL_ENUM_DEF (READ, = (1 << 0))
  RL_ENUM_DEF (WRITE, = (1 << 1))
  RL_ENUM_DEF (EXEC, = (1 << 2))
RL_END_ENUM ()

#undef RL_TYPE_NAME
#define RL_TYPE_NAME point_t
RL_TYPEDEF_STRUCT (__attribute__((packed)))
  RL_FLOAT (x, "Comment", &((meta_info_t){.meta_type = SIMPLE, .info = "comment field",}))
  RL_DOUBLE (y, "Comment", "comment")
  RL_POINTER_STRUCT (sample_t, sample)
RL_END_STRUCT ("Comment", &((meta_info_t){.meta_type = SIMPLE, .info = "comment struct",}))

#undef RL_TYPE_NAME
#define RL_TYPE_NAME char_t
RL_TYPEDEF_STRUCT ()
  RL_CHAR (c)
RL_END_STRUCT ()

#undef RL_TYPE_NAME
#define RL_TYPE_NAME union_t
RL_TYPEDEF_UNION (__attribute__((__transparent_union__)))
  RL_UINT32 (union_uint32)
  RL_FLOAT (union_float)
RL_END_UNION ("comment")

#undef RL_TYPE_NAME
#define RL_TYPE_NAME union_enum_discriminator_t
RL_TYPEDEF_ENUM ()
  RL_ENUM_DEF (UED_DEFAULT)
  RL_ENUM_DEF (UED_INT32, , "union_uint32")
  RL_ENUM_DEF (UED_FLOAT, , "union_float")
RL_END_ENUM ()

#undef RL_TYPE_NAME
#define RL_TYPE_NAME sample_t
RL_TYPEDEF_STRUCT ()
  RL_POINTER (point_t, ptr_fwd)
  RL_POINTER (point_t, ptr_fwd_mismatch)
  RL_STRUCT (point_t, point)
  RL_POINTER (point_t, ptr_bkw)
  RL_POINTER (point_t, ptr_bkw_mismatch)
  RL_POINTER (point_t, ptr_external)
  RL_POINTER (void, pointer_on_pointer)
  RL_INT8 (_int8, "Comment", (meta_info_t[]){{.format = "0x%02" SCNx8 " "}})
  RL_UINT8 (_uint8, "Comment", &((meta_info_t){.format = "0x%02" SCNx8 " "}))
  RL_INT16 (_int16)
  RL_UINT16 (_uint16)
  RL_INT32 (_int32)
  RL_UINT32 (_uint32)
  RL_AUTO (int, _int)
  RL_INT64 (_int64)
  RL_UINT64 (_uint64)
  RL_FLOAT (_float)
  RL_DOUBLE (_double)
  RL_LONG_DOUBLE (ld)
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
  RL_ENUM (boolean_t, bool)
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
  RL_RARRAY (char, rarray_)
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
  RL_NONE (void*, ext_info, , "user extended info", "one more extra string")
  RL_NONE (int , res1[0], , "test of memory cleanup")
  RL_NONE (int , res2, [0])
  RL_NONE (int , res3, [], "array with flexible number of elements")
RL_END_STRUCT ("Comment", "One more comment")

#undef RL_MODE
