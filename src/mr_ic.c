/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

#include <string.h>

#include <metaresc.h>
#include <mr_hsort.h>
#include <mr_btree.h>
#include <mr_ic.h>

#undef MR_SAVE
#define MR_SAVE(TDP, S_PTR) ({				\
      void * __ptr__ = (void*)S_PTR;			\
      mr_fd_t __fd__;					\
      memset (&__fd__, 0, sizeof (__fd__));		\
      __fd__.non_persistent = true;			\
      __fd__.stype.tdp = TDP;				\
      __fd__.name = __fd__.stype.tdp->type;		\
      __fd__.stype.type = __fd__.stype.tdp->type.str;	\
      __fd__.stype.size = __fd__.stype.tdp->size;	\
      __fd__.stype.mr_type = __fd__.stype.tdp->mr_type;	\
      mr_save (__ptr__, &__fd__);			\
    })

/**
 * Hash function for keys of a registered type, computed from the saved graph
 * of the key.
 * @param x key
 * @param context type descriptor of the key
 * @return hash value
 */
mr_hash_value_t
mr_generic_hash (const mr_ptr_t x, const void * context)
{
  mr_td_t * tdp = (mr_td_t*)context;
  return (MR_HASH_STRUCT (tdp, x.ptr));
}

/**
 * Compares keys of a registered type by their saved graphs.
 * @param x key
 * @param y another key
 * @param context type descriptor of the key
 * @return comparation sign
 */
int
mr_generic_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  mr_td_t * tdp = (mr_td_t*)context;
  return (MR_CMP_STRUCTS (tdp, x.ptr, y.ptr));
}

/* ----------------------- MR_IC_UNSORTED_ARRAY ----------------------- */

/**
 * Adds a key to an unsorted array. The unsorted array does not own its
 * memory, so it is converted into a sorted array first.
 * @param ic indexed collection
 * @param key key to add
 * @return pointer on the added or already present key, NULL on error
 */
mr_ptr_t *
mr_ic_unsorted_array_add (mr_ic_t * ic, mr_ptr_t key)
{
  mr_ptr_t * find = mr_ic_find (ic, key);
  if (find != NULL)
    return (find);

  mr_ic_t dst_ic;
  mr_status_t status = mr_ic_sorted_array_new (&dst_ic, ic->compar_fn, ic->key_type, &ic->context);
  if (MR_SUCCESS != status)
    return (NULL);
      
  status = mr_ic_index (&dst_ic, ic->rarray.ra, ic->rarray.size);
  if (MR_SUCCESS != status)
    return (NULL);
      
  *ic = dst_ic;
  return (mr_ic_add (ic, key));
}

/**
 * Deletes a key from an unsorted array. The array is converted into a
 * sorted array first.
 * @param ic indexed collection
 * @param key key to delete
 * @return status, failure if the key is not found
 */
mr_status_t
mr_ic_unsorted_array_del (mr_ic_t * ic, mr_ptr_t key)
{
  mr_ptr_t * find = mr_ic_find (ic, key);
  if (NULL == find)
    return (MR_FAILURE);
  
  mr_ic_t dst_ic;
  mr_status_t status = mr_ic_sorted_array_new (&dst_ic, ic->compar_fn, ic->key_type, &ic->context);
  if (MR_SUCCESS != status)
    return (status);
      
  status = mr_ic_index (&dst_ic, ic->rarray.ra, ic->rarray.size);
  if (MR_SUCCESS != status)
    return (status);
      
  *ic = dst_ic;
  return (mr_ic_del (ic, key));
}

/**
 * Finds a key in an unsorted array with a linear search.
 * @param ic indexed collection
 * @param key key to look for
 * @return pointer on the found key or NULL
 */
mr_ptr_t *
mr_ic_unsorted_array_find (mr_ic_t * ic, mr_ptr_t key)
{
  unsigned i, count = ic->rarray.size / sizeof (ic->rarray.ra[0]);
  for (i = 0; i < count; ++i)
    if (0 == ic->compar_fn (key, ic->rarray.ra[i], ic->context.data.ptr))
      return (&ic->rarray.ra[i]);
  return (NULL);
}

/**
 * Calls a function for each key of an array in the stored order.
 * @param ic indexed collection
 * @param visit_fn function to call
 * @param context context for the function
 * @return status, failure if a call failed
 */
mr_status_t
mr_ic_unsorted_array_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, void * context)
{
  unsigned i, count = ic->rarray.size / sizeof (ic->rarray.ra[0]);
  for (i = 0; i < count; ++i)
    if (MR_SUCCESS != visit_fn (ic->rarray.ra[i], context))
      return (MR_FAILURE);
  return (MR_SUCCESS);
}

/**
 * Makes an unsorted array over an external array of keys. The keys are not
 * copied, the collection refers on the given array.
 * @param ic indexed collection
 * @param rarray array of keys
 * @param size size of the array in bytes
 * @return MR_SUCCESS
 */
