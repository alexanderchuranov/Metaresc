/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#ifdef HAVE_CONFIG_H
# include <mr_config.h>
#endif /* HAVE_CONFIG_H */

#include <metaresc.h>
#include <mr_ic.h>
#include <mr_stringify.h>
#include <mr_save.h>

MR_COMPILETIME_ASSERT (sizeof (mr_ptrdes_t) <= 96);

TYPEDEF_FUNC (int, mr_save_handler_t, (mr_save_data_t *))

static mr_save_handler_t mr_save_handler[];

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
  mr_type_t mr_type = parent_fdp->mr_type;
  /* if discriminator is a pointer then we need address of the content */
  if (MR_TYPE_POINTER == parent_fdp->mr_type)
    {
      discriminator = *(void**)discriminator;
      mr_type = parent_fdp->mr_type_aux;
    }
  if (MR_TYPE_VOID == parent_fdp->mr_type)
    mr_type = parent_fdp->mr_type_aux;

  /* switch over basic types */
  if (discriminator)
    switch (mr_type)
      {
#define CASE_RETURN_BY_TYPE(TYPE) case MR_TYPE_DETECT (TYPE): return (mr_union_discriminator_by_idx (tdp, *(TYPE*)discriminator));

	MR_FOREACH (CASE_RETURN_BY_TYPE, bool, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t);

      case MR_TYPE_BITFIELD:
	{
	  uint64_t value = 0;
	  mr_ptrdes_t ptrdes = { .data.ptr = discriminator, .fdp = parent_fdp, };
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
	      mr_enum_value_type_t value = mr_get_enum_value (enum_tdp, discriminator);
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
mr_type_get_discriminator_fd (char * type, char * discriminator)
{
  /* get type descriptor */
  mr_td_t * tdp = mr_get_td_by_name (type);
  if (NULL == tdp)
    return (NULL);
  /* lookup for a discriminator field */
  mr_fd_t * fdp = mr_get_fd_by_name (tdp, discriminator);
  if (NULL == fdp)
    return (NULL);

  /* check that this field is of valid type - structure, union or a pointer to something */
  if (MR_TYPE_ARRAY == fdp->mr_type)
    return (NULL);

  return (fdp);
}

int
mr_uds_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const mr_save_data_t * mr_save_data = context;
  int diff = (mr_save_data->mr_ra_ud[x.intptr].fdp > mr_save_data->mr_ra_ud[y.intptr].fdp) -
    (mr_save_data->mr_ra_ud[x.intptr].fdp < mr_save_data->mr_ra_ud[y.intptr].fdp);
  if (diff)
    return (diff);

  diff = mr_hashed_string_cmp (&mr_save_data->mr_ra_ud[x.intptr].type,
				   &mr_save_data->mr_ra_ud[y.intptr].type);
  if (diff)
    return (diff);
  return (mr_hashed_string_cmp (&mr_save_data->mr_ra_ud[x.intptr].discriminator,
				&mr_save_data->mr_ra_ud[y.intptr].discriminator));
}

mr_hash_value_t
mr_uds_get_hash (mr_ptr_t x, const void * context)
{
  const mr_save_data_t * mr_save_data = context;
  return ((mr_hashed_string_get_hash (&mr_save_data->mr_ra_ud[x.intptr].type) << 1) +
	  mr_hashed_string_get_hash (&mr_save_data->mr_ra_ud[x.intptr].discriminator) +
	  (intptr_t)mr_save_data->mr_ra_ud[x.intptr].fdp);
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
  int diff = mr_hashed_string_cmp (&mr_save_data->mr_ra_ud[x.intptr].type,
				   &mr_save_data->mr_ra_ud[y.intptr].type);
  if (diff)
    return (diff);
  return (mr_hashed_string_cmp (&mr_save_data->mr_ra_ud[x.intptr].discriminator,
				&mr_save_data->mr_ra_ud[y.intptr].discriminator));
}

mr_hash_value_t
mr_ud_get_hash (mr_ptr_t x, const void * context)
{
  const mr_save_data_t * mr_save_data = context;
  return ((mr_hashed_string_get_hash (&mr_save_data->mr_ra_ud[x.intptr].type) << 1) +
	  mr_hashed_string_get_hash (&mr_save_data->mr_ra_ud[x.intptr].discriminator));
}

static int
mr_ud_find (mr_ptrdes_t * ptrdes, mr_ptr_t key, mr_save_data_t * mr_save_data)
{
  if (ptrdes->ud_is_ic)
    {
      mr_ptr_t * find = mr_ic_find (ptrdes->save_params.union_discriminator, key);
      return (find ? find->intptr : -1);
    }

  int i;
  for (i = 0; i < ptrdes->ud_count; ++i)
    if (0 == mr_ud_cmp (key, (intptr_t)ptrdes->save_params.static_array[i], mr_save_data))
      return (ptrdes->save_params.static_array[i]);
  return (-1);
}

static int
mr_ud_add (mr_ptrdes_t * ptrdes, mr_ptr_t key, mr_save_data_t * mr_save_data)
{
  if (ptrdes->ud_is_ic)
    {
      mr_ptr_t * add = mr_ic_add (ptrdes->save_params.union_discriminator, key);
      return (add ? add->intptr : -1);
    }

  int find = mr_ud_find (ptrdes, key, mr_save_data);
  if (find != -1)
    return (find);

  if (ptrdes->ud_count > sizeof (ptrdes->save_params.static_array) / sizeof (ptrdes->save_params.static_array[0]))
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_NUMBER_OF_ITEMS, ptrdes->ud_count);
      ptrdes->ud_count = 0;
    }

  if (ptrdes->ud_count == sizeof (ptrdes->save_params.static_array) / sizeof (ptrdes->save_params.static_array[0]))
    {
      mr_ic_t * dst_ic = MR_CALLOC (1, sizeof (*dst_ic));
      if (NULL == dst_ic)
	{
	  MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	  return (-1);
	}
      mr_res_t context = {
	.data = { mr_save_data },
	.type = "mr_save_data_t",
	.MR_SIZE = sizeof (mr_save_data_t),
      };
      mr_status_t status = mr_ic_hash_new (dst_ic, mr_ud_get_hash, mr_ud_cmp, "intptr", &context);
      if (MR_SUCCESS != status)
	{
	  MR_FREE (dst_ic);
	  return (-1);
	}

      int i;
      mr_ic_rarray_t rarray;
      mr_ptr_t static_array[sizeof (ptrdes->save_params.static_array) / sizeof (ptrdes->save_params.static_array[0])];

      for (i = 0; i < sizeof (static_array) / sizeof (static_array[0]); ++i)
	static_array[i].intptr = ptrdes->save_params.static_array[i];
      rarray.ra = static_array;
      rarray.size = sizeof (static_array);
      rarray.alloc_size = -1;

      status = mr_ic_index (dst_ic, &rarray);
      if (MR_SUCCESS != status)
	{
	  MR_FREE (dst_ic);
	  return (-1);
	}

      ptrdes->save_params.union_discriminator = dst_ic;
      ptrdes->ud_is_ic = true;
      mr_ptr_t * add = mr_ic_add (dst_ic, key);
      return (add ? add->intptr : -1);
    }

  ptrdes->save_params.static_array[ptrdes->ud_count++] = key.intptr;
  return (key.intptr);
}

