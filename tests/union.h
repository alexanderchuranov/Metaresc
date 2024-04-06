#include <metaresc.h>

TYPEDEF_UNION (empty_union_t)
TYPEDEF_STRUCT (empty_struct_t)

TYPEDEF_UNION (union_int32_float_t, float x, int32_t y)

TYPEDEF_ENUM (enum_discriminator_t,
	      (UD_FLOAT, , "x"),
	      (UD_INT32, , "y"))

TYPEDEF_ENUM (enum8_discriminator_t,
	      ATTRIBUTES (__attribute__ ((packed))),
	      (U8D_FLOAT, , "x"),
	      (U8D_INT32, , "y"))

TYPEDEF_ENUM (enum16_discriminator_t,
	      ATTRIBUTES (__attribute__ ((packed))),
	      (U16D_FLOAT, , "x"),
	      (U16D_INT32, , "y"),
	      (U16D_LAST, = 1ULL << (__CHAR_BIT__ * sizeof (uint16_t) - 1)))

TYPEDEF_ENUM (enum32_discriminator_t,
	      ATTRIBUTES (__attribute__ ((packed))),
	      (U32D_FLOAT, , "x"),
	      (U32D_INT32, , "y"),
	      (U32D_LAST, = 1ULL << (__CHAR_BIT__ * sizeof (uint32_t) - 1)))

TYPEDEF_ENUM (enum64_discriminator_t,
	      ATTRIBUTES (__attribute__ ((packed))),
	      (U64D_FLOAT, , "x"),
	      (U64D_INT32, , "y"),
	      (U64D_LAST, = 1ULL << (__CHAR_BIT__ * sizeof (uint64_t) - 1)))

TYPEDEF_STRUCT (struct_embed_anon_union_t,
		int dummy,
		ANON_UNION (),
		ANON_UNION (),
		int32_t y,
		END_ANON_UNION (),
		float x,
		END_ANON_UNION (),
		)

TYPEDEF_STRUCT (struct_anon_union_enum_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		ANON_UNION (),
		float x,
		int32_t y,
		END_ANON_UNION ("discriminator"),
		(enum_discriminator_t, discriminator),
		)

TYPEDEF_STRUCT (struct_named_anon_union_enum_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		ANON_UNION (xy),
		float x,
		int32_t y,
		END_ANON_UNION ("discriminator"),
		(enum_discriminator_t, discriminator),
		)

TYPEDEF_STRUCT (struct_union_enum8_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(enum8_discriminator_t, discriminator),
		)

TYPEDEF_STRUCT (struct_union_enum16_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(enum16_discriminator_t, discriminator),
		)

TYPEDEF_STRUCT (struct_union_enum32_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(enum32_discriminator_t, discriminator),
		)

TYPEDEF_STRUCT (struct_union_enum64_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(enum64_discriminator_t, discriminator),
		)

TYPEDEF_STRUCT (struct_union_enum_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(enum_discriminator_t, discriminator),
		)

TYPEDEF_STRUCT (struct_union_enum_ptr_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(enum_discriminator_t *, discriminator),
		)

TYPEDEF_STRUCT (struct_union_int8_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(int8_t, discriminator),
		)

TYPEDEF_STRUCT (struct_union_uint8_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(uint8_t, discriminator),
		)

TYPEDEF_STRUCT (struct_union_int16_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(int16_t, discriminator),
		)

TYPEDEF_STRUCT (struct_union_uint16_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(uint16_t, discriminator),
		)

TYPEDEF_STRUCT (struct_union_int32_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(int32_t, discriminator),
		)

TYPEDEF_STRUCT (struct_union_uint32_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(uint32_t, discriminator),
		)

TYPEDEF_STRUCT (struct_union_int64_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(int64_t, discriminator),
		)

TYPEDEF_STRUCT (struct_union_uint64_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(uint64_t, discriminator),
		)

TYPEDEF_STRUCT (struct_union_bitfield_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		BITFIELD (enum_discriminator_t, discriminator, :1),
		)

TYPEDEF_STRUCT (struct_union_string_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(string_t, discriminator),
		)

TYPEDEF_STRUCT (struct_union_string_ptr_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(string_t *, discriminator),
		)

TYPEDEF_STRUCT (string_struct_t,
		string_t discriminator1,
		string_t discriminator2,
		)

TYPEDEF_STRUCT (struct_union_string_struct_ptr_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(string_struct_t *, discriminator),
		)

TYPEDEF_STRUCT (struct_union_string_array_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(string_t, discriminator, [1]),
		)

TYPEDEF_STRUCT (struct_union_string_zero_array_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(string_struct_t, real_discriminator),
		(string_t, discriminator, [0], ,
		 .offset = offsetof (struct_union_string_zero_array_t, real_discriminator.discriminator2)),
		)

TYPEDEF_STRUCT (struct_union_string_struct_ptr_array_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(string_struct_t *, discriminator, [1]),
		)

TYPEDEF_CHAR_ARRAY (ca2_t, 2)

TYPEDEF_STRUCT (struct_union_ca_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(ca2_t, discriminator),
		)

TYPEDEF_STRUCT (struct_union_ca_ptr_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(ca2_t *, discriminator),
		)

TYPEDEF_STRUCT (struct_mr_ptr_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		(mr_ptr_t, ptr, , "discriminator"),
		string_t discriminator,
		)

#define ENUM_OVERRIDE				\
  (mr_ud_override_t[]) {			\
    {UD_FLOAT, "y"},				\
      {UD_INT32, "x"},				\
	}

TYPEDEF_STRUCT (struct_union_enum_overtided_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		(union_int32_float_t, xy, , "discriminator", { ENUM_OVERRIDE }, "mr_ud_override_t", sizeof (ENUM_OVERRIDE)),
		(enum_discriminator_t, discriminator),
		)

#define IDX_OVERRIDE				\
  (mr_ud_override_t[]) {			\
    {0, "y"},					\
      {1, "x"},					\
	}

TYPEDEF_STRUCT (struct_union_int_overtided_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		(union_int32_float_t, xy, , "discriminator", { IDX_OVERRIDE }, "mr_ud_override_t", sizeof (IDX_OVERRIDE)),
		(int, discriminator),
		)

TYPEDEF_STRUCT (struct_union_bool_overtided_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		(union_int32_float_t, xy, , "discriminator", { IDX_OVERRIDE }, "mr_ud_override_t", sizeof (IDX_OVERRIDE)),
		(bool, discriminator),
		)

TYPEDEF_STRUCT (struct_union_bitfield_overtided_t,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		int dummy,
		(union_int32_float_t, xy, , "discriminator", { IDX_OVERRIDE }, "mr_ud_override_t", sizeof (IDX_OVERRIDE)),
		BITFIELD (int, discriminator, : 1),
		)