mr_status_t
mr_ic_unsorted_array_index (mr_ic_t * ic, mr_ptr_t * rarray, size_t size)
{
  ic->items_count = size / sizeof (rarray[0]);
  ic->rarray.ra = rarray;
  ic->rarray.size = ic->items_count * sizeof (ic->rarray.ra[0]);
  ic->rarray.alloc_size = -1;
  return (MR_SUCCESS);
}

/**
 * Detaches an unsorted array from its keys. The keys are not freed.
 * @param ic indexed collection
 */
void
mr_ic_unsorted_array_free (mr_ic_t * ic)
{
  ic->rarray.ra = NULL;
  ic->rarray.size = ic->rarray.alloc_size = 0;
  ic->items_count = 0;
}

/**
 * Initializes an unsorted array collection. Without a comparison function
 * and context the generic comparison of the key type is used.
 * @param ic indexed collection
 * @param compar_fn comparison function
 * @param key_type type name of the keys
 * @param context context for the comparison function, may be NULL
 * @return status
 */
mr_status_t
mr_ic_unsorted_array_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type, mr_res_t * context)
{
  static mr_ic_virt_func_t virt_func = {
    .add = mr_ic_unsorted_array_add,
    .del = mr_ic_unsorted_array_del,
    .find = mr_ic_unsorted_array_find,
    .foreach = mr_ic_unsorted_array_foreach,
    .index = mr_ic_unsorted_array_index,
    .free = mr_ic_unsorted_array_free,
  };
  mr_res_t generic_context = {
    .type = "mr_td_t",
    .mr_size = sizeof (mr_td_t),
  };
  
  if (NULL == ic)
    return (MR_FAILURE);

  if ((compar_fn == NULL) && (context == NULL))
    {
      mr_td_t * tdp = mr_get_td_by_name_internal (key_type);
      if (tdp)
        {
          generic_context.data.ptr = tdp;
          compar_fn = mr_generic_cmp;
          context = &generic_context;
        }
    }

  if (NULL == compar_fn)
    return (MR_FAILURE);
  
  if (context)
    ic->context = *context;
  else
    memset (&ic->context, 0, sizeof (ic->context));
  
  ic->ic_type = MR_IC_UNSORTED_ARRAY;
  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->virt_func = &virt_func;
  ic->items_count = 0;
  
  ic->rarray.ra = NULL;
  ic->rarray.size = ic->rarray.alloc_size = 0;

  return (MR_SUCCESS);
}

/* ----------------------- MR_IC_SORTED_ARRAY ----------------------- */

/**
 * Binary search in a sorted array.
 * @param key key to look for
 * @param rarray sorted array
 * @param compar_fn comparison function
 * @param context context for the comparison function
 * @param idx receives the index of the found key or the insert position
 * @return 0 if the key is found, comparation sign of the last comparison otherwise
 */
int
mr_ic_sorted_array_find_idx (mr_ptr_t key, mr_ic_rarray_t * rarray, mr_compar_fn_t compar_fn, void * context, unsigned * idx)
{
  unsigned down = 0;
  unsigned up = rarray->size / sizeof (rarray->ra[0]);
  int diff = !0;

  while ((up != down) && (diff != 0))
    {
      unsigned mid = (down + up) >> 1;
      diff = compar_fn (key, rarray->ra[mid], context);
      if (diff <= 0)
        up = mid;
      else
        down = mid + 1;
    }
  *idx = up;
  return (diff);
}

/**
 * Adds a key to a sorted array at its sorted position.
 * @param ic indexed collection
 * @param key key to add
 * @return pointer on the added or already present key, NULL on error
 */
mr_ptr_t *
mr_ic_sorted_array_add (mr_ic_t * ic, mr_ptr_t key)
{
  unsigned idx;
  if (mr_ic_sorted_array_find_idx (key, &ic->rarray, ic->compar_fn, ic->context.data.ptr, &idx) == 0)
    return (&ic->rarray.ra[idx]);

  mr_ptr_t * add = mr_rarray_allocate_element ((void*)&ic->rarray.ra, &ic->rarray.size, &ic->rarray.alloc_size, sizeof (ic->rarray.ra[0]));
  if (NULL == add)
    return (NULL);

  memmove (&ic->rarray.ra[idx + 1], &ic->rarray.ra[idx], ic->rarray.size - (idx + 1) * sizeof (ic->rarray.ra[0]));
  ic->rarray.ra[idx] = key;
  ic->items_count = ic->rarray.size / sizeof (ic->rarray.ra[0]);

  return (&ic->rarray.ra[idx]);
}

/**
 * Deletes a key from a sorted array.
 * @param ic indexed collection
 * @param key key to delete
 * @return status, failure if the key is not found
 */
mr_status_t
mr_ic_sorted_array_del (mr_ic_t * ic, mr_ptr_t key)
{
  unsigned idx;
  if (mr_ic_sorted_array_find_idx (key, &ic->rarray, ic->compar_fn, ic->context.data.ptr, &idx) != 0)
    return (MR_FAILURE);
   
  ic->rarray.size -= sizeof (ic->rarray.ra[0]);
  ic->items_count = ic->rarray.size / sizeof (ic->rarray.ra[0]);
  memmove (&ic->rarray.ra[idx], &ic->rarray.ra[idx + 1], (ic->items_count - idx) * sizeof (ic->rarray.ra[0]));
  return (MR_SUCCESS);
}

