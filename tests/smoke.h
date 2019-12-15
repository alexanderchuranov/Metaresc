#include <string.h>
#include <stddef.h>
#include <sys/times.h>
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

  One more test checks that complexity of save/load is not O(n * n). It should be O(n * log (n))
  Methods that have limitation of structure depth due to recursive implementation should define
  SKIP_PERFORMANCE_TEST_{METHOD} to 0
*/

#define MULTIPLE (32)

#define SMOKE_METHOD(METHOD, ...) START_TEST (mr_conf_save_load) {	\
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
	mr_conf = mr_conf_saved;					\
	MR_FREE_RECURSIVELY (mr_conf_t, &mr_conf_loaded);		\
  } END_TEST								\
									\
  TYPEDEF_STRUCT (mr_empty_t);						\
  TYPEDEF_STRUCT (mr_incomplete_t, (int, x, [0]), VOID (int, y, []));	\
  TYPEDEF_STRUCT (list_t, (mr_ptr_t, mr_ptr, , "ptr_type"), (list_t *, next)); \
  TYPEDEF_STRUCT (typed_list_t, (char *, ptr_type), (list_t *, root));	\
  TYPEDEF_STRUCT (array_t, (list_t *, ra, , , { "size" }, "char"), (ssize_t, size), (char *, ptr_type)); \
									\
  int test_run (int count)						\
  {									\
    int i;								\
    array_t array, array_;						\
    mr_rarray_t ra;							\
    typed_list_t list_, typed_list = { .ptr_type = "char", };		\
    struct tms start, end;						\
    times (&start);							\
    memset (&array, 0, sizeof (array));					\
    array.size = count * sizeof (array.ra[0]);				\
    array.ptr_type = "char";						\
    array.ra = MR_MALLOC (array.size);					\
    ck_assert_msg (NULL != array.ra, "Memory allocation failed.");	\
    for (i = 1; i < count; ++i)						\
      {									\
	array.ra[i - 1].next = &array.ra[i];				\
	array.ra[i - 1].mr_ptr.ptr = "string_t";			\
      }									\
    array.ra[count - 1].next = &array.ra[0];				\
    array.ra[count - 1].mr_ptr.ptr = "string_t";			\
    typed_list.root = &array.ra[0];					\
    ra = MR_SAVE_ ## METHOD ## _RA (typed_list_t, &typed_list);		\
      ck_assert_msg ((ra.MR_SIZE > 0) && (ra.data.ptr != NULL),		\
		     "Serialization for method " #METHOD " failed.");	\
      ck_assert_msg (MR_SUCCESS ==					\
		     MR_LOAD_ ## METHOD ## _RA (typed_list_t, &ra, &list_), \
		     "Deserialization for method " #METHOD " failed.");	\
      MR_FREE_RECURSIVELY (typed_list_t, &list_);			\
      MR_FREE (ra.data.ptr);						\
      ra = MR_SAVE_ ## METHOD ## _RA (array_t, &array);			\
	ck_assert_msg ((ra.MR_SIZE > 0) && (ra.data.ptr != NULL),	\
		       "Serialization for method " #METHOD " failed.");	\
	ck_assert_msg (MR_SUCCESS ==					\
		       MR_LOAD_ ## METHOD ## _RA (array_t, &ra, &array_), \
		       "Deserialization for method " #METHOD " failed."); \
	MR_FREE_RECURSIVELY (array_t, &array_);				\
	MR_FREE (ra.data.ptr);						\
	MR_FREE (array.ra);						\
	times (&end);							\
	return ((int)((end.tms_utime - start.tms_utime)));		\
  }									\
  START_TEST (test_performance) {					\
    MR_IF_ELSE (MR_PASTE2 (SKIP_PERFORMANCE_TEST_, METHOD)) ()(return;)	\
    int size = 2;							\
    do size += size >> 1; while (test_run (size) == 0);			\
    int x1 = test_run (size * MULTIPLE);				\
    int x2 = test_run (size * MULTIPLE * 4);				\
    ck_assert_msg (((double)x2 / (double)x1 < 5), "performance issue for method " #METHOD); \
  } END_TEST								\
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
    tcase_add_test (tcase, test_performance);				\
    suite_add_tcase (suite, tcase);					\
    srunner_run_all (srunner, CK_ENV);					\
    number_failed = srunner_ntests_failed (srunner);			\
    srunner_free (srunner);						\
    return ((number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE);	\
  }
