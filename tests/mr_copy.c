#include <check.h>
#include <metaresc.h>
#include <regression.h>

TYPEDEF_STRUCT (ll_t,
		(void *, ptr));

START_TEST (mr_copy_self_void_ptr) {
  ll_t x, y;
  x.ptr = &x;

  mr_status_t status = MR_COPY_RECURSIVELY (ll_t, &x, &y);
  ck_assert_msg ((MR_SUCCESS == status), "MR_COPY_RECURSIVELY failed");

  ck_assert_msg (MR_CMP_STRUCTS (ll_t, &x, &y) == 0, "dump of MR_COPY_RECURSIVELY mismatched original");
} END_TEST

START_TEST (mr_copy_args2) {
  mr_conf_t mr_conf_copy = MR_COPY_RECURSIVELY (mr_conf_t, &mr_conf);
  int orig_eq_restored = (0 == CMP_SERIALIAZED (mr_conf_t, &mr_conf, &mr_conf_copy));
  ck_assert_msg (orig_eq_restored, "dump of MR_COPY_RECURSIVELY mismatched dump of original mr_conf");
  MR_FREE_RECURSIVELY (mr_conf_t, &mr_conf_copy);
} END_TEST

START_TEST (mr_copy_arg3) {
  mr_conf_t mr_conf_copy;
  int orig_eq_restored;
  mr_status_t status = MR_COPY_RECURSIVELY (mr_conf_t, &mr_conf, &mr_conf_copy);
  ck_assert_msg ((MR_SUCCESS == status), "MR_COPY_RECURSIVELY failed");
  orig_eq_restored = (0 == CMP_SERIALIAZED (mr_conf_t, &mr_conf, &mr_conf_copy));
  ck_assert_msg (orig_eq_restored, "dump of MR_COPY_RECURSIVELY mismatched dump of original mr_conf");
  MR_FREE_RECURSIVELY (mr_conf_t, &mr_conf_copy);
} END_TEST

MAIN_TEST_SUITE ((mr_copy_self_void_ptr, "test MR_COPY_RECURSIVELY works for self-refereed void pointers"),
		 (mr_copy_args2, "test that dump of mr_conf matches with dump of MR_COPY_RECURSIVELY (mr_conf_t, &mr_conf)"),
		 (mr_copy_arg3, "test that dump of mr_conf matches with dump of MR_COPY_RECURSIVELY (mr_conf_t, &mr_conf, &mr_conf_copy)")
		 );
