/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

#include <tsearch.h>

#ifdef HAVE_CONFIG_H
# include <rlconfig.h>
#endif /* HAVE_CONFIG_H */

#define RL_MODE PROTO /* explicitly set type of inclusion */
#include <reslib.h>

static void dummy_free_func (void * nodep) {};

static int
rl_cmp_ptrdes (rl_ptrdes_t * x, rl_ptrdes_t * y)
{
  int diff = x->data - y->data;
  if (diff) return (diff);
  diff = x->fd.rl_type_ext - y->fd.rl_type_ext;
  if (diff) return (diff);
  diff = x->fd.rl_type - y->fd.rl_type;
  if (diff) return (diff);

  switch (x->fd.rl_type)
    {
    case RL_TYPE_STRING:
    case RL_TYPE_CHAR_ARRAY:
    case RL_TYPE_CHAR:
    case RL_TYPE_VOID:
    case RL_TYPE_INT8:
    case RL_TYPE_UINT8:
    case RL_TYPE_INT16:
    case RL_TYPE_UINT16:
    case RL_TYPE_INT32:
    case RL_TYPE_UINT32:
    case RL_TYPE_INT64:
    case RL_TYPE_UINT64:
    case RL_TYPE_FLOAT:
    case RL_TYPE_DOUBLE:
    case RL_TYPE_LONG_DOUBLE:
      break;
    case RL_TYPE_BITFIELD:
      diff = x->fd.shift - y->fd.shift;
      if (diff) return (diff);
      diff = x->fd.width - y->fd.width;
      if (diff) return (diff);
      break;
    default:
      diff = strcmp (x->fd.type, y->fd.type);
      if (diff) return (diff);
      break;
    }
  return (0);
}

#define CMP_TYPED_PTRDES(PTRS) int cmp_typed_ptrdes (const void * x, const void * y) { return (rl_cmp_ptrdes (&PTRS->ra.data[(long)x], &PTRS->ra.data[(long)y])); }

#define CMP_UNTYPED_PTRDES(PTRS) int cmp_untyped_ptrdes (const void * x, const void * y) { return (PTRS->ra.data[(long)x].data - PTRS->ra.data[(long)y].data); }

/**
 * If node is a plain structure and previously it was saved as a pointer
 * then we need to exchange nodes in the tree
 * @param ptrs resizable array with pointers on already save structures
 * @return an index of node in ptrs
 */
static int
rl_resolve_typed_forward_ref (rl_save_data_t * rl_save_data)
{
  rl_ra_rl_ptrdes_t * ptrs = &rl_save_data->ptrs;
  long count = ptrs->ra.size / sizeof (ptrs->ra.data[0]) - 1;
  void * tree_search_result;
  int ref_idx;

  CMP_TYPED_PTRDES (ptrs);

  tree_search_result = tsearch ((void*)count, (void*)&rl_save_data->typed_ptrs_tree, cmp_typed_ptrdes);
  if (NULL == tree_search_result)
    {
      RL_MESSAGE (RL_LL_FATAL, RL_MESSAGE_OUT_OF_MEMORY);
      return (-1);
    }
  ref_idx = *(long*)tree_search_result;
  if ((ref_idx != count) && (ptrs->ra.data[ref_idx].parent >= 0))
    if (RL_TYPE_EXT_POINTER == ptrs->ra.data[ptrs->ra.data[ref_idx].parent].fd.rl_type_ext)
      return (ref_idx);
  return (-1);
}

/**
 * If node is a plain structure and previously it was saved as a pointer
 * then we need to exchange nodes in the tree
 * @param rl_save_data save routines data and lookup structures
 * @return an index of node in ptrs
 */
