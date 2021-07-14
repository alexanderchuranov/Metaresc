#include <execinfo.h>
#include <stdbool.h>

#include <check.h>
#include <metaresc.h>
#include <mr_ic.h>
#include <regression.h>
#include <mem_failures.h>

#define TRACK_ALLOCATED_BLOCKS

static int malloc_cnt = 0;
static int realloc_cnt = 0;
static int free_cnt = 0;
static int successful_allocs = -1; 

static mr_ic_t alloc_seen;
static mr_ic_t alloc_blocks;

mr_mem_t _mr_mem, mr_mem;

TYPEDEF_STRUCT (stack_trace_t,
		(mr_ptr_t, ptr, , "type"),
		(const char *, filename),
		(const char *, function),
		(int, line),
		(mr_ptr_t *, stack, , "type", { .offset = offsetof (stack_trace_t, size), }, "offset"),
		(ssize_t, size),
		(char *, type))

mr_hash_value_t
ab_hash (mr_ptr_t x, const void * context)
{
  stack_trace_t * x_ = x.ptr;
  return (x_->ptr.uintptr);
}

int
ab_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  stack_trace_t * x_ = x.ptr;
  stack_trace_t * y_ = y.ptr;
  return ((x_->ptr.ptr > y_->ptr.ptr) - (x_->ptr.ptr < y_->ptr.ptr));
}

mr_hash_value_t
st_hash (mr_ptr_t x, const void * context)
{
  stack_trace_t * x_ = x.ptr;
  return (x_->size + (x_->stack ? x_->stack[0].uintptr : 0));
}

int
st_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const stack_trace_t * x_ = x.ptr;
  const stack_trace_t * y_ = y.ptr;
  int diff = x_->size - y_->size;
  if (diff)
    return (diff);
  int i, count = x_->size / sizeof (x_->stack[0]);
  for (i = 0; i < count; ++i)
    {
      diff = (x_->stack[i].ptr > y_->stack[i].ptr) - (x_->stack[i].ptr < y_->stack[i].ptr);
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

static void
st_print (stack_trace_t * stack_trace)
{
  int size = stack_trace->size / sizeof (stack_trace->stack[0]);
  char ** strings = backtrace_symbols ((void**)stack_trace->stack, size);
  fprintf (stderr, "Pointer %p from file %s function %s line %d:\n", 
	   stack_trace->ptr.ptr, stack_trace->filename, stack_trace->function, stack_trace->line);
  if (strings)
    {
      int i;
      for (i = 0; i < size; ++i)
	fprintf (stderr, "%s\n", strings[i]);
      free (strings);
    }
  fflush (stderr);
}

mr_status_t
print_block (mr_ptr_t x, const void * context)
{
  fprintf (stderr, "Lost pointer:\n");
  st_print (x.ptr);
  return (MR_SUCCESS);
}

mr_hash_value_t
stack_entry_hash (mr_ptr_t x, const void * context)
{
  const stack_trace_t * stack_trace = context;
  return (stack_trace->stack[x.uintptr].uintptr / sizeof (void*));
}

int
stack_entry_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const stack_trace_t * stack_trace = context;
  return ((stack_trace->stack[x.uintptr].ptr > stack_trace->stack[y.uintptr].ptr) -
	  (stack_trace->stack[x.uintptr].ptr < stack_trace->stack[y.uintptr].ptr));
}

static inline void
normalize_recursion (stack_trace_t * stack_trace)
{
  intptr_t i;
  int count = stack_trace->size / sizeof (stack_trace->stack[0]);
  mr_ic_t last_call;
  mr_res_t res = {
    .data = { stack_trace },
    .type = "stack_trace_t",
    .mr_size = sizeof (stack_trace_t),
  };
  mr_status_t status = mr_ic_new (&last_call, stack_entry_hash, stack_entry_cmp, "intptr_t", MR_IC_HASH, &res);
  bool has_duplicates = false;
  
  if (MR_SUCCESS != status)
    return;
  
  for (i = count - 1; i >= 0; --i)
    {
      mr_ptr_t * add = mr_ic_add (&last_call, i);
      if (NULL == add)
	goto free_index;
      has_duplicates |= (add->uintptr != i);
    }

  if (has_duplicates)
    {
      int dst = 0;
      for (i = 0; i < count; ++i)
	{
	  mr_ptr_t * find = mr_ic_find (&last_call, i);
	  if (NULL == find)
	    goto free_index;
	  i = find->uintptr;
	  stack_trace->stack[dst++] = stack_trace->stack[i];
	}

      stack_trace->size = dst * sizeof (stack_trace->stack[0]);
    }
 free_index:
  mr_ic_free (&last_call);
}

