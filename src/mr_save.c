/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <tsearch.h>
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

/**
 * Dummy stub for tree delete function
 * @param nodep pointer on a tree node
 */
static void dummy_free_func (void * nodep) {};

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

static int
cmp_typed_ptrdes (const void * x, const void * y, const void * context)
{
  const mr_ra_mr_ptrdes_t * ptrs = context;
  return (mr_cmp_ptrdes (&ptrs->ra.data[(long)x], &ptrs->ra.data[(long)y]));
}

static int
cmp_untyped_ptrdes (const void * x, const void * y, const void * context)
{
  const mr_ra_mr_ptrdes_t * ptrs = context;
  return (ptrs->ra.data[(long)x].data - ptrs->ra.data[(long)y].data);
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
  long count = ptrs->ra.size / sizeof (ptrs->ra.data[0]) - 1;
  void * tree_search_result;
  int ref_idx;

  tree_search_result = tsearch ((void*)count, (void*)&mr_save_data->typed_ptrs_tree, cmp_typed_ptrdes, ptrs);
  if (NULL == tree_search_result)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (-1);
    }
  ref_idx = *(long*)tree_search_result;
  if ((ref_idx != count) && (ptrs->ra.data[ref_idx].parent >= 0))
    if (MR_TYPE_EXT_POINTER == ptrs->ra.data[ptrs->ra.data[ref_idx].parent].fd.mr_type_ext)
      return (ref_idx);
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
  long count = ptrs->ra.size / sizeof (ptrs->ra.data[0]) - 1;
  void * data = ptrs->ra.data[count].data;
  int same_ptr = count;
  void * tree_search_result;
  int ref_idx;

  tree_search_result = tsearch ((void*)count, (void*)&mr_save_data->untyped_ptrs_tree, cmp_untyped_ptrdes, ptrs);  
  if (NULL == tree_search_result)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (-1);
    }
  ref_idx = *(long*)tree_search_result;

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
      int parent = ptrs->ra.data[same_ptr].parent;
      if ((parent < 0) || (ptrs->ra.data[parent].data != data))
	break;
      same_ptr = parent;
    }
  /*
    node with the same address was found and it was not a parent structure.
    Found node is a pointer if its parent has mr_type_ext == MR_TYPE_EXT_POINTER
   */
  if ((ref_idx != same_ptr) && (ptrs->ra.data[ref_idx].parent >= 0))
    if (MR_TYPE_EXT_POINTER == ptrs->ra.data[ptrs->ra.data[ref_idx].parent].fd.mr_type_ext)
      return (ref_idx);
  return (-1);
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
  void * tree_find_result;
  long idx_;

  idx_ = mr_add_ptr_to_list (ptrs);
  if (idx_ < 0)
    return (idx_); /* memory allocation error occured */
  ptrs->ra.data[idx_].data = data;
  ptrs->ra.data[idx_].fd = *fdp;
  
  ptrs->ra.size -= sizeof (ptrs->ra.data[0]);
  tree_find_result = tfind ((void*)idx_, (void*)&mr_save_data->typed_ptrs_tree, cmp_typed_ptrdes, ptrs);
  if (tree_find_result)
    return (*(long*)tree_find_result);
  tree_find_result = tfind ((void*)idx_, (void*)&mr_save_data->untyped_ptrs_tree, cmp_untyped_ptrdes, ptrs);
  if (tree_find_result)
    return (*(long*)tree_find_result);
  return (-1);
}

/**
 * Save scheduler. Save any object into internal representation.
 * @param data a pointer on data
 * @param fdp a ponter of field descriptor
 * @param mr_save_data save routines data and lookup structures
 */