static int
rl_resolve_untyped_forward_ref (rl_save_data_t * rl_save_data)
{
  rl_ra_rl_ptrdes_t * ptrs = &rl_save_data->ptrs;
  long count = ptrs->ra.size / sizeof (ptrs->ra.data[0]) - 1;
  void * data = ptrs->ra.data[count].data;
  int same_ptr = count;
  void * tree_search_result;
  int ref_idx;

  CMP_UNTYPED_PTRDES (ptrs);

  tree_search_result = tsearch ((void*)count, (void*)&rl_save_data->untyped_ptrs_tree, cmp_untyped_ptrdes);  
  if (NULL == tree_search_result)
    {
      RL_MESSAGE (RL_LL_FATAL, RL_MESSAGE_OUT_OF_MEMORY);
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
    Found node is a pointer if its parent has rl_type_ext == RL_TYPE_EXT_POINTER
   */
  if ((ref_idx != same_ptr) && (ptrs->ra.data[ref_idx].parent >= 0))
    if (RL_TYPE_EXT_POINTER == ptrs->ra.data[ptrs->ra.data[ref_idx].parent].fd.rl_type_ext)
      return (ref_idx);
  return (-1);
}

/**
 * Check whether pointer is presented in the list.
 * @param rl_save_data save routines data and lookup structures
 * @param idx index of search element
 * @return an index of node in ptrs
 */
static int
rl_check_ptr_in_list (rl_save_data_t * rl_save_data, int idx)
{
  rl_ra_rl_ptrdes_t * ptrs = &rl_save_data->ptrs;
  void * data = *(void**)ptrs->ra.data[idx].data;
  void * tree_find_result;
  long idx_;

  CMP_TYPED_PTRDES (ptrs);
  CMP_UNTYPED_PTRDES (ptrs);
  
  idx_ = rl_add_ptr_to_list (ptrs);
  if (idx_ < 0)
    return (idx); /* memory allocation error occured */
  ptrs->ra.data[idx_].data = data;
  ptrs->ra.data[idx_].fd = ptrs->ra.data[idx].fd;
  ptrs->ra.data[idx_].fd.rl_type_ext = RL_TYPE_EXT_NONE;
  
  ptrs->ra.size -= sizeof (ptrs->ra.data[0]);
  tree_find_result = tfind ((void*)idx_, (void*)&rl_save_data->typed_ptrs_tree, cmp_typed_ptrdes);
  if (tree_find_result)
    return (*(long*)tree_find_result);
  tree_find_result = tfind ((void*)idx_, (void*)&rl_save_data->untyped_ptrs_tree, cmp_untyped_ptrdes);
  if (tree_find_result)
    return (*(long*)tree_find_result);
  return (-1);
}

static void
rl_save_string (rl_save_data_t * rl_save_data)
{
  int idx = rl_save_data->ptrs.ra.size / sizeof (rl_save_data->ptrs.ra.data[0]) - 1;
  char * str = *(char**)rl_save_data->ptrs.ra.data[idx].data;
  if (NULL == str)
    rl_save_data->ptrs.ra.data[idx].flags |= RL_PDF_IS_NULL;
}

/**
 * Save scheduler. Save any object into internal representation.
 * @param data a pointer on data
 * @param fdp a ponter of field descriptor
 * @param rl_save_data save routines data and lookup structures
 */
static void
rl_save_inner (void * data, rl_fd_t * fdp, rl_save_data_t * rl_save_data)
{
  int idx, ref_idx;

  /* add pointer on saving structure to list ptrs */
  idx = rl_add_ptr_to_list (&rl_save_data->ptrs);
  if (idx < 0)
    return; /* memory allocation error occured */
  rl_save_data->ptrs.ra.data[idx].data = data;
  rl_save_data->ptrs.ra.data[idx].fd = *fdp;
  rl_save_data->ptrs.ra.data[idx].parent = rl_save_data->parent; /* NB: rl_add_child do the same, but also adds links from parent to child. This link is requred for rl_resolve_untyped_forward_ref  */
  
  /* forward reference resolving */
  ref_idx = rl_resolve_typed_forward_ref (rl_save_data);
  if (ref_idx >= 0)
    {
      int parent = rl_save_data->ptrs.ra.data[ref_idx].parent;
      rl_save_data->ptrs.ra.data[parent].ref_idx = ref_idx;
      rl_save_data->ptrs.ra.data[parent].first_child = -1;
      rl_save_data->ptrs.ra.data[parent].last_child = -1;
      rl_save_data->ptrs.ra.data[ref_idx].flags |= RL_PDF_IS_REFERENCED;
      rl_save_data->ptrs.ra.data[ref_idx].fd.name = fdp->name;
      rl_add_child (rl_save_data->parent, ref_idx, &rl_save_data->ptrs);
      return;
    }
  ref_idx = rl_resolve_untyped_forward_ref (rl_save_data);
  /* it is impossible to correctly resolve untyped pointer match so we ignore it */
  rl_add_child (rl_save_data->parent, idx, &rl_save_data->ptrs);

  /* route saving handler */
  if ((fdp->rl_type_ext >= 0) && (fdp->rl_type_ext < RL_MAX_TYPES)
      && rl_io_ext_handlers[fdp->rl_type_ext].save.rl)
    rl_io_ext_handlers[fdp->rl_type_ext].save.rl (rl_save_data);
  else if ((fdp->rl_type >= 0) && (fdp->rl_type < RL_MAX_TYPES)
	   && rl_io_handlers[fdp->rl_type].save.rl)
    rl_io_handlers[fdp->rl_type].save.rl (rl_save_data);
}

/**
 * RL_STRUCT type saving handler. Saves structure as internal representation tree node.
 * @param rl_save_data save routines data and lookup structures
 */
static void
rl_save_struct (rl_save_data_t * rl_save_data)
{
  int idx = rl_save_data->ptrs.ra.size / sizeof (rl_save_data->ptrs.ra.data[0]) - 1;
  rl_td_t * tdp = rl_get_td_by_name (rl_save_data->ptrs.ra.data[idx].fd.type); /* look up for type descriptor */
  char * data = rl_save_data->ptrs.ra.data[idx].data;
  int count;
  int i;
  
  if (NULL == tdp) /* check whether type descriptor was found */
    {
      RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_NO_TYPE_DESCRIPTOR, rl_save_data->ptrs.ra.data[idx].fd.type);
      return;
    }

  rl_save_data->parent = idx;
  /* add each child to this node */
  count = tdp->fields.size / sizeof (tdp->fields.data[0]);
  for (i = 0; i < count; ++i)
    rl_save_inner (data + tdp->fields.data[i].offset, &tdp->fields.data[i], rl_save_data);
  rl_save_data->parent = rl_save_data->ptrs.ra.data[rl_save_data->parent].parent;
}

