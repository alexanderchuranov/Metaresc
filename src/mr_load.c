/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */
#ifdef HAVE_CONFIG_H
#include <mr_config.h>
#endif /* HAVE_CONFIG_H */
#include <stdio.h>
#include <string.h>
#include <time.h>
#ifdef HAVE_DLFCN_H
#define __USE_GNU
#include <dlfcn.h>
#endif /* HAVE_DLFCN_H */

#include <metaresc.h>
#include <mr_ic.h>
#include <mr_save.h>
#include <mr_load.h>
#include <mr_value.h>

TYPEDEF_FUNC (mr_status_t, mr_load_handler_t, (mr_idx_t /* idx */, mr_ra_ptrdes_t * /* ptrs */));

/**
 * Frees memory that was allocated by the loader for pointers and strings.
 * It is used to roll back a failed load. References do not own the memory.
 * @param ptrs array with descriptors of loaded data
 */
void
mr_free_loaded_pointers (mr_ra_ptrdes_t * ptrs)
{
  mr_idx_t i, count = ptrs->size / sizeof (ptrs->ra[0]);

  /* in the reverse order, so a block is freed after the pointers that are stored in it */
  for (i = count - 1; i > 0; --i)
    if (((MR_TYPE_POINTER == ptrs->ra[i].mr_type) || (MR_TYPE_STRING == ptrs->ra[i].mr_type))
        && ptrs->ra[i].data.ptr && *(void**)ptrs->ra[i].data.ptr)
      {
        /* references do not own the memory, but they could be already set to the memory of other nodes */
        if (!(ptrs->ra[i].flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE)))
          MR_FREE (*(void**)ptrs->ra[i].data.ptr);
        *(void**)ptrs->ra[i].data.ptr = NULL;
      }
}

/**
 * Checks that a node may be referenced by a pointer or by a string.
 * References are made on save only between nodes of the same type
 * (mr_cmp_ptrdes), so a reference to a node of another type comes from a
 * damaged document. The pointer would be set to an object that is
 * traversed later as an object of another type. A content reference must
 * refer on a string, and only pointers on bytes may hold one.
 * @param ptrs array with descriptor of loaded data
 * @param idx index of a node that is a reference
 * @param ref_idx index of the node that is referenced
 * @return true if the target is valid
 */
static bool
mr_ref_target_is_valid (mr_ra_ptrdes_t * ptrs, mr_idx_t idx, mr_idx_t ref_idx)
{
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  mr_ptrdes_t * target = &ptrs->ra[ref_idx];

  /*
    A reference on content takes the value of a pointer from a string node.
    Characters of a string are accessible through pointers on bytes only.
  */
  if (ptrdes->flags & MR_IS_CONTENT_REFERENCE)
    {
      if (MR_TYPE_STRING != target->mr_type)
        return (false);
      if (MR_TYPE_POINTER != ptrdes->mr_type)
        return (true);
      return ((MR_TYPE_VOID == ptrdes->mr_type_aux) || (MR_TYPE_CHAR_ARRAY == ptrdes->mr_type_aux));
    }

  /* a string refers to the characters of another string */
  if (MR_TYPE_STRING == ptrdes->mr_type)
    return (MR_TYPE_CHAR_ARRAY == target->mr_type);

  /* nothing is known about the type that a void pointer points to */
  mr_type_t type = ptrdes->mr_type_aux;
  if ((MR_TYPE_VOID == type) || (MR_TYPE_NONE == type))
    return (true);

  if (target->mr_type != type)
    return (false);

  /* the same rule as in mr_cmp_ptrdes: types that have a descriptor must have the same one */
  if (((MR_TYPED_TYPES >> type) & 1) || (MR_TYPE_POINTER == type))
    {
      mr_td_t * tdp = ptrdes->fdp ? ptrdes->fdp->stype.tdp : NULL;
      mr_td_t * target_tdp = target->fdp ? target->fdp->stype.tdp : NULL;
      return (tdp == target_tdp);
    }
  return (true);
}

/**
 * Sets a single cross reference. The node is marked as a reference and
 * must be substituted with the pointer to the data of the target node.
 * This substitution can't be made during loading because of forward references.
 * The function is shared by all loaders. Loaders differ in a way how
 * they resolve a reference to a target node.
 * @param ptrs array with descriptors of loaded data
 * @param idx node index of the reference
 * @param target node index of the node that the reference points to
 * @return MR_SUCCESS if the reference was set
 */
mr_status_t
mr_set_crossref (mr_ra_ptrdes_t * ptrs, mr_idx_t idx, mr_idx_t target)
{
  mr_idx_t count = ptrs->size / sizeof (ptrs->ra[0]);
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];

  if ((target <= 0) || (target >= count))
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNDEFINED_REF_IDX, MR_REF_IDX, target);
      return (MR_FAILURE);
    }

  if ((MR_TYPE_POINTER != ptrdes->mr_type) && (MR_TYPE_STRING != ptrdes->mr_type))
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_TARGET_TYPE, ptrdes->mr_type);
      return (MR_FAILURE);
    }

  if (NULL == ptrdes->data.ptr)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
      return (MR_FAILURE);
    }

  if (!mr_ref_target_is_valid (ptrs, idx, target))
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_REF_TARGET_TYPE, ptrs->ra[target].mr_type, ptrdes->mr_type);
      return (MR_FAILURE);
    }

  void * data = ptrs->ra[target].data.ptr;
  if (ptrdes->flags & MR_IS_CONTENT_REFERENCE)
    {
      if (NULL == data)
        {
          MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
          return (MR_FAILURE);
        }
      data = *(void**)data;
    }

  *(void**)ptrdes->data.ptr = data;
  return (MR_SUCCESS);
}

/* the key of mr_ref_idx_set_t that stands for the searched reference index, never a node */
#define MR_REF_IDX_SEARCH_KEY ((uintptr_t)-1)

/**
 * Gives the reference index of a key of mr_ref_idx_set_t.
 * @param key node index or MR_REF_IDX_SEARCH_KEY
 * @param set the set
 * @return reference index
 */
static inline mr_idx_t
mr_ref_idx_set_key (mr_ptr_t key, const mr_ref_idx_set_t * set)
{
  return ((MR_REF_IDX_SEARCH_KEY == key.uintptr) ? set->ref_idx : set->ra[key.uintptr].idx);
}

/**
 * Hashes a reference index with the multiplier of the set. The high bits of
 * the product depend on all bits of the index.
 * @param key node index or MR_REF_IDX_SEARCH_KEY
 * @param context the set
 * @return hash value
 */
static mr_hash_value_t
mr_ref_idx_set_hash (mr_ptr_t key, const void * context)
{
  const mr_ref_idx_set_t * set = context;
  return ((mr_ref_idx_set_key (key, set) * set->seed) >> 32);
}

/**
 * Makes a random odd multiplier for the hash of a set. It does not need to be
 * secure, only unknown to the author of a document: it mixes the time, the
 * processor time and the address of the set (randomized by ASLR) with
 * splitmix64. It keeps no state, so loads in several threads do not race.
 * @param set the set
 * @return odd multiplier
 */
static uint64_t
mr_ref_idx_set_seed (mr_ref_idx_set_t * set)
{
  uint64_t x = (uint64_t)time (NULL) ^ ((uint64_t)clock () << 20) ^ ((uint64_t)(uintptr_t)set * 0x9E3779B97F4A7C15ULL);
  x = (x ^ (x >> 30)) * 0xBF58476D1CE4E5B9ULL;
  x = (x ^ (x >> 27)) * 0x94D049BB133111EBULL;
  x ^= x >> 31;
  return (x | 1);
}

static int
mr_ref_idx_set_cmp (mr_ptr_t x, mr_ptr_t y, const void * context)
{
  mr_idx_t x_idx = mr_ref_idx_set_key (x, context);
  mr_idx_t y_idx = mr_ref_idx_set_key (y, context);
  return ((x_idx > y_idx) - (x_idx < y_idx));
}

/**
 * Adds a node with a reference index to the set. If another node has the same
 * reference index, the added node replaces it, and a warning is reported if
 * the set was made with warn_collisions.
 * @param set the set
 * @param node index of the node
 * @return status, failure if out of memory
 */
mr_status_t
mr_ref_idx_set_add (mr_ref_idx_set_t * set, mr_idx_t node)
{
  mr_ptr_t key = { .uintptr = node };
  mr_ptr_t * found = mr_ic_add (&set->ic, key);
  if (NULL == found)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (MR_FAILURE);
    }
  if (found->uintptr != node)
    {
      if (set->warn_collisions)
        MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_IDS_COLLISION, MR_REF_IDX, set->ra[node].idx);
      *found = key;
    }
  return (MR_SUCCESS);
}

