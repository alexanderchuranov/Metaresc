/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <mr_tsearch.h>
#ifdef HAVE_CONFIG_H
# include <mr_config.h>
#endif /* HAVE_CONFIG_H */
#ifdef HAVE_ENDIAN_H
#include <endian.h>
#endif /* HAVE_ENDIAN_H */
#ifdef HAVE_MACHINE_ENDIAN_H
#include <machine/endian.h>
#endif /* HAVE_MACHINE_ENDIAN_H */

#include <metaresc.h>
#include <mr_ic.h>
#include <mr_stringify.h>
#include <mr_save.h>

/* MR_IC_RBTREE   ( / 1001626 7396.0) ratio: 135.42 */
/* MR_IC_HASH_NEXT ( / 237032 9368.0) ratio: 25.30 */
/* MR_IC_HASH_TREE  ( / 51574 7396.0) ratio: 6.97 */

#define MR_IC_DYNAMIC_DEFAULT MR_IC_HASH_TREE

TYPEDEF_FUNC (void, mr_save_handler_t, (mr_save_data_t *))

static mr_save_handler_t mr_save_handler[];
static mr_save_handler_t ext_mr_save_handler[];

/**
 * Comparator for mr_ptrdes_t.
 * @param x value to compare
 * @param y value to compare
 * @return comparation sign
 */
static int
mr_cmp_ptrdes (mr_ptrdes_t * x, mr_ptrdes_t * y)
{
  int diff = x->data - y->data;
  if (diff) return (diff);
  diff = x->fd.mr_type_ext - y->fd.mr_type_ext;
  if (diff) return (diff);
  diff = x->fd.mr_type - y->fd.mr_type;
  if (diff) return (diff);

  switch (x->fd.mr_type)
    {
    case MR_TYPE_STRING:
    case MR_TYPE_CHAR_ARRAY:
    case MR_TYPE_CHAR:
    case MR_TYPE_NONE:
    case MR_TYPE_VOID:
    case MR_TYPE_BOOL:
    case MR_TYPE_INT8:
    case MR_TYPE_UINT8:
    case MR_TYPE_INT16:
    case MR_TYPE_UINT16:
    case MR_TYPE_INT32:
    case MR_TYPE_UINT32:
    case MR_TYPE_INT64:
    case MR_TYPE_UINT64:
    case MR_TYPE_FLOAT:
    case MR_TYPE_DOUBLE:
    case MR_TYPE_LONG_DOUBLE:
      break;
    case MR_TYPE_BITFIELD:
      diff = x->fd.param.bitfield_param.shift - y->fd.param.bitfield_param.shift;
      if (diff) return (diff);
      diff = x->fd.param.bitfield_param.width - y->fd.param.bitfield_param.width;
      if (diff) return (diff);
      break;
    default:
      diff = strcmp (x->fd.type, y->fd.type);
      if (diff) return (diff);
      break;
    }
  return (0);
}

mr_hash_value_t __attribute__ ((unused))
mr_typed_ptrdes_get_hash (const mr_ptr_t x, const void * context)
{
  const mr_ra_mr_ptrdes_t * ptrs = context;
  const mr_ptrdes_t * ptrdes = &ptrs->ra[x.long_int_t];
  return ((long)ptrdes->data + ptrdes->fd.mr_type + ptrdes->fd.mr_type_ext * MR_TYPE_LAST);
}

int
mr_typed_ptrdes_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const mr_ra_mr_ptrdes_t * ptrs = context;
  return (mr_cmp_ptrdes (&ptrs->ra[x.long_int_t], &ptrs->ra[y.long_int_t]));
}

mr_hash_value_t __attribute__ ((unused))
mr_untyped_ptrdes_get_hash (const mr_ptr_t x, const void * context)
{
  const mr_ra_mr_ptrdes_t * ptrs = context;
  return ((long)ptrs->ra[x.long_int_t].data);
}

int
mr_untyped_ptrdes_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const mr_ra_mr_ptrdes_t * ptrs = context;
  return (ptrs->ra[x.long_int_t].data - ptrs->ra[y.long_int_t].data);
}

/**
 * Typed lookup for a pointer (last element in collection) in collection of already saved pointers.
 * If node is a plain structure and previously it was saved as a pointer
 * then we need to exchange nodes in the tree
 * @param mr_save_data save routines data and lookup structures
 * @return an index of node in collection of already saved nodes
 */
static int
mr_resolve_typed_forward_ref (mr_save_data_t * mr_save_data)
{
  mr_ra_mr_ptrdes_t * ptrs = &mr_save_data->ptrs;
  long count = ptrs->size / sizeof (ptrs->ra[0]) - 1;
  mr_ptr_t * search_result;

  search_result = mr_ic_add (&mr_save_data->typed_ptrs, count, ptrs);
  if (NULL == search_result) /* out of memory */
    return (-1);
  if ((search_result->long_int_t != count) &&
      (ptrs->ra[search_result->long_int_t].parent >= 0))
    if (MR_TYPE_EXT_POINTER == ptrs->ra[ptrs->ra[search_result->long_int_t].parent].fd.mr_type_ext)
      return (search_result->long_int_t);
  return (-1);
}

