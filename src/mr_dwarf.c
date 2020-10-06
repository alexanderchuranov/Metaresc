#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> /* EXIT_SUCCESS/EXIT_FAILURE */
#include <fcntl.h> /* open () */
#include <unistd.h> /* close () */

#include <metaresc.h>
#include <mr_config.h>
#include <mr_ic.h>

#ifdef HAVE_LIBDWARF_DWARF_H
#include <libdwarf/dwarf.h>
#else /* ! HAVE_LIBDWARF_DWARF_H */
# ifdef HAVE_DWARF_H
# include <dwarf.h>
# endif /* HAVE_DWARF_H */
#endif /* HAVE_LIBDWARF_DWARF_H */

#ifdef HAVE_LIBDWARF_LIBDWARF_H
#include <libdwarf/libdwarf.h>
#else /* ! HAVE_LIBDWARF_LIBDWARF_H */
# ifdef HAVE_LIBDWARF_H
# include <libdwarf.h>
# endif /* HAVE_LIBDWARF_H */
#endif /* HAVE_LIBDWARF_LIBDWARF_H */

/* awk '/#define DW_TAG_/{print "(_"$2", = "$3"),"}' < /usr/include/libdwarf/dwarf.h */
TYPEDEF_ENUM (mr_dw_tag_t, ATTRIBUTES ( , "awk '/#define DW_TAG_/{print \"(_\"$2\", = \"$3\"),\"}' < /usr/include/libdwarf/dwarf.h"),
	      (_DW_TAG_undefined, = 0),
	      (_DW_TAG_array_type, = 0x01),
	      (_DW_TAG_class_type, = 0x02),
	      (_DW_TAG_entry_point, = 0x03),
	      (_DW_TAG_enumeration_type, = 0x04),
	      (_DW_TAG_formal_parameter, = 0x05),
	      (_DW_TAG_imported_declaration, = 0x08),
	      (_DW_TAG_label, = 0x0a),
	      (_DW_TAG_lexical_block, = 0x0b),
	      (_DW_TAG_member, = 0x0d),
	      (_DW_TAG_pointer_type, = 0x0f),
	      (_DW_TAG_reference_type, = 0x10),
	      (_DW_TAG_compile_unit, = 0x11),
	      (_DW_TAG_string_type, = 0x12),
	      (_DW_TAG_structure_type, = 0x13),
	      (_DW_TAG_subroutine_type, = 0x15),
	      (_DW_TAG_typedef, = 0x16),
	      (_DW_TAG_union_type, = 0x17),
	      (_DW_TAG_unspecified_parameters, = 0x18),
	      (_DW_TAG_variant, = 0x19),
	      (_DW_TAG_common_block, = 0x1a),
	      (_DW_TAG_common_inclusion, = 0x1b),
	      (_DW_TAG_inheritance, = 0x1c),
	      (_DW_TAG_inlined_subroutine, = 0x1d),
	      (_DW_TAG_module, = 0x1e),
	      (_DW_TAG_ptr_to_member_type, = 0x1f),
	      (_DW_TAG_set_type, = 0x20),
	      (_DW_TAG_subrange_type, = 0x21),
	      (_DW_TAG_with_stmt, = 0x22),
	      (_DW_TAG_access_declaration, = 0x23),
	      (_DW_TAG_base_type, = 0x24),
	      (_DW_TAG_catch_block, = 0x25),
	      (_DW_TAG_const_type, = 0x26),
	      (_DW_TAG_constant, = 0x27),
	      (_DW_TAG_enumerator, = 0x28),
	      (_DW_TAG_file_type, = 0x29),
	      (_DW_TAG_friend, = 0x2a),
	      (_DW_TAG_namelist, = 0x2b),
	      (_DW_TAG_namelist_item, = 0x2c),
	      (_DW_TAG_namelist_items, = 0x2c),
	      (_DW_TAG_packed_type, = 0x2d),
	      (_DW_TAG_subprogram, = 0x2e),
	      (_DW_TAG_template_type_parameter, = 0x2f),
	      (_DW_TAG_template_type_param, = 0x2f),
	      (_DW_TAG_template_value_parameter, = 0x30),
	      (_DW_TAG_template_value_param, = 0x30),
	      (_DW_TAG_thrown_type, = 0x31),
	      (_DW_TAG_try_block, = 0x32),
	      (_DW_TAG_variant_part, = 0x33),
	      (_DW_TAG_variable, = 0x34),
	      (_DW_TAG_volatile_type, = 0x35),
	      (_DW_TAG_dwarf_procedure, = 0x36),
	      (_DW_TAG_restrict_type, = 0x37),
	      (_DW_TAG_interface_type, = 0x38),
	      (_DW_TAG_namespace, = 0x39),
	      (_DW_TAG_imported_module, = 0x3a),
	      (_DW_TAG_unspecified_type, = 0x3b),
	      (_DW_TAG_partial_unit, = 0x3c),
	      (_DW_TAG_imported_unit, = 0x3d),
	      (_DW_TAG_mutable_type, = 0x3e),
	      (_DW_TAG_condition, = 0x3f),
	      (_DW_TAG_shared_type, = 0x40),
	      (_DW_TAG_type_unit, = 0x41),
	      (_DW_TAG_rvalue_reference_type, = 0x42),
	      (_DW_TAG_template_alias, = 0x43),
	      (_DW_TAG_coarray_type, = 0x44),
	      (_DW_TAG_generic_subrange, = 0x45),
	      (_DW_TAG_dynamic_type, = 0x46),
	      (_DW_TAG_atomic_type, = 0x47),
	      (_DW_TAG_call_site, = 0x48),
	      (_DW_TAG_call_site_parameter, = 0x49),
	      (_DW_TAG_skeleton_unit, = 0x4a),
	      (_DW_TAG_immutable_type, = 0x4b),
	      (_DW_TAG_lo_user, = 0x4080),
	      (_DW_TAG_MIPS_loop, = 0x4081),
	      (_DW_TAG_HP_array_descriptor, = 0x4090),
	      (_DW_TAG_format_label, = 0x4101),
	      (_DW_TAG_function_template, = 0x4102),
	      (_DW_TAG_class_template, = 0x4103),
	      (_DW_TAG_GNU_BINCL, = 0x4104),
	      (_DW_TAG_GNU_EINCL, = 0x4105),
	      (_DW_TAG_GNU_template_template_parameter, = 0x4106),
	      (_DW_TAG_GNU_template_template_param, = 0x4106),
	      (_DW_TAG_GNU_template_parameter_pack, = 0x4107),
	      (_DW_TAG_GNU_formal_parameter_pack, = 0x4108),
	      (_DW_TAG_GNU_call_site, = 0x4109),
	      (_DW_TAG_GNU_call_site_parameter, = 0x410a),
	      (_DW_TAG_ALTIUM_circ_type, = 0x5101),
	      (_DW_TAG_ALTIUM_mwa_circ_type, = 0x5102),
	      (_DW_TAG_ALTIUM_rev_carry_type, = 0x5103),
	      (_DW_TAG_ALTIUM_rom, = 0x5111),
	      (_DW_TAG_upc_shared_type, = 0x8765),
	      (_DW_TAG_upc_strict_type, = 0x8766),
	      (_DW_TAG_upc_relaxed_type, = 0x8767),
	      (_DW_TAG_PGI_kanji_type, = 0xa000),
	      (_DW_TAG_PGI_interface_block, = 0xa020),
	      (_DW_TAG_SUN_function_template, = 0x4201),
	      (_DW_TAG_SUN_class_template, = 0x4202),
	      (_DW_TAG_SUN_struct_template, = 0x4203),
	      (_DW_TAG_SUN_union_template, = 0x4204),
	      (_DW_TAG_SUN_indirect_inheritance, = 0x4205),
	      (_DW_TAG_SUN_codeflags, = 0x4206),
	      (_DW_TAG_SUN_memop_info, = 0x4207),
	      (_DW_TAG_SUN_omp_child_func, = 0x4208),
	      (_DW_TAG_SUN_rtti_descriptor, = 0x4209),
	      (_DW_TAG_SUN_dtor_info, = 0x420a),
	      (_DW_TAG_SUN_dtor, = 0x420b),
	      (_DW_TAG_SUN_f90_interface, = 0x420c),
	      (_DW_TAG_SUN_fortran_vax_structure, = 0x420d),
	      (_DW_TAG_SUN_hi, = 0x42ff),
	      (_DW_TAG_hi_user, = 0xffff),
	      )