/**
 * Builds the set of the nodes of a document that have a reference index.
 * @param set the set, must be freed with mr_ref_idx_set_free on success
 * @param ptrs nodes of the document
 * @param warn_collisions report nodes with the same reference index
 * @return status
 */
mr_status_t
mr_ref_idx_set_new (mr_ref_idx_set_t * set, mr_ra_ptrdes_t * ptrs, bool warn_collisions)
{
  mr_res_t context = {
    .data = { set },
    .type = "mr_ref_idx_set_t",
    .MR_SIZE = sizeof (*set),
  };
  mr_idx_t i, count = ptrs->size / sizeof (ptrs->ra[0]);

  set->ra = ptrs->ra;
  set->ref_idx = MR_NULL_IDX;
  set->seed = mr_ref_idx_set_seed (set);
  set->warn_collisions = warn_collisions;
  if (MR_SUCCESS != mr_ic_new (&set->ic, mr_ref_idx_set_hash, mr_ref_idx_set_cmp, "uintptr", MR_IC_HASH, &context))
    return (MR_FAILURE);

  for (i = 1; i < count; ++i)
    if (ptrs->ra[i].idx != MR_NULL_IDX)
      if (MR_SUCCESS != mr_ref_idx_set_add (set, i))
        {
          mr_ref_idx_set_free (set);
          return (MR_FAILURE);
        }
  return (MR_SUCCESS);
}

/**
 * Finds the node with a reference index.
 * @param set the set
 * @param ref_idx reference index of the document
 * @return index of the node or MR_NULL_IDX if no node has this reference index
 */
mr_idx_t
mr_ref_idx_set_find (mr_ref_idx_set_t * set, mr_idx_t ref_idx)
{
  if (MR_NULL_IDX == ref_idx)
    return (MR_NULL_IDX);
  set->ref_idx = ref_idx;
  mr_ptr_t * found = mr_ic_find (&set->ic, (mr_ptr_t){ .uintptr = MR_REF_IDX_SEARCH_KEY });
  return (found ? found->uintptr : MR_NULL_IDX);
}

/**
 * Frees the set.
 * @param set the set
 */
void
mr_ref_idx_set_free (mr_ref_idx_set_t * set)
{
  mr_ic_free (&set->ic);
}

/**
 * Post load references setting. A node that is marked as a reference is set
 * to the address of its target. This can't be done during loading because of
 * forward references. The reference index of the document in first_child is
 * replaced with the node index of the target.
 * @param ptrs array with descriptors of loaded data
 * @return MR_SUCCESS if all references were resolved and set
 */
static mr_status_t
mr_set_crossrefs (mr_ra_ptrdes_t * ptrs)
{
  mr_idx_t i, count = ptrs->size / sizeof (ptrs->ra[0]);
  mr_ref_idx_set_t set;

  /* a set and not a table indexed by the reference index: the indexes come from the document */
  if (MR_SUCCESS != mr_ref_idx_set_new (&set, ptrs, true))
    return (MR_FAILURE);

  mr_status_t status = MR_SUCCESS;

  /* set all cross refernces */
  for (i = 1; (MR_SUCCESS == status) && (i < count); ++i)
    if (ptrs->ra[i].flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE))
      {
        mr_idx_t idx = mr_ref_idx_set_find (&set, ptrs->ra[i].first_child);
        if (MR_NULL_IDX == idx)
          {
            MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNDEFINED_REF_IDX, MR_REF_IDX, ptrs->ra[i].first_child);
            status = MR_FAILURE;
            continue;
          }

        /* the problem is reported, the failed load frees the pointers that were set */
        if (MR_SUCCESS != mr_set_crossref (ptrs, i, idx))
          status = MR_FAILURE;
        else
          ptrs->ra[i].first_child = idx; /* the validation of the loaded data needs the node, not the index of the document */
      }

  mr_ref_idx_set_free (&set);

  return (status);
}

/**
 * Frees values that were allocated by the loader for the nodes. The values
 * share memory with union discriminators cache, so they must be freed before
 * the cache is used.
 * @param ptrs array with descriptors of loaded data
 */
void
mr_free_ptrdes_values (mr_ra_ptrdes_t * ptrs)
{
  if (NULL == ptrs)
    return;
  if (NULL == ptrs->ra)
    return;

  mr_idx_t i, count = ptrs->size / sizeof (ptrs->ra[0]);
  for (i = 1; i < count; ++i)
    {
      switch (ptrs->ra[i].value_type)
        {
        case MR_VT_STRING:
          if (ptrs->ra[i].vt_string)
            MR_FREE (ptrs->ra[i].vt_string);
          break;

        case MR_VT_INTMAX:
          if (ptrs->ra[i].vt_intmax)
            MR_FREE (ptrs->ra[i].vt_intmax);
          break;

        case MR_VT_LONG_DOUBLE:
          if (ptrs->ra[i].vt_long_double)
            MR_FREE (ptrs->ra[i].vt_long_double);
          break;

        case MR_VT_COMPLEX_LONG_DOUBLE:
          if (ptrs->ra[i].vt_complex_long_double)
            MR_FREE (ptrs->ra[i].vt_complex_long_double);
          break;

        default:
          break;
        }
      ptrs->ra[i].value_type = MR_VT_VOID;
    }
}

/**
 * Frees the values that a text loader stored in the node descriptors and the
 * array of descriptors itself, and clears the array.
 * @param ptrs array with descriptors of loaded data, may be NULL
 */
void
mr_free_load_values (mr_ra_ptrdes_t * ptrs)
{
  if (NULL == ptrs)
    return;
  if (NULL == ptrs->ra)
    return;

  mr_free_ptrdes_values (ptrs);
  MR_FREE (ptrs->ra);
  memset (ptrs, 0, sizeof (*ptrs));
}

/**
 * Adds a node of a document to the children of its parent. A reference keeps
 * the reference index of the document in first_child until mr_set_crossrefs,
 * so a node that is a reference can not have children. XML1 and YAML read
 * the annotation of a reference before the children of the node; CINIT and SCM
 * read it after them, and check it in mr_load_set_reference.
 * @param ptrs array with descriptors of loaded data
 * @param parent index of the parent node or MR_NULL_IDX
 * @param child index of the new node
 * @return status, failure if the parent is a reference
 */
mr_status_t
mr_load_add_child (mr_ra_ptrdes_t * ptrs, mr_idx_t parent, mr_idx_t child)
{
  if ((parent != MR_NULL_IDX) && (ptrs->ra[parent].flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE)))
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_SUBNODES);
      return (MR_FAILURE);
    }
  mr_add_child (ptrs, parent, child);
  return (MR_SUCCESS);
}

/**
 * Marks a node of a document as a reference. CINIT and SCM read the annotation
 * of a reference after the value, so the node may already have child nodes,
 * or be marked as a reference by another annotation. The reference index
 * would replace the first child, and the child nodes would not be loaded.
 * @param ptrs array with descriptors of loaded data
 * @param idx index of the node
 * @param ref_idx reference index of the document
 * @param flag MR_IS_REFERENCE or MR_IS_CONTENT_REFERENCE
 * @return status, failure if the node has child nodes or is a reference already
 */
mr_status_t
mr_load_set_reference (mr_ra_ptrdes_t * ptrs, mr_idx_t idx, mr_idx_t ref_idx, mr_ptrdes_flags_t flag)
{
  if ((ptrs->ra[idx].first_child != MR_NULL_IDX) || (ptrs->ra[idx].flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE)))
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_SUBNODES);
      return (MR_FAILURE);
    }
  ptrs->ra[idx].first_child = ref_idx;
  ptrs->ra[idx].flags |= flag;
  return (MR_SUCCESS);
}

/**
 * MR_NONE load handler (dummy)
 * @param idx node index
 * @param ptrs array with descriptors of loaded data
 * @return MR_SUCCESS or MR_FAILURE
 */
static mr_status_t
mr_load_none (mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  return (MR_SUCCESS);
}

/**
 * Checks that an integer fits a field of the given size as a signed or as an
 * unsigned value, so that -1 still loads into an unsigned field, but 257 does
 * not load into a byte.
 * @param value value to check
 * @param size size of the field in bytes
 * @return true if the value fits
 */
static inline bool
mr_int_fits (mr_intmax_t value, size_t size)
{
  if (size >= sizeof (value))
    return (true);
  int bits = size * __CHAR_BIT__;
  mr_intmax_t min = -(((mr_intmax_t)1) << (bits - 1));
  mr_intmax_t max = (((mr_intmax_t)1) << bits) - 1;
  return ((value >= min) && (value <= max));
}

