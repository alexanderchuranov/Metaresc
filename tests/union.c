#include <math.h>
#include <check.h>
#include <metaresc.h>
#include <regression.h>
#include <flt_values.h>

#include <union.h>

#define ASSERT_SAVE_LOAD_ANON_UNION(METHOD, TYPE, VALUE, ...) ({	\
      TYPE x = { .dummy = 0, "", { M_PI }, VALUE };			\
      ASSERT_SAVE_LOAD (METHOD, TYPE, &x, __VA_ARGS__);			\
    })

#define ASSERT_SAVE_LOAD_UNION(METHOD, TYPE, VALUE, ...) ({	\
      TYPE x = { .dummy = 0, { M_PI }, VALUE };			\
      ASSERT_SAVE_LOAD (METHOD, TYPE, &x, __VA_ARGS__);		\
    })

#define ASSERT_UNION_RESOLUTION(TYPE, VALUE) ({				\
      TYPE orig = { .dummy = 0, { M_PI }, VALUE };			\
      mr_ra_ptrdes_t ptrs = MR_SAVE (TYPE, &orig);			\
      bool union_resolved_correctly = false;				\
      if (ptrs.ra != NULL)						\
	{								\
	  int i;							\
	  for (i = ptrs.size / sizeof (ptrs.ra[0]) - 1; i >= 0; --i)	\
	    {								\
	      if ((ptrs.ra[i].name != NULL) &&				\
		  (0 == strcmp (ptrs.ra[i].name, "y")))			\
		{							\
		  union_resolved_correctly = true;			\
		  break;						\
		}							\
	    }								\
	  MR_FREE (ptrs.ra);						\
	}								\
      ck_assert_msg (union_resolved_correctly, "Union resolved incorrectly"); \
    })

MR_COMPILETIME_ASSERT (sizeof (enum8_discriminator_t) == sizeof (uint8_t));
MR_COMPILETIME_ASSERT (sizeof (enum16_discriminator_t) == sizeof (uint16_t));
MR_COMPILETIME_ASSERT (sizeof (enum32_discriminator_t) == sizeof (uint32_t));
MR_COMPILETIME_ASSERT (sizeof (enum64_discriminator_t) == sizeof (uint64_t));

START_TEST (embed_anon_union) {
  struct_embed_anon_union_t orig = { 0, { { -1 } }, };
  ALL_METHODS (ASSERT_SAVE_LOAD, struct_embed_anon_union_t, &orig);
} END_TEST

START_TEST (anon_union) {
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_anon_union_enum_t, UD_FLOAT);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_anon_union_enum_t, UD_INT32);
} END_TEST

START_TEST (named_anon_union) {
  ALL_METHODS (ASSERT_SAVE_LOAD_ANON_UNION, struct_named_anon_union_enum_t, UD_FLOAT);
  ALL_METHODS (ASSERT_SAVE_LOAD_ANON_UNION, struct_named_anon_union_enum_t, UD_INT32);
} END_TEST

#define STRUCT_XY_X_CMP(TYPE, X, Y, ...) ((X)->xy.x != (Y)->xy.x)

START_TEST (union_enum) {
  ASSERT_UNION_RESOLUTION (struct_union_enum_t, UD_INT32);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_enum_t, UD_FLOAT, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_enum_t, UD_INT32, STRUCT_XY_X_CMP);
} END_TEST

START_TEST (union_enum8) {
  ASSERT_UNION_RESOLUTION (struct_union_enum8_t, U8D_INT32);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_enum8_t, U8D_FLOAT, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_enum8_t, U8D_INT32, STRUCT_XY_X_CMP);
} END_TEST

START_TEST (union_enum16) {
  ASSERT_UNION_RESOLUTION (struct_union_enum16_t, U16D_INT32);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_enum16_t, U16D_FLOAT, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_enum16_t, U16D_INT32, STRUCT_XY_X_CMP);
} END_TEST

START_TEST (union_enum32) {
  ASSERT_UNION_RESOLUTION (struct_union_enum32_t, U32D_INT32);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_enum32_t, U32D_FLOAT, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_enum32_t, U32D_INT32, STRUCT_XY_X_CMP);
} END_TEST

START_TEST (union_enum64) {
  ASSERT_UNION_RESOLUTION (struct_union_enum64_t, U64D_INT32);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_enum64_t, U64D_FLOAT, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_enum64_t, U64D_INT32, STRUCT_XY_X_CMP);
} END_TEST

START_TEST (union_bitfield) {
  ASSERT_UNION_RESOLUTION (struct_union_bitfield_t, UD_INT32);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_bitfield_t, UD_FLOAT, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_bitfield_t, UD_INT32, STRUCT_XY_X_CMP);
} END_TEST

