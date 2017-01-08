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

/* MR_IC_RBTREE    ( / 372146 1671.0) ratio: 222.70 */
/* MR_IC_HASH_NEXT ( / 139148 1671.0) ratio: 83.27 */
/* MR_IC_HASH_TREE  ( / 28611 1671.0) ratio: 17.12 */

#define MR_IC_DYNAMIC_DEFAULT MR_IC_HASH_TREE

TYPEDEF_FUNC (void, mr_save_handler_t, (mr_save_data_t *))

static mr_save_handler_t mr_save_handler[];
static mr_save_handler_t ext_mr_save_handler[];

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
	  mr_ptrdes_t ptrdes = { .data.ptr = discriminator, .fd = *parent_fdp, };
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

static mr_fd_t *
mr_node_get_discriminator_fd (mr_save_data_t * mr_save_data, int node, char * discriminator)
{
  /* get type descriptor for this node */
  mr_td_t * tdp = mr_get_td_by_name (mr_save_data->ptrs.ra[node].fd.type);
  if (NULL == tdp)
    return (NULL);
  /* lookup for a discriminator field in this parent */
  mr_fd_t * fdp = mr_get_fd_by_name (tdp, discriminator);
  if (NULL == fdp)
    return (NULL);

  /* check that this field is of valid type - structure, union or a pointer to something */
  if ((MR_TYPE_EXT_NONE != fdp->mr_type_ext) && (MR_TYPE_EXT_POINTER != fdp->mr_type_ext))
    return (NULL);

  return (fdp);
}

/**
 * Finds out union discriminator
 * @param mr_save_data save routines data and lookup structures
 */
static mr_fd_t *
mr_union_discriminator (mr_save_data_t * mr_save_data, int node, char * union_type, char * discriminator)
{
  mr_fd_t * fdp = NULL; /* marker that no valid discriminator was found */
  int parent, idx;
  mr_ptr_t ud_idx;
  mr_ptr_t * ud_find = NULL;
  mr_union_discriminator_t * ud;
  mr_td_t * tdp = mr_get_td_by_name (union_type); /* look up for type descriptor */

  if (NULL == tdp)
    return (NULL);

  /* if union meta field is a valid field name, then traverse thruogh parents and look for union discriminator */
  if (!mr_is_valid_field_name (discriminator))
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
  ud->type.str = union_type; /* union type */
  ud->discriminator.str = discriminator; /* union discriminator */

  /* traverse through parents up to root node */
  for (parent = node; parent >= 0; parent = mr_save_data->ptrs.ra[parent].parent)
    if (MR_TYPE_EXT_NONE == mr_save_data->ptrs.ra[parent].fd.mr_type_ext)
      {
	mr_fd_t * parent_fdp;
	void * discriminator_ptr;

	/* checks if this parent already have union resolution info */
	ud_find = mr_ic_find (&mr_save_data->ptrs.ra[parent].union_discriminator, ud_idx, mr_save_data);
	/* break the traverse loop if it has */
	if (ud_find)
	  break;

	parent_fdp = mr_node_get_discriminator_fd (mr_save_data, parent, ud->discriminator.str);
	if (NULL == parent_fdp)
	  continue;
	/* get an address of discriminator field */
	discriminator_ptr = (char*)mr_save_data->ptrs.ra[parent].data.ptr + parent_fdp->offset;

	/* if discriminator is a pointer then we need address of the content */
	if (MR_TYPE_EXT_POINTER == parent_fdp->mr_type_ext)
	  discriminator_ptr = *(void**)discriminator_ptr;

	fdp = mr_union_discriminator_by_type (tdp, parent_fdp, discriminator_ptr);
	break;
      }

  if (NULL != ud_find)
    fdp = mr_save_data->mr_ra_ud[ud_find->long_int_t].fdp; /* union discriminator info was found in some of the parents */
  else
    {
      if (NULL == fdp)
	fdp = mr_union_discriminator_by_name (tdp, NULL);
      /* union discriminator info was not found in parents so we add new record */
      mr_save_data->mr_ra_ud_size += sizeof (mr_save_data->mr_ra_ud[0]);
      ud->fdp = fdp;
      ud_find = &ud_idx;
    }

  /* add union discriminator information to all parents wchich doesn't have it yet */
  for (idx = node; idx != parent; idx = mr_save_data->ptrs.ra[idx].parent)
    if (MR_TYPE_EXT_NONE == mr_save_data->ptrs.ra[idx].fd.mr_type_ext)
      if (NULL == mr_ic_add (&mr_save_data->ptrs.ra[idx].union_discriminator, *ud_find, mr_save_data))
	break;

  return (fdp);
}

