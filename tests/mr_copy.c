#include <check.h>
#include <metaresc.h>
#include <regression.h>

MR_START_TEST (mr_copy_args2, "test that dump of mr_conf matches with dump of MR_COPY_RECURSIVELY (mr_conf_t, &mr_conf)") {
  mr_conf_t mr_conf_copy = MR_COPY_RECURSIVELY (mr_conf_t, &mr_conf);
  int orig_eq_restored = (0 == CMP_SERIALIAZED (mr_conf_t, &mr_conf, &mr_conf_copy));
  ck_assert_msg (orig_eq_restored, "dump of MR_COPY_RECURSIVELY mismatched dump of original mr_conf");
  MR_FREE_RECURSIVELY (mr_conf_t, &mr_conf_copy);
} END_TEST

MR_START_TEST (mr_copy_arg3, "test that dump of mr_conf matches with dump of MR_COPY_RECURSIVELY (mr_conf_t, &mr_conf, &mr_conf_copy)") {
  mr_conf_t mr_conf_copy;
  int orig_eq_restored;
  mr_status_t status = MR_COPY_RECURSIVELY (mr_conf_t, &mr_conf, &mr_conf_copy);
  ck_assert_msg ((MR_SUCCESS == status), "MR_COPY_RECURSIVELY failed");
  orig_eq_restored = (0 == CMP_SERIALIAZED (mr_conf_t, &mr_conf, &mr_conf_copy));
  ck_assert_msg (orig_eq_restored, "dump of MR_COPY_RECURSIVELY mismatched dump of original mr_conf");
  MR_FREE_RECURSIVELY (mr_conf_t, &mr_conf_copy);
} END_TEST

MAIN ();