TYPEDEF_ENUM (mr_dw_attribute_code_t,
	      (_DW_AT_sibling, = 0x01),
	      (_DW_AT_location, = 0x02),
	      (_DW_AT_name, = 0x03),
	      (_DW_AT_ordering, = 0x09),
	      (_DW_AT_subscr_data, = 0x0a),
	      (_DW_AT_byte_size, = 0x0b),
	      (_DW_AT_bit_offset, = 0x0c),
	      (_DW_AT_bit_size, = 0x0d),
	      (_DW_AT_element_list, = 0x0f),
	      (_DW_AT_stmt_list, = 0x10),
	      (_DW_AT_low_pc, = 0x11),
	      (_DW_AT_high_pc, = 0x12),
	      (_DW_AT_language, = 0x13),
	      (_DW_AT_member, = 0x14),
	      (_DW_AT_discr, = 0x15),
	      (_DW_AT_discr_value, = 0x16),
	      (_DW_AT_visibility, = 0x17),
	      (_DW_AT_import, = 0x18),
	      (_DW_AT_string_length, = 0x19),
	      (_DW_AT_common_reference, = 0x1a),
	      (_DW_AT_comp_dir, = 0x1b),
	      (_DW_AT_const_value, = 0x1c),
	      (_DW_AT_containing_type, = 0x1d),
	      (_DW_AT_default_value, = 0x1e),
	      (_DW_AT_inline, = 0x20),
	      (_DW_AT_is_optional, = 0x21),
	      (_DW_AT_lower_bound, = 0x22),
	      (_DW_AT_producer, = 0x25),
	      (_DW_AT_prototyped, = 0x27),
	      (_DW_AT_return_addr, = 0x2a),
	      (_DW_AT_start_scope, = 0x2c),
	      (_DW_AT_bit_stride, = 0x2e),
	      (_DW_AT_stride_size, = 0x2e),
	      (_DW_AT_upper_bound, = 0x2f),
	      (_DW_AT_abstract_origin, = 0x31),
	      (_DW_AT_accessibility, = 0x32),
	      (_DW_AT_address_class, = 0x33),
	      (_DW_AT_artificial, = 0x34),
	      (_DW_AT_base_types, = 0x35),
	      (_DW_AT_calling_convention, = 0x36),
	      (_DW_AT_count, = 0x37),
	      (_DW_AT_data_member_location, = 0x38),
	      (_DW_AT_decl_column, = 0x39),
	      (_DW_AT_decl_file, = 0x3a),
	      (_DW_AT_decl_line, = 0x3b),
	      (_DW_AT_declaration, = 0x3c),
	      (_DW_AT_discr_list, = 0x3d),
	      (_DW_AT_encoding, = 0x3e),
	      (_DW_AT_external, = 0x3f),
	      (_DW_AT_frame_base, = 0x40),
	      (_DW_AT_friend, = 0x41),
	      (_DW_AT_identifier_case, = 0x42),
	      (_DW_AT_macro_info, = 0x43),
	      (_DW_AT_namelist_item, = 0x44),
	      (_DW_AT_priority, = 0x45),
	      (_DW_AT_segment, = 0x46),
	      (_DW_AT_specification, = 0x47),
	      (_DW_AT_static_link, = 0x48),
	      (_DW_AT_type, = 0x49),
	      (_DW_AT_use_location, = 0x4a),
	      (_DW_AT_variable_parameter, = 0x4b),
	      (_DW_AT_virtuality, = 0x4c),
	      (_DW_AT_vtable_elem_location, = 0x4d),
	      (_DW_AT_allocated, = 0x4e),
	      (_DW_AT_associated, = 0x4f),
	      (_DW_AT_data_location, = 0x50),
	      (_DW_AT_byte_stride, = 0x51),
	      (_DW_AT_stride, = 0x51),
	      (_DW_AT_entry_pc, = 0x52),
	      (_DW_AT_use_UTF8, = 0x53),
	      (_DW_AT_extension, = 0x54),
	      (_DW_AT_ranges, = 0x55),
	      (_DW_AT_trampoline, = 0x56),
	      (_DW_AT_call_column, = 0x57),
	      (_DW_AT_call_file, = 0x58),
	      (_DW_AT_call_line, = 0x59),
	      (_DW_AT_description, = 0x5a),
	      (_DW_AT_binary_scale, = 0x5b),
	      (_DW_AT_decimal_scale, = 0x5c),
	      (_DW_AT_small, = 0x5d),
	      (_DW_AT_decimal_sign, = 0x5e),
	      (_DW_AT_digit_count, = 0x5f),
	      (_DW_AT_picture_string, = 0x60),
	      (_DW_AT_mutable, = 0x61),
	      (_DW_AT_threads_scaled, = 0x62),
	      (_DW_AT_explicit, = 0x63),
	      (_DW_AT_object_pointer, = 0x64),
	      (_DW_AT_endianity, = 0x65),
	      (_DW_AT_elemental, = 0x66),
	      (_DW_AT_pure, = 0x67),
	      (_DW_AT_recursive, = 0x68),
	      (_DW_AT_signature, = 0x69),
	      (_DW_AT_main_subprogram, = 0x6a),
	      (_DW_AT_data_bit_offset, = 0x6b),
	      (_DW_AT_const_expr, = 0x6c),
	      (_DW_AT_enum_class, = 0x6d),
	      (_DW_AT_linkage_name, = 0x6e),
	      (_DW_AT_string_length_bit_size, = 0x6f),
	      (_DW_AT_string_length_byte_size, = 0x70),
	      (_DW_AT_rank, = 0x71),
	      (_DW_AT_str_offsets_base, = 0x72),
	      (_DW_AT_addr_base, = 0x73),
	      (_DW_AT_rnglists_base, = 0x74),
	      (_DW_AT_dwo_id, = 0x75),
	      (_DW_AT_dwo_name, = 0x76),
	      (_DW_AT_reference, = 0x77),
	      (_DW_AT_rvalue_reference, = 0x78),
	      (_DW_AT_macros, = 0x79),
	      (_DW_AT_call_all_calls, = 0x7a),
	      (_DW_AT_call_all_source_calls, = 0x7b),
	      (_DW_AT_call_all_tail_calls, = 0x7c),
	      (_DW_AT_call_return_pc, = 0x7d),
	      (_DW_AT_call_value, = 0x7e),
	      (_DW_AT_call_origin, = 0x7f),
	      (_DW_AT_call_parameter, = 0x80),
	      (_DW_AT_call_pc, = 0x81),
	      (_DW_AT_call_tail_call, = 0x82),
	      (_DW_AT_call_target, = 0x83),
	      (_DW_AT_call_target_clobbered, = 0x84),
	      (_DW_AT_call_data_location, = 0x85),
	      (_DW_AT_call_data_value, = 0x86),
	      (_DW_AT_noreturn, = 0x87),
	      (_DW_AT_alignment, = 0x88),
	      (_DW_AT_export_symbols, = 0x89),
	      (_DW_AT_deleted, = 0x8a),
	      (_DW_AT_defaulted, = 0x8b),
	      (_DW_AT_loclists_base, = 0x8c),
	      (_DW_AT_APPLE_optimized, = 0x3fe1),
	      )

