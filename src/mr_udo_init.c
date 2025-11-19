#include <metaresc.h>
#include <mr_save.h>
#include <mr_ic.h>
#include <mr_udo_init.h>

TYPEDEF_STRUCT (mr_type_ref_t, ATTRIBUTES ( , "type desciptor and list of type indexes that reffers this type"),
		(mr_td_t *, tdp, , "type defscriptor"),
		(unsigned *, ref, , "list of type indexes that reffer to this type",
		 { .size_field_offset = offsetof (mr_type_ref_t, size), }, "size_field_offset"),
		(ssize_t, size, , "size of ref list"),
		(ssize_t, alloc_size, , "allocated size of ref list"),
		);

TYPEDEF_STRUCT (mr_types_dep_t, ATTRIBUTES ( , "graph of types dependencies"),
		(mr_type_ref_t *, types, , "all struct and unions",
		 { .size_field_offset = offsetof (mr_types_dep_t, size), }, "size_field_offset"),
		(ssize_t, size, , "size of types list"),
		(ssize_t, alloc_size, , "allocated size of types list"),
		(mr_ic_t, index, , "lookup by tdp"),
		);

static mr_hash_value_t
mr_tr_hash  (const mr_ptr_t x, const void * context)
{
  const mr_types_dep_t * types_dep = context;
  return ((uintptr_t)types_dep->types[x.uintptr].tdp);
}

static int
mr_tr_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const mr_types_dep_t * types_dep = context;
  mr_td_t * x_tdp = types_dep->types[x.uintptr].tdp;
  mr_td_t * y_tdp = types_dep->types[y.uintptr].tdp;
  return ((x_tdp > y_tdp) - (x_tdp < y_tdp));
}

static mr_status_t
mr_append_td  (mr_ptr_t key, void * context)
{
  mr_types_dep_t * types_dep = context;
  mr_td_t * tdp = key.ptr;

  if ((MR_STRUCT_TYPES >> tdp->mr_type) & 1)
    {
      mr_type_ref_t * type_ref = mr_rarray_allocate_element ((void**)&types_dep->types, &types_dep->size, &types_dep->alloc_size, sizeof (types_dep->types[0]));
      if (NULL == type_ref)
	return (MR_FAILURE);
      memset (type_ref, 0, sizeof (*type_ref));
      type_ref->tdp = tdp;
    }

  return (MR_SUCCESS);
}

static mr_status_t
mr_udo_init_index (mr_types_dep_t * types_dep)
{
  mr_res_t context = {
    .data = { types_dep },
    .type = "mr_types_dep_t",
    .MR_SIZE = sizeof (mr_types_dep_t),
  };

  mr_status_t status = mr_ic_new (&types_dep->index, mr_tr_hash, mr_tr_cmp, "uintptr", MR_IC_HASH, &context);
  if (status != MR_SUCCESS)
    return (status);

  int i, types_count = types_dep->size / sizeof (types_dep->types[0]);
  mr_ptr_t index[types_count];

  for (i = 0; i < types_count; ++i)
    index[i].uintptr = i;

  return (mr_ic_index (&types_dep->index, index, sizeof (index)));
}

TYPEDEF_STRUCT (mr_append_ref_t,
		(mr_types_dep_t *, types_dep),
		(int, idx),
		);

static mr_status_t
mr_append_ref  (mr_ptr_t key, void * context)
{
  mr_append_ref_t * append_ref = context;
  mr_fd_t * fdp = key.ptr;
  mr_types_dep_t * types_dep = append_ref->types_dep;
  uintptr_t type_ref_idx = types_dep->size / sizeof (types_dep->types[0]);

  types_dep->types[type_ref_idx].tdp = fdp->stype.tdp;
  mr_ptr_t * find = mr_ic_find (&types_dep->index, type_ref_idx);
  if (find != NULL)
    {
      mr_type_ref_t * tr = &types_dep->types[find->uintptr];
      typeof (tr->ref) ref = mr_rarray_allocate_element ((void**)&tr->ref, &tr->size, &tr->alloc_size, sizeof (tr->ref[0]));
      if (NULL == ref)
	return (MR_FAILURE);
      *ref = append_ref->idx;
    }

  return (MR_SUCCESS);
}