/**
 * MR_INTEGER load handler
 * @param idx node index
 * @param ptrs array with descriptors of loaded data
 * @return MR_SUCCESS or MR_FAILURE
 */
mr_status_t
mr_load_integer (mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  mr_value_t mr_value;

  if (MR_SUCCESS != mr_ptrdes_to_mr_value (&mr_value, ptrdes, ptrs->str))
    return (MR_FAILURE);
  if (MR_SUCCESS != mr_value_cast (MR_VT_INTMAX, &mr_value))
    return (MR_FAILURE);

  switch (ptrdes->mr_type)
    {
#define CASE_SET_VALUE_BY_TYPE(TYPE) CASE_SET_VALUE_BY_TYPE_ (TYPE, MR_TYPE_DETECT (TYPE))
#define CASE_SET_VALUE_BY_TYPE_(TYPE, MR_TYPE) case MR_TYPE: if (!mr_int_fits (mr_value.vt_intmax, sizeof (TYPE))) goto out_of_range; *(TYPE*)ptrdes->data.ptr = mr_value.vt_intmax; break;
      MR_FOREACH (CASE_SET_VALUE_BY_TYPE, bool, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t);
      CASE_SET_VALUE_BY_TYPE_ (mr_intmax_t, MR_TYPE_INT128);
      CASE_SET_VALUE_BY_TYPE_ (mr_uintmax_t, MR_TYPE_UINT128);
      
    case MR_TYPE_ENUM:
      switch (ptrdes->fdp->stype.size)
        {
#define CASE_SET_VALUE_BY_SIZE(TYPE) case sizeof (TYPE): if (!mr_int_fits (mr_value.vt_intmax, sizeof (TYPE))) goto out_of_range; *(TYPE*)ptrdes->data.ptr = mr_value.vt_intmax; break;
          MR_FOREACH (CASE_SET_VALUE_BY_SIZE, uint8_t, uint16_t, uint32_t, uint64_t);
#ifdef HAVE_INT128
          MR_FOREACH (CASE_SET_VALUE_BY_SIZE, mr_uint128_t);
#endif /* HAVE_INT128 */

        default:
          if (ptrdes->fdp)
            memcpy (ptrdes->data.ptr, &mr_value.vt_intmax, MR_MIN (ptrdes->fdp->stype.size, sizeof (mr_value.vt_intmax)));
          break;
        }
      break;
    default:
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_TARGET_TYPE, ptrdes->mr_type);
      return (MR_FAILURE);
    }
  return (MR_SUCCESS);

 out_of_range:
  MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_INT_FIELD_RANGE, ptrdes->fdp ? ptrdes->fdp->name.str : "");
  return (MR_FAILURE);
}

/**
 * Resolves a function pointer from its name with dlsym, or parses it as a
 * number (an address) if the symbol is not found.
 * @param dst pointer on a variable that receives the function pointer
 * @param func_name name of the function or its address
 * @return status
 */
static mr_status_t
mr_get_func (void ** dst, char * func_name)
{
  void * func_addr = NULL;
#ifdef HAVE_LIBDL
  func_addr = dlsym (RTLD_DEFAULT, func_name);
#endif /* HAVE_LIBDL */
  if (func_addr != NULL)
    *dst = func_addr;
  else
    {
      char * end = NULL;
      mr_uintmax_t ptr = mr_strtouintmax (func_name, &end, 0);

      while (isspace (*end))
        ++end;
      if (*end != 0)
        {
          MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_FUNC, func_name);
          return (MR_FAILURE);
        }

      *dst = (void*)(uintptr_t)ptr;
    }

  return (MR_SUCCESS);
}

/**
 * MR_FUNC load handler
 * @param idx node index
 * @param ptrs array with descriptors of loaded data
 * @return MR_SUCCESS or MR_FAILURE
 */
mr_status_t
mr_load_func (mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  mr_status_t status = MR_SUCCESS;
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];

  *(void**)ptrdes->data.ptr = NULL;
  if (!(ptrdes->flags & MR_IS_NULL))
    switch (ptrdes->value_type)
      {
      case MR_VT_INTMAX:
        *(void**)ptrdes->data.ptr = (void*)(uintptr_t)ptrdes->vt_intmax;
        break;

      case MR_VT_UINTPTR:
      case MR_VT_INTPTR:
        *(void**)ptrdes->data.ptr = (void*)ptrdes->vt_intptr;
        break;

      case MR_VT_SUBSTR_POS:
        {
          char buf[1 << 6];
          size_t length = (ptrdes->vt_substr_pos.length < sizeof (buf) - 1) ? ptrdes->vt_substr_pos.length : sizeof (buf) - 1;

          buf[0] = 0;
          buf[length] = 0;
          if (ptrs->str)
            memcpy (buf, &ptrs->str[ptrdes->vt_substr_pos.offset], length);

          if (ptrdes->vt_substr_pos.length <= sizeof (buf) - 1)
            status = mr_get_func (ptrdes->data.ptr, buf);
          else
            {
              MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_FUNC, buf);
              status = MR_FAILURE;
            }
        }
        break;

      case MR_VT_STRING:
      case MR_VT_UNALLOCATED_STRING:
        if (NULL == ptrdes->vt_string)
          {
            MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
            status = MR_FAILURE;
          }
        else
          status = mr_get_func (ptrdes->data.ptr, ptrdes->vt_string);
        break;

      default:
        MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_TARGET_TYPE, ptrdes->value_type);
        status = MR_FAILURE;
        break;
      }
  return (status);
}

/**
 * MR_BITFIELD load handler. Load int from string and save it to
 * bit shifted field.
 * @param idx node index
 * @param ptrs array with descriptors of loaded data
 * @return MR_SUCCESS or MR_FAILURE
 */
static mr_status_t
mr_load_bitfield (mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  mr_uintmax_t value;
  mr_value_t mr_value;

  if (MR_SUCCESS != mr_ptrdes_to_mr_value (&mr_value, ptrdes, ptrs->str))
    return (MR_FAILURE);
  if (MR_SUCCESS != mr_value_cast (MR_VT_INTMAX, &mr_value))
    return (MR_FAILURE);

  value = mr_value.vt_intmax;
  return (mr_load_bitfield_value (ptrdes, &value));
}

/**
 * Loads a floating point node: casts the parsed value to long double and
 * stores it as float, double or long double.
 * @param idx index of the node
 * @param ptrs array with descriptors of loaded data
 * @return status
 */
static mr_status_t
mr_load_float (mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  mr_value_t mr_value;

  if (MR_SUCCESS != mr_ptrdes_to_mr_value (&mr_value, ptrdes, ptrs->str))
    return (MR_FAILURE);
  if (MR_SUCCESS != mr_value_cast (MR_VT_LONG_DOUBLE, &mr_value))
    return (MR_FAILURE);

  switch (ptrdes->mr_type)
    {
#define CASE_SET_FLOAT_BY_TYPE(TYPE) case MR_TYPE_DETECT (TYPE): *(TYPE*)ptrdes->data.ptr = mr_value.vt_long_double; break;
      MR_FOREACH (CASE_SET_FLOAT_BY_TYPE, float, double, long double);

    default:
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_TARGET_TYPE, mr_value.value_type);
      return (MR_FAILURE);
    }
  return (MR_SUCCESS);
}

/**
 * Loads a complex node: casts the parsed value to complex long double and
 * stores it with the precision of the field.
 * @param idx index of the node
 * @param ptrs array with descriptors of loaded data
 * @return status
 */
static mr_status_t
mr_load_complex (mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  mr_value_t mr_value;

  if (MR_SUCCESS != mr_ptrdes_to_mr_value (&mr_value, ptrdes, ptrs->str))
    return (MR_FAILURE);
  if (MR_SUCCESS != mr_value_cast (MR_VT_COMPLEX_LONG_DOUBLE, &mr_value))
    return (MR_FAILURE);

  switch (ptrdes->mr_type)
    {
#define CASE_SET_COMPLEX_BY_TYPE(TYPE) case MR_TYPE_DETECT (TYPE): *(TYPE*)ptrdes->data.ptr = mr_value.vt_complex_long_double; break;
      MR_FOREACH (CASE_SET_COMPLEX_BY_TYPE, complex_float_t, complex_double_t, complex_long_double_t);

    default:
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_TARGET_TYPE, mr_value.value_type);
      return (MR_FAILURE);
    }
  return (MR_SUCCESS);
}

/**
 * MR_CHAR load handler. Handles nonprint characters in octal format.
 * @param idx node index
 * @param ptrs array with descriptors of loaded data
 * @return MR_SUCCESS or MR_FAILURE
 */
