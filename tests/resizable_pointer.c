#include <check.h>
#include <metaresc.h>
#include <regression.h>

#define CMP_RPTR_SIZE(TYPE, X, Y, ...) (((X)->size != (Y)->size) || memcmp ((X)->data, (Y)->data, (X)->size))
#define CMP_RPTR_COUNT(TYPE, X, Y, ...) (((X)->count != (Y)->count) || memcmp ((X)->data, (Y)->data, sizeof ((X)->data[0]) * (X)->count))

/********************************************************************************************************/

TYPEDEF_STRUCT (resizable_array_size_t,
		(int32_t *, data, , , { "size" }, "size_field_name"),
		int size,
		);

START_TEST (resizable_array_size) {
  resizable_array_size_t orig =
    {
      .data = (__typeof__ (orig.data[0])[]){ -1, 1, },
      .size = 2 * sizeof (orig.data[0]),
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_size_t, &orig, CMP_RPTR_SIZE);
} END_TEST

/********************************************************************************************************/

TYPEDEF_STRUCT (resizable_array_count_t,
		(int32_t *, data, , , { "count" }, "count_field_name"),
		int count,
		);

START_TEST (resizable_array_count) {
  resizable_array_count_t orig =
    {
      .data = (__typeof__ (orig.data[0])[]){ -1, 1, },
      .count = 2,
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_count_t, &orig, CMP_RPTR_COUNT);
} END_TEST

/********************************************************************************************************/

TYPEDEF_STRUCT (resizable_array_size_as_bool_t,
		(int8_t *, data, , , { "size" }, "size_field_name"),
		bool size,
		);

START_TEST (resizable_array_size_as_bool) {
  resizable_array_size_as_bool_t orig =
    {
      .data = (__typeof__ (orig.data[0])[]){ -1, },
      .size = 1 * sizeof (orig.data[0]),
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_size_as_bool_t, &orig, CMP_RPTR_SIZE);
} END_TEST

/********************************************************************************************************/

TYPEDEF_STRUCT (resizable_array_count_as_bool_t,
		(int32_t *, data, , , { "count" }, "count_field_name"),
		bool count,
		);

START_TEST (resizable_array_count_as_bool) {
  resizable_array_count_as_bool_t orig =
    {
      .data = (__typeof__ (orig.data[0])[]){ -1, },
      .count = 1,
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_count_as_bool_t, &orig, CMP_RPTR_COUNT);
} END_TEST

/********************************************************************************************************/

TYPEDEF_STRUCT (resizable_array_size_as_int8_t,
		(int8_t *, data, , , { "size" }, "size_field_name"),
		int8_t size,
		);

START_TEST (resizable_array_size_as_int8) {
  resizable_array_size_as_int8_t orig =
    {
      .data = (__typeof__ (orig.data[0])[]){ -1, 1, },
      .size = 2 * sizeof (orig.data[0]),
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_size_as_int8_t, &orig, CMP_RPTR_SIZE);
} END_TEST

/********************************************************************************************************/

TYPEDEF_STRUCT (resizable_array_count_as_int8_t,
		(int8_t *, data, , , { "count" }, "count_field_name"),
		int8_t count,
		);

START_TEST (resizable_array_count_as_int8) {
  resizable_array_count_as_int8_t orig =
    {
      .data = (__typeof__ (orig.data[0])[]){ -1, 1, },
      .count = 2,
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_count_as_int8_t, &orig, CMP_RPTR_COUNT);
} END_TEST

/********************************************************************************************************/

TYPEDEF_STRUCT (resizable_array_size_as_int16_t,
		(int8_t *, data, , , { "size" }, "size_field_name"),
		int16_t size,
		);

START_TEST (resizable_array_size_as_int16) {
  resizable_array_size_as_int16_t orig =
    {
      .data = (__typeof__ (orig.data[0])[]){ -1, 1, },
      .size = 2 * sizeof (orig.data[0]),
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_size_as_int16_t, &orig, CMP_RPTR_SIZE);
} END_TEST

/********************************************************************************************************/

TYPEDEF_STRUCT (resizable_array_count_as_int16_t,
		(int8_t *, data, , , { "count" }, "count_field_name"),
		int16_t count,
		);