static mr_status_t
mr_udo_build_ref_graph (mr_types_dep_t * types_dep)
{
  mr_type_ref_t * type_ref = mr_rarray_allocate_element ((void**)&types_dep->types, &types_dep->size, &types_dep->alloc_size, sizeof (types_dep->types[0]));
  if (NULL == type_ref)
    return (MR_FAILURE);

  types_dep->size -= sizeof (types_dep->types[0]);

  mr_append_ref_t append_ref;
  append_ref.types_dep = types_dep;

  int i, types_count = types_dep->size / sizeof (types_dep->types[0]);
  for (i = 0; i < types_count; ++i)
    {
      append_ref.idx = i;
      mr_status_t status = mr_ic_foreach (&types_dep->types[i].tdp->param.struct_param.field_by_name, mr_append_ref, &append_ref);
      if (status != MR_SUCCESS)
	return (status);
    }
  return (MR_SUCCESS);
}

static mr_status_t
mr_udo_dfs (mr_types_dep_t * types_dep, int type_idx, bool * visited, char * field_name, char ** discriminator_type)
{
  if (visited[type_idx])
    return (MR_SUCCESS);

  visited[type_idx] = true;

  mr_fd_t * fdp = mr_get_fd_by_name (types_dep->types[type_idx].tdp, field_name);
  if (fdp && fdp->stype.tdp)
    {
      if (NULL == *discriminator_type)
	*discriminator_type = fdp->stype.tdp->type.str;
      else if (*discriminator_type != fdp->stype.tdp->type.str)
	return (MR_FAILURE);
      return (MR_SUCCESS);
    }

  mr_type_ref_t * type_ref = &types_dep->types[type_idx];
  int i, ref_count = type_ref->size / sizeof (type_ref->ref[0]);
  for (i = 0; i < ref_count; ++i)
    if (MR_SUCCESS != mr_udo_dfs (types_dep, type_ref->ref[i], visited, field_name, discriminator_type))
      return (MR_FAILURE);

  return (MR_SUCCESS);
}

static void
mr_udo_detect_types (mr_types_dep_t * types_dep, int type_idx, mr_fd_t * fdp)
{
  if ((NULL == fdp->res.ptr) || (NULL == fdp->res_type) || (NULL == fdp->stype.tdp)
       || (0 == fdp->MR_SIZE) || (NULL == fdp->meta))
    return;
  if (strcmp (fdp->res_type, "mr_ud_override_t") != 0)
    return;

  mr_ud_override_t * udo = fdp->res.ptr;
  int i, udo_count = fdp->MR_SIZE / sizeof (mr_ud_override_t);
  for (i = 0; i < udo_count; ++i)
    if (udo[i].type == NULL)
      break;

  if (i >= udo_count)
    return; /* all udo are typed */

  char * discriminator_type = NULL;
  bool visited[types_dep->size / sizeof (types_dep->types[0])];
  memset (visited, 0, sizeof (visited));
  mr_status_t status = mr_udo_dfs (types_dep, type_idx, visited, fdp->meta, &discriminator_type);

  if ((MR_SUCCESS == status) && (discriminator_type != NULL))
    for (i = 0; i < udo_count; ++i)
      if (udo[i].type == NULL)
	{
	  udo[i].type = discriminator_type;
	  udo[i].typed_value.ptr = &udo[i].value;
	}
}

static void
mr_udo_foreach (mr_types_dep_t * types_dep)
{
  int i, types_count = types_dep->size / sizeof (types_dep->types[0]);
  for (i = 0; i < types_count; ++i)
    {
      mr_struct_param_t * sp = &types_dep->types[i].tdp->param.struct_param;
      int j, fields_count = sp->fields_size / sizeof (sp->fields[0]);
      for (j = 0; j < fields_count; ++j)
	mr_udo_detect_types (types_dep, i, sp->fields[j]);
    }
}

static void
mr_udo_free (mr_types_dep_t * types_dep)
{
  mr_ic_free (&types_dep->index);

  if (types_dep->types)
    {
      int i, types_count = types_dep->size / sizeof (types_dep->types[0]);
      for (i = 0; i < types_count; ++i)
	if (types_dep->types[i].ref)
	  MR_FREE (types_dep->types[i].ref);
      MR_FREE (types_dep->types);
    }
}

void mr_udo_init ()
{
  mr_types_dep_t types_dep;
  mr_status_t status;

  memset (&types_dep, 0, sizeof (types_dep));

  status = mr_ic_foreach (&mr_conf.type_by_name, mr_append_td, &types_dep);
  if (status != MR_SUCCESS)
    goto graph_free;

  status = mr_udo_init_index (&types_dep);
  if (status != MR_SUCCESS)
    goto graph_free;

  status = mr_udo_build_ref_graph (&types_dep);
  if (status != MR_SUCCESS)
    goto graph_free;

  mr_udo_foreach (&types_dep);

 graph_free:
  mr_udo_free (&types_dep);
}