TYPEDEF_ENUM (mr_dw_form_t,
	      (_DW_FORM_addr, = 0x01, "dw_addr"),
	      (_DW_FORM_block2, = 0x03),
	      (_DW_FORM_block4, = 0x04),
	      (_DW_FORM_data2, = 0x05, "dw_unsigned"),
	      (_DW_FORM_data4, = 0x06, "dw_unsigned"),
	      (_DW_FORM_data8, = 0x07, "dw_unsigned"),
	      (_DW_FORM_string, = 0x08, "dw_str"),
	      (_DW_FORM_block, = 0x09),
	      (_DW_FORM_block1, = 0x0a, "dw_unsigned"),
	      (_DW_FORM_data1, = 0x0b, "dw_unsigned"),
	      (_DW_FORM_flag, = 0x0c, "dw_flag"),
	      (_DW_FORM_sdata, = 0x0d, "dw_signed"),
	      (_DW_FORM_strp, = 0x0e, "dw_str"),
	      (_DW_FORM_udata, = 0x0f, "dw_unsigned"),
	      (_DW_FORM_ref_addr, = 0x10, "dw_off"),
	      (_DW_FORM_ref1, = 0x11, "dw_off"),
	      (_DW_FORM_ref2, = 0x12, "dw_off"),
	      (_DW_FORM_ref4, = 0x13, "dw_off"),
	      (_DW_FORM_ref8, = 0x14, "dw_off"),
	      (_DW_FORM_ref_udata, = 0x15, "dw_off"),
	      (_DW_FORM_indirect, = 0x16),
	      (_DW_FORM_sec_offset, = 0x17, "dw_off"),
	      (_DW_FORM_exprloc, = 0x18),
	      (_DW_FORM_flag_present, = 0x19, "dw_flag"),
	      (_DW_FORM_strx, = 0x1a),
	      (_DW_FORM_addrx, = 0x1b),
	      (_DW_FORM_ref_sup4, = 0x1c),
	      (_DW_FORM_strp_sup, = 0x1d),
	      (_DW_FORM_data16, = 0x1e),
	      (_DW_FORM_line_strp, = 0x1f),
	      (_DW_FORM_ref_sig8, = 0x20, "dw_off"),
	      (_DW_FORM_implicit_const, = 0x21),
	      (_DW_FORM_loclistx, = 0x22),
	      (_DW_FORM_rnglistx, = 0x23),
	      (_DW_FORM_ref_sup8, = 0x24),
	      (_DW_FORM_strx1, = 0x25),
	      (_DW_FORM_strx2, = 0x26),
	      (_DW_FORM_strx3, = 0x27),
	      (_DW_FORM_strx4, = 0x28),
	      (_DW_FORM_addrx1, = 0x29),
	      (_DW_FORM_addrx2, = 0x2a),
	      (_DW_FORM_addrx3, = 0x2b),
	      (_DW_FORM_addrx4, = 0x2c),
	      (_DW_FORM_GNU_addr_index, = 0x1f01),
	      (_DW_FORM_GNU_str_index, = 0x1f02),
	      (_DW_FORM_GNU_ref_alt, = 0x1f20),
	      (_DW_FORM_GNU_strp_alt, = 0x1f21),
	      )	      

TYPEDEF_STRUCT (mr_dw_attribute_t,
		(mr_dw_attribute_code_t, code),
		(mr_dw_form_t, form),
		ANON_UNION (),
		VOID (int, dw_default),
		(Dwarf_Bool, dw_flag),
		(Dwarf_Signed, dw_signed),
		(Dwarf_Unsigned, dw_unsigned),
		(Dwarf_Addr, dw_addr),
		(Dwarf_Off, dw_off),
		(char *, dw_str),
		END_ANON_UNION ("form"),
		)

TYPEDEF_STRUCT (mr_die_t,
		(Dwarf_Off, off),
		(mr_dw_tag_t, tag),
		(mr_dw_attribute_t *, attributes, , "", { "attributes_size" }, "string"),
		VOID (ssize_t, attributes_size),
		(mr_die_t *, children, , "", { "children_size" }, "string"),
		VOID (ssize_t, children_size),
		VOID (ssize_t, children_alloc_size),
		)

TYPEDEF_STRUCT (mr_type_sign_t,
		(mr_hashed_string_t, type),
		(mr_size_t, size),
		(mr_type_t, mr_type),
		)

static mr_ic_t mr_type_sign_ic;

