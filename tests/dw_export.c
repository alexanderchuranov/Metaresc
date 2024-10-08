#include <string.h>

#include <metaresc.h>
#include <dw_mr_types.h>
#include <mr_ic.h>
#include <regression.h>

static mr_dwarf_t mr_dwarf[] = {
#include <dw_export.h>
  ,
#include <dw_types.h>
};

static mr_td_t *
get_td_by_name (mr_hashed_string_t * type)
{
  int j, mr_dwarf_count = sizeof (mr_dwarf) / sizeof (mr_dwarf[0]);
  for (j = 0; j < mr_dwarf_count; ++j)
    {
      int i, td_count = mr_dwarf[j].tdps_size / sizeof (mr_dwarf[j].tdps[0]);
      for (i = 0; i < td_count; ++i)
	if (mr_hashed_string_cmp (&mr_dwarf[j].tdps[i]->type, type) == 0)
	  return (mr_dwarf[j].tdps[i]);
    }
  return (NULL);
}

static void
compare_fields_meta (mr_td_t * mr_td, mr_td_t * dw_td)
{
  int i, j, named_anon_union_count = 0;
  int count = mr_td->param.struct_param.fields_size / sizeof (mr_td->param.struct_param.fields[0]);
  dw_td->param.struct_param.fields_size -= sizeof (dw_td->param.struct_param.fields[0]);

  for (i = 0; i < count; ++i)
    {
      mr_fd_t * mr_fdp = mr_td->param.struct_param.fields[i];
      
      if (mr_fdp->stype.mr_type == MR_TYPE_NAMED_ANON_UNION)
	{
	  ++named_anon_union_count;
	  continue;
	}
      else if ((mr_fdp->stype.mr_type == MR_TYPE_ANON_UNION) && (named_anon_union_count > 0))
	continue;

      mr_fd_t * dw_fdp = NULL;
      for (j = dw_td->param.struct_param.fields_size / sizeof (dw_td->param.struct_param.fields[0]) - 1; j >= 0; --j)
	if (mr_hashed_string_cmp (&dw_td->param.struct_param.fields[j]->name, &mr_fdp->name) == 0)
	  {
	    dw_fdp = dw_td->param.struct_param.fields[j];
	    break;
	  }

      ck_assert_msg (dw_fdp != NULL, "DWARF descriptor for type '%s' mismatched builtin: missed field '%s'",
		     mr_td->type.str, mr_fdp->name.str);

      mr_type_t mr_type = mr_fdp->stype.mr_type;
      if (mr_type == MR_TYPE_FUNC)
	mr_type = MR_TYPE_FUNC_TYPE;
      else if ((mr_type == MR_TYPE_POINTER) && (mr_fdp->stype.mr_type_aux == MR_TYPE_CHAR))
	mr_type = MR_TYPE_STRING;
      else if (mr_type == MR_TYPE_VOID)
	{
	  if ((dw_fdp->stype.mr_type == MR_TYPE_BITFIELD) ||
	      (dw_fdp->stype.mr_type == MR_TYPE_STRUCT) ||
	      (dw_fdp->stype.mr_type == MR_TYPE_UNION) ||
	      (dw_fdp->stype.mr_type == MR_TYPE_ANON_UNION) ||
	      (dw_fdp->stype.mr_type == MR_TYPE_ARRAY) ||
	      (dw_fdp->stype.mr_type == MR_TYPE_POINTER))
	    mr_type = dw_fdp->stype.mr_type;
	  else
	    mr_type = mr_fdp->stype.mr_type_aux;
	}

      ck_assert_msg (mr_type == dw_fdp->stype.mr_type
#if SIZEOF_DOUBLE == SIZEOF_LONG_DOUBLE
		     || ((MR_TYPE_COMPLEX_LONG_DOUBLE == mr_type) && (MR_TYPE_COMPLEX_DOUBLE == dw_fdp->stype.mr_type))
#endif
		     , "DWARF descriptor for type '%s' mismatched builtin: field '%s' mr_type %d != %d",
		     mr_td->type.str, mr_fdp->name.str, mr_type, dw_fdp->stype.mr_type);
      
      if (((mr_fdp->stype.mr_type == MR_TYPE_POINTER) || (mr_fdp->stype.mr_type == MR_TYPE_ARRAY) || (mr_fdp->stype.mr_type == MR_TYPE_BITFIELD)) &&
	  !((mr_fdp->stype.mr_type == MR_TYPE_POINTER) && (mr_fdp->stype.mr_type_aux == MR_TYPE_CHAR)) &&
	  !((mr_fdp->stype.mr_type == MR_TYPE_POINTER) && (mr_fdp->stype.mr_type_aux == MR_TYPE_VOID)))
	ck_assert_msg ((mr_fdp->stype.mr_type_aux == dw_fdp->stype.mr_type_aux)
#if SIZEOF_DOUBLE == SIZEOF_LONG_DOUBLE
		       || ((MR_TYPE_COMPLEX_LONG_DOUBLE == mr_fdp->stype.mr_type_aux) && (MR_TYPE_COMPLEX_DOUBLE == dw_fdp->stype.mr_type_aux))
#endif
		       , "DWARF descriptor for type '%s' mismatched builtin: field '%s' mr_type_aux %d != %d",
		       mr_td->type.str, mr_fdp->name.str, mr_fdp->stype.mr_type_aux, dw_fdp->stype.mr_type_aux);
      
      if (mr_fdp->stype.mr_type != MR_TYPE_VOID)
	ck_assert_msg (mr_fdp->stype.size == dw_fdp->stype.size,
		       "DWARF descriptor for type '%s' mismatched builtin: field '%s' size %d != %d",
		       mr_td->type.str, mr_fdp->name.str, (int)mr_fdp->stype.size, (int)dw_fdp->stype.size);

      if (mr_fdp->stype.mr_type != MR_TYPE_VOID)
	ck_assert_msg (mr_fdp->offset == dw_fdp->offset,
		       "DWARF descriptor for type '%s' mismatched builtin: field '%s' offset %d != %d",
		       mr_td->type.str, mr_fdp->name.str, (int)mr_fdp->offset, (int)dw_fdp->offset);
      
      if (mr_fdp->stype.mr_type == MR_TYPE_ARRAY)
	{
	  ck_assert_msg (mr_fdp->stype.dim.size == dw_fdp->stype.dim.size,
			 "DWARF descriptor for type '%s' mismatched builtin: field '%s' dim.size %d != %d",
			 mr_td->type.str, mr_fdp->name.str,
			 (int)mr_fdp->stype.dim.size, (int)dw_fdp->stype.dim.size);

	  for (j = 0; j < mr_fdp->stype.dim.size / sizeof (mr_fdp->stype.dim.dim[0]); ++j)
	    ck_assert_msg (mr_fdp->stype.dim.dim[j] == dw_fdp->stype.dim.dim[j],
			   "DWARF descriptor for type '%s' mismatched builtin: field '%s' dim[%d] %d != %d",
			   mr_td->type.str, mr_fdp->name.str, j,
			   (int)mr_fdp->stype.dim.dim[j], (int)dw_fdp->stype.dim.dim[j]);
	}
      
      if (mr_fdp->stype.mr_type == MR_TYPE_BITFIELD)
	{
	  ck_assert_msg (mr_fdp->bitfield_param.width == dw_fdp->bitfield_param.width,
			 "DWARF descriptor for type '%s' mismatched builtin: field '%s' width %d != %d",
			 mr_td->type.str, mr_fdp->name.str,
			 mr_fdp->bitfield_param.width, dw_fdp->bitfield_param.width);
	  ck_assert_msg (mr_fdp->bitfield_param.shift == dw_fdp->bitfield_param.shift,
			 "DWARF descriptor for type '%s' mismatched builtin: field '%s' shift %d != %d",
			 mr_td->type.str, mr_fdp->name.str,
			 mr_fdp->bitfield_param.shift, dw_fdp->bitfield_param.shift);
	}
      
      if ((mr_fdp->stype.mr_type == MR_TYPE_STRUCT) ||
	  (mr_fdp->stype.mr_type == MR_TYPE_UNION) ||
	  (mr_fdp->stype.mr_type == MR_TYPE_ENUM) ||
	  ((mr_fdp->stype.mr_type == MR_TYPE_BITFIELD) && (mr_fdp->stype.mr_type_aux == MR_TYPE_ENUM)))
	ck_assert_msg (strcmp (mr_fdp->stype.type, dw_fdp->stype.type) == 0,
		       "DWARF descriptor for type '%s' mismatched builtin: field '%s' type '%s' != '%s'",
		       mr_td->type.str, mr_fdp->name.str, mr_fdp->stype.type, dw_fdp->stype.type);
    }

  ck_assert_msg (dw_td->param.struct_param.fields_size == mr_td->param.struct_param.fields_size + named_anon_union_count * sizeof (mr_td->param.struct_param.fields[0]),
		 "DWARF descriptor for type '%s' mismatched builtin: fields list size %d != %d",
		 mr_td->type.str, (int)dw_td->param.struct_param.fields_size, (int)mr_td->param.struct_param.fields_size);
}