/**
 * Finds a key in a sorted array with a binary search.
 * @param ic indexed collection
 * @param key key to look for
 * @return pointer on the found key or NULL
 */
mr_ptr_t *
mr_ic_sorted_array_find (mr_ic_t * ic, mr_ptr_t key)
{
  unsigned idx;
  return ((mr_ic_sorted_array_find_idx (key, &ic->rarray, ic->compar_fn, ic->context.data.ptr, &idx) == 0) ?
          &ic->rarray.ra[idx] : NULL);
}

TYPEDEF_STRUCT (mr_sort_context_t,
                (mr_ptr_t *, rarray, , "input array for indexing"),
                (mr_compar_fn_t, compar_fn, , "sorting function"),
                (typeof (((mr_ic_t*)0)->context.data.ptr), context_data_ptr, , "context pointer for sorting function"),
                );

/**
 * Compares sort entries by key and then by the index in the input array,
 * which makes the heap sort stable.
 * @param x sort entry
 * @param y another sort entry
 * @param context sort context with the keys and the comparison function
 * @return comparation sign
 */
static int
mr_sort_key_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const mr_sort_context_t * mr_sort_context = context;
  const mr_ptr_t * _x = x.ptr;
  const mr_ptr_t * _y = y.ptr;
  /* First compare values */
  int diff = mr_sort_context->compar_fn (mr_sort_context->rarray[_x->uintptr],
                                         mr_sort_context->rarray[_y->uintptr],
                                         mr_sort_context->context_data_ptr);
  if (diff)
    return (diff);
  /* If values are equal sort by indexes in input array */
  return ((_x->uintptr > _y->uintptr) - (_x->uintptr < _y->uintptr));
}

/**
 * Builds a sorted array from an array of keys. The keys are copied and
 * sorted, duplicates are dropped, the first one is kept.
 * @param ic indexed collection
 * @param rarray array of keys
 * @param size size of the array in bytes
 * @return status
 */
mr_status_t
mr_ic_sorted_array_index (mr_ic_t * ic, mr_ptr_t * rarray, size_t size)
{
  ic->items_count = 0;
  ic->rarray.size = 0;
  unsigned items_count = size / sizeof (rarray[0]);
  if (items_count > 0)
    {
      mr_ptr_t * add = mr_rarray_allocate_element ((void*)&ic->rarray.ra, &ic->rarray.size, &ic->rarray.alloc_size, items_count * sizeof (ic->rarray.ra[0]));
      if (NULL == add)
        return (MR_FAILURE);

      /*
        Heap sort is not a stable sort. As a result elements with equal keys might come out in an arbitrary order.
        To achieve stable sorting here we will sort array of indexes in original array. After that will make deduplication
        and finally replace indexes on values from the input array.
      */
      unsigned i;
      for (i = 0; i < items_count; ++i)
        ic->rarray.ra[i].uintptr = i;
      mr_sort_context_t mr_sort_context = {
        .rarray = rarray,
        .compar_fn = ic->compar_fn,
        .context_data_ptr = ic->context.data.ptr,
      };

      mr_hsort (ic->rarray.ra, items_count, sizeof (ic->rarray.ra[0]), mr_sort_key_cmp, &mr_sort_context);

      /* Deduplicate equal elements and replace indexes on values from the input array */
      unsigned src, dst = 0;
      mr_ptr_t prev = rarray[ic->rarray.ra[0].uintptr];
      ic->rarray.ra[dst++] = prev;
      for (src = 1; src < items_count; ++src)
        {
          mr_ptr_t next = rarray[ic->rarray.ra[src].uintptr];
          if (ic->compar_fn (next, prev, ic->context.data.ptr) != 0)
            ic->rarray.ra[dst++] = next;
          prev = next;
        }
      
      ic->items_count = dst; /* after deduplication actual number of elements might be lower */
      ic->rarray.size = ic->items_count * sizeof (ic->rarray.ra[0]); /* used array size */
    }
  return (MR_SUCCESS);
}

/**
 * Frees the memory of a sorted array.
 * @param ic indexed collection
 */
void
mr_ic_sorted_array_free (mr_ic_t * ic)
{
  if (NULL != ic->rarray.ra)
    MR_FREE (ic->rarray.ra);
  ic->rarray.ra = NULL;
  ic->rarray.size = ic->rarray.alloc_size = 0;
  ic->items_count = 0;
}

/**
 * Initializes a sorted array collection. Without a comparison function
 * and context the generic comparison of the key type is used.
 * @param ic indexed collection
 * @param compar_fn comparison function
 * @param key_type type name of the keys
 * @param context context for the comparison function, may be NULL
 * @return status
 */
