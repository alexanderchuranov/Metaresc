#define __USE_GNU
#include <math.h>
#include <complex.h>
#include <check.h>
#include <metaresc.h>
#include <regression.h>
#include <flt_values.h>


#define ASSERT_SAVE_LOAD_COMPLEX_FLOAT(METHOD, VALUE) ({		\
      ASSERT_SAVE_LOAD_TYPE (METHOD, complex float, VALUE, MEM_CMP);	\
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_complex_float_t, VALUE, MEM_CMP); \
    })

TYPEDEF_STRUCT (struct_complex_float_t, complex float x)

MR_START_TEST (zero_float, "zero float") { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_FLOAT, 0); } END_TEST
MR_START_TEST (nan_float, "NAN float") { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_FLOAT, NAN); } END_TEST
MR_START_TEST (inf_float, "INFINITY float") { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_FLOAT, INFINITY); } END_TEST
MR_START_TEST (huge_valf_float, "HUGE_VALF float") { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_FLOAT, HUGE_VALF); } END_TEST
MR_START_TEST (flt_max_float, "FLT_MAX float") { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_FLOAT, FLT_MAX + FLT_MAX * I); } END_TEST
MR_START_TEST (flt_min_float, "FLT_MIN float") { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_FLOAT, FLT_MIN + FLT_MIN * I); } END_TEST
MR_START_TEST (flt_epsilon_float, "FLT_EPSILON float") { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_FLOAT, FLT_EPSILON + FLT_EPSILON * I); } END_TEST
MR_START_TEST (pi_float_complex, "pi + e*i") { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_FLOAT, M_PI + M_E * I); } END_TEST 
													
MAIN ();