static mr_status_t
mr_load_char (mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  mr_status_t status = MR_SUCCESS;
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  
  switch (ptrdes->value_type)
    {
    case MR_VT_SUBSTR_POS:
      if ((ptrdes->vt_substr_pos.length == 0) || (ptrs->str == NULL))
        *(char*)ptrdes->data.ptr = 0;
      else if (ptrdes->vt_substr_pos.length == sizeof (char))
        *(char*)ptrdes->data.ptr = ptrs->str[ptrdes->vt_substr_pos.offset];
      else
        {
          char buf[1 << 8];
          size_t length = (ptrdes->vt_substr_pos.length < sizeof (buf) - 1) ? ptrdes->vt_substr_pos.length : sizeof (buf) - 1;

          buf[length] = 0;
          memcpy (buf, &ptrs->str[ptrdes->vt_substr_pos.offset], length);

          MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_READ_CHAR, buf);
        }
      break;

    case MR_VT_CHAR:
      *(char*)ptrdes->data.ptr = ptrdes->vt_char;
      break;

    case MR_VT_STRING:
    case MR_VT_UNALLOCATED_STRING:
      if (NULL == ptrdes->vt_string)
        {
          MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
          status = MR_FAILURE;
          break;
        }

      if (strlen (ptrdes->vt_string) == sizeof (char))
        {
          *(char*)ptrdes->data.ptr = ptrdes->vt_string[0];
          break;
        }
      /*
        JSON and XML are UTF-8, and save writes a char beyond ASCII as an escape of
        the code point U+0080 to U+00FF, which loads as a sequence of two bytes.
      */
      {
        unsigned char * s = (unsigned char *)ptrdes->vt_string;
        if ((2 == strlen (ptrdes->vt_string)) && ((0xC2 == s[0]) || (0xC3 == s[0])) && (0x80 == (s[1] & 0xC0)))
          {
            *(unsigned char*)ptrdes->data.ptr = ((s[0] & 0x1F) << 6) | (s[1] & 0x3F);
            break;
          }
      }
      __attribute__ ((fallthrough));

    default:
      {
        mr_value_t mr_value;
        if (MR_SUCCESS != mr_ptrdes_to_mr_value (&mr_value, ptrdes, ptrs->str))
          return (MR_FAILURE);
        if (MR_SUCCESS != mr_value_cast (MR_VT_INTMAX, &mr_value))
          return (MR_FAILURE);
        *(char*)ptrdes->data.ptr = mr_value.vt_intmax;
        break;
      }
    }

  return (status);
}

/**
 * Sets a reference of a string node given by a number: a positive number is a
 * reference, a negative number is a content reference.
 * @param ptrdes node of the string
 * @param value index of the referenced node, negated for a content reference
 * @return status, failure if the index does not fit mr_idx_t
 */
static mr_status_t
mr_load_string_reference (mr_ptrdes_t * ptrdes, mr_intmax_t value)
{
  mr_uintmax_t magnitude = (value >= 0) ? (mr_uintmax_t)value : -(mr_uintmax_t)value;
  if (magnitude > (mr_idx_t)-1)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_INT_FIELD_RANGE, ptrdes->fdp ? ptrdes->fdp->name.str : "");
      return (MR_FAILURE);
    }
  ptrdes->first_child = magnitude;
  ptrdes->flags |= (value >= 0) ? MR_IS_REFERENCE : MR_IS_CONTENT_REFERENCE;
  return (MR_SUCCESS);
}

/**
 * MR_STRING load handler. Allocate memory for a string.
 * @param idx node index
 * @param ptrs array with descriptors of loaded data
 * @return MR_SUCCESS or MR_FAILURE
 */
static mr_status_t
mr_load_string (mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  mr_status_t status = MR_SUCCESS;
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  
  *(char**)ptrdes->data.ptr = NULL;
  if (!(ptrdes->flags & (MR_IS_NULL | MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE)))
    {
      switch (ptrdes->value_type)
        {
        case MR_VT_SUBSTR_POS:
          *(char**)ptrdes->data.ptr = MR_CALLOC (1, ptrdes->vt_substr_pos.length + sizeof (char));
          if (*(char**)ptrdes->data.ptr)
            memcpy (*(char**)ptrdes->data.ptr, &ptrs->str[ptrdes->vt_substr_pos.offset], ptrdes->vt_substr_pos.length);
          else
            {
              MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
              status = MR_FAILURE;
            }
          break;
          
        case MR_VT_STRING:
          *(char**)ptrdes->data.ptr = ptrdes->vt_string;
          ptrdes->vt_string = NULL;
          break;

        case MR_VT_UNALLOCATED_STRING:
          *(char**)ptrdes->data.ptr = mr_strdup (ptrdes->vt_string);
          break;

        case MR_VT_INTPTR:
          status = mr_load_string_reference (ptrdes, ptrdes->vt_intptr);
          break;

        case MR_VT_UINTPTR:
          if (ptrdes->vt_uintptr > (mr_idx_t)-1)
            {
              MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_INT_FIELD_RANGE, ptrdes->fdp ? ptrdes->fdp->name.str : "");
              status = MR_FAILURE;
            }
          else
            {
              ptrdes->first_child = ptrdes->vt_uintptr;
              ptrdes->flags |= MR_IS_REFERENCE;
            }
          break;

        case MR_VT_INTMAX:
          status = mr_load_string_reference (ptrdes, *ptrdes->vt_intmax);
          break;

        default:
          MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_TARGET_TYPE, ptrdes->value_type);
          status = MR_FAILURE;
          break;
        }
    }
  return (status);
}

/**
 * Copies a string into a char array. A char array that is the target of a
 * pointer is reallocated to fit the string, a fixed char array truncates it
 * with a warning.
 * @param idx index of the node
 * @param ptrs array with descriptors of loaded data
 * @param str string to copy, not necessarily zero terminated
 * @param str_len length of the string
 * @return status
 */
static mr_status_t
mr_get_char_array (mr_idx_t idx, mr_ra_ptrdes_t * ptrs, char * str, size_t str_len)
{
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  typeof (str_len) max_size = ptrdes->fdp ? ptrdes->fdp->stype.size : 0;
  mr_status_t status = MR_SUCCESS;

  /*
    Only a char array of unknown size (a pointer on a char array type without a descriptor)
    is a dynamic string that owns its block. Elements of a known size are fixed char arrays
    in the block of the pointer, and the address of an element after the first one
    is not an address of an allocated block.
  */
  if ((0 == max_size) && (ptrdes->parent != MR_NULL_IDX))
    if (MR_TYPE_POINTER == ptrs->ra[ptrdes->parent].mr_type)
      {
        void * data = MR_REALLOC (ptrdes->data.ptr, str_len + 1);
        if (NULL == data)
          {
            if (ptrdes->data.ptr != NULL)
              MR_FREE (ptrdes->data.ptr);
            MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
            status = MR_FAILURE;
          }

        *(void**)ptrs->ra[ptrdes->parent].data.ptr = ptrdes->data.ptr = data;
        max_size = str_len + 1;
      }

  if (str_len > max_size)
    {
      str_len = max_size;
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_STRING_TRUNCATED);
    }

  if (ptrdes->data.ptr != NULL)
    {
      memcpy (ptrdes->data.string, str, str_len);
      if (str_len < max_size)
        ptrdes->data.string[str_len] = 0;
    }

  return (status);
}  

/**
 * MR_CHAR_ARRAY load handler.
 * Save string in place (truncate string if needed).
 * @param idx node index
 * @param ptrs array with descriptors of loaded data
 * @return MR_SUCCESS or MR_FAILURE
 */
static mr_status_t
mr_load_char_array (mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  mr_status_t status = MR_SUCCESS;

  if (ptrdes->fdp)
    if (ptrdes->fdp->stype.size > 0)
      *(char*)ptrdes->data.ptr = 0;
  
  switch (ptrdes->value_type)
    {
    case MR_VT_SUBSTR_POS:
      status = mr_get_char_array (idx, ptrs, &ptrs->str[ptrdes->vt_substr_pos.offset], ptrdes->vt_substr_pos.length);
      break;

    case MR_VT_STRING:
    case MR_VT_UNALLOCATED_STRING:
      if (NULL == ptrdes->vt_string)
        {
          MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
          status = MR_FAILURE;
        }
      else
        status = mr_get_char_array (idx, ptrs, ptrdes->vt_string, strlen (ptrdes->vt_string));
      break;

    default:
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_TARGET_TYPE, ptrdes->value_type);
      status = MR_FAILURE;
      break;
    }
  
  return (status);
}

/**
 * Returns the field that follows a given field in the declaration order.
 * Used to load unnamed values of a structure.
 * @param tdp type descriptor of the structure
 * @param fdp current field or NULL for the first field
 * @return next field or NULL after the last one
 */
