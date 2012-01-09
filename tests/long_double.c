#define __USE_GNU
#include <math.h>
#include <values.h>
#include <check.h>
#include <reslib.h>
#include <regression.h>

#define ASSERT_SAVE_LOAD_LONG_DOUBLE(METHOD, VALUE) ({			\
      ASSERT_SAVE_LOAD_TYPE (METHOD, long double, VALUE, SCALAR_CMP);	\
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_long_double, VALUE, STRUCT_X_CMP); \
    })

TYPEDEF_STRUCT (struct_long_double, long_double_t x);

RL_START_TEST (zero_long_double, "zero long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, 0); } END_TEST
RL_START_TEST (nan_long_double, "NAN long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, NAN); } END_TEST
RL_START_TEST (inf_long_double, "INFINITY long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, INFINITY); } END_TEST
RL_START_TEST (huge_val_long_double, "HUGE_VAL long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, HUGE_VAL); } END_TEST
RL_START_TEST (ldbl_max_long_double, "LDBL_MAX long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, LDBL_MAX); } END_TEST
RL_START_TEST (ldbl_min_long_double, "LDBL_MIN long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, LDBL_MIN); } END_TEST
RL_START_TEST (ldbl_epsilon_long_double, "LDBL_EPSILON long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, LDBL_EPSILON); } END_TEST
RL_START_TEST (random_long_double, "random long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, 1.23456789012345678909L); } END_TEST
RL_START_TEST (pi_long_double, "pi long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, M_PI); } END_TEST
RL_START_TEST (e_long_double, "e long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, M_E); } END_TEST

MAIN ();