/**
 * RL_UNION type saving handler. Saves structure as internal representation tree node.
 * @param rl_save_data save routines data and lookup structures
 */
static void
rl_save_union (rl_save_data_t * rl_save_data)
{
  int idx = rl_save_data->ptrs.ra.size / sizeof (rl_save_data->ptrs.ra.data[0]) - 1;
  char * data = rl_save_data->ptrs.ra.data[idx].data;
  rl_td_t * tdp = rl_get_td_by_name (rl_save_data->ptrs.ra.data[idx].fd.type); /* look up for type descriptor */
  int field_idx = 0;
  
  if (NULL == tdp) /* check whether type descriptor was found */
    {
      RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_NO_TYPE_DESCRIPTOR, rl_save_data->ptrs.ra.data[idx].fd.type);
      return;
    }
  if ((tdp->rl_type != RL_TYPE_UNION) && (tdp->rl_type != RL_TYPE_ANON_UNION))
    {
      RL_MESSAGE (RL_LL_WARN, RL_MESSAGE_NOT_A_UNION, tdp->type);
      return;
    }

  if ((rl_save_data->ptrs.ra.data[idx].fd.comment) && (rl_save_data->ptrs.ra.data[idx].fd.comment[0]) &&
      (rl_save_data->ptrs.ra.data[idx].parent >= 0) &&
      (RL_TYPE_STRUCT == rl_save_data->ptrs.ra.data[rl_save_data->ptrs.ra.data[idx].parent].fd.rl_type) &&
      (RL_TYPE_EXT_NONE == rl_save_data->ptrs.ra.data[rl_save_data->ptrs.ra.data[idx].parent].fd.rl_type_ext))
    {
      rl_td_t * parent_tdp = rl_get_td_by_name (rl_save_data->ptrs.ra.data[rl_save_data->ptrs.ra.data[idx].parent].fd.type);
      if (parent_tdp)
	{
	  rl_fd_t * parent_fdp = rl_get_fd_by_name (parent_tdp, rl_save_data->ptrs.ra.data[idx].fd.comment);
	  if (parent_fdp)
	    {
	      void * discriminator = (char*)rl_save_data->ptrs.ra.data[rl_save_data->ptrs.ra.data[idx].parent].data + parent_fdp->offset;
	      char * named_discriminator = NULL;
	      if (RL_TYPE_EXT_NONE == parent_fdp->rl_type_ext)
		switch (parent_fdp->rl_type)
		  {
		  case RL_TYPE_UINT8: field_idx = *(uint8_t*)discriminator; break;
		  case RL_TYPE_INT8: field_idx = *(int8_t*)discriminator; break;
		  case RL_TYPE_UINT16: field_idx = *(uint16_t*)discriminator; break;
		  case RL_TYPE_INT16: field_idx = *(int16_t*)discriminator; break;
		  case RL_TYPE_UINT32: field_idx = *(uint32_t*)discriminator; break;
		  case RL_TYPE_INT32: field_idx = *(int32_t*)discriminator; break;
		  case RL_TYPE_UINT64: field_idx = *(uint64_t*)discriminator; break;
		  case RL_TYPE_INT64: field_idx = *(int64_t*)discriminator; break;
		  case RL_TYPE_CHAR_ARRAY: named_discriminator = (char*)discriminator; break;
		  case RL_TYPE_STRING: named_discriminator = *(char**)discriminator; break;
		  case RL_TYPE_ENUM:
		    {
		      int64_t enum_value = 0;
		      rl_td_t * enum_tdp = rl_get_td_by_name (parent_fdp->type);
		      rl_fd_t * enum_fdp;
		      if (NULL == enum_tdp)
			break;
		      
		      switch (parent_fdp->size)
			{
			case sizeof (uint8_t): enum_value = *(uint8_t*)discriminator; break;
			case sizeof (uint16_t): enum_value = *(uint16_t*)discriminator; break;
			case sizeof (uint32_t): enum_value = *(uint32_t*)discriminator; break;
			case sizeof (uint64_t): enum_value = *(uint64_t*)discriminator; break;
			default:
			  memcpy (&enum_value, discriminator, RL_MIN (parent_fdp->size, sizeof (enum_value))); /* to be fixed for bigendian systems */
			  break;
			}
		      
		      enum_fdp = rl_get_enum_by_value (enum_tdp, enum_value);
		      if (enum_fdp)
			named_discriminator = enum_fdp->comment;
		      break;
		    }
		  default: break;
		  }
	      if (named_discriminator && named_discriminator[0])
		{
		  rl_fd_t * union_fdp = rl_get_fd_by_name (tdp, named_discriminator);
		  if (union_fdp)
		    field_idx = union_fdp - tdp->fields.data;
		}
	    }
	}
    }
  
  rl_save_data->ptrs.ra.data[idx].union_field_idx = field_idx; /* field index is required for XDR serialization */
  rl_save_data->parent = idx;
  if ((field_idx >= 0) && (field_idx < tdp->fields.size / sizeof (tdp->fields.data[0])))
    rl_save_inner (data + tdp->fields.data[field_idx].offset, &tdp->fields.data[field_idx], rl_save_data);
  rl_save_data->parent = rl_save_data->ptrs.ra.data[rl_save_data->parent].parent;
}

