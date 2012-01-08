#define __USE_GNU
#include <math.h>
#include <values.h>
#include <check.h>
#include <reslib.h>
#include <regression.h>

#define ASSERT_SAVE_LOAD_DOUBLE(METHOD, VALUE) ({			\
      ASSERT_SAVE_LOAD_TYPE (METHOD, double, VALUE, CMP_SCALAR);	\
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_double, VALUE, CMP_STRUCT_X); \
    })

TYPEDEF_STRUCT (struct_double, double x);

RL_START_TEST(zero_double, "zero double") { ALL_METHODS (ASSERT_SAVE_LOAD_DOUBLE, 0); } END_TEST
RL_START_TEST(nan_double, "NAN double") { ALL_METHODS (ASSERT_SAVE_LOAD_DOUBLE, NAN); } END_TEST
RL_START_TEST(inf_double, "INFINITY double") { ALL_METHODS (ASSERT_SAVE_LOAD_DOUBLE, INFINITY); } END_TEST
RL_START_TEST(huge_val_double, "HUGE_VAL double") { ALL_METHODS (ASSERT_SAVE_LOAD_DOUBLE, HUGE_VAL); } END_TEST
RL_START_TEST(dbl_max_double, "DBL_MAX double") { ALL_METHODS (ASSERT_SAVE_LOAD_DOUBLE, DBL_MAX); } END_TEST
RL_START_TEST(dbl_min_double, "DBL_MIN double") { ALL_METHODS (ASSERT_SAVE_LOAD_DOUBLE, DBL_MIN); } END_TEST
RL_START_TEST(dbl_epsilon_double, "DBL_EPSILON double") { ALL_METHODS (ASSERT_SAVE_LOAD_DOUBLE, DBL_EPSILON); } END_TEST
RL_START_TEST(random_double, "random double") { ALL_METHODS (ASSERT_SAVE_LOAD_DOUBLE, 1.23456789012345678909L); } END_TEST
RL_START_TEST(pi_double, "pi long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_DOUBLE, M_PI); } END_TEST
RL_START_TEST(e_double, "e long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_DOUBLE, M_E); } END_TEST
