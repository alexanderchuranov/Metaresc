/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

#include <metaresc.h>
#include <mr_ic.h>
#include <mr_stringify.h>
#include <mr_save.h>

TYPEDEF_FUNC (mr_idx_t, mr_save_handler_t, (mr_save_data_t *))

static mr_save_handler_t mr_save_handler[];

static inline mr_fd_t *
mr_union_discriminator_by_idx (mr_td_t * tdp, int idx)
{
  /* check that field index in union is valid and reset to default otherwise */
  if ((idx < 0) || (idx >= tdp->param.union_param.fields_size / sizeof (tdp->param.union_param.fields[0])))
    idx = 0;
  if (tdp->param.union_param.fields_size > 0) /* check for an empty union */
    return (tdp->param.struct_param.fields[idx]);
  else
    return (NULL);
}

static inline mr_fd_t *
mr_union_discriminator_by_name (mr_td_t * tdp, char * name)
{
  if (name != NULL)
    if (name[0])
      {
	mr_fd_t * fdp = mr_get_fd_by_name (tdp, name);
	if (NULL != fdp)
	  return (fdp);
      }
  
  if (tdp->param.union_param.fields_size > 0) /* check for an empty union */
    return (tdp->param.union_param.fields[0]);
  else
    return (NULL);
}

static inline mr_fd_t *
mr_ud_override_value (mr_ic_t * ud_overrides, uint64_t value)
{
  if (NULL == ud_overrides)
    return (NULL);
  
  uintptr_t key = (uintptr_t)&value - offsetof (mr_ud_override_t, value); /* (mr_ud_override_t[]){{ .value = value }}; */
  mr_ptr_t * result = mr_ic_find (ud_overrides, key);
  mr_ud_override_t * udo = result ? result->ptr : NULL;
  return (udo ? udo->fdp : NULL);
}

static void
mr_init_pointer_fd (mr_fd_t ** fdp, mr_fd_t * ptr_fdp)
{
  *ptr_fdp = **fdp;
  ptr_fdp->stype.mr_type = ptr_fdp->stype.mr_type_aux;
  ptr_fdp->stype.mr_type_aux = ptr_fdp->mr_type_base;
  ptr_fdp->stype.mr_type_class = MR_POINTER_TYPE_CLASS;
  ptr_fdp->stype.size = sizeof (void*);
  ptr_fdp->offset = 0;
  ptr_fdp->unnamed = true;
  *fdp = ptr_fdp;
}

static mr_fd_t *
mr_union_discriminator_by_type (mr_td_t * tdp, mr_fd_t * parent_fdp, void * discriminator, mr_ic_t * ud_overrides)
{
  mr_type_t mr_type = parent_fdp->stype.mr_type;
  mr_fd_t fd;
  for (;;)
    {
      switch (mr_type) /* switch over basic types */
	{
	case MR_TYPE_BOOL:
	  {
	    int idx = !!*(uint8_t*)discriminator;
	    mr_fd_t * fdp = mr_ud_override_value (ud_overrides, idx);
	    if (fdp)
	      return (fdp);
	    return (mr_union_discriminator_by_idx (tdp, idx));
	  }

#define CASE_RETURN_BY_TYPE(TYPE) CASE_RETURN_BY_TYPE_ (TYPE, MR_TYPE_DETECT (TYPE))
#define CASE_RETURN_BY_TYPE_(TYPE, MR_TYPE)				\
	  case MR_TYPE:							\
	    {								\
	      mr_fd_t * fdp = mr_ud_override_value (ud_overrides, *(TYPE*)discriminator); \
	      if (fdp)							\
		return (fdp);						\
	      return (mr_union_discriminator_by_idx (tdp, *(TYPE*)discriminator)); \
	    }

	  MR_FOREACH (CASE_RETURN_BY_TYPE, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t);
	  CASE_RETURN_BY_TYPE_ (mr_uintmax_t, MR_TYPE_UINT128);
	  CASE_RETURN_BY_TYPE_ (mr_intmax_t, MR_TYPE_INT128);

	case MR_TYPE_BITFIELD:
	  {
	    mr_uintmax_t value = 0;
	    mr_ptrdes_t ptrdes = { .data.ptr = discriminator, .fdp = parent_fdp, };
	    mr_td_t * enum_tdp = parent_fdp->stype.tdp;

	    mr_save_bitfield_value (&ptrdes, &value); /* get value of the bitfield */

	    mr_fd_t * fdp = mr_ud_override_value (ud_overrides, value);
	    if (fdp)
	      return (fdp);
	  
	    if (enum_tdp)
	      if (MR_TYPE_ENUM == enum_tdp->mr_type)
		{
		  /* if bitfield is a enumeration then get named discriminator from enum value meta field */
		  mr_ed_t * edp = mr_get_enum_by_value (enum_tdp, value);
		  return (mr_union_discriminator_by_name (tdp, edp ? edp->meta : NULL));
		}
	    
	    return (mr_union_discriminator_by_idx (tdp, value));
	  }
	
	case MR_TYPE_CHAR_ARRAY:
	  return (mr_union_discriminator_by_name (tdp, (char*)discriminator));
	
	case MR_TYPE_STRING:
	  return (mr_union_discriminator_by_name (tdp, *(char**)discriminator));
	
	case MR_TYPE_ENUM:
	  {
	    mr_td_t * enum_tdp = parent_fdp->stype.tdp;
	    if (NULL == enum_tdp)
	      return (mr_union_discriminator_by_name (tdp, NULL));

	    mr_enum_value_type_t value = mr_get_enum_value (enum_tdp, discriminator);
	    mr_fd_t * fdp = mr_ud_override_value (ud_overrides, value);
	    if (fdp)
	      return (fdp);
	    mr_ed_t * edp = mr_get_enum_by_value (enum_tdp, value); /* get named discriminator from enum value meta field */
	    return (mr_union_discriminator_by_name (tdp, edp ? edp->meta : NULL));
	  }
	    
	case MR_TYPE_ARRAY:
	  if (parent_fdp->stype.mr_type_aux != MR_TYPE_POINTER)
	    {
	      mr_type = parent_fdp->stype.mr_type_aux;
	      break;
	    }

	  mr_init_pointer_fd (&parent_fdp, &fd);
	  __attribute__ ((fallthrough));
	  /* NB! proceed to pointer branch */
	case MR_TYPE_POINTER:
	  /* if discriminator is a pointer then we need address of the content */
	  discriminator = *(void**)discriminator;
	  if (NULL == discriminator)
	    return (mr_union_discriminator_by_name (tdp, NULL));
	  mr_type = parent_fdp->stype.mr_type_aux;
	  if (MR_TYPE_POINTER == mr_type)
	    mr_init_pointer_fd (&parent_fdp, &fd);
	  break;
	    
	case MR_TYPE_VOID:
	  mr_type = parent_fdp->stype.mr_type_aux;
#define NONRESOLVABLE (0 MR_FOREACH (MR_ONE_SHIFT, MR_TYPE_VOID, MR_TYPE_ARRAY, MR_TYPE_POINTER))
	  if ((NONRESOLVABLE >> mr_type) & 1)
	    mr_type = MR_TYPE_NONE;
	  break;

	case MR_TYPE_STRUCT:
	case MR_TYPE_UNION:
	case MR_TYPE_ANON_UNION:
	case MR_TYPE_NAMED_ANON_UNION:
	  if (parent_fdp->stype.tdp)
	    if ((parent_fdp->stype.tdp->param.union_param.fields_size >= sizeof (parent_fdp->stype.tdp->param.union_param.fields[0])) &&
		parent_fdp->stype.tdp->is_union_discriminator)
	      {
		discriminator = (char*)discriminator + parent_fdp->stype.tdp->param.union_param.fields[0]->offset;
		parent_fdp = parent_fdp->stype.tdp->param.union_param.fields[0];
		mr_type = parent_fdp->stype.mr_type;
		break;
	      }
	  __attribute__ ((fallthrough));
	  /* NB! proceed to default branch */
	default:
	  mr_type = MR_TYPE_NONE;
	  break;
	}
      
      /* prevent from infinit redirect loop */
      if (MR_TYPE_NONE == mr_type)
	break;
    }
  
  return (mr_union_discriminator_by_name (tdp, NULL));
}