static void
compare_enum_meta (mr_td_t * mr_td, mr_td_t * dw_td)
{
  dw_td->param.enum_param.enums_size -= sizeof (dw_td->param.enum_param.enums[0]);

  ck_assert_msg (mr_td->param.enum_param.mr_type_effective == dw_td->param.enum_param.mr_type_effective,
		 "DWARF descriptor for type '%s' mismatched builtin: effective mr_type %d != %d",
		 mr_td->type.str, (int)mr_td->param.enum_param.mr_type_effective, (int)dw_td->param.enum_param.mr_type_effective);
  ck_assert_msg (mr_td->param.enum_param.size_effective == dw_td->param.enum_param.size_effective,
		 "DWARF descriptor for type '%s' mismatched builtin: effective size %d != %d",
		 mr_td->type.str, (int)mr_td->param.enum_param.size_effective, (int)dw_td->param.enum_param.size_effective);

  int i, j;
  for (i = mr_td->param.enum_param.enums_size / sizeof (mr_td->param.enum_param.enums[0]) - 1; i >= 0; --i)
    {
      mr_ed_t * mr_edp = mr_td->param.enum_param.enums[i];
      mr_ed_t * dw_edp = NULL;
      for (j = dw_td->param.enum_param.enums_size / sizeof (dw_td->param.enum_param.enums[0]) - 1; j >= 0; --j)
	if (mr_hashed_string_cmp (&dw_td->param.enum_param.enums[j]->name, &mr_edp->name) == 0)
	  {
	    dw_edp = dw_td->param.enum_param.enums[j];
	    break;
	  }

      ck_assert_msg (dw_edp != NULL, "DWARF descriptor for type '%s' mismatched builtin: missed enumeration '%s'",
		     mr_td->type.str, mr_edp->name.str);
      ck_assert_msg (mr_edp->value._signed == dw_edp->value._signed,
		     "DWARF descriptor for type '%s' mismatched builtin: enumeration value '%'" SCNi64 " != '%'" SCNi64,
		     mr_td->type.str, mr_edp->value._signed, dw_edp->value._signed);
    }

  ck_assert_msg (dw_td->param.enum_param.enums_size == mr_td->param.enum_param.enums_size,
		 "DWARF descriptor for type '%s' mismatched builtin: fields list size %d != %d",
		 mr_td->type.str, (int)dw_td->param.enum_param.enums_size, (int)mr_td->param.enum_param.enums_size);
}