static mr_status_t
mr_ud_foreach (mr_ptrdes_t * ptrdes, mr_visit_fn_t visit_fn, const void * context)
{
  if (ptrdes->ud_is_ic)
    return (mr_ic_foreach (ptrdes->save_params.union_discriminator, visit_fn, context));

  int i;
  for (i = 0; i < ptrdes->ud_count; ++i)
    if (MR_SUCCESS != visit_fn ((intptr_t)ptrdes->save_params.static_array[i], context))
      return (MR_FAILURE);
  return (MR_SUCCESS);
}

static void
mr_ud_free (mr_ptrdes_t * ptrdes)
{
  if (ptrdes->ud_is_ic)
    {
      mr_ic_free (ptrdes->save_params.union_discriminator);
      MR_FREE (ptrdes->save_params.union_discriminator);
      ptrdes->save_params.union_discriminator = NULL;
    }
  ptrdes->ud_is_ic = false;
  ptrdes->ud_count = 0;
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
  int ud_idx, ud_find = -1;
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
    return (NULL);

  memset (ud, 0, sizeof (*ud));
  /* this record is only for lookups and there is no guarantee that parents already have union resolution info */
  mr_save_data->mr_ra_ud_size -= sizeof (mr_save_data->mr_ra_ud[0]);
  ud_idx = mr_save_data->mr_ra_ud_size / sizeof (mr_save_data->mr_ra_ud[0]); /* index of lookup record */
  ud->type.str = union_type; /* union type */
  ud->discriminator.str = discriminator; /* union discriminator */

  /* traverse through parents up to root node */
  for (parent = node; parent >= 0; parent = mr_save_data->ptrs.ra[parent].parent)
    {
      mr_fd_t * parent_fdp;
      void * discriminator_ptr;

      /* checks if this parent already have union resolution info */
      ud_find = mr_ud_find (&mr_save_data->ptrs.ra[parent], (intptr_t)ud_idx, mr_save_data);
      /* break the traverse loop if it has */
      if (ud_find != -1)
	break;

      if ((MR_TYPE_ARRAY == mr_save_data->ptrs.ra[parent].mr_type)
	  || (MR_TYPE_POINTER == mr_save_data->ptrs.ra[parent].mr_type))
	continue;
      
      parent_fdp = mr_type_get_discriminator_fd (mr_save_data->ptrs.ra[parent].type, ud->discriminator.str);
      if (NULL == parent_fdp)
	continue;
      
      /* get an address of discriminator field */
      discriminator_ptr = (char*)mr_save_data->ptrs.ra[parent].data.ptr + parent_fdp->offset;

      fdp = mr_union_discriminator_by_type (tdp, parent_fdp, discriminator_ptr);
      break;
    }

  if (-1 != ud_find)
    fdp = mr_save_data->mr_ra_ud[ud_find].fdp; /* union discriminator info was found in some of the parents */
  else
    {
      if (NULL == fdp)
	fdp = mr_union_discriminator_by_name (tdp, NULL);

      ud->fdp = fdp;
      mr_ptr_t * add = mr_ic_add (&mr_save_data->union_discriminators, (intptr_t)ud_idx);
      if (NULL == add)
	return (NULL);
      if (add->intptr == ud_idx)
	{
	  /* union discriminator info was not found in parents so we add new record */
	  mr_save_data->mr_ra_ud_size += sizeof (mr_save_data->mr_ra_ud[0]);
	  /* set 'parent' on one level up to include actual parent node into traversal below */
	  if (parent >= 0)
	    parent = mr_save_data->ptrs.ra[parent].parent;
	}
      ud_find = add->intptr;
    }

  /* add union discriminator information to all parents which doesn't have it yet */
  for (idx = node; idx != parent; idx = mr_save_data->ptrs.ra[idx].parent)
    if (-1 == mr_ud_add (&mr_save_data->ptrs.ra[idx], (intptr_t)ud_find, mr_save_data))
      return (NULL);

  return (fdp);
}

/**
 * Comparator for mr_ptrdes_t.
 * @param x value to compare
 * @param y value to compare
 * @return comparation sign
 */
static inline int
mr_cmp_ptrdes (mr_ptrdes_t * x, mr_ptrdes_t * y)
{
  int diff = ((x->data.uintptr > y->data.uintptr) - (x->data.uintptr < y->data.uintptr));
  if (diff)
    return (diff);
  
  diff = x->mr_type - y->mr_type;
  if (diff)
    return (diff);

  switch (x->mr_type)
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
      
    case MR_TYPE_ARRAY:
    case MR_TYPE_POINTER:
    case MR_TYPE_BITFIELD:
      return ((x > y) - (x < y));
      
    case MR_TYPE_STRUCT:
    case MR_TYPE_FUNC_TYPE:
    case MR_TYPE_FUNC:
    case MR_TYPE_ENUM:
    case MR_TYPE_UNION:
    case MR_TYPE_ANON_UNION:
    case MR_TYPE_NAMED_ANON_UNION:
      return (strcmp (x->type, y->type));
      
    case MR_TYPE_END_ANON_UNION:
    case MR_TYPE_LAST:
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_MR_TYPE);
      break;
    }
  return (0);
}