static void
dump_attribute (Dwarf_Debug debug, Dwarf_Attribute dw_attribute, mr_dw_attribute_t * mr_attr)
{
  Dwarf_Half code, form;
  int rv = dwarf_whatattr (dw_attribute, &code, NULL);
  assert (rv == DW_DLV_OK);
  mr_attr->code = code;
    
  rv = dwarf_whatform (dw_attribute, &form, NULL);
  assert (rv == DW_DLV_OK);
  mr_attr->form = form;

  switch (form)
    {
    case _DW_FORM_string:
    case _DW_FORM_strp:
      rv = dwarf_formstring (dw_attribute, &mr_attr->dw_str, NULL);
      assert (rv == DW_DLV_OK);
      if (mr_attr->dw_str)
	mr_attr->dw_str = mr_strdup (mr_attr->dw_str);
      break;
	
    case _DW_FORM_flag:
    case _DW_FORM_flag_present:
      rv = dwarf_formflag (dw_attribute, &mr_attr->dw_flag, NULL);
      assert (rv == DW_DLV_OK);
      break;
	
    case _DW_FORM_addr:
      rv = dwarf_formaddr (dw_attribute, &mr_attr->dw_addr, NULL);
      assert (rv == DW_DLV_OK);
      break;
	
    case _DW_FORM_sdata:
      rv = dwarf_formsdata (dw_attribute, &mr_attr->dw_signed, NULL);
      assert (rv == DW_DLV_OK);
      break;
	
    case _DW_FORM_ref_addr:
    case _DW_FORM_ref_udata:
    case _DW_FORM_ref1:
    case _DW_FORM_ref2:
    case _DW_FORM_ref4:
    case _DW_FORM_ref8:
    case _DW_FORM_ref_sig8:
    case _DW_FORM_sec_offset:
      rv = dwarf_global_formref (dw_attribute, &mr_attr->dw_off, NULL);
      assert (rv == DW_DLV_OK);
      break;
	
    case _DW_FORM_data1:
    case _DW_FORM_data2:
    case _DW_FORM_data4:
    case _DW_FORM_data8:
    case _DW_FORM_udata:
      rv = dwarf_formudata (dw_attribute, &mr_attr->dw_unsigned, NULL);
      assert (rv == DW_DLV_OK);
      break;

    case _DW_FORM_block1:
      {
	Dwarf_Block * block;
	rv = dwarf_formblock (dw_attribute, &block, NULL);
	assert (rv == DW_DLV_OK);

	if (DW_OP_plus_uconst == *(uint8_t*)block->bl_data)
	  {
	    uint8_t * leb128 = block->bl_data + sizeof (uint8_t);
	    Dwarf_Unsigned value = 0;
	    int i, shift = 0;
	    for (i = 1; i < block->bl_len; ++i)
	      {
		value |= (*leb128++ & ((1 << 7) - 1)) << shift;
		shift += 7;
	      }
	    mr_attr->dw_unsigned = value;
	  }
      }
      break;
    }
}

static void
dump_die_tree (Dwarf_Debug debug, Dwarf_Die die, mr_die_t * mr_parent_die)
{
  mr_die_t * mr_die = mr_rarray_allocate_element
    ((void*)&mr_parent_die->children, &mr_parent_die->children_size,
     &mr_parent_die->children_alloc_size, sizeof (mr_parent_die->children[0]));
  assert (mr_die != NULL);

  memset (mr_die, 0, sizeof (*mr_die));
      
  Dwarf_Half tagval;
  int rv = dwarf_tag (die, &tagval, NULL);
  assert (rv == DW_DLV_OK);
  mr_die->tag = tagval;
  
  rv = dwarf_dieoffset (die, &mr_die->off, NULL);
  assert (rv == DW_DLV_OK);
  
  Dwarf_Signed dw_count, i;
  Dwarf_Attribute * dw_attributes;

  /* Retrieve the attribute list from the DIE. */
  rv = dwarf_attrlist (die, &dw_attributes, &dw_count, NULL);
  assert (rv != DW_DLV_ERROR);
  if (rv == DW_DLV_OK)
    {
      mr_die->attributes = MR_CALLOC (dw_count, sizeof (mr_die->attributes[0]));
      assert (mr_die->attributes != NULL);
      mr_die->attributes_size = dw_count * sizeof (mr_die->attributes[0]);
  
      /* Process the attribute list. */
      for (i = 0; i < dw_count; ++i)
	dump_attribute (debug, dw_attributes[i], &mr_die->attributes[i]);
      dwarf_dealloc (debug, dw_attributes, DW_DLA_LIST);
    }
  
  Dwarf_Die child_die;
  rv = dwarf_child (die, &child_die, NULL);
  assert (rv != DW_DLV_ERROR);
  
  if (rv == DW_DLV_OK)
    for (;;)
      {
	Dwarf_Die sibling_die;
	dump_die_tree (debug, child_die, mr_die);
	rv = dwarf_siblingof (debug, child_die, &sibling_die, NULL);
	assert (rv != DW_DLV_ERROR);
	if (rv == DW_DLV_NO_ENTRY)
	  break;
	child_die = sibling_die;
      }
}

static void
dump_cu_list (Dwarf_Debug debug, mr_die_t * mr_parent_die)
{
  int             rv;
  Dwarf_Die       die;
  Dwarf_Half      tagval;
  Dwarf_Unsigned  next_cu_header;

  for (;;)
    {
      rv = dwarf_next_cu_header (debug, NULL, NULL, NULL, NULL, &next_cu_header, NULL);
      if (rv == DW_DLV_NO_ENTRY)
	break;
      assert (rv == DW_DLV_OK);

      rv = dwarf_siblingof (debug, 0, &die, NULL);
      assert (rv != DW_DLV_ERROR);
      rv = dwarf_tag (die, &tagval, NULL);
      assert (rv == DW_DLV_OK);
      assert (tagval == DW_TAG_compile_unit);

      dump_die_tree (debug, die, mr_parent_die);
    }
}

static void
walk_dies (mr_die_t * mr_die, mr_ic_rarray_t * ic_rarray)
{
  mr_ptr_t * die_ptr;
  
  switch (mr_die->tag)
    {
    case _DW_TAG_base_type:
    case _DW_TAG_typedef:
    case _DW_TAG_structure_type:
    case _DW_TAG_union_type:
    case _DW_TAG_enumeration_type:
    case _DW_TAG_array_type:
    case _DW_TAG_const_type:
    case _DW_TAG_volatile_type:
    case _DW_TAG_pointer_type:
    case _DW_TAG_subroutine_type:
      die_ptr = mr_rarray_allocate_element ((void*)&ic_rarray->ra, &ic_rarray->size, &ic_rarray->alloc_size, sizeof (ic_rarray->ra[0]));
      assert (die_ptr != NULL);
      die_ptr->ptr = mr_die;
      break;
    default:
      break;
    }
  
  int i, count = mr_die->children_size / sizeof (mr_die->children[0]);
  for (i = 0; i < count; ++i)
    walk_dies (&mr_die->children[i], ic_rarray);
}

static inline mr_dw_attribute_t *
die_attribute (mr_die_t * mr_die, mr_dw_attribute_code_t code)
{
  int i;
  for (i = mr_die->attributes_size / sizeof (mr_die->attributes[0]) - 1; i >= 0; --i)
    if (mr_die->attributes[i].code == code)
      return (&mr_die->attributes[i]);
  return (NULL);
}