static mr_fd_t *
mr_load_struct_next_field (mr_td_t * tdp, mr_fd_t * fdp)
{
  int i;

  for (i = 0; i < tdp->param.struct_param.fields_count; ++i)
    if (NULL == fdp)
      return (tdp->param.struct_param.fields[i]);
    else if (tdp->param.struct_param.fields[i] == fdp)
      fdp = NULL;

  return (NULL);
}

/**
 * Checks whether a loaded node or any node below it sets a pointer: a string
 * that is set, a pointer with elements, or a reference. Elements of pointers
 * are allocated, and references are set, after the whole document is parsed,
 * so the pointer would be written through this node later, and the memory it
 * refers on would be freed through every node that shares the field.
 * The first_child of a reference is not a child node.
 * @param ptrs array with descriptors of loaded data
 * @param root index of the node
 * @return true if another node must not share the memory of this node
 */
static bool
mr_node_owns_memory (mr_ra_ptrdes_t * ptrs, mr_idx_t root)
{
  mr_idx_t node = root;
  for (;;)
    {
      mr_ptrdes_t * ptrdes = &ptrs->ra[node];
      bool reference = ptrdes->flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE);

      if (MR_TYPE_STRING == ptrdes->mr_type)
        {
          if (reference || (ptrdes->data.ptr && *(char**)ptrdes->data.ptr))
            return (true);
        }
      else if (MR_TYPE_POINTER == ptrdes->mr_type)
        {
          /* mr_load_pointer_postponed makes a reference of a pointer with an integer value */
          bool int_value = (MR_VT_INTMAX == ptrdes->value_type) || (MR_VT_UINTPTR == ptrdes->value_type) || (MR_VT_INTPTR == ptrdes->value_type);
          if (reference || int_value || (!(ptrdes->flags & MR_IS_NULL) && (ptrdes->first_child != MR_NULL_IDX)))
            return (true);
        }
      else if (!reference && (ptrdes->first_child != MR_NULL_IDX))
        {
          node = ptrdes->first_child;
          continue;
        }

      /* go to the next sibling, climbing up until the root */
      while ((node != root) && (MR_NULL_IDX == ptrs->ra[node].next))
        node = ptrs->ra[node].parent;
      if (node == root)
        return (false);
      node = ptrs->ra[node].next;
    }
}

/**
 * MR_STRUCT load handler.
 * Save content of subnodes to structure fileds.
 * @param idx node index
 * @param ptrs array with descriptors of loaded data
 * @return MR_SUCCESS or MR_FAILURE
 */
static mr_status_t
mr_load_struct (mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  mr_fd_t * fdp = NULL;
  char * data = ptrs->ra[idx].data.ptr;
  mr_idx_t first_child = ptrs->ra[idx].first_child;
  mr_td_t * tdp = ptrs->ra[idx].fdp ? ptrs->ra[idx].fdp->stype.tdp : NULL;
  mr_status_t status = MR_SUCCESS;

  /* get pointer on structure descriptor */
  if (NULL == tdp)
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_NO_TYPE_DESCRIPTOR, "unknown");
      return (MR_FAILURE);
    }

  /*
    Fields of a document are nodes that refer on the memory of the field. As in C, a field may
    be initialized more than once, and the last value wins, unless the earlier node owns memory
    that would be overwritten. Branches of a union share the memory in the same way.
  */
  int fields_count = tdp->param.struct_param.fields_count;
  mr_idx_t loaded[fields_count > 0 ? fields_count : 1];
  mr_idx_t branch = MR_NULL_IDX;
  int field_idx = -1;
  memset (loaded, 0, sizeof (loaded));

  /* loop on all subnodes */
  for (idx = first_child; (MR_SUCCESS == status) && (idx != MR_NULL_IDX); idx = ptrs->ra[idx].next)
    {
      char * name = ptrs->ra[idx].fdp ? ptrs->ra[idx].fdp->name.str : NULL;
      mr_fd_t * next_fdp = name ? mr_get_fd_by_name (tdp, name) : NULL;
      fdp = next_fdp ? next_fdp : mr_load_struct_next_field (tdp, fdp);

      if (NULL == fdp)
        {
          MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNKNOWN_SUBNODE, tdp->type.str, name);
          status = MR_FAILURE;
          continue;
        }

      /* mr_validate_load checks every branch node against the discriminator, so a union has one branch */
      mr_idx_t prev_branch = branch;
      if ((MR_UNION_TYPES >> tdp->mr_type) & 1)
        {
          if (branch != MR_NULL_IDX)
            {
              MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNION_BRANCHES, tdp->type.str);
              status = MR_FAILURE;
              continue;
            }
          branch = idx;
        }

      /* fields usually follow in the order of declaration, so check the next field first */
      if ((field_idx + 1 < fields_count) && (tdp->param.struct_param.fields[field_idx + 1] == fdp))
        ++field_idx;
      else
        for (field_idx = 0; field_idx < fields_count; ++field_idx)
          if (tdp->param.struct_param.fields[field_idx] == fdp)
            break;

      mr_idx_t prev_loaded = MR_NULL_IDX;
      if (field_idx < fields_count)
        {
          prev_loaded = loaded[field_idx];
          if ((prev_loaded != MR_NULL_IDX) && mr_node_owns_memory (ptrs, prev_loaded))
            {
              MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_FIELD_LOADED_TWICE, fdp->name.str, tdp->type.str);
              status = MR_FAILURE;
              continue;
            }
          loaded[field_idx] = idx;
        }

      /*
        CINIT saves an anonymous union as a placeholder with its name followed by an unnamed node.
        mr_load_anon_union passes the field to the next node, which loads the union itself.
      */
      mr_idx_t next = ptrs->ra[idx].next;
      bool placeholder = (MR_TYPE_NAMED_ANON_UNION == fdp->stype.mr_type) && (MR_NULL_IDX == ptrs->ra[idx].first_child)
        && (next != MR_NULL_IDX) && (NULL == ptrs->ra[next].fdp);

      /* recursively load subnode */
      status = mr_load (&data[fdp->offset], fdp, idx, ptrs);

      if (placeholder && (MR_SUCCESS == status) && (ptrs->ra[next].fdp == fdp))
        {
          if (field_idx < fields_count)
            loaded[field_idx] = prev_loaded;
          branch = prev_branch;
        }
    }
  return (status);
}

/**
 * MR_ARRAY load handler.
 * Save content of subnodes to array elements.
 * @param idx node index
 * @param ptrs array with descriptors of loaded data
 * @return MR_SUCCESS or MR_FAILURE
 */
static mr_status_t
mr_load_array (mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  char * data = ptrs->ra[idx].data.ptr;
  mr_fd_t fd_ = *ptrs->ra[idx].fdp;
  mr_status_t status = MR_SUCCESS;
  mr_idx_t i, count = fd_.stype.dim.dim[0];
  mr_idx_t array_idx = idx;

  if (0 == count)
    {
      if (ptrs->ra[idx].first_child != MR_NULL_IDX)
        {
          MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_RANGE_CHECK, fd_.name.str);
          return (MR_FAILURE);
        }
      return (MR_SUCCESS);
    }

  if (0 == fd_.stype.size)
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_ZERO_SIZE_FIELD, fd_.name.str);
      return (MR_FAILURE);
    }

  fd_.non_persistent = true;
  fd_.unnamed = true;
  fd_.offset = 0;
  fd_.stype.size /= count;

  if (fd_.stype.dim.dim_count == 1)
    {
      fd_.stype.mr_type = fd_.stype.mr_type_aux; /* prepare copy of filed descriptor for array elements loading */
      fd_.stype.mr_type_aux = fd_.stype.tdp ? fd_.stype.tdp->mr_type : MR_TYPE_VOID;
    }
  else
    {
      --fd_.stype.dim.dim_count;
      memmove (&fd_.stype.dim.dim[0], &fd_.stype.dim.dim[1], fd_.stype.dim.dim_count * sizeof (fd_.stype.dim.dim[0]));
    }

  /* loop on subnodes */
  i = 0;
  for (idx = ptrs->ra[idx].first_child; idx != MR_NULL_IDX; idx = ptrs->ra[idx].next)
    {
      /* check if array index is in range */
      if (i >= count)
        {
          MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_RANGE_CHECK, fd_.name.str);
          return (MR_FAILURE);
        }
      /* load recursively */
      status = mr_load (&data[i++ * fd_.stype.size], &fd_, idx, ptrs);
      if (status != MR_SUCCESS)
        break;
    }

  /* a size field of a single dimensional array declares the number of loaded elements */
  if (1 == ptrs->ra[array_idx].fdp->stype.dim.dim_count)
    ptrs->ra[array_idx].MR_SIZE = i * fd_.stype.size;
  return (status);
}

