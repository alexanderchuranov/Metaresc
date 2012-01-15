#include <check.h>
#include <reslib.h>
#include <regression.h>

TYPEDEF_ENUM (packed_enum_t, ATTRIBUTES (__attribute__ ((packed, aligned(sizeof (uint16_t))))), ZERO, ONE, TWO)
TYPEDEF_STRUCT (char_array_t, (char, x, [2]))
TYPEDEF_STRUCT (packed_enum_array_t, (packed_enum_t, x, [2]))
TYPEDEF_STRUCT (int8_array_t, (int8_t, x, [2]))
TYPEDEF_STRUCT (int64_array_t, (int64_t, x, [2]))
TYPEDEF_STRUCT (ld_array_t, (long double, x, [2]))
TYPEDEF_STRUCT (string_array_t, (string_t, x, [2]))

RL_START_TEST (char_array, "array of chars") {
  char_array_t orig = { { 'a', 'b' }, };
  ALL_METHODS (ASSERT_SAVE_LOAD, char_array_t, &orig);
} END_TEST

RL_START_TEST (enum_array, "array of enums") {
  packed_enum_array_t orig = { { ONE, TWO }, };
  ALL_METHODS (ASSERT_SAVE_LOAD, packed_enum_array_t, &orig);
} END_TEST

RL_START_TEST (int8_array, "array of int8") {
  int8_array_t orig = { { 1, 2 }, };
  ALL_METHODS (ASSERT_SAVE_LOAD, int8_array_t, &orig);
} END_TEST

RL_START_TEST (int64_array, "array of int64") {
  int64_array_t orig = { { 1, 2 }, };
  ALL_METHODS (ASSERT_SAVE_LOAD, int64_array_t, &orig);
} END_TEST

RL_START_TEST (ld_array, "array of long double") {
  ld_array_t orig = { { 1, 2 }, };
  ALL_METHODS (ASSERT_SAVE_LOAD, ld_array_t, &orig);
} END_TEST

RL_START_TEST (string_array, "array of strings") {
  string_array_t orig = { { "ONE", "TWO" }, };
  ALL_METHODS (ASSERT_SAVE_LOAD, string_array_t, &orig);
} END_TEST

MAIN ();