/**
 * Untyped lookup for a pointer (last element in collection) in collection of already saved pointers.
 * @param mr_save_data save routines data and lookup structures
 * @return an index of node in collection of already saved nodes
 */
static int
mr_resolve_untyped_forward_ref (mr_save_data_t * mr_save_data)
{
  mr_ra_mr_ptrdes_t * ptrs = &mr_save_data->ptrs;
  long count = ptrs->size / sizeof (ptrs->ra[0]) - 1;
  void * data = ptrs->ra[count].data;
  int same_ptr = count;
  mr_ptr_t * search_result;

  search_result = mr_ic_add (&mr_save_data->untyped_ptrs, count, ptrs);
  if (NULL == search_result) /* out of memory */
    return (-1);

  /*
    We need to walk up in a object tree and find upper parent with the same address.
    I.e. we need to detect situation like
    .ptr = (type1_t[]){
    .first_field1 = {
    .first_field2 = xxx,
    }
    }
    first_field2 has the same address as .ptr and lookup will return match for .ptr node.
  */
  for (;;)
    {
      int parent = ptrs->ra[same_ptr].parent;
      if ((parent < 0) || (ptrs->ra[parent].data != data))
	break;
      same_ptr = parent;
    }
  /*
    node with the same address was found and it was not a parent structure.
    Found node is a pointer if its parent has mr_type_ext == MR_TYPE_EXT_POINTER
  */
  if ((search_result->long_int_t != same_ptr) &&
      (ptrs->ra[search_result->long_int_t].parent >= 0))
    if (MR_TYPE_EXT_POINTER == ptrs->ra[ptrs->ra[search_result->long_int_t].parent].fd.mr_type_ext)
      return (search_result->long_int_t);
  return (-1);
}

/**
 * Comparator for union discriminator info structures
 * @param x index in mr_union_discriminator_t rarray
 * @param y index in mr_union_discriminator_t rarray
 * @param content void pointer to context
 */
int
mr_ud_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const mr_save_data_t * mr_save_data = context;
  int diff = mr_hashed_string_cmp (&mr_save_data->mr_ra_ud[x.long_int_t].type,
				   &mr_save_data->mr_ra_ud[y.long_int_t].type, NULL);
  if (diff)
    return (diff);
  return (mr_hashed_string_cmp (&mr_save_data->mr_ra_ud[x.long_int_t].discriminator,
				&mr_save_data->mr_ra_ud[y.long_int_t].discriminator, NULL));
}

mr_hash_value_t __attribute__ ((unused))
mr_ud_get_hash (mr_ptr_t x, const void * context)
{
  const mr_save_data_t * mr_save_data = context;
  return ((mr_hashed_string_get_hash (&mr_save_data->mr_ra_ud[x.long_int_t].type, NULL) << 1) +
	  mr_hashed_string_get_hash (&mr_save_data->mr_ra_ud[x.long_int_t].discriminator, NULL));
}

/**
 * Check whether pointer is presented in the collection of already saved pointers.
 * @param mr_save_data save routines data and lookup structures
 * @param data pointer on saved object
 * @param fdp field descriptor
 * @return an index of node in collection of already saved nodes
 */
static int
mr_check_ptr_in_list (mr_save_data_t * mr_save_data, void * data, mr_fd_t * fdp)
{
  mr_ra_mr_ptrdes_t * ptrs = &mr_save_data->ptrs;
  mr_ptr_t * find_result;
  long idx;

  idx = mr_add_ptr_to_list (ptrs);
  if (idx < 0)
    return (idx); /* memory allocation error occured */
  /* populate attributes of new node */
  ptrs->ra[idx].data = data;
  ptrs->ra[idx].fd = *fdp;
  /* this element is required only for a search so we need to adjust back size of collection */
  ptrs->size -= sizeof (ptrs->ra[0]);
  /* search in index of typed references */
  find_result = mr_ic_find (&mr_save_data->typed_ptrs, idx, ptrs);
  if (find_result)
    return (find_result->long_int_t);
  /* search in index of untyped references */
  find_result = mr_ic_find (&mr_save_data->untyped_ptrs, idx, ptrs);
  if (find_result)
    return (find_result->long_int_t);
  return (-1);
}

/**
 * Save scheduler. Save any object into internal representation.
 * @param data a pointer on data
 * @param fdp a ponter of field descriptor
 * @param mr_save_data save routines data and lookup structures
 */
