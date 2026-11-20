#include <check.h>
#include <metaresc.h>
#include <regression.h>

/*
  The type of the data in mr_res_t is defined by the text field `type`, which
  is the discriminator of the union `data`, and the size of the data by
  `mr_size`. The document below is inconsistent in both ways. The size of
  the dynamic array is 1, but the initializer has 8 bytes. And the branch of
  the discriminated union is `uint8_t`, while the data is initialized as an
  array of `uint64_t`. A document like this must be rejected by the loader.
*/

#define MR_RES_CINIT { .type = "uint8_t", .mr_size = 1, .data = { (uint64_t[]){ 0x0123456789ABCDEF } }, }

START_TEST (cinit_load_oversized_resource) {
  char text[] = MR_STRINGIFY_READONLY (MR_RES_CINIT);
  mr_res_t res;
  memset (&res, 0, sizeof (res));

  mr_status_t status = MR_LOAD_CINIT (mr_res_t, text, &res);
  ck_assert_msg (MR_FAILURE == status, "load of a resource with data larger than its size succeeded");

  MR_FREE_RECURSIVELY (mr_res_t, &res);
} END_TEST

MAIN_TEST_SUITE ((cinit_load_oversized_resource, "load of a resource with data larger than its size fails"));