static void
mr_save_inner (void * data, mr_fd_t * fdp, mr_save_data_t * mr_save_data)
{
  int idx, ref_idx;

  /* add pointer on saving structure to list ptrs */
  idx = mr_add_ptr_to_list (&mr_save_data->ptrs);
  if (idx < 0)
    return; /* memory allocation error occured */
  mr_save_data->ptrs.ra.data[idx].data = data;
  mr_save_data->ptrs.ra.data[idx].fd = *fdp;
  mr_save_data->ptrs.ra.data[idx].parent = mr_save_data->parent; /* NB: mr_add_child do the same, but also adds links from parent to child. This link is requred for mr_resolve_untyped_forward_ref  */
  
  /* forward reference resolving */
  ref_idx = mr_resolve_typed_forward_ref (mr_save_data);
  if (ref_idx >= 0)
    {
      int parent = mr_save_data->ptrs.ra.data[ref_idx].parent;
      mr_save_data->ptrs.ra.data[parent].ref_idx = ref_idx;
      mr_save_data->ptrs.ra.data[parent].first_child = -1;
      mr_save_data->ptrs.ra.data[parent].last_child = -1;
      mr_save_data->ptrs.ra.data[ref_idx].flags.is_referenced = MR_TRUE;
      mr_save_data->ptrs.ra.data[ref_idx].fd.name = fdp->name;
      mr_add_child (mr_save_data->parent, ref_idx, &mr_save_data->ptrs);
      return;
    }
  ref_idx = mr_resolve_untyped_forward_ref (mr_save_data);
  /* it is impossible to correctly resolve untyped pointer match so we ignore it */
  mr_add_child (mr_save_data->parent, idx, &mr_save_data->ptrs);

  /* route saving handler */
  if ((fdp->mr_type_ext >= 0) && (fdp->mr_type_ext < MR_MAX_TYPES)
      && mr_conf.io_ext_handlers[fdp->mr_type_ext].save.rl)
    mr_conf.io_ext_handlers[fdp->mr_type_ext].save.rl (mr_save_data);
  else if ((fdp->mr_type >= 0) && (fdp->mr_type < MR_MAX_TYPES)
	   && mr_conf.io_handlers[fdp->mr_type].save.rl)
    mr_conf.io_handlers[fdp->mr_type].save.rl (mr_save_data);
}

/**
 * MR_STRING type saving handler. Saves string as internal representation tree node.
 * Detects if string content was already saved.
 * @param mr_save_data save routines data and lookup structures
 */
static void
mr_save_string (mr_save_data_t * mr_save_data)
{
  int idx = mr_save_data->ptrs.ra.size / sizeof (mr_save_data->ptrs.ra.data[0]) - 1;
  char * str = *(char**)mr_save_data->ptrs.ra.data[idx].data;
  if (NULL == str)
    mr_save_data->ptrs.ra.data[idx].flags.is_null = MR_TRUE;
  else
    {
      static mr_fd_t fd_ = {
	.mr_type = MR_TYPE_CHAR_ARRAY,
	.mr_type_ext = MR_TYPE_EXT_NONE,
      };
      int ref_idx = mr_check_ptr_in_list (mr_save_data, str, &fd_);
      if (ref_idx >= 0)
	{
	  mr_save_data->ptrs.ra.data[idx].ref_idx = ref_idx;
	  mr_save_data->ptrs.ra.data[ref_idx].flags.is_referenced = MR_TRUE;
	}
      else
	{
	  mr_save_data->parent = idx;
	  mr_save_inner (str, &fd_, mr_save_data);
	  mr_save_data->parent = mr_save_data->ptrs.ra.data[mr_save_data->parent].parent;
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
  int idx = mr_save_data->ptrs.ra.size / sizeof (mr_save_data->ptrs.ra.data[0]) - 1;
  mr_td_t * tdp = mr_get_td_by_name (mr_save_data->ptrs.ra.data[idx].fd.type); /* look up for type descriptor */
  char * data = mr_save_data->ptrs.ra.data[idx].data;
  int count;
  int i;
  
  if (NULL == tdp) /* check whether type descriptor was found */
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_NO_TYPE_DESCRIPTOR, mr_save_data->ptrs.ra.data[idx].fd.type);
      return;
    }
  if (tdp->mr_type != MR_TYPE_STRUCT)
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_TYPE_NOT_STRUCT, tdp->type);
      return;
    }

  mr_save_data->parent = idx;
  /* add each child to this node */
  count = tdp->fields.size / sizeof (tdp->fields.data[0]);
  for (i = 0; i < count; ++i)
    mr_save_inner (data + tdp->fields.data[i].offset, &tdp->fields.data[i], mr_save_data);
  mr_save_data->parent = mr_save_data->ptrs.ra.data[mr_save_data->parent].parent;
}

