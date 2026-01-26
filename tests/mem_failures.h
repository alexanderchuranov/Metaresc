#ifndef _MEM_FAILURES_H_

#include <metaresc.h>

#include <check.h>
#include <regression.h>

extern void tweak_types (void);
extern void mem_failures_method (mr_status_t (*method) (void * arg), void * arg, bool once_per_allocation);
extern mr_mem_t _mr_mem, mr_mem;

#undef MR_SAVE
#define MR_SAVE(MR_TYPE, PTR) ({					\
      mr_ptrdes_t * __ptrs = MR_SAVE_TYPED (MR_TYPE, PTR);		\
      mr_reorder_strings (__ptrs);					\
      __ptrs;								\
    })

#define TEST_MF_SAVE_METHOD(METHOD)					\
  static mr_status_t							\
  METHOD ## _save_method (void * arg)					\
  {									\
    mr_rarray_t ra = MR_SAVE_ ## METHOD ## _RA (mr_conf_t, &mr_conf);	\
    if (ra.data.ptr)							\
      {									\
	MR_FREE (ra.data.ptr);						\
	return (MR_SUCCESS);						\
      }									\
    return (MR_FAILURE);						\
  }									\
  START_TEST (mem_save_failures_one_per_allocation_ ## METHOD)		\
  { mem_failures_method (METHOD ## _save_method, NULL, true); } END_TEST \
  START_TEST (mem_save_failures_on_every_allocation_ ## METHOD)		\
  { mem_failures_method (METHOD ## _save_method, NULL, false); } END_TEST \
  MAIN_TEST_SUITE ((mem_save_failures_one_per_allocation_ ## METHOD,	\
		    "test failure of memory allocation (once per allocation point) for " #METHOD), \
		   (mem_save_failures_on_every_allocation_ ## METHOD,	\
		    "test failure of memory allocation (after N allocations) for " #METHOD) \
		   );

#define TEST_MF_LOAD_METHOD(METHOD)					\
  static mr_status_t							\
  METHOD ## _load_method (void * arg)					\
  {									\
    mr_conf_t _mr_conf;							\
    mr_status_t status;							\
    mr_rarray_t * ra = arg;						\
    memset (&_mr_conf, 0, sizeof (_mr_conf));				\
    status = MR_LOAD_## METHOD ## _RA (mr_conf_t, ra, &_mr_conf);	\
    mr_conf.mr_mem = mr_mem;						\
    mr_ptrdes_t * ptrs = MR_SAVE (mr_conf_t, &_mr_conf);		\
    mr_conf.mr_mem = _mr_mem;						\
    if (ptrs != NULL)							\
      {									\
	mr_free_recursively (ptrs);					\
	mr_mem.free (NULL, NULL, 0, ptrs);				\
      }									\
    return (status);							\
  }									\
  void mem_load_failures (bool once_per_allocation)			\
  {									\
    mr_conf_init ();							\
    tweak_types ();							\
    mr_rarray_t ra = MR_SAVE_ ## METHOD ## _RA (mr_conf_t, &mr_conf);	\
    ck_assert_msg (ra.data.ptr != NULL, "Failed to save mr_conf for tests of load methods"); \
    mem_failures_method (METHOD ## _load_method, &ra, once_per_allocation); \
    MR_FREE (ra.data.ptr);						\
  }									\
  START_TEST (mem_load_failures_one_per_allocation_ ## METHOD)		\
  { mem_load_failures (true); } END_TEST				\
  START_TEST (mem_load_failures_on_every_allocation_ ## METHOD)		\
  { mem_load_failures (false); } END_TEST				\
  MAIN_TEST_SUITE ((mem_load_failures_one_per_allocation_ ## METHOD, "test load memory operations failures for " #METHOD), \
		   (mem_load_failures_on_every_allocation_ ## METHOD, "test load memory operations failures for " #METHOD));

#endif /* _MEM_FAILURES_H_ */