static void
get_type_name (mr_fd_t * fdp, mr_die_t * mr_die, mr_ic_t * die_off_ic)
{
  if (fdp->type == NULL)
    {
      mr_dw_attribute_t * attr = die_attribute (mr_die, _DW_AT_name);
      if (attr == NULL)
	{
#define ANONYMOUS_TYPE_TEMPLATE "mr_type_anonymous_%" SCNu64 "_t"
	  char type_name[sizeof (ANONYMOUS_TYPE_TEMPLATE) + sizeof (mr_die->off) * 3];
	  sprintf (type_name, ANONYMOUS_TYPE_TEMPLATE, (uint64_t)mr_die->off);
	  mr_die->attributes = MR_REALLOC (mr_die->attributes, mr_die->attributes_size + sizeof (mr_die->attributes[0]));
	  assert (mr_die->attributes != NULL);
	  int idx = mr_die->attributes_size / sizeof (mr_die->attributes[0]);
	  mr_die->attributes_size += sizeof (mr_die->attributes[0]);
	  mr_die->attributes[idx].code = _DW_AT_name;
	  mr_die->attributes[idx].form = _DW_FORM_strp;
	  mr_die->attributes[idx].dw_str = mr_strdup (type_name);
	  attr = &mr_die->attributes[idx];
	}
      assert (_DW_FORM_strp == attr->form);
      assert (attr->dw_str != NULL);
      fdp->type = mr_strdup (attr->dw_str);
    }
}

static void
get_base_mr_type (mr_fd_t * fdp, mr_die_t * mr_die)
{
  mr_type_sign_t mr_type_sign;
  memset (&mr_type_sign, 0, sizeof (mr_type_sign));

  mr_dw_attribute_t * attr = die_attribute (mr_die, _DW_AT_byte_size);
  assert (attr != NULL);
  assert ((_DW_FORM_data1 == attr->form) || (_DW_FORM_data2 == attr->form) || (_DW_FORM_data4 == attr->form) || (_DW_FORM_data8 == attr->form));
  mr_type_sign.size = attr->dw_unsigned;
	
  attr = die_attribute (mr_die, _DW_AT_name);
  assert (attr != NULL);
  assert ((_DW_FORM_strp == attr->form) || (_DW_FORM_string == attr->form));
  mr_type_sign.type.str = attr->dw_str;

  mr_ptr_t * find = mr_ic_find (&mr_type_sign_ic, &mr_type_sign);
  assert (find != NULL);

  mr_type_sign_t * found_sign = find->ptr;
  fdp->mr_type_aux = found_sign->mr_type;
  fdp->size = mr_type_sign.size;
  if (fdp->type == NULL)
    fdp->type = mr_strdup (mr_type_sign.type.str);
}

static void
get_array_mr_type (mr_fd_t * fdp, mr_die_t * mr_die)
{
  assert (fdp->mr_type == MR_TYPE_NONE);
  fdp->mr_type = MR_TYPE_ARRAY;

  unsigned count = 1, dimension = 1;
  int i;
  for (i = mr_die->children_size / sizeof (mr_die->children[0]) - 1; i >= 0; --i)
    {
      assert (mr_die->children[i].tag == _DW_TAG_subrange_type);
      count *= dimension;
      dimension = 1;
      mr_dw_attribute_t * attr = die_attribute (&mr_die->children[i], _DW_AT_count);
      if (attr != NULL)
	{
	  assert ((_DW_FORM_data1 == attr->form) || (_DW_FORM_data2 == attr->form) || (_DW_FORM_data4 == attr->form) || (_DW_FORM_data8 == attr->form));
	  dimension = attr->dw_unsigned;
	}

      attr = die_attribute (&mr_die->children[i], _DW_AT_upper_bound);
      if (attr != NULL)
	{
	  assert ((_DW_FORM_data1 == attr->form) || (_DW_FORM_data2 == attr->form) || (_DW_FORM_data4 == attr->form) || (_DW_FORM_data8 == attr->form));
	  dimension = attr->dw_unsigned + 1;
	}
    }
  
  fdp->param.array_param.count = count * dimension;
  fdp->param.array_param.row_count = count;
}

static void
get_mr_type (mr_fd_t * fdp, mr_die_t * mr_die, mr_ic_t * die_off_ic)
{
  for (;;)
    {
      mr_dw_attribute_t * attr = die_attribute (mr_die, _DW_AT_type);
      if (attr == NULL)
	break;
      assert (_DW_FORM_ref4 == attr->form);
	
      mr_ptr_t * find = mr_ic_find (die_off_ic, (mr_die_t[]){{ .off = attr->dw_off }});
      assert (find != NULL);
      mr_die = find->ptr;
      
      switch (mr_die->tag)
	{
	case _DW_TAG_base_type:
	  get_base_mr_type (fdp, mr_die);
	  break;
      
	case _DW_TAG_subroutine_type:
	  assert (fdp->mr_type == MR_TYPE_POINTER);
	  fdp->mr_type = MR_TYPE_FUNC_TYPE;
	  break;
	
	case _DW_TAG_array_type:
	  get_array_mr_type (fdp, mr_die);
	  continue;
	
	case _DW_TAG_pointer_type:
	  if (fdp->mr_type != MR_TYPE_NONE)
	    break;
	  fdp->mr_type = MR_TYPE_POINTER;
	  continue;
	
	case _DW_TAG_typedef:
	  get_type_name (fdp, mr_die, die_off_ic);
	  continue;

	case _DW_TAG_structure_type:
	  fdp->mr_type_aux = MR_TYPE_STRUCT;
	  get_type_name (fdp, mr_die, die_off_ic);
	  break;
	
	case _DW_TAG_union_type:
	  fdp->mr_type_aux = MR_TYPE_UNION;
	  get_type_name (fdp, mr_die, die_off_ic);
	  break;
	
	case _DW_TAG_enumeration_type:
	  fdp->mr_type_aux = MR_TYPE_ENUM;
	  get_type_name (fdp, mr_die, die_off_ic);
	  break;
	
	case _DW_TAG_const_type:
	case _DW_TAG_volatile_type:
	  continue;
	  
	default:
	  assert (false);
	}
      break;
    }
}

static void
load_enumerator (mr_fd_t * fdp, mr_die_t * mr_die, mr_ic_t * die_off_ic)
{
  memset (fdp, 0, sizeof (*fdp));
  fdp->mr_type = MR_TYPE_ENUM_VALUE;
  
  mr_dw_attribute_t * attr = die_attribute (mr_die, _DW_AT_name);
  assert (attr != NULL);
  assert ((_DW_FORM_strp == attr->form) || (_DW_FORM_string == attr->form));
  assert (attr->dw_str != NULL);
  fdp->name.str = mr_strdup (attr->dw_str);

  attr = die_attribute (mr_die, _DW_AT_const_value);
  assert (attr != NULL);
  assert ((_DW_FORM_data1 == attr->form) || (_DW_FORM_data2 == attr->form) ||
	  (_DW_FORM_data4 == attr->form) || (_DW_FORM_data8 == attr->form) ||
	  (_DW_FORM_udata == attr->form) || (_DW_FORM_sdata == attr->form));

  if (_DW_FORM_sdata == attr->form)
    {
      fdp->param.enum_value._signed = attr->dw_signed;
      fdp->mr_type_aux = MR_TYPE_INT64;
    }
  else
    {
      fdp->param.enum_value._unsigned = attr->dw_unsigned;
      fdp->mr_type_aux = MR_TYPE_UINT64;
    }
}