static void
mr_save_inner (void * data, mr_fd_t * fdp, mr_save_data_t * mr_save_data, int parent)
{
  int idx, ref_idx;

  /* add pointer on saving structure to list ptrs */
  idx = mr_add_ptr_to_list (&mr_save_data->ptrs);
  if (idx < 0)
    return; /* memory allocation error occured */
  mr_save_data->ptrs.ra[idx].data = data;
  mr_save_data->ptrs.ra[idx].fd = *fdp;
  mr_save_data->ptrs.ra[idx].parent = parent; /* NB: mr_add_child do the same, but also adds links from parent to child. This link is requred for mr_resolve_untyped_forward_ref  */

  mr_ic_new (&mr_save_data->ptrs.ra[idx].union_discriminator, mr_ud_get_hash, mr_ud_cmp, "long_int_t", MR_IC_DYNAMIC_DEFAULT);

  /* forward reference resolving */
  ref_idx = mr_resolve_typed_forward_ref (mr_save_data);
  if (ref_idx >= 0)
    {
      int ref_parent = mr_save_data->ptrs.ra[ref_idx].parent;
      mr_save_data->ptrs.ra[ref_parent].ref_idx = ref_idx;
      mr_save_data->ptrs.ra[ref_parent].first_child = -1;
      mr_save_data->ptrs.ra[ref_parent].last_child = -1;
      mr_save_data->ptrs.ra[ref_idx].flags.is_referenced = TRUE;
      mr_save_data->ptrs.ra[ref_idx].fd.name.str = fdp->name.str;
      mr_add_child (parent, ref_idx, &mr_save_data->ptrs);
      return;
    }
  mr_resolve_untyped_forward_ref (mr_save_data);
  /* it is impossible to correctly resolve untyped pointer match so we ignore it */
  mr_add_child (parent, idx, &mr_save_data->ptrs);

  /* route saving handler */
  if ((fdp->mr_type_ext < MR_TYPE_EXT_LAST) && ext_mr_save_handler[fdp->mr_type_ext])
    ext_mr_save_handler[fdp->mr_type_ext] (mr_save_data);
  else if ((fdp->mr_type < MR_TYPE_LAST) && mr_save_handler[fdp->mr_type])
    mr_save_handler[fdp->mr_type] (mr_save_data);
}

/**
 * MR_TYPE_FUNC & MR_TYPE_FUNC_TYPE type saving handler. Detects NULL pointers.
 * @param mr_save_data save routines data and lookup structures
 */
static void
mr_save_func (mr_save_data_t * mr_save_data)
{
  int idx = mr_save_data->ptrs.size / sizeof (mr_save_data->ptrs.ra[0]) - 1;
  if (NULL == *(void**)mr_save_data->ptrs.ra[idx].data)
    mr_save_data->ptrs.ra[idx].flags.is_null = TRUE;
}

/**
 * MR_STRING type saving handler. Saves string as internal representation tree node.
 * Detects if string content was already saved.
 * @param mr_save_data save routines data and lookup structures
 */
static void
mr_save_string (mr_save_data_t * mr_save_data)
{
  int idx = mr_save_data->ptrs.size / sizeof (mr_save_data->ptrs.ra[0]) - 1;
  char * str = *(char**)mr_save_data->ptrs.ra[idx].data;
  if (NULL == str)
    mr_save_data->ptrs.ra[idx].flags.is_null = TRUE;
  else
    {
      static mr_fd_t fd_ = {
	.mr_type = MR_TYPE_CHAR_ARRAY,
	.mr_type_ext = MR_TYPE_EXT_NONE,
      };
      int ref_idx = mr_check_ptr_in_list (mr_save_data, str, &fd_);
      if (ref_idx < 0)
	mr_save_inner (str, &fd_, mr_save_data, idx);
      else
	{
	  mr_save_data->ptrs.ra[idx].ref_idx = ref_idx;
	  mr_save_data->ptrs.ra[ref_idx].flags.is_referenced = TRUE;
	}
    }
}

/**
 * MR_STRUCT type saving handler. Saves structure as internal representation tree node.
 * @param mr_save_data save routines data and lookup structures
 */
static void
mr_save_struct (mr_save_data_t * mr_save_data)
{
  int idx = mr_save_data->ptrs.size / sizeof (mr_save_data->ptrs.ra[0]) - 1;
  mr_td_t * tdp = mr_get_td_by_name (mr_save_data->ptrs.ra[idx].fd.type);
  char * data = mr_save_data->ptrs.ra[idx].data;
  int i, count;

  if (NULL == tdp) /* check whether type descriptor was found */
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_NO_TYPE_DESCRIPTOR, mr_save_data->ptrs.ra[idx].fd.type);
      return;
    }

  if (tdp->mr_type != MR_TYPE_STRUCT)
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_TYPE_NOT_STRUCT, tdp->type.str);
      return;
    }

  count = tdp->fields_size / sizeof (tdp->fields[0]);
  for (i = 0; i < count; ++i)
    {
      mr_fd_t * fdp = tdp->fields[i].fdp;
      mr_save_inner (&data[fdp->offset], fdp, mr_save_data, idx); /* add each child to this node */
    }
}

