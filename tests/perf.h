#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#include <check.h>
#include <metaresc.h>

#define TEST_PERFORMANCE(METHOD, ...)					\
  TYPEDEF_STRUCT (list_t, (mr_ptr_t, mr_ptr, , "ptr_type"), (list_t *, next)); \
  TYPEDEF_STRUCT (typed_list_t, (char *, ptr_type), (list_t *, root));	\
  TYPEDEF_STRUCT (array_t, (list_t *, ra, , , { "size" }, "string"), (ssize_t, size), (char *, ptr_type)); \
									\
  int test_run (int count)						\
  {									\
    int i;								\
    array_t array, array_;						\
    mr_rarray_t ra;							\
    typed_list_t list_, typed_list = { .ptr_type = "string", };		\
    memset (&array, 0, sizeof (array));					\
    array.size = count * sizeof (array.ra[0]);				\
    array.ptr_type = "string";						\
    array.ra = MR_CALLOC (count, sizeof (array.ra[0]));			\
    ck_assert_msg (NULL != array.ra, "Memory allocation failed.");	\
    for (i = 1; i < count; ++i)						\
      {									\
	array.ra[i - 1].next = &array.ra[i];				\
	array.ra[i - 1].mr_ptr.ptr = "string";				\
      }									\
    array.ra[count - 1].next = &array.ra[0];				\
    array.ra[count - 1].mr_ptr.ptr = "string";				\
    typed_list.root = &array.ra[0];					\
    clock_t _start = clock ();						\
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
	clock_t _finish = clock ();					\
	return (_finish - _start);					\
  }									\
									\
  START_TEST (test_performance) {					\
    mr_conf_init ();							\
    int i, size = 1 << 8;						\
    int base_time, double_time = test_run (size);			\
    do {								\
      size <<= 1;							\
      base_time = double_time;						\
      double_time = test_run (size);					\
    } while (double_time < CLOCKS_PER_SEC / 8);				\
    for (i = 0; i < 3; ++i)						\
      {									\
	int another_try = test_run (size);				\
	double_time = MR_MIN (double_time, another_try);		\
      }									\
    ck_assert_msg (double_time < (5 * base_time) / 2, "performance issue for method " #METHOD " %d / %d = %.02g", double_time, base_time, (double)double_time / base_time); \
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
    TCase * tcase = tcase_create ("performance test");			\
    if (NULL == tcase)							\
      return (EXIT_FAILURE);						\
    tcase_set_timeout (tcase, 0);					\
    tcase_add_test (tcase, test_performance);				\
    suite_add_tcase (suite, tcase);					\
    srunner_run_all (srunner, CK_ENV);					\
    number_failed = srunner_ntests_failed (srunner);			\
    srunner_free (srunner);						\
    return ((number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE);	\
  }
