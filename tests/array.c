#include <check.h>
#include <metaresc.h>
#include <mr_save.h> /* MR_ONE_SHIFT */
#include <regression.h>

TYPEDEF_ENUM (packed_enum_t, ATTRIBUTES (__attribute__ ((packed))), ZERO, ONE, TWO, THREE)
TYPEDEF_STRUCT (char_array_t, (char, x, [2]))
TYPEDEF_STRUCT (packed_enum_array_t, (packed_enum_t, x, [2]))
TYPEDEF_STRUCT (int8_array_t, (int8_t, x, [2]))
TYPEDEF_STRUCT (int16_array_t, (int16_t, x, [2]))
TYPEDEF_STRUCT (int32_array_t, (int32_t, x, [2]))
TYPEDEF_STRUCT (int64_array_t, (int64_t, x, [2]))
TYPEDEF_STRUCT (float_array_t, (float, x, [2]))
TYPEDEF_STRUCT (double_array_t, (double, x, [2]))
TYPEDEF_STRUCT (ld_array_t, (long double, x, [2]))
TYPEDEF_STRUCT (string_array_t, (string_t, x, [2]))
TYPEDEF_STRUCT (packed_enum_array2d_t, (packed_enum_t, x, [2][2]))
TYPEDEF_STRUCT (string_array2d_t, (string_t, x, [2][2]))
TYPEDEF_STRUCT (int_array2d_t, (int, x, [2][2]))

TYPEDEF_STRUCT (int_ptr_array_t, (int *, x, [2]))
TYPEDEF_STRUCT (enum_ptr_array_t, (packed_enum_t *, x, [2]))

TYPEDEF_UNION (union_int32_float_t, float _float, int32_t _int32)

TYPEDEF_ENUM (discriminator_t,
	      (UD_FLOAT, , "_float"),
	      (UD_INT32, , "_int32"),
	      )

TYPEDEF_STRUCT (union_ptr_array_t,
		(union_int32_float_t *, x, [2], "discriminator"),
		(discriminator_t, discriminator)
		)

#define OVERRIDE				\
  (mr_ud_override_t[]){				\
    {UD_FLOAT, "_int32"},			\
      {UD_INT32, "_float"},			\
	}

TYPEDEF_STRUCT (ud_overrided_ptr_array_t,
		(union_int32_float_t *, x, [2], "discriminator", { OVERRIDE }, "mr_ud_override_t", sizeof (OVERRIDE)),
		(discriminator_t, discriminator)
		)

#define ASSERT_SAVE_LOAD_ARRAY(METHOD, TYPE, ...) ({	\
      TYPE x = { { 1, 2 } };				\
      ASSERT_SAVE_LOAD (METHOD, TYPE, &x, __VA_ARGS__);	\
    })

START_TEST (numeric_array_int8) { ALL_METHODS (ASSERT_SAVE_LOAD_ARRAY, int8_array_t); } END_TEST
START_TEST (numeric_array_int16) { ALL_METHODS (ASSERT_SAVE_LOAD_ARRAY, int16_array_t); } END_TEST
START_TEST (numeric_array_int32) { ALL_METHODS (ASSERT_SAVE_LOAD_ARRAY, int32_array_t); } END_TEST
START_TEST (numeric_array_int64) { ALL_METHODS (ASSERT_SAVE_LOAD_ARRAY, int64_array_t); } END_TEST
START_TEST (numeric_array_float) { ALL_METHODS (ASSERT_SAVE_LOAD_ARRAY, float_array_t); } END_TEST
START_TEST (numeric_array_double) { ALL_METHODS (ASSERT_SAVE_LOAD_ARRAY, double_array_t); } END_TEST
START_TEST (numeric_array_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_ARRAY, ld_array_t); } END_TEST

START_TEST (char_array) {
  char_array_t orig = { { 'a', 'b' }, };
  ALL_METHODS (ASSERT_SAVE_LOAD, char_array_t, &orig);
} END_TEST

START_TEST (enum_array) {
  packed_enum_array_t orig = { { ONE, TWO }, };
  ALL_METHODS (ASSERT_SAVE_LOAD, packed_enum_array_t, &orig);
} END_TEST

START_TEST (string_array) {
  string_array_t orig = { { "ONE", "TWO" }, };
  ALL_METHODS (ASSERT_SAVE_LOAD, string_array_t, &orig);
} END_TEST

START_TEST (two_dimensional_array_enum) {
  packed_enum_array2d_t packed_enum_array2d = { { { ZERO, ONE, }, { TWO, THREE, }, } };
  ALL_METHODS (ASSERT_SAVE_LOAD, packed_enum_array2d_t, &packed_enum_array2d);
} END_TEST

