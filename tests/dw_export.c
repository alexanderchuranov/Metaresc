#include <string.h>

#include <metaresc.h>
#include <dw_mr_types.h>
#include <mr_ic.h>
#include <regression.h>

static mr_td_t ra_td[] = {
#include <dw_export.h>
#include <dw_types.h>
};

static mr_td_t *
get_td_by_name (mr_hashed_string_t * type)
{
  int i;
  for (i = sizeof (ra_td) / sizeof (ra_td[0]) - 1; i >= 0; --i)
    if (mr_hashed_string_cmp (&ra_td[i].type, type) == 0)
      return (&ra_td[i]);
  return (NULL);
}

static void
compare_fields_meta (mr_td_t * mr_td, mr_td_t * dw_td)
{
  int i, j, named_anon_union_count = 0;
  dw_td->param.struct_param.fields_size -= sizeof (dw_td->param.struct_param.fields[0]);

  for (i = mr_td->param.struct_param.fields_size / sizeof (mr_td->param.struct_param.fields[0]) - 1; i >= 0; --i)
    {
      mr_fd_t * mr_fdp = mr_td->param.struct_param.fields[i];
      
      if (mr_fdp->mr_type == MR_TYPE_NAMED_ANON_UNION)
	++named_anon_union_count;
      
      if ((mr_fdp->mr_type == MR_TYPE_ANON_UNION) || (mr_fdp->mr_type == MR_TYPE_NAMED_ANON_UNION))
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

      mr_type_t mr_type = mr_fdp->mr_type;
      if (mr_type == MR_TYPE_FUNC)
	mr_type = MR_TYPE_FUNC_TYPE;
      else if ((mr_type == MR_TYPE_POINTER) && (mr_fdp->mr_type_aux == MR_TYPE_CHAR))
	mr_type = MR_TYPE_STRING;
      else if (mr_fdp->mr_type == MR_TYPE_VOID)
	{
	  if ((dw_fdp->mr_type == MR_TYPE_BITFIELD) ||
	      (dw_fdp->mr_type == MR_TYPE_STRUCT) ||
	      (dw_fdp->mr_type == MR_TYPE_UNION) ||
	      (dw_fdp->mr_type == MR_TYPE_ARRAY) ||
	      (dw_fdp->mr_type == MR_TYPE_POINTER))
	    mr_type = dw_fdp->mr_type;
	  else
	    mr_type = mr_fdp->mr_type_aux;
	}

      ck_assert_msg (mr_type == dw_fdp->mr_type,
		     "DWARF descriptor for type '%s' mismatched builtin: field '%s' mr_type %d != %d",
		     mr_td->type.str, mr_fdp->name.str, mr_type, dw_fdp->mr_type);
      
      if (((mr_fdp->mr_type == MR_TYPE_POINTER) || (mr_fdp->mr_type == MR_TYPE_ARRAY) || (mr_fdp->mr_type == MR_TYPE_BITFIELD)) &&
	  !((mr_fdp->mr_type == MR_TYPE_POINTER) && (mr_fdp->mr_type_aux == MR_TYPE_CHAR)))
	ck_assert_msg (mr_fdp->mr_type_aux == dw_fdp->mr_type_aux,
		       "DWARF descriptor for type '%s' mismatched builtin: field '%s' mr_type_aux %d != %d",
		       mr_td->type.str, mr_fdp->name.str, mr_fdp->mr_type_aux, dw_fdp->mr_type_aux);
      
      if (mr_fdp->mr_type != MR_TYPE_VOID)
	ck_assert_msg (mr_fdp->size == dw_fdp->size,
		       "DWARF descriptor for type '%s' mismatched builtin: field '%s' size %d != %d",
		       mr_td->type.str, mr_fdp->name.str, (int)mr_fdp->size, (int)dw_fdp->size);

      if (mr_fdp->mr_type != MR_TYPE_VOID)
	ck_assert_msg (mr_fdp->offset == dw_fdp->offset,
		       "DWARF descriptor for type '%s' mismatched builtin: field '%s' offset %d != %d",
		       mr_td->type.str, mr_fdp->name.str, (int)mr_fdp->offset, (int)dw_fdp->offset);
      
      if (mr_fdp->mr_type == MR_TYPE_ARRAY)
	for (j = 0; j < sizeof (mr_fdp->param.array_param.dim.dim) / sizeof (mr_fdp->param.array_param.dim.dim[0]); ++j)
	  {
	    ck_assert_msg (mr_fdp->param.array_param.dim.dim[j].count == dw_fdp->param.array_param.dim.dim[j].count,
			   "DWARF descriptor for type '%s' mismatched builtin: field '%s' dim[%d].count %d != %d",
			   mr_td->type.str, mr_fdp->name.str, j,
			   (int)mr_fdp->param.array_param.dim.dim[j].count, (int)dw_fdp->param.array_param.dim.dim[j].count);
	    ck_assert_msg (mr_fdp->param.array_param.dim.dim[j].is_last == dw_fdp->param.array_param.dim.dim[j].is_last,
			   "DWARF descriptor for type '%s' mismatched builtin: field '%s' dim[%d].is_last",
			   mr_td->type.str, mr_fdp->name.str,j);
	    if (mr_fdp->param.array_param.dim.dim[j].is_last)
	      break;
	  }
      
      if (mr_fdp->mr_type == MR_TYPE_BITFIELD)
	{
	  ck_assert_msg (mr_fdp->param.bitfield_param.width == dw_fdp->param.bitfield_param.width,
			 "DWARF descriptor for type '%s' mismatched builtin: field '%s' width %d != %d",
			 mr_td->type.str, mr_fdp->name.str,
			 mr_fdp->param.bitfield_param.width, dw_fdp->param.bitfield_param.width);
	  ck_assert_msg (mr_fdp->param.bitfield_param.shift == dw_fdp->param.bitfield_param.shift,
			 "DWARF descriptor for type '%s' mismatched builtin: field '%s' shift %d != %d",
			 mr_td->type.str, mr_fdp->name.str,
			 mr_fdp->param.bitfield_param.shift, dw_fdp->param.bitfield_param.shift);
	}
      
      if ((mr_fdp->mr_type == MR_TYPE_STRUCT) ||
	  (mr_fdp->mr_type == MR_TYPE_UNION) ||
	  (mr_fdp->mr_type == MR_TYPE_ENUM) ||
	  ((mr_fdp->mr_type == MR_TYPE_BITFIELD) && (mr_fdp->mr_type_aux == MR_TYPE_ENUM)))
	ck_assert_msg (strcmp (mr_fdp->type, dw_fdp->type) == 0,
		       "DWARF descriptor for type '%s' mismatched builtin: field '%s' type '%s' != '%s'",
		       mr_td->type.str, mr_fdp->name.str, mr_fdp->type, dw_fdp->type);
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
check_td (mr_ptr_t key, const void * context)
{
  mr_td_t * mr_td = key.ptr;
  mr_td_t * dw_td = get_td_by_name (&mr_td->type);

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
  mr_type_void_fields ("ieee_half_t", "is_nan");
  mr_type_void_fields ("ieee_float_t", "is_nan");
  mr_type_void_fields ("ieee_double_t", "is_nan");
  mr_type_void_fields ("ieee_long_double_t", "is_nan");
  mr_ic_foreach (&mr_conf.type_by_name, check_td, NULL);
} END_TEST

MAIN_TEST_SUITE ((dw_check_all, "Check DWARF export"));