static void
load_member (mr_fd_t * fdp, mr_die_t * mr_die, mr_ic_t * die_off_ic)
{
  memset (fdp, 0, sizeof (*fdp));
  
  mr_dw_attribute_t * attr = die_attribute (mr_die, _DW_AT_name);
  if (attr != NULL)
    {
      assert ((_DW_FORM_strp == attr->form) || (_DW_FORM_string == attr->form));
      assert (attr->dw_str != NULL);
      fdp->name.str = mr_strdup (attr->dw_str);
    }
  if (fdp->name.str == NULL)
    fdp->unnamed = true;

  attr = die_attribute (mr_die, _DW_AT_byte_size);
  if (attr != NULL)
    {
      assert ((_DW_FORM_data1 == attr->form) || (_DW_FORM_data2 == attr->form) || (_DW_FORM_data4 == attr->form) || (_DW_FORM_data8 == attr->form));
      fdp->size = attr->dw_unsigned;
    }
  
  attr = die_attribute (mr_die, _DW_AT_bit_size);
  if (attr != NULL)
    {
      assert ((_DW_FORM_data1 == attr->form) || (_DW_FORM_data2 == attr->form) || (_DW_FORM_data4 == attr->form) || (_DW_FORM_data8 == attr->form));
      fdp->mr_type = MR_TYPE_BITFIELD;
      fdp->param.bitfield_param.width = attr->dw_unsigned;

      attr = die_attribute (mr_die, _DW_AT_data_bit_offset);
      if (attr != NULL)
	{
	  assert ((_DW_FORM_data1 == attr->form) || (_DW_FORM_data2 == attr->form) || (_DW_FORM_data4 == attr->form) || (_DW_FORM_data8 == attr->form));
	  fdp->param.bitfield_param.shift = attr->dw_unsigned % __CHAR_BIT__;
	  fdp->offset = attr->dw_unsigned / __CHAR_BIT__;
	}

      attr = die_attribute (mr_die, _DW_AT_bit_offset);
      if (attr != NULL)
	{
	  assert ((_DW_FORM_data1 == attr->form) || (_DW_FORM_data2 == attr->form) || (_DW_FORM_data4 == attr->form) || (_DW_FORM_data8 == attr->form));
	  fdp->param.bitfield_param.shift = fdp->size * __CHAR_BIT__ - fdp->param.bitfield_param.width - attr->dw_unsigned;
	}
    }

  attr = die_attribute (mr_die, _DW_AT_data_member_location);
  if (attr != NULL)
    {
      assert ((_DW_FORM_data1 == attr->form) ||
	      (_DW_FORM_data2 == attr->form) ||
	      (_DW_FORM_data4 == attr->form) ||
	      (_DW_FORM_data8 == attr->form) ||
	      (_DW_FORM_block1 == attr->form));
      fdp->offset = attr->dw_unsigned;
    }

  get_mr_type (fdp, mr_die, die_off_ic);
  if (MR_TYPE_NONE == fdp->mr_type)
    fdp->mr_type = fdp->mr_type_aux;
}

static void
create_td (mr_ic_t * td_ic, mr_die_t * mr_die, mr_ic_t * die_off_ic)
{
  /* skip empty type declarations */
  mr_dw_attribute_t * attr = die_attribute (mr_die, _DW_AT_declaration);
  if (attr != NULL)
    return;

  /* skip unnamed anonymous types */
  attr = die_attribute (mr_die, _DW_AT_name);
  if (attr == NULL)
    return;
  
  assert ((_DW_FORM_strp == attr->form) || (_DW_FORM_string == attr->form));
  assert (attr->dw_str != NULL);

  /* skip types that are already extracted */
  mr_ptr_t * find = mr_ic_find (td_ic, (mr_td_t[]){ { .type = { .str = attr->dw_str, .hash_value = 0 } } });
  if (find != NULL)
    return;

  while (_DW_TAG_typedef == mr_die->tag)
    {
      /* traverse nested typedef definitions */
      mr_dw_attribute_t * attr = die_attribute (mr_die, _DW_AT_type);
      if (attr == NULL)
	return;
      assert (_DW_FORM_ref4 == attr->form);
	
      mr_ptr_t * find = mr_ic_find (die_off_ic, (mr_die_t[]){{ .off = attr->dw_off }});
      assert (find != NULL);
      mr_die = find->ptr;
    }

  mr_type_t mr_type = MR_TYPE_NONE;
  mr_dw_tag_t children_tag = _DW_TAG_undefined;
  void (*load_child) (mr_fd_t * fdp, mr_die_t * mr_die, mr_ic_t * die_off_ic) = NULL;

  switch (mr_die->tag)
    {
    case _DW_TAG_structure_type:
      mr_type = MR_TYPE_STRUCT;
      children_tag = _DW_TAG_member;
      load_child = load_member;
      break;
    case _DW_TAG_union_type:
      mr_type = MR_TYPE_UNION;
      children_tag = _DW_TAG_member;
      load_child = load_member;
      break;
    case _DW_TAG_enumeration_type:
      mr_type = MR_TYPE_ENUM;
      children_tag = _DW_TAG_enumerator;
      load_child = load_enumerator;
      break;
    case _DW_TAG_pointer_type:
      {
	mr_fd_t fd = { .mr_type = MR_TYPE_POINTER };
	get_mr_type (&fd, mr_die, die_off_ic);
	MR_FREE_RECURSIVELY (mr_fd_t, &fd);
	
	if (MR_TYPE_CHAR == fd.mr_type_aux)
	  mr_type = MR_TYPE_STRING;
	else if (MR_TYPE_FUNC_TYPE == fd.mr_type)
	  mr_type = MR_TYPE_FUNC_TYPE;
	else
	  return;
      }
      break;
    default:
      return;
    }

  mr_td_t * tdp = MR_CALLOC (1, sizeof (*tdp));
  assert (tdp != NULL);
  
  memset (tdp, 0, sizeof (*tdp));
  tdp->mr_type = mr_type;
  tdp->type.str = mr_strdup (attr->dw_str);
  if ((MR_TYPE_STRING == mr_type) || (MR_TYPE_FUNC_TYPE == mr_type))
    tdp->size = sizeof (void *);

  find = mr_ic_add (td_ic, tdp);
  assert (find != NULL);

  attr = die_attribute (mr_die, _DW_AT_byte_size);
  if (attr != NULL)
    {
      assert ((_DW_FORM_data1 == attr->form) || (_DW_FORM_data2 == attr->form) || (_DW_FORM_data4 == attr->form) || (_DW_FORM_data8 == attr->form));
      tdp->size = attr->dw_unsigned;
    }

  ssize_t alloc_size = 0;
  if (load_child != NULL)
    {
      int i, count = mr_die->children_size / sizeof (mr_die->children[0]);
      for (i = 0; i < count; ++i)
	if (mr_die->children[i].tag == children_tag)
	  {
	    mr_fd_ptr_t * fdp_ptr = mr_rarray_allocate_element ((void*)&tdp->fields, &tdp->fields_size, &alloc_size, sizeof (tdp->fields[0]));
	    assert (fdp_ptr != NULL);
	    fdp_ptr->fdp = MR_CALLOC (1, sizeof (*fdp_ptr->fdp));
	    assert (fdp_ptr->fdp != NULL);
      
	    load_child (fdp_ptr->fdp, &mr_die->children[i], die_off_ic);
	  }
    }
  
  mr_fd_ptr_t * fdp_ptr = mr_rarray_allocate_element ((void*)&tdp->fields, &tdp->fields_size, &alloc_size, sizeof (tdp->fields[0]));
  assert (fdp_ptr != NULL);
  fdp_ptr->fdp = MR_CALLOC (1, sizeof (*fdp_ptr->fdp));
  assert (fdp_ptr->fdp != NULL);
  fdp_ptr->fdp->mr_type = MR_TYPE_TRAILING_RECORD;
}