START_TEST (two_dimensional_array_string) {
  string_array2d_t string_array2d = { { { "ZERO", "ONE", }, { "TWO", "THREE", }, } };
  ALL_METHODS (ASSERT_SAVE_LOAD, string_array2d_t, &string_array2d);
} END_TEST

START_TEST (two_dimensional_array_duplicated_strings) {
  string_array2d_t string_array2d_duplicate = { { { "ZERO", "ONE", }, { "ZERO", "ONE", }, } };
  ALL_METHODS (ASSERT_SAVE_LOAD, string_array2d_t, &string_array2d_duplicate);
} END_TEST

START_TEST (two_dimensional_array_int) {
  int_array2d_t int_array2d = { { { 0, 1, }, { 2, 3, }, } };
  ALL_METHODS (ASSERT_SAVE_LOAD, int_array2d_t, &int_array2d);
} END_TEST

START_TEST (int_ptr_array)
{
  int_ptr_array_t orig = { { (int[]){ 0x12345678 }, NULL } };
  mr_ptrdes_t expected[] =
    {
      {},
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int_ptr_array_t", .name.str = "int_ptr_array_t", }},
	.mr_type = MR_TYPE_STRUCT,
	.flags = MR_IS_UNNAMED,
	.first_child = 2,
	.next = 0
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "x", }},
	.mr_type = MR_TYPE_ARRAY,
	.first_child = 3,
	.next = 0
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "x", }},
	.mr_type = MR_TYPE_POINTER,
	.flags = MR_IS_UNNAMED,
	.first_child = 5,
	.next = 4
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "x", }},
	.mr_type = MR_TYPE_POINTER,
	.flags = MR_IS_UNNAMED | MR_IS_NULL,
	.first_child = 0,
	.next = 0
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "x", }},
	.mr_type = MR_TYPE_INT32,
	.flags = MR_IS_UNNAMED,
	.first_child = 0,
	.next = 0
      }
      ,
    };
  ASSERT_MR_SAVE (int_ptr_array_t, &orig, expected);
  ALL_METHODS (ASSERT_SAVE_LOAD, int_ptr_array_t, &orig);
} END_TEST

START_TEST (enum_ptr_array)
{
  enum_ptr_array_t orig = { { (packed_enum_t[]){ TWO }, NULL } };
  mr_ptrdes_t expected[] =
    {
      {},
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "enum_ptr_array_t", .name.str = "enum_ptr_array_t", }},
	.mr_type = MR_TYPE_STRUCT,
	.flags = MR_IS_UNNAMED,
	.first_child = 2,
	.next = 0
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "x", }},
	.mr_type = MR_TYPE_ARRAY,
	.first_child = 3,
	.next = 0
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "x", }},
	.mr_type = MR_TYPE_POINTER,
	.flags = MR_IS_UNNAMED,
	.first_child = 5,
	.next = 4
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "x", }},
	.mr_type = MR_TYPE_POINTER,
	.flags = MR_IS_UNNAMED | MR_IS_NULL,
	.first_child = 0,
	.next = 0
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "packed_enum_t *", .name.str = "x", }},
	.mr_type = MR_TYPE_ENUM,
	.flags = MR_IS_UNNAMED,
	.first_child = 0,
	.next = 0
      }
      ,
    };
  ASSERT_MR_SAVE (enum_ptr_array_t, &orig, expected);
  ALL_METHODS (ASSERT_SAVE_LOAD, enum_ptr_array_t, &orig);
} END_TEST

START_TEST (union_ptr_array)
{
  union_ptr_array_t orig = { { (union_int32_float_t[]){ { ._float = 1.2345678 } }, NULL }, UD_FLOAT };
  mr_ptrdes_t expected[] =
    {
      {},
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "union_ptr_array_t", .name.str = "union_ptr_array_t", }},
	.mr_type = MR_TYPE_STRUCT,
	.flags = MR_IS_UNNAMED,
	.first_child = 2,
	.next = 0
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "x", }},
	.mr_type = MR_TYPE_ARRAY,
	.first_child = 3,
	.next = 5
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "x", }},
	.mr_type = MR_TYPE_POINTER,
	.flags = MR_IS_UNNAMED,
	.first_child = 6,
	.next = 4
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "x", }},
	.mr_type = MR_TYPE_POINTER,
	.flags = MR_IS_UNNAMED | MR_IS_NULL,
	.first_child = 0,
	.next = 0
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "discriminator_t", .name.str = "discriminator", }},
	.mr_type = MR_TYPE_ENUM,
	.first_child = 0,
	.next = 0
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "union_int32_float_t *", .name.str = "x", }},
	.mr_type = MR_TYPE_UNION,
	.flags = MR_IS_UNNAMED,
	.first_child = 7,
	.next = 0
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "_float", }},
	.mr_type = MR_TYPE_FLOAT,
	.first_child = 0,
	.next = 0
      }
      ,
    };
  ASSERT_MR_SAVE (union_ptr_array_t, &orig, expected);
  ALL_METHODS (ASSERT_SAVE_LOAD, union_ptr_array_t, &orig);
} END_TEST