/**
 * Comparator for mr_ptrdes_t.
 * @param x value to compare
 * @param y value to compare
 * @return comparation sign
 */
static int
mr_cmp_ptrdes (mr_ptrdes_t * x, mr_ptrdes_t * y)
{
  int diff = x->data.ptr - y->data.ptr;
  if (diff)
    return (diff);
  
  diff = x->fd.mr_type_ext - y->fd.mr_type_ext;
  if (diff)
    return (diff);

  if (MR_TYPE_EXT_ARRAY == x->fd.mr_type_ext)
    {
      diff = x->fd.param.array_param.count - y->fd.param.array_param.count;
      if (diff)
	return (diff);
      diff = x->fd.param.array_param.row_count - y->fd.param.array_param.row_count;
      if (diff)
	return (diff);
    }
  
  diff = x->fd.mr_type - y->fd.mr_type;
  if (diff)
    return (diff);

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
    case MR_TYPE_COMPLEX_FLOAT:
    case MR_TYPE_DOUBLE:
    case MR_TYPE_COMPLEX_DOUBLE:
    case MR_TYPE_LONG_DOUBLE:
    case MR_TYPE_COMPLEX_LONG_DOUBLE:
      break;
    case MR_TYPE_BITFIELD:
      diff = x->fd.param.bitfield_param.shift - y->fd.param.bitfield_param.shift;
      if (diff)
	return (diff);
      diff = x->fd.param.bitfield_param.width - y->fd.param.bitfield_param.width;
      if (diff)
	return (diff);
      break;
    case MR_TYPE_STRUCT:
    case MR_TYPE_BITMASK:
    case MR_TYPE_FUNC_TYPE:
    case MR_TYPE_FUNC:
    case MR_TYPE_ENUM:
    case MR_TYPE_UNION:
    case MR_TYPE_ANON_UNION:
    case MR_TYPE_NAMED_ANON_UNION:
      diff = strcmp (x->fd.type, y->fd.type);
      if (diff)
	return (diff);
      break;
    case MR_TYPE_ENUM_VALUE:
    case MR_TYPE_END_ANON_UNION:
    case MR_TYPE_TRAILING_RECORD:
    case MR_TYPE_LAST:
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_UNEXPECTED_MR_TYPE);
      break;
    }
  return (0);
}

mr_hash_value_t __attribute__ ((unused))
mr_typed_ptrdes_get_hash (const mr_ptr_t x, const void * context)
{
  const mr_ra_ptrdes_t * ptrs = context;
  const mr_ptrdes_t * ptrdes = &ptrs->ra[x.long_int_t];
  return ((long)ptrdes->data.ptr + ptrdes->fd.mr_type + ptrdes->fd.mr_type_ext * MR_TYPE_LAST);
}

int
mr_typed_ptrdes_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const mr_ra_ptrdes_t * ptrs = context;
  return (mr_cmp_ptrdes (&ptrs->ra[x.long_int_t], &ptrs->ra[y.long_int_t]));
}

mr_hash_value_t __attribute__ ((unused))
mr_untyped_ptrdes_get_hash (const mr_ptr_t x, const void * context)
{
  const mr_ra_ptrdes_t * ptrs = context;
  return ((long)ptrs->ra[x.long_int_t].data.ptr);
}

int
mr_untyped_ptrdes_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const mr_ra_ptrdes_t * ptrs = context;
  return (ptrs->ra[x.long_int_t].data.ptr - ptrs->ra[y.long_int_t].data.ptr);
}

TYPEDEF_STRUCT (mr_check_ud_ctx_t,
		(mr_save_data_t *, mr_save_data),
		int node,
		int parent,
		)
/**
 * We have a previously saved node that was a pointer,
 * but now we need to save the same pointer as a part of bigger structure. This means that
 * we need to relink content of the pointer to new place and set a reference on it.
 * Types of previously saved node matches with the type of node that we need to save.
 * Nevertheless there migth be child unions that were resolved dynamically in the context of
 * previously saved pointer. Context of new parent might be different and we need to double check
 * resolution of all unions. This function is a visitor to collection of union discriminators.
 * @return MR_SUCCESS if union discriminator resolves to the same branch of union, and MR_FAILURE otherwise
 */
