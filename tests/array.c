#include <check.h>
#include <metaresc.h>
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

#define ASSERT_SAVE_LOAD_ARRAY(METHOD, TYPE, ...) ({	\
      TYPE x = { { 1, 2 } };				\
      ASSERT_SAVE_LOAD (METHOD, TYPE, &x, __VA_ARGS__);	\
    })

MR_START_TEST (numeric_array_int8, "array of numerics") { ALL_METHODS (ASSERT_SAVE_LOAD_ARRAY, int8_array_t); } END_TEST
MR_START_TEST (numeric_array_int16, "array of numerics") { ALL_METHODS (ASSERT_SAVE_LOAD_ARRAY, int16_array_t); } END_TEST
MR_START_TEST (numeric_array_int32, "array of numerics") { ALL_METHODS (ASSERT_SAVE_LOAD_ARRAY, int32_array_t); } END_TEST
MR_START_TEST (numeric_array_int64, "array of numerics") { ALL_METHODS (ASSERT_SAVE_LOAD_ARRAY, int64_array_t); } END_TEST
MR_START_TEST (numeric_array_float, "array of numerics") { ALL_METHODS (ASSERT_SAVE_LOAD_ARRAY, float_array_t); } END_TEST
MR_START_TEST (numeric_array_double, "array of numerics") { ALL_METHODS (ASSERT_SAVE_LOAD_ARRAY, double_array_t); } END_TEST
MR_START_TEST (numeric_array_long_double, "array of numerics") { ALL_METHODS (ASSERT_SAVE_LOAD_ARRAY, ld_array_t); } END_TEST

MR_START_TEST (char_array, "array of chars") {
  char_array_t orig = { { 'a', 'b' }, };
  ALL_METHODS (ASSERT_SAVE_LOAD, char_array_t, &orig);
} END_TEST

MR_START_TEST (enum_array, "array of enums") {
  packed_enum_array_t orig = { { ONE, TWO }, };
  ALL_METHODS (ASSERT_SAVE_LOAD, packed_enum_array_t, &orig);
} END_TEST

MR_START_TEST (string_array, "array of strings") {
  string_array_t orig = { { "ONE", "TWO" }, };
  ALL_METHODS (ASSERT_SAVE_LOAD, string_array_t, &orig);
} END_TEST

MR_START_TEST (two_dimensional_array_enum, "array2d") {
  packed_enum_array2d_t packed_enum_array2d = { { { ZERO, ONE, }, { TWO, THREE, }, } };
  ALL_METHODS (ASSERT_SAVE_LOAD, packed_enum_array2d_t, &packed_enum_array2d);
} END_TEST

MR_START_TEST (two_dimensional_array_string, "array2d") {
  string_array2d_t string_array2d = { { { "ZERO", "ONE", }, { "TWO", "THREE", }, } };
  ALL_METHODS (ASSERT_SAVE_LOAD, string_array2d_t, &string_array2d);
} END_TEST

MR_START_TEST (two_dimensional_array_duplicated_strings, "array2d") {
  string_array2d_t string_array2d_duplicate = { { { "ZERO", "ONE", }, { "ZERO", "ONE", }, } };
  ALL_METHODS (ASSERT_SAVE_LOAD, string_array2d_t, &string_array2d_duplicate);
} END_TEST

MR_START_TEST (two_dimensional_array_int, "array2d") {
  int_array2d_t int_array2d = { { { 0, 1, }, { 2, 3, }, } };
  ALL_METHODS (ASSERT_SAVE_LOAD, int_array2d_t, &int_array2d);
} END_TEST

MAIN ();