START_TEST (ud_overrided_ptr_array)
{
  ud_overrided_ptr_array_t orig = { { (union_int32_float_t[]){ { ._float = 1.2345678 } }, NULL }, UD_INT32 };
  mr_ptrdes_t expected[] =
    {
      {},
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "ud_overrided_ptr_array_t", .name.str = "ud_overrided_ptr_array_t", }},
	.mr_type = MR_TYPE_STRUCT,
	.flags = MR_IS_UNNAMED,
	.first_child = 2,
	.next = 0
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "x", }},
	.mr_type = MR_TYPE_ARRAY,
	.first_child = 3,
	.next = 5
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "x", }},
	.mr_type = MR_TYPE_POINTER,
	.flags = MR_IS_UNNAMED,
	.first_child = 6,
	.next = 4
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "x", }},
	.mr_type = MR_TYPE_POINTER,
	.flags = MR_IS_UNNAMED | MR_IS_NULL,
	.first_child = 0,
	.next = 0
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "discriminator_t", .name.str = "discriminator", }},
	.mr_type = MR_TYPE_ENUM,
	.first_child = 0,
	.next = 0
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "union_int32_float_t *", .name.str = "x", }},
	.mr_type = MR_TYPE_UNION,
	.flags = MR_IS_UNNAMED,
	.first_child = 7,
	.next = 0
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "_float", }},
	.mr_type = MR_TYPE_FLOAT,
	.first_child = 0,
	.next = 0
      }
      ,
    };
  ASSERT_MR_SAVE (ud_overrided_ptr_array_t, &orig, expected);
  ALL_METHODS (ASSERT_SAVE_LOAD, ud_overrided_ptr_array_t, &orig);
} END_TEST

START_TEST (string_ptr_array)
{
  char ** orig[] = { NULL, (char*[]){NULL}, (char*[]){"1"}};
  mr_ptrdes_t expected[] =
    {
      {},
      {
	.fdp = (mr_fd_t[]){{ .name.str = "string_t", }},
	.mr_type = MR_TYPE_ARRAY,
	.flags = MR_IS_UNNAMED,
	.first_child = 2,
	.next = 0
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "string_t", }},
	.mr_type = MR_TYPE_POINTER,
	.flags = MR_IS_UNNAMED | MR_IS_NULL,
	.first_child = 0,
	.next = 3
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "string_t", }},
	.mr_type = MR_TYPE_POINTER,
	.flags = MR_IS_UNNAMED,
	.first_child = 5,
	.next = 4
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "string_t", }},
	.mr_type = MR_TYPE_POINTER,
	.flags = MR_IS_UNNAMED,
	.first_child = 6,
	.next = 0
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "string_t", }},
	.mr_type = MR_TYPE_STRING,
	.flags = MR_IS_UNNAMED | MR_IS_NULL,
	.first_child = 0,
	.next = 0
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "string_t", }},
	.mr_type = MR_TYPE_STRING,
	.flags = MR_IS_UNNAMED,
	.first_child = 0,
	.next = 0
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "string_t", }},
	.mr_type = MR_TYPE_CHAR_ARRAY,
	.flags = MR_IS_UNNAMED,
	.first_child = 0,
	.next = 0
      }
      ,
    };
  ASSERT_MR_SAVE ( , orig, expected);
} END_TEST

TYPEDEF_STRUCT (dynamically_limitted_array_t,
		(int32_t, x, [2], "meta", { "size" }, "size_field_name"),
		(int32_t, size)
		);

