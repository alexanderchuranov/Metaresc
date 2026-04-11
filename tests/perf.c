#include <time.h>

#include <metaresc.h>
#include <check.h>

#include <regression.h>

TYPEDEF_STRUCT (list_t,
		(mr_ptr_t, mr_ptr, , "ptr_type"),
		(list_t *, next));

TYPEDEF_STRUCT (typed_list_t,
		(char *, ptr_type),
		(list_t *, root));

TYPEDEF_STRUCT (vector_t,
		(vector_t *, vec),
		(size_t, vec_count));

static clock_t
union_resolution (int count, void (*save_load) (mr_res_t * res))
{
  int i;
  list_t * ra = MR_CALLOC (count, sizeof (ra[0]));
  ck_assert_msg (ra != NULL, "Memory allocation failed.");

  for (i = 1; i < count; ++i)
    {
      ra[i - 1].next = &ra[i];
      ra[i - 1].mr_ptr.ptr = "string";
    }
  ra[count - 1].next = &ra[0];

  typed_list_t typed_list = {
    .ptr_type = "string",
    .root = &ra[0],
  };

  mr_res_t res = {
    .type = "typed_list_t",
    .MR_SIZE = sizeof (typed_list_t),
    .data.ptr = &typed_list,
  };

  clock_t _start = clock ();
  save_load (&res);
  clock_t _finish = clock ();

  MR_FREE (ra);

  return (_finish - _start);
}

static clock_t
pointers_extension_overlap (int count, void (*save_load) (mr_res_t * res))
{
  int i;
  vector_t * ra = MR_CALLOC (count, sizeof (ra[0]));
  ck_assert_msg (ra != NULL, "Memory allocation failed.");

  for (i = 0; i < count; ++i)
    {
      ra[i].vec = &ra[i];
      ra[i].vec_count = 2;
    }
  ra[count - 1].vec_count = 1;

  mr_res_t res = {
    .type = "vector_t",
    .MR_SIZE = sizeof (vector_t),
    .data.ptr = &ra[0],
  };

  clock_t _start = clock ();
  save_load (&res);
  clock_t _finish = clock ();

  MR_FREE (ra);

  return (_finish - _start);
}

static void
check_cpu_complexity (void (*save_load) (mr_res_t * res), char * method,
		      clock_t (*test_run) (int count, void (*save_load) (mr_res_t * res)))
{
  int i, size = 1 << 8;
  clock_t base_time, double_time = test_run (size, save_load);

  do {
    size <<= 1;
    base_time = double_time;
    double_time = test_run (size, save_load);
  } while (double_time < CLOCKS_PER_SEC / 32);

  for (i = 0; i < 3; ++i)
    {
      clock_t another_try = test_run (size, save_load);
      double_time = MR_MIN (double_time, another_try);
    }

  ck_assert_msg (double_time < (5 * base_time) / 2,
		 "performance issue for method '%s' %lu / %lu = %.02g",
		 method, (unsigned long)double_time, (unsigned long)base_time, (double)double_time / base_time);
}  

static void
mr_save_callback (mr_res_t * res)
{
  mr_ptrdes_t * ptrs = MR_SAVE (mr_res_t, res);
  ck_assert_msg (ptrs != NULL, "MR_SAVE failed");
  MR_FREE (ptrs);
}

START_TEST (union_resolution_complexity_mr_save) {
  check_cpu_complexity (mr_save_callback, "MR_SAVE", union_resolution);
} END_TEST

START_TEST (pointers_extension_overlap_complexity_mr_save) {
  check_cpu_complexity (mr_save_callback, "MR_SAVE", pointers_extension_overlap);
} END_TEST

#define CPU_COMPLEXITY(METHOD, ...)					\
  static void mr_save_ ## METHOD ## _callback (mr_res_t * res)		\
  {									\
    mr_rarray_t ra = MR_SAVE_ ## METHOD ## _RA (mr_res_t, res);		\
    ck_assert_msg (ra.data.ptr != NULL, "MR_SAVE_" #METHOD "_RA failed"); \
    mr_res_t _res = {};							\
    mr_status_t status = MR_LOAD_ ## METHOD ## _RA (mr_res_t, &ra, &_res); \
    ck_assert_msg (status == MR_SUCCESS, "MR_LOAD_" #METHOD "_RA failed"); \
    MR_FREE_RECURSIVELY (mr_res_t, &_res);				\
    MR_FREE (ra.data.ptr);						\
  }									\
  START_TEST (union_resolution_complexity_ ## METHOD) {			\
    check_cpu_complexity (mr_save_ ## METHOD ## _callback, #METHOD,	\
			  union_resolution);				\
  } END_TEST								\
  START_TEST (pointers_extension_overlap_complexity_ ## METHOD) {	\
    check_cpu_complexity (mr_save_ ## METHOD ## _callback, #METHOD,	\
			  pointers_extension_overlap);			\
  } END_TEST

#define SKIP_METHOD_XDR 0
#define SKIP_METHOD_XML2 0
#define SKIP_METHOD_YAML 0

ALL_METHODS (CPU_COMPLEXITY);

#define CPU_OPS_COMPLEXITY(METHOD)					\
  MR_IF_ELSE (SKIP_METHOD_ ## METHOD)					\
    ((union_resolution_complexity_ ## METHOD, "test union resolution complexity for " #METHOD), \
     (pointers_extension_overlap_complexity_ ## METHOD, "test union resolution complexity for " #METHOD), \
     )									\
    ()

MAIN_TEST_SUITE (
		 MR_FOREACH (CPU_OPS_COMPLEXITY, TEST_METHODS)
		 (union_resolution_complexity_mr_save, "test union resolution complexity for MR_SAVE"),
		 (pointers_extension_overlap_complexity_mr_save, "test pointers extension overlap complexity for MR_SAVE")
		 );
