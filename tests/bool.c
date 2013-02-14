#include <stdbool.h>

#include <check.h>
#include <metaresc.h>
#include <regression.h>

TYPEDEF_STRUCT (struct_bool_t, (bool, x))

MR_START_TEST (bool_false, "boolean false") {
  ALL_METHODS (ASSERT_SAVE_LOAD, bool, (bool[]) { FALSE }, SCALAR_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_bool_t, FALSE, STRUCT_X_CMP);
} END_TEST

MR_START_TEST (bool_true, "boolean true") {
  ALL_METHODS (ASSERT_SAVE_LOAD, bool, (bool[]) { TRUE }, SCALAR_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_bool_t, TRUE, STRUCT_X_CMP);
} END_TEST

MR_START_TEST (bool_zero, "boolean zero") {
  ALL_METHODS (ASSERT_SAVE_LOAD, bool, (bool[]) { 0 }, SCALAR_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_bool_t, 0, STRUCT_X_CMP);
} END_TEST

MR_START_TEST (bool_nonzero, "boolean non-zero") {
  ALL_METHODS (ASSERT_SAVE_LOAD, bool, (bool[]) { 2 }, SCALAR_CMP);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_bool_t, 2, STRUCT_X_CMP);
} END_TEST

MAIN ();
