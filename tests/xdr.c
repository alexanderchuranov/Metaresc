#include <stdlib.h>
#include <string.h>
#include <check.h>

#include <reslib.h>

TYPEDEF_STRUCT (rarray_t, RARRAY (uint8_t, x));

static Suite * suite = NULL;

#define RL_START_TEST(NAME, ...)					\
  static void NAME (int);						\
  static inline void __attribute__ ((constructor)) init_ ## NAME (void) \
  {									\
    if (NULL == suite)							\
      suite = suite_create ("main");					\
    if (suite)								\
      {									\
	TCase * tcase = tcase_create ("" __VA_ARGS__);			\
	if (tcase)							\
	  {								\
	    tcase_add_test (tcase, NAME);				\
	    suite_add_tcase (suite, tcase);				\
	  }								\
      }									\
  };									\
  START_TEST(NAME)

#define ASSERT_XDR_INT(TYPE, X, SUFFIX...) ({				\
      rl_rarray_t result = RL_SAVE_XDR_RA (TYPE, &X);			\
      uint32_t expected[(sizeof (X) + sizeof (uint32_t) - 1) / sizeof (uint32_t)]; \
      int i;								\
      uint64_t _x = X SUFFIX;						\
      for (i = 0; i < sizeof (expected) / sizeof (expected[0]); ++i)	\
	{								\
	  uint32_t _x_; /* tricks to break strict aliasing rules */	\
	  memcpy (&_x_, (uint32_t*)&_x + (sizeof (expected) / sizeof (expected[0]) - 1 - i), sizeof (_x_)); \
	  expected[i] = htonl (_x_);					\
	}								\
      if (NULL == result.data)						\
	ck_abort_msg ("Result is NULL");				\
      ck_assert_msg (((result.size == sizeof (expected)) &&		\
		      (0 == memcmp (result.data, expected, sizeof (expected)))), \
		     "XDR of " #TYPE " for value 0x%" PRIx64, _x);	\
      RL_FREE (result.data);						\
    })

#define ASSERT_XDR_INT_VALUE(BITS, VALUE) ({			\
      uint ## BITS ## _t ux;					\
      int ## BITS ## _t ix;					\
      struct_uint ## BITS ## _t us;				\
      struct_int ## BITS ## _t is;				\
      ux = ix = us.x = is.x = VALUE;				\
      ASSERT_XDR_INT (uint ## BITS ## _t, ux);			\
      ASSERT_XDR_INT (int ## BITS ## _t, ix);			\
      ASSERT_XDR_INT (struct_uint ## BITS ## _t, us, .x);	\
      ASSERT_XDR_INT (struct_int ## BITS ## _t, is, .x);	\
    })

TYPEDEF_STRUCT (struct_uint8_t, uint8_t x);
TYPEDEF_STRUCT (struct_int8_t, int8_t x);

RL_START_TEST(XDR_int8_t, "XDR [u]int8_t") {
  ASSERT_XDR_INT_VALUE (8, 0);
  ASSERT_XDR_INT_VALUE (8, -1);
  ASSERT_XDR_INT_VALUE (8, 0x12);
} END_TEST

TYPEDEF_STRUCT (struct_uint16_t, uint16_t x);
TYPEDEF_STRUCT (struct_int16_t, int16_t x);

RL_START_TEST(XDR_int16_t, "XDR [u]int16_t") {
  ASSERT_XDR_INT_VALUE (16, 0);
  ASSERT_XDR_INT_VALUE (16, -1);
  ASSERT_XDR_INT_VALUE (16, 0x1234);
} END_TEST

TYPEDEF_STRUCT (struct_uint32_t, uint32_t x);
TYPEDEF_STRUCT (struct_int32_t, int32_t x);

RL_START_TEST(XDR_int32_t, "XDR [u]int32_t") {
  ASSERT_XDR_INT_VALUE (32, 0);
  ASSERT_XDR_INT_VALUE (32, -1);
  ASSERT_XDR_INT_VALUE (32, 0x12345678);
} END_TEST

TYPEDEF_STRUCT (struct_uint64_t, uint64_t x);
TYPEDEF_STRUCT (struct_int64_t, int64_t x);

RL_START_TEST(XDR_int64_t, "XDR [u]int64_t") {
  ASSERT_XDR_INT_VALUE (64, 0);
  ASSERT_XDR_INT_VALUE (64, -1);
  ASSERT_XDR_INT_VALUE (64, 0x123456789abcdef0LL);
} END_TEST

void huh (void) {
  char * format_uint8(rl_ptrdes_t * ptrdes) {
    char str[RL_INT_TO_STRING_BUF_SIZE] = "";
    sprintf (str, "0x%" PRIx8, *(uint8_t*)ptrdes->data);
    return (RL_STRDUP (str));
  }

  rl_conf.output_format[RL_TYPE_UINT8] = format_uint8;
  struct_uint64_t is = { .x = 0x123456789abcdef0LL };
  rl_rarray_t result = RL_SAVE_XDR_RA (struct_uint64_t, &is);
  printf ("got %s\n", RL_SAVE_CINIT (rarray_t, (rarray_t*)&result));
}

int main (int argc, char * argv[])
{
  int number_failed;
  SRunner * srunner = srunner_create (suite);
  srunner_run_all (srunner, CK_ENV);
  number_failed = srunner_ntests_failed (srunner);
  srunner_free (srunner);
  return ((number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
}