/**
 * Comparator for union discriminator info structures
 * @param x index in mr_union_discriminator_t rarray
 * @param y index in mr_union_discriminator_t rarray
 * @param content void pointer to context
 */
static int
cmp_ud (const void * x, const void * y, const void * context)
{
  const mr_save_data_t * mr_save_data = context;
  int diff = strcmp (mr_save_data->mr_ra_ud.data[(long)x].type, mr_save_data->mr_ra_ud.data[(long)y].type);
  if (diff)
    return (diff);
  return (strcmp (mr_save_data->mr_ra_ud.data[(long)x].discriminator, mr_save_data->mr_ra_ud.data[(long)y].discriminator));
}

/**
 * Finds out union discriminator
 * @param mr_save_data save routines data and lookup structures
 */
static int
mr_union_discriminator (mr_save_data_t * mr_save_data)
{
  int field_idx = -1; /* marker that no valid discriminator was found */
  int idx = mr_save_data->ptrs.ra.size / sizeof (mr_save_data->ptrs.ra.data[0]) - 1; /* index of the last element - it is union itself */
  int parent;
  long ud_idx;
  void * ud_find = NULL;
  mr_td_t * tdp = mr_get_td_by_name (mr_save_data->ptrs.ra.data[idx].fd.type); /* look up for type descriptor */
  mr_union_discriminator_t * ud = mr_rarray_append ((void*)&mr_save_data->mr_ra_ud, sizeof (mr_save_data->mr_ra_ud.data[0]));

  /* create a record for further lookups in parent nodes for discriminator value */
  if (NULL == ud)
    return (0);
  memset (ud, 0, sizeof (*ud));
  /* this record is only for lookups and there is no guarantee that parents already have union resolution info */
  mr_save_data->mr_ra_ud.size -= sizeof (mr_save_data->mr_ra_ud.data[0]);
  ud_idx = mr_save_data->mr_ra_ud.size / sizeof (mr_save_data->mr_ra_ud.data[0]); /* index of lookup record */
  ud->type = mr_save_data->ptrs.ra.data[idx].fd.type; /* union type */
  ud->discriminator = mr_save_data->ptrs.ra.data[idx].fd.comment; /* union discriminator */

  /* traverse through parent up to root node */
  for (parent = mr_save_data->ptrs.ra.data[idx].parent; parent >= 0; parent = mr_save_data->ptrs.ra.data[parent].parent)
    if (MR_TYPE_EXT_NONE == mr_save_data->ptrs.ra.data[parent].fd.mr_type_ext)
      {
	mr_td_t * parent_tdp;
	mr_fd_t * parent_fdp;
	void * discriminator;
	char * named_discriminator = NULL;
	/* checks if this parent already have union resolution info */
	ud_find = tfind ((void*)ud_idx, (void*)&mr_save_data->ptrs.ra.data[parent].union_discriminator, cmp_ud, mr_save_data);
	/* break the traverse loop if it has */
	if (ud_find)
	  break;
	/* otherwise get type descriptor for this parent */
	parent_tdp = mr_get_td_by_name (mr_save_data->ptrs.ra.data[parent].fd.type);
	if (NULL == parent_tdp)
	  continue;
	/* lookup for a discriminator field in this parent */
	parent_fdp = mr_get_fd_by_name (parent_tdp, mr_save_data->ptrs.ra.data[idx].fd.comment);
	if (NULL == parent_fdp)
	  continue; /* continue traverse if it doesn't */

	/* check that this parent is of valid type - structure, union or a pointer to something */
	if ((MR_TYPE_EXT_NONE != parent_fdp->mr_type_ext) && (MR_TYPE_EXT_POINTER != parent_fdp->mr_type_ext))
	  continue;

	/* get an address of discriminator field */
	discriminator = (char*)mr_save_data->ptrs.ra.data[parent].data + parent_fdp->offset;

	/* if discriminator is a pointer then we need address of the content */
	if (MR_TYPE_EXT_POINTER == parent_fdp->mr_type_ext)
	  discriminator = *(void**)discriminator;
	/* switch over basic types */
	switch (parent_fdp->mr_type)
	  {
	  case MR_TYPE_UINT8: field_idx = *(uint8_t*)discriminator; break;
	  case MR_TYPE_INT8: field_idx = *(int8_t*)discriminator; break;
	  case MR_TYPE_UINT16: field_idx = *(uint16_t*)discriminator; break;
	  case MR_TYPE_INT16: field_idx = *(int16_t*)discriminator; break;
	  case MR_TYPE_UINT32: field_idx = *(uint32_t*)discriminator; break;
	  case MR_TYPE_INT32: field_idx = *(int32_t*)discriminator; break;
	  case MR_TYPE_UINT64: field_idx = *(uint64_t*)discriminator; break;
	  case MR_TYPE_INT64: field_idx = *(int64_t*)discriminator; break;
	  case MR_TYPE_BITFIELD:
	    {
	      uint64_t value = 0;
	      mr_ptrdes_t ptrdes = { .data = discriminator, .fd = *parent_fdp, };
	      mr_td_t * enum_tdp = mr_get_td_by_name (parent_fdp->type);
	      mr_save_bitfield_value (&ptrdes, &value); /* get value of the bitfield */
	      if (enum_tdp && (MR_TYPE_ENUM == enum_tdp->mr_type))
		{
		  /* if bitfield is a enumeration then get named discriminator from enum value comment */
		  mr_fd_t * enum_fdp = mr_get_enum_by_value (enum_tdp, value);
		  if (enum_fdp)
		    named_discriminator = enum_fdp->comment;
		}
	      else
		field_idx = value;
	      break;
	    }
	  case MR_TYPE_CHAR_ARRAY: named_discriminator = (char*)discriminator; break;
	  case MR_TYPE_STRING: named_discriminator = *(char**)discriminator; break;
	  case MR_TYPE_ENUM:
	    {
	      int64_t enum_value = 0;
	      mr_td_t * enum_tdp = mr_get_td_by_name (parent_fdp->type);
	      mr_fd_t * enum_fdp;
			
	      if (enum_tdp && (MR_TYPE_ENUM == enum_tdp->mr_type))
		{
		  /*
		    GCC caluculates sizeof for the type according alignment, but initialize only effective bytes
		    i.e. for typedef enum __attribute__ ((packed, aligned (sizeof (uint16_t)))) {} enum_t;
		    sizeof (enum_t) == 2, but type has size only 1 byte
		  */
		  switch (enum_tdp->size_effective)
		    {
		    case sizeof (uint8_t): enum_value = *(uint8_t*)discriminator; break;
		    case sizeof (uint16_t): enum_value = *(uint16_t*)discriminator; break;
		    case sizeof (uint32_t): enum_value = *(uint32_t*)discriminator; break;
		    case sizeof (uint64_t): enum_value = *(uint64_t*)discriminator; break;
		    default:
#if __BYTE_ORDER == __LITTLE_ENDIAN
		      memcpy (&enum_value, discriminator, MR_MIN (enum_tdp->size_effective, sizeof (enum_value)));
#else
#error Support for non little endian architectures to be implemented
#endif /*__BYTE_ORDER == __LITTLE_ENDIAN */
		      break;
		    }
		  /* get named discriminator from enum value comment */
		  enum_fdp = mr_get_enum_by_value (enum_tdp, enum_value);
		  if (enum_fdp)
		    named_discriminator = enum_fdp->comment;
		}
	      break;
	    }
	  default: break;
	  }
	/* if named discriminator was found then calculate corresponding field index */
	if (named_discriminator && named_discriminator[0])
	  {
	    mr_fd_t * union_fdp = mr_get_fd_by_name (tdp, named_discriminator);
	    if (union_fdp)
	      field_idx = union_fdp - tdp->fields.data;
	  }
	break;
      }

  if (ud_find)
    field_idx = mr_save_data->mr_ra_ud.data[*(long*)ud_find].field_idx; /* union discriminator info was found in some of the parents */
  else
    {
      /* union discriminator info was not found in parents so we add new record */
      mr_save_data->mr_ra_ud.size += sizeof (mr_save_data->mr_ra_ud.data[0]);
      ud->field_idx = field_idx;
      ud_find = &ud_idx;
    }

  /* add union discriminator information to all parents wchich doesn't have it yet */
  for (parent = mr_save_data->ptrs.ra.data[idx].parent; parent >= 0; parent = mr_save_data->ptrs.ra.data[parent].parent)
    if (MR_TYPE_EXT_NONE == mr_save_data->ptrs.ra.data[parent].fd.mr_type_ext)
      {
	void * ud_search = tsearch (*(void**)ud_find, (void*)&mr_save_data->ptrs.ra.data[parent].union_discriminator, cmp_ud, mr_save_data);
	if (NULL == ud_search)
	  {
	    MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	    break;
	  }
	if (ud_search == ud_find)
	  break;
      }  
  /* check that field index in union is valid and reset to default otherwise */
  if ((field_idx < 0) || (field_idx >= tdp->fields.size / sizeof (tdp->fields.data[0])))
    field_idx = 0;

  return (field_idx);
}

