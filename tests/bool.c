#include <stdbool.h>

#include <check.h>
#include <metaresc.h>
#include <regression.h>

TYPEDEF_STRUCT (struct_bool_t, bool x)

MR_START_TEST (bool_false, "boolean false") {
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, bool, false);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_bool_t, false);
} END_TEST

MR_START_TEST (bool_true, "boolean true") {
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, bool, true);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_bool_t, true);
} END_TEST

MR_START_TEST (bool_zero, "boolean zero") {
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, bool, 0);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_bool_t, 0);
} END_TEST

MR_START_TEST (bool_nonzero, "boolean non-zero") {
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, bool, 2);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_bool_t, 2);
} END_TEST

MAIN ();
