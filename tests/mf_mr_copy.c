#include <metaresc.h>

#include <check.h>
#include <regression.h>

#include <mem_failures.h>

mr_status_t
mr_copy_method (void * arg)
{
  mr_ptrdes_t * ptrs = arg;
  mr_conf_t _mr_conf;
  mr_status_t status = mr_copy_recursively (ptrs, &_mr_conf);
  if (MR_SUCCESS == status)
    {
      mr_conf.mr_mem = mr_mem;
      mr_ptrdes_t * alloc_ptrs = MR_SAVE (mr_conf_t, &_mr_conf);
      mr_conf.mr_mem = _mr_mem;
      if (alloc_ptrs != NULL)
	{
	  mr_free_recursively (alloc_ptrs);
	  mr_mem.free (__FILE__, __FUNCTION__, __LINE__, alloc_ptrs);
	}
      return (MR_SUCCESS);
    }
  return (MR_FAILURE);
}

START_TEST (mem_failures_mr_copy_once_per_allocation) {
  mr_conf_init ();
  mr_ptrdes_t * ptrs = MR_SAVE (mr_conf_t, &mr_conf);
  ck_assert_msg (ptrs != NULL, "Failed to save mr_conf for tests of MR_COPY_RECURSIVELY");
  mem_failures_method (mr_copy_method, ptrs, true);
  MR_FREE (ptrs);
} END_TEST

MAIN_TEST_SUITE ((mem_failures_mr_copy_once_per_allocation, "test memory operations failures for MR_COPY_RECURSIVELY"));