/**
 * Size of an element that a pointer points to.
 * @param ptrdes descriptor of a pointer node
 * @return size of the element or zero if it is not known
 */
static mr_size_t
mr_pointer_element_size (mr_ptrdes_t * ptrdes)
{
  /* a pointer on void does not have elements, whatever descriptor is attached to it */
  if ((MR_TYPE_VOID == ptrdes->mr_type_aux) || (MR_TYPE_NONE == ptrdes->mr_type_aux))
    return (0);
  mr_size_t element_size = mr_type_size (ptrdes->mr_type_aux);
  mr_td_t * tdp = ptrdes->fdp ? ptrdes->fdp->stype.tdp : NULL;
  if (0 == element_size)
    element_size = tdp ? tdp->size : 0;
  return (element_size);
}

/**
 * Reads a size field of a pointer or of an array as a number of bytes.
 * A size field of a document may be of any integer type, so the value is
 * read without truncation and checked to fit into mr_size_t.
 * @param size_field descriptor of the size field (mr_pointer_get_size_ptrdes)
 * @param element_size size of an element, zero if it is not known
 * @param bytes the number of bytes that the size field declares
 * @return MR_SUCCESS if the value is not negative and fits into mr_size_t
 */
static mr_status_t
mr_get_size_field_bytes (mr_ptrdes_t * size_field, mr_size_t element_size, mr_size_t * bytes)
{
  const mr_size_t max_size = ~(mr_size_t)0;
  mr_intmax_t value = 0;
  mr_ptrdes_t dst = {
    .data.ptr = &value,
    .mr_type = MR_TYPE_DETECT (__typeof__ (value)),
  };
  mr_assign_int (&dst, size_field);

  /* a count field (count_field_offset or count_field_name) holds the number of elements */
  mr_size_t scale = ((size_field->vt_string[0] == 'c') && (0 != element_size)) ? element_size : 1;
  if ((value < 0) || (value > max_size / scale))
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_WRONG_SIZE_FIELD, (long long)value);
      return (MR_FAILURE);
    }

  /* the check above makes the value fit into mr_size_t, and the product fits too */
  *bytes = (mr_size_t)value * scale;
  return (MR_SUCCESS);
}

/**
 * Checks the size field of a pointer or of an array against the size of
 * the loaded data. A serializable size field must match the loaded data.
 * A size field of type void is not in a document, so it is set from
 * the loaded data.
 * @param size_field descriptor of the size field (mr_pointer_get_size_ptrdes)
 * @param size number of loaded bytes
 * @param element_size size of an element, zero if it is not known
 * @return MR_SUCCESS if the size field is consistent with the loaded data
 */
static mr_status_t
mr_validate_size_field (mr_ptrdes_t * size_field, mr_size_t size, mr_size_t element_size)
{
  if (MR_TYPE_VOID == size_field->mr_type)
    {
      mr_size_t value = size;
      mr_ptrdes_t src = {
        .data.ptr = &value,
        .mr_type = MR_TYPE_DETECT (__typeof__ (value)),
      };
      /* a count field (count_field_offset or count_field_name) holds the number of elements */
      if ((size_field->vt_string[0] == 'c') && (0 != element_size))
        value /= element_size;
      mr_assign_int (size_field, &src);
      return (MR_SUCCESS);
    }

  mr_size_t doc_size = 0;
  if (MR_SUCCESS != mr_get_size_field_bytes (size_field, element_size, &doc_size))
    return (MR_FAILURE);

  if (doc_size != size)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_POINTER_SIZE_MISMATCH, doc_size, size);
      return (MR_FAILURE);
    }
  return (MR_SUCCESS);
}

/**
 * Number of bytes that are available for a pointer that refers on a node,
 * starting from the address of this node.
 * @param ptrs array with descriptors of loaded data
 * @param ref_idx index of the referenced node
 * @param content true if the reference is on a content of a string node
 * @return number of bytes, zero if the extent is not known
 */
static mr_size_t
mr_ref_target_extent (mr_ra_ptrdes_t * ptrs, mr_idx_t ref_idx, bool content)
{
  mr_ptrdes_t * target = &ptrs->ra[ref_idx];

  if (NULL == target->data.ptr)
    return (0);

  /* content of a string is the block that the string points to, mr_set_crossref accepts only string targets for content */
  if (content)
    {
      char * str = *(char**)target->data.ptr;
      return (str ? strlen (str) + 1 : 0);
    }

  /* an element of a block that is owned by a pointer or of an array extends up to the end of the block */
  mr_idx_t parent = target->parent;
  if ((parent != MR_NULL_IDX) && (NULL != ptrs->ra[parent].data.ptr) &&
      ((MR_TYPE_POINTER == ptrs->ra[parent].mr_type) || (MR_TYPE_ARRAY == ptrs->ra[parent].mr_type)))
    {
      /*
        a row of a multi dimensional array is a part of the block of the outermost array,
        MR_SIZE of the outermost array is the size of the whole block
      */
      if (MR_TYPE_ARRAY == ptrs->ra[parent].mr_type)
        while ((ptrs->ra[parent].parent != MR_NULL_IDX) &&
               (MR_TYPE_ARRAY == ptrs->ra[ptrs->ra[parent].parent].mr_type))
          parent = ptrs->ra[parent].parent;

      /* the block starts with the first element (the first row starts at the same address) */
      mr_idx_t first_child = ptrs->ra[parent].first_child;
      if (MR_NULL_IDX == first_child)
        return (0);

      char * start = ptrs->ra[first_child].data.ptr;
      if ((char*)target->data.ptr < start)
        return (0);
      mr_offset_t offset = (char*)target->data.ptr - start;
      mr_size_t extent = ptrs->ra[parent].MR_SIZE;
      return (offset < extent ? extent - offset : 0);
    }

  mr_size_t size = mr_type_size (target->mr_type);
  mr_td_t * tdp = target->fdp ? target->fdp->stype.tdp : NULL;
  if ((0 == size) && tdp && (((MR_TYPED_TYPES >> target->mr_type) & 1)))
    size = tdp->size;
  if ((0 == size) && target->fdp)
    size = target->fdp->stype.size;
  return (size);
}

/**
 * A pointer that refers on a node does not own a block, but it must not
 * declare more data than there is in the block that it refers on.
 * @param idx node index of a pointer
 * @param ptrs array with descriptors of loaded data
 * @return MR_SUCCESS if the declared size fits into the referenced block
 */
static mr_status_t
mr_validate_reference_size (mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  mr_size_t element_size = mr_pointer_element_size (ptrdes);
  mr_ptrdes_t src;
  mr_pointer_get_size_ptrdes (&src, idx, ptrs);

  mr_size_t declared = element_size; /* a pointer without a size field points on a single element */
  if (src.data.ptr != NULL)
    {
      if (MR_TYPE_VOID == src.mr_type)
        return (MR_SUCCESS); /* there is nothing declared in a document */
      if (MR_SUCCESS != mr_get_size_field_bytes (&src, element_size, &declared))
        return (MR_FAILURE);
    }

  /* mr_set_crossrefs replaced the index of the document with the index of the target node and checked its type */
  bool content = ptrdes->flags & MR_IS_CONTENT_REFERENCE;
  mr_size_t available = mr_ref_target_extent (ptrs, ptrdes->first_child, content);
  if (declared > available)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_REFERENCE_SIZE_MISMATCH, declared, available);
      return (MR_FAILURE);
    }
  return (MR_SUCCESS);
}

/**
 * Checks that a pointer that was loaded from a document has the size that the
 * document declares in the size field. The size field of type void is updated
 * to the size of loaded data. A pointer without a size field points on a
 * single element. A pointer that refers on another node must fit into
 * the referenced block.
 * @param idx node index of a pointer
 * @param ptrs array with descriptors of loaded data
 * @return MR_SUCCESS if the size field is consistent with the loaded data
 */
static mr_status_t
mr_validate_pointer_size (mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];

  /* a reference is set by the loader of references even if the node is flagged as NULL */
  if (ptrdes->flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE))
    return (mr_validate_reference_size (idx, ptrs));
  if (ptrdes->flags & MR_IS_NULL)
    return (MR_SUCCESS);
  /*
    there are no loaded data in a pointer without content,
    but opaque data are loaded as a block of bytes without child nodes
  */
  if (((MR_NULL_IDX == ptrdes->first_child) && !mr_is_opaque_data (ptrdes)) ||
      (NULL == ptrdes->data.ptr) || (NULL == *(void**)ptrdes->data.ptr))
    return (MR_SUCCESS);

  mr_size_t element_size = mr_pointer_element_size (ptrdes);
  mr_ptrdes_t src;
  mr_pointer_get_size_ptrdes (&src, idx, ptrs);
  if (src.data.ptr != NULL)
    return (mr_validate_size_field (&src, ptrdes->MR_SIZE, element_size));

  /* nothing declares the size, so the pointer owns a single element */
  if ((0 != element_size) && (ptrdes->MR_SIZE != element_size))
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_POINTER_SIZE_MISMATCH, element_size, ptrdes->MR_SIZE);
      return (MR_FAILURE);
    }
  return (MR_SUCCESS);
}

