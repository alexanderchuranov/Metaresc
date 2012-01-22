#include <string.h>
#include <stddef.h>
#include <check.h>
#include <reslib.h>

/*
  Test is doing the following:
  1. for all resizeable array in rl_conf set ra.alloc_size to ra.size. Required for further compare with another serialized rl_conf
  2. serialize rl_conf - library config structure
  3. load back config from serialized representation into another variable
  4. replace global variable with the main rl_conf on a loaded one
  5. serialize rl_conf once again
  6. match it with the first dump
  7. free up all allocated memory
*/

#define SMOKE_METHOD(METHOD, ...) START_TEST (rl_conf_save_load) {	\
    rl_conf_t rl_conf_saved = rl_conf;					\
    rl_ra_rl_ptrdes_t ptrs = RL_SAVE_RL (rl_conf_t, &rl_conf);		\
    int i;								\
    if ((0 == ptrs.ra.size) || (NULL == ptrs.ra.data))			\
      ck_abort_msg ("save into internal representation failed");	\
    for (i = ptrs.ra.size / sizeof (ptrs.ra.data[0]) - 1; i >= 0; --i)	\
      if (RL_TYPE_EXT_RARRAY_DATA == ptrs.ra.data[i].fd.rl_type_ext)	\
	{								\
	  rl_rarray_t * ra = (rl_rarray_t*)&((char*)ptrs.ra.data[i].data)[-offsetof (rl_rarray_t, data)]; \
	  ra->alloc_size = ra->size;					\
	}								\
    RL_FREE (ptrs.ra.data);						\
    rl_rarray_t rl_conf_serialized = RL_SAVE_ ## METHOD ## _RA (rl_conf_t, &rl_conf); \
    if ((0 == rl_conf_serialized.size) || (NULL == rl_conf_serialized.data)) \
      ck_abort_msg ("save for method " #METHOD " failed");		\
    rl_conf_t rl_conf_loaded = rl_conf;					\
    if (0 == RL_LOAD_ ## METHOD ## _RA (rl_conf_t, &rl_conf_serialized, &rl_conf_loaded)) \
      ck_abort_msg ("load for method " #METHOD " failed");		\
    rl_conf = rl_conf_loaded;						\
    rl_rarray_t rl_conf_serialized_ = RL_SAVE_ ## METHOD ## _RA (rl_conf_t, &rl_conf); \
    if ((rl_conf_serialized.size != rl_conf_serialized_.size) ||	\
	(0 != memcmp (rl_conf_serialized.data, rl_conf_serialized_.data, rl_conf_serialized.size))) \
      ck_abort_msg ("restored rl_conf mismatched original dump for method " #METHOD); \
    RL_FREE (rl_conf_serialized_.data);					\
    RL_FREE (rl_conf_serialized.data);					\
    rl_conf = rl_conf_saved;						\
    RL_FREE_RECURSIVELY (rl_conf_t, &rl_conf_loaded);			\
  } END_TEST								\
  TYPEDEF_STRUCT (rl_empty_t);						\
  int main (int argc, char * argv[])					\
  {									\
    int number_failed;							\
    Suite * suite = suite_create ("main");				\
    if (NULL == suite)							\
      return (EXIT_FAILURE);						\
    SRunner * srunner = srunner_create (suite);				\
    if (NULL == srunner)						\
      return (EXIT_FAILURE);						\
    TCase * tcase = tcase_create ("rl_conf save/load");			\
    if (NULL == tcase)							\
      return (EXIT_FAILURE);						\
    tcase_set_timeout (tcase, 0);					\
    tcase_add_test (tcase, rl_conf_save_load);				\
    suite_add_tcase (suite, tcase);					\
    srunner_run_all (srunner, CK_ENV);					\
    number_failed = srunner_ntests_failed (srunner);			\
    srunner_free (srunner);						\
    return ((number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE);	\
  }