mr_status_t
mr_ic_sorted_array_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type, mr_res_t * context)
{
  static mr_ic_virt_func_t virt_func = {
    .add = mr_ic_sorted_array_add,
    .del = mr_ic_sorted_array_del,
    .find = mr_ic_sorted_array_find,
    .foreach = mr_ic_unsorted_array_foreach,
    .index = mr_ic_sorted_array_index,
    .free = mr_ic_sorted_array_free,
  };
  mr_res_t generic_context = {
    .type = "mr_td_t",
    .mr_size = sizeof (mr_td_t),
  };
  
  if (NULL == ic)
    return (MR_FAILURE);

  if ((compar_fn == NULL) && (context == NULL))
    {
      mr_td_t * tdp = mr_get_td_by_name_internal (key_type);
      if (tdp)
        {
          generic_context.data.ptr = tdp;
          compar_fn = mr_generic_cmp;
          context = &generic_context;
        }
    }
  
  if (NULL == compar_fn)
    return (MR_FAILURE);

  if (context)
    ic->context = *context;
  else
    memset (&ic->context, 0, sizeof (ic->context));

  ic->ic_type = MR_IC_SORTED_ARRAY;
  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->virt_func = &virt_func;
  ic->items_count = 0;

  ic->rarray.ra = NULL;
  ic->rarray.size = ic->rarray.alloc_size = 0;
  
  return (MR_SUCCESS);
}

/* ----------------------- MR_IC_HASH ----------------------- */

/**
 * Frees the memory of a hash table.
 * @param ic indexed collection
 */
void
mr_ic_hash_free (mr_ic_t * ic)
{
  if (NULL != ic->hash.hash_table)
    MR_FREE (ic->hash.hash_table);
  ic->hash.hash_table = NULL;
  ic->hash.count = 0;
  ic->hash.zero_key = false;
  ic->items_count = 0;
}

/**
 * Adds a key to a hash table with open addressing and linear probing. The
 * table must have a free slot. The zero key is kept in the last slot.
 * @param ic indexed collection
 * @param key key to add
 * @return pointer on the added or already present key, NULL if the table is full
 */
static mr_ptr_t *
mr_ic_hash_index_add (mr_ic_t * ic, mr_ptr_t key)
{
  if (ic->hash.count >= 2)
    {
      unsigned count = ic->hash.count - 1;
      
      if (0 == key.intptr)
        {
          ++ic->items_count;
          ic->hash.zero_key = true;
          ic->hash.hash_table[count].intptr = 0;
          return (&ic->hash.hash_table[count]);
        }

      unsigned i, bucket = ic->hash.hash_fn (key, ic->context.data.ptr) % count;

      for (i = bucket; ;)
        {
          if (0 == ic->hash.hash_table[i].intptr)
            {
              ++ic->items_count;
              ic->hash.hash_table[i] = key;
              return (&ic->hash.hash_table[i]);
            }
          if (0 == ic->compar_fn (key, ic->hash.hash_table[i], ic->context.data.ptr))
            return (&ic->hash.hash_table[i]);
          if (++i >= count)
            i = 0;
          if (i == bucket)
            break;
        }
    }
  MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_HASH_TABLE_ERROR);
  return (NULL);
}

/**
 * Visitor that adds a key into another hash table.
 * @param key key to add
 * @param context destination hash table
 * @return status
 */
static mr_status_t
mr_ic_hash_index_visitor (mr_ptr_t key, void * context)
{
  mr_ic_t * dst_ic = context;
  mr_ptr_t * add = mr_ic_hash_index_add (dst_ic, key);
  return ((NULL == add) ? MR_FAILURE : MR_SUCCESS);
}

/**
 * Rebuilds a hash table from the keys of another collection. The table is
 * sized to four times the number of keys.
 * @param dst_ic destination hash table
 * @param src_ic source collection
 * @return status
 */
static mr_status_t
mr_ic_hash_reindex (mr_ic_t * dst_ic, mr_ic_t * src_ic)
{
  dst_ic->hash.zero_key = false;
  dst_ic->items_count = 0;

  if (0 != src_ic->items_count)
    {
      unsigned count = (src_ic->items_count << 2) + 6; /* 6 is a magic constant found in a manual performance test */

      if (dst_ic->hash.count < count)
        {
          dst_ic->hash.count = count;
          typeof (dst_ic->hash.hash_table) hash_table =
            MR_REALLOC (dst_ic->hash.hash_table, dst_ic->hash.count * sizeof (dst_ic->hash.hash_table[0]));
          if (NULL == hash_table)
            {
              MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
              mr_ic_hash_free (dst_ic);
              return (MR_FAILURE);
            }
          dst_ic->hash.hash_table = hash_table;
        }
    }

  if ((NULL != dst_ic->hash.hash_table) && (0 != dst_ic->hash.count))
    memset (dst_ic->hash.hash_table, 0, dst_ic->hash.count * sizeof (dst_ic->hash.hash_table[0]));
  return (mr_ic_foreach (src_ic, mr_ic_hash_index_visitor, dst_ic));
}

/**
 * Builds a hash table from an array of keys.
 * @param ic indexed collection
 * @param rarray array of keys
 * @param size size of the array in bytes
 * @return status
 */