START_TEST (dynamically_limitted_array)
{
  dynamically_limitted_array_t orig = {{1, 2}};

  mr_ptrdes_t expected_0[] =
    {
      {},
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "dynamically_limitted_array_t", .name.str = "dynamically_limitted_array_t", }},
	.mr_type = MR_TYPE_STRUCT,
	.flags = MR_IS_UNNAMED,
	.first_child = 2,
	.next = 0
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "x", }},
	.mr_type = MR_TYPE_ARRAY,
	.first_child = 0,
	.next = 3
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "size", }},
	.mr_type = MR_TYPE_INT32,
	.first_child = 0,
	.next = 0
      }
      ,
    };

  orig.size = 0;
  ASSERT_MR_SAVE (dynamically_limitted_array_t, &orig, expected_0);

  mr_ptrdes_t expected_1[] =
    {
      {},
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "dynamically_limitted_array_t", .name.str = "dynamically_limitted_array_t", }},
	.mr_type = MR_TYPE_STRUCT,
	.flags = MR_IS_UNNAMED,
	.first_child = 2,
	.next = 0
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "x", }},
	.mr_type = MR_TYPE_ARRAY,
	.first_child = 3,
	.next = 4
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "x", }},
	.mr_type = MR_TYPE_INT32,
	.flags = MR_IS_UNNAMED,
	.first_child = 0,
	.next = 0
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "size", }},
	.mr_type = MR_TYPE_INT32,
	.first_child = 0,
	.next = 0
      }
      ,
    };

  orig.size = sizeof (orig.x[0]);
  ASSERT_MR_SAVE (dynamically_limitted_array_t, &orig, expected_1);

  mr_ptrdes_t expected_3[] =
    {
      {},
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "dynamically_limitted_array_t", .name.str = "dynamically_limitted_array_t", }},
	.mr_type = MR_TYPE_STRUCT,
	.flags = MR_IS_UNNAMED,
	.first_child = 2,
	.next = 0
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "x", }},
	.mr_type = MR_TYPE_ARRAY,
	.first_child = 3,
	.next = 5
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "x", }},
	.mr_type = MR_TYPE_INT32,
	.flags = MR_IS_UNNAMED,
	.first_child = 0,
	.next = 4
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "x", }},
	.mr_type = MR_TYPE_INT32,
	.flags = MR_IS_UNNAMED,
	.first_child = 0,
	.next = 0
      }
      ,
      {
	.fdp = (mr_fd_t[]){{ .name.str = "size", }},
	.mr_type = MR_TYPE_INT32,
	.first_child = 0,
	.next = 0
      }
      ,
    };

  orig.size = 3 * sizeof (orig.x[0]);
  ASSERT_MR_SAVE (dynamically_limitted_array_t, &orig, expected_3);
} END_TEST

START_TEST (dynamically_limitted_array_xdr)
{
#ifdef HAVE_RPC_TYPES_H
  dynamically_limitted_array_t orig = {{1, 2}};
  orig.size = sizeof (orig.x[0]);
  mr_rarray_t ra = MR_SAVE_XDR_RA (dynamically_limitted_array_t, &orig);
  dynamically_limitted_array_t restored;
  memset (&restored, 0, sizeof (restored));
  mr_status_t status = MR_LOAD_XDR_RA (dynamically_limitted_array_t, &ra, &restored);

  ck_assert_msg (status == MR_SUCCESS, "Failed to load from XDR");
  ck_assert_msg (MR_CMP_STRUCTS (dynamically_limitted_array_t, &orig, &restored) == 0, "Restored value mismatched orig");

  if (ra.data.ptr)
    MR_FREE (ra.data.ptr);
#endif /* HAVE_RPC_TYPES_H */
} END_TEST

MAIN_TEST_SUITE ((numeric_array_int8, "array of numerics"),
		 (numeric_array_int16, "array of numerics"),
		 (numeric_array_int32, "array of numerics"),
		 (numeric_array_int64, "array of numerics"),
		 (numeric_array_float, "array of numerics"),
		 (numeric_array_double, "array of numerics"),
		 (numeric_array_long_double, "array of numerics"),
		 (char_array, "array of chars"),
		 (enum_array, "array of enums"),
		 (string_array, "array of strings"),
		 (two_dimensional_array_enum, "array2d"),
		 (two_dimensional_array_string, "array2d"),
		 (two_dimensional_array_duplicated_strings, "array2d"),
		 (two_dimensional_array_int, "array2d"),
		 (int_ptr_array, "array of int pointers"),
		 (enum_ptr_array, "array of pointers on enums"),
		 (union_ptr_array, "array of union pointers"),
		 (ud_overrided_ptr_array, "array of union pointers with overrides"),
		 (string_ptr_array, "array of pointers on strings"),
		 (dynamically_limitted_array, "dynamically limitted array"),
		 (dynamically_limitted_array_xdr, "dynamically limitted array with XDR")
		 );