mr_hash_value_t
mr_typed_ptrdes_get_hash (const mr_ptr_t x, const void * context)
{
  const mr_ra_ptrdes_t * ra_ptrdes = context;
  const mr_ptrdes_t * ptrdes = &ra_ptrdes->ra[x.intptr];
  return (ptrdes->data.uintptr + ptrdes->mr_type * 3);
}

int
mr_typed_ptrdes_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const mr_ra_ptrdes_t * ra_ptrdes = context;
  return (mr_cmp_ptrdes (&ra_ptrdes->ra[x.intptr], &ra_ptrdes->ra[y.intptr]));
}

mr_hash_value_t
mr_untyped_ptrdes_get_hash (const mr_ptr_t x, const void * context)
{
  const mr_ra_ptrdes_t * ra_ptrdes = context;
  return (ra_ptrdes->ra[x.intptr].data.uintptr);
}

int
mr_untyped_ptrdes_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const mr_ra_ptrdes_t * ra_ptrdes = context;
  void * x_ptr = ra_ptrdes->ra[x.intptr].data.ptr;
  void * y_ptr = ra_ptrdes->ra[y.intptr].data.ptr;
  return ((x_ptr > y_ptr) - (x_ptr < y_ptr));
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
  mr_union_discriminator_t * ud = &mr_save_data->mr_ra_ud[key.intptr];
  /* mr_ra_ud would be reallocaed within this function, so we need to get values from this node */
  char * discriminator = ud->discriminator.str;
  char * type = ud->type.str;
  mr_fd_t * ud_fdp = ud->fdp;
  mr_fd_t * fdp = mr_type_get_discriminator_fd (mr_save_data->ptrs.ra[mr_check_ud_ctx->node].type, discriminator);

  /* here we check that union discriminator was resolved at the level of saved node.
     This means that resolution is not dependant on upper tree */
  if (NULL != fdp)
    return (MR_SUCCESS);
  /* otherwise we need to find union resolution in the context of new parent */
  fdp = mr_union_discriminator (mr_save_data, mr_check_ud_ctx->parent, type, discriminator);
  
  return ((fdp == ud_fdp) ? MR_SUCCESS : MR_FAILURE);
}

static bool
ref_is_parent (mr_ptrdes_t * ra, int node, int ref_idx)
{
  while ((node >= 0) && (node != ref_idx))
    node = ra[node].parent;
  return (node == ref_idx);
}

static int
move_nodes_to_parent (mr_ptrdes_t * ra, int ref_parent, int parent, int idx, mr_size_t element_size)
{
  int count, ref_idx = ra[ref_parent].first_child;
    
  ra[ref_parent].ref_idx = ref_idx;
  ra[ref_parent].first_child = -1;
  ra[ref_parent].last_child = -1;
  ra[ref_idx].flags.is_referenced = true;
			  
  for (count = 0; ref_idx >= 0; ++count)
    {
      int next = ra[ref_idx].next;
      ra[ref_idx].fdp = ra[idx].fdp;
      ra[ref_idx].mr_type = ra[idx].mr_type;
      ra[ref_idx].mr_type_aux = ra[idx].mr_type_aux;
      ra[ref_idx].type = ra[idx].type;
      ra[ref_idx].name = ra[idx].name;
      ra[ref_idx].unnamed = ra[idx].unnamed;
      ra[ref_idx].non_persistent = ra[idx].non_persistent;
      ra[ref_idx].non_serializable = ra[idx].non_serializable;

      ra[ref_idx].MR_SIZE = ra[idx].MR_SIZE - count * element_size;
      mr_add_child (parent, ref_idx, ra);
      ref_idx = next;
    }
  return (count);
}
  
static int mr_save_inner (void * data, mr_fd_t * fdp, int count, mr_save_data_t * mr_save_data, int parent);

static int
resolve_pointer (mr_save_data_t * mr_save_data, int idx, int parent, int ref_idx, mr_size_t element_size)
{
  mr_ptrdes_t * ra = mr_save_data->ptrs.ra;
  int ref_parent = ra[ref_idx].parent;
  
  if (ra[parent].first_child < 0) /* this is the first element in resizable array */
    {
      if (ra[idx].MR_SIZE <= ra[ref_idx].MR_SIZE)
	{
	  /* new resizable pointer is a part of already saved */
	  ra[parent].ref_idx = ref_idx;
	  ra[ref_idx].flags.is_referenced = true;
	  return (ra[idx].MR_SIZE / element_size);
	}
      /* otherwise we can handle only match with another resizable pointer */
      if (MR_TYPE_POINTER == ra[ref_parent].mr_type)
	{
	  if (ra[ref_idx].prev < 0)
	    /*
	      previously saved resizable pointer was pointing to the same address, but was shorter.
	      we need to reassign nodes to bigger resizable pointer and make a references for shorter one.
	    */
	    return (move_nodes_to_parent (ra, ref_parent, parent, idx, element_size));
	  else
	    {
	      ssize_t size_delta = ra[idx].MR_SIZE - ra[ref_idx].MR_SIZE;
	      int i, count =  size_delta / element_size;
	      char * data = ((char*)ra[ref_idx].data.ptr) + ra[ref_idx].MR_SIZE;
	      int node;
			  
	      /*
		Currently saving resizable pointer is pointing into the middle of previously saved resizable pointer,
		but previously saved pointer is shorter then we need for new one.
		We need to append required number of nodes to previously saved pointer and set new resizable pointer as a references.
	      */
			  
	      ra[parent].ref_idx = ref_idx;
	      ra[ref_idx].flags.is_referenced = true;
			  
	      for (node = ra[ref_parent].first_child; node >= 0; node = ra[node].next)
		ra[node].MR_SIZE += size_delta;
			  
	      for (i = 0; i < count; )
		{
		  int nodes_added = mr_save_inner (data + i * element_size, ra[ref_idx].fdp, count - i, mr_save_data, ref_parent);
		  if (nodes_added <= 0)
		    break;
		  i += nodes_added;
		}
	      return (i);
	    }
	}
    }
  else
    {
      /* we can handle only match with another resizable pointer */
      if (MR_TYPE_POINTER == ra[ref_parent].mr_type)
	{
	  /*
	    in the middle of saving of resizable pointer we matched another resizable pointer
	    we need to append all nodes from found resizable pointer to new one and
	    adjust counters if total length of sequence increased
	  */

	  if (ra[ref_idx].MR_SIZE > ra[idx].MR_SIZE)
	    {
	      int i;
	      ssize_t size_delta = ra[ref_idx].MR_SIZE - ra[idx].MR_SIZE;
	      
	      /* this is required for proper reindexing of nodes that will be moved by move_nodes_to_parent */
	      ra[idx].MR_SIZE = ra[ref_idx].MR_SIZE;
			  
	      for (i = ra[parent].first_child; i >= 0; i = ra[i].next)
		ra[i].MR_SIZE += size_delta;
	    }

	  return (move_nodes_to_parent (ra, ref_parent, parent, idx, element_size));
	}
    }
  
  return (-1);
}