mr_status_t
mr_ic_hash_index (mr_ic_t * ic, mr_ptr_t * rarray, size_t size)
{
  mr_ic_t ic_unsorted_array;
  mr_status_t status = mr_ic_unsorted_array_new (&ic_unsorted_array, ic->compar_fn, ic->key_type, &ic->context);
  if (MR_SUCCESS != status)
    return (status);
  
  status = mr_ic_index (&ic_unsorted_array, rarray, size);
  if (MR_SUCCESS != status)
    return (status);
  
  status = mr_ic_hash_reindex (ic, &ic_unsorted_array);
  mr_ic_free (&ic_unsorted_array);

  return (status);
}

/**
 * Adds a key to a hash table. The table is rebuilt with a bigger size when
 * it is half full.
 * @param ic indexed collection
 * @param key key to add
 * @return pointer on the added or already present key, NULL on error
 */
mr_ptr_t *
mr_ic_hash_add (mr_ic_t * ic, mr_ptr_t key)
{
  mr_ptr_t * find = mr_ic_find (ic, key);
  if (find != NULL)
    return (find);
  
  if (ic->items_count >= ic->hash.count / 2)
    {
      mr_ic_t dst_ic;
      if (MR_SUCCESS != mr_ic_hash_new (&dst_ic, ic->hash.hash_fn, ic->compar_fn, ic->key_type, &ic->context))
        return (NULL);
      ++ic->items_count;
      if (MR_SUCCESS != mr_ic_hash_reindex (&dst_ic, ic))
        return (NULL);

      mr_ic_free (ic);
      *ic = dst_ic;
    }
  return (mr_ic_hash_index_add (ic, key));
}

/**
 * Deletes a key from a hash table and moves the following keys of the
 * probe sequence to keep them reachable.
 * @param ic indexed collection
 * @param key key to delete
 * @return status, failure if the key is not found
 */
mr_status_t
mr_ic_hash_del (mr_ic_t * ic, mr_ptr_t key)
{
  mr_ptr_t * find = mr_ic_find (ic, key);
  if (NULL == find)
    return (MR_FAILURE);

  --ic->items_count;
  unsigned count = ic->hash.count - 1;
  unsigned bucket = find - ic->hash.hash_table;
  
  if (bucket == count)
    ic->hash.zero_key = false; /* release zero element */
  else
    {
      unsigned i = bucket;

      ic->hash.hash_table[i].intptr = 0;
      for (;;) /* need to re-index all elements in sequential blocks after deleted element */
        {
          if (++i >= count)
            i = 0;

          if (0 == ic->hash.hash_table[i].intptr)
            break;

          if (i == bucket)
            {
              MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_HASH_TABLE_ERROR);
              return (MR_FAILURE);
            }

          mr_ptr_t key = ic->hash.hash_table[i];
          ic->hash.hash_table[i].intptr = 0;
          --ic->items_count;

          if (mr_ic_hash_index_add (ic, key) == NULL)
            return (MR_FAILURE);
        }
    }

  return (MR_SUCCESS);
}

/**
 * Finds a key in a hash table.
 * @param ic indexed collection
 * @param key key to look for
 * @return pointer on the found key or NULL
 */
mr_ptr_t *
mr_ic_hash_find (mr_ic_t * ic, mr_ptr_t key)
{
  if (ic->hash.count < 2)
    return (NULL);
  
  unsigned count = ic->hash.count - 1;
  unsigned i, bucket = ic->hash.hash_fn (key, ic->context.data.ptr) % count;

  for (i = bucket; ;)
    {
      if (0 == ic->hash.hash_table[i].intptr)
        break;
      if (0 == ic->compar_fn (key, ic->hash.hash_table[i], ic->context.data.ptr))
        return (&ic->hash.hash_table[i]);
      if (++i >= count)
        i = 0;
      if (i == bucket)
        break;
    }
  
  if (ic->hash.zero_key &&
      (0 == ic->compar_fn (key, ic->hash.hash_table[count], ic->context.data.ptr)))
    return (&ic->hash.hash_table[count]);

  return (NULL);
}

/**
 * Calls a function for each key of a hash table.
 * @param ic indexed collection
 * @param visit_fn function to call
 * @param context context for the function
 * @return status, failure if a call failed
 */
mr_status_t
mr_ic_hash_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, void * context)
{
  if (ic->hash.count < 2)
    return (MR_SUCCESS);
  
  unsigned i, count = ic->hash.count - 1;
  
  if (ic->hash.zero_key)
    if (MR_SUCCESS != visit_fn (ic->hash.hash_table[count], context))
      return (MR_FAILURE);

  for (i = 0; i < count; ++i)
    if (0 != ic->hash.hash_table[i].intptr)
      if (MR_SUCCESS != visit_fn (ic->hash.hash_table[i], context))
        return (MR_FAILURE);
  return (MR_SUCCESS);
}

/**
 * Initializes a hash table collection. Without hash and comparison functions
 * and context the generic functions of the key type are used.
 * @param ic indexed collection
 * @param hash_fn hash function
 * @param compar_fn comparison function
 * @param key_type type name of the keys
 * @param context context for the hash and comparison functions, may be NULL
 * @return status
 */