/**
 * Checks that a loaded union has the branch that its discriminator resolves
 * to. The branch of a document is selected by the name of a field or by a type
 * cast, and the discriminator is an ordinary field of a document, so they may
 * not agree. All consumers of a loaded object (save, copy, free) resolve the
 * branch by the discriminator and would treat the data as a type that it was
 * not loaded as. The resolution is cached in the nodes in the same way as on
 * save.
 * @param idx node index of a union
 * @param mr_save_data context of the union discriminators resolution
 * @return MR_SUCCESS if all loaded branches are the discriminated one
 */
static mr_status_t
mr_validate_union_branch (mr_idx_t idx, mr_save_data_t * mr_save_data)
{
  mr_ra_ptrdes_t * ptrs = &mr_save_data->ptrs;
  mr_idx_t parent, child;

  if (MR_NULL_IDX == ptrs->ra[idx].first_child)
    return (MR_SUCCESS);

  /*
    The same lookup of a field descriptor of the union as in mr_save_union.
    Elements of arrays and pointers are saved with temporary descriptors,
    and mr_save_union takes the descriptor of the array or pointer field with
    the discriminator. The loaders replace temporary descriptors with
    the descriptor of the type (mr_ptr_fd) on exit, so these are skipped too.
    A root union has no field with a discriminator in both cases: text loaders
    validate before the temporary descriptor of the root is replaced, and XDR
    replaces it with mr_ptr_fd.
  */
  for (parent = idx; parent != MR_NULL_IDX; parent = ptrs->ra[parent].parent)
    {
      mr_fd_t * fdp = ptrs->ra[parent].fdp;
      if (fdp && !fdp->non_persistent && !(fdp->stype.tdp && (fdp == &fdp->stype.tdp->mr_ptr_fd)))
        break;
    }
  if (MR_NULL_IDX == parent)
    return (MR_SUCCESS);

  mr_fd_t * union_fdp = ptrs->ra[parent].fdp;
  /* a union without a discriminator is always saved as the default branch,
     but documents may select any of the branches */
  if (NULL == mr_get_any_fd_by_name (union_fdp->meta, NULL))
    return (MR_SUCCESS);

  mr_fd_t * discriminated_fdp = mr_union_discriminator (mr_save_data, idx, union_fdp);
  for (child = ptrs->ra[idx].first_child; child != MR_NULL_IDX; child = ptrs->ra[child].next)
    if (!(ptrs->ra[child].flags & MR_IS_NULL) && (ptrs->ra[child].fdp != discriminated_fdp))
      {
        MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNION_BRANCH,
                    ptrs->ra[child].fdp ? ptrs->ra[child].fdp->name.str : "unknown",
                    discriminated_fdp ? discriminated_fdp->name.str : "unknown");
        return (MR_FAILURE);
      }
  return (MR_SUCCESS);
}

/**
 * A single dimensional array may have a size field in the same way as
 * a pointer. The size field limits the number of serialized elements.
 * A serializable size field must match the number of loaded elements,
 * and a size field of type void is set from the loaded data.
 * @param idx node index of an array
 * @param ptrs array with descriptors of loaded data
 * @return MR_SUCCESS if the size field is consistent with the loaded data
 */
static mr_status_t
mr_validate_array_size (mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  mr_fd_t * fdp = ptrdes->fdp;

  /* only arrays that are fields of a structure have a size field */
  if ((NULL == fdp) || fdp->non_persistent || (fdp->stype.dim.dim_count != 1) || (0 == fdp->stype.dim.dim[0]))
    return (MR_SUCCESS);

  mr_ptrdes_t src;
  mr_pointer_get_size_ptrdes (&src, idx, ptrs);
  if (NULL == src.data.ptr)
    return (MR_SUCCESS);

  return (mr_validate_size_field (&src, ptrdes->MR_SIZE, fdp->stype.size / fdp->stype.dim.dim[0]));
}

/**
 * Validates consistency of a loaded document: unions must be loaded as
 * the branch that the discriminator selects, sizes of pointers and arrays
 * must be the same as in their size fields, and references must fit into
 * the blocks that they refer on. Memory of the values that
 * were allocated by the loader is shared with the cache of union
 * discriminators, so the values must be freed before this call
 * (mr_free_ptrdes_values).
 * @param ptrs array with descriptors of loaded data
 * @return MR_SUCCESS if the document is consistent
 */
mr_status_t
mr_validate_load (mr_ra_ptrdes_t * ptrs)
{
  mr_save_data_t mr_save_data = { .ptrs = *ptrs, };
  mr_idx_t i, count = ptrs->size / sizeof (ptrs->ra[0]);
  mr_status_t status = MR_SUCCESS;
  mr_res_t context = {
    .data = { &mr_save_data.ptrs },
    .type = "mr_ra_ptrdes_t",
    .MR_SIZE = sizeof (mr_ra_ptrdes_t),
  };

  if (MR_SUCCESS != mr_ic_new (&mr_save_data.union_discriminators, mr_uds_get_hash, mr_uds_cmp, "uintptr", MR_IC_HASH, &context))
    return (MR_FAILURE);

  for (i = 1; i < count; ++i)
    {
      ptrs->ra[i].value_type = MR_VT_UD_SET;
      ptrs->ra[i].ud_set.count = 0;
    }

  for (i = 1; (MR_SUCCESS == status) && (i < count); ++i)
    switch (ptrs->ra[i].mr_type)
      {
      case MR_TYPE_POINTER:
        status = mr_validate_pointer_size (i, ptrs);
        break;

      case MR_TYPE_ARRAY:
        status = mr_validate_array_size (i, ptrs);
        break;

      case MR_TYPE_UNION:
      case MR_TYPE_ANON_UNION:
      case MR_TYPE_NAMED_ANON_UNION:
        status = mr_validate_union_branch (i, &mr_save_data);
        break;

      default:
        break;
      }

  for (i = 1; i < count; ++i)
    mr_ud_free (&ptrs->ra[i]);

  if (mr_save_data.mr_ra_ud != NULL)
    MR_FREE (mr_save_data.mr_ra_ud);
  mr_ic_free (&mr_save_data.union_discriminators);

  return (status);
}

/**
 * Allocates the block of a pointer and loads its elements into it.
 * A pointer whose value is a reference index is marked as a reference
 * and is set later by mr_set_crossrefs.
 * @param idx node index
 * @param ptrs array with descriptors of loaded data
 * @return MR_SUCCESS or MR_FAILURE
 */
static mr_status_t
mr_load_pointer_postponed (mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  mr_status_t status = MR_SUCCESS;
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  char ** data = ptrs->ra[idx].data.ptr;
  mr_idx_t count = 0;
  mr_idx_t node;

  if (NULL == data)
    return (MR_FAILURE);

  *data = NULL; /* default initializer */
  if (ptrdes->flags & MR_IS_NULL)
    return (MR_SUCCESS);
  
  if ((MR_VT_INTMAX == ptrdes->value_type) || (MR_VT_UINTPTR == ptrdes->value_type) || (MR_VT_INTPTR == ptrdes->value_type))
    {
      mr_intmax_t vt_int = 0;
      if (MR_VT_INTMAX == ptrdes->value_type)
        vt_int = *ptrdes->vt_intmax;
      else if (MR_VT_UINTPTR == ptrdes->value_type)
        vt_int = ptrdes->vt_uintptr;
      else if (MR_VT_INTPTR == ptrdes->value_type)
        vt_int = ptrdes->vt_intptr;

      if (vt_int >= 0)
        {
          ptrdes->first_child = vt_int;
          ptrdes->flags |= MR_IS_REFERENCE;
        }
      else
        {
          ptrdes->first_child = - vt_int;
          ptrdes->flags |= MR_IS_CONTENT_REFERENCE;
        }
    }

  if (ptrdes->flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE))
    return (MR_SUCCESS);

  for (node = ptrs->ra[idx].first_child; node != MR_NULL_IDX; node = ptrs->ra[node].next)
    ++count;
  if (0 == count)
    return (MR_SUCCESS);

  mr_td_t * tdp = ptrs->ra[idx].fdp ? ptrs->ra[idx].fdp->stype.tdp : NULL;
  mr_fd_t fd_;
  memset (&fd_, 0, sizeof (fd_));
  fd_.non_persistent = true;
  fd_.unnamed = true;
  fd_.stype.mr_type = ptrs->ra[idx].mr_type_aux;
  fd_.stype.mr_type_aux = tdp ? tdp->mr_type : MR_TYPE_VOID;
  fd_.stype.tdp = tdp;
  fd_.stype.size = mr_type_size (fd_.stype.mr_type);
  if (fd_.stype.size == 0)
    fd_.stype.size = fd_.stype.tdp ? fd_.stype.tdp->size : 0;
  if (fd_.stype.size == 0)
    return (MR_FAILURE);

  /* allocate memory */
  *data = MR_CALLOC (count, fd_.stype.size);
  if (NULL == *data)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (MR_FAILURE);
    }
  
  /* load recursively */
  mr_idx_t i = 0;
  for (node = ptrs->ra[idx].first_child; (MR_SUCCESS == status) && (node != MR_NULL_IDX); node = ptrs->ra[node].next)
    status = mr_load (*data + i++ * fd_.stype.size, &fd_, node, ptrs); /* mr_load sets MR_SIZE of the element */

  ptrs->ra[idx].MR_SIZE = count * fd_.stype.size;
  
  return (status);
}