static mr_fd_t *
mr_union_discriminator_by_idx (mr_td_t * tdp, int idx)
{
  /* check that field index in union is valid and reset to default otherwise */
  if ((idx < 0) || (idx >= tdp->fields_size / sizeof (tdp->fields[0])))
    idx = 0;
  if (tdp->fields_size > 0) /* check for an empty union */
    return (tdp->fields[idx].fdp);
  else
    return (NULL);
}

static mr_fd_t *
mr_union_discriminator_by_name (mr_td_t * tdp, char * name)
{
  if (name && name[0])
    {
      mr_fd_t * fdp = mr_get_fd_by_name (tdp, name);
      if (NULL != fdp)
	return (fdp);
    }
  if (tdp->fields_size > 0) /* check for an empty union */
    return (tdp->fields[0].fdp);
  else
    return (NULL);
}

static mr_fd_t *
mr_union_discriminator_by_type (mr_td_t * tdp, mr_fd_t * parent_fdp, void * discriminator)
{
  /* switch over basic types */
  if (discriminator)
    switch (parent_fdp->mr_type)
      {
      case MR_TYPE_BOOL:
	return (mr_union_discriminator_by_idx (tdp, *(bool*)discriminator));
      case MR_TYPE_UINT8:
	return (mr_union_discriminator_by_idx (tdp, *(uint8_t*)discriminator));
      case MR_TYPE_INT8:
	return (mr_union_discriminator_by_idx (tdp, *(int8_t*)discriminator));
      case MR_TYPE_UINT16:
	return (mr_union_discriminator_by_idx (tdp, *(uint16_t*)discriminator));
      case MR_TYPE_INT16:
	return (mr_union_discriminator_by_idx (tdp, *(int16_t*)discriminator));
      case MR_TYPE_UINT32:
	return (mr_union_discriminator_by_idx (tdp, *(uint32_t*)discriminator));
      case MR_TYPE_INT32:
	return (mr_union_discriminator_by_idx (tdp, *(int32_t*)discriminator));
      case MR_TYPE_UINT64:
	return (mr_union_discriminator_by_idx (tdp, *(uint64_t*)discriminator));
      case MR_TYPE_INT64:
	return (mr_union_discriminator_by_idx (tdp, *(int64_t*)discriminator));
      case MR_TYPE_BITFIELD:
	{
	  uint64_t value = 0;
	  mr_ptrdes_t ptrdes = { .data = discriminator, .fd = *parent_fdp, };
	  mr_td_t * enum_tdp = mr_get_td_by_name (parent_fdp->type);
	  mr_save_bitfield_value (&ptrdes, &value); /* get value of the bitfield */
	  if (enum_tdp && (MR_TYPE_ENUM == enum_tdp->mr_type))
	    {
	      /* if bitfield is a enumeration then get named discriminator from enum value meta field */
	      mr_fd_t * enum_fdp = mr_get_enum_by_value (enum_tdp, value);
	      return (mr_union_discriminator_by_name (tdp, enum_fdp ? enum_fdp->meta : NULL));
	    }
	  else
	    return (mr_union_discriminator_by_idx (tdp, value));
	}
      case MR_TYPE_CHAR_ARRAY:
	return (mr_union_discriminator_by_name (tdp, (char*)discriminator));
      case MR_TYPE_STRING:
	return (mr_union_discriminator_by_name (tdp, *(char**)discriminator));
      case MR_TYPE_ENUM:
	{
	  mr_td_t * enum_tdp = mr_get_td_by_name (parent_fdp->type);
	  if (enum_tdp && (MR_TYPE_ENUM == enum_tdp->mr_type))
	    {
	      int64_t value = mr_get_enum_value (enum_tdp, discriminator);
	      mr_fd_t * enum_fdp = mr_get_enum_by_value (enum_tdp, value); /* get named discriminator from enum value meta field */
	      return (mr_union_discriminator_by_name (tdp, enum_fdp ? enum_fdp->meta : NULL));
	    }
	  break;
	}
      default:
	break;
      }
  return (mr_union_discriminator_by_name (tdp, NULL));
}

/**
 * Finds out union discriminator
 * @param mr_save_data save routines data and lookup structures
 */