mr_status_t
mr_ic_hash_new (mr_ic_t * ic, mr_hash_fn_t hash_fn, mr_compar_fn_t compar_fn, char * key_type, mr_res_t * context)
{
  static mr_ic_virt_func_t virt_func = {
    .add = mr_ic_hash_add,
    .del = mr_ic_hash_del,
    .find = mr_ic_hash_find,
    .foreach = mr_ic_hash_foreach,
    .index = mr_ic_hash_index,
    .free = mr_ic_hash_free,
  };
  mr_res_t generic_context = {
    .type = "mr_td_t",
    .mr_size = sizeof (mr_td_t),
  };

  if ((compar_fn == NULL) && (context == NULL) && (NULL == hash_fn))
    {
      mr_td_t * tdp = mr_get_td_by_name_internal (key_type);
      if (tdp)
        {
          generic_context.data.ptr = tdp;
          hash_fn = mr_generic_hash;
          compar_fn = mr_generic_cmp;
          context = &generic_context;
        }
    }
  
  if ((NULL == ic) || (NULL == compar_fn) || (NULL == hash_fn))
    return (MR_FAILURE);

  if (context)
    ic->context = *context;
  else
    memset (&ic->context, 0, sizeof (ic->context));

  ic->ic_type = MR_IC_HASH;
  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->virt_func = &virt_func;
  ic->items_count = 0;

  memset (&ic->hash, 0, sizeof (ic->hash));
  ic->hash.hash_fn = hash_fn;

  return (MR_SUCCESS);
}

/* ----------------------- MR_IC_STATIC_ARRAY ----------------------- */

/**
 * Adds a key to a small array embedded in the collection. A full array is
 * converted into a sorted array.
 * @param ic indexed collection
 * @param key key to add
 * @return pointer on the added or already present key, NULL on error
 */
mr_ptr_t *
mr_ic_static_array_add (mr_ic_t * ic, mr_ptr_t key)
{
  mr_ptr_t * find = mr_ic_find (ic, key);
  if (NULL != find)
    return (find);

  if (ic->items_count > sizeof (ic->static_array) / sizeof (ic->static_array[0]))
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_NUMBER_OF_ITEMS, ic->items_count);
      ic->items_count = 0;
    }

  if (ic->items_count == sizeof (ic->static_array) / sizeof (ic->static_array[0]))
    {
      mr_ic_t dst_ic;
      mr_status_t status = mr_ic_sorted_array_new (&dst_ic, ic->compar_fn, ic->key_type, &ic->context);
      if (MR_SUCCESS != status)
        return (NULL);

      status = mr_ic_index (&dst_ic, ic->static_array, sizeof (ic->static_array));
      if (MR_SUCCESS != status)
        return (NULL);

      *ic = dst_ic;
      return (mr_ic_add (ic, key));
    }

  mr_ptr_t * add = &ic->static_array[ic->items_count++];
  *add = key;
  return (add);
}

/**
 * Deletes a key from an embedded array.
 * @param ic indexed collection
 * @param key key to delete
 * @return status, failure if the key is not found
 */
mr_status_t
mr_ic_static_array_del (mr_ic_t * ic, mr_ptr_t key)
{
  mr_ptr_t * find = mr_ic_find (ic, key);
  if (NULL == find)
    return (MR_FAILURE);

  ptrdiff_t offset = (char*)find - (char*)&ic->static_array[0];
  --ic->items_count;
  memmove (find, &find[1], ic->items_count * sizeof (ic->static_array[0]) - offset);
  ic->static_array[ic->items_count].intptr = 0;
  return (MR_SUCCESS);
}

/**
 * Finds a key in an embedded array with a linear search.
 * @param ic indexed collection
 * @param key key to look for
 * @return pointer on the found key or NULL
 */
mr_ptr_t *
mr_ic_static_array_find (mr_ic_t * ic, mr_ptr_t key)
{
  int i;
  for (i = 0; i < ic->items_count; ++i)
    if (0 == ic->compar_fn (key, ic->static_array[i], ic->context.data.ptr))
      return (&ic->static_array[i]);
  return (NULL);
}

/**
 * Calls a function for each key of an embedded array.
 * @param ic indexed collection
 * @param visit_fn function to call
 * @param context context for the function
 * @return status, failure if a call failed
 */
mr_status_t
mr_ic_static_array_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, void * context)
{
  unsigned i;
  for (i = 0; i < ic->items_count; ++i)
    if (MR_SUCCESS != visit_fn (ic->static_array[i], context))
      return (MR_FAILURE);
  return (MR_SUCCESS);
}

/**
 * Clears an embedded array.
 * @param ic indexed collection
 */
void
mr_ic_static_array_free (mr_ic_t * ic)
{
  ic->items_count = 0;
  memset (&ic->static_array, 0, sizeof (ic->static_array));
}

/**
 * Fills an embedded array from an array of keys. Too many keys convert the
 * collection into a sorted array.
 * @param ic indexed collection
 * @param rarray array of keys
 * @param size size of the array in bytes
 * @return status
 */