static mr_status_t
mr_check_ud (mr_ptr_t key, const void * context)
{
  const mr_check_ud_ctx_t * mr_check_ud_ctx = context;
  mr_save_data_t * mr_save_data = mr_check_ud_ctx->mr_save_data;
  mr_union_discriminator_t * ud = &mr_save_data->mr_ra_ud[key.long_int_t];
  /* mr_ra_ud would be reallocaed within this function, so we need to get values from this node */
  char * discriminator = ud->discriminator.str;
  char * type = ud->type.str;
  mr_fd_t * ud_fdp = ud->fdp;
  mr_fd_t * fdp = mr_node_get_discriminator_fd (mr_save_data, mr_check_ud_ctx->node, discriminator);

  /* here we check that union discriminator was resolved at the level of saved node.
     This means that resolution is not dependant on upper tree */
  if (NULL != fdp)
    return (MR_SUCCESS);
  /* otherwise we need to find union resolution in the context of new parent */
  fdp = mr_union_discriminator (mr_save_data, mr_check_ud_ctx->parent, type, discriminator);
  
  return ((fdp == ud_fdp) ? MR_SUCCESS : MR_FAILURE);
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
 * @return an index of node in collection of already saved nodes
 */
static int
mr_check_ptr_in_list (mr_save_data_t * mr_save_data, void * data)
{
  mr_ra_ptrdes_t * ptrs = &mr_save_data->ptrs;
  mr_ptr_t * find_result;
  long_int_t idx = mr_add_ptr_to_list (ptrs);

  if (idx < 0)
    return (idx); /* memory allocation error occured */

  /* populate attributes of new node */
  ptrs->ra[idx].data.ptr = data;

  /* this element is required only for a search so we need to adjust back size of collection */
  ptrs->size -= sizeof (ptrs->ra[0]);

  /* search in index of typed references */
  find_result = mr_ic_find (&mr_save_data->untyped_ptrs, idx, ptrs);
  
  return (find_result ? find_result->long_int_t : -1);
}

static mr_status_t
register_type_name (mr_ptr_t key, const void * context)
{
  mr_td_t * tdp = key.ptr;
  char ** type_name = (void*)context;
  type_name[tdp->mr_type] = tdp->type.str;
  return (MR_SUCCESS);
}

static bool
ref_is_parent (mr_ptrdes_t * ra, int node, int ref_idx)
{
  while (node >= 0)
    {
      if (node == ref_idx)
	return (TRUE);
      node = ra[node].parent;
    }
  return (FALSE);
}

/**
 * Save scheduler. Save any object into internal representation.
 * @param data a pointer on data
 * @param fdp a ponter of field descriptor
 * @param mr_save_data save routines data and lookup structures
 * @param parent index of parent node
 */
static void
mr_save_inner (void * data, mr_fd_t * fdp, mr_save_data_t * mr_save_data, int parent)
{
  static char * type_name[] = { [0 ... MR_TYPE_LAST - 1] = NULL };
  static bool initialized = FALSE;
  
  long_int_t idx = mr_add_ptr_to_list (&mr_save_data->ptrs); /* add pointer on saving structure to list ptrs */
  mr_ptrdes_t * ra = mr_save_data->ptrs.ra;
  if (idx < 0)
    return; /* memory allocation error occured */

  ra[idx].data.ptr = data;
  ra[idx].fd = *fdp;
  ra[idx].next_typed = -1;
  ra[idx].next_untyped = -1;

  if (!initialized)
    {
      mr_ic_foreach (&mr_conf.lookup_by_name, register_type_name, type_name);
      type_name[MR_TYPE_STRUCT] = NULL;
      type_name[MR_TYPE_ENUM] = NULL;
      type_name[MR_TYPE_UNION] = NULL;
      type_name[MR_TYPE_FUNC] = NULL;
      type_name[MR_TYPE_FUNC_TYPE] = NULL;
      initialized = TRUE;
    }

  if (MR_TYPE_EXT_NONE == fdp->mr_type_ext)
    {
      if ((MR_TYPE_STRUCT == fdp->mr_type) || (MR_TYPE_ENUM == fdp->mr_type))
	ra[idx].type = fdp->type;
      else
	ra[idx].type = type_name[fdp->mr_type];
    }
  
  if (MR_TYPE_EXT_ARRAY == fdp->mr_type_ext)
    ra[idx].MR_SIZE = fdp->size * fdp->param.array_param.count;
  else
    ra[idx].MR_SIZE = fdp->size;
  
  /* forward reference resolving */
  mr_ptr_t * search_result = mr_ic_add (&mr_save_data->typed_ptrs, idx, &mr_save_data->ptrs);
  if ((NULL != search_result) && (search_result->long_int_t != idx))
    {
      long_int_t ref_idx;
      for (ref_idx = search_result->long_int_t; ref_idx >= 0; ref_idx = ra[ref_idx].next_typed)
	{
	  mr_check_ud_ctx_t mr_check_ud_ctx = {
	    .mr_save_data = mr_save_data,
	    .node = ref_idx,
	    .parent = parent,
	  };
	  
	  mr_status_t status = mr_ic_foreach (&ra[ref_idx].union_discriminator, mr_check_ud, &mr_check_ud_ctx);
	  if (MR_SUCCESS == status)
	    {
	      if ((MR_TYPE_EXT_POINTER == ra[parent].fd.mr_type_ext) ||
		  ((MR_TYPE_EXT_NONE == ra[parent].fd.mr_type_ext) && (MR_TYPE_STRING == ra[parent].fd.mr_type)))
		{
		  ra[parent].ref_idx = ref_idx;
		  ra[ref_idx].flags.is_referenced = TRUE;
		  mr_save_data->ptrs.size -= sizeof (mr_save_data->ptrs.ra[0]);
		  return;
		}
	      else
		{
		  int ref_parent = ra[ref_idx].parent;

		  if ((ref_parent >= 0)
		      && ((MR_TYPE_EXT_POINTER == ra[ref_parent].fd.mr_type_ext) ||
			  ((MR_TYPE_EXT_NONE == ra[ref_parent].fd.mr_type_ext) &&
			   (MR_TYPE_STRING == ra[ref_parent].fd.mr_type)))
		      && !ref_is_parent (ra, parent, ref_idx))
		    {
		      ra[ref_parent].ref_idx = ref_idx;
		      ra[ref_parent].first_child = -1;
		      ra[ref_parent].last_child = -1;
		      ra[ref_idx].flags.is_referenced = TRUE;
		      ra[ref_idx].fd.name = ra[idx].fd.name;
		      ra[ref_idx].fd.unnamed = ra[idx].fd.unnamed;
      
		      mr_add_child (parent, ref_idx, &mr_save_data->ptrs);
		      mr_save_data->ptrs.size -= sizeof (mr_save_data->ptrs.ra[0]);
		      return;
		    }
		}
	    }
	}

      mr_save_data->ptrs.ra[idx].next_typed = search_result->long_int_t;
      search_result->long_int_t = idx;
    }

  search_result = mr_ic_add (&mr_save_data->untyped_ptrs, idx, &mr_save_data->ptrs);
  if ((search_result != NULL) && (search_result->long_int_t != idx))
    {
      if (mr_save_data->ptrs.ra[idx].MR_SIZE > mr_save_data->ptrs.ra[search_result->long_int_t].MR_SIZE)
	{
	  mr_save_data->ptrs.ra[idx].next_untyped = search_result->long_int_t;
	  search_result->long_int_t = idx;
	}
      else
	{
	  mr_save_data->ptrs.ra[idx].next_untyped = mr_save_data->ptrs.ra[search_result->long_int_t].next_untyped;
	  mr_save_data->ptrs.ra[search_result->long_int_t].next_untyped = idx;
	}
    }
  
  mr_ic_new (&ra[idx].union_discriminator, mr_ud_get_hash, mr_ud_cmp, "long_int_t", MR_IC_DYNAMIC_DEFAULT);

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
  if (NULL == *(void**)mr_save_data->ptrs.ra[idx].data.ptr)
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
  char * str = *(char**)mr_save_data->ptrs.ra[idx].data.ptr;
  if (NULL == str)
    mr_save_data->ptrs.ra[idx].flags.is_null = TRUE;
  else
    {
      mr_fd_t fd_ = mr_save_data->ptrs.ra[idx].fd;
      fd_.mr_type = MR_TYPE_CHAR_ARRAY;
      fd_.mr_type_ext = MR_TYPE_EXT_NONE;
      fd_.type = "char";
      mr_save_inner (str, &fd_, mr_save_data, idx);
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
  char * data = mr_save_data->ptrs.ra[idx].data.ptr;
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
      mr_save_inner (&data[fdp->offset], fdp, mr_save_data, idx);
    }
}

/**
 * MR_UNION type saving handler. Saves structure as internal representation tree node.
 * @param mr_save_data save routines data and lookup structures
 */
static void
mr_save_union (mr_save_data_t * mr_save_data)
{
  int idx = mr_save_data->ptrs.size / sizeof (mr_save_data->ptrs.ra[0]) - 1;
  char * data = mr_save_data->ptrs.ra[idx].data.ptr;
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
  
  fdp = mr_union_discriminator (mr_save_data, idx, mr_save_data->ptrs.ra[idx].fd.type, mr_save_data->ptrs.ra[idx].fd.meta);

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
  char * data = mr_save_data->ptrs.ra[idx].data.ptr;
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
  char ** data = mr_save_data->ptrs.ra[idx].data.ptr;
  int count = 1;

  if (mr_save_data->ptrs.ra[idx].fd.size != 0)
    count = mr_save_data->ptrs.ra[idx].MR_SIZE / mr_save_data->ptrs.ra[idx].fd.size;
  
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
  void ** data = mr_save_data->ptrs.ra[idx].data.ptr;

  if (NULL == *data)
    mr_save_data->ptrs.ra[idx].flags.is_null = TRUE; /* return empty node if pointer is NULL */
  else
    {
      mr_type_t mr_type = mr_save_data->ptrs.ra[idx].fd.mr_type;

      if (postpone)
	{
	  mr_ptrdes_t src, dst;
	  /* at first attempt to save pointer we need to determine size of structure */
	  if ((mr_type == MR_TYPE_NONE) || (mr_type == MR_TYPE_VOID))
	    /* unserializable types will have zero size */
	    mr_save_data->ptrs.ra[idx].MR_SIZE = 0;
	  
	  /* pointers might have assosiated field with the size for resizable arrays.
	     name of the size field is stored in 'res' of field meta-data.
	     'res_type' in this case will be 'char' */

	  mr_pointer_get_size_ptrdes (&src, idx, &mr_save_data->ptrs);
	  if (src.data.ptr != NULL)
	    {
	      dst.data.ptr = &mr_save_data->ptrs.ra[idx].MR_SIZE;
	      dst.fd.mr_type = MR_TYPE_DETECT (__typeof__ (mr_save_data->ptrs.ra[idx].MR_SIZE));
	      dst.fd.mr_type_ext = MR_TYPE_EXT_NONE;
	      mr_assign_int (&dst, &src);
	    }
	}
      
      if (0 == strcmp (mr_save_data->ptrs.ra[idx].fd.name.str, MR_OPAQUE_DATA_STR))
	{
	  if (mr_save_data->ptrs.ra[idx].MR_SIZE <= 0)
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
mr_ptrs_ds (mr_ra_ptrdes_t * ptrs, mr_ptrdes_processor_t processor, void * context)
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
mr_renumber_node (mr_ra_ptrdes_t * ptrs, int idx, void * context)
{
  int * idx_ = context;
  ptrs->ra[idx].idx = (*idx_)++;
  return (MR_SUCCESS);
}

static mr_status_t
mr_post_process_node  (mr_ra_ptrdes_t * ptrs, int idx, void * context)
{
  mr_save_data_t * mr_save_data = context;
  int parent = mr_save_data->ptrs.ra[idx].parent;

  if (parent < 0)
    mr_save_data->ptrs.ra[idx].level = 0;
  else
    mr_save_data->ptrs.ra[idx].level = mr_save_data->ptrs.ra[parent].level + 1;

  if ((MR_TYPE_EXT_POINTER == mr_save_data->ptrs.ra[idx].fd.mr_type_ext) &&
      ((MR_TYPE_NONE == mr_save_data->ptrs.ra[idx].fd.mr_type) || (MR_TYPE_VOID == mr_save_data->ptrs.ra[idx].fd.mr_type)) &&
      !mr_save_data->ptrs.ra[idx].flags.is_null)
    {
      int ref_idx = mr_check_ptr_in_list (mr_save_data, *(void**)mr_save_data->ptrs.ra[idx].data.ptr);
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

  return (MR_SUCCESS);
}

static mr_status_t
mr_process_strings  (mr_ra_ptrdes_t * ptrs, int idx, void * context)
{
  mr_save_data_t * mr_save_data = context;
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
  mr_ptrs_ds (&mr_save_data->ptrs, mr_process_strings, mr_save_data);
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