/**
 * Checks that string is a valid field name [_a-zA-A][_a-zA-Z0-9]*
 * @param name union comment string
 */
static int
mr_is_valid_field_name (char * name)
{
  if (NULL == name)
    return (0);
  if (!isalpha (*name) && ('_' != *name))
    return (0);
  for (++name; *name; ++name)
    if (!isalnum (*name) && ('_' != *name))
      return (0);
  return (!0);
}

/**
 * MR_UNION type saving handler. Saves structure as internal representation tree node.
 * @param mr_save_data save routines data and lookup structures
 */
static void
mr_save_union (mr_save_data_t * mr_save_data)
{
  int idx = mr_save_data->ptrs.ra.size / sizeof (mr_save_data->ptrs.ra.data[0]) - 1;
  char * data = mr_save_data->ptrs.ra.data[idx].data;
  mr_td_t * tdp = mr_get_td_by_name (mr_save_data->ptrs.ra.data[idx].fd.type); /* look up for type descriptor */
  int field_idx = 0;
  
  if (NULL == tdp) /* check whether type descriptor was found */
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_NO_TYPE_DESCRIPTOR, mr_save_data->ptrs.ra.data[idx].fd.type);
      return;
    }
  if ((tdp->mr_type != MR_TYPE_UNION) && (tdp->mr_type != MR_TYPE_ANON_UNION) && (tdp->mr_type != MR_TYPE_NAMED_ANON_UNION))
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_TYPE_NOT_UNION, tdp->type);
      return;
    }

  /* if union comment is a valid field name, then traverse thruogh parents and look for union discriminator */
  if (mr_is_valid_field_name (mr_save_data->ptrs.ra.data[idx].fd.comment))
    field_idx = mr_union_discriminator (mr_save_data);
  
  mr_save_data->parent = idx;
  mr_save_data->ptrs.ra.data[idx].union_field_name = tdp->fields.data[field_idx].name; /* field name is required for XDR serialization */
  mr_save_inner (data + tdp->fields.data[field_idx].offset, &tdp->fields.data[field_idx], mr_save_data);
  mr_save_data->parent = mr_save_data->ptrs.ra.data[mr_save_data->parent].parent;
}

