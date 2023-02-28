#include <string.h>
#include <stddef.h>
#include <stdlib.h>

#include <check.h>
#include <metaresc.h>

/*
  Test is doing the following:
  1. serialize mr_conf - library config structure
  2. load back config from serialized representation into another variable
  3. replace global variable with the main mr_conf on a loaded one
  4. serialize mr_conf once again using loaded configuration
  5. match it with the first dump
  6. restore mr_conf from saved copy
  7. free up all allocated memory

  One more test checks that complexity of save/load is not O(n * n). It should be O(n).
  Methods that have limitation of structure depth due to recursive implementation should define
  SKIP_PERFORMANCE_TEST_{METHOD} to 0
*/

#define SMOKE_METHOD(METHOD, ...) START_TEST (mr_conf_save_load) {	\
    mr_conf_init ();							\
    mr_conf_t mr_conf_saved = mr_conf;					\
    mr_rarray_t mr_conf_serialized = MR_SAVE_ ## METHOD ## _RA (mr_conf_t, &mr_conf); \
      ck_assert_msg (((0 != mr_conf_serialized.MR_SIZE) && (NULL != mr_conf_serialized.data.ptr)), \
		     "save for method " #METHOD " failed");		\
      mr_conf_t mr_conf_loaded;						\
      ck_assert_msg (MR_SUCCESS == MR_LOAD_ ## METHOD ## _RA (mr_conf_t, &mr_conf_serialized, &mr_conf_loaded), \
		     "load for method " #METHOD " failed");		\
      mr_conf = mr_conf_loaded;						\
      mr_rarray_t mr_conf_serialized_ = MR_SAVE_ ## METHOD ## _RA (mr_conf_t, &mr_conf); \
      ck_assert_msg ((mr_conf_serialized.MR_SIZE == mr_conf_serialized_.MR_SIZE) && \
		     (0 == memcmp (mr_conf_serialized.data.ptr, mr_conf_serialized_.data.ptr, mr_conf_serialized.MR_SIZE)), \
		     "restored mr_conf mismatched original dump for method " #METHOD); \
      MR_FREE (mr_conf_serialized_.data.ptr);				\
      MR_FREE (mr_conf_serialized.data.ptr);				\
      mr_conf = mr_conf_saved;						\
      MR_FREE_RECURSIVELY (mr_conf_t, &mr_conf_loaded);			\
  } END_TEST								\
									\
  int main (int argc, char * argv[])					\
  {									\
    int number_failed;							\
    Suite * suite = suite_create ("main");				\
    if (NULL == suite)							\
      return (EXIT_FAILURE);						\
    SRunner * srunner = srunner_create (suite);				\
    if (NULL == srunner)						\
      return (EXIT_FAILURE);						\
    TCase * tcase = tcase_create ("mr_conf save/load");			\
    if (NULL == tcase)							\
      return (EXIT_FAILURE);						\
    tcase_set_timeout (tcase, 0);					\
    tcase_add_test (tcase, mr_conf_save_load);				\
    suite_add_tcase (suite, tcase);					\
    srunner_run_all (srunner, CK_ENV);					\
    number_failed = srunner_ntests_failed (srunner);			\
    srunner_free (srunner);						\
    return ((number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE);	\
  }