static mr_fd_t *
mr_union_discriminator (mr_save_data_t * mr_save_data)
{
  mr_fd_t * fdp = NULL; /* marker that no valid discriminator was found */
  int idx = mr_save_data->ptrs.size / sizeof (mr_save_data->ptrs.ra[0]) - 1; /* index of the last element - it is union itself */
  int parent;
  mr_ptr_t ud_idx;
  mr_ptr_t * ud_find = NULL;
  mr_union_discriminator_t * ud;
  mr_td_t * tdp = mr_get_td_by_name (mr_save_data->ptrs.ra[idx].fd.type); /* look up for type descriptor */

  /* if union meta field is a valid field name, then traverse thruogh parents and look for union discriminator */
  if (!mr_is_valid_field_name (mr_save_data->ptrs.ra[idx].fd.meta))
    return (mr_union_discriminator_by_name (tdp, NULL));

  ud = mr_rarray_allocate_element ((void*)&mr_save_data->mr_ra_ud,
				   &mr_save_data->mr_ra_ud_size, &mr_save_data->mr_ra_ud_alloc_size,
				   sizeof (mr_save_data->mr_ra_ud[0]));
  /* create a record for further lookups in parent nodes for discriminator value */
  if (NULL == ud)
    return (mr_union_discriminator_by_name (tdp, NULL));

  memset (ud, 0, sizeof (*ud));
  /* this record is only for lookups and there is no guarantee that parents already have union resolution info */
  mr_save_data->mr_ra_ud_size -= sizeof (mr_save_data->mr_ra_ud[0]);
  ud_idx.long_int_t = mr_save_data->mr_ra_ud_size / sizeof (mr_save_data->mr_ra_ud[0]); /* index of lookup record */
  ud->type.str = mr_save_data->ptrs.ra[idx].fd.type; /* union type */
  ud->discriminator.str = mr_save_data->ptrs.ra[idx].fd.meta; /* union discriminator */

  /* traverse through parents up to root node */
  for (parent = mr_save_data->ptrs.ra[idx].parent; parent >= 0; parent = mr_save_data->ptrs.ra[parent].parent)
    if (MR_TYPE_EXT_NONE == mr_save_data->ptrs.ra[parent].fd.mr_type_ext)
      {
	mr_td_t * parent_tdp;
	mr_fd_t * parent_fdp;
	void * discriminator;

	/* checks if this parent already have union resolution info */
	ud_find = mr_ic_find (&mr_save_data->ptrs.ra[parent].union_discriminator, ud_idx, mr_save_data);
	/* break the traverse loop if it has */
	if (ud_find)
	  break;
	/* otherwise get type descriptor for this parent */
	parent_tdp = mr_get_td_by_name (mr_save_data->ptrs.ra[parent].fd.type);
	if (NULL == parent_tdp)
	  continue;
	/* lookup for a discriminator field in this parent */
	parent_fdp = mr_get_fd_by_name (parent_tdp, mr_save_data->ptrs.ra[idx].fd.meta);
	if (NULL == parent_fdp)
	  continue; /* continue traverse if it doesn't */

	/* check that this parent is of valid type - structure, union or a pointer to something */
	if ((MR_TYPE_EXT_NONE != parent_fdp->mr_type_ext) && (MR_TYPE_EXT_POINTER != parent_fdp->mr_type_ext))
	  continue;

	/* get an address of discriminator field */
	discriminator = (char*)mr_save_data->ptrs.ra[parent].data + parent_fdp->offset;

	/* if discriminator is a pointer then we need address of the content */
	if (MR_TYPE_EXT_POINTER == parent_fdp->mr_type_ext)
	  discriminator = *(void**)discriminator;

	fdp = mr_union_discriminator_by_type (tdp, parent_fdp, discriminator);
	break;
      }

  if (NULL != ud_find)
    fdp = mr_save_data->mr_ra_ud[ud_find->long_int_t].fdp; /* union discriminator info was found in some of the parents */
  else
    {
      /* union discriminator info was not found in parents so we add new record */
      mr_save_data->mr_ra_ud_size += sizeof (mr_save_data->mr_ra_ud[0]);
      ud->fdp = fdp;
      ud_find = &ud_idx;
    }

  /* add union discriminator information to all parents wchich doesn't have it yet */
  for (idx = mr_save_data->ptrs.ra[idx].parent; idx != parent; idx = mr_save_data->ptrs.ra[idx].parent)
    if (MR_TYPE_EXT_NONE == mr_save_data->ptrs.ra[idx].fd.mr_type_ext)
      if (NULL == mr_ic_add (&mr_save_data->ptrs.ra[idx].union_discriminator, *ud_find, mr_save_data))
	break;

  return (fdp ? fdp : mr_union_discriminator_by_name (tdp, NULL)); /* fdp might be NULL */
}

/**
 * MR_UNION type saving handler. Saves structure as internal representation tree node.
 * @param mr_save_data save routines data and lookup structures
 */