static int
resolve_matched (mr_save_data_t * mr_save_data, int idx, int parent, int ref_idx)
{
  mr_ptrdes_t * ra = mr_save_data->ptrs.ra;
  mr_check_ud_ctx_t mr_check_ud_ctx = {
    .mr_save_data = mr_save_data,
    .parent = parent,
  };
  int nodes_added;
  mr_size_t element_size = mr_type_size (ra[idx].mr_type, ra[idx].type);

  if (element_size == 0)
    return (-1);
  
  for ( ; ref_idx >= 0; ref_idx = ra[ref_idx].save_params.next_typed)
    {
      int ref_parent = ra[ref_idx].parent;

      mr_check_ud_ctx.node = ref_idx;
      mr_status_t status = mr_ud_foreach (&ra[ref_idx], mr_check_ud, &mr_check_ud_ctx);
      if (MR_SUCCESS == status)
	switch (ra[parent].mr_type)
	  {
	  case MR_TYPE_STRING:
	    ra[parent].ref_idx = ref_idx;
	    ra[ref_idx].flags.is_referenced = true;
	    ra[parent].save_params.next_typed = ra[ref_parent].save_params.next_typed;
	    ra[ref_parent].save_params.next_typed = parent;
	    return (0);

	  case MR_TYPE_POINTER:
	    nodes_added = resolve_pointer (mr_save_data, idx, parent, ref_idx, element_size);
	    if (nodes_added >= 0)
	      return (nodes_added);
	    
	  default:
	    if ((ref_parent >= 0)
		&& ((MR_TYPE_STRING == ra[ref_parent].mr_type)
		    || ((MR_TYPE_POINTER == ra[ref_parent].mr_type)
			&& (ra[ref_idx].prev < 0)
			&& (ra[idx].MR_SIZE >= ra[ref_idx].MR_SIZE)))
		&& !ref_is_parent (ra, parent, ref_idx)
		)
	      return (move_nodes_to_parent (ra, ref_parent, parent, idx, element_size));
	  }
    }
  return (-1);
}  

/**
 * Save scheduler. Save any object into internal representation.
 * @param data a pointer on data
 * @param fdp a ponter of field descriptor
 * @param mr_save_data save routines data and lookup structures
 * @param parent index of parent node
 */
static int
mr_save_inner (void * data, mr_fd_t * fdp, int count, mr_save_data_t * mr_save_data, int parent)
{
  intptr_t idx = mr_add_ptr_to_list (&mr_save_data->ptrs); /* add pointer on saving structure to list ptrs */
  mr_ptrdes_t * ra = mr_save_data->ptrs.ra;
  if (idx < 0)
    return (-1); /* memory allocation error occured */

  ra[idx].data.ptr = data;

  ra[idx].fdp = fdp;
  ra[idx].mr_type = fdp->mr_type;
  ra[idx].mr_type_aux = fdp->mr_type_aux;
  ra[idx].type = fdp->type;
  ra[idx].name = fdp->name.str;
  ra[idx].unnamed = fdp->unnamed;
  ra[idx].non_persistent = fdp->non_persistent;
  ra[idx].MR_SIZE = fdp->size * count;

  ra[idx].save_params.next_typed = -1;
  ra[idx].save_params.next_untyped = -1;

  ra[idx].non_serializable = (MR_NON_SERIALIZABLE >> fdp->mr_type) & 1;

  /* forward reference resolving */
  mr_ptr_t * search_result = mr_ic_add (&mr_save_data->typed_ptrs, idx);
  if (NULL == search_result)
    return (-1);
  if (search_result->intptr != idx)
    {
      mr_save_data->ptrs.size -= sizeof (mr_save_data->ptrs.ra[0]);
      int nodes_matched = resolve_matched (mr_save_data, idx, parent, search_result->intptr);
      if (nodes_matched >= 0)
	return (nodes_matched);

      mr_save_data->ptrs.size += sizeof (mr_save_data->ptrs.ra[0]);
      mr_save_data->ptrs.ra[idx].save_params.next_typed = search_result->intptr;
      search_result->intptr = idx;
    }

  search_result = mr_ic_add (&mr_save_data->untyped_ptrs, idx);
  if (NULL == search_result)
    return (-1);
  if (search_result->intptr != idx)
    {
      if (ra[idx].MR_SIZE > ra[search_result->intptr].MR_SIZE)
	{
	  ra[idx].save_params.next_untyped = search_result->intptr;
	  search_result->intptr = idx;
	}
      else
	{
	  ra[idx].save_params.next_untyped = ra[search_result->intptr].save_params.next_untyped;
	  ra[search_result->intptr].save_params.next_untyped = idx;
	}
    }

  mr_add_child (parent, idx, ra);
  /* route saving handler */
  if ((fdp->mr_type < MR_TYPE_LAST) && mr_save_handler[fdp->mr_type])
    {
      int nodes_added = mr_save_handler[fdp->mr_type] (mr_save_data);
      if (nodes_added < 0)
	return (nodes_added);
    }

  return (1);
}

