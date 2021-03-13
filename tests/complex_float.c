#define __USE_GNU
#include <math.h>
#include <complex.h>
#include <check.h>
#include <metaresc.h>
#include <regression.h>
#include <flt_values.h>

#define ASSERT_SAVE_LOAD_COMPLEX_FLOAT(METHOD, VALUE) ({		\
      ASSERT_SAVE_LOAD_TYPE (METHOD, complex float, VALUE);		\
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_complex_float_t, VALUE);	\
    })

TYPEDEF_STRUCT (struct_complex_float_t, complex float x)

START_TEST (zero_float) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_FLOAT, 0); } END_TEST
START_TEST (nan1_float) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_FLOAT, NAN); } END_TEST
START_TEST (nan2_float) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_FLOAT, NAN * I); } END_TEST
START_TEST (nan3_float) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_FLOAT, NAN + NAN * I); } END_TEST
START_TEST (inf_float) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_FLOAT, INFINITY); } END_TEST
START_TEST (huge_valf_float) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_FLOAT, HUGE_VALF); } END_TEST
START_TEST (flt_max_float) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_FLOAT, FLT_MAX + FLT_MAX * I); } END_TEST
START_TEST (flt_min_float) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_FLOAT, FLT_MIN + FLT_MIN * I); } END_TEST
START_TEST (flt_epsilon_float) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_FLOAT, FLT_EPSILON + FLT_EPSILON * I); } END_TEST
START_TEST (pi_float_complex) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_FLOAT, M_PI + M_E * I); } END_TEST 
START_TEST (one1_float_complex) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_FLOAT, 1); } END_TEST
START_TEST (one2_float_complex) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_FLOAT, I); } END_TEST
													
MAIN_TEST_SUITE ((zero_float, "zero float"),
		 (nan1_float, "NAN float"),
		 (nan2_float, "NAN float"),
		 (nan3_float, "NAN float"),
		 (inf_float, "INFINITY float"),
		 (huge_valf_float, "HUGE_VALF float"),
		 (flt_max_float, "FLT_MAX float"),
		 (flt_min_float, "FLT_MIN float"),
		 (flt_epsilon_float, "FLT_EPSILON float"),
		 (pi_float_complex, "pi + e*i"),
		 (one1_float_complex, "1"),
		 (one2_float_complex, "I")
		 );
