#ifndef _REGRESSION_H_
#define _REGRESSION_H_

#include <check.h>
#include <string.h>

#include <reslib.h>

//#define TEST_METHODS XDR, SCM, CINIT, JSON, XML1, XML2
#define TEST_METHODS XDR

extern Suite * suite;

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

#define MEMCMP(X, Y) memcmp (X, Y, sizeof (*(Y)))

#define ASSERT_SAVE_LOAD(METHOD, TYPE, X, ...)				\
  RL_IF_ELSE (RL_IS_EMPTY (__VA_ARGS__))				\
  (ASSERT_SAVE_LOAD_ (METHOD, TYPE, X, MEMCMP))				\
  (ASSERT_SAVE_LOAD_ (METHOD, TYPE, X, __VA_ARGS__))

#define ASSERT_SAVE_LOAD_(METHOD, TYPE, X, CMP) ({			\
      rl_rarray_t serialized = RL_SAVE_ ## METHOD ## _RA (TYPE, X);	\
      TYPE restored;							\
      if (0 == RL_LOAD_ ## METHOD ## _RA (TYPE, &serialized, &restored)) \
	ck_abort_msg ("load for method " #METHOD " on type " #TYPE "failed"); \
      if (0) printf ("%s\n", (char*)serialized.data);			\
      if (serialized.data)						\
	RL_FREE (serialized.data);					\
      ck_assert_msg ((0 == CMP (X, &restored)),				\
		     "restored value mismatched original for method " #METHOD " on type " #TYPE); \
      RL_FREE_RECURSIVELY (TYPE, &restored);				\
    })

#define ASSERT_SAVE_LOAD_TYPE(METHOD, TYPE, VALUE, ...) ({		\
      TYPE x = VALUE;							\
      ASSERT_SAVE_LOAD (METHOD, TYPE, &x, __VA_ARGS__);			\
    })

#define ASSERT_GET_TEST(TEST, ...) TEST
#define ASSERT_GET_ARGS(TEST, ...) __VA_ARGS__
#define ASSERT_ITERATOR(ARGS, METHOD, I) ASSERT_ITERATOR_ (ASSERT_GET_TEST ARGS, METHOD, ASSERT_GET_ARGS ARGS)
#define ASSERT_ITERATOR_(TEST, METHOD, ...) TEST (METHOD, __VA_ARGS__)
#define SERIAL(NAME, I, REC, X) REC; X
#define ALL_METHODS(...) RL_FOR ((__VA_ARGS__), RL_NARG (TEST_METHODS), SERIAL, ASSERT_ITERATOR, TEST_METHODS)

#define FDLD_CMP(X, ...) (*(X) != *(__VA_ARGS__))
#define STRUCT_FDLD_CMP(X, ...) ((X)->x != (__VA_ARGS__)->x)

#endif /* _REGRESSION_H_ */
