#include <metaresc.h>

#include <check.h>
#include <regression.h>

#include <mem_failures.h>

mr_status_t
mr_save_method (void * arg)
{
  mr_ra_ptrdes_t ptrs = MR_SAVE (mr_conf_t, &mr_conf);
  if (ptrs.ra)
    {
      MR_FREE (ptrs.ra);
      return (MR_SUCCESS);
    }
  return (MR_FAILURE);
}

MR_START_TEST (mem_failures_mr_save, "test memory operations failures for MR_SAVE") { mem_failures_method (mr_save_method, NULL); } END_TEST