START_TEST (resizable_array_count_as_int16) {
  resizable_array_count_as_int16_t orig =
    {
      .data = (__typeof__ (orig.data[0])[]){ -1, 1, },
      .count = 2,
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_count_as_int16_t, &orig, CMP_RPTR_COUNT);
} END_TEST

/********************************************************************************************************/

TYPEDEF_STRUCT (resizable_array_size_as_int32_t,
		(int8_t *, data, , , { "size" }, "size_field_name"),
		int32_t size,
		);

START_TEST (resizable_array_size_as_int32) {
  resizable_array_size_as_int32_t orig =
    {
      .data = (__typeof__ (orig.data[0])[]){ -1, 1, },
      .size = 2 * sizeof (orig.data[0]),
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_size_as_int32_t, &orig, CMP_RPTR_SIZE);
} END_TEST

/********************************************************************************************************/

TYPEDEF_STRUCT (resizable_array_count_as_int32_t,
		(int8_t *, data, , , { "count" }, "count_field_name"),
		int32_t count,
		);

START_TEST (resizable_array_count_as_int32) {
  resizable_array_count_as_int32_t orig =
    {
      .data = (__typeof__ (orig.data[0])[]){ -1, 1, },
      .count = 2,
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_count_as_int32_t, &orig, CMP_RPTR_COUNT);
} END_TEST

/********************************************************************************************************/

TYPEDEF_STRUCT (resizable_array_size_as_int64_t,
		(int8_t *, data, , , { "size" }, "size_field_name"),
		int64_t size,
		);

START_TEST (resizable_array_size_as_int64) {
  resizable_array_size_as_int64_t orig =
    {
      .data = (__typeof__ (orig.data[0])[]){ -1, 1, },
      .size = 2 * sizeof (orig.data[0]),
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_size_as_int64_t, &orig, CMP_RPTR_SIZE);
} END_TEST

/********************************************************************************************************/

TYPEDEF_STRUCT (resizable_array_count_as_int64_t,
		(int8_t *, data, , , { "count" }, "count_field_name"),
		int64_t count,
		);

START_TEST (resizable_array_count_as_int64) {
  resizable_array_count_as_int64_t orig =
    {
      .data = (__typeof__ (orig.data[0])[]){ -1, 1, },
      .count = 2,
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_count_as_int64_t, &orig, CMP_RPTR_COUNT);
} END_TEST

/********************************************************************************************************/

TYPEDEF_STRUCT (resizable_array_size_as_bitfield_t,
		(int8_t *, data, , , { "size" }, "size_field_name"),
		(int8_t, size, :3),
		);

START_TEST (resizable_array_size_as_bitfield) {
  resizable_array_size_as_bitfield_t orig =
    {
      .data = (__typeof__ (orig.data[0])[]){ -1, 1, },
      .size = 2 * sizeof (orig.data[0]),
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_size_as_bitfield_t, &orig, CMP_RPTR_SIZE);
} END_TEST

/********************************************************************************************************/

TYPEDEF_STRUCT (resizable_array_count_as_bitfield_t,
		(int8_t *, data, , , { "count" }, "count_field_name"),
		(int8_t, count, :3),
		);

START_TEST (resizable_array_count_as_bitfield) {
  resizable_array_count_as_bitfield_t orig =
    {
      .data = (__typeof__ (orig.data[0])[]){ -1, 1, },
      .count = 2,
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_count_as_bitfield_t, &orig, CMP_RPTR_COUNT);
} END_TEST

/********************************************************************************************************/

TYPEDEF_ENUM (array_size_t,
	      (ZERO, = 0),
	      (ONE, = sizeof (int8_t)),
	      (TWO, = 2 * sizeof (int8_t)),
	      );

TYPEDEF_STRUCT (resizable_array_size_as_enum_t,
		(int8_t *, data, , , { "size" }, "size_field_name"),
		(array_size_t, size),
		);

START_TEST (resizable_array_size_as_enum) {
  resizable_array_size_as_enum_t orig =
    {
      .data = (__typeof__ (orig.data[0])[]){ -1, 1, },
      .size = TWO,
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_size_as_enum_t, &orig, CMP_RPTR_SIZE);
} END_TEST

