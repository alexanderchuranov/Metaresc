#include <string.h>
#include <stddef.h>
#include <sys/times.h>
#include <check.h>
#include <metaresc.h>

/*
  Test is doing the following:
  1. for all resizeable arrays in mr_conf set ra.alloc_size to ra.size. Required for further compare with another serialized mr_conf
  2. serialize mr_conf - library config structure
  3. load back config from serialized representation into another variable
  4. replace global variable with the main mr_conf on a loaded one
  5. serialize mr_conf once again
  6. match it with the first dump
  7. restore mr_conf from saved copy
  8. free up all allocated memory

  One more tests checks that complexity of save/load is not O(n * n). It should be O(n * log (n))
  Methods that have limitation of structure depth due to recursive implementation should define
  SKIP_PERFORMANCE_TEST_{METHOD} to 0
*/

#define MULTIPLE (64)

#define SMOKE_METHOD(METHOD, ...) START_TEST (mr_conf_save_load) {	\
    mr_conf_t mr_conf_saved = mr_conf;					\
    mr_ra_mr_ptrdes_t ptrs = MR_SAVE (mr_conf_t, &mr_conf);		\
    int i;								\
    ck_assert_msg (((0 != ptrs.size) && (NULL != ptrs.ra)),		\
		   "save into internal representation failed");		\
    for (i = ptrs.size / sizeof (ptrs.ra[0]) - 1; i >= 0; --i)		\
      if (MR_TYPE_EXT_RARRAY_DATA == ptrs.ra[i].fd.mr_type_ext)		\
	{								\
	  mr_rarray_t * ra = (mr_rarray_t*)&((char*)ptrs.ra[i].data)[-offsetof (mr_rarray_t, data)]; \
	  ra->alloc_size = ra->size;					\
	}								\
      else if ((MR_TYPE_EXT_NONE == ptrs.ra[i].fd.mr_type_ext) &&	\
	       (MR_TYPE_STRUCT == ptrs.ra[i].fd.mr_type) &&		\
	       (0 == strcmp ("mr_hashed_string_t", ptrs.ra[i].fd.type))) \
	{								\
	  mr_hashed_string_t * mr_hashed_name = ptrs.ra[i].data;	\
	  mr_hashed_name->hash_value = mr_hash_str (mr_hashed_name->str); \
	}								\
    MR_FREE (ptrs.ra);							\
    mr_rarray_t mr_conf_serialized = MR_SAVE_ ## METHOD ## _RA (mr_conf_t, &mr_conf); \
      ck_assert_msg (((0 != mr_conf_serialized.size) && (NULL != mr_conf_serialized.data)), \
		     "save for method " #METHOD " failed");		\
      mr_conf_t mr_conf_loaded;						\
      ck_assert_msg (MR_SUCCESS == MR_LOAD_ ## METHOD ## _RA (mr_conf_t, &mr_conf_serialized, &mr_conf_loaded), \
		     "load for method " #METHOD " failed");		\
      ptrs = MR_SAVE (mr_conf_t, &mr_conf_loaded);			\
      ck_assert_msg (((0 != ptrs.size) && (NULL != ptrs.ra)),		\
		     "save into internal representation failed");	\
      MR_FREE (ptrs.ra);						\
      mr_conf = mr_conf_loaded;						\
      mr_rarray_t mr_conf_serialized_ = MR_SAVE_ ## METHOD ## _RA (mr_conf_t, &mr_conf); \
	ck_assert_msg ((mr_conf_serialized.size == mr_conf_serialized_.size) && \
		       (0 == memcmp (mr_conf_serialized.data, mr_conf_serialized_.data, mr_conf_serialized.size)), \
		       "restored mr_conf mismatched original dump for method " #METHOD); \
	MR_FREE (mr_conf_serialized_.data);				\
	MR_FREE (mr_conf_serialized.data);				\
	mr_conf = mr_conf_saved;					\
	MR_FREE_RECURSIVELY (mr_conf_t, &mr_conf_loaded);		\
  } END_TEST								\
  TYPEDEF_STRUCT (mr_empty_t);						\
  TYPEDEF_STRUCT (mr_incomplete_t, (int, x, [0]), NONE (int, y[]));	\
  TYPEDEF_STRUCT (list_t, (mr_ptr_t, mr_ptr, , "ptr_type"), (list_t *, next)); \
  TYPEDEF_STRUCT (typed_list_t, (char *, ptr_type), (list_t *, root));	\
  TYPEDEF_STRUCT (array_t, RARRAY (list_t, ra));			\
  int test_run (int count)						\
  {									\
    int i;								\
    array_t array, array_;						\
    mr_rarray_t ra;							\
    typed_list_t list_, typed_list = { .ptr_type = "char", };		\
    struct tms start, end;						\
    times (&start);							\
    memset (&array, 0, sizeof (array));					\
    array.ra.size = count * sizeof (array.ra.data[0]);			\
    array.ra.data = MR_MALLOC (array.ra.size);				\
    ck_assert_msg (NULL != array.ra.data, "Memory allocation failed.");	\
    for (i = 1; i < count; ++i)						\
      {									\
	array.ra.data[i - 1].next = &array.ra.data[i];			\
	array.ra.data[i - 1].mr_ptr.ptr = "string_t";			\
      }									\
    array.ra.data[count - 1].next = &array.ra.data[0];			\
    array.ra.data[count - 1].mr_ptr.ptr = "string_t";			\
    typed_list.root = &array.ra.data[0];				\
    ra = MR_SAVE_ ## METHOD ## _RA (typed_list_t, &typed_list);		\
      ck_assert_msg ((ra.size > 0) && (ra.data != NULL),		\
		     "Serialization for method " #METHOD " failed.");	\
      ck_assert_msg (MR_SUCCESS ==					\
		     MR_LOAD_ ## METHOD ## _RA (typed_list_t, &ra, &list_), \
		     "Deserialization for method " #METHOD " failed.");	\
      MR_FREE_RECURSIVELY (typed_list_t, &list_);			\
      MR_FREE (ra.data);						\
      array.ra.res_type = "char";					\
      ra = MR_SAVE_ ## METHOD ## _RA (array_t, &array);			\
	ck_assert_msg ((ra.size > 0) && (ra.data != NULL),		\
		       "Serialization for method " #METHOD " failed.");	\
	ck_assert_msg (MR_SUCCESS ==					\
		       MR_LOAD_ ## METHOD ## _RA (array_t, &ra, &array_), \
		       "Deserialization for method " #METHOD " failed."); \
	MR_FREE_RECURSIVELY (array_t, &array_);				\
	MR_FREE (ra.data);						\
	MR_FREE (array.ra.data);					\
	times (&end);							\
	return ((int)((end.tms_utime - start.tms_utime)));		\
  }									\
  START_TEST (test_performance) {					\
    MR_IF_ELSE (MR_PASTE2 (SKIP_PERFORMANCE_TEST_, METHOD)) ()(return;)	\
      int size = 2;							\
    do size += size >> 1; while (0 == test_run (size));			\
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