/**
 * MR_TYPE_FUNC & MR_TYPE_FUNC_TYPE type saving handler. Detects NULL pointers.
 * @param mr_save_data save routines data and lookup structures
 */
static int
mr_save_func (mr_save_data_t * mr_save_data)
{
  int idx = mr_save_data->ptrs.size / sizeof (mr_save_data->ptrs.ra[0]) - 1;
  if (NULL == *(void**)mr_save_data->ptrs.ra[idx].data.ptr)
    mr_save_data->ptrs.ra[idx].flags.is_null = true;
  return (1);
}

/**
 * MR_STRING type saving handler. Saves string as internal representation tree node.
 * Detects if string content was already saved.
 * @param mr_save_data save routines data and lookup structures
 */
static int
mr_save_string (mr_save_data_t * mr_save_data)
{
  int idx = mr_save_data->ptrs.size / sizeof (mr_save_data->ptrs.ra[0]) - 1;
  char * str = *(char**)mr_save_data->ptrs.ra[idx].data.ptr;
  if (NULL == str)
    {
      mr_save_data->ptrs.ra[idx].flags.is_null = true;
      return (1);
    }
  mr_fd_t fd_ = *mr_save_data->ptrs.ra[idx].fdp;
  fd_.non_persistent = true;
  fd_.mr_type = MR_TYPE_CHAR_ARRAY;
  fd_.size = sizeof (char);
  fd_.type = "char";
  return (mr_save_inner (str, &fd_, 1, mr_save_data, idx));
}

/**
 * MR_TYPE_ENUM saving handler. Set mr_type_aux to mr_type from type descriptor.
 * @param mr_save_data save routines data and lookup structures
 */
static int
mr_save_enum (mr_save_data_t * mr_save_data)
{
  int idx = mr_save_data->ptrs.size / sizeof (mr_save_data->ptrs.ra[0]) - 1;

  switch (mr_save_data->ptrs.ra[idx].mr_type_aux)
    {
    case MR_TYPE_UINT8:
    case MR_TYPE_INT8:
    case MR_TYPE_UINT16:
    case MR_TYPE_INT16:
    case MR_TYPE_UINT32:
    case MR_TYPE_INT32:
    case MR_TYPE_UINT64:
    case MR_TYPE_INT64:
      break;
    default:
      {
	mr_td_t * tdp = mr_get_td_by_name (mr_save_data->ptrs.ra[idx].type);
	mr_save_data->ptrs.ra[idx].mr_type_aux = tdp ? tdp->param.enum_param.mr_type_effective : MR_TYPE_UINT8;
	break;
      }
    }
  return (1);
}

/**
 * MR_STRUCT type saving handler. Saves structure as internal representation tree node.
 * @param mr_save_data save routines data and lookup structures
 */
static int
mr_save_struct (mr_save_data_t * mr_save_data)
{
  int idx = mr_save_data->ptrs.size / sizeof (mr_save_data->ptrs.ra[0]) - 1;
  mr_td_t * tdp = mr_get_td_by_name (mr_save_data->ptrs.ra[idx].type);
  char * data = mr_save_data->ptrs.ra[idx].data.ptr;
  int i, count;

  if (NULL == tdp) /* check whether type descriptor was found */
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_NO_TYPE_DESCRIPTOR, mr_save_data->ptrs.ra[idx].type);
      return (0);
    }

  if (tdp->mr_type != MR_TYPE_STRUCT)
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_TYPE_NOT_STRUCT, tdp->type.str);
      return (0);
    }

  count = tdp->fields_size / sizeof (tdp->fields[0]);
  for (i = 0; i < count; ++i)
    {
      mr_fd_t * fdp = tdp->fields[i].fdp;
      int nodes_added = mr_save_inner (&data[fdp->offset], fdp, 1, mr_save_data, idx);
      if (nodes_added < 0)
	return (nodes_added);
    }
  return (i);
}

/**
 * MR_UNION type saving handler. Saves structure as internal representation tree node.
 * @param mr_save_data save routines data and lookup structures
 */
static int
mr_save_union (mr_save_data_t * mr_save_data)
{
  int idx = mr_save_data->ptrs.size / sizeof (mr_save_data->ptrs.ra[0]) - 1;
  char * data = mr_save_data->ptrs.ra[idx].data.ptr;
  mr_td_t * tdp = mr_get_td_by_name (mr_save_data->ptrs.ra[idx].type); /* look up for type descriptor */
  mr_fd_t * fdp;

  if (NULL == tdp) /* check whether type descriptor was found */
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_NO_TYPE_DESCRIPTOR, mr_save_data->ptrs.ra[idx].type);
      return (0);
    }
  if ((tdp->mr_type != MR_TYPE_UNION) && (tdp->mr_type != MR_TYPE_ANON_UNION) && (tdp->mr_type != MR_TYPE_NAMED_ANON_UNION))
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_TYPE_NOT_UNION, tdp->type.str);
      return (0);
    }
  
  fdp = mr_union_discriminator (mr_save_data, idx, mr_save_data->ptrs.ra[idx].type, mr_save_data->ptrs.ra[idx].fdp->meta);

  if (NULL != fdp)
    return (mr_save_inner (&data[fdp->offset], fdp, 1, mr_save_data, idx));
  return (0);
}

/**
 * MR_ARRAY type saving handler. Saves array into internal representation.
 * @param mr_save_data save routines data and lookup structures
 */