static void
mr_save_union (mr_save_data_t * mr_save_data)
{
  int idx = mr_save_data->ptrs.size / sizeof (mr_save_data->ptrs.ra[0]) - 1;
  char * data = mr_save_data->ptrs.ra[idx].data;
  mr_td_t * tdp = mr_get_td_by_name (mr_save_data->ptrs.ra[idx].fd.type); /* look up for type descriptor */
  mr_fd_t * fdp;

  if (NULL == tdp) /* check whether type descriptor was found */
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_NO_TYPE_DESCRIPTOR, mr_save_data->ptrs.ra[idx].fd.type);
      return;
    }
  if ((tdp->mr_type != MR_TYPE_UNION) && (tdp->mr_type != MR_TYPE_ANON_UNION) && (tdp->mr_type != MR_TYPE_NAMED_ANON_UNION))
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_TYPE_NOT_UNION, tdp->type.str);
      return;
    }

  fdp = mr_union_discriminator (mr_save_data);

  if (NULL != fdp)
    mr_save_inner (&data[fdp->offset], fdp, mr_save_data, idx);
}

/**
 * MR_ARRAY type saving handler. Saves array into internal representation.
 * @param mr_save_data save routines data and lookup structures
 */
static void
mr_save_array (mr_save_data_t * mr_save_data)
{
  int idx = mr_save_data->ptrs.size / sizeof (mr_save_data->ptrs.ra[0]) - 1;
  char * data = mr_save_data->ptrs.ra[idx].data;
  mr_fd_t fd_ = mr_save_data->ptrs.ra[idx].fd;
  int row_count = fd_.param.array_param.row_count;
  int count = fd_.param.array_param.count;
  int i;

  fd_.unnamed = TRUE;
  if (1 == fd_.param.array_param.row_count)
    fd_.mr_type_ext = MR_TYPE_EXT_NONE; /* set extended type property to MR_NONE in copy of field descriptor */
  else
    {
      fd_.param.array_param.count = row_count;
      fd_.param.array_param.row_count = 1;
    }
  /* add each array element to this node */
  for (i = 0; i < count; i += row_count)
    mr_save_inner (data + i * fd_.size, &fd_, mr_save_data, idx);
}

/**
 * Saves pointer into internal representation.
 * @param idx node index
 * @param mr_save_data save routines data and lookup structures
 */
static void
mr_save_pointer_content (int idx, mr_save_data_t * mr_save_data)
{
  char ** data = mr_save_data->ptrs.ra[idx].data;
  int count = 1;

  if (mr_save_data->ptrs.ra[idx].fd.size != 0)
    count = mr_save_data->ptrs.ra[idx].size / mr_save_data->ptrs.ra[idx].fd.size;
  
  /* add each array element to this node */
  if (count <= 0)
    mr_save_data->ptrs.ra[idx].flags.is_null = TRUE; /* return empty node if pointer is NULL */
  else
    {
      int i;
      mr_fd_t fd_ = mr_save_data->ptrs.ra[idx].fd;
      fd_.mr_type_ext = MR_TYPE_EXT_NONE;
      fd_.unnamed = TRUE;
      
      for (i = 0; i < count; ++i)
	mr_save_inner (*data + i * fd_.size, &fd_, mr_save_data, idx);
    }
}

/**
 * Check if pointer points on data that could be saved and was not saved before
 * @param postpone flag for postponed saving
 * @param idx node index
 * @param mr_save_data save routines data and lookup structures
 */