/********************************************************************************************************/

TYPEDEF_ENUM (array_count_t,
	      (CNT_ZERO, = 0),
	      (CNT_ONE, = 1),
	      (CNT_TWO, = 2),
	      );

TYPEDEF_STRUCT (resizable_array_count_as_enum_t,
		(int8_t *, data, , , { "count" }, "count_field_name"),
		(array_count_t, count),
		);

START_TEST (resizable_array_count_as_enum) {
  resizable_array_count_as_enum_t orig =
    {
      .data = (__typeof__ (orig.data[0])[]){ -1, 1, },
      .count = CNT_TWO,
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_count_as_enum_t, &orig, CMP_RPTR_COUNT);
} END_TEST

/********************************************************************************************************/

#define CMP_RPTR_SIZEPTR(TYPE, X, Y, ...) ((*(X)->size != *(Y)->size) || memcmp ((X)->data, (Y)->data, *(X)->size))

TYPEDEF_STRUCT (resizable_array_size_as_int_ptr_t,
		(int8_t *, data, , , { "size" }, "size_field_name"),
		(int *, size),
		);

START_TEST (resizable_array_size_as_int_ptr) {
  resizable_array_size_as_int_ptr_t orig =
    {
      .data = (__typeof__ (orig.data[0])[]){ -1, 1, },
      .size = (int[]) { 2 * sizeof (orig.data[0]) },
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_size_as_int_ptr_t, &orig, CMP_RPTR_SIZEPTR);
} END_TEST

/********************************************************************************************************/

#define CMP_RPTR_COUNTPTR(TYPE, X, Y, ...) ((*(X)->count != *(Y)->count) || memcmp ((X)->data, (Y)->data, *(X)->count * sizeof ((X)->data[0])))

TYPEDEF_STRUCT (resizable_array_count_as_int_ptr_t,
		(int8_t *, data, , , { "count" }, "count_field_name"),
		(int *, count),
		);

START_TEST (resizable_array_count_as_int_ptr) {
  resizable_array_count_as_int_ptr_t orig =
    {
      .data = (__typeof__ (orig.data[0])[]){ -1, 1, },
      .count = (int[]) { 2 },
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_count_as_int_ptr_t, &orig, CMP_RPTR_COUNTPTR);
} END_TEST

/********************************************************************************************************/

TYPEDEF_STRUCT (resizable_array_without_size_t,
		(int8_t *, data, , , { "size" }, "size_field_name"),
		VOID (int, size),
		);

START_TEST (resizable_array_without_size) {
  resizable_array_without_size_t orig =
    {
      .data = (__typeof__ (orig.data[0])[]){ -1, 1, },
      .size = 2 * sizeof (orig.data[0]),
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_without_size_t, &orig, CMP_RPTR_SIZE);
} END_TEST

/********************************************************************************************************/

TYPEDEF_STRUCT (resizable_array_without_count_t,
		(int8_t *, data, , , { "count" }, "count_field_name"),
		VOID (int, count),
		);

START_TEST (resizable_array_without_count) {
  resizable_array_without_count_t orig =
    {
      .data = (__typeof__ (orig.data[0])[]){ -1, 1, },
      .count = 2,
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_without_count_t, &orig, CMP_RPTR_COUNT);
} END_TEST

/********************************************************************************************************/

TYPEDEF_STRUCT (resizable_array_size_casted_by_offset_t,
		(int8_t *, data, , , { .size_field_offset = offsetof (resizable_array_size_casted_by_offset_t, size) }, "size_field_offset"),
		int size,
		);

START_TEST (resizable_array_size_casted_by_offset) {
  resizable_array_size_casted_by_offset_t orig =
    {
      .data = (__typeof__ (orig.data[0])[]){ -1, 1, },
      .size = 2 * sizeof (orig.data[0]),
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_size_casted_by_offset_t, &orig, CMP_RPTR_SIZE);
} END_TEST

/********************************************************************************************************/

