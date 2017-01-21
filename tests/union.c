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
      int i;								\
      bool union_resolved_correctly = false;				\
      if (ptrs.ra != NULL)						\
	{								\
	  for (i = ptrs.size / sizeof (ptrs.ra[0]) - 1; i >= 0; --i)	\
	    {								\
	      if ((ptrs.ra[i].fd.name.str != NULL) &&			\
		  (0 == strcmp (ptrs.ra[i].fd.name.str, "y")))		\
		{							\
		  union_resolved_correctly = true;			\
		  break;						\
		}							\
	    }								\
	  MR_FREE (ptrs.ra);						\
	}								\
      ck_assert_msg (union_resolved_correctly, "Union resolved incorrectly"); \
    })
  
MR_START_TEST (embed_anon_union, "embeded anonymous union") {
  struct_embed_anon_union_t orig = { 0, { { -1 } }, };
  ALL_METHODS (ASSERT_SAVE_LOAD, struct_embed_anon_union_t, &orig);
} END_TEST

MR_START_TEST (anon_union, "anonymous union") {
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_anon_union_enum_t, UD_FLOAT, STRUCT_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_anon_union_enum_t, UD_INT32, STRUCT_X_CMP);
} END_TEST

MR_START_TEST (named_anon_union, "named anonymous union") {
  ALL_METHODS (ASSERT_SAVE_LOAD_ANON_UNION, struct_named_anon_union_enum_t, UD_FLOAT, STRUCT_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_ANON_UNION, struct_named_anon_union_enum_t, UD_INT32, STRUCT_X_CMP);
} END_TEST

#define STRUCT_XY_X_CMP(TYPE, X, Y, ...) ((X)->xy.x != (Y)->xy.x)

MR_START_TEST (union_enum, "union discriminated by enum") {
  ASSERT_UNION_RESOLUTION (struct_union_enum_t, UD_INT32);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_enum_t, UD_FLOAT, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_enum_t, UD_INT32, STRUCT_XY_X_CMP);
} END_TEST

MR_START_TEST (union_enum8, "union discriminated by enum8") {
  ASSERT_UNION_RESOLUTION (struct_union_enum8_t, U8D_INT32);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_enum8_t, U8D_FLOAT, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_enum8_t, U8D_INT32, STRUCT_XY_X_CMP);
} END_TEST

MR_START_TEST (union_enum16, "union discriminated by enum16") {
  ASSERT_UNION_RESOLUTION (struct_union_enum16_t, U16D_INT32);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_enum16_t, U16D_FLOAT, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_enum16_t, U16D_INT32, STRUCT_XY_X_CMP);
} END_TEST

MR_START_TEST (union_enum32, "union discriminated by enum32") {
  ASSERT_UNION_RESOLUTION (struct_union_enum32_t, U32D_INT32);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_enum32_t, U32D_FLOAT, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_enum32_t, U32D_INT32, STRUCT_XY_X_CMP);
} END_TEST

MR_START_TEST (union_enum64, "union discriminated by enum64") {
  ASSERT_UNION_RESOLUTION (struct_union_enum64_t, U64D_INT32);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_enum64_t, U64D_FLOAT, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_enum64_t, U64D_INT32, STRUCT_XY_X_CMP);
} END_TEST

MR_START_TEST (union_bitfield, "union discriminated by bitfield") {
  ASSERT_UNION_RESOLUTION (struct_union_bitfield_t, UD_INT32);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_bitfield_t, UD_FLOAT, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_bitfield_t, UD_INT32, STRUCT_XY_X_CMP);
} END_TEST

MR_START_TEST (union_enum_ptr, "union discriminated by pointer on enum") {
  ASSERT_UNION_RESOLUTION (struct_union_enum_ptr_t, (enum_discriminator_t[]){ UD_INT32 });
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_enum_ptr_t, (enum_discriminator_t[]){ UD_FLOAT }, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_enum_ptr_t, (enum_discriminator_t[]){ UD_INT32 }, STRUCT_XY_X_CMP);
} END_TEST

MR_START_TEST (union_int8, "union discriminated by int8_t") {
  ASSERT_UNION_RESOLUTION (struct_union_int8_t, 1);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_int8_t, 0, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_int8_t, 1, STRUCT_XY_X_CMP);
} END_TEST

MR_START_TEST (union_int16, "union discriminated by int16_t") {
  ASSERT_UNION_RESOLUTION (struct_union_int16_t, 1);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_int16_t, 1, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_int16_t, 0, STRUCT_XY_X_CMP);
} END_TEST

MR_START_TEST (union_int32, "union discriminated by int32_t") {
  ASSERT_UNION_RESOLUTION (struct_union_int32_t, 1);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_int32_t, 0, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_int32_t, 1, STRUCT_XY_X_CMP);
} END_TEST

MR_START_TEST (union_int64, "union discriminated by int64_t") {
  ASSERT_UNION_RESOLUTION (struct_union_int64_t, 1);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_int64_t, 0, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_int64_t, 1, STRUCT_XY_X_CMP);
} END_TEST

MR_START_TEST (union_uint8, "union discriminated by uint8_t") {
  ASSERT_UNION_RESOLUTION (struct_union_uint8_t, 1);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_uint8_t, 0, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_uint8_t, 1, STRUCT_XY_X_CMP);
} END_TEST

MR_START_TEST (union_uint16, "union discriminated by uint16_t") {
  ASSERT_UNION_RESOLUTION (struct_union_uint16_t, 1);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_uint16_t, 1, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_uint16_t, 0, STRUCT_XY_X_CMP);
} END_TEST

MR_START_TEST (union_uint32, "union discriminated by uint32_t") {
  ASSERT_UNION_RESOLUTION (struct_union_uint32_t, 1);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_uint32_t, 0, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_uint32_t, 1, STRUCT_XY_X_CMP);
} END_TEST

MR_START_TEST (union_uint64, "union discriminated by uint64_t") {
  ASSERT_UNION_RESOLUTION (struct_union_uint64_t, 1);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_uint64_t, 0, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_uint64_t, 1, STRUCT_XY_X_CMP);
} END_TEST

MR_START_TEST (union_str, "union discriminated by string") {
  ASSERT_UNION_RESOLUTION (struct_union_string_t, "y");
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_string_t, "x", STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_string_t, "y", STRUCT_XY_X_CMP);
} END_TEST

MR_START_TEST (union_ca, "union discriminated by char array") {
  ASSERT_UNION_RESOLUTION (struct_union_ca_t, "y");
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_ca_t, "x", STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_ca_t, "y", STRUCT_XY_X_CMP);
} END_TEST

MR_START_TEST (union_str_ptr, "union discriminated by pointer on string") {
  ASSERT_UNION_RESOLUTION (struct_union_string_ptr_t, (string_t[]) { "y" });
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_string_ptr_t, (string_t[]) { "x" }, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_string_ptr_t, (string_t[]) { "y" }, STRUCT_XY_X_CMP);
} END_TEST

MR_START_TEST (union_ca_ptr, "union discriminated by pointer on char array") {
  ASSERT_UNION_RESOLUTION (struct_union_ca_ptr_t, (ca2_t[]){ "y" });
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_ca_ptr_t, (ca2_t[]){ "x" }, STRUCT_XY_X_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_UNION, struct_union_ca_ptr_t, (ca2_t[]){ "y" }, STRUCT_XY_X_CMP);
} END_TEST

MAIN ();
