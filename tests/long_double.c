#define __USE_GNU
#include <math.h>
#include <check.h>
#include <metaresc.h>
#include <regression.h>
#include <flt_values.h>

#define SCALAR_DOUBLE(TYPE, X, Y, ...) ({				\
  int cmp = memcmp (X, Y, MR_SIZEOF_LONG_DOUBLE);			\
  if (cmp)								\
    {									\
      char * _x = MR_SAVE_CINIT (ieee_long_double_t, (ieee_long_double_t*)X); \
      char * _y = MR_SAVE_CINIT (ieee_long_double_t, (ieee_long_double_t*)Y); \
      printf (#X " = %.20Lg " #Y " = %.20Lg\n", *(long double*)X, *(long double*)Y); \
      if (_x)								\
	{								\
	  printf ("x = %s\n", _x);					\
	  MR_FREE (_x);							\
	}								\
      if (_y)								\
	{								\
	  printf ("y = %s\n", _y);					\
	  MR_FREE (_y);							\
	}								\
    }									\
  cmp; })

#define ASSERT_SAVE_LOAD_LONG_DOUBLE(METHOD, VALUE) ({			\
      ASSERT_SAVE_LOAD_TYPE (METHOD, long double, VALUE, SCALAR_DOUBLE); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_long_double_t, VALUE, SCALAR_DOUBLE); \
    })

#define LD_NAN ((ieee_long_double_t){ { .long_double = 0, .ieee_854_long_double_nan = { .quiet_nan = !0, .one = 1, .exponent = -1, }, }, }).long_double

TYPEDEF_STRUCT (struct_long_double_t, long_double_t x)

MR_START_TEST (zero_long_double, "zero long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, 0); } END_TEST
MR_START_TEST (nan_long_double, "NAN long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, LD_NAN); } END_TEST
MR_START_TEST (inf_long_double, "INFINITY long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, INFINITY); } END_TEST
MR_START_TEST (huge_val_long_double, "HUGE_VAL long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, HUGE_VAL); } END_TEST
MR_START_TEST (ldbl_max_long_double, "LDBL_MAX long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, LDBL_MAX); } END_TEST
MR_START_TEST (ldbl_min_long_double, "LDBL_MIN long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, LDBL_MIN); } END_TEST
MR_START_TEST (ldbl_epsilon_long_double, "LDBL_EPSILON long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, LDBL_EPSILON); } END_TEST
MR_START_TEST (random_long_double, "random long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, 1.23456789012345678909L); } END_TEST
MR_START_TEST (pi_long_double, "pi long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, M_PI); } END_TEST
MR_START_TEST (e_long_double, "e long_double") { ALL_METHODS (ASSERT_SAVE_LOAD_LONG_DOUBLE, M_E); } END_TEST

MAIN ();
