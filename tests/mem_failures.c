#include <execinfo.h>
#include <stdbool.h>

#include <check.h>
#include <metaresc.h>
#include <mr_ic.h>
#include <regression.h>

static int malloc_cnt = 0;
static int realloc_cnt = 0;
static int free_cnt = 0;

static mr_ic_t malloc_seen;
static mr_ic_t realloc_seen;

static mr_mem_t _mem;
static mr_mem_t mr_mem;

TYPEDEF_STRUCT (stack_trace_t,
		(mr_ptr_t *, stack, , "type", { .offset = offsetof (stack_trace_t, size), }, "offset"),
		(ssize_t, size),
		(char *, type))

mr_hash_value_t
st_hash (mr_ptr_t x, const void * context)
{
  stack_trace_t * x_ = x.ptr;
  return (x_->size + (x_->stack ? x_->stack[0].long_int_t : 0));
}

int
st_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const stack_trace_t * x_ = x.ptr;
  const stack_trace_t * y_ = y.ptr;
  int diff = x_->size - y_->size;
  if (diff)
    return (diff);
  int i;
  for (i = 0; i < x_->size / sizeof (x_->stack[0]); ++i)
    {
      diff = x_->stack[i].ptr - y_->stack[i].ptr;
      if (diff)
	return (diff);
    }
  return (0);
}

static mr_status_t
st_free (mr_ptr_t x, const void * context)
{
  stack_trace_t * stack_trace = x.ptr;
  if (stack_trace->stack)
    free (stack_trace->stack);
  free (stack_trace);
  return (MR_SUCCESS);
}

static inline stack_trace_t * stack_trace_get ()
{
  stack_trace_t * stack_trace = malloc (sizeof (*stack_trace));
  if (NULL == stack_trace)
    return (NULL);
  stack_trace->type = "long_int_t";
  stack_trace->size = 8 * sizeof (stack_trace->stack[0]);
  stack_trace->stack = malloc (stack_trace->size);
  if (NULL == stack_trace->stack)
    goto fail;

  for (;;)
    {
      int alloc_count = stack_trace->size / sizeof (stack_trace->stack[0]);
      int count = backtrace ((void**)stack_trace->stack, alloc_count);
      if (count < alloc_count)
	{
	  stack_trace->size = count * sizeof (stack_trace->stack[0]);
	  return (stack_trace);
	}

      stack_trace->size <<= 1;
      typeof (stack_trace->stack) _stack = realloc (stack_trace->stack, stack_trace->size);
      if (NULL == _stack)
	{
	  free (stack_trace->stack);
	  break;
	}
      stack_trace->stack = _stack;
    }

 fail:
  free (stack_trace);
  return (NULL);
}

static inline bool st_is_seen (mr_ic_t * seen)
{
  mr_conf.mr_mem = mr_mem;
  stack_trace_t * stack_trace = stack_trace_get ();
  ck_assert_msg (NULL != stack_trace, "Failed to alloca memory");

  mr_ptr_t * add = mr_ic_add (seen, stack_trace);
  ck_assert_msg (NULL != add, "Failed to alloca memory");
  mr_conf.mr_mem = _mem;

  if (add->ptr == stack_trace)
    return (false);

  free (stack_trace->stack);
  free (stack_trace);
  return (true);
}

static void * _malloc (const char * filename, const char * function, int line, size_t size) 
{ 
  if (!st_is_seen (&malloc_seen))
    return (NULL);

  ++malloc_cnt;

  void * rv = mr_mem.malloc (filename, function, line, size);
  ck_assert_msg (NULL != rv, "Failed to alloca memory");
  return (rv);
}

static void * _realloc (const char * filename, const char * function, int line, void * ptr, size_t size) 
{
  if (NULL == ptr)
    return (_malloc (filename, function, line, size));

  if (!st_is_seen (&realloc_seen))
    return (NULL);

  ++realloc_cnt;

  void * rv = mr_mem.realloc (filename, function, line, ptr, size);
  ck_assert_msg (NULL != rv, "Failed to alloca memory");
  return (rv);
}

static void _free (const char * filename, const char * function, int line, void * ptr) 
{
  ++free_cnt;
  mr_mem.free (filename, function, line, ptr);
}

static void _mr_message (const char * file_name, const char * func_name, int line, mr_log_level_t log_level, mr_message_id_t message_id, va_list args) {}

static void mem_failures_method (mr_status_t (*method) ())
{
  mr_mem = mr_conf.mr_mem;
  _mem.mem_alloc_strategy = mr_mem.mem_alloc_strategy;
  _mem.malloc = _malloc;
  _mem.realloc = _realloc;
  _mem.free = _free;

  malloc_cnt = realloc_cnt = free_cnt = 0;

  mr_status_t status;

  status = mr_ic_new (&malloc_seen, st_hash, st_cmp, "stack_trace_t", MR_IC_RBTREE, NULL);
  ck_assert_msg (MR_SUCCESS == status, "failed to init indexed collection for malloc");
  status = mr_ic_new (&realloc_seen, st_hash, st_cmp, "stack_trace_t", MR_IC_RBTREE, NULL);
  ck_assert_msg (MR_SUCCESS == status, "failed to init indexed collection for realloc");

  mr_conf.msg_handler = _mr_message;
  mr_conf.mr_mem = _mem;

  while (MR_SUCCESS != method ());
    
  mr_conf.mr_mem = mr_mem;

  mr_ic_foreach (&malloc_seen, st_free, NULL);
  mr_ic_free (&malloc_seen);
  mr_ic_foreach (&realloc_seen, st_free, NULL);
  mr_ic_free (&realloc_seen);

  ck_assert_msg (malloc_cnt == free_cnt, "Mismatch of allocations (%d) and free (%d)", malloc_cnt, free_cnt);
}

static mr_status_t
mr_save_method ()
{
  mr_ra_ptrdes_t ptrs = MR_SAVE (mr_conf_t, &mr_conf);
  if (ptrs.ra)
    {
      MR_FREE (ptrs.ra);
      return (MR_SUCCESS);
    }
  return (MR_FAILURE);
}

MR_START_TEST (mem_failures_mr_save, "test memory operations failures for MR_SAVE") { mem_failures_method (mr_save_method); } END_TEST

#define TEST_MR_SAVE_METHOD(METHOD)					\
  static mr_status_t							\
  METHOD ## _method ()							\
  {									\
    mr_rarray_t ra = METHOD (mr_conf_t, &mr_conf);			\
    if (ra.data.ptr)							\
      {									\
	MR_FREE (ra.data.ptr);						\
	return (MR_SUCCESS);						\
      }									\
    return (MR_FAILURE);						\
  }									\
  MR_START_TEST (mem_failures_ ## METHOD,				\
		 "test memory operations failures for " #METHOD)	\
  { mem_failures_method (METHOD ## _method); } END_TEST

TEST_MR_SAVE_METHOD (MR_SAVE_SCM_RA)
TEST_MR_SAVE_METHOD (MR_SAVE_CINIT_RA)
TEST_MR_SAVE_METHOD (MR_SAVE_JSON_RA)
TEST_MR_SAVE_METHOD (MR_SAVE_XML2_RA)

MAIN ();