int
mr_uds_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const mr_save_data_t * mr_save_data = context;
  const mr_union_discriminator_t * x_ud = &mr_save_data->mr_ra_ud[x.uintptr];
  const mr_union_discriminator_t * y_ud = &mr_save_data->mr_ra_ud[y.uintptr];
  int diff = (x_ud->union_fdp > y_ud->union_fdp) - (x_ud->union_fdp < y_ud->union_fdp);
  if (diff)
    return (diff);
  return ((x_ud->discriminated_fdp > y_ud->discriminated_fdp) -
	  (x_ud->discriminated_fdp < y_ud->discriminated_fdp));
}

mr_hash_value_t
mr_uds_get_hash (mr_ptr_t x, const void * context)
{
  const mr_save_data_t * mr_save_data = context;
  const mr_union_discriminator_t * x_ud = &mr_save_data->mr_ra_ud[x.uintptr];
  return ((uintptr_t)x_ud->union_fdp + (uintptr_t)x_ud->discriminated_fdp);
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
  const mr_union_discriminator_t * x_ud = &mr_save_data->mr_ra_ud[x.uintptr];
  const mr_union_discriminator_t * y_ud = &mr_save_data->mr_ra_ud[y.uintptr];
  return ((x_ud->union_fdp > y_ud->union_fdp) - (x_ud->union_fdp < y_ud->union_fdp));
}

mr_hash_value_t
mr_ud_get_hash (mr_ptr_t x, const void * context)
{
  const mr_save_data_t * mr_save_data = context;
  const mr_union_discriminator_t * x_ud = &mr_save_data->mr_ra_ud[x.uintptr];
  return ((uintptr_t)x_ud->union_fdp);
}

static int
mr_ud_find (mr_ud_set_t * uds, mr_ptr_t key, mr_save_data_t * mr_save_data)
{
  if (uds->is_ic)
    {
      mr_ptr_t * find = mr_ic_find (uds->union_discriminator, key);
      return (find ? find->uintptr : -1);
    }

  int i, count = uds->size / sizeof (uds->idx[0]);
  for (i = 0; i < count; ++i)
    if (0 == mr_ud_cmp (key, (intptr_t)uds->idx[i], mr_save_data))
      return (uds->idx[i]);
  return (-1);
}

static int
mr_ud_add (mr_ud_set_t * uds, mr_ptr_t key, mr_save_data_t * mr_save_data)
{
  if (uds->is_ic)
    {
      mr_ptr_t * add = mr_ic_add (uds->union_discriminator, key);
      return (add ? add->uintptr : -1);
    }

  int find = mr_ud_find (uds, key, mr_save_data);
  if (find != -1)
    return (find);

  if (uds->size > sizeof (uds->idx))
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_NUMBER_OF_ITEMS, uds->size / sizeof (uds->idx[0]));
      uds->size = 0;
    }

  if (uds->size == sizeof (uds->idx))
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
      mr_ptr_t idx[sizeof (uds->idx) / sizeof (uds->idx[0])];

      for (i = 0; i < sizeof (idx) / sizeof (idx[0]); ++i)
	idx[i].uintptr = uds->idx[i];

      status = mr_ic_index (dst_ic, idx, sizeof (idx));
      if (MR_SUCCESS != status)
	{
	  MR_FREE (dst_ic);
	  return (-1);
	}

      uds->union_discriminator = dst_ic;
      uds->is_ic = true;
      mr_ptr_t * add = mr_ic_add (dst_ic, key);
      return (add ? add->uintptr : -1);
    }

  int idx = uds->size / sizeof (uds->idx[0]);
  uds->idx[idx] = key.uintptr;
  uds->size += sizeof (uds->idx[0]);

  return (key.uintptr);
}

static mr_status_t
mr_ud_foreach (mr_ud_set_t * uds, mr_visit_fn_t visit_fn, const void * context)
{
  if (uds->is_ic)
    return (mr_ic_foreach (uds->union_discriminator, visit_fn, context));

  int i, count = uds->size / sizeof (uds->idx[0]);
  for (i = 0; i < count; ++i)
    if (MR_SUCCESS != visit_fn ((intptr_t)uds->idx[i], context))
      return (MR_FAILURE);
  return (MR_SUCCESS);
}

static void
mr_ud_free (mr_ud_set_t * uds)
{
  if (uds->is_ic)
    {
      mr_ic_free (uds->union_discriminator);
      MR_FREE (uds->union_discriminator);
      uds->union_discriminator = NULL;
    }
  uds->is_ic = false;
  uds->size = 0;
}

/**
 * Finds out union discriminator
 * @param mr_save_data save routines data and lookup structures
 */
static mr_fd_t *
mr_union_discriminator (mr_save_data_t * mr_save_data, mr_idx_t node, mr_fd_t * union_fdp)
{
  mr_fd_t * fdp = NULL; /* marker that no valid discriminator was found */
  mr_idx_t parent, idx;
  intptr_t ud_idx, ud_find = -1;
  mr_union_discriminator_t * ud;
  char * discriminator = union_fdp->meta;
  mr_td_t * tdp = union_fdp->stype.tdp;

  if (NULL == tdp)
    return (NULL);

  /* if union meta field is a valid field name, then traverse thruogh parents and look for union discriminator */
  if (mr_get_any_fd_by_name (discriminator, NULL) == NULL)
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
  if ((ud_idx >> __CHAR_BIT__) >= (1UL << (__CHAR_BIT__ * (sizeof (MR_RA_UD_IDX_TYPE) - 1))))
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_TOO_MANY_UD, MR_STRINGIFY_READONLY (MR_RA_UD_IDX_TYPE));
      return (NULL);
    }
  ud->union_fdp = union_fdp; /* union field descriptor */

  /* traverse through parents up to root node */
  for (parent = node; parent != MR_NULL_IDX; parent = mr_save_data->ptrs.ra[parent].parent)
    {
      mr_ptrdes_t * parent_ptrdes = &mr_save_data->ptrs.ra[parent];
      /* checks if this parent already have union resolution info */
      ud_find = mr_ud_find (&parent_ptrdes->save_params.ud_set, ud_idx, mr_save_data);
      /* break the traverse loop if it has */
      if (ud_find != -1)
	break;

      if ((MR_TYPE_ARRAY == parent_ptrdes->mr_type) || (MR_TYPE_POINTER == parent_ptrdes->mr_type))
	continue;

      /* check if for some reason parent node doesn't have static field descriptor */
      if (NULL == parent_ptrdes->fdp)
	continue;
      /* check if for some reason parent node doesn't have cached type descriptor */
      if (NULL == parent_ptrdes->fdp->stype.tdp)
	continue;
      /* lookup for discriminator in this parent */
      mr_fd_t * parent_fdp = mr_get_fd_by_name (parent_ptrdes->fdp->stype.tdp, discriminator);
      if (NULL == parent_fdp)
	continue;
      
      /* get an address of discriminator field */
      void * discriminator_ptr = (char*)parent_ptrdes->data.ptr + parent_fdp->offset;
      fdp = mr_union_discriminator_by_type (tdp, parent_fdp, discriminator_ptr, &union_fdp->union_param);
      break;
    }

  if (-1 != ud_find)
    fdp = mr_save_data->mr_ra_ud[ud_find].discriminated_fdp; /* union discriminator info was found in some of the parents */
  else
    {
      /* if union is still not resolved, set it to default branch */
      if (NULL == fdp)
	fdp = mr_union_discriminator_by_name (tdp, NULL);

      /* form union discriminator resolution entity and deduplicate it with existing */
      ud->discriminated_fdp = fdp;
      mr_ptr_t * add = mr_ic_add (&mr_save_data->union_discriminators, ud_idx);
      if (NULL == add)
	return (NULL);
      if (add->uintptr == ud_idx)
	/* union discriminator info was not found in the set so we add new record */
	mr_save_data->mr_ra_ud_size += sizeof (mr_save_data->mr_ra_ud[0]);

      /* set 'parent' on one level up to include actual parent node into traversal below */
      if (parent != MR_NULL_IDX)
	parent = mr_save_data->ptrs.ra[parent].parent;
      ud_find = add->uintptr;
    }

  /* add union discriminator information to all parents which doesn't have it yet */
  for (idx = node; idx != parent; idx = mr_save_data->ptrs.ra[idx].parent)
    if (-1 == mr_ud_add (&mr_save_data->ptrs.ra[idx].save_params.ud_set, ud_find, mr_save_data))
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
    case MR_TYPE_INT128:
    case MR_TYPE_UINT128:
    case MR_TYPE_FLOAT:
    case MR_TYPE_COMPLEX_FLOAT:
    case MR_TYPE_DOUBLE:
    case MR_TYPE_COMPLEX_DOUBLE:
    case MR_TYPE_LONG_DOUBLE:
    case MR_TYPE_COMPLEX_LONG_DOUBLE:
    case MR_TYPE_FUNC_TYPE:
    case MR_TYPE_FUNC:
      break;
      
    case MR_TYPE_ARRAY:
    case MR_TYPE_POINTER:
    case MR_TYPE_BITFIELD:
      return ((x > y) - (x < y));
      
    case MR_TYPE_STRUCT:
    case MR_TYPE_ENUM:
    case MR_TYPE_UNION:
    case MR_TYPE_ANON_UNION:
    case MR_TYPE_NAMED_ANON_UNION:
      if ((NULL == x->fdp) || (NULL == y->fdp))
	return ((x->fdp > y->fdp) - (x->fdp < y->fdp));
      return ((x->fdp->stype.tdp > y->fdp->stype.tdp) - (x->fdp->stype.tdp < y->fdp->stype.tdp));
      
    case MR_TYPE_END_ANON_UNION:
    case MR_TYPE_LAST:
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_MR_TYPE);
      break;
    }
  return (0);
}

