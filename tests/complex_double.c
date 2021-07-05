#define __USE_GNU
#include <math.h>
#include <complex.h>
#include <check.h>
#include <metaresc.h>
#include <regression.h>
#include <flt_values.h>

#define ASSERT_SAVE_LOAD_COMPLEX_DOUBLE(METHOD, VALUE) ({		\
      ASSERT_SAVE_LOAD_TYPE (METHOD, complex double, VALUE);		\
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_complex_double_t, VALUE);	\
    })

TYPEDEF_STRUCT (struct_complex_double_t, complex double x)

START_TEST (zero_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_DOUBLE, 0); } END_TEST
START_TEST (nan1_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_DOUBLE, NAN); } END_TEST
START_TEST (nan2_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_DOUBLE, NAN * I); } END_TEST
START_TEST (nan3_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_DOUBLE, NAN + NAN * I); } END_TEST
START_TEST (inf_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_DOUBLE, INFINITY); } END_TEST
START_TEST (huge_val_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_DOUBLE, HUGE_VAL); } END_TEST
START_TEST (dbl_max_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_DOUBLE, DBL_MAX); } END_TEST
START_TEST (dbl_min_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_DOUBLE, DBL_MIN); } END_TEST
START_TEST (dbl_epsilon_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_DOUBLE, DBL_EPSILON); } END_TEST
START_TEST (pi_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_DOUBLE, M_PI + M_E * I); } END_TEST
START_TEST (one1_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_DOUBLE, 1); } END_TEST
START_TEST (one2_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_DOUBLE, I); } END_TEST
START_TEST (two_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_DOUBLE, 2I); } END_TEST

MAIN_TEST_SUITE ((zero_double, "zero double"),
		 (nan1_double, "NAN double"),
		 (nan2_double, "NAN double"),
		 (nan3_double, "NAN double"),
		 (inf_double, "INFINITY double"),
		 (huge_val_double, "HUGE_VAL double"),
		 (dbl_max_double, "DBL_MAX double"),
		 (dbl_min_double, "DBL_MIN double"),
		 (dbl_epsilon_double, "DBL_EPSILON double"),
		 (pi_double, "pi + e*i long_double"),
		 (one1_double, "1"),
		 (one2_double, "I"),
		 (two_double, "2I")
		 );
