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
      if (isinf (_x_real_))						\
	cmp_real = !isinf (_y_real_);					\
      if (isnan (_x_real_))						\
	cmp_real = !isnan (_y_real_);					\
      bool cmp_imag = (_x_imag_ != _y_imag_);				\
      if (isinf (_x_imag_))						\
	cmp_imag = !isinf (_y_imag_);					\
      if (isnan (_x_imag_))						\
	cmp_imag = !isnan (_y_imag_);					\
      cmp_real || cmp_imag;						\
    })

#define ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE(METHOD, VALUE) ({		\
      ASSERT_SAVE_LOAD_TYPE (METHOD, complex long double, VALUE, SCALAR_DOUBLE); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_complex_long_double_t, VALUE, SCALAR_DOUBLE); \
    })


TYPEDEF_STRUCT (struct_complex_long_double_t, complex long double x)

MR_START_TEST (zero_long_double, "zero long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE, 0); } END_TEST
MR_START_TEST (nan_long_double, "NAN long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE, LD_NAN); } END_TEST
MR_START_TEST (inf_long_double, "INFINITY long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE, INFINITY); } END_TEST
MR_START_TEST (huge_val_long_double, "HUGE_VAL long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE, HUGE_VAL); } END_TEST
MR_START_TEST (ldbl_max_long_double, "LDBL_MAX long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE, LD_LDBL_MAX); } END_TEST
MR_START_TEST (ldbl_min_long_double, "LDBL_MIN long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE, LD_LDBL_MIN); } END_TEST
MR_START_TEST (ldbl_epsilon_long_double, "LDBL_EPSILON long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE, LDBL_EPSILON); } END_TEST
MR_START_TEST (pi_long_double, "pi + e*i long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_COMPLEX_LONG_DOUBLE, M_PI + M_E * I); } END_TEST

MAIN ();
