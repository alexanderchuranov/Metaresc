#include <metaresc.h>

#include <check.h>
#include <regression.h>

#include <mem_failures.h>

mr_status_t
mr_save_method (void * arg)
{
  mr_ptrdes_t * ptrs = MR_SAVE (mr_conf_t, &mr_conf);
  if (ptrs)
    {
      MR_FREE (ptrs);
      return (MR_SUCCESS);
    }
  return (MR_FAILURE);
}

START_TEST (mem_failures_mr_save_once_per_allocation) { mem_failures_method (mr_save_method, NULL, true); } END_TEST
START_TEST (mem_failures_mr_save_on_every_allocation) { mem_failures_method (mr_save_method, NULL, false); } END_TEST

MAIN_TEST_SUITE ((mem_failures_mr_save_once_per_allocation, "test memory operations failures for MR_SAVE (fail once per allocation)"),
		 (mem_failures_mr_save_on_every_allocation, "test memory operations failures for MR_SAVE (fail after N allocations)")
		 );