TYPEDEF_STRUCT (resizable_array_count_casted_by_offset_t,
		(int8_t *, data, , , { .count_field_offset = offsetof (resizable_array_count_casted_by_offset_t, count) }, "count_field_offset"),
		int count,
		);

START_TEST (resizable_array_count_casted_by_offset) {
  resizable_array_count_casted_by_offset_t orig =
    {
      .data = (__typeof__ (orig.data[0])[]){ -1, 1, },
      .count = 2,
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_count_casted_by_offset_t, &orig, CMP_RPTR_COUNT);
} END_TEST

/********************************************************************************************************/

TYPEDEF_STRUCT (resizable_array_without_size_casted_by_offset_t,
		(int8_t *, data, , , { .size_field_offset = offsetof (resizable_array_without_size_casted_by_offset_t, size) }, "size_field_offset"),
		VOID (int, size),
		);

START_TEST (resizable_array_without_size_casted_by_offset) {
  resizable_array_without_size_casted_by_offset_t orig =
    {
      .data = (__typeof__ (orig.data[0])[]){ -1, 1, },
      .size = 2 * sizeof (orig.data[0]),
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_without_size_casted_by_offset_t, &orig, CMP_RPTR_SIZE);
} END_TEST

/********************************************************************************************************/

TYPEDEF_STRUCT (resizable_array_without_count_casted_by_offset_t,
		(int8_t *, data, , , { .count_field_offset = offsetof (resizable_array_without_count_casted_by_offset_t, count) }, "count_field_offset"),
		VOID (int, count),
		);

START_TEST (resizable_array_without_count_casted_by_offset) {
  resizable_array_without_count_casted_by_offset_t orig =
    {
      .data = (__typeof__ (orig.data[0])[]){ -1, 1, },
      .count = 2,
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_without_count_casted_by_offset_t, &orig, CMP_RPTR_COUNT);
} END_TEST

/********************************************************************************************************/

TYPEDEF_STRUCT (resizable_array_casted_by_string_t,
		(int8_t *, data, , , { "size" }, "size_field_name"),
		int size,
		);

START_TEST (resizable_array_casted_by_string) {
  resizable_array_casted_by_string_t orig =
    {
      .data = (__typeof__ (orig.data[0])[]){ -1, 1, },
      .size = 2 * sizeof (orig.data[0]),
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_casted_by_string_t, &orig, CMP_RPTR_SIZE);
} END_TEST

/********************************************************************************************************/

TYPEDEF_STRUCT (resizable_array_without_size_casted_by_string_t,
		(int8_t *, data, , , { "size" }, "size_field_name"),
		VOID (int, size),
		);

START_TEST (resizable_array_without_size_casted_by_string) {
  resizable_array_without_size_casted_by_string_t orig =
    {
      .data = (__typeof__ (orig.data[0])[]){ -1, 1, },
      .size = 2 * sizeof (orig.data[0]),
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_without_size_casted_by_string_t, &orig, CMP_RPTR_SIZE);
} END_TEST

/********************************************************************************************************/

TYPEDEF_STRUCT (resizable_array_without_count_casted_by_string_t,
		(int8_t *, data, , , { "count" }, "count_field_name"),
		VOID (int, count),
		);

START_TEST (resizable_array_without_count_casted_by_string) {
  resizable_array_without_count_casted_by_string_t orig =
    {
      .data = (__typeof__ (orig.data[0])[]){ -1, 1, },
      .count = 2,
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_without_count_casted_by_string_t, &orig, CMP_RPTR_COUNT);
} END_TEST

/********************************************************************************************************/

TYPEDEF_STRUCT (resizable_mr_ptr_t,
		(mr_ptr_t, data, , "dynamic_type"),
		int MR_SIZE,
		(char *, dynamic_type),
		);

START_TEST (resizable_mr_ptr) {
  resizable_mr_ptr_t orig =
    {
      .data = { (resizable_mr_ptr_t[]){ { { NULL }, 0, "mr_ptr_t", }, { { NULL }, 0, "string_t", }, } },
      .MR_SIZE = 2 * sizeof (resizable_mr_ptr_t),
      .dynamic_type = "resizable_mr_ptr_t",
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_mr_ptr_t, &orig);
} END_TEST