#define MR_PTR_HASH_MASK (~3)

mr_hash_value_t
mr_typed_ptrdes_get_hash (const mr_ptr_t x, const void * context)
{
  const mr_ra_ptrdes_t * ra_ptrdes = context;
  return (ra_ptrdes->ra[x.uintptr].data.uintptr & MR_PTR_HASH_MASK);
}

int
mr_untyped_ptrdes_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const mr_ra_ptrdes_t * ra_ptrdes = context;
  uintptr_t x_ptr = ra_ptrdes->ra[x.uintptr].data.uintptr & MR_PTR_HASH_MASK;
  uintptr_t y_ptr = ra_ptrdes->ra[y.uintptr].data.uintptr & MR_PTR_HASH_MASK;
  return ((x_ptr > y_ptr) - (x_ptr < y_ptr));
}

TYPEDEF_STRUCT (mr_check_ud_ctx_t,
		(mr_save_data_t *, mr_save_data),
		(mr_idx_t, node),
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
  mr_ra_ptrdes_t * ptrs = &mr_save_data->ptrs;
  mr_idx_t idx = ptrs->size / sizeof (ptrs->ra[0]);
  mr_idx_t parent = ptrs->ra[idx].parent;
  mr_union_discriminator_t * ud = &mr_save_data->mr_ra_ud[key.uintptr];
  /* mr_ra_ud would be reallocaed within this function, so we need to get values from this node */
  mr_fd_t * discriminated_fdp = ud->discriminated_fdp;
  mr_fd_t * fdp = ptrs->ra[mr_check_ud_ctx->node].fdp;
  mr_td_t * tdp = fdp ? fdp->stype.tdp : NULL;

  /* here we check that union discriminator was resolved at the level of saved node.
     This means that resolution is not dependant on upper tree */
  if (NULL != tdp)
    if (NULL != mr_get_fd_by_name (tdp, ud->union_fdp->meta /* discriminator */))
      return (MR_SUCCESS);
  
  /* otherwise we need to find union resolution in the context of new parent */
  fdp = mr_union_discriminator (mr_save_data, parent, ud->union_fdp);
  
  return ((fdp == discriminated_fdp) ? MR_SUCCESS : MR_FAILURE);
}

static bool
is_first_child (mr_ptrdes_t * ra, mr_idx_t node)
{
  mr_idx_t parent = ra[node].parent;
  if (parent == MR_NULL_IDX)
    return (false);
  return (ra[parent].first_child == node);
}

static bool
ref_is_parent (mr_ptrdes_t * ra, mr_idx_t node, mr_idx_t ref_idx)
{
  while ((node != MR_NULL_IDX) && (node != ref_idx))
    node = ra[node].parent;
  return (node == ref_idx);
}

static mr_fd_t *
mr_get_persistent_fd (mr_ptrdes_t * ra, mr_idx_t idx)
{
  mr_fd_t * fdp = ra[idx].fdp;
  if (fdp->non_persistent) /* replace non persistent field descriptor on persistent */
    {
      mr_idx_t parent;
      for (parent = ra[idx].parent; parent != MR_NULL_IDX; parent = ra[parent].parent)
	if (ra[parent].fdp)
	  if (!ra[parent].fdp->non_persistent)
	    break;
      if (parent != MR_NULL_IDX)
	fdp = ra[parent].fdp;
      else
	fdp = fdp->stype.tdp ? &fdp->stype.tdp->mr_ptr_fd : NULL;
    }
  return (fdp);
}

static mr_idx_t
move_nodes_to_parent (mr_ra_ptrdes_t * ptrs, mr_idx_t ref_parent, mr_idx_t idx)
{
  mr_ptrdes_t * ra = ptrs->ra;
  mr_idx_t count, ref_idx = ra[ref_parent].first_child;
  mr_idx_t parent = ra[idx].parent;
  mr_size_t element_size = ra[idx].fdp ? ra[idx].fdp->stype.size : 0;
    
  ra[ref_parent].first_child = ref_idx;
  ra[ref_parent].flags |= MR_IS_REFERENCE;
  ra[ref_idx].flags |= MR_IS_REFERENCED;
			  
  for (count = 0; ref_idx != MR_NULL_IDX; ++count)
    {
      mr_idx_t next = ra[ref_idx].next;
      ra[ref_idx].fdp = mr_get_persistent_fd (ra, idx);
      ra[ref_idx].mr_type = ra[idx].mr_type;
      ra[ref_idx].mr_type_aux = ra[idx].mr_type_aux;
      ra[ref_idx].flags &= ~MR_IS_UNNAMED;
      ra[ref_idx].flags |= ra[idx].flags & MR_IS_UNNAMED;

      ra[ref_idx].MR_SIZE = ra[idx].MR_SIZE - count * element_size;
      mr_add_child (ptrs, parent, ref_idx);
      ref_idx = next;
    }
  return (count);
}
  
static mr_idx_t mr_save_inner (void * data, mr_fd_t * fdp, mr_idx_t count, mr_save_data_t * mr_save_data, mr_idx_t parent);

/**
 * Saves pointer into internal representation.
 * @param idx node index
 * @param mr_save_data save routines data and lookup structures
 */
static mr_idx_t
mr_save_pointer_content (mr_idx_t idx, mr_save_data_t * mr_save_data)
{
  mr_ptrdes_t * ptrdes = &mr_save_data->ptrs.ra[idx];
  char ** data = ptrdes->data.ptr;
  mr_fd_t fd_;

  memset (&fd_, 0, sizeof (fd_));
  fd_.stype.mr_type = ptrdes->mr_type_aux;
  fd_.stype.tdp = ptrdes->fdp ? ptrdes->fdp->stype.tdp : NULL;
  fd_.stype.mr_type_aux = ptrdes->fdp ? ptrdes->fdp->mr_type_base : MR_TYPE_VOID;

  fd_.non_persistent = true;
  fd_.unnamed = true;
  fd_.stype.size = mr_type_size (fd_.stype.mr_type);
  if (fd_.stype.size == 0)
    fd_.stype.size = fd_.stype.tdp ? fd_.stype.tdp->size : 0;
  if (fd_.stype.size == 0)
    return (0);

  mr_idx_t i = 0;
  mr_idx_t count = ptrdes->MR_SIZE / fd_.stype.size;
  mr_idx_t node;

  for (node = mr_save_data->ptrs.ra[idx].first_child; node != MR_NULL_IDX; node = mr_save_data->ptrs.ra[node].next)
    {
      mr_size_t mr_size = (count - i) * fd_.stype.size;
      if (mr_save_data->ptrs.ra[node].MR_SIZE < mr_size)
	mr_save_data->ptrs.ra[node].MR_SIZE = mr_size;
      if (++i >= count)
	break;
    }

  while (i < count)
    {
      mr_idx_t nodes_added = mr_save_inner (*data + i * fd_.stype.size, &fd_, count - i, mr_save_data, idx);
      if (nodes_added == 0)
	return (0);
      i += nodes_added;
    }
  return (i);
}

