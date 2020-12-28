#include <stdbool.h>

#include <check.h>
#include <metaresc.h>
#include <regression.h>

TYPEDEF_STRUCT (struct_bool_t, bool x)

START_TEST (bool_false) {
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, bool, false);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_bool_t, false);
} END_TEST

START_TEST (bool_true) {
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, bool, true);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_bool_t, true);
} END_TEST

START_TEST (bool_zero) {
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, bool, 0);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_bool_t, 0);
} END_TEST

START_TEST (bool_nonzero) {
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, bool, 2);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_bool_t, 2);
} END_TEST

MAIN_TEST_SUITE ((bool_false, "boolean false"),
		 (bool_true, "boolean true"),
		 (bool_zero, "boolean zero"),
		 (bool_nonzero, "boolean non-zero")
		 );