static mr_status_t
check_td (mr_ptr_t key, void * context)
{
  mr_td_t * mr_td = key.ptr;
  mr_td_t * dw_td = get_td_by_name (&mr_td->type);

  if (MR_TDP_DWARF == mr_td->td_producer)
    return (MR_SUCCESS);
  /*
    Metaresc has type descriptors for most of builin types like int, long, float.
    This list also include va_args which on x86 Linux is an alias for char*.
    On aarch64 platform va_args is not a char*, but a struct, that's why builtin type for it is MR_TYPE_NONE.
    We need to skip types that were not detected by DWARF (most of builtin types) and types that were not properly detected Metaresc.
  */
  if ((dw_td != NULL) && (mr_td->mr_type != MR_TYPE_NONE))
    {
      ck_assert_msg (dw_td->mr_type == mr_td->mr_type, "DWARF descriptor for type '%s' mismatched builtin: mr_type %d != %d",
		     mr_td->type.str, dw_td->mr_type, mr_td->mr_type);
      ck_assert_msg (dw_td->size == mr_td->size, "DWARF descriptor for type '%s' mismatched builtin: type size %d != %d",
		     mr_td->type.str, (int)dw_td->size, (int)mr_td->size);

      switch (mr_td->mr_type)
	{
	case MR_TYPE_STRUCT:
	case MR_TYPE_UNION:
	  compare_fields_meta (mr_td, dw_td);
	  break;
	case MR_TYPE_ENUM:
	  compare_enum_meta (mr_td, dw_td);
	  break;
	default:
	  break;
	}
    }    
  return (MR_SUCCESS);
}

START_TEST (dw_check_all)
{
  int i, count = sizeof (mr_dwarf) / sizeof (mr_dwarf[0]);
  for (i = 0; i < count; ++i)
    mr_add_dwarf (&mr_dwarf[i]);

  mr_type_void_fields ("mr_fd_t", "mr_type");
  mr_ic_foreach (&mr_conf.type_by_name, check_td, NULL);
} END_TEST

MAIN_TEST_SUITE ((dw_check_all, "Check DWARF export"));
