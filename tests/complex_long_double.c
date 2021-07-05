#define __USE_GNU
#include <math.h>
#include <complex.h>
#include <check.h>
#include <metaresc.h>
#include <regression.h>
#include <flt_values.h>

#define ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE(METHOD, VALUE) ({		\
      ASSERT_SAVE_LOAD_TYPE (METHOD, complex long double, VALUE);	\
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_complex_long_double_t, VALUE); \
    })


TYPEDEF_STRUCT (struct_complex_long_double_t, complex long double x)

START_TEST (zero_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE, 0); } END_TEST
START_TEST (nan1_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE, LD_NAN); } END_TEST
START_TEST (nan2_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE, LD_NAN * I); } END_TEST
START_TEST (nan3_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE, LD_NAN + LD_NAN * I); } END_TEST
START_TEST (inf_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE, INFINITY); } END_TEST
START_TEST (huge_val_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE, HUGE_VAL); } END_TEST
START_TEST (ldbl_max_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE, LD_LDBL_MAX); } END_TEST
START_TEST (ldbl_min_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE, LD_LDBL_MIN); } END_TEST
START_TEST (ldbl_epsilon_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE, LDBL_EPSILON); } END_TEST
START_TEST (pi_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE, M_PI + M_E * I); } END_TEST
START_TEST (one1_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE, 1); } END_TEST
START_TEST (one2_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE, I); } END_TEST
START_TEST (two_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE, 2I); } END_TEST

MAIN_TEST_SUITE ((zero_long_double, "zero long_double"),
		 (nan1_long_double, "NAN long_double"),
		 (nan2_long_double, "NAN long_double"),
		 (nan3_long_double, "NAN long_double"),
		 (inf_long_double, "INFINITY long_double"),
		 (huge_val_long_double, "HUGE_VAL long_double"),
		 (ldbl_max_long_double, "LDBL_MAX long_double"),
		 (ldbl_min_long_double, "LDBL_MIN long_double"),
		 (ldbl_epsilon_long_double, "LDBL_EPSILON long_double"),
		 (pi_long_double, "pi + e*i long_double"),
		 (one1_long_double, "1"),
		 (one2_long_double, "I"),
		 (two_long_double, "2I")
		 );