static mr_hash_value_t
die_off_hash (mr_ptr_t x, const void * context)
{
  mr_die_t * x_ = x.ptr;
  return (x_->off);
}

static int
die_off_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const mr_die_t * x_ = x.ptr;
  const mr_die_t * y_ = y.ptr;
  return ((x_->off > y_->off) - (x_->off < y_->off));
}

static mr_hash_value_t
mr_type_sign_hash (mr_ptr_t x, const void * context)
{
  mr_type_sign_t * x_ = x.ptr;
  return (mr_hashed_string_get_hash (&x_->type) + x_->size);
}

static int
mr_type_sign_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const mr_type_sign_t * x_ = x.ptr;
  const mr_type_sign_t * y_ = y.ptr;
  int diff = (x_->size > y_->size) - (x_->size < y_->size);
  if (diff)
    return (diff);
  return (mr_hashed_string_cmp (&x_->type, &y_->type));
}

#define MR_BI_SIGN_(TYPE, TYPE_STR) (mr_type_sign_t[]){{ .type = { .str = TYPE_STR, .hash_value = 0, }, .size = sizeof (TYPE), .mr_type = MR_TYPE_DETECT (TYPE), }}
#define MR_BI_SIGN(TYPE, ...) MR_IF_ELSE (MR_IS_EMPTY (__VA_ARGS__)) (MR_BI_SIGN_ (TYPE, #TYPE)) (MR_BI_SIGN_ (TYPE, __VA_ARGS__)) 

static mr_type_sign_t * mr_type_sign[] =
{
  MR_BI_SIGN (_Bool),
  MR_BI_SIGN (typeof (sizeof (0)), "__ARRAY_SIZE_TYPE__"),
  MR_BI_SIGN (char),
  MR_BI_SIGN (complex float, "complex"),
  MR_BI_SIGN (complex double, "complex"),
  MR_BI_SIGN (complex long double, "complex"),
  MR_BI_SIGN (complex float),
  MR_BI_SIGN (complex double),
  MR_BI_SIGN (complex long double),
  MR_BI_SIGN (double),
  MR_BI_SIGN (float),
  MR_BI_SIGN (int),
  MR_BI_SIGN (long double),
  MR_BI_SIGN (long int),
  MR_BI_SIGN (long long int),
  MR_BI_SIGN (long long unsigned int),
  MR_BI_SIGN (long unsigned int),
  MR_BI_SIGN (short),
  MR_BI_SIGN (short int),
  MR_BI_SIGN (short unsigned int),
  MR_BI_SIGN (signed char),
  MR_BI_SIGN (unsigned char),
  MR_BI_SIGN (unsigned int),
  MR_BI_SIGN (unsigned short),
};

static void
extract_type_descriptors (mr_ic_t * td_ic, mr_die_t * mr_die)
{
  mr_ic_rarray_t ra_die_ptr;
  mr_ic_t die_off_ic;
  mr_status_t status;
  
  memset (&ra_die_ptr, 0, sizeof (ra_die_ptr));
  walk_dies (mr_die, &ra_die_ptr);

  status = mr_ic_new (&die_off_ic, die_off_hash, die_off_cmp, "mr_die_t", MR_IC_HASH, NULL);
  assert (status == MR_SUCCESS);
  status = mr_ic_index (&die_off_ic, &ra_die_ptr);
  assert (status == MR_SUCCESS);
  
  int i, j;
  for (j = 0; j < 2; ++j) /* required anonymous types identified on a first iteration and extracted on a second */
    for (i = ra_die_ptr.size / sizeof (ra_die_ptr.ra[0]) - 1; i >= 0; --i)
      {
	mr_die_t * mr_die = ra_die_ptr.ra[i].ptr;
      
	switch (mr_die->tag)
	  {
	  case _DW_TAG_typedef:
	  case _DW_TAG_structure_type:
	  case _DW_TAG_union_type:
	  case _DW_TAG_enumeration_type:
	    create_td (td_ic, mr_die, &die_off_ic);
	    break;
	  default:
	    break;
	  }
      }

  if (ra_die_ptr.ra)
    MR_FREE (ra_die_ptr.ra);
  
  mr_ic_free (&die_off_ic);
}