/**
 * MR_ARRAY type saving handler. Saves array into internal representation.
 * @param mr_save_data save routines data and lookup structures
 */
static void
mr_save_array (mr_save_data_t * mr_save_data)
{
  int idx = mr_save_data->ptrs.ra.size / sizeof (mr_save_data->ptrs.ra.data[0]) - 1;
  char * data = mr_save_data->ptrs.ra.data[idx].data;
  mr_fd_t fd_ = mr_save_data->ptrs.ra.data[idx].fd;
  int row_count = fd_.param.array_param.row_count;
  int count = fd_.param.array_param.count;
  int i;

  if (1 == fd_.param.array_param.row_count)
    fd_.mr_type_ext = MR_TYPE_EXT_NONE; /* set extended type property to MR_NONE in copy of field descriptor */
  else
    {
      fd_.param.array_param.count = row_count;
      fd_.param.array_param.row_count = 1;
    }
  /* add each array element to this node */
  mr_save_data->parent = idx;
  for (i = 0; i < count; i += row_count)
    mr_save_inner (data + i * fd_.size, &fd_, mr_save_data);
  mr_save_data->parent = mr_save_data->ptrs.ra.data[mr_save_data->parent].parent;
}

/**
 * MR_RARRAY type saving handler. Saves resizeable array into internal representation.
 * @param mr_save_data save routines data and lookup structures
 */
