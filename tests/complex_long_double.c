#define __USE_GNU
#include <math.h>
#include <complex.h>
#include <check.h>
#include <metaresc.h>
#include <regression.h>
#include <flt_values.h>

#define SCALAR_DOUBLE(TYPE, X, Y, ...) ({				\
      complex long double _x_cld_ = *(complex long double*)X;		\
      complex long double _y_cld_ = *(complex long double*)Y;		\
      long double _x_real_ = __real__ _x_cld_;				\
      long double _x_imag_ = __imag__ _x_cld_;				\
      long double _y_real_ = __real__ _y_cld_;				\
      long double _y_imag_ = __imag__ _y_cld_;				\
      bool cmp_real = (_x_real_ != _y_real_);				\
      if (MR_ISNAN (_x_real_))						\
	cmp_real = !MR_ISNAN (_y_real_);				\
      bool cmp_imag = (_x_imag_ != _y_imag_);				\
      if (MR_ISNAN (_x_imag_))						\
	cmp_imag = !MR_ISNAN (_y_imag_);				\
      cmp_real || cmp_imag;						\
    })

#define ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE(METHOD, VALUE) ({		\
      ASSERT_SAVE_LOAD_TYPE (METHOD, complex long double, VALUE, SCALAR_DOUBLE); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_complex_long_double_t, VALUE, SCALAR_DOUBLE); \
    })


TYPEDEF_STRUCT (struct_complex_long_double_t, complex long double x)

START_TEST (zero_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE, 0); } END_TEST
START_TEST (nan_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE, LD_NAN); } END_TEST
START_TEST (inf_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE, INFINITY); } END_TEST
START_TEST (huge_val_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE, HUGE_VAL); } END_TEST
START_TEST (ldbl_max_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE, LD_LDBL_MAX); } END_TEST
START_TEST (ldbl_min_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE, LD_LDBL_MIN); } END_TEST
START_TEST (ldbl_epsilon_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE, LDBL_EPSILON); } END_TEST
START_TEST (pi_long_double) { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE, M_PI + M_E * I); } END_TEST

MAIN_TEST_SUITE ((zero_long_double, "zero long_double"),
		 (nan_long_double, "NAN long_double"),
		 (inf_long_double, "INFINITY long_double"),
		 (huge_val_long_double, "HUGE_VAL long_double"),
		 (ldbl_max_long_double, "LDBL_MAX long_double"),
		 (ldbl_min_long_double, "LDBL_MIN long_double"),
		 (ldbl_epsilon_long_double, "LDBL_EPSILON long_double"),
		 (pi_long_double, "pi + e*i long_double")
		 );