static inline stack_trace_t * 
stack_trace_get ()
{
  stack_trace_t * stack_trace = malloc (sizeof (*stack_trace));
  if (NULL == stack_trace)
    return (NULL);
  memset (stack_trace, 0, sizeof (*stack_trace));
  stack_trace->type = "uintptr";
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
	  normalize_recursion (stack_trace);
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

static inline bool alloc_fail ()
{
  if (0 == successful_allocs)
    return (true);
  if (successful_allocs > 0)
    {
      --successful_allocs;
      return (false);
    }
  
  mr_conf.mr_mem = mr_mem;
  stack_trace_t * stack_trace = stack_trace_get ();
  ck_assert_msg (NULL != stack_trace, "Failed to alloca memory");

  mr_ptr_t * add = mr_ic_add (&alloc_seen, stack_trace);
  ck_assert_msg (NULL != add, "Failed to alloca memory");
  mr_conf.mr_mem = _mr_mem;

  if (add->ptr == stack_trace)
    {
#ifdef DEBUG
      fprintf (stderr, "Fire error from:\n");
      st_print (stack_trace);
#endif /* DEBUG */
      return (true);
    }

  free (stack_trace->stack);
  free (stack_trace);
  return (false);
}

static void * _calloc (const char * filename, const char * function, int line, size_t count, size_t size) 
{ 
  if (alloc_fail ())
    {
#ifdef DEBUG
      fprintf (stderr, "Fire error from: %s %s %d\n", filename, function, line);
#endif /* DEBUG */
      return (NULL);
    }

  ++malloc_cnt;

  void * rv = mr_mem.calloc (filename, function, line, count, size);
  ck_assert_msg (NULL != rv, "Failed to alloca memory");

  mr_conf.mr_mem = mr_mem;
  stack_trace_t * stack_trace = stack_trace_get ();
  ck_assert_msg (NULL != stack_trace, "Failed to alloca memory");
  stack_trace->ptr.ptr = rv;
  stack_trace->filename = filename;
  stack_trace->function = function;
  stack_trace->line = line;
  
  mr_ic_add (&alloc_blocks, stack_trace);
  mr_conf.mr_mem = _mr_mem;

  return (rv);
}

static void * _realloc (const char * filename, const char * function, int line, void * ptr, size_t size) 
{
  if (NULL == ptr)
    return (_calloc (filename, function, line, 1, size));

  if (alloc_fail ())
    {
#ifdef DEBUG
      fprintf (stderr, "Fire error from: %s %s %d\n", filename, function, line);
#endif /* DEBUG */
      return (NULL);
    }

  mr_conf.mr_mem = mr_mem;
  stack_trace_t stack_trace_del;

  stack_trace_del.ptr.ptr = ptr;
  stack_trace_t * stack_trace_ = NULL;
  mr_ptr_t * find = mr_ic_find (&alloc_blocks, &stack_trace_del);
  if (find != NULL)
    stack_trace_ = find->ptr;
  mr_status_t status = mr_ic_del (&alloc_blocks, &stack_trace_del);

  if (stack_trace_ != NULL)
    {
      if (stack_trace_->stack) 
	MR_FREE (stack_trace_->stack);
      MR_FREE (stack_trace_);

    }
  mr_conf.mr_mem = _mr_mem;

  if (MR_SUCCESS != status)
    {
      fprintf (stderr, "ptr %p realloc error from %s func %s line %d\n", ptr, filename, function, line);
      return (NULL);
    }

  ++realloc_cnt;

  void * rv = mr_mem.realloc (filename, function, line, ptr, size);
  ck_assert_msg (NULL != rv, "Failed to alloca memory");

  mr_conf.mr_mem = mr_mem;
  stack_trace_t * stack_trace_add = stack_trace_get ();
  ck_assert_msg (NULL != stack_trace_add, "Failed to alloca memory");
  stack_trace_add->ptr.ptr = rv;
  stack_trace_add->filename = filename;
  stack_trace_add->function = function;
  stack_trace_add->line = line;

  mr_ic_add (&alloc_blocks, stack_trace_add);
  mr_conf.mr_mem = _mr_mem;

  return (rv);
}

static void _free (const char * filename, const char * function, int line, void * ptr) 
{
  mr_conf.mr_mem = mr_mem;
  stack_trace_t stack_trace;
  stack_trace.ptr.ptr = ptr;

  stack_trace_t * stack_trace_ = NULL;
  mr_ptr_t * find = mr_ic_find (&alloc_blocks, &stack_trace);
  if (find != NULL)
    stack_trace_ = find->ptr;

  mr_status_t status = mr_ic_del (&alloc_blocks, &stack_trace);

  if (stack_trace_ != NULL)
    {
      if (stack_trace_->stack) 
	MR_FREE (stack_trace_->stack);
      MR_FREE (stack_trace_);
    }
  mr_conf.mr_mem = _mr_mem;

  if (MR_SUCCESS != status)
    {
      fprintf (stderr, "ptr %p free error from %s func %s line %d\n", ptr, filename, function, line);
      return;
    }

  ++free_cnt;
  mr_mem.free (filename, function, line, ptr);
}

static void _mr_message (const char * file_name, const char * func_name, int line, mr_log_level_t log_level, mr_message_id_t message_id, va_list args) {}

void 
mem_failures_method (mr_status_t (*method) (void * arg), void * arg, bool once_per_allocation)
{
  mr_status_t status;
  int i;

  mr_detect_type (NULL); /* explicitly init library */

  mr_conf.cache_func_resolve = false;
  
  mr_mem = mr_conf.mr_mem;
  _mr_mem.calloc = _calloc;
  _mr_mem.realloc = _realloc;
  _mr_mem.free = _free;
  
#define IC_TYPE MR_IC_HASH
  
  status = mr_ic_new (&alloc_blocks, ab_hash, ab_cmp, "stack_trace_t", IC_TYPE, NULL);
  ck_assert_msg (MR_SUCCESS == status, "failed to init indexed collection for allocated blocks");

  status = mr_ic_new (&alloc_seen, st_hash, st_cmp, "stack_trace_t", IC_TYPE, NULL);
  ck_assert_msg (MR_SUCCESS == status, "failed to init indexed collection for malloc");

  mr_conf.msg_handler = _mr_message;
  mr_conf.mr_mem = _mr_mem;

  for (i = 0; ; ++i)
    {
      malloc_cnt = realloc_cnt = free_cnt = 0;
      successful_allocs = once_per_allocation ? -1 : i;
      status = method (arg);

#ifdef DEBUG
      fprintf (stderr, "#%d (%d) ~ (%d)\n", i, malloc_cnt, free_cnt);
#endif /* DEBUG */

      if (malloc_cnt != free_cnt)
	break;
      if (MR_SUCCESS == status)
	break;
    }
    
  mr_conf.mr_mem = mr_mem;
  mr_conf.msg_handler = NULL;

  mr_ic_foreach (&alloc_seen, st_free, NULL);
  mr_ic_free (&alloc_seen);

  mr_ic_foreach (&alloc_blocks, print_block, NULL);
  mr_ic_foreach (&alloc_blocks, st_free, NULL);
  mr_ic_free (&alloc_blocks);

  ck_assert_msg (malloc_cnt == free_cnt, "Mismatch of allocations (%d) and free (%d) on round %d", malloc_cnt, free_cnt, i);
}