/**
 * RL_ARRAY type saving handler. Saves array into internal representation.
 * @param rl_save_data save routines data and lookup structures
 */
static void
rl_save_array (rl_save_data_t * rl_save_data)
{
  int idx = rl_save_data->ptrs.ra.size / sizeof (rl_save_data->ptrs.ra.data[0]) - 1;
  char * data = rl_save_data->ptrs.ra.data[idx].data;
  rl_fd_t fd_ = rl_save_data->ptrs.ra.data[idx].fd;
  int row_count = fd_.row_count;
  int count = fd_.count;
  int i;
  
  if (1 == fd_.row_count)
    fd_.rl_type_ext = RL_TYPE_EXT_NONE; /* set extended type property to RL_NONE in copy of field descriptor */
  else
    {
      fd_.count = row_count;
      fd_.row_count = 1;
    }
  /* add each array element to this node */
  rl_save_data->parent = idx;
  for (i = 0; i < count; i += row_count)
    rl_save_inner (data + i * fd_.size, &fd_, rl_save_data);
  rl_save_data->parent = rl_save_data->ptrs.ra.data[rl_save_data->parent].parent;
}

/**
 * RL_RARRAY type saving handler. Saves resizeable array into internal representation.
 * @param rl_save_data save routines data and lookup structures
 */