/********************************************************************************************************/

#ifdef HAVE_RPC_TYPES_H
#undef TEST_METHODS
#define TEST_METHODS XDR

TYPEDEF_STRUCT (resizable_array_as_opaque_data_t,
		(int8_t *, MR_OPAQUE_DATA, , , { "size" }, "size_field_name"),
		VOID (int, size),
		);

START_TEST (resizable_array_as_opaque_data) {
  resizable_array_as_opaque_data_t orig =
    {
      .MR_OPAQUE_DATA = (__typeof__ (orig.MR_OPAQUE_DATA[0])[]){ -1, 1, },
      .size = 2 * sizeof (orig.MR_OPAQUE_DATA[0]),
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_as_opaque_data_t, &orig);
} END_TEST
#endif /* HAVE_RPC_TYPES_H */

  /********************************************************************************************************/

MAIN_TEST_SUITE (
		 (resizable_array_size, "test pointer as a resizable array casted by name of size field as char[]"),
		 (resizable_array_count, "test pointer as a resizable array casted by name of count field as char[]"),
		 (resizable_array_size_as_bool, "test pointer as a resizable array with size typed as bool"),
		 (resizable_array_count_as_bool, "test pointer as a resizable array with count typed as bool"),
		 (resizable_array_size_as_int8, "test pointer as a resizable array with size typed as int8_t"),
		 (resizable_array_count_as_int8, "test pointer as a resizable array with count typed as int8_t"),
		 (resizable_array_size_as_int16, "test pointer as a resizable array with size typed as int16_t"),
		 (resizable_array_count_as_int16, "test pointer as a resizable array with count typed as int16_t"),
		 (resizable_array_size_as_int32, "test pointer as a resizable array with size typed as int32_t"),
		 (resizable_array_count_as_int32, "test pointer as a resizable array with count typed as int32_t"),
		 (resizable_array_size_as_int64, "test pointer as a resizable array with size typed as int64_t"),
		 (resizable_array_count_as_int64, "test pointer as a resizable array with count typed as int64_t"),
		 (resizable_array_size_as_bitfield, "test pointer as a resizable array with size typed as bitfield"),
		 (resizable_array_count_as_bitfield, "test pointer as a resizable array with count typed as bitfield"),
		 (resizable_array_size_as_enum, "test pointer as a resizable array with size typed as enum"),
		 (resizable_array_count_as_enum, "test pointer as a resizable array with count typed as enum"),
		 (resizable_array_size_as_int_ptr, "test pointer as a resizable array with size typed as pointer to int"),
		 (resizable_array_count_as_int_ptr, "test pointer as a resizable array with count typed as pointer to int"),
		 (resizable_array_without_size, "test pointer as a resizable array with non-serializable size casted by size as a string"),
		 (resizable_array_without_count, "test pointer as a resizable array with non-serializable count casted by size as a string"),
		 (resizable_array_size_casted_by_offset, "test pointer as a resizable array casted by offset of the size field"),
		 (resizable_array_count_casted_by_offset, "test pointer as a resizable array casted by offset of the count field"),
		 (resizable_array_without_size_casted_by_offset, "test pointer as a resizable array with non-serializable size casted by offset of the size field"),
		 (resizable_array_without_count_casted_by_offset, "test pointer as a resizable array with non-serializable count casted by offset of the size field"),
		 (resizable_array_casted_by_string, "test pointer as a resizable array casted by name of the size field as string"),
		 (resizable_array_casted_by_string, "test pointer as a resizable array casted by name of the count field as string"),
		 (resizable_array_without_size_casted_by_string, "test pointer as a resizable array with non-serializable size casted by name of the size field as string"),
		 (resizable_array_without_count_casted_by_string, "test pointer as a resizable array with non-serializable count casted by name of the size field as string"),
		 (resizable_mr_ptr, "test mr_ptr_t as a resizable array")
#ifdef HAVE_RPC_TYPES_H
		 , (resizable_array_as_opaque_data, "test pointer as a resizable array as opaque data")
#endif /* HAVE_RPC_TYPES_H */
		 );
