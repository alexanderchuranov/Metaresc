/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

#include <stddef.h> /* size_t */
#include <string.h> /* memcpy */

#include <metaresc.h>
#include <mr_hsort.h>

/**
 * Sifts an element down a binary heap.
 * @param array heap
 * @param count index of the last element of the heap
 * @param size size of an element
 * @param compar_fn comparison function
 * @param context context for the comparison function
 * @param idx0 index of the element to sift
 */
static inline void
sift (char * array, size_t count, size_t size, mr_compar_fn_t compar_fn, void * context, unsigned idx0)
{
  unsigned idx1;
  char x[size];
  memcpy (x, &array[idx0 * size], size);

  for (idx1 = (idx0 << 1) + 1; idx1 < count; idx1 = (idx1 << 1) + 1)
    {
      if (compar_fn (&array[idx1 * size], &array[(idx1 + 1) * size], context) <= 0)
        ++idx1;
      if (compar_fn (&array[idx1 * size], x, context) <= 0)
        break;
      memcpy (&array[idx0 * size], &array[idx1 * size], size);
      idx0 = idx1;
    }
  if (idx1 == count)
    if (compar_fn (&array[idx1 * size], x, context) > 0)
      {
        memcpy (&array[idx0 * size], &array[idx1 * size], size);
        idx0 = idx1;
      }
  memcpy (&array[idx0 * size], x, size);
}

/**
 * Sorts an array with heap sort. Unlike qsort it passes a context to the
 * comparison function.
 * @param array array to sort
 * @param count number of elements
 * @param size size of an element
 * @param compar_fn comparison function
 * @param context context for the comparison function
 */
void
mr_hsort (void * array, size_t count, size_t size, mr_compar_fn_t compar_fn, void * context)
{
  unsigned i;

  if (count-- <= 1)
    return;

  for (i = count >> 1; i > 0; --i)
    sift (array, count, size, compar_fn, context, i);

  for ( ; count > 0; --count)
    {
      char x[size];
      void * last = &(((char*)array)[count * size]);
      sift (array, count, size, compar_fn, context, 0);
      memcpy (x, last, size);
      memcpy (last, array, size);
      memcpy (array, x, size);
    }
}
