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
    if ((0 == ptrs.ra.size) || (NULL == ptrs.ra.data))			\
      ck_abort_msg ("save into internal representation failed");	\
    for (i = ptrs.ra.size / sizeof (ptrs.ra.data[0]) - 1; i >= 0; --i)	\
      if (MR_TYPE_EXT_RARRAY_DATA == ptrs.ra.data[i].fd.mr_type_ext)	\
	{								\
	  mr_rarray_t * ra = (mr_rarray_t*)&((char*)ptrs.ra.data[i].data)[-offsetof (mr_rarray_t, data)]; \
	  ra->alloc_size = ra->size;					\
	}								\
    MR_FREE (ptrs.ra.data);						\
    mr_rarray_t mr_conf_serialized = MR_SAVE_ ## METHOD ## _RA (mr_conf_t, &mr_conf); \
    if ((0 == mr_conf_serialized.size) || (NULL == mr_conf_serialized.data)) \
      ck_abort_msg ("save for method " #METHOD " failed");		\
    mr_conf_t mr_conf_loaded = mr_conf;					\
    if (0 == MR_LOAD_ ## METHOD ## _RA (mr_conf_t, &mr_conf_serialized, &mr_conf_loaded)) \
      ck_abort_msg ("load for method " #METHOD " failed");		\
    mr_conf = mr_conf_loaded;						\
    mr_rarray_t mr_conf_serialized_ = MR_SAVE_ ## METHOD ## _RA (mr_conf_t, &mr_conf); \
    if ((mr_conf_serialized.size != mr_conf_serialized_.size) ||	\
	(0 != memcmp (mr_conf_serialized.data, mr_conf_serialized_.data, mr_conf_serialized.size))) \
      ck_abort_msg ("restored mr_conf mismatched original dump for method " #METHOD); \
    MR_FREE (mr_conf_serialized_.data);					\
    MR_FREE (mr_conf_serialized.data);					\
    mr_conf = mr_conf_saved;						\
    MR_FREE_RECURSIVELY (mr_conf_t, &mr_conf_loaded);			\
  } END_TEST								\
  TYPEDEF_STRUCT (mr_empty_t);						\
  TYPEDEF_STRUCT (mr_incomplete_t, (int, x, [0]), NONE (int, y[]));	\
  TYPEDEF_STRUCT (list_t, (mr_ptr_t, mr_ptr, , "ptr_type"), (list_t *, next)); \
  TYPEDEF_STRUCT (typed_list_t, (char *, ptr_type), (list_t *, root));	\
  TYPEDEF_STRUCT (array_t, RARRAY (list_t, ra));			\
  int test_run (int count)						\
  {									\
    array_t array;							\
    mr_rarray_t ra;							\
    typed_list_t typed_list = { .ptr_type = "string_t", };		\
    struct tms start, end;						\
    times (&start);							\
    memset (&array, 0, sizeof (array));					\
    array.ra.size = count * sizeof (array.ra.data[0]);			\
    array.ra.data = MR_MALLOC (array.ra.size);				\
    if (array.ra.data)							\
      {									\
	int i;								\
	int count_ = array.ra.size / sizeof (array.ra.data[0]);		\
	for (i = 1; i < count_; ++i)					\
	  {								\
	    array.ra.data[i - 1].next = &array.ra.data[i];		\
	    array.ra.data[i - 1].mr_ptr.ptr = "string_t";		\
	  }								\
	array.ra.data[count_ - 1].next = &array.ra.data[0];		\
	array.ra.data[count_ - 1].mr_ptr.ptr = "string_t";		\
	typed_list.root = &array.ra.data[0];				\
	ra = MR_SAVE_ ## METHOD ## _RA (typed_list_t, &typed_list);	\
	if ((ra.size > 0) && (ra.data != NULL))				\
	  {								\
	    typed_list_t list_;						\
	    if (MR_LOAD_ ## METHOD ## _RA (typed_list_t, &ra, &list_))	\
	      MR_FREE_RECURSIVELY (typed_list_t, &list_);		\
	    MR_FREE (ra.data);						\
	  }								\
	array.ra.ptr_type = "string_t";					\
	ra = MR_SAVE_ ## METHOD ## _RA (array_t, &array);		\
	if ((ra.size > 0) && (ra.data != NULL))				\
	  {								\
	    array_t array_;						\
	    if (MR_LOAD_ ## METHOD ## _RA (array_t, &ra, &array_))	\
	      MR_FREE_RECURSIVELY (array_t, &array_);			\
	    MR_FREE (ra.data);						\
	  }								\
	MR_FREE (array.ra.data);					\
      }									\
    times (&end);							\
    return ((int)((end.tms_utime - start.tms_utime)));			\
  }									\
  START_TEST (test_performance) {					\
    MR_IF_ELSE (MR_PASTE2 (SKIP_PERFORMANCE_TEST_, METHOD)) ()(return;)	\
    int size = 2;							\
    do size += size >> 1; while (0 == test_run (size));			\
    int x1 = test_run (size * MULTIPLE);				\
    int x2 = test_run (size * MULTIPLE * 4);				\
    if ((double)x2 / (double)x1 > 5)					\
      ck_abort_msg ("performance issue for method " #METHOD);		\
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