static mr_status_t
process_td (mr_ptr_t key, const void * context)
{
  mr_td_t * tdp = key.ptr;
  mr_ic_t * td_ic = (mr_ic_t*)context;
  int i;

  if (tdp->mr_type == MR_TYPE_ENUM)
    {
      bool _signed = ((tdp->fields_size >= sizeof (tdp->fields[0])) &&
		      (tdp->fields[0].fdp->mr_type_aux == MR_TYPE_INT64));
      tdp->param.enum_param.size_effective = tdp->size;
      switch (tdp->size)
	{
	case sizeof (uint8_t):
	  tdp->param.enum_param.mr_type_effective = _signed ? MR_TYPE_INT8 : MR_TYPE_UINT8;
	  break;
	case sizeof (uint16_t):
	  tdp->param.enum_param.mr_type_effective = _signed ? MR_TYPE_INT16 : MR_TYPE_UINT16;
	  break;
	case sizeof (uint32_t):
	  tdp->param.enum_param.mr_type_effective = _signed ? MR_TYPE_INT32 : MR_TYPE_UINT32;
	  break;
	case sizeof (uint64_t):
	  tdp->param.enum_param.mr_type_effective = _signed ? MR_TYPE_INT64 : MR_TYPE_UINT64;
	  break;
	}
    }
  else
    for (i = tdp->fields_size / sizeof (tdp->fields[0]) - 1; i >= 0; --i)
      {
	mr_fd_t * fdp = tdp->fields[i].fdp;

	switch (fdp->mr_type)
	  {
	  case MR_TYPE_UNION:
	  case MR_TYPE_ENUM:
	  case MR_TYPE_STRUCT:
	  case MR_TYPE_ARRAY:
	  case MR_TYPE_BITFIELD:
	    if (0 == fdp->size)
	      {
		mr_ptr_t * find = mr_ic_find (td_ic, (mr_td_t[]){{ .type = { .str = fdp->type, .hash_value = 0 }}});
		if (find)
		  {
		    mr_td_t * field_tdp = find->ptr;
		    fdp->size = field_tdp->size;
		  }
	      }
	    break;

	  case MR_TYPE_FUNC_TYPE:
	    fdp->size = sizeof (void *);
	    break;

	  case MR_TYPE_POINTER:
	    fdp->size = sizeof (void *);
	    if (fdp->mr_type_aux == MR_TYPE_NONE)
	      fdp->mr_type_aux = MR_TYPE_VOID;
	    else if (fdp->mr_type_aux == MR_TYPE_CHAR)
	      fdp->mr_type = MR_TYPE_STRING;
	    else
	      {
#define POINTER_SIZE_SUFFIX "_size"
		assert (fdp->name.str != NULL);
		char * size = MR_CALLOC (strlen (fdp->name.str) + sizeof (POINTER_SIZE_SUFFIX), sizeof (fdp->name.str[0]));
		assert (size != NULL);
		strcpy (size, fdp->name.str);
		strcat (size, POINTER_SIZE_SUFFIX);
		fdp->res.ptr = size;
		fdp->res_type = mr_strdup ("string");
		assert (fdp->res_type != NULL);
	      }
	    break;

	  default:
	    break;
	  }

	if (MR_TYPE_ARRAY == fdp->mr_type)
	  fdp->size *= fdp->param.array_param.count;

	if ((fdp->name.str != NULL) &&
	    ((MR_TYPE_UNION == fdp->mr_type) || (MR_TYPE_UNION == fdp->mr_type_aux)))
	  {
#define UNION_DISCRIMINATOR_SUFFIX "_discriminator"
	    char * discriminator = MR_CALLOC (strlen (fdp->name.str) + sizeof (UNION_DISCRIMINATOR_SUFFIX), sizeof (fdp->name.str[0]));
	    assert (discriminator != NULL);
	    strcpy (discriminator, fdp->name.str);
	    strcat (discriminator, UNION_DISCRIMINATOR_SUFFIX);
	    fdp->meta = discriminator;
	  }
      }
  return (MR_SUCCESS);
}

static mr_status_t
print_td (mr_ptr_t key, const void * context)
{
  mr_td_t * tdp = key.ptr;
  char * dump = MR_SAVE_CINIT (mr_td_t, tdp);
  if (dump)
    {
      printf ("%s,\n", dump);
      MR_FREE (dump);
    }
  return (MR_SUCCESS);
}

static mr_status_t
free_td (mr_ptr_t key, const void * context)
{
  mr_td_t * tdp = key.ptr;
  MR_FREE_RECURSIVELY (mr_td_t, tdp);
  MR_FREE (tdp);
  return (MR_SUCCESS);
}

static void
tweak_mr_conf ()
{
  mr_type_void_fields ("mr_td_t", "field_by_name", "attr", "meta", "res", "res_type", "mr_size", NULL);
  mr_type_void_fields ("mr_fd_t", "mr_size", NULL);
  mr_type_void_fields ("mr_struct_param_t", "field_by_offset", NULL);
  mr_type_void_fields ("mr_enum_param_t", "enum_by_value", "is_bitmask", NULL);
  mr_type_void_fields ("mr_hashed_string_t", "hash_value", "padding", NULL);
}

int
main (int argc, char * argv [])
{
  if (argc < 2)
    {
      fprintf (stderr, "Usage: %s filename\n", argv[0]);
      return (EXIT_FAILURE);
    }
  
  mr_ic_rarray_t ic_rarray = { .ra = (void*)mr_type_sign, .size = sizeof (mr_type_sign), };
  mr_status_t status = mr_ic_new (&mr_type_sign_ic, mr_type_sign_hash, mr_type_sign_cmp, "mr_type_sign_t", MR_IC_HASH, NULL);
  assert (status == MR_SUCCESS);
  status = mr_ic_index (&mr_type_sign_ic, &ic_rarray);
  assert (status == MR_SUCCESS);

  mr_ic_t td_ic;
  status = mr_ic_new (&td_ic, mr_td_name_get_hash, mr_td_name_cmp, "mr_td_t", MR_IC_HASH, NULL);
  assert (status == MR_SUCCESS);

  int i;
  for (i = 1; i < argc; ++i)
    {
      Dwarf_Debug debug;
      Dwarf_Error error = 0;
  
#ifdef HAVE_DWARF_INIT_PATH
      char path[1 << 13];
      int rv = dwarf_init_path (argv[i], path, sizeof (path), DW_DLC_READ, DW_GROUPNUMBER_ANY, NULL, NULL, &debug, NULL, 0, NULL, &error);
#else /* ! HAVE_DWARF_INIT_PATH */
      int fd = open (argv[i], O_RDONLY);
      assert (fd > 0);
      int rv = dwarf_init (fd, DW_DLC_READ, NULL, NULL, &debug, &error);
#endif /* HAVE_DWARF_INIT_PATH */
  
      if (rv != DW_DLV_OK)
	{
	  printf ("libdwarf error: exit code %d, error code %llu, %s\n", rv, dwarf_errno (error), dwarf_errmsg (error));
	  return (EXIT_FAILURE);
	}

      mr_die_t mr_die;
      memset (&mr_die, 0, sizeof (mr_die));
      dump_cu_list (debug, &mr_die);

#ifdef DEBUG
      {
	char * dump = MR_SAVE_CINIT (mr_die_t, &mr_die);
	if (dump)
	  {
	    fprintf (stderr, "mr_die_root = %s", dump);
	    MR_FREE (dump);
	  }
      }
#endif
  
      extract_type_descriptors (&td_ic, &mr_die);

      MR_FREE_RECURSIVELY (mr_die_t, &mr_die);

      rv = dwarf_finish (debug, NULL);
      assert (rv == DW_DLV_OK);
  
#ifndef HAVE_DWARF_INIT_PATH
      rv = close (fd);
      assert (rv == 0);
#endif /* HAVE_DWARF_INIT_PATH */
    }

  tweak_mr_conf ();
  mr_ic_foreach (&td_ic, process_td, &td_ic);
  mr_ic_foreach (&td_ic, print_td, NULL);
  mr_ic_foreach (&td_ic, free_td, NULL);
  mr_ic_free (&td_ic);

  mr_ic_free (&mr_type_sign_ic);

  return (EXIT_SUCCESS);
}