static void
mr_save_rarray (mr_save_data_t * mr_save_data)
{
  int idx = mr_save_data->ptrs.ra.size / sizeof (mr_save_data->ptrs.ra.data[0]) - 1;
  int data_idx;
  mr_fd_t fd_ = mr_save_data->ptrs.ra.data[idx].fd;
  mr_rarray_t * ra = mr_save_data->ptrs.ra.data[idx].data;
  int count = ra->size / fd_.size;

  /* save as mr_rarray_t */
  mr_save_data->ptrs.ra.data[idx].fd.type = "mr_rarray_t";
  mr_save_data->ptrs.ra.data[idx].fd.mr_type = MR_TYPE_STRUCT;
  mr_save_data->ptrs.ra.data[idx].fd.mr_type_ext = MR_TYPE_EXT_NONE;
  mr_save_struct (mr_save_data);

  /* lookup for subnode .data */
  for (data_idx = mr_save_data->ptrs.ra.data[idx].first_child; data_idx >= 0; data_idx = mr_save_data->ptrs.ra.data[data_idx].next)
    if (0 == strcmp ("data", mr_save_data->ptrs.ra.data[data_idx].fd.name))
      break;

  if (data_idx < 0)
    MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_RARRAY_FAILED);
  else
    {
      /* change void * on appropriate type */
      mr_save_data->ptrs.ra.data[data_idx].fd.type = fd_.type;
      mr_save_data->ptrs.ra.data[data_idx].fd.size = ra->size;
      mr_save_data->ptrs.ra.data[data_idx].fd.mr_type_ext = MR_TYPE_EXT_RARRAY_DATA;
      mr_save_data->ptrs.ra.data[data_idx].flags.is_opaque_data = ra->ptr_type ? !strcmp (ra->ptr_type, MR_RARRAY_OPAQUE_DATA_T_STR) : MR_FALSE;

      if (mr_save_data->ptrs.ra.data[data_idx].ref_idx < 0)
	{
	  if ((NULL == ra->data) || (0 == count))
	    mr_save_data->ptrs.ra.data[data_idx].flags.is_null = MR_TRUE;
	  else
	    {
	      int i;
	      fd_.mr_type_ext = MR_TYPE_EXT_NONE;
	      /* add each array element to this node */
	      mr_save_data->parent = data_idx;
	      for (i = 0; i < count; ++i)
		mr_save_inner (&((char*)ra->data)[i * fd_.size], &fd_, mr_save_data);
	      mr_save_data->parent = mr_save_data->ptrs.ra.data[idx].parent;
	    }
	}
    }
}

/**
 * Saves pointer into internal representation.
 * @param postpone flag for postponed saving
 * @param idx node index
 * @param mr_save_data save routines data and lookup structures
 */