mr_status_t
mr_ic_static_array_index (mr_ic_t * ic, mr_ptr_t * rarray, size_t size)
{
  if (size > sizeof (ic->static_array))
    {
      mr_status_t status = mr_ic_sorted_array_new (ic, ic->compar_fn, ic->key_type, &ic->context);
      if (MR_SUCCESS != status)
        return (status);
      return (mr_ic_index (ic, rarray, size));
    }

  size /= sizeof (rarray[0]);
  ic->items_count = 0;
  
  int i;
  for (i = 0; i < size; ++i)
    mr_ic_add (ic, rarray[i]);

  return (MR_SUCCESS);
}

/**
 * Initializes an embedded array collection. Without a comparison function
 * and context the generic comparison of the key type is used.
 * @param ic indexed collection
 * @param hash_fn unused
 * @param compar_fn comparison function
 * @param key_type type name of the keys
 * @param context context for the comparison function, may be NULL
 * @return status
 */
mr_status_t
mr_ic_static_array_new (mr_ic_t * ic, mr_hash_fn_t hash_fn, mr_compar_fn_t compar_fn, char * key_type, mr_res_t * context)
{
  static mr_ic_virt_func_t virt_func = {
    .add = mr_ic_static_array_add,
    .del = mr_ic_static_array_del,
    .find = mr_ic_static_array_find,
    .foreach = mr_ic_static_array_foreach,
    .index = mr_ic_static_array_index,
    .free = mr_ic_static_array_free,
  };
  mr_res_t generic_context = {
    .type = "mr_td_t",
    .mr_size = sizeof (mr_td_t),
  };

  if (NULL == ic)
    return (MR_FAILURE);

  if ((compar_fn == NULL) && (context == NULL))
    {
      mr_td_t * tdp = mr_get_td_by_name_internal (key_type);
      if (tdp)
        {
          generic_context.data.ptr = tdp;
          compar_fn = mr_generic_cmp;
          context = &generic_context;
        }
    }
  
  if (NULL == compar_fn)
    return (MR_FAILURE);

  if (context)
    ic->context = *context;
  else
    memset (&ic->context, 0, sizeof (ic->context));

  ic->ic_type = MR_IC_STATIC_ARRAY;
  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->virt_func = &virt_func;
  ic->items_count = 0;

  memset (&ic->static_array, 0, sizeof (ic->static_array));

  return (MR_SUCCESS);
}

/* ----------------------- MR_IC_RBTREE ----------------------- */

/**
 * Adds a key to a red-black tree.
 * @param ic indexed collection
 * @param key key to add
 * @return pointer on the added or already present key, NULL on error
 */
mr_ptr_t *
mr_ic_rbtree_add (mr_ic_t * ic, mr_ptr_t key)
{
  mr_ptr_t * add = mr_rbtree_add (key, &ic->rb, ic->compar_fn, ic->context.data.ptr);
  ic->items_count = ic->rb.size / sizeof (ic->rb.pool[0]) - 1;
  return (add);
}

/**
 * Deletes a key from a red-black tree.
 * @param ic indexed collection
 * @param key key to delete
 * @return status, failure if the key is not found
 */
mr_status_t
mr_ic_rbtree_del (mr_ic_t * ic, mr_ptr_t key)
{
  mr_status_t status = mr_rbtree_del (key, &ic->rb, ic->compar_fn, ic->context.data.ptr);
  ic->items_count = ic->rb.size / sizeof (ic->rb.pool[0]) - 1;
  return (status);
}

/**
 * Finds a key in a red-black or AVL tree.
 * @param ic indexed collection
 * @param key key to look for
 * @return pointer on the found key or NULL
 */
mr_ptr_t *
mr_ic_tree_find (mr_ic_t * ic, mr_ptr_t key)
{
  mr_tree_traverse_t traverse;
  mr_tree_find (key, &ic->tree, ic->compar_fn, ic->context.data.ptr, &traverse);
  return (traverse.equal ?
          &ic->tree.pool[traverse.path[traverse.count - 1].idx].key
          : NULL);
}

/**
 * Calls a function for each key of a tree in the order of the node pool.
 * @param ic indexed collection
 * @param visit_fn function to call
 * @param context context for the function
 * @return status, failure if a call failed
 */
mr_status_t
mr_ic_tree_foreach_unsorted (mr_ic_t * ic, mr_visit_fn_t visit_fn, void * context)
{
  unsigned i;
  if (ic->tree.size >= 2 * sizeof (ic->tree.pool[0]))
    for (i = ic->tree.size / sizeof (ic->tree.pool[0]) - 1; i > 0; --i)
      if (MR_SUCCESS != visit_fn (ic->tree.pool[i].key, context))
        return (MR_FAILURE);
  return (MR_SUCCESS);
}

/**
 * Calls a function for each key of a tree in sorted order.
 * @param ic indexed collection
 * @param visit_fn function to call
 * @param context context for the function
 * @return status, failure if a call failed
 */
mr_status_t
mr_ic_tree_foreach_sorted (mr_ic_t * ic, mr_visit_fn_t visit_fn, void * context)
{
  return (mr_tree_walk (&ic->tree, visit_fn, context));
}

/**
 * Frees the node pool of a tree.
 * @param ic indexed collection
 */
