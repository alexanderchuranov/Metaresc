/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */
#ifdef HAVE_CONFIG_H
#include <mr_config.h>
#endif /* HAVE_CONFIG_H */
#include <stdio.h>
#include <string.h>
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
 * traversed later as an object of another type.
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

/**
 * Post load references setting. If node was marked as references
 * it should be substitute with actual pointer. This substition
 * can't be made during structure loading because of forward references.
 * @param mr_load_data structures that holds context of loading
 * @return Status.
 */
static mr_status_t
mr_set_crossrefs (mr_ra_ptrdes_t * ptrs)
{
  mr_idx_t i, count = ptrs->size / sizeof (ptrs->ra[0]);
  mr_idx_t max_idx = 0;
  mr_idx_t * table;

  for (i = 1; i < count; ++i)
    if (ptrs->ra[i].idx > max_idx)
      max_idx = ptrs->ra[i].idx;

  table = MR_CALLOC (max_idx + 1, sizeof (table[0]));
  if (NULL == table)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (MR_FAILURE);
    }

  for (i = 1; i < count; ++i)
    if (ptrs->ra[i].idx != MR_NULL_IDX)
      {
        /* checking indexes collision */
        if (table[ptrs->ra[i].idx] != 0)
          MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_IDS_COLLISION, MR_REF_IDX, ptrs->ra[i].idx);
        table[ptrs->ra[i].idx] = i;
      }

  mr_status_t status = MR_SUCCESS;

  /* set all cross refernces */
  for (i = 1; (MR_SUCCESS == status) && (i < count); ++i)
    if (ptrs->ra[i].flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE))
      {
        if (ptrs->ra[i].first_child > max_idx)
          {
            MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNDEFINED_REF_IDX, MR_REF_IDX, ptrs->ra[i].first_child);
            status = MR_FAILURE;
            continue;
          }

        mr_idx_t idx = table[ptrs->ra[i].first_child];
        if (idx == 0)
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
  
  MR_FREE (table);

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
 * MR_NONE load handler (dummy)
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read
 */
static mr_status_t
mr_load_none (mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  return (MR_SUCCESS);
}

/**
 * MR_INTEGER load handler
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read (0 - failure, !0 - success)
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
#define CASE_SET_VALUE_BY_TYPE_(TYPE, MR_TYPE) case MR_TYPE: *(TYPE*)ptrdes->data.ptr = mr_value.vt_intmax; break;
      MR_FOREACH (CASE_SET_VALUE_BY_TYPE, bool, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t);
      CASE_SET_VALUE_BY_TYPE_ (mr_intmax_t, MR_TYPE_INT128);
      CASE_SET_VALUE_BY_TYPE_ (mr_uintmax_t, MR_TYPE_UINT128);
      
    case MR_TYPE_ENUM:
      switch (ptrdes->fdp->stype.size)
        {
#define CASE_SET_VALUE_BY_SIZE(TYPE) case sizeof (TYPE): *(TYPE*)ptrdes->data.ptr = mr_value.vt_intmax; break;
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
}

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
 * @param mr_load_data structures that holds context of loading
 * @return Status of read (0 - failure, !0 - success)
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
 * @param mr_load_data structures that holds context of loading
 * @return Status of read
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
 * @param mr_load_data structures that holds context of loading
 * @return Status of read
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
 * MR_STRING load handler. Allocate memory for a string.
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read
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
          if (ptrdes->vt_intptr >= 0)
            {
              ptrdes->first_child = ptrdes->vt_intptr;
              ptrdes->flags |= MR_IS_REFERENCE;
            }
          else
            {
              ptrdes->first_child = - ptrdes->vt_intptr;
              ptrdes->flags |= MR_IS_CONTENT_REFERENCE;
            }
          break;

        case MR_VT_UINTPTR:
          ptrdes->first_child = ptrdes->vt_uintptr;
          ptrdes->flags |= MR_IS_REFERENCE;
          break;

        case MR_VT_INTMAX:
          {
            if (*ptrdes->vt_intmax >= 0)
              {
                ptrdes->first_child = *ptrdes->vt_intmax;
                ptrdes->flags |= MR_IS_REFERENCE;
              }
            else
              {
                ptrdes->first_child = - *ptrdes->vt_intmax;
                ptrdes->flags |= MR_IS_CONTENT_REFERENCE;
              }
          }
          break;

        default:
          MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_TARGET_TYPE, ptrdes->value_type);
          status = MR_FAILURE;
          break;
        }
    }
  return (status);
}

static mr_status_t
mr_get_char_array (mr_idx_t idx, mr_ra_ptrdes_t * ptrs, char * str, size_t str_len)
{
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  typeof (str_len) max_size = ptrdes->fdp ? ptrdes->fdp->stype.size : 0;
  mr_status_t status = MR_SUCCESS;

  if ((str_len >= max_size) && (ptrdes->parent != MR_NULL_IDX))
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
 * @param mr_load_data structures that holds context of loading
 * @return Status of read
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
 * MR_STRUCT load handler.
 * Save content of subnodes to structure fileds.
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read
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

      /* recursively load subnode */
      status = mr_load (&data[fdp->offset], fdp, idx, ptrs);
    }
  return (status);
}

/**
 * MR_ARRAY load handler.
 * Save content of subnodes to array elements.
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read
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

  *bytes = value * scale;
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
 * the branch that the discriminator selects, and sizes of pointers must be
 * the same as in the size fields of the document. Memory of the values that
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
 * Loads element into newly allocate memory.
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read (0 - failure, !0 - success)
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
 * @param mr_load_data structures that holds context of loading
 * @return Status of read
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
 * @param data pointer on place to save data
 * @param fdp filed descriptor
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read
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