static mr_idx_t
resolve_pointer (mr_save_data_t * mr_save_data, mr_idx_t ref_idx, bool * resolved)
{
  mr_ra_ptrdes_t * ptrs = &mr_save_data->ptrs;
  mr_ptrdes_t * ra = ptrs->ra;
  mr_idx_t idx = ptrs->size / sizeof (ptrs->ra[0]);
  mr_idx_t parent = ra[idx].parent;
  mr_idx_t ref_parent = ra[ref_idx].parent;
  mr_size_t element_size = ra[idx].fdp->stype.size;

  *resolved = false;
  if (element_size == 0)
    return (0);
  
  if (MR_NULL_IDX == ra[parent].first_child) /* this is the first element in resizable array */
    {
      if (ra[idx].MR_SIZE <= ra[ref_idx].MR_SIZE)
	{
	  *resolved = true;
	  /* new resizable pointer is a part of already saved */
	  ra[parent].first_child = ref_idx;
	  ra[parent].flags |= MR_IS_REFERENCE;
	  ra[ref_idx].flags |= MR_IS_REFERENCED;
	  return (ra[idx].MR_SIZE / element_size);
	}
      /* otherwise we can handle only match with another resizable pointer */
      if (MR_TYPE_POINTER == ra[ref_parent].mr_type)
	{
	  *resolved = true;
	  if (is_first_child (ra, ref_idx))
	    /*
	      previously saved resizable pointer was pointing to the same address, but was shorter.
	      we need to reassign nodes to bigger resizable pointer and make a references for shorter one.
	    */
	    return (move_nodes_to_parent (ptrs, ref_parent, idx));
	  else
	    {
	      typeof (ra[idx].MR_SIZE) size_delta = ra[idx].MR_SIZE - ra[ref_idx].MR_SIZE;
	      mr_idx_t total_required = ra[idx].MR_SIZE / element_size;
	      /*
		Currently saving resizable pointer is pointing into the middle of previously saved resizable pointer,
		but previously saved pointer is shorter then we need for new one.
		We need to append required number of nodes to previously saved pointer and set new resizable pointer as a references.
	      */
	      ra[ref_parent].MR_SIZE += size_delta;
	      ra[parent].first_child = ref_idx;
	      ra[parent].flags |= MR_IS_REFERENCE;
	      ra[ref_idx].flags |= MR_IS_REFERENCED;
	      mr_idx_t nodes_added = mr_save_pointer_content (ref_parent, mr_save_data);
	      if (nodes_added == 0)
		return (0);
	      return (total_required);
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
	  *resolved = true;
	  if (ra[ref_idx].MR_SIZE > ra[idx].MR_SIZE)
	    {
	      mr_idx_t node;
	      typeof (ra[idx].MR_SIZE) size_delta = ra[ref_idx].MR_SIZE - ra[idx].MR_SIZE;
	      
	      /* this is required for proper reindexing of nodes that will be moved by move_nodes_to_parent */
	      ra[idx].MR_SIZE = ra[ref_idx].MR_SIZE;
			  
	      for (node = ra[parent].first_child; node != MR_NULL_IDX; node = ra[node].next)
		ra[node].MR_SIZE += size_delta; /* increase size for forward saved elements in resizable array */

	      ra[parent].MR_SIZE += size_delta; /* increase size of resizable array on detected delta */
	    }

	  return (move_nodes_to_parent (ptrs, ref_parent, idx));
	}
    }
  
  return (0);
}

static mr_idx_t
resolve_matched (mr_save_data_t * mr_save_data, mr_idx_t ref_idx, bool * resolved)
{
  mr_ra_ptrdes_t * ptrs = &mr_save_data->ptrs;
  mr_ptrdes_t * ra = ptrs->ra;
  mr_idx_t idx = ptrs->size / sizeof (ptrs->ra[0]);
  mr_idx_t parent = ra[idx].parent;
  mr_check_ud_ctx_t mr_check_ud_ctx = { .mr_save_data = mr_save_data, };

  for ( ; ref_idx != MR_NULL_IDX; ref_idx = ra[ref_idx].save_params.next_untyped)
    {
      mr_idx_t ref_parent = ra[ref_idx].parent;

      if (mr_cmp_ptrdes (&ra[ref_idx], &ra[idx]) != 0) /* skip pointers that typed differently */
	continue;

      mr_check_ud_ctx.node = ref_idx;
      mr_status_t status = mr_ud_foreach (&ra[ref_idx].save_params.ud_set, mr_check_ud, &mr_check_ud_ctx);

      if (MR_SUCCESS == status)
	switch (ra[parent].mr_type)
	  {
	  case MR_TYPE_STRING:
	    *resolved = true;
	    ra[parent].first_child = ref_idx;
	    ra[parent].flags |= MR_IS_REFERENCE;
	    ra[ref_idx].flags |= MR_IS_REFERENCED;
	    return (1);

	  case MR_TYPE_POINTER:
	    {
	      mr_idx_t nodes_added = resolve_pointer (mr_save_data, ref_idx, resolved);
	      if (resolved)
		return (nodes_added);
	      break;
	    }
	    
	  default:
	    if (ref_parent != MR_NULL_IDX)
	      if (((MR_TYPE_STRING == ra[ref_parent].mr_type)
		   || ((MR_TYPE_POINTER == ra[ref_parent].mr_type)
		       && is_first_child (ra, ref_idx)
		       && (ra[idx].MR_SIZE >= ra[ref_idx].MR_SIZE)))
		  && !ref_is_parent (ra, parent, ref_idx))
		{
		  *resolved = true;
		  return (move_nodes_to_parent (ptrs, ref_parent, idx));
		}
	    break;
	  }
    }
  *resolved = false;
  return (0);
}

void
mr_assign_int (mr_ptrdes_t * dst, mr_ptrdes_t * src)
{
  mr_uintmax_t value = 0;
  mr_type_t mr_type;
  void * src_data = src->data.ptr;
  void * dst_data = dst->data.ptr;

  if (MR_TYPE_POINTER == src->mr_type)
    src_data = *(void**)src_data;

  if (NULL == src_data)
    return;
  
  if (MR_TYPE_POINTER == dst->mr_type)
    dst_data = *(void**)dst_data;

  if (NULL == dst_data)
    return;
  
  if ((MR_TYPE_VOID == src->mr_type) || (MR_TYPE_POINTER == src->mr_type))
    mr_type = src->mr_type_aux;
  else
    mr_type = src->mr_type;

  switch (mr_type)
    {
    case MR_TYPE_VOID:
    case MR_TYPE_ENUM:
      {
	mr_td_t * tdp = src->fdp ? src->fdp->stype.tdp : NULL;
	if (NULL == tdp)
	  break;
	if (tdp->mr_type != MR_TYPE_ENUM)
	  break;
	
	switch (tdp->param.enum_param.mr_type_effective)
	  {
#define GET_VALUE_BY_TYPE(TYPE) GET_VALUE_BY_TYPE_ (TYPE, MR_TYPE_DETECT (TYPE))
#define GET_VALUE_BY_TYPE_(TYPE, MR_TYPE) case MR_TYPE: value = *(TYPE*)src_data; break;
	    MR_FOREACH (GET_VALUE_BY_TYPE, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t);
#ifdef HAVE_INT128
	    MR_FOREACH (GET_VALUE_BY_TYPE, mr_uint128_t, mr_int128_t);
#endif /* HAVE_INT128 */
	  default:
	    break;
	  }
	break;
      }
      
      MR_FOREACH (GET_VALUE_BY_TYPE, bool, char, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t);
      GET_VALUE_BY_TYPE_ (mr_uintmax_t, MR_TYPE_UINT128);
      GET_VALUE_BY_TYPE_ (mr_intmax_t, MR_TYPE_INT128);

    case MR_TYPE_BITFIELD:
      mr_save_bitfield_value (src, &value); /* get value of the bitfield */
      break;
    default:
      break;
    }

  if ((MR_TYPE_VOID == dst->mr_type) || (MR_TYPE_POINTER == dst->mr_type))
    mr_type = dst->mr_type_aux;
  else
    mr_type = dst->mr_type;

  switch (mr_type)
    {
    case MR_TYPE_VOID:
    case MR_TYPE_ENUM:
      {
	mr_td_t * tdp = dst->fdp ? dst->fdp->stype.tdp : NULL;
	if (NULL == tdp)
	  break;
	if (tdp->mr_type != MR_TYPE_ENUM)
	  break;
	
	switch (tdp->param.enum_param.mr_type_effective)
	  {
#define SET_VALUE_BY_TYPE(TYPE) SET_VALUE_BY_TYPE_ (TYPE, MR_TYPE_DETECT (TYPE))
#define SET_VALUE_BY_TYPE_(TYPE, MR_TYPE) case MR_TYPE: *(TYPE*)dst_data = value; break;
	    MR_FOREACH (SET_VALUE_BY_TYPE, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t);
#ifdef HAVE_INT128
	    MR_FOREACH (SET_VALUE_BY_TYPE, mr_uint128_t, mr_int128_t);
#endif /* HAVE_INT128 */
	  default:
	    break;
	  }
	break;
      }

      MR_FOREACH (SET_VALUE_BY_TYPE, bool, char, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t);
      SET_VALUE_BY_TYPE_ (mr_uintmax_t, MR_TYPE_UINT128);
      SET_VALUE_BY_TYPE_ (mr_intmax_t, MR_TYPE_INT128);

    case MR_TYPE_BITFIELD:
      mr_load_bitfield_value (dst, &value); /* set value of the bitfield */
      break;
    default:
      break;
    }
}

static mr_fd_t *
mr_get_fd_by_offset (mr_td_t * tdp, __typeof__ (((mr_fd_t*)0)->offset) offset)
{
  unsigned idx;
  uintptr_t key = (uintptr_t)&offset - offsetof (mr_fd_t, offset); /* (mr_fd_t[]){{ .offset = offset }}; */
  mr_ic_rarray_t ic_rarray = { .ra = (mr_ptr_t*)tdp->param.struct_param.fields, .size = tdp->param.struct_param.fields_size, };
  int diff = mr_ic_sorted_array_find_idx (key, &ic_rarray, mr_fd_offset_cmp, NULL, &idx);
  /* do binary search first */
  if (diff)
    return (NULL);
  /*
    There might be zero size fields declared before designated field with specific offset.
    We need to skip them and find last relevant field in the ordered array.
    Iterate to the last field with the requested offset.
  */
  unsigned count = tdp->param.struct_param.fields_size / sizeof (tdp->param.struct_param.fields[0]);
  while ((idx + 1 < count) && (tdp->param.struct_param.fields[idx + 1]->offset == offset))
    ++idx;
  return (tdp->param.struct_param.fields[idx]);
}

void
mr_pointer_get_size_ptrdes (mr_ptrdes_t * ptrdes, mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  char * name = NULL;
  memset (ptrdes, 0, sizeof (*ptrdes));

  mr_fd_t * fdp = ptrs->ra[idx].fdp;
  if (fdp->res_type != NULL)
    {
      if (0 == strcmp ("offset", fdp->res_type))
	name = ""; /* detect case if size field defined by offset */
      else if (0 == strcmp ("string", fdp->res_type))
	if (mr_get_any_fd_by_name (fdp->res.string, NULL) != NULL)
	  name = fdp->res.string; /* detect case if size field defined by name */
    }
  
  /* quit if size field is not defined */  
  if (NULL == name)
    return;

  mr_idx_t parent;
  /* traverse through parents up to first structure */
  for (parent = ptrs->ra[idx].parent; parent != MR_NULL_IDX; parent = ptrs->ra[parent].parent)
    if (MR_TYPE_STRUCT == ptrs->ra[parent].mr_type)
      break;
  
  /* quit if parent structure was not found */    
  if (MR_NULL_IDX == parent)
    return;

  mr_fd_t * parent_fdp;
  mr_td_t * parent_tdp = ptrs->ra[parent].fdp ? ptrs->ra[parent].fdp->stype.tdp : NULL;
  /* quit if structure type descriptor is missing */
  if (NULL == parent_tdp)
    return;

  /* lookup for a size field in this parent */
  if (0 == name[0])
    parent_fdp = mr_get_fd_by_offset (parent_tdp, fdp->res.offset);
  else
    parent_fdp = mr_get_fd_by_name (parent_tdp, name);
  /* quit if size field was not found in parent structure */
  if (NULL == parent_fdp)
    return;

  ptrdes->fdp = parent_fdp;
  ptrdes->mr_type = parent_fdp->stype.mr_type;
  ptrdes->mr_type_aux = parent_fdp->stype.mr_type_aux;
  ptrdes->data.ptr = (char*)ptrs->ra[parent].data.ptr + parent_fdp->offset; /* get an address of size field */
}

/**
 * Save scheduler. Save any object into internal representation.
 * @param data a pointer on data
 * @param fdp a ponter of field descriptor
 * @param mr_save_data save routines data and lookup structures
 * @param parent index of parent node
 */
static mr_idx_t
mr_save_inner (void * data, mr_fd_t * fdp, mr_idx_t count, mr_save_data_t * mr_save_data, mr_idx_t parent)
{
  mr_idx_t idx = mr_add_ptr_to_list (&mr_save_data->ptrs); /* add pointer on saving structure to list ptrs */
  if (MR_NULL_IDX == idx)
    return (0); /* memory allocation error occured */
  mr_ptrdes_t * ptrdes = &mr_save_data->ptrs.ra[idx];

  ptrdes->data.ptr = data;
  ptrdes->fdp = fdp;
  ptrdes->mr_type = fdp->stype.mr_type;
  ptrdes->mr_type_aux = fdp->stype.mr_type_aux;
  ptrdes->flags = fdp->unnamed ? MR_IS_UNNAMED : MR_NO_FLAGS;
  ptrdes->MR_SIZE = fdp->stype.size * count;
  ptrdes->parent = parent;
  ptrdes->first_child = MR_NULL_IDX;
  ptrdes->next = MR_NULL_IDX;
  ptrdes->save_params.next_untyped = MR_NULL_IDX;
  ptrdes->save_params.ud_set.size = 0;
  ptrdes->save_params.ud_set.is_ic = false;

  /* forward reference resolving */
  mr_ptr_t * search_result = mr_ic_add (&mr_save_data->untyped_ptrs, (uintptr_t)idx);
  if (NULL == search_result)
    return (0);
  if (search_result->uintptr != idx)
    {
      mr_save_data->ptrs.size -= sizeof (mr_save_data->ptrs.ra[0]);
      bool resolved = false;
      mr_idx_t nodes_matched = resolve_matched (mr_save_data, search_result->uintptr, &resolved);
      if (resolved)
	return (nodes_matched);

      mr_save_data->ptrs.size += sizeof (mr_save_data->ptrs.ra[0]);

      mr_save_data->ptrs.ra[idx].save_params.next_untyped = search_result->uintptr;
      search_result->uintptr = idx;
    }

  mr_add_child (&mr_save_data->ptrs, parent, idx);

  /* route saving handler */
  mr_save_handler_t save_handler = NULL;
  if ((fdp->stype.mr_type >= 0) && (fdp->stype.mr_type < MR_TYPE_LAST))
    save_handler = mr_save_handler[fdp->stype.mr_type];

  mr_idx_t nodes_added = 1;
  if (save_handler)
    nodes_added = save_handler (mr_save_data);

  mr_save_data->ptrs.ra[idx].fdp = mr_get_persistent_fd (mr_save_data->ptrs.ra, idx);

  return (nodes_added);
}

/**
 * MR_TYPE_FUNC & MR_TYPE_FUNC_TYPE type saving handler. Detects NULL pointers.
 * @param mr_save_data save routines data and lookup structures
 */
static mr_idx_t
mr_save_func (mr_save_data_t * mr_save_data)
{
  mr_idx_t idx = mr_save_data->ptrs.size / sizeof (mr_save_data->ptrs.ra[0]) - 1;
  if (NULL == *(void**)mr_save_data->ptrs.ra[idx].data.ptr)
    mr_save_data->ptrs.ra[idx].flags |= MR_IS_NULL;
  return (1);
}

/**
 * MR_STRING type saving handler. Saves string as internal representation tree node.
 * Detects if string content was already saved.
 * @param mr_save_data save routines data and lookup structures
 */
static mr_idx_t
mr_save_string (mr_save_data_t * mr_save_data)
{
  mr_idx_t idx = mr_save_data->ptrs.size / sizeof (mr_save_data->ptrs.ra[0]) - 1;
  char * str = *(char**)mr_save_data->ptrs.ra[idx].data.ptr;
  if (NULL == str)
    {
      mr_save_data->ptrs.ra[idx].flags |= MR_IS_NULL;
      return (1);
    }
  mr_fd_t fd_ = *mr_save_data->ptrs.ra[idx].fdp;
  fd_.non_persistent = true;
  fd_.stype.mr_type = MR_TYPE_CHAR_ARRAY;
  fd_.stype.size = sizeof (char);
  fd_.stype.type = "char";
  return (mr_save_inner (str, &fd_, 1, mr_save_data, idx));
}

/**
 * MR_TYPE_ENUM saving handler. Set mr_type_aux to mr_type from type descriptor.
 * @param mr_save_data save routines data and lookup structures
 */
static mr_idx_t
mr_save_enum (mr_save_data_t * mr_save_data)
{
  mr_idx_t idx = mr_save_data->ptrs.size / sizeof (mr_save_data->ptrs.ra[0]) - 1;
  mr_ptrdes_t * ptrdes = &mr_save_data->ptrs.ra[idx];
  mr_td_t * tdp = ptrdes->fdp->stype.tdp;

#define MR_INT_TYPES (0 MR_FOREACH (MR_ONE_SHIFT, MR_TYPE_UINT8, MR_TYPE_INT8, MR_TYPE_UINT16, MR_TYPE_INT16, MR_TYPE_UINT32, MR_TYPE_INT32, MR_TYPE_UINT64, MR_TYPE_INT64, MR_TYPE_UINT128, MR_TYPE_INT128))
  if (!((MR_INT_TYPES >> ptrdes->mr_type_aux) & 1))
    ptrdes->mr_type_aux = tdp ? tdp->param.enum_param.mr_type_effective : MR_TYPE_UINT8;
  return (1);
}

/**
 * MR_STRUCT type saving handler. Saves structure as internal representation tree node.
 * @param mr_save_data save routines data and lookup structures
 */
static mr_idx_t
mr_save_struct (mr_save_data_t * mr_save_data)
{
  mr_idx_t idx = mr_save_data->ptrs.size / sizeof (mr_save_data->ptrs.ra[0]) - 1;
  mr_td_t * tdp = mr_save_data->ptrs.ra[idx].fdp->stype.tdp;
  char * data = mr_save_data->ptrs.ra[idx].data.ptr;
  mr_idx_t i, count = tdp->param.struct_param.fields_size / sizeof (tdp->param.struct_param.fields[0]);
  for (i = 0; i < count; ++i)
    {
      mr_fd_t * fdp = tdp->param.struct_param.fields[i];
      mr_idx_t nodes_added = mr_save_inner (&data[fdp->offset], fdp, 1, mr_save_data, idx);
      if (nodes_added == 0)
	return (0);
    }
  return (1);
}

/**
 * MR_UNION type saving handler. Saves structure as internal representation tree node.
 * @param mr_save_data save routines data and lookup structures
 */
static mr_idx_t
mr_save_union (mr_save_data_t * mr_save_data)
{
  mr_idx_t parent, idx = mr_save_data->ptrs.size / sizeof (mr_save_data->ptrs.ra[0]) - 1;
  mr_td_t * union_tdp = mr_save_data->ptrs.ra[idx].fdp->stype.tdp;
  char * data = mr_save_data->ptrs.ra[idx].data.ptr;

  for (parent = idx; parent != MR_NULL_IDX; parent = mr_save_data->ptrs.ra[parent].parent)
    if (mr_save_data->ptrs.ra[parent].fdp)
      if (!mr_save_data->ptrs.ra[parent].fdp->non_persistent)
	break;
      
  mr_fd_t * discriminated_fdp = NULL;
  if (parent != MR_NULL_IDX)
    discriminated_fdp = mr_union_discriminator (mr_save_data, idx, mr_save_data->ptrs.ra[parent].fdp);
  else if (union_tdp != NULL)
    discriminated_fdp = mr_union_discriminator_by_idx (union_tdp, 0);

  if (NULL != discriminated_fdp)
    return (mr_save_inner (&data[discriminated_fdp->offset], discriminated_fdp, 1, mr_save_data, idx));
  return (1);
}

/**
 * MR_ARRAY type saving handler. Saves array into internal representation.
 * @param mr_save_data save routines data and lookup structures
 */
static mr_idx_t
mr_save_array (mr_save_data_t * mr_save_data)
{
  mr_idx_t idx = mr_save_data->ptrs.size / sizeof (mr_save_data->ptrs.ra[0]) - 1;
  char * data = mr_save_data->ptrs.ra[idx].data.ptr;
  mr_fd_t fd_ = *mr_save_data->ptrs.ra[idx].fdp;
  mr_idx_t i, count = fd_.stype.dim.dim[0];

  if (0 == count)
    return (1);
  if (0 == fd_.stype.size)
    return (0);

  fd_.non_persistent = true;
  fd_.unnamed = true;
  fd_.offset = 0;
  fd_.stype.size /= count;

  if (fd_.stype.dim.size == sizeof (fd_.stype.dim.dim[0]))
    {
      fd_.stype.mr_type = fd_.stype.mr_type_aux;
      fd_.stype.mr_type_aux = fd_.mr_type_base;
      if (!mr_save_data->ptrs.ra[idx].fdp->non_persistent)
	{
	  mr_ptrdes_t src, dst;
	  mr_pointer_get_size_ptrdes (&src, idx, &mr_save_data->ptrs);
	  if (src.data.ptr != NULL)
	    {
	      ssize_t size;
	      dst.data.ptr = &size;
	      dst.mr_type = MR_TYPE_DETECT (typeof (size));
	      mr_assign_int (&dst, &src);
	      if (size <= 0)
		count = 0;
	      else
		{
		  mr_idx_t count_ = size / fd_.stype.size;
		  if (count > count_)
		    count = count_;
		}
	    }
	  mr_save_data->ptrs.ra[idx].MR_SIZE = count * fd_.stype.size;
	}
    }
  else
    {
      fd_.stype.dim.size -= sizeof (fd_.stype.dim.dim[0]);
      memmove (&fd_.stype.dim.dim[0], &fd_.stype.dim.dim[0], fd_.stype.dim.size);
    }

  for (i = 0; i < count; )
    {
      mr_idx_t nodes_added = mr_save_inner (data + i * fd_.stype.size, &fd_, count - i, mr_save_data, idx);
      if (nodes_added == 0)
	return (0);
      i += nodes_added;
    }
  return (1);
}

/**
 * Abstract DFS for pointers descriptors
 * @param mr_ra_ptrdes_t resizable array with pointers descriptors
 * @param processor visitor function
 * @param context untyped pointer on context passed through DFS traverse
 */
mr_status_t
mr_ptrs_dfs_impl (mr_ra_ptrdes_t * ptrs, mr_ptrdes_processor_t processor, mr_ptr_t context, mr_idx_t start)
{
  int level = 0;
  mr_idx_t idx = start;
  while (idx != MR_NULL_IDX)
    {
      if (MR_SUCCESS != processor (ptrs, idx, level, MR_DFS_PRE_ORDER, context))
	return (MR_FAILURE);

      if ((ptrs->ra[idx].first_child != MR_NULL_IDX) && !(ptrs->ra[idx].flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE)))
	{
	  idx = ptrs->ra[idx].first_child;
	  ++level;
	}
      else
	{
	  if (MR_SUCCESS != processor (ptrs, idx, level, MR_DFS_POST_ORDER, context))
	    return (MR_FAILURE);
	  while ((ptrs->ra[idx].next == MR_NULL_IDX) && (ptrs->ra[idx].parent != MR_NULL_IDX))
	    {
	      idx = ptrs->ra[idx].parent;
	      --level;
	      if (MR_SUCCESS != processor (ptrs, idx, level, MR_DFS_POST_ORDER, context))
		return (MR_FAILURE);
	      if (idx == start)
		return (MR_SUCCESS);
	    }
	  idx = ptrs->ra[idx].next;
	}
    }
  return (MR_SUCCESS);
}