static void
mr_save_pointer_postponed (int postpone, int idx, mr_save_data_t * mr_save_data)
{
  void ** data = mr_save_data->ptrs.ra[idx].data;

  if (NULL == *data)
    mr_save_data->ptrs.ra[idx].flags.is_null = TRUE; /* return empty node if pointer is NULL */
  else
    {
      int ref_idx;
      mr_type_t mr_type = mr_save_data->ptrs.ra[idx].fd.mr_type;
      mr_fd_t fd_ = mr_save_data->ptrs.ra[idx].fd;

      if (postpone)
	{
	  mr_ptrdes_t src, dst;
	  /* at first attempt to save pointer we need need to determine size of structure */
	  if ((mr_type != MR_TYPE_NONE) && (mr_type != MR_TYPE_VOID))
	    /* for serializable types we take size of the type as default value */
	    mr_save_data->ptrs.ra[idx].size = mr_save_data->ptrs.ra[idx].fd.size;
	  else
	    /* unserializable types will have zero size */
	    mr_save_data->ptrs.ra[idx].size = 0;
	  
	  /* pointers might have assosiated field with the size for resizable arrays.
	     name of the size field is stored in 'res' of field meta-data.
	     'res_type' in this case will be 'char' */

	  mr_pointer_get_size_ptrdes (&src, idx, &mr_save_data->ptrs);
	  if (src.data != NULL)
	    {
	      dst.data = &mr_save_data->ptrs.ra[idx].size;
	      dst.fd.mr_type = MR_TYPE_DETECT (__typeof__ (mr_save_data->ptrs.ra[idx].size));
	      dst.fd.mr_type_ext = MR_TYPE_EXT_NONE;
	      mr_assign_int (&dst, &src);
	    }
	}
      
      /* set extended type property to NONE in copy of field descriptor */
      fd_.mr_type_ext = MR_TYPE_EXT_NONE;
      /* check if this pointer is already saved */
      ref_idx = mr_check_ptr_in_list (mr_save_data, *data, &fd_);
      
      if (ref_idx >= 0)
	{
	  mr_save_data->ptrs.ra[idx].ref_idx = ref_idx;
	  mr_save_data->ptrs.ra[ref_idx].flags.is_referenced = TRUE;
	}
      else if (0 == strcmp (mr_save_data->ptrs.ra[idx].fd.name.str, MR_OPAQUE_DATA_STR))
	{
	  if (mr_save_data->ptrs.ra[idx].size <= 0)
	    mr_save_data->ptrs.ra[idx].flags.is_null = TRUE;
	  else
	    mr_save_data->ptrs.ra[idx].flags.is_opaque_data = TRUE;
	}
      else if ((mr_type != MR_TYPE_NONE) && (mr_type != MR_TYPE_VOID)) /* look ahead optimization for void pointers */
	{
	  if (postpone)
	    {
	      int * idx_ = mr_rarray_allocate_element ((void*)&mr_save_data->mr_ra_idx,
						       &mr_save_data->mr_ra_idx_size, &mr_save_data->mr_ra_idx_alloc_size, 
						       sizeof (mr_save_data->mr_ra_idx[0]));
	      if (NULL == idx_)
		return;
	      *idx_ = idx;
	    }
	  else
	    mr_save_pointer_content (idx, mr_save_data);
	}
    }
}

mr_status_t
mr_ptrs_ds (mr_ra_mr_ptrdes_t * ptrs, mr_ptrdes_processor_t processor, void * context)
{
  int idx = 0;
  while (idx >= 0)
    {
      if (MR_SUCCESS != processor (ptrs, idx, context))
	return (MR_FAILURE);

      if (ptrs->ra[idx].first_child >= 0)
	idx = ptrs->ra[idx].first_child;
      else
	{
	  while ((ptrs->ra[idx].next < 0) && (ptrs->ra[idx].parent >= 0))
	    idx = ptrs->ra[idx].parent;
	  idx = ptrs->ra[idx].next;
	}
    }
  return (MR_SUCCESS);
}

mr_status_t
mr_renumber_node (mr_ra_mr_ptrdes_t * ptrs, int idx, void * context)
{
  int * idx_ = context;
  ptrs->ra[idx].idx = (*idx_)++;
  return (MR_SUCCESS);
}

static mr_status_t
mr_post_process_node  (mr_ra_mr_ptrdes_t * ptrs, int idx, void * context)
{
  mr_save_data_t * mr_save_data = context;
  int parent = mr_save_data->ptrs.ra[idx].parent;

  if (parent < 0)
    mr_save_data->ptrs.ra[idx].level = 0;
  else
    mr_save_data->ptrs.ra[idx].level = mr_save_data->ptrs.ra[parent].level + 1;

  if ((MR_TYPE_EXT_POINTER == mr_save_data->ptrs.ra[idx].fd.mr_type_ext) &&
      ((MR_TYPE_NONE == mr_save_data->ptrs.ra[idx].fd.mr_type) || (MR_TYPE_VOID == mr_save_data->ptrs.ra[idx].fd.mr_type)) &&
      (mr_save_data->ptrs.ra[idx].ref_idx < 0) &&
      (NULL != *(void**)mr_save_data->ptrs.ra[idx].data))
    {
      static mr_fd_t fd_ = {
	.mr_type = MR_TYPE_NONE,
	.mr_type_ext = MR_TYPE_EXT_NONE,
      };
      int ref_idx = mr_check_ptr_in_list (mr_save_data, *(void**)mr_save_data->ptrs.ra[idx].data, &fd_);
      if (ref_idx >= 0)
	{
	  mr_save_data->ptrs.ra[idx].ref_idx = ref_idx;
	  mr_save_data->ptrs.ra[ref_idx].flags.is_referenced = TRUE;
	}
      else
	mr_save_data->ptrs.ra[idx].flags.is_null = TRUE; /* unresolved void pointers are saved as NULL */
    }

  if (mr_save_data->ptrs.ra[idx].ref_idx >= 0)
    {
      int ref_parent = mr_save_data->ptrs.ra[mr_save_data->ptrs.ra[idx].ref_idx].parent;
      if ((ref_parent >= 0) && (MR_TYPE_STRING == mr_save_data->ptrs.ra[ref_parent].fd.mr_type)
	  && (MR_TYPE_EXT_NONE == mr_save_data->ptrs.ra[ref_parent].fd.mr_type_ext))
	{
	  mr_save_data->ptrs.ra[idx].ref_idx = ref_parent;
	  mr_save_data->ptrs.ra[idx].flags.is_content_reference = TRUE;
	  mr_save_data->ptrs.ra[ref_parent].flags.is_referenced = TRUE;
	}
    }

  if ((MR_TYPE_STRING == mr_save_data->ptrs.ra[idx].fd.mr_type) &&
      (MR_TYPE_EXT_NONE == mr_save_data->ptrs.ra[idx].fd.mr_type_ext))
    mr_save_data->ptrs.ra[idx].first_child = mr_save_data->ptrs.ra[idx].last_child = -1;

  return (MR_SUCCESS);
}

