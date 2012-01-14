#include <check.h>
#include <reslib.h>
#include <regression.h>

#undef TEST_METHODS
#define TEST_METHODS CINIT

TYPEDEF_ENUM (packed_enum_t, ATTRIBUTES (__attribute__ ((packed, aligned(sizeof (uint16_t))))), ONE, TWO)
TYPEDEF_STRUCT (char_array_t, (char, x, [2]))
TYPEDEF_STRUCT (int8_array_t, (int8_t, x, [2]))
TYPEDEF_STRUCT (int64_array_t, (int64_t, x, [2]))
TYPEDEF_STRUCT (packed_enum_array_t, (packed_enum_t, x, [2]))
TYPEDEF_STRUCT (ld_array_t, (long double, x, [2]))
TYPEDEF_STRUCT (string_array_t, (string_t, x, [2]))

RL_START_TEST (char_array, "array of chars") {
  char_array_t orig = {
    .x = { 'a', 'b' },
  };
  ALL_METHODS (ASSERT_SAVE_LOAD, char_array_t, &orig);
} END_TEST

RL_START_TEST (enum_array, "array of enums") {
  packed_enum_array_t orig = {
    .x = { ONE, TWO },
  };
  ALL_METHODS (ASSERT_SAVE_LOAD, packed_enum_array_t, &orig);
} END_TEST

MAIN ();