static int
mr_save_array (mr_save_data_t * mr_save_data)
{
  int idx = mr_save_data->ptrs.size / sizeof (mr_save_data->ptrs.ra[0]) - 1;
  char * data = mr_save_data->ptrs.ra[idx].data.ptr;
  mr_fd_t fd_ = *mr_save_data->ptrs.ra[idx].fdp;
  int count, i;

  fd_.non_persistent = true;
  fd_.unnamed = true;
  fd_.size = mr_type_size (fd_.mr_type_aux, fd_.type);
  if (fd_.size == 0)
    return (1);

  if (1 == fd_.param.array_param.row_count)
    {
      count = fd_.param.array_param.count;
      fd_.mr_type = fd_.mr_type_aux;
    }
  else
    {
      count = fd_.param.array_param.count / fd_.param.array_param.row_count;
      fd_.size *= fd_.param.array_param.row_count;
      fd_.param.array_param.count = fd_.param.array_param.row_count;
      fd_.param.array_param.row_count = 1;
    }

  for (i = 0; i < count; )
    {
      int nodes_added = mr_save_inner (data + i * fd_.size, &fd_, count - i, mr_save_data, idx);
      if (nodes_added <= 0)
	return (nodes_added);
      i += nodes_added;
    }
  return (i);
}

/**
 * Saves pointer into internal representation.
 * @param idx node index
 * @param mr_save_data save routines data and lookup structures
 */
static int
mr_save_pointer_content (int idx, mr_save_data_t * mr_save_data)
{
  char ** data = mr_save_data->ptrs.ra[idx].data.ptr;
  mr_fd_t fd_ = *mr_save_data->ptrs.ra[idx].fdp;
  int count, i;

  fd_.mr_type = fd_.mr_type_aux;
  fd_.non_persistent = true;
  fd_.unnamed = true;
  fd_.size = mr_type_size (fd_.mr_type, fd_.type);
  count = mr_save_data->ptrs.ra[idx].MR_SIZE / fd_.size;

  for (i = 0; i < count; )
    {
      int nodes_added = mr_save_inner (*data + i * fd_.size, &fd_, count - i, mr_save_data, idx);
      if (nodes_added <= 0)
	return (nodes_added);
      i += nodes_added;
    }
  return (i);
}

/**
 * Abstract DFS for pointers descriptors
 * @param mr_ra_ptrdes_t resizable array with pointers descriptors
 * @param processor visitor function
 * @param context untyped pointer on context passed through DFS traverse
 */
mr_status_t
mr_ptrs_dfs (mr_ra_ptrdes_t * ptrs, mr_ptrdes_processor_t processor, void * context)
{
  int level = 0;
  int idx = 0;
  while (idx >= 0)
    {
      if (MR_SUCCESS != processor (ptrs, idx, level, MR_DFS_PRE_ORDER, context))
	return (MR_FAILURE);

      if (ptrs->ra[idx].first_child >= 0)
	{
	  idx = ptrs->ra[idx].first_child;
	  ++level;
	}
      else
	{
	  if (MR_SUCCESS != processor (ptrs, idx, level, MR_DFS_POST_ORDER, context))
	    return (MR_FAILURE);
	  while ((ptrs->ra[idx].next < 0) && (ptrs->ra[idx].parent >= 0))
	    {
	      idx = ptrs->ra[idx].parent;
	      --level;
	      if (MR_SUCCESS != processor (ptrs, idx, level, MR_DFS_POST_ORDER, context))
		return (MR_FAILURE);
	    }
	  idx = ptrs->ra[idx].next;
	}
    }
  return (MR_SUCCESS);
}

/**
 * DFS visitor for nodes renumbering
 * @param mr_ra_ptrdes_t resizable array with pointers descriptors
 * @param idx index of processed node
 * @param context untyped pointer on context passed through DFS traverse
 */
mr_status_t
mr_renumber_node (mr_ra_ptrdes_t * ptrs, int idx, int level, mr_dfs_order_t order, void * context)
{
  if (MR_DFS_PRE_ORDER != order)
    return (MR_SUCCESS);
  int * idx_ = context;
  ptrs->ra[idx].idx = (*idx_)++;
  return (MR_SUCCESS);
}

/**
 * Post process void pointers and strings.
 * @param mr_ra_ptrdes_t resizable array with pointers descriptors
 * @param idx index of processed node
 * @param context untyped pointer on context passed through DFS traverse
 */
static mr_status_t
mr_post_process_node (mr_ra_ptrdes_t * ptrs, int idx, int level, mr_dfs_order_t order, void * context)
{
  if (MR_DFS_PRE_ORDER != order)
    return (MR_SUCCESS);

  mr_save_data_t * mr_save_data = context;

  /* Try resolve void pointers that were not resolved at save time.
     Those pointers might be saved as typed entries on a later stages. */
  if ((MR_TYPE_POINTER == ptrs->ra[idx].mr_type) &&
      ((MR_TYPE_NONE == ptrs->ra[idx].mr_type_aux) || (MR_TYPE_VOID == ptrs->ra[idx].mr_type_aux)) &&
      !ptrs->ra[idx].flags.is_null)
    {
      intptr_t alloc_idx = mr_add_ptr_to_list (ptrs);

      if (alloc_idx < 0)
	return (MR_FAILURE); /* memory allocation error occured */

      /* populate attributes of new node */
      ptrs->ra[alloc_idx].data.ptr = *(void**)ptrs->ra[idx].data.ptr;

      /* this element is required only for a search so we need to adjust back size of collection */
      ptrs->size -= sizeof (ptrs->ra[0]);

      /* search in index of typed references */
      mr_ptr_t * find_result = mr_ic_find (&mr_save_data->untyped_ptrs, alloc_idx);
  
      if (find_result != NULL)
	{
	  /* typed entry was found and here we configure reference on it */
	  int ref_idx = find_result->intptr;
	  ptrs->ra[idx].ref_idx = ref_idx;
	  ptrs->ra[ref_idx].flags.is_referenced = true;
	}
      else
	ptrs->ra[idx].flags.is_null = true; /* unresolved void pointers are saved as NULL */
    }

  /* Save procedure creates additional entries for strings content required for pointers resolution.
     Those additional entries are not required for serialization process and we remove them here, but
     before that we need to update all references on string content.
   */
  if (ptrs->ra[idx].ref_idx >= 0)
    {
      int ref_parent = ptrs->ra[ptrs->ra[idx].ref_idx].parent;
      if ((ref_parent >= 0) && (MR_TYPE_STRING == ptrs->ra[ref_parent].mr_type))
	{
	  /* move ref_idx on a parent node (of type MR_TYPE_STRING) */
	  ptrs->ra[idx].ref_idx = ref_parent;
	  /* mark that this is a reference on content, but not on an entry itself */
	  ptrs->ra[idx].flags.is_content_reference = true;
	  /* mark parent entry as referenced */
	  ptrs->ra[ref_parent].flags.is_referenced = true;
	}
    }

  /* unlink string content, but keep links from content on a parent node */
  if (MR_TYPE_STRING == ptrs->ra[idx].mr_type)
    ptrs->ra[idx].first_child = ptrs->ra[idx].last_child = -1;

  return (MR_SUCCESS);
}