static void
mr_save_pointer_postponed (int postpone, int idx, mr_save_data_t * mr_save_data)
{
  void ** data = mr_save_data->ptrs.ra.data[idx].data;
  mr_fd_t fd_ = mr_save_data->ptrs.ra.data[idx].fd;
  int ref_idx;
  
  if (NULL == *data)
    mr_save_data->ptrs.ra.data[idx].flags.is_null = MR_TRUE; /* return empty node if pointer is NULL */
  else
    {
      /* set extended type property to NONE in copy of field descriptor */
      fd_.mr_type_ext = MR_TYPE_EXT_NONE;
  
      /* check if this pointer is already saved */
      ref_idx = mr_check_ptr_in_list (mr_save_data, *data, &fd_);
      if (ref_idx >= 0)
	{
	  mr_save_data->ptrs.ra.data[idx].ref_idx = ref_idx;
	  mr_save_data->ptrs.ra.data[ref_idx].flags.is_referenced = MR_TRUE;
	}
      else if ((fd_.mr_type != MR_TYPE_NONE) && (fd_.mr_type != MR_TYPE_VOID))
	{
	  if (postpone)
	    {
	      int * idx_ = mr_rarray_append ((void*)&mr_save_data->mr_ra_idx, sizeof (mr_save_data->mr_ra_idx.data[0]));
	      if (NULL == idx_)
		return;
	      *idx_ = idx;
	    }
	  else
	    {
	      mr_save_data->parent = idx;
	      mr_save_inner (*data, &fd_, mr_save_data); /* save referenced content */
	      mr_save_data->parent = mr_save_data->ptrs.ra.data[mr_save_data->parent].parent;
	    }
	}
    }
}

/**
 * Set indexes to nodes according saving sequence.
 * @param mr_save_data save routines data and lookup structures
 */
static void
mr_post_process (mr_save_data_t * mr_save_data)
{
  int idx = 0;
  int idx_ = 0;
  
  while (idx >= 0)
    {
      int parent = mr_save_data->ptrs.ra.data[idx].parent;
      mr_save_data->ptrs.ra.data[idx].idx = idx_++;
      if (parent < 0)
	mr_save_data->ptrs.ra.data[idx].level = 0;
      else
	mr_save_data->ptrs.ra.data[idx].level = mr_save_data->ptrs.ra.data[parent].level + 1;

      if ((MR_TYPE_EXT_POINTER == mr_save_data->ptrs.ra.data[idx].fd.mr_type_ext) &&
	  ((MR_TYPE_NONE == mr_save_data->ptrs.ra.data[idx].fd.mr_type) || (MR_TYPE_VOID == mr_save_data->ptrs.ra.data[idx].fd.mr_type)) &&
	  (mr_save_data->ptrs.ra.data[idx].ref_idx < 0) &&
	  (NULL != *(void**)mr_save_data->ptrs.ra.data[idx].data))
	{
	  static mr_fd_t fd_ = {
	    .mr_type = MR_TYPE_NONE,
	    .mr_type_ext = MR_TYPE_EXT_NONE,
	  };
	  int ref_idx = mr_check_ptr_in_list (mr_save_data, *(void**)mr_save_data->ptrs.ra.data[idx].data, &fd_);
	  if (ref_idx >= 0)
	    {
	      mr_save_data->ptrs.ra.data[idx].ref_idx = ref_idx;
	      mr_save_data->ptrs.ra.data[ref_idx].flags.is_referenced = MR_TRUE;
	    }
	  else
	    mr_save_data->ptrs.ra.data[idx].flags.is_null = MR_TRUE; /* unresolved void pointers are saved as NULL */
	}

      if (mr_save_data->ptrs.ra.data[idx].ref_idx >= 0)
	{
	  int ref_parent = mr_save_data->ptrs.ra.data[mr_save_data->ptrs.ra.data[idx].ref_idx].parent;
	  if ((ref_parent >= 0) && (MR_TYPE_STRING == mr_save_data->ptrs.ra.data[ref_parent].fd.mr_type)
	      && (MR_TYPE_EXT_NONE == mr_save_data->ptrs.ra.data[ref_parent].fd.mr_type_ext))
	    {
	      mr_save_data->ptrs.ra.data[idx].ref_idx = ref_parent;
	      mr_save_data->ptrs.ra.data[idx].flags.is_content_reference = MR_TRUE;
	      mr_save_data->ptrs.ra.data[ref_parent].flags.is_referenced = MR_TRUE;
	    }
	}

      if ((MR_TYPE_STRING == mr_save_data->ptrs.ra.data[idx].fd.mr_type) &&
	  (MR_TYPE_EXT_NONE == mr_save_data->ptrs.ra.data[idx].fd.mr_type_ext))
	mr_save_data->ptrs.ra.data[idx].first_child = mr_save_data->ptrs.ra.data[idx].last_child = -1;

      if (mr_save_data->ptrs.ra.data[idx].first_child >= 0)
	idx = mr_save_data->ptrs.ra.data[idx].first_child;
      else
	{
	  while ((mr_save_data->ptrs.ra.data[idx].next < 0) && (mr_save_data->ptrs.ra.data[idx].parent >= 0))
	    idx = mr_save_data->ptrs.ra.data[idx].parent;
	  idx = mr_save_data->ptrs.ra.data[idx].next;
	}
    }      
}