static mr_status_t
mr_reorder_strings_visitor (mr_ra_ptrdes_t * ptrs, mr_idx_t idx, int level, mr_dfs_order_t order, void * context)
{
  if (MR_DFS_POST_ORDER != order)
    return (MR_SUCCESS);

  mr_idx_t ref_idx = ptrs->ra[idx].first_child;
  if ((MR_TYPE_STRING == ptrs->ra[idx].mr_type) && (ptrs->ra[idx].flags & MR_IS_CONTENT_REFERENCE))
    {
      if (ptrs->ra[ref_idx].flags & MR_IS_CONTENT_REFERENCE)
	ptrs->ra[idx].first_child = ptrs->ra[ref_idx].first_child;
      else if (ptrs->ra[idx].idx < ptrs->ra[ref_idx].idx)
	{
	  ptrs->ra[idx].first_child = MR_NULL_IDX;
	  ptrs->ra[idx].flags |= MR_IS_REFERENCED;
	  ptrs->ra[idx].flags &= ~MR_IS_CONTENT_REFERENCE;
	  ptrs->ra[ref_idx].first_child = idx;
	  ptrs->ra[ref_idx].flags &= ~MR_IS_REFERENCED;
	  ptrs->ra[ref_idx].flags |= MR_IS_CONTENT_REFERENCE;
	}
    }
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
  if ((NULL == ptrs) || (NULL == ptrs->ra))
    return;
  mr_ptrs_dfs (ptrs, mr_reorder_strings_visitor, NULL);
  mr_idx_t i, count = ptrs->size / sizeof (ptrs->ra[0]);
  for (i = 1; i < count; ++i)
    {
      mr_idx_t ref_idx = ptrs->ra[i].first_child;
      if ((MR_TYPE_POINTER == ptrs->ra[i].mr_type) &&
	  (ptrs->ra[i].flags & MR_IS_CONTENT_REFERENCE) &&
	  (ptrs->ra[ref_idx].flags & MR_IS_CONTENT_REFERENCE))
	ptrs->ra[i].first_child = ptrs->ra[ref_idx].first_child;
    }
}

