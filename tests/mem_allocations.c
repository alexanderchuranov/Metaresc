#include <metaresc.h>
#include <check.h>

#include <regression.h>

#define REFERENCE_SIZE (1 << 10)
#define REFERENCE_TYPE int32_t

mr_mem_t mr_mem;
static int mr_malloc_cnt = 0;
static int mr_realloc_cnt = 0;
static int mr_free_cnt = 0;

static void * _calloc (const char * filename, const char * function, int line, size_t count, size_t size)
{
  ++mr_malloc_cnt;
  return (mr_mem.calloc (filename, function, line, count, size));
}

static void * _realloc (const char * filename, const char * function, int line, void * ptr, size_t size)
{
  ++mr_realloc_cnt;
  return (mr_mem.realloc (filename, function, line, ptr, size));
}

static void _free (const char * filename, const char * function, int line, void * ptr)
{
  ++mr_free_cnt;
  mr_mem.free (filename, function, line, ptr);
}

static int
get_number_of_memory_ops (int n, void callback (mr_res_t * res))
{
  int i;
  mr_res_t res;
  REFERENCE_TYPE * array;
  
  res.type = MR_STRINGIFY_READONLY (REFERENCE_TYPE);
  res.MR_SIZE = n * sizeof (REFERENCE_TYPE);
  res.data.ptr = array = MR_CALLOC (n, sizeof (REFERENCE_TYPE));

  ck_assert_msg (array != NULL, "Failed to allocate reference array");

  for (i = 0; i < n; ++i)
    array[i] = i;

  mr_malloc_cnt = mr_realloc_cnt = mr_free_cnt = 0;
  callback (&res);
  int count = mr_malloc_cnt + mr_realloc_cnt + mr_free_cnt;

  MR_FREE (res.data.ptr);
  return (count);
}

static void
check_mem_ops_complexity (void callback (mr_res_t * res), char * method)
{ 
  mr_detect_type (NULL);
  mr_mem = mr_conf.mr_mem;
  mr_conf.mr_mem.calloc = _calloc;
  mr_conf.mr_mem.realloc = _realloc;
  mr_conf.mr_mem.free = _free;
  
  int mem_ops_for_doubled_size =
    get_number_of_memory_ops (REFERENCE_SIZE << 1, callback) - get_number_of_memory_ops (REFERENCE_SIZE, callback);

  mr_conf.mr_mem = mr_mem;
  
  ck_assert_msg (mem_ops_for_doubled_size < 12,
		 "Number of memory operations (%d) grows higher than logarithmically for method %s",
		 mem_ops_for_doubled_size, method);
}

static void
mr_save_callback (mr_res_t * res)
{
  mr_ra_ptrdes_t ptrs = MR_SAVE (mr_res_t, res);
  if (ptrs.ra)
    MR_FREE (ptrs.ra);
}

START_TEST (mem_allocation_complexity_mr_save) {
  check_mem_ops_complexity (mr_save_callback, "MR_SAVE");
} END_TEST

#define CHECK_MEM_OPS_COMPLEXITY(METHOD, ...)				\
  static void MR_SAVE_ ## METHOD ## _CALLBACK (mr_res_t * res)		\
  {									\
    mr_rarray_t ra = MR_SAVE_ ## METHOD ## _RA (mr_res_t, res);		\
    if (ra.data.ptr)							\
      MR_FREE (ra.data.ptr);						\
  }									\
  START_TEST (mem_allocation_complexity_ ## METHOD) {			\
    check_mem_ops_complexity (MR_SAVE_ ## METHOD ## _CALLBACK, #METHOD); \
  } END_TEST

  ALL_METHODS (CHECK_MEM_OPS_COMPLEXITY);

#define MEM_OPS_COMPLEXITY(METHOD) MR_IF_ELSE (MR_IS_EMPTY (METHOD)) ()	\
    ((mem_allocation_complexity_ ## METHOD,				\
      "test number of memory allocations for " #METHOD),)

MAIN_TEST_SUITE (
		 MR_FOREACH (MEM_OPS_COMPLEXITY, TEST_METHODS)
		 (mem_allocation_complexity_mr_save, "test number of memory allocations for MR_SAVE")
		 );