static void
rl_save_rarray (rl_save_data_t * rl_save_data)
{
  int idx = rl_save_data->ptrs.ra.size / sizeof (rl_save_data->ptrs.ra.data[0]) - 1;
  rl_rarray_t * ra = rl_save_data->ptrs.ra.data[idx].data;
  rl_fd_t fd_ = rl_save_data->ptrs.ra.data[idx].fd;
  int count;
  int i;
  /* set extended type property to RL_NONE in copy of field descriptor */
  fd_.rl_type_ext = RL_TYPE_EXT_NONE;
  count = rl_save_data->ptrs.ra.data[idx].fd.count = ra->size / fd_.size;
  /* do nothing if rarray is empty */
  if (NULL == ra->data)
    return;
  /* add each array element to this node */
  rl_save_data->parent = idx;
  for (i = 0; i < count; ++i)
    rl_save_inner (&((char*)ra->data)[i * fd_.size], &fd_, rl_save_data);
  rl_save_data->parent = rl_save_data->ptrs.ra.data[rl_save_data->parent].parent;
}

static void
rl_save_pointer_postponed (int postpone, int idx, rl_save_data_t * rl_save_data)
{
  void ** data = rl_save_data->ptrs.ra.data[idx].data;
  rl_fd_t fd_ = rl_save_data->ptrs.ra.data[idx].fd;
  int ref_idx;
  
  /* set extended type property to NONE in copy of field descriptor */
  fd_.rl_type_ext = RL_TYPE_EXT_NONE;
  /* return empty node if pointer is NULL */
  if (NULL == *data)
    return;
  /* check if this pointer is already saved */
  ref_idx = rl_check_ptr_in_list (rl_save_data, idx);
  if (ref_idx >= 0)
    {
      rl_save_data->ptrs.ra.data[idx].ref_idx = ref_idx;
      rl_save_data->ptrs.ra.data[ref_idx].flags |= RL_PDF_IS_REFERENCED;
    }
  else if ((fd_.rl_type != RL_TYPE_NONE) && (fd_.rl_type != RL_TYPE_VOID))
    {
      if (postpone)
	{
	  int * idx_ = rl_rarray_append ((void*)&rl_save_data->rl_ra_idx, sizeof (rl_save_data->rl_ra_idx.data[0]));
	  if (NULL == idx_)
	    return;
	  *idx_ = idx;
	}
      else
	{
	  rl_save_data->parent = idx;
	  rl_save_inner (*data, &fd_, rl_save_data); /* save referenced content */
	  rl_save_data->parent = rl_save_data->ptrs.ra.data[rl_save_data->parent].parent;
	}
    }
}

/**
 * Set indexes to nodes according saving sequence.
 * @param rl_save_data save routines data and lookup structures
 */