#define REMOVE_IF_EMPTY (0 MR_FOREACH (MR_ONE_SHIFT, MR_TYPE_VOID, MR_TYPE_STRUCT, MR_TYPE_ARRAY, MR_TYPE_UNION, MR_TYPE_ANON_UNION, MR_TYPE_NAMED_ANON_UNION))

static mr_status_t
mr_remove_empty_node (mr_ra_ptrdes_t * ptrs, mr_idx_t idx, int level, mr_dfs_order_t order, void * context)
{
  if (MR_DFS_POST_ORDER != order)
    return (MR_SUCCESS);

  if (!(ptrs->ra[idx].flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE)))
    {
      mr_idx_t * next = &ptrs->ra[idx].first_child;

      while (*next != MR_NULL_IDX)
	if ((ptrs->ra[*next].first_child == MR_NULL_IDX)
	    && !(ptrs->ra[*next].flags & (MR_IS_NULL | MR_IS_REFERENCED))
	    && ((REMOVE_IF_EMPTY >> ptrs->ra[*next].mr_type) & 1))
	  {
	    bool * need_reindex = context;
	    *need_reindex = true;
	    /* empty node found - unchain it from previous node */
	    *next = ptrs->ra[*next].next;
	  }
	else
	  next = &ptrs->ra[*next].next;
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
mr_renumber_node (mr_ra_ptrdes_t * ptrs, mr_idx_t idx, int level, mr_dfs_order_t order, void * context)
{
  if (MR_DFS_PRE_ORDER != order)
    return (MR_SUCCESS);
  mr_idx_t * idx_ = context;
  ptrs->ra[idx].idx = (*idx_)++;
  return (MR_SUCCESS);
}

/**
 * There is no need to save empty nodes and possibly their parent structures 
 * @param mr_ra_ptrdes_t resizable array with pointers descriptors
 */
void
mr_remove_empty_nodes (mr_ra_ptrdes_t * ptrs)
{
  bool need_reindex_empty = false;
  mr_ptrs_dfs (ptrs, mr_remove_empty_node, &need_reindex_empty);
  /* re-enumerate nodes after empty nodes removal */
  if (need_reindex_empty)
    {
      mr_idx_t idx = 1;
      mr_ptrs_dfs (ptrs, mr_renumber_node, &idx);
    }
}

/**
 * Post process void pointers and strings.
 * @param mr_save_data saved graph with indexes
 * @param idx index of processed node
 */
static inline mr_status_t
resolve_void_ptr_and_strings (mr_save_data_t * mr_save_data, mr_idx_t idx)
{
  mr_ra_ptrdes_t * ptrs = &mr_save_data->ptrs;
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  
  /* Try resolve void pointers that were not resolved at save time.
     Those pointers might be saved as typed entries on a later stages. */
  switch (ptrdes->mr_type)
    {
    case MR_TYPE_POINTER:
      if (((MR_TYPE_NONE == ptrdes->mr_type_aux) || (MR_TYPE_VOID == ptrdes->mr_type_aux)) &&
	  !(ptrdes->flags & MR_IS_NULL))
	{
	  void * data_ptr = *(void**)ptrdes->data.ptr;
	  mr_idx_t alloc_idx = mr_add_ptr_to_list (ptrs);

	  if (MR_NULL_IDX == alloc_idx)
	    return (MR_FAILURE); /* memory allocation error occured */
	  ptrdes = &ptrs->ra[idx]; /* ptrs->ra might be reallocated in mr_add_ptr_to_list */
	  ptrdes->flags |= MR_IS_NULL; /* void pointers are saved as NULL */

	  /* populate attributes of new node */
	  ptrs->ra[alloc_idx].data.ptr = data_ptr;

	  /* this element is required only for a search so we need to adjust back size of collection */
	  ptrs->size -= sizeof (ptrs->ra[0]);

	  /* search in index of typed references */
	  mr_ptr_t * find_result = mr_ic_find (&mr_save_data->untyped_ptrs, (uintptr_t)alloc_idx);
	  if (NULL == find_result)
	    break;

	  /* typed entry was found and here we configure reference on it */
	  /* as we put multiple addresses into one bucket,
	     we need to traverse through the list in this bucket and
	     filter out only entries with matching address */
	  mr_idx_t bucket, ref_idx = MR_NULL_IDX;
	  for (bucket = find_result->uintptr; bucket != MR_NULL_IDX; bucket = ptrs->ra[bucket].save_params.next_untyped)
	    if (ptrs->ra[bucket].data.ptr == data_ptr)
	      {
		if (MR_NULL_IDX == ref_idx)
		  ref_idx = bucket; /* first entry with the matched address */
		else if (ptrs->ra[bucket].MR_SIZE > ptrs->ra[ref_idx].MR_SIZE)
		  ref_idx = bucket; /* another entry, but bigger in size */
	      }
	  if (MR_NULL_IDX == ref_idx)
	    break;

	  ptrdes->first_child = ref_idx;
	  ptrdes->flags |= MR_IS_REFERENCE;
	  ptrs->ra[ref_idx].flags |= MR_IS_REFERENCED;
	}
      break;
      /* unlink string content, but keep links from content on a parent node */
    case MR_TYPE_STRING:
      if (!(ptrdes->flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE)))
	ptrdes->first_child = MR_NULL_IDX;
      break;
    case MR_TYPE_CHAR_ARRAY:
#define MR_VECTOR_TYPES (0 MR_FOREACH (MR_ONE_SHIFT, MR_TYPE_ARRAY, MR_TYPE_POINTER))
      if (ptrdes->parent != MR_NULL_IDX) /* in array of MR_TYPE_CHAR_ARRAY adjust MR_SIZE to a size of individual element */
	if (((MR_VECTOR_TYPES >> ptrs->ra[ptrdes->parent].mr_type) & 1) &&
	    (ptrdes->next != MR_NULL_IDX))
	  ptrdes->MR_SIZE -= ptrs->ra[ptrdes->next].MR_SIZE;
      break;
    default:
      break;
    }

  /* Save procedure creates additional entries for strings content required for pointers resolution.
     Those additional entries are not required for serialization process and we remove them here, but
     before that we need to update all references on string content.
   */
  if (ptrdes->flags & MR_IS_REFERENCE)
    {
      mr_idx_t ref_parent = ptrs->ra[ptrdes->first_child].parent;
      if (ref_parent != MR_NULL_IDX)
	if (MR_TYPE_STRING == ptrs->ra[ref_parent].mr_type)
	  {
	    /* move ref_idx on a parent node (of type MR_TYPE_STRING) */
	    ptrdes->first_child = ref_parent;
	    /* mark that this is a reference on content, but not on an entry itself */
	    ptrdes->flags &= ~MR_IS_REFERENCE;
	    ptrdes->flags |= MR_IS_CONTENT_REFERENCE;
	    /* mark parent entry as referenced */
	    ptrs->ra[ref_parent].flags |= MR_IS_REFERENCED;
	  }
    }

  return (MR_SUCCESS);
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
  mr_idx_t i, count = mr_save_data->ptrs.size / sizeof (mr_save_data->ptrs.ra[0]);
  for (i = 1; i < count; ++i)
    resolve_void_ptr_and_strings (mr_save_data, i);
  i = 1;
  mr_ptrs_dfs (&mr_save_data->ptrs, mr_renumber_node, &i); /* enumeration of nodes should be done only after strings processing */
}

