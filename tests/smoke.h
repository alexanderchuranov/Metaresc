#include <string.h>
#include <stddef.h>
#include <sys/times.h>
#include <check.h>
#include <reslib.h>

/*
  Test is doing the following:
  1. for all resizeable arrays in rl_conf set ra.alloc_size to ra.size. Required for further compare with another serialized rl_conf
  2. serialize rl_conf - library config structure
  3. load back config from serialized representation into another variable
  4. replace global variable with the main rl_conf on a loaded one
  5. serialize rl_conf once again
  6. match it with the first dump
  7. restore rl_conf from saved copy
  8. free up all allocated memory

  One more tests checks that complexity of save/load is not O(n * n). It should be O(n * log (n))
  Methods that have limitation of structure depth due to recursive implementation should define
  SKIP_PERFORMANCE_TEST_{METHOD} to 0
*/

#define MULTIPLE (64)

#define SMOKE_METHOD(METHOD, ...) START_TEST (rl_conf_save_load) {	\
    rl_conf_t rl_conf_saved = rl_conf;					\
    rl_ra_rl_ptrdes_t ptrs = RL_SAVE_RL (rl_conf_t, &rl_conf);		\
    int i;								\
    if ((0 == ptrs.ra.size) || (NULL == ptrs.ra.data))			\
      ck_abort_msg ("save into internal representation failed");	\
    for (i = ptrs.ra.size / sizeof (ptrs.ra.data[0]) - 1; i >= 0; --i)	\
      if (RL_TYPE_EXT_RARRAY_DATA == ptrs.ra.data[i].fd.rl_type_ext)	\
	{								\
	  rl_rarray_t * ra = (rl_rarray_t*)&((char*)ptrs.ra.data[i].data)[-offsetof (rl_rarray_t, data)]; \
	  ra->alloc_size = ra->size;					\
	}								\
    RL_FREE (ptrs.ra.data);						\
    rl_rarray_t rl_conf_serialized = RL_SAVE_ ## METHOD ## _RA (rl_conf_t, &rl_conf); \
    if ((0 == rl_conf_serialized.size) || (NULL == rl_conf_serialized.data)) \
      ck_abort_msg ("save for method " #METHOD " failed");		\
    rl_conf_t rl_conf_loaded = rl_conf;					\
    if (0 == RL_LOAD_ ## METHOD ## _RA (rl_conf_t, &rl_conf_serialized, &rl_conf_loaded)) \
      ck_abort_msg ("load for method " #METHOD " failed");		\
    rl_conf = rl_conf_loaded;						\
    rl_rarray_t rl_conf_serialized_ = RL_SAVE_ ## METHOD ## _RA (rl_conf_t, &rl_conf); \
    if ((rl_conf_serialized.size != rl_conf_serialized_.size) ||	\
	(0 != memcmp (rl_conf_serialized.data, rl_conf_serialized_.data, rl_conf_serialized.size))) \
      ck_abort_msg ("restored rl_conf mismatched original dump for method " #METHOD); \
    RL_FREE (rl_conf_serialized_.data);					\
    RL_FREE (rl_conf_serialized.data);					\
    rl_conf = rl_conf_saved;						\
    RL_FREE_RECURSIVELY (rl_conf_t, &rl_conf_loaded);			\
  } END_TEST								\
  TYPEDEF_STRUCT (rl_empty_t);						\
  TYPEDEF_STRUCT (rl_incomplete_t, (int, x, [0]), NONE (int, y, []));	\
  TYPEDEF_STRUCT (list_t, (list_t *, next));				\
  TYPEDEF_STRUCT (array_t, RARRAY (list_t, ra));			\
  int test_run (int count)						\
  {									\
    array_t array;							\
    rl_rarray_t ra;							\
    struct tms start, end;						\
    times (&start);							\
    memset (&array, 0, sizeof (array));					\
    array.ra.size = count * sizeof (array.ra.data[0]);			\
    array.ra.data = RL_MALLOC (array.ra.size);				\
    if (array.ra.data)							\
      {									\
	int i;								\
	int count_ = array.ra.size / sizeof (array.ra.data[0]);		\
	for (i = 1; i < count_; ++i)					\
	  array.ra.data[i - 1].next = &array.ra.data[i];		\
	array.ra.data[count_ - 1].next = &array.ra.data[0];		\
	ra = RL_SAVE_ ## METHOD ## _RA (list_t, array.ra.data);		\
	if ((ra.size > 0) && (ra.data != NULL))				\
	  {								\
	    list_t list_;						\
	    if (RL_LOAD_ ## METHOD ## _RA (list_t, &ra, &list_))	\
	      RL_FREE_RECURSIVELY (list_t, &list_);			\
	    RL_FREE (ra.data);						\
	  }								\
	ra = RL_SAVE_ ## METHOD ## _RA (array_t, &array);		\
	if ((ra.size > 0) && (ra.data != NULL))				\
	  {								\
	    array_t array_;						\
	    if (RL_LOAD_ ## METHOD ## _RA (array_t, &ra, &array_))	\
	      RL_FREE_RECURSIVELY (array_t, &array_);			\
	    RL_FREE (ra.data);						\
	  }								\
	RL_FREE (array.ra.data);					\
      }									\
    times (&end);							\
    return ((int)((end.tms_utime - start.tms_utime)));			\
  }									\
  START_TEST (test_performance) {					\
    RL_IF_ELSE (RL_PASTE2 (SKIP_PERFORMANCE_TEST_, METHOD)) ()(return;)	\
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
    TCase * tcase = tcase_create ("rl_conf save/load");			\
    if (NULL == tcase)							\
      return (EXIT_FAILURE);						\
    tcase_set_timeout (tcase, 0);					\
    tcase_add_test (tcase, rl_conf_save_load);				\
    tcase_add_test (tcase, test_performance);				\
    suite_add_tcase (suite, tcase);					\
    srunner_run_all (srunner, CK_ENV);					\
    number_failed = srunner_ntests_failed (srunner);			\
    srunner_free (srunner);						\
    return ((number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE);	\
  }