/**
 * MR_ANON_UNION load handler.
 * Load anonymous union
 * @param idx node index
 * @param ptrs array with descriptors of loaded data
 * @return MR_SUCCESS or MR_FAILURE
 */
static mr_status_t
mr_load_anon_union (mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];

  /*
    Anonimous unions in C init style saved as named field folowed by union itself. Named field has type of zero length static string and must be inited by empty string. Here is an example.
    .anon_union_79 = "", {
    .union_float = 3.1415927410,
    },
  */
  if ((ptrdes->first_child == 0) && /* if node has no childs, then it is C init style anonumous union */
      (MR_VT_SUBSTR_POS == ptrdes->value_type)
      && (0 == ptrdes->vt_substr_pos.length) && /* content must be an empty string */
      (ptrdes->fdp != NULL) && /* node must have a name */
      (ptrdes->next != MR_NULL_IDX))
    if (NULL == ptrs->ra[ptrdes->next].fdp) /* there should be a next node without name */
      {
        ptrs->ra[ptrdes->next].fdp = ptrdes->fdp;
        return (MR_SUCCESS); /* now next node has a name and will be loaded by top level procedure */
      }
  return (mr_load_struct (idx, ptrs));
}

/**
 * Init IO handlers Table
 */
static mr_load_handler_t mr_load_handler[MR_TYPE_LAST] =
  {
    [MR_TYPE_NONE] = mr_load_none,
    [MR_TYPE_VOID] = mr_load_none,
    [MR_TYPE_ENUM] = mr_load_integer,
    [MR_TYPE_BITFIELD] = mr_load_bitfield,
    [MR_TYPE_BOOL] = mr_load_integer,
    [MR_TYPE_INT8] = mr_load_integer,
    [MR_TYPE_UINT8] = mr_load_integer,
    [MR_TYPE_INT16] = mr_load_integer,
    [MR_TYPE_UINT16] = mr_load_integer,
    [MR_TYPE_INT32] = mr_load_integer,
    [MR_TYPE_UINT32] = mr_load_integer,
    [MR_TYPE_INT64] = mr_load_integer,
    [MR_TYPE_UINT64] = mr_load_integer,
    [MR_TYPE_INT128] = mr_load_integer,
    [MR_TYPE_UINT128] = mr_load_integer,
    [MR_TYPE_FLOAT] = mr_load_float,
    [MR_TYPE_COMPLEX_FLOAT] = mr_load_complex,
    [MR_TYPE_DOUBLE] = mr_load_float,
    [MR_TYPE_COMPLEX_DOUBLE] = mr_load_complex,
    [MR_TYPE_LONG_DOUBLE] = mr_load_float,
    [MR_TYPE_COMPLEX_LONG_DOUBLE] = mr_load_complex,
    [MR_TYPE_CHAR] = mr_load_char,
    [MR_TYPE_CHAR_ARRAY] = mr_load_char_array,
    [MR_TYPE_STRING] = mr_load_string,
    [MR_TYPE_STRUCT] = mr_load_struct,
    [MR_TYPE_FUNC] = mr_load_func,
    [MR_TYPE_FUNC_TYPE] = mr_load_func,
    [MR_TYPE_ARRAY] = mr_load_array,
    [MR_TYPE_POINTER] = mr_load_none,
    [MR_TYPE_UNION] = mr_load_struct,
    [MR_TYPE_ANON_UNION] = mr_load_struct,
    [MR_TYPE_NAMED_ANON_UNION] = mr_load_anon_union,
  };

/**
 * Public function. Load router. Load any object from internal representation graph.
 * On the root node (idx 1) it also loads the pointers, sets the references
 * and validates the loaded data, and frees what was allocated if it fails.
 * @param data pointer on place to save data
 * @param fdp field descriptor
 * @param idx node index
 * @param ptrs array with descriptors of loaded data
 * @return MR_SUCCESS or MR_FAILURE
 */
mr_status_t
mr_load (void * data, mr_fd_t * fdp, mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  mr_status_t status = MR_FAILURE;

  if (NULL == data)
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_NULL_POINTER);
      return (MR_FAILURE);
    }

  if ((idx <= 0) || (idx >= ptrs->size / sizeof (ptrs->ra[0])))
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_SAVE_IDX_RANGE_CHECK);
      return (MR_FAILURE);
    }

  if ((ptrs->ra[idx].flags & MR_IS_TYPED) && ptrs->ra[idx].fdp)
    if (fdp->stype.tdp != ptrs->ra[idx].fdp->stype.tdp)
      {
        MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_NODE_TYPE_MISSMATCH, fdp->name.str,
                    fdp->stype.tdp ? fdp->stype.tdp->type.str : "undefined",
                    ptrs->ra[idx].fdp->stype.tdp ? ptrs->ra[idx].fdp->stype.tdp->type.str : "undefined");
        return (MR_FAILURE);
      }

  if (!fdp->unnamed && ptrs->ra[idx].fdp && fdp->name.str)
    if (ptrs->ra[idx].fdp->name.str)
      if (0 != strcmp (fdp->name.str, ptrs->ra[idx].fdp->name.str))
        {
          MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_NODE_NAME_MISSMATCH, fdp->name.str, ptrs->ra[idx].fdp->name.str);
          return (MR_FAILURE);
        }

  /*
    Save makes references only of pointers and strings. A reference keeps the reference
    index of the document in first_child, and load handlers of other types would take it
    for the index of the first child node.
  */
  if ((ptrs->ra[idx].flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE)) &&
      (MR_TYPE_POINTER != fdp->stype.mr_type) && (MR_TYPE_STRING != fdp->stype.mr_type))
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_REFERENCE_NOT_A_POINTER, fdp->name.str ? fdp->name.str : MR_DEFAULT_NODE_NAME);
      return (MR_FAILURE);
    }

  ptrs->ra[idx].data.ptr = data;
  ptrs->ra[idx].fdp = fdp;
  ptrs->ra[idx].MR_SIZE = fdp->stype.size;
  ptrs->ra[idx].mr_type = fdp->stype.mr_type;
  ptrs->ra[idx].mr_type_aux = fdp->stype.mr_type_aux;

  /* route loading */
  mr_load_handler_t load_handler = NULL;
  if ((fdp->stype.mr_type >= 0) && (fdp->stype.mr_type < MR_TYPE_LAST))
    load_handler = mr_load_handler[fdp->stype.mr_type];

  if (load_handler != NULL)
    status = load_handler (idx, ptrs);
  else
    MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNSUPPORTED_NODE_TYPE, fdp->stype.mr_type);

  /* set cross references at the upper level */
  if (1 == idx)
    {
      mr_idx_t i, count = ptrs->size / sizeof (ptrs->ra[0]);

      for (i = 0; (MR_SUCCESS == status) && (i < count); ++i)
        if (MR_TYPE_POINTER == ptrs->ra[i].mr_type)
          status = mr_load_pointer_postponed (i, ptrs);
      
      if (MR_SUCCESS == status)
        status = mr_set_crossrefs (ptrs);

      if (MR_SUCCESS == status)
        {
          /* values of the nodes share memory with the cache of union discriminators */
          mr_free_ptrdes_values (ptrs);
          status = mr_validate_load (ptrs);
        }

      if (MR_SUCCESS != status)
        mr_free_loaded_pointers (ptrs);
    }

  if (fdp->non_persistent)
    ptrs->ra[idx].fdp = fdp->stype.tdp ? &fdp->stype.tdp->mr_ptr_fd : NULL;

  return (status);
}