/**
 * MR_POINTER_STRUCT type saving handler. Save referenced structure into internal representation.
 * @param mr_save_data save routines data and lookup structures
 */
static mr_idx_t
mr_save_pointer (mr_save_data_t * mr_save_data)
{

  mr_idx_t idx = mr_save_data->ptrs.size / sizeof (mr_save_data->ptrs.ra[0]) - 1;
  void ** data = mr_save_data->ptrs.ra[idx].data.ptr;

  if (NULL == *data)
    mr_save_data->ptrs.ra[idx].flags |= MR_IS_NULL; /* return empty node if pointer is NULL */
  else
    {
      mr_ptrdes_t src, dst;
      mr_ptrdes_t * ptrdes = &mr_save_data->ptrs.ra[idx];
      /* at first attempt to save pointer we need to determine size of structure */
      mr_size_t element_size = mr_type_size (ptrdes->mr_type_aux);
      mr_td_t * tdp = ptrdes->fdp ? ptrdes->fdp->stype.tdp : NULL;
      if (0 == element_size)
	element_size = tdp ? tdp->size : 0;
      ptrdes->MR_SIZE = element_size;
      
      /* pointers might have assosiated field with the size for resizable arrays.
	 name of the size field is stored in 'res' of field meta-data.
	 'res_type' in this case will be 'string' */
      
      mr_pointer_get_size_ptrdes (&src, idx, &mr_save_data->ptrs);
      if (src.data.ptr != NULL)
	{
	  dst.data.ptr = &ptrdes->MR_SIZE;
	  dst.mr_type = MR_TYPE_DETECT (typeof (ptrdes->MR_SIZE));
	  mr_assign_int (&dst, &src);
	}

      if (ptrdes->fdp->name.str != NULL)
	if (0 == strcmp (ptrdes->fdp->name.str, MR_OPAQUE_DATA_STR))
	  {
	    if (ptrdes->MR_SIZE == 0)
	      ptrdes->flags |= MR_IS_NULL;
	    else
	      ptrdes->flags |= MR_IS_OPAQUE_DATA;
	  }

      if (!(ptrdes->flags & MR_IS_OPAQUE_DATA) &&
	  ((0 == element_size) || (ptrdes->MR_SIZE < element_size)))
	ptrdes->flags |= MR_IS_NULL;
    }
  return (1);
}  

