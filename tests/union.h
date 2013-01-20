#include <metaresc.h>

TYPEDEF_UNION (empty_union_t)
TYPEDEF_STRUCT (empty_struct_t)

TYPEDEF_UNION (union_int32_float_t, float x, int32_t y)

TYPEDEF_ENUM (enum_discriminator_t, (UD_FLOAT, , "x"), (UD_INT32, , "y"))
TYPEDEF_ENUM (enum8_discriminator_t, ATTRIBUTES (__attribute__ ((packed, aligned (sizeof (uint8_t))))), (U8D_FLOAT, , "x"), (U8D_INT32, , "y"))
TYPEDEF_ENUM (enum16_discriminator_t, ATTRIBUTES (__attribute__ ((packed, aligned (sizeof (uint16_t))))), (U16D_FLOAT, , "x"), (U16D_INT32, , "y"))
TYPEDEF_ENUM (enum32_discriminator_t, ATTRIBUTES (__attribute__ ((packed, aligned (sizeof (uint32_t))))), (U32D_FLOAT, , "x"), (U32D_INT32, , "y"))
TYPEDEF_ENUM (enum64_discriminator_t, ATTRIBUTES (__attribute__ ((packed, aligned (sizeof (uint64_t))))), (U64D_FLOAT, , "x"), (U64D_INT32, , "y"))

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
		int dummy,
		ANON_UNION (),
		float x,
		int32_t y,
		END_ANON_UNION ("discriminator"),
		(enum_discriminator_t, discriminator),
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		)

TYPEDEF_STRUCT (struct_named_anon_union_enum_t,
		int dummy,
		ANON_UNION (xy),
		float x,
		int32_t y,
		END_ANON_UNION ("discriminator"),
		(enum_discriminator_t, discriminator),
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		)

TYPEDEF_STRUCT (struct_union_enum8_t,
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(enum8_discriminator_t, discriminator),
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		)

TYPEDEF_STRUCT (struct_union_enum16_t,
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(enum16_discriminator_t, discriminator),
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		)

TYPEDEF_STRUCT (struct_union_enum32_t,
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(enum32_discriminator_t, discriminator),
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		)

TYPEDEF_STRUCT (struct_union_enum64_t,
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(enum64_discriminator_t, discriminator),
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		)

TYPEDEF_STRUCT (struct_union_enum_t,
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(enum_discriminator_t, discriminator),
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		)

TYPEDEF_STRUCT (struct_union_enum_ptr_t,
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(enum_discriminator_t *, discriminator),
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		)

TYPEDEF_STRUCT (struct_union_int8_t,
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(int8_t, discriminator),
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		)

TYPEDEF_STRUCT (struct_union_uint8_t,
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(uint8_t, discriminator),
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		)

TYPEDEF_STRUCT (struct_union_int16_t,
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(int16_t, discriminator),
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		)

TYPEDEF_STRUCT (struct_union_uint16_t,
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(uint16_t, discriminator),
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		)

TYPEDEF_STRUCT (struct_union_int32_t,
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(int32_t, discriminator),
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		)

TYPEDEF_STRUCT (struct_union_uint32_t,
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(uint32_t, discriminator),
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		)

TYPEDEF_STRUCT (struct_union_int64_t,
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(int64_t, discriminator),
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		)

TYPEDEF_STRUCT (struct_union_uint64_t,
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(uint64_t, discriminator),
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		)

TYPEDEF_STRUCT (struct_union_bitfield_t,
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		BITFIELD (enum_discriminator_t, discriminator, :1),
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		)

TYPEDEF_STRUCT (struct_union_string_t,
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(string_t, discriminator),
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		)

TYPEDEF_STRUCT (struct_union_string_ptr_t,
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(string_t *, discriminator),
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		)

TYPEDEF_CHAR_ARRAY (ca2_t, 2)

TYPEDEF_STRUCT (struct_union_ca_t,
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(ca2_t, discriminator),
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		)

TYPEDEF_STRUCT (struct_union_ca_ptr_t,
		int dummy,
		(union_int32_float_t, xy, , "discriminator"),
		(ca2_t *, discriminator),
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		)

TYPEDEF_STRUCT (struct_mr_ptr_t,
		int dummy,
		(mr_ptr_t, ptr, , "discriminator"),
		string_t discriminator,
		(empty_union_t, empty_union),
		(empty_struct_t, empty_struct),
		)
