#define __USE_GNU
#include <math.h>
#include <check.h>
#include <metaresc.h>
#include <regression.h>
#include <flt_values.h>

#define SCALAR_DOUBLE(TYPE, X, Y, ...) memcmp (X, Y, MR_SIZEOF_LONG_DOUBLE)

#define ASSERT_SAVE_LOAD_LONG_DOUBLE(METHOD, VALUE) ({			\
      ASSERT_SAVE_LOAD_TYPE (METHOD, long double, VALUE, SCALAR_DOUBLE); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_long_double_t, VALUE, SCALAR_DOUBLE); \
    })


TYPEDEF_STRUCT (struct_long_double_t, long_double_t x)

START_TEST (zero_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, 0); } END_TEST
START_TEST (nan_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, LD_NAN); } END_TEST
START_TEST (inf_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, INFINITY); } END_TEST
START_TEST (neg_inf_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, -INFINITY); } END_TEST
START_TEST (huge_val_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, HUGE_VAL); } END_TEST
START_TEST (neg_huge_val_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, -HUGE_VAL); } END_TEST
START_TEST (ldbl_max_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, LD_LDBL_MAX); } END_TEST
START_TEST (neg_ldbl_max_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, -LD_LDBL_MAX); } END_TEST
START_TEST (ldbl_min_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, LD_LDBL_MIN); } END_TEST
START_TEST (neg_ldbl_min_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, -LD_LDBL_MIN); } END_TEST
START_TEST (ldbl_epsilon_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, LDBL_EPSILON); } END_TEST
START_TEST (neg_ldbl_epsilon_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, -LDBL_EPSILON); } END_TEST
START_TEST (random_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, 1.2345678901234566904L); } END_TEST
START_TEST (pi_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, M_PI); } END_TEST
START_TEST (e_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, M_E); } END_TEST

MAIN_TEST_SUITE ((zero_long_double, "zero long_double"),
		 (nan_long_double, "NAN long_double"),
		 (inf_long_double, "INFINITY long_double"),
		 (neg_inf_long_double, "negative INFINITY long_double"),
		 (huge_val_long_double, "HUGE_VAL long_double"),
		 (neg_huge_val_long_double, "negative HUGE_VAL long_double"),
		 (ldbl_max_long_double, "LDBL_MAX long_double"),
		 (neg_ldbl_max_long_double, "negative LDBL_MAX long_double"),
		 (ldbl_min_long_double, "LDBL_MIN long_double"),
		 (neg_ldbl_min_long_double, "negative LDBL_MIN long_double"),
		 (ldbl_epsilon_long_double, "LDBL_EPSILON long_double"),
		 (neg_ldbl_epsilon_long_double, "negative LDBL_EPSILON long_double"),
		 (random_long_double, "random long_double"),
		 (pi_long_double, "pi long_double"),
		 (e_long_double, "e long_double")
		 );