/**
 * Set indexes to nodes according saving sequence.
 * @param mr_save_data save routines data and lookup structures
 */
static void
mr_post_process (mr_save_data_t * mr_save_data)
{
  int idx_ = 0;
  mr_ptrs_ds (&mr_save_data->ptrs, mr_post_process_node, mr_save_data);
  mr_ptrs_ds (&mr_save_data->ptrs, mr_renumber_node, &idx_); /* enumeration of nodes should be done only after strings processing */
}

/**
 * MR_POINTER_STRUCT type saving handler. Save referenced structure into internal representation.
 * @param mr_save_data save routines data and lookup structures
 */
static void
mr_save_pointer (mr_save_data_t * mr_save_data)
{
  mr_save_pointer_postponed (!0, mr_save_data->ptrs.size / sizeof (mr_save_data->ptrs.ra[0]) - 1, mr_save_data);
}

/**
 * Public function. Calls save scheduler and frees lookup tables.
 * @param data a pointer on data
 * @param fdp a ponter of field descriptor
 * @param mr_save_data save routines data and lookup structures
 */
void
mr_save (void * data, mr_fd_t * fdp, mr_save_data_t * mr_save_data)
{
  int i;

  memset (mr_save_data, 0, sizeof (*mr_save_data));
  mr_ic_new (&mr_save_data->typed_ptrs, mr_typed_ptrdes_get_hash, mr_typed_ptrdes_cmp, "long_int_t", MR_IC_DYNAMIC_DEFAULT);
  mr_ic_new (&mr_save_data->untyped_ptrs, mr_untyped_ptrdes_get_hash, mr_untyped_ptrdes_cmp, "long_int_t", MR_IC_DYNAMIC_DEFAULT);

  mr_save_data->mr_ra_ud_size = 0;
  mr_save_data->mr_ra_ud = NULL;
  mr_save_data->mr_ra_idx_size = 0;
  mr_save_data->mr_ra_idx = NULL;
  mr_save_data->ptrs.size = 0;
  mr_save_data->ptrs.ra = NULL;

  fdp->unnamed = TRUE;
  
  mr_save_inner (data, fdp, mr_save_data, -1);

  while (mr_save_data->mr_ra_idx_size > 0)
    {
      mr_save_data->mr_ra_idx_size -= sizeof (mr_save_data->mr_ra_idx[0]);
      mr_save_pointer_postponed (0, mr_save_data->mr_ra_idx[mr_save_data->mr_ra_idx_size / sizeof (mr_save_data->mr_ra_idx[0])],
				 mr_save_data);
    }
  mr_post_process (mr_save_data);

  for (i = mr_save_data->ptrs.size / sizeof (mr_save_data->ptrs.ra[0]) - 1; i >= 0; --i)
    mr_ic_free (&mr_save_data->ptrs.ra[i].union_discriminator);

  if (mr_save_data->mr_ra_ud != NULL)
    MR_FREE (mr_save_data->mr_ra_ud);
  mr_save_data->mr_ra_ud = NULL;
  
  if (mr_save_data->mr_ra_idx != NULL)
    MR_FREE (mr_save_data->mr_ra_idx);
  mr_save_data->mr_ra_idx = NULL;

  mr_ic_free (&mr_save_data->typed_ptrs);
  mr_ic_free (&mr_save_data->untyped_ptrs);
}

/**
 * Init IO handlers Table
 */
static mr_save_handler_t mr_save_handler[] =
  {
    [MR_TYPE_FUNC] = mr_save_func,
    [MR_TYPE_FUNC_TYPE] = mr_save_func,
    [MR_TYPE_STRING] = mr_save_string,
    [MR_TYPE_STRUCT] = mr_save_struct,
    [MR_TYPE_UNION] = mr_save_union,
    [MR_TYPE_ANON_UNION] = mr_save_union,
    [MR_TYPE_NAMED_ANON_UNION] = mr_save_union,
  };

static mr_save_handler_t ext_mr_save_handler[] =
  {
    [MR_TYPE_EXT_ARRAY] = mr_save_array,
    [MR_TYPE_EXT_POINTER] = mr_save_pointer,
  };
