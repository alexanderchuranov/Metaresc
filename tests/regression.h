#ifndef _REGRESSION_H_
#define _REGRESSION_H_

#include <check.h>
#include <string.h>

#include <reslib.h>

#ifdef HAVE_BISON_FLEX
#define BISON_FLEX_METHODS SCM, CINIT, JSON, XML1,
#else /* HAVE_BISON_FLEX */
#define BISON_FLEX_METHODS
#endif /* HAVE_BISON_FLEX */

#ifdef HAVE_LIBXML2
#define LIBXML2_METHODS XML2,
#else /* HAVE_LIBXML2 */
#define LIBXML2_METHODS
#endif /* HAVE_LIBXML2 */

#define TEST_METHODS LIBXML2_METHODS BISON_FLEX_METHODS XDR

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
  START_TEST (NAME)

#define MEM_CMP(TYPE, X, Y, ...) memcmp (X, Y, sizeof (TYPE))
#define CINIT_CMP(TYPE, X, Y, ...) ({					\
      char * x_ = RL_SAVE_CINIT (TYPE, X);				\
      char * y_ = RL_SAVE_CINIT (TYPE, Y);				\
      int xy_cmp = !0;							\
      if (x_ && y_)							\
	{								\
	  xy_cmp = strcmp (x_, y_);					\
	  if (~xy_cmp)							\
	    printf (#TYPE " %s = %s;\n"					\
		    #TYPE " %s = %s;\n", &#X[1], x_, &#Y[1], y_);	\
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
  (ASSERT_SAVE_LOAD_(METHOD, TYPE, X, __VA_ARGS__)) (memcmp (X, X, 1))

#define ASSERT_SAVE_LOAD_(METHOD, TYPE, X, TYPE_CMP, ...)		\
  RL_IF_ELSE (RL_PASTE3 (RL_IS__EQ_, TYPE_CMP, _))			\
  (ASSERT_SAVE_LOAD__ (METHOD, TYPE, X, TYPE_CMP, __VA_ARGS__))		\
  (ASSERT_SAVE_LOAD__ (METHOD, TYPE, X, CINIT_CMP, __VA_ARGS__))

#define ASSERT_SAVE_LOAD__(METHOD, TYPE, X, TYPE_CMP, ...) ({		\
      rl_rarray_t serialized = RL_SAVE_ ## METHOD ## _RA (TYPE, X);	\
      int orig_eq_restored;						\
      TYPE METHOD ## _restored;						\
      if (0 == RL_LOAD_ ## METHOD ## _RA (TYPE, &serialized, &METHOD ## _restored)) \
	ck_abort_msg ("load for method " #METHOD " on type " #TYPE " failed"); \
      if (serialized.data)						\
	RL_FREE (serialized.data);					\
      orig_eq_restored = (0 == TYPE_CMP (TYPE, X, &METHOD ## _restored, __VA_ARGS__)); \
      ck_assert_msg (orig_eq_restored,					\
		     "restored value mismatched original for method " #METHOD " on type " #TYPE); \
      RL_FREE_RECURSIVELY (TYPE, &METHOD ## _restored);			\
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

#define SCALAR_CMP(TYPE, X, Y, ...) (*(X) != *(Y))
#define STRUCT_X_CMP(TYPE, X, Y, ...) ((X)->x != (Y)->x)

#define MAIN(...)							\
  Suite * suite = NULL;							\
  int main (int argc, char * argv[])					\
  {									\
    int number_failed;							\
    SRunner * srunner = srunner_create (suite);				\
    __VA_ARGS__;							\
    srunner_run_all (srunner, CK_ENV);					\
    number_failed = srunner_ntests_failed (srunner);			\
    srunner_free (srunner);						\
    return ((number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE);	\
  }									\
  
#endif /* _REGRESSION_H_ */