/**
 * Public function. Calls save scheduler and frees lookup tables.
 * @param data a pointer on data
 * @param fdp a ponter of field descriptor
 * @param mr_save_data save routines data and lookup structures
 */
mr_ra_ptrdes_t
mr_save (void * data, mr_fd_t * fdp)
{
  mr_save_data_t mr_save_data;
  typeof (mr_save_data.ptrs.size) i;
  mr_res_t context = {
    .data = { &mr_save_data.ptrs },
    .type = "mr_ra_ptrdes_t",
    .MR_SIZE = sizeof (mr_ra_ptrdes_t),
  };

  memset (&mr_save_data, 0, sizeof (mr_save_data));
  if (NULL == data)
    return (mr_save_data.ptrs);

  mr_add_ptr_to_list (&mr_save_data.ptrs); /* add first descriptor that plays a role of NULL pointer */
  if (NULL == mr_save_data.ptrs.ra)
    return (mr_save_data.ptrs);

  mr_save_data.ptrs.ptrdes_type = MR_PD_SAVE;
#define MR_IC_METHOD MR_IC_HASH
  mr_ic_new (&mr_save_data.untyped_ptrs, mr_typed_ptrdes_get_hash, mr_untyped_ptrdes_cmp, "uintptr", MR_IC_METHOD, &context);
  mr_ic_new (&mr_save_data.union_discriminators, mr_uds_get_hash, mr_uds_cmp, "uintptr", MR_IC_METHOD, &context);

  fdp->unnamed = true;
  mr_idx_t nodes_added = mr_save_inner (data, fdp, 1, &mr_save_data, MR_NULL_IDX);

  if (nodes_added > 0)
    {
      for (i = 1; i < mr_save_data.ptrs.size / sizeof (mr_save_data.ptrs.ra[0]); ++i)
	if ((MR_TYPE_POINTER == mr_save_data.ptrs.ra[i].mr_type) &&
	    (MR_TYPE_VOID != mr_save_data.ptrs.ra[i].mr_type_aux) &&
	    (MR_TYPE_NONE != mr_save_data.ptrs.ra[i].mr_type_aux) &&
	    !(mr_save_data.ptrs.ra[i].flags & (MR_IS_NULL | MR_IS_OPAQUE_DATA)))
	  {
	    nodes_added = mr_save_pointer_content (i, &mr_save_data);
	    if (nodes_added == 0)
	      break;
	  }
      if (nodes_added > 0)
	mr_post_process (&mr_save_data);
    }

  for (i = mr_save_data.ptrs.size / sizeof (mr_save_data.ptrs.ra[0]) - 1; i > 0; --i)
    mr_ud_free (&mr_save_data.ptrs.ra[i].save_params.ud_set);

  if ((nodes_added == 0) && (mr_save_data.ptrs.ra != NULL))
    {
      MR_FREE (mr_save_data.ptrs.ra);
      mr_save_data.ptrs.ra = NULL;
    }

  if (mr_save_data.mr_ra_ud != NULL)
    MR_FREE (mr_save_data.mr_ra_ud);

  mr_ic_free (&mr_save_data.union_discriminators);
  mr_ic_free (&mr_save_data.untyped_ptrs);

  return (mr_save_data.ptrs);
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
