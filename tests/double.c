#include <check.h>
#include <metaresc.h>
#include <regression.h>
#include <flt_values.h>

#define ASSERT_SAVE_LOAD_DOUBLE(METHOD, VALUE) ({		       \
      ASSERT_SAVE_LOAD_TYPE (METHOD, double, VALUE);		       \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_double_t, VALUE);	       \
    })

TYPEDEF_STRUCT (struct_double_t, double x)

START_TEST (zero_double) { ALL_METHODS (ASSERT_SAVE_LOAD_DOUBLE, 0); } END_TEST
START_TEST (nan_double) { ALL_METHODS (ASSERT_SAVE_LOAD_DOUBLE, NAN); } END_TEST
START_TEST (inf_double) { ALL_METHODS (ASSERT_SAVE_LOAD_DOUBLE, INFINITY); } END_TEST
START_TEST (neg_inf_double) { ALL_METHODS (ASSERT_SAVE_LOAD_DOUBLE, -INFINITY); } END_TEST
START_TEST (huge_val_double) { ALL_METHODS (ASSERT_SAVE_LOAD_DOUBLE, HUGE_VAL); } END_TEST
START_TEST (neg_huge_val_double) { ALL_METHODS (ASSERT_SAVE_LOAD_DOUBLE, -HUGE_VAL); } END_TEST
START_TEST (dbl_max_double) { ALL_METHODS (ASSERT_SAVE_LOAD_DOUBLE, DBL_MAX); } END_TEST
START_TEST (neg_dbl_max_double) { ALL_METHODS (ASSERT_SAVE_LOAD_DOUBLE, -DBL_MAX); } END_TEST
START_TEST (dbl_min_double) { ALL_METHODS (ASSERT_SAVE_LOAD_DOUBLE, DBL_MIN); } END_TEST
START_TEST (neg_dbl_min_double) { ALL_METHODS (ASSERT_SAVE_LOAD_DOUBLE, -DBL_MIN); } END_TEST
START_TEST (dbl_epsilon_double) { ALL_METHODS (ASSERT_SAVE_LOAD_DOUBLE, DBL_EPSILON); } END_TEST
START_TEST (neg_dbl_epsilon_double) { ALL_METHODS (ASSERT_SAVE_LOAD_DOUBLE, -DBL_EPSILON); } END_TEST
START_TEST (random_double) { ALL_METHODS (ASSERT_SAVE_LOAD_DOUBLE, 1.23456789012345678909L); } END_TEST
START_TEST (pi_double) { ALL_METHODS (ASSERT_SAVE_LOAD_DOUBLE, MR_PI); } END_TEST
START_TEST (e_double) { ALL_METHODS (ASSERT_SAVE_LOAD_DOUBLE, MR_E); } END_TEST

MAIN_TEST_SUITE ((zero_double, "zero double"),
		 (nan_double, "NAN double"),
		 (inf_double, "INFINITY double"),
		 (neg_inf_double, "negative INFINITY double"),
		 (huge_val_double, "HUGE_VAL double"),
		 (neg_huge_val_double, "negative HUGE_VAL double"),
		 (dbl_max_double, "DBL_MAX double"),
		 (neg_dbl_max_double, "negative DBL_MAX double"),
		 (dbl_min_double, "DBL_MIN double"),
		 (neg_dbl_min_double, "negative DBL_MIN double"),
		 (dbl_epsilon_double, "DBL_EPSILON double"),
		 (neg_dbl_epsilon_double, "negative DBL_EPSILON double"),
		 (random_double, "random double"),
		 (pi_double, "pi long_double"),
		 (e_double, "e long_double")
		 );
