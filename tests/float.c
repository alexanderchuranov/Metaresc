#define __USE_GNU
#include <math.h>
#include <check.h>
#include <metaresc.h>
#include <regression.h>
#include <flt_values.h>


#define ASSERT_SAVE_LOAD_FLOAT(METHOD, VALUE) ({			\
      ASSERT_SAVE_LOAD_TYPE (METHOD, float, VALUE);			\
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_float_t, VALUE);		\
    })

TYPEDEF_STRUCT (struct_float_t, float x)

START_TEST (zero_float) { ALL_METHODS (ASSERT_SAVE_LOAD_FLOAT, 0); } END_TEST
START_TEST (nan_float) { ALL_METHODS (ASSERT_SAVE_LOAD_FLOAT, NAN); } END_TEST
START_TEST (inf_float) { ALL_METHODS (ASSERT_SAVE_LOAD_FLOAT, INFINITY); } END_TEST
START_TEST (huge_valf_float) { ALL_METHODS (ASSERT_SAVE_LOAD_FLOAT, HUGE_VALF); } END_TEST
START_TEST (flt_max_float) { ALL_METHODS (ASSERT_SAVE_LOAD_FLOAT, FLT_MAX); } END_TEST
START_TEST (flt_min_float) { ALL_METHODS (ASSERT_SAVE_LOAD_FLOAT, FLT_MIN); } END_TEST
START_TEST (flt_epsilon_float) { ALL_METHODS (ASSERT_SAVE_LOAD_FLOAT, FLT_EPSILON); } END_TEST
START_TEST (random_float) { ALL_METHODS (ASSERT_SAVE_LOAD_FLOAT, 1.23456789012345678909L); } END_TEST
START_TEST (pi_float) { ALL_METHODS (ASSERT_SAVE_LOAD_FLOAT, M_PI); } END_TEST
START_TEST (e_float) { ALL_METHODS (ASSERT_SAVE_LOAD_FLOAT, M_E); } END_TEST

MAIN_TEST_SUITE ((zero_float, "zero float"),
		 (nan_float, "NAN float"),
		 (inf_float, "INFINITY float"),
		 (huge_valf_float, "HUGE_VALF float"),
		 (flt_max_float, "FLT_MAX float"),
		 (flt_min_float, "FLT_MIN float"),
		 (flt_epsilon_float, "FLT_EPSILON float"),
		 (random_float, "random float"),
		 (pi_float, "pi float"),
		 (e_float, "e float")
		 );

