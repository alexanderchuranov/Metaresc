#include <check.h>
#include <metaresc.h>
#include <regression.h>

MR_START_TEST (mr_copy_args2, "test that dump of mr_conf matches with dump of MR_DEEP_COPY (mr_conf_t, &mr_conf)") {
  mr_conf_t mr_conf_copy = MR_DEEP_COPY (mr_conf_t, &mr_conf);
  int orig_eq_restored = (0 == CMP_SERIALIAZED (mr_conf_t, &mr_conf, &mr_conf_copy));
  ck_assert_msg (orig_eq_restored, "dump of MR_DEEP_COPY mismatched dump of original mr_conf");
  MR_FREE_RECURSIVELY (mr_conf_t, &mr_conf_copy);
} END_TEST

MR_START_TEST (mr_copy_arg3, "test that dump of mr_conf matches with dump of MR_DEEP_COPY (mr_conf_t, &mr_conf, copy)") {
  mr_conf_t * copy = MR_MALLOC (sizeof (*copy));
  int orig_eq_restored;
  int status = MR_DEEP_COPY (mr_conf_t, &mr_conf, copy);
  ck_assert_msg (status, "MR_DEEP_COPY failed");
  orig_eq_restored = (0 == CMP_SERIALIAZED (mr_conf_t, &mr_conf, copy));
  ck_assert_msg (orig_eq_restored, "dump of MR_DEEP_COPY mismatched dump of original mr_conf");
  MR_FREE_RECURSIVELY (mr_conf_t, copy);
  MR_FREE (copy);
} END_TEST

MAIN ();
