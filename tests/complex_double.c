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

MR_START_TEST (zero_double, "zero double") { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_DOUBLE, 0); } END_TEST
MR_START_TEST (nan_double, "NAN double") { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_DOUBLE, NAN); } END_TEST
MR_START_TEST (inf_double, "INFINITY double") { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_DOUBLE, INFINITY); } END_TEST
MR_START_TEST (huge_val_double, "HUGE_VAL double") { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_DOUBLE, HUGE_VAL); } END_TEST
MR_START_TEST (dbl_max_double, "DBL_MAX double") { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_DOUBLE, DBL_MAX); } END_TEST
MR_START_TEST (dbl_min_double, "DBL_MIN double") { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_DOUBLE, DBL_MIN); } END_TEST
MR_START_TEST (dbl_epsilon_double, "DBL_EPSILON double") { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_DOUBLE, DBL_EPSILON); } END_TEST
MR_START_TEST (pi_double, "pi + e*i long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_DOUBLE, M_PI + M_E * I); } END_TEST

MAIN ();
