/* -*- C -*- */
/* This file is a part of Metaresc project */

/*
  Fuzzer for the XDR loader.

  The same source is built in two ways:
  - with libFuzzer (clang -fsanitize=fuzzer), see README.md
  - together with fuzz_replay_main.c, which feeds the files from the corpus
    directory to LLVMFuzzerTestOneInput. This runs in "make check" on every
    compiler and platform, so inputs found by the fuzzer stay as regression
    tests.

  The harness checks only that the loader does not crash and does not
  corrupt memory (run it with sanitizers). Two measures keep the fuzzer
  productive:
  - allocations above FUZZ_MAX_ALLOCATION are refused, otherwise a few
    mutated bytes in a length field make the loader request gigabytes and
    the fuzzer stops on out-of-memory before it reaches anything else
  - all blocks are tracked and whatever is left after an iteration is
    released. The loader does not free what it has allocated when a load
    fails, and the partially loaded object is not safe to free
    recursively. A successfully loaded object is freed recursively, which
    also checks that the loaded graph is consistent. Define
    FUZZ_LOADER_ONLY to skip that step and fuzz the loader alone.
*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <metaresc.h>
#include <fuzz_xdr_types.h>

#define FUZZ_MAX_ALLOCATION (1 << 20)

#ifdef HAVE_RPC_TYPES_H

#define FUZZ_MAX_BLOCKS (1 << 10)

static mr_mem_t fuzz_mem;
static void * blocks[FUZZ_MAX_BLOCKS];
static int blocks_count;

static void
forget_block (void * ptr)
{
  int i;
  for (i = 0; i < blocks_count; ++i)
    if (blocks[i] == ptr)
      {
        blocks[i] = blocks[--blocks_count];
        return;
      }
}

static void *
fuzz_calloc (const char * filename, const char * function, int line, size_t count, size_t size)
{
  if ((size != 0) && (count > FUZZ_MAX_ALLOCATION / size))
    return (NULL);
  if (blocks_count >= FUZZ_MAX_BLOCKS)
    return (NULL);
  void * ptr = fuzz_mem.calloc (filename, function, line, count, size);
  if (ptr != NULL)
    blocks[blocks_count++] = ptr;
  return (ptr);
}

static void *
fuzz_realloc (const char * filename, const char * function, int line, void * ptr, size_t size)
{
  if (size > FUZZ_MAX_ALLOCATION)
    return (NULL);
  if ((NULL == ptr) && (blocks_count >= FUZZ_MAX_BLOCKS))
    return (NULL);
  void * new_ptr = fuzz_mem.realloc (filename, function, line, ptr, size);
  if (new_ptr != NULL)
    {
      if (ptr != NULL)
        forget_block (ptr);
      blocks[blocks_count++] = new_ptr;
    }
  return (new_ptr);
}

static void
fuzz_free (const char * filename, const char * function, int line, void * ptr)
{
  forget_block (ptr);
  fuzz_mem.free (filename, function, line, ptr);
}

static void
fuzz_message (const char * file_name, const char * func_name, int line, mr_log_level_t log_level, mr_message_id_t message_id, va_list args)
{
}

int
LLVMFuzzerInitialize (int * argc, char *** argv)
{
  mr_conf_init ();
  fuzz_mem = mr_conf.mr_mem;
  mr_conf.mr_mem.calloc = fuzz_calloc;
  mr_conf.mr_mem.realloc = fuzz_realloc;
  mr_conf.mr_mem.free = fuzz_free;
  mr_conf.msg_handler = fuzz_message;
  return (0);
}

int
LLVMFuzzerTestOneInput (const uint8_t * data, size_t size)
{
  /* decoder only reads from the stream */
  mr_rarray_t ra = { .type = "uint8_t", };
  ra.data.ptr = (void*)data;
  ra.MR_SIZE = ra.alloc_size = size;

  /* heap allocated object, so AddressSanitizer has a red zone right after the last field */
  fuzz_t * dst = MR_CALLOC (1, sizeof (*dst));
  if (NULL == dst)
    return (0);

  mr_status_t status = MR_LOAD_XDR_RA (fuzz_t, &ra, dst);
#ifndef FUZZ_LOADER_ONLY
  /* an object that was loaded successfully must be safe to traverse */
  if (MR_SUCCESS == status)
    MR_FREE_RECURSIVELY (fuzz_t, dst);
#else /* FUZZ_LOADER_ONLY */
  (void)status;
#endif /* FUZZ_LOADER_ONLY */

  /* release the blocks of a failed load, and the object itself */
  while (blocks_count > 0)
    fuzz_mem.free (__FILE__, __FUNCTION__, __LINE__, blocks[--blocks_count]);
  return (0);
}

#else /* HAVE_RPC_TYPES_H */

int
LLVMFuzzerInitialize (int * argc, char *** argv)
{
  return (0);
}

int
LLVMFuzzerTestOneInput (const uint8_t * data, size_t size)
{
  return (0);
}

#endif /* HAVE_RPC_TYPES_H */