/**
 * Memory allocation failure test for XDR can't properly deallocate strings if it is saved as forward reference.
 * This function makes first reference on a string in DFS traverse as a primary entry and point other references
 * on this entry.
 * @param mr_ra_ptrdes_t resizable array with pointers descriptors
 */
void
mr_reorder_strings (mr_ra_ptrdes_t * ptrs)
{
  int idx, i, count = ptrs->size / sizeof (ptrs->ra[0]);
  for (i = 0; i < count; ++i)
    if ((MR_TYPE_STRING == ptrs->ra[i].mr_type) && (ptrs->ra[i].ref_idx < 0)) /* primary entry for the string */
      {
	int min_idx = i;
	/* iterate over other references on this string and find the one with minimal DFS index */
	for (idx = ptrs->ra[i].save_params.next_typed; idx >= 0; idx = ptrs->ra[idx].save_params.next_typed)
	  if (ptrs->ra[idx].idx < ptrs->ra[min_idx].idx)
	    min_idx = idx;
	
	if (min_idx != i) /* check if reindexing is required */
	  {
	    /* point other references on new primary entry */
	    for (idx = ptrs->ra[i].save_params.next_typed; idx >= 0; idx = ptrs->ra[idx].save_params.next_typed)
	      ptrs->ra[idx].ref_idx = min_idx;

	    /* change old primary entry to be a reference on a new one */
	    ptrs->ra[i].ref_idx = min_idx;
	    ptrs->ra[i].flags.is_referenced = false;
	    ptrs->ra[i].flags.is_content_reference = true;
	    /* configure new primary entry */
	    ptrs->ra[min_idx].ref_idx = -1;
	    ptrs->ra[min_idx].flags.is_referenced = true;
	    ptrs->ra[min_idx].flags.is_content_reference = false;
	  }
      }
}

static mr_status_t
mr_remove_empty_node (mr_ra_ptrdes_t * ptrs, int idx, int level, mr_dfs_order_t order, void * context)
{
#define REMOVE_IF_EMPTY (0 MR_FOREACH (MR_ONE_SHIFT, MR_TYPE_VOID, MR_TYPE_STRUCT, MR_TYPE_ARRAY, MR_TYPE_UNION, MR_TYPE_ANON_UNION, MR_TYPE_NAMED_ANON_UNION))

  if (MR_DFS_POST_ORDER != order)
    return (MR_SUCCESS);

  if (0 == level)
    return (MR_SUCCESS);

  if ((ptrs->ra[idx].first_child < 0) && (ptrs->ra[idx].ref_idx < 0)
      && !ptrs->ra[idx].flags.is_null && !ptrs->ra[idx].flags.is_referenced
      && ((REMOVE_IF_EMPTY >> ptrs->ra[idx].mr_type) & 1))
    {
      bool * need_reindex = context;

      *need_reindex = true;
      /* empty node found - unchain it from parent node */
      if (ptrs->ra[idx].prev < 0) /* node was a first child at parent node */
	ptrs->ra[ptrs->ra[idx].parent].first_child = ptrs->ra[idx].next;
      else
	ptrs->ra[ptrs->ra[idx].prev].next = ptrs->ra[idx].next;

      if (ptrs->ra[idx].next < 0) /* node was a last child at parent node */
	ptrs->ra[ptrs->ra[idx].parent].last_child = ptrs->ra[idx].prev;
      else
	ptrs->ra[ptrs->ra[idx].next].prev = ptrs->ra[idx].prev;
    }

  return (MR_SUCCESS);
}

/**
 * There is no need to save empty nodes and possibly their parent structures 
 * @param mr_ra_ptrdes_t resizable array with pointers descriptors
 */
void
mr_remove_empty_nodes (mr_ra_ptrdes_t * ptrs)
{
  bool need_reindex = false;
  mr_ptrs_dfs (ptrs, mr_remove_empty_node, &need_reindex);
  /* re-enumerate nodes after empty nodes removal */
  if (need_reindex)
    {
      int idx_ = 0;
      mr_ptrs_dfs (ptrs, mr_renumber_node, &idx_);
    }
}

/**
 * Post process void pointers and strings.
 * Set indexes to nodes according to DFS saving sequence.
 * Adjust primary etries for strings to be first in DFS sequence.
 * @param mr_save_data save routines data and lookup structures
 */
static void
mr_post_process (mr_save_data_t * mr_save_data)
{
  int idx_ = 0;
  mr_ptrs_dfs (&mr_save_data->ptrs, mr_post_process_node, mr_save_data);
  mr_ptrs_dfs (&mr_save_data->ptrs, mr_renumber_node, &idx_); /* enumeration of nodes should be done only after strings processing */
}

/**
 * MR_POINTER_STRUCT type saving handler. Save referenced structure into internal representation.
 * @param mr_save_data save routines data and lookup structures
 */
