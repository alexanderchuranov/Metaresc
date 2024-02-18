#ifndef _REGRESSION_H_
#define _REGRESSION_H_

#include <string.h>
#include <stdlib.h>

#include <check.h>
#include <metaresc.h>

#ifdef HAVE_BISON_FLEX
#define BISON_FLEX_METHODS XML1, SCM, CINIT, JSON,
#else /* HAVE_BISON_FLEX */
#define BISON_FLEX_METHODS
#endif /* HAVE_BISON_FLEX */

#ifdef HAVE_LIBXML2
#define LIBXML2_METHODS XML2,
#else /* HAVE_LIBXML2 */
#define LIBXML2_METHODS
#endif /* HAVE_LIBXML2 */

#ifdef HAVE_LIBYAML
#define LIBYAML_METHODS YAML,
#else /* HAVE_LIBYAML */
#define LIBYAML_METHODS
#endif /* HAVE_LIBYAML */

#ifdef HAVE_RPC_TYPES_H
#define XDR_METHODS XDR
#else /* HAVE_RPC_TYPES_H */
#define XDR_METHODS
#endif /* HAVE_RPC_TYPES_H */

#define TEST_METHODS LIBXML2_METHODS LIBYAML_METHODS BISON_FLEX_METHODS XDR_METHODS

#define CMP_SERIALIAZED(TYPE, X, Y, ...) MR_CMP_STRUCTS(TYPE, X, Y)

#define SKIP_METHOD_ 0

#define MR_IS__EQ__ 0

#define ASSERT_SAVE_LOAD(METHOD, TYPE, X, ...)		\
  MR_IF_ELSE (MR_PASTE2 (SKIP_METHOD_, METHOD))		\
  (ASSERT_SAVE_LOAD_(METHOD, TYPE, X, __VA_ARGS__)) (0)

#define ASSERT_SAVE_LOAD_(METHOD, TYPE, X, TYPE_CMP, ...)		\
  MR_IF_ELSE (MR_PASTE3 (MR_IS__EQ_, TYPE_CMP, _))			\
  (ASSERT_SAVE_LOAD__ (METHOD, TYPE, X, TYPE_CMP, __VA_ARGS__))		\
  (ASSERT_SAVE_LOAD__ (METHOD, TYPE, X, CMP_SERIALIAZED, __VA_ARGS__))

#define ASSERT_SAVE_LOAD__(METHOD, TYPE, X, TYPE_CMP, ...) ({		\
      mr_rarray_t serialized = MR_SAVE_ ## METHOD ## _RA (TYPE, X);	\
      int orig_eq_restored, load_success;				\
      TYPE METHOD ## _restored;						\
      memset (&METHOD ## _restored, 0, sizeof (TYPE));			\
      load_success = (MR_SUCCESS == MR_LOAD_ ## METHOD ## _RA (TYPE, &serialized, &METHOD ## _restored)); \
      ck_assert_msg (load_success,					\
		     "load for method " #METHOD " on type " #TYPE " failed"); \
      orig_eq_restored = (0 == TYPE_CMP (TYPE, X, &METHOD ## _restored, __VA_ARGS__)); \
      if (!orig_eq_restored) fprintf (stderr, "%s\n", serialized.data.string); \
      if (serialized.data.ptr)						\
	MR_FREE (serialized.data.ptr);					\
      ck_assert_msg (orig_eq_restored,					\
		     "restored value mismatched original for method " #METHOD " on type " #TYPE); \
      MR_FREE_RECURSIVELY (TYPE, &METHOD ## _restored);			\
    })

#define ASSERT_SAVE_LOAD_TYPE(METHOD, TYPE, VALUE, ...) ({		\
      TYPE x = { VALUE };						\
      ASSERT_SAVE_LOAD (METHOD, TYPE, &x, __VA_ARGS__);			\
    })

#define ASSERT_GET_TEST(TEST, ...) TEST
#define ASSERT_GET_ARGS(TEST, ...) __VA_ARGS__
#define ASSERT_ITERATOR(ARGS, METHOD, I) MR_IF_ELSE (MR_IS_EMPTY (METHOD)) () (ASSERT_ITERATOR_ (ASSERT_GET_TEST ARGS, METHOD, ASSERT_GET_ARGS ARGS))
#define ASSERT_ITERATOR_(TEST, METHOD, ...) MR_IF_ELSE (SKIP_METHOD_ ## METHOD) (TEST (METHOD, __VA_ARGS__)) ()
#define SERIAL(NAME, I, REC, X) REC; X
#define ALL_METHODS(...) MR_FOR ((__VA_ARGS__), MR_NARG (TEST_METHODS), SERIAL, ASSERT_ITERATOR, TEST_METHODS)

#define ADD_TCASE(NAME_DESCRIPTION) {					\
    TCase * tcase = tcase_create ("" P00_GET_SECOND NAME_DESCRIPTION);	\
    if (tcase)								\
      {									\
	tcase_set_timeout (tcase, 120);					\
	tcase_add_test (tcase, P00_GET_FIRST NAME_DESCRIPTION);		\
	suite_add_tcase (suite, tcase);					\
      }									\
  }

#define MAIN_TEST_SUITE(...)						\
  int main (int argc, char * argv[])					\
  {									\
    int number_failed;							\
    Suite * suite = suite_create (__FILE__);				\
    if (NULL == suite)							\
      return (EXIT_FAILURE);						\
    MR_FOREACH (ADD_TCASE, __VA_ARGS__);				\
    SRunner * srunner = srunner_create (suite);				\
    srunner_run_all (srunner, CK_ENV);					\
    number_failed = srunner_ntests_failed (srunner);			\
    srunner_free (srunner);						\
    return ((number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE);	\
  }									\

#endif /* _REGRESSION_H_ */