START_TEST (union_enum_ptr) {
  ASSERT_UNION_RESOLUTION (struct_union_enum_ptr_t, (enum_discriminator_t[]){ UD_INT32 });
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_enum_ptr_t, (enum_discriminator_t[]){ UD_FLOAT }, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_enum_ptr_t, (enum_discriminator_t[]){ UD_INT32 }, STRUCT_XY_X_CMP);
} END_TEST

START_TEST (union_int8) {
  ASSERT_UNION_RESOLUTION (struct_union_int8_t, 1);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_int8_t, 0, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_int8_t, 1, STRUCT_XY_X_CMP);
} END_TEST

START_TEST (union_int16) {
  ASSERT_UNION_RESOLUTION (struct_union_int16_t, 1);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_int16_t, 1, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_int16_t, 0, STRUCT_XY_X_CMP);
} END_TEST

START_TEST (union_int32) {
  ASSERT_UNION_RESOLUTION (struct_union_int32_t, 1);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_int32_t, 0, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_int32_t, 1, STRUCT_XY_X_CMP);
} END_TEST

START_TEST (union_int64) {
  ASSERT_UNION_RESOLUTION (struct_union_int64_t, 1);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_int64_t, 0, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_int64_t, 1, STRUCT_XY_X_CMP);
} END_TEST

START_TEST (union_uint8) {
  ASSERT_UNION_RESOLUTION (struct_union_uint8_t, 1);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_uint8_t, 0, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_uint8_t, 1, STRUCT_XY_X_CMP);
} END_TEST

START_TEST (union_uint16) {
  ASSERT_UNION_RESOLUTION (struct_union_uint16_t, 1);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_uint16_t, 1, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_uint16_t, 0, STRUCT_XY_X_CMP);
} END_TEST

START_TEST (union_uint32) {
  ASSERT_UNION_RESOLUTION (struct_union_uint32_t, 1);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_uint32_t, 0, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_uint32_t, 1, STRUCT_XY_X_CMP);
} END_TEST

START_TEST (union_uint64) {
  ASSERT_UNION_RESOLUTION (struct_union_uint64_t, 1);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_uint64_t, 0, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_uint64_t, 1, STRUCT_XY_X_CMP);
} END_TEST

START_TEST (union_str) {
  ASSERT_UNION_RESOLUTION (struct_union_string_t, "y");
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_string_t, "x", STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_string_t, "y", STRUCT_XY_X_CMP);
} END_TEST

START_TEST (union_ca) {
  ASSERT_UNION_RESOLUTION (struct_union_ca_t, "y");
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_ca_t, "x", STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_ca_t, "y", STRUCT_XY_X_CMP);
} END_TEST

START_TEST (union_str_ptr) {
  ASSERT_UNION_RESOLUTION (struct_union_string_ptr_t, (string_t[]) { "y" });
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_string_ptr_t, (string_t[]) { "x" }, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_string_ptr_t, (string_t[]) { "y" }, STRUCT_XY_X_CMP);
} END_TEST

START_TEST (union_ca_ptr) {
  ASSERT_UNION_RESOLUTION (struct_union_ca_ptr_t, (ca2_t[]){ "y" });
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_ca_ptr_t, (ca2_t[]){ "x" }, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_ca_ptr_t, (ca2_t[]){ "y" }, STRUCT_XY_X_CMP);
} END_TEST

MAIN_TEST_SUITE ((embed_anon_union, "embeded anonymous union"),
		 (anon_union, "anonymous union"),
		 (named_anon_union, "named anonymous union"),
		 (union_enum, "union discriminated by enum"),
		 (union_enum8, "union discriminated by enum8"),
		 (union_enum16, "union discriminated by enum16"),
		 (union_enum32, "union discriminated by enum32"),
		 (union_enum64, "union discriminated by enum64"),
		 (union_bitfield, "union discriminated by bitfield"),
		 (union_enum_ptr, "union discriminated by pointer on enum"),
		 (union_int8, "union discriminated by int8_t"),
		 (union_int16, "union discriminated by int16_t"),
		 (union_int32, "union discriminated by int32_t"),
		 (union_int64, "union discriminated by int64_t"),
		 (union_uint8, "union discriminated by uint8_t"),
		 (union_uint16, "union discriminated by uint16_t"),
		 (union_uint32, "union discriminated by uint32_t"),
		 (union_uint64, "union discriminated by uint64_t"),
		 (union_str, "union discriminated by string"),
		 (union_ca, "union discriminated by char array"),
		 (union_str_ptr, "union discriminated by pointer on string"),
		 (union_ca_ptr, "union discriminated by pointer on char array")
		 );