void
mr_ic_tree_free (mr_ic_t * ic)
{
  ic->items_count = 0;
  mr_tree_free (&ic->tree);
}

/**
 * Builds a tree from an array of keys. Of equal keys only the first is added.
 * @param ic indexed collection
 * @param rarray array of keys
 * @param size size of the array in bytes
 * @return status
 */
mr_status_t
mr_ic_tree_index (mr_ic_t * ic, mr_ptr_t * rarray, size_t size)
{
  /* Add elements into the tree in the natuaral order. In case of equal elements only first will be added. */
  int i, count = size / sizeof (rarray[0]);
  mr_tree_reserve (&ic->tree, count, true);
  for (i = 0; i < count; ++i)
    if (NULL == mr_ic_add (ic, rarray[i]))
      return (MR_FAILURE);

  return (MR_SUCCESS);
}

/**
 * Initializes a red-black tree collection. Without a comparison function
 * and context the generic comparison of the key type is used.
 * @param ic indexed collection
 * @param compar_fn comparison function
 * @param key_type type name of the keys
 * @param context context for the comparison function, may be NULL
 * @return status
 */
mr_status_t
mr_ic_rbtree_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type, mr_res_t * context)
{
  static mr_ic_virt_func_t virt_func = {
    .add = mr_ic_rbtree_add,
    .del = mr_ic_rbtree_del,
    .find = mr_ic_tree_find,
    .foreach = mr_ic_tree_foreach_unsorted,
    .index = mr_ic_tree_index,
    .free = mr_ic_tree_free,
  };
  mr_res_t generic_context = {
    .type = "mr_td_t",
    .mr_size = sizeof (mr_td_t),
  };
  
  if (NULL == ic)
    return (MR_FAILURE);

  if ((compar_fn == NULL) && (context == NULL))
    {
      mr_td_t * tdp = mr_get_td_by_name_internal (key_type);
      if (tdp)
        {
          generic_context.data.ptr = tdp;
          compar_fn = mr_generic_cmp;
          context = &generic_context;
        }
    }
  
  if (NULL == compar_fn)
    return (MR_FAILURE);

  if (context)
    ic->context = *context;
  else
    memset (&ic->context, 0, sizeof (ic->context));

  ic->ic_type = MR_IC_RBTREE;
  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->virt_func = &virt_func;
  ic->items_count = 0;

  mr_tree_init (&ic->rb);

  return (MR_SUCCESS);
}

/* ----------------------- MR_IC_AVLTREE ----------------------- */

/**
 * Adds a key to an AVL tree.
 * @param ic indexed collection
 * @param key key to add
 * @return pointer on the added or already present key, NULL on error
 */
mr_ptr_t *
mr_ic_avltree_add (mr_ic_t * ic, mr_ptr_t key)
{
  mr_ptr_t * add = mr_avltree_add (key, &ic->avl, ic->compar_fn, ic->context.data.ptr);
  ic->items_count = ic->avl.size / sizeof (ic->avl.pool[0]) - 1;
  return (add);
}

/**
 * Deletes a key from an AVL tree.
 * @param ic indexed collection
 * @param key key to delete
 * @return status, failure if the key is not found
 */
mr_status_t
mr_ic_avltree_del (mr_ic_t * ic, mr_ptr_t key)
{
  mr_status_t status = mr_avltree_del (key, &ic->avl, ic->compar_fn, ic->context.data.ptr);
  ic->items_count = ic->avl.size / sizeof (ic->avl.pool[0]) - 1;
  return (status);
}

/**
 * Initializes an AVL tree collection. Without a comparison function
 * and context the generic comparison of the key type is used.
 * @param ic indexed collection
 * @param compar_fn comparison function
 * @param key_type type name of the keys
 * @param context context for the comparison function, may be NULL
 * @return status
 */
mr_status_t
mr_ic_avltree_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type, mr_res_t * context)
{
  static mr_ic_virt_func_t virt_func = {
    .add = mr_ic_avltree_add,
    .del = mr_ic_avltree_del,
    .find = mr_ic_tree_find,
    .foreach = mr_ic_tree_foreach_unsorted,
    .index = mr_ic_tree_index,
    .free = mr_ic_tree_free,
  };
  mr_res_t generic_context = {
    .type = "mr_td_t",
    .mr_size = sizeof (mr_td_t),
  };
  
  if (NULL == ic)
    return (MR_FAILURE);

  if ((compar_fn == NULL) && (context == NULL))
    {
      mr_td_t * tdp = mr_get_td_by_name_internal (key_type);
      if (tdp)
        {
          generic_context.data.ptr = tdp;
          compar_fn = mr_generic_cmp;
          context = &generic_context;
        }
    }
  
  if (NULL == compar_fn)
    return (MR_FAILURE);

  if (context)
    ic->context = *context;
  else
    memset (&ic->context, 0, sizeof (ic->context));

  ic->ic_type = MR_IC_AVLTREE;
  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->virt_func = &virt_func;
  ic->items_count = 0;

  mr_tree_init (&ic->avl);

  return (MR_SUCCESS);
}
