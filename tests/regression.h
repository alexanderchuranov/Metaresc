#ifndef _REGRESSION_H_
#define _REGRESSION_H_

#include <check.h>
#include <string.h>

#include <reslib.h>

#define TEST_METHODS XDR, SCM, CINIT, JSON, XML1, XML2
//#define TEST_METHODS XDR
//#define TEST_METHODS CINIT

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

#define MEM_CMP(TYPE, X, Y, ...) memcmp (X, Y, sizeof (TYPE))
#define CINIT_CMP(TYPE, X, Y, ...) ({					\
      char * x_ = RL_SAVE_CINIT (TYPE, X);				\
      char * y_ = RL_SAVE_CINIT (TYPE, Y);				\
      int xy_cmp = !0;							\
      if (x_ && y_)							\
	{								\
	  xy_cmp = strcmp (x_, y_);					\
	  if (xy_cmp)							\
	    printf ("orig = %s;\nrest = %s;\n", x_, y_);		\
	}								\
      if (x_)								\
	RL_FREE (x_);							\
      if (y_)								\
	RL_FREE (y_);							\
      xy_cmp;								\
    })

#define RL_IS__EQ__ 0

#define ASSERT_SAVE_LOAD(METHOD, TYPE, X, ...)		\
  RL_IF_ELSE (RL_PASTE2 (SKIP_METHOD_, METHOD))		\
  (ASSERT_SAVE_LOAD_(METHOD, TYPE, X, __VA_ARGS__)) (X != X)

#define ASSERT_SAVE_LOAD_(METHOD, TYPE, X, TYPE_CMP, ...)		\
  RL_IF_ELSE (RL_PASTE3 (RL_IS__EQ_, TYPE_CMP, _))			\
  (ASSERT_SAVE_LOAD__ (METHOD, TYPE, X, TYPE_CMP, __VA_ARGS__))		\
  (ASSERT_SAVE_LOAD__ (METHOD, TYPE, X, CINIT_CMP, __VA_ARGS__))

#define ASSERT_SAVE_LOAD__(METHOD, TYPE, X, TYPE_CMP, ...) ({		\
      rl_rarray_t serialized = RL_SAVE_ ## METHOD ## _RA (TYPE, X);	\
      int orig_eq_restored;						\
      TYPE RL_PASTE2(METHOD, _restored);				\
      if (0) printf ("%s\n", (char*)serialized.data);			\
      if (0 == RL_LOAD_ ## METHOD ## _RA (TYPE, &serialized, &RL_PASTE2(METHOD, _restored))) \
	ck_abort_msg ("load for method " #METHOD " on type " #TYPE " failed"); \
      if (serialized.data)						\
	RL_FREE (serialized.data);					\
      orig_eq_restored = (0 == TYPE_CMP (TYPE, X, &RL_PASTE2(METHOD, _restored), __VA_ARGS__)); \
      ck_assert_msg (orig_eq_restored,					\
		     "restored value mismatched original for method " #METHOD " on type " #TYPE); \
      RL_FREE_RECURSIVELY (TYPE, &RL_PASTE2(METHOD, _restored));	\
    })

#define ASSERT_SAVE_LOAD_TYPE(METHOD, TYPE, VALUE, ...) ({		\
      TYPE x = { VALUE };						\
      ASSERT_SAVE_LOAD (METHOD, TYPE, &x, __VA_ARGS__);			\
    })

#define ASSERT_GET_TEST(TEST, ...) TEST
#define ASSERT_GET_ARGS(TEST, ...) __VA_ARGS__
#define ASSERT_ITERATOR(ARGS, METHOD, I) ASSERT_ITERATOR_ (ASSERT_GET_TEST ARGS, METHOD, ASSERT_GET_ARGS ARGS)
#define ASSERT_ITERATOR_(TEST, METHOD, ...) TEST (METHOD, __VA_ARGS__)
#define SERIAL(NAME, I, REC, X) REC; X
#define ALL_METHODS(...) RL_FOR ((__VA_ARGS__), RL_NARG (TEST_METHODS), SERIAL, ASSERT_ITERATOR, TEST_METHODS)

#define CMP_SCALAR(TYPE, X, Y, ...) (*(X) != *(Y))
#define CMP_STRUCT_X(TYPE, X, Y, ...) ((X)->x != (Y)->x)

#endif /* _REGRESSION_H_ */