/**
 * MR_POINTER_STRUCT type saving handler. Save referenced structure into internal representation.
 * @param mr_save_data save routines data and lookup structures
 */
static void
mr_save_pointer (mr_save_data_t * mr_save_data)
{
  mr_save_pointer_postponed (!0, mr_save_data->ptrs.ra.size / sizeof (mr_save_data->ptrs.ra.data[0]) - 1, mr_save_data);
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

  mr_save_data->parent = -1;
  mr_save_data->typed_ptrs_tree = NULL;
  mr_save_data->untyped_ptrs_tree = NULL;
  mr_save_data->tree_key_ptr_wrapper_type = "long_int_key";
  mr_save_data->mr_ra_ud.size = 0;
  mr_save_data->mr_ra_ud.data = NULL;
  mr_save_data->mr_ra_idx.size = 0;
  mr_save_data->mr_ra_idx.data = NULL;
  mr_save_data->ptrs.ra.size = 0;
  mr_save_data->ptrs.ra.data = NULL;

  mr_save_inner (data, fdp, mr_save_data);

  while (mr_save_data->mr_ra_idx.size > 0)
    {
      mr_save_data->mr_ra_idx.size -= sizeof (mr_save_data->mr_ra_idx.data[0]);
      mr_save_pointer_postponed (0, mr_save_data->mr_ra_idx.data[mr_save_data->mr_ra_idx.size / sizeof (mr_save_data->mr_ra_idx.data[0])], mr_save_data);
    }
  mr_post_process (mr_save_data);

  for (i = mr_save_data->ptrs.ra.size / sizeof (mr_save_data->ptrs.ra.data[0]) - 1; i >= 0; --i)
    {
      tdestroy (mr_save_data->ptrs.ra.data[i].union_discriminator, dummy_free_func);
      mr_save_data->ptrs.ra.data[i].union_discriminator = NULL;
    }
  if (mr_save_data->mr_ra_ud.data)
    MR_FREE (mr_save_data->mr_ra_ud.data);
  mr_save_data->mr_ra_ud.data = NULL;
  if (mr_save_data->mr_ra_idx.data)
    MR_FREE (mr_save_data->mr_ra_idx.data);
  mr_save_data->mr_ra_idx.data = NULL;
  if (mr_save_data->typed_ptrs_tree)
    tdestroy (mr_save_data->typed_ptrs_tree, dummy_free_func);
  mr_save_data->typed_ptrs_tree = NULL;
  if (mr_save_data->untyped_ptrs_tree)
    tdestroy (mr_save_data->untyped_ptrs_tree, dummy_free_func);
  mr_save_data->untyped_ptrs_tree = NULL;
  
}

/**
 * Init IO handlers Table
 */
static void __attribute__((constructor)) mr_init_save_rl (void)
{
  mr_conf.io_handlers[MR_TYPE_STRING].save.rl = mr_save_string;
  
  mr_conf.io_handlers[MR_TYPE_STRUCT].save.rl = mr_save_struct;
  mr_conf.io_handlers[MR_TYPE_UNION].save.rl = mr_save_union;
  mr_conf.io_handlers[MR_TYPE_ANON_UNION].save.rl = mr_save_union;
  mr_conf.io_handlers[MR_TYPE_NAMED_ANON_UNION].save.rl = mr_save_union;

  mr_conf.io_ext_handlers[MR_TYPE_EXT_ARRAY].save.rl = mr_save_array;
  mr_conf.io_ext_handlers[MR_TYPE_EXT_RARRAY].save.rl = mr_save_rarray;
  mr_conf.io_ext_handlers[MR_TYPE_EXT_POINTER].save.rl = mr_save_pointer;
}