static void
rl_post_process (rl_save_data_t * rl_save_data)
{
  int idx = 0;
  int idx_ = 0;

  rl_save_data->ptrs.ra.data[idx].idx = idx_++;
  rl_save_data->ptrs.ra.data[idx].level = 0;
  idx = rl_save_data->ptrs.ra.data[idx].first_child;
  
  while (idx >= 0)
    {
      rl_save_data->ptrs.ra.data[idx].idx = idx_++;
      rl_save_data->ptrs.ra.data[idx].level = rl_save_data->ptrs.ra.data[rl_save_data->ptrs.ra.data[idx].parent].level + 1;
      
      if ((RL_TYPE_EXT_POINTER == rl_save_data->ptrs.ra.data[idx].fd.rl_type_ext) &&
	  ((RL_TYPE_NONE == rl_save_data->ptrs.ra.data[idx].fd.rl_type) || (RL_TYPE_VOID == rl_save_data->ptrs.ra.data[idx].fd.rl_type)) &&
	  (rl_save_data->ptrs.ra.data[idx].ref_idx < 0) &&
	  (NULL != *(void**)rl_save_data->ptrs.ra.data[idx].data))
	{
	  int ref_idx = rl_check_ptr_in_list (rl_save_data, idx);
	  if (ref_idx >= 0)
	    {
	      rl_save_data->ptrs.ra.data[idx].ref_idx = ref_idx;
	      rl_save_data->ptrs.ra.data[ref_idx].flags |= RL_PDF_IS_REFERENCED;
	    }
	}

      if (rl_save_data->ptrs.ra.data[idx].first_child >= 0)
	idx = rl_save_data->ptrs.ra.data[idx].first_child;
      else
	{
	  while ((rl_save_data->ptrs.ra.data[idx].next < 0) && (rl_save_data->ptrs.ra.data[idx].parent >= 0))
	    idx = rl_save_data->ptrs.ra.data[idx].parent;
	  idx = rl_save_data->ptrs.ra.data[idx].next;
	}
    }      
}

/**
 * RL_POINTER_STRUCT type saving handler. Save referenced structure into internal representation.
 * @param rl_save_data save routines data and lookup structures
 */
static void
rl_save_pointer (rl_save_data_t * rl_save_data)
{
  rl_save_pointer_postponed (!0, rl_save_data->ptrs.ra.size / sizeof (rl_save_data->ptrs.ra.data[0]) - 1, rl_save_data);
}

/**
 * Public function. Calls save scheduler and frees lookup tables.
 * @param data a pointer on data
 * @param fdp a ponter of field descriptor
 * @param rl_save_data save routines data and lookup structures
 */
void
rl_save (void * data, rl_fd_t * fdp, rl_save_data_t * rl_save_data)
{
  rl_save_inner (data, fdp, rl_save_data);

  while (rl_save_data->rl_ra_idx.size > 0)
    {
      rl_save_data->rl_ra_idx.size -= sizeof (rl_save_data->rl_ra_idx.data[0]);
      rl_save_pointer_postponed (0, rl_save_data->rl_ra_idx.data[rl_save_data->rl_ra_idx.size / sizeof (rl_save_data->rl_ra_idx.data[0])], rl_save_data);
    }
  rl_post_process (rl_save_data);
  if (rl_save_data->rl_ra_idx.data)
    RL_FREE (rl_save_data->rl_ra_idx.data);
  rl_save_data->rl_ra_idx.data = NULL;
  if (rl_save_data->typed_ptrs_tree)
    tdestroy (rl_save_data->typed_ptrs_tree, dummy_free_func);
  rl_save_data->typed_ptrs_tree = NULL;
  if (rl_save_data->untyped_ptrs_tree)
    tdestroy (rl_save_data->untyped_ptrs_tree, dummy_free_func);
  rl_save_data->untyped_ptrs_tree = NULL;
}

/**
 * Init IO handlers Table
 */
static void __attribute__((constructor)) rl_init_save_rl (void)
{
  rl_io_handlers[RL_TYPE_STRING].save.rl = rl_save_string;
  
  rl_io_handlers[RL_TYPE_STRUCT].save.rl = rl_save_struct;
  rl_io_handlers[RL_TYPE_UNION].save.rl = rl_save_union;
  rl_io_handlers[RL_TYPE_ANON_UNION].save.rl = rl_save_union;

  rl_io_ext_handlers[RL_TYPE_EXT_ARRAY].save.rl = rl_save_array;
  rl_io_ext_handlers[RL_TYPE_EXT_RARRAY].save.rl = rl_save_rarray;
  rl_io_ext_handlers[RL_TYPE_EXT_POINTER].save.rl = rl_save_pointer;
}