static int
mr_save_pointer (mr_save_data_t * mr_save_data)
{
  int idx = mr_save_data->ptrs.size / sizeof (mr_save_data->ptrs.ra[0]) - 1;
  void ** data = mr_save_data->ptrs.ra[idx].data.ptr;

  if (NULL == *data)
    mr_save_data->ptrs.ra[idx].flags.is_null = true; /* return empty node if pointer is NULL */
  else
    {
      mr_type_t mr_type = mr_save_data->ptrs.ra[idx].mr_type_aux;
      mr_ptrdes_t src, dst;
      /* at first attempt to save pointer we need to determine size of structure */
      mr_size_t element_size = mr_type_size (mr_type, mr_save_data->ptrs.ra[idx].fdp->type);
      mr_save_data->ptrs.ra[idx].MR_SIZE = element_size;
      
      /* pointers might have assosiated field with the size for resizable arrays.
	 name of the size field is stored in 'res' of field meta-data.
	 'res_type' in this case will be 'string' */
      
      mr_pointer_get_size_ptrdes (&src, idx, &mr_save_data->ptrs);
      if (src.data.ptr != NULL)
	{
	  dst.data.ptr = &mr_save_data->ptrs.ra[idx].MR_SIZE;
	  dst.mr_type = MR_TYPE_DETECT (__typeof__ (mr_save_data->ptrs.ra[idx].MR_SIZE));
	  mr_assign_int (&dst, &src);
	}

      if (0 == strcmp (mr_save_data->ptrs.ra[idx].name, MR_OPAQUE_DATA_STR))
	{
	  if (mr_save_data->ptrs.ra[idx].MR_SIZE <= 0)
	    mr_save_data->ptrs.ra[idx].flags.is_null = true;
	  else
	    mr_save_data->ptrs.ra[idx].flags.is_opaque_data = true;
	}
      else if ((0 == element_size) || (mr_save_data->ptrs.ra[idx].MR_SIZE < element_size))
	mr_save_data->ptrs.ra[idx].flags.is_null = true;
      else if ((mr_type != MR_TYPE_NONE) && (mr_type != MR_TYPE_VOID)) /* look ahead optimization for void pointers */
	{
	  int * idx_ = mr_rarray_allocate_element ((void*)&mr_save_data->mr_ra_idx,
						   &mr_save_data->mr_ra_idx_size, &mr_save_data->mr_ra_idx_alloc_size, 
						   sizeof (mr_save_data->mr_ra_idx[0]));
	  if (NULL == idx_)
	    return (-1);
	  *idx_ = idx;
	}
    }
  return (1);
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
  mr_res_t context = {
    .data = { &mr_save_data->ptrs },
    .type = "mr_ra_ptrdes_t",
    .MR_SIZE = sizeof (mr_ra_ptrdes_t),
  };

  memset (mr_save_data, 0, sizeof (*mr_save_data));
  mr_save_data->ptrs.ptrdes_type = MR_PD_SAVE;
#define MR_IC_METHOD MR_IC_HASH
  mr_ic_new (&mr_save_data->typed_ptrs, mr_typed_ptrdes_get_hash, mr_typed_ptrdes_cmp, "intptr", MR_IC_METHOD, &context);
  mr_ic_new (&mr_save_data->untyped_ptrs, mr_untyped_ptrdes_get_hash, mr_untyped_ptrdes_cmp, "intptr", MR_IC_METHOD, &context);
  mr_ic_new (&mr_save_data->union_discriminators, mr_uds_get_hash, mr_uds_cmp, "intptr", MR_IC_METHOD, &context);

  mr_save_data->mr_ra_ud_size = 0;
  mr_save_data->mr_ra_ud = NULL;
  mr_save_data->mr_ra_idx_size = 0;
  mr_save_data->mr_ra_idx = NULL;
  mr_save_data->ptrs.size = 0;
  mr_save_data->ptrs.ra = NULL;

  fdp->unnamed = true;
  int nodes_added = mr_save_inner (data, fdp, 1, mr_save_data, -1);
  if (nodes_added > 0)
    {
      while (mr_save_data->mr_ra_idx_size > 0)
	{
	  mr_save_data->mr_ra_idx_size -= sizeof (mr_save_data->mr_ra_idx[0]);
	  nodes_added = mr_save_pointer_content (mr_save_data->mr_ra_idx[mr_save_data->mr_ra_idx_size / sizeof (mr_save_data->mr_ra_idx[0])], mr_save_data);
	  if (nodes_added < 0)
	    break;
	}
      if (nodes_added >= 0)
	mr_post_process (mr_save_data);
    }

  for (i = mr_save_data->ptrs.size / sizeof (mr_save_data->ptrs.ra[0]) - 1; i >= 0; --i)
    mr_ud_free (&mr_save_data->ptrs.ra[i]);

  if ((nodes_added < 0) && (mr_save_data->ptrs.ra != NULL))
    {
      MR_FREE (mr_save_data->ptrs.ra);
      mr_save_data->ptrs.ra = NULL;
    }

  if (mr_save_data->mr_ra_ud != NULL)
    MR_FREE (mr_save_data->mr_ra_ud);
  mr_save_data->mr_ra_ud = NULL;
  
  if (mr_save_data->mr_ra_idx != NULL)
    MR_FREE (mr_save_data->mr_ra_idx);
  mr_save_data->mr_ra_idx = NULL;

  mr_ic_free (&mr_save_data->union_discriminators);
  mr_ic_free (&mr_save_data->typed_ptrs);
  mr_ic_free (&mr_save_data->untyped_ptrs);
}

/**
 * Init IO handlers Table
 */
static mr_save_handler_t mr_save_handler[MR_TYPE_LAST] =
  {
    [MR_TYPE_FUNC] = mr_save_func,
    [MR_TYPE_FUNC_TYPE] = mr_save_func,
    [MR_TYPE_STRING] = mr_save_string,
    [MR_TYPE_ENUM] = mr_save_enum,
    [MR_TYPE_STRUCT] = mr_save_struct,
    [MR_TYPE_ARRAY] = mr_save_array,
    [MR_TYPE_POINTER] = mr_save_pointer,
    [MR_TYPE_UNION] = mr_save_union,
    [MR_TYPE_ANON_UNION] = mr_save_union,
    [MR_TYPE_NAMED_ANON_UNION] = mr_save_union,
  };
