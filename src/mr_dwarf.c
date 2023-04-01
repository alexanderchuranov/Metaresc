#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> /* EXIT_SUCCESS/EXIT_FAILURE */
#include <fcntl.h> /* open () */
#include <unistd.h> /* close () */

#include <metaresc.h>
#include <mr_ic.h>
#include <mr_save.h> /* MR_ONE_SHIFT */

#include <dwarf.h>
#include <libdwarf.h>

/* awk '/#define DW_TAG_/{print "(_"$2", = "$2"),"}' < ../../libdwarf-code/src/lib/libdwarf/dwarf.h */
TYPEDEF_ENUM (mr_dw_tag_t, ATTRIBUTES ( , "awk '/#define DW_TAG_/{print \"(_\"$2\", = \"$2\"),\"}' < ../../libdwarf-code/src/lib/libdwarf/dwarf.h"),
	      (_DW_TAG_undefined, = 0),
	      (_DW_TAG_array_type, = DW_TAG_array_type),
	      (_DW_TAG_class_type, = DW_TAG_class_type),
	      (_DW_TAG_entry_point, = DW_TAG_entry_point),
	      (_DW_TAG_enumeration_type, = DW_TAG_enumeration_type),
	      (_DW_TAG_formal_parameter, = DW_TAG_formal_parameter),
	      (_DW_TAG_imported_declaration, = DW_TAG_imported_declaration),
	      (_DW_TAG_label, = DW_TAG_label),
	      (_DW_TAG_lexical_block, = DW_TAG_lexical_block),
	      (_DW_TAG_member, = DW_TAG_member),
	      (_DW_TAG_pointer_type, = DW_TAG_pointer_type),
	      (_DW_TAG_reference_type, = DW_TAG_reference_type),
	      (_DW_TAG_compile_unit, = DW_TAG_compile_unit),
	      (_DW_TAG_string_type, = DW_TAG_string_type),
	      (_DW_TAG_structure_type, = DW_TAG_structure_type),
	      (_DW_TAG_subroutine_type, = DW_TAG_subroutine_type),
	      (_DW_TAG_typedef, = DW_TAG_typedef),
	      (_DW_TAG_union_type, = DW_TAG_union_type),
	      (_DW_TAG_unspecified_parameters, = DW_TAG_unspecified_parameters),
	      (_DW_TAG_variant, = DW_TAG_variant),
	      (_DW_TAG_common_block, = DW_TAG_common_block),
	      (_DW_TAG_common_inclusion, = DW_TAG_common_inclusion),
	      (_DW_TAG_inheritance, = DW_TAG_inheritance),
	      (_DW_TAG_inlined_subroutine, = DW_TAG_inlined_subroutine),
	      (_DW_TAG_module, = DW_TAG_module),
	      (_DW_TAG_ptr_to_member_type, = DW_TAG_ptr_to_member_type),
	      (_DW_TAG_set_type, = DW_TAG_set_type),
	      (_DW_TAG_subrange_type, = DW_TAG_subrange_type),
	      (_DW_TAG_with_stmt, = DW_TAG_with_stmt),
	      (_DW_TAG_access_declaration, = DW_TAG_access_declaration),
	      (_DW_TAG_base_type, = DW_TAG_base_type),
	      (_DW_TAG_catch_block, = DW_TAG_catch_block),
	      (_DW_TAG_const_type, = DW_TAG_const_type),
	      (_DW_TAG_constant, = DW_TAG_constant),
	      (_DW_TAG_enumerator, = DW_TAG_enumerator),
	      (_DW_TAG_file_type, = DW_TAG_file_type),
	      (_DW_TAG_friend, = DW_TAG_friend),
	      (_DW_TAG_namelist, = DW_TAG_namelist),
	      (_DW_TAG_namelist_item, = DW_TAG_namelist_item),
	      (_DW_TAG_namelist_items, = DW_TAG_namelist_items),
	      (_DW_TAG_packed_type, = DW_TAG_packed_type),
	      (_DW_TAG_subprogram, = DW_TAG_subprogram),
	      (_DW_TAG_template_type_parameter, = DW_TAG_template_type_parameter),
	      (_DW_TAG_template_type_param, = DW_TAG_template_type_param),
	      (_DW_TAG_template_value_parameter, = DW_TAG_template_value_parameter),
	      (_DW_TAG_template_value_param, = DW_TAG_template_value_param),
	      (_DW_TAG_thrown_type, = DW_TAG_thrown_type),
	      (_DW_TAG_try_block, = DW_TAG_try_block),
	      (_DW_TAG_variant_part, = DW_TAG_variant_part),
	      (_DW_TAG_variable, = DW_TAG_variable),
	      (_DW_TAG_volatile_type, = DW_TAG_volatile_type),
	      (_DW_TAG_dwarf_procedure, = DW_TAG_dwarf_procedure),
	      (_DW_TAG_restrict_type, = DW_TAG_restrict_type),
	      (_DW_TAG_interface_type, = DW_TAG_interface_type),
	      (_DW_TAG_namespace, = DW_TAG_namespace),
	      (_DW_TAG_imported_module, = DW_TAG_imported_module),
	      (_DW_TAG_unspecified_type, = DW_TAG_unspecified_type),
	      (_DW_TAG_partial_unit, = DW_TAG_partial_unit),
	      (_DW_TAG_imported_unit, = DW_TAG_imported_unit),
	      (_DW_TAG_mutable_type, = DW_TAG_mutable_type),
	      (_DW_TAG_condition, = DW_TAG_condition),
	      (_DW_TAG_shared_type, = DW_TAG_shared_type),
	      (_DW_TAG_type_unit, = DW_TAG_type_unit),
	      (_DW_TAG_rvalue_reference_type, = DW_TAG_rvalue_reference_type),
	      (_DW_TAG_template_alias, = DW_TAG_template_alias),
	      (_DW_TAG_coarray_type, = DW_TAG_coarray_type),
	      (_DW_TAG_generic_subrange, = DW_TAG_generic_subrange),
	      (_DW_TAG_dynamic_type, = DW_TAG_dynamic_type),
	      (_DW_TAG_atomic_type, = DW_TAG_atomic_type),
	      (_DW_TAG_call_site, = DW_TAG_call_site),
	      (_DW_TAG_call_site_parameter, = DW_TAG_call_site_parameter),
	      (_DW_TAG_skeleton_unit, = DW_TAG_skeleton_unit),
	      (_DW_TAG_immutable_type, = DW_TAG_immutable_type),
	      (_DW_TAG_lo_user, = DW_TAG_lo_user),
	      (_DW_TAG_MIPS_loop, = DW_TAG_MIPS_loop),
	      (_DW_TAG_HP_array_descriptor, = DW_TAG_HP_array_descriptor),
	      (_DW_TAG_format_label, = DW_TAG_format_label),
	      (_DW_TAG_function_template, = DW_TAG_function_template),
	      (_DW_TAG_class_template, = DW_TAG_class_template),
	      (_DW_TAG_GNU_BINCL, = DW_TAG_GNU_BINCL),
	      (_DW_TAG_GNU_EINCL, = DW_TAG_GNU_EINCL),
	      (_DW_TAG_GNU_template_template_parameter, = DW_TAG_GNU_template_template_parameter),
	      (_DW_TAG_GNU_template_template_param, = DW_TAG_GNU_template_template_param),
	      (_DW_TAG_GNU_template_parameter_pack, = DW_TAG_GNU_template_parameter_pack),
	      (_DW_TAG_GNU_formal_parameter_pack, = DW_TAG_GNU_formal_parameter_pack),
	      (_DW_TAG_GNU_call_site, = DW_TAG_GNU_call_site),
	      (_DW_TAG_GNU_call_site_parameter, = DW_TAG_GNU_call_site_parameter),
	      (_DW_TAG_SUN_function_template, = DW_TAG_SUN_function_template),
	      (_DW_TAG_SUN_class_template, = DW_TAG_SUN_class_template),
	      (_DW_TAG_SUN_struct_template, = DW_TAG_SUN_struct_template),
	      (_DW_TAG_SUN_union_template, = DW_TAG_SUN_union_template),
	      (_DW_TAG_SUN_indirect_inheritance, = DW_TAG_SUN_indirect_inheritance),
	      (_DW_TAG_SUN_codeflags, = DW_TAG_SUN_codeflags),
	      (_DW_TAG_SUN_memop_info, = DW_TAG_SUN_memop_info),
	      (_DW_TAG_SUN_omp_child_func, = DW_TAG_SUN_omp_child_func),
	      (_DW_TAG_SUN_rtti_descriptor, = DW_TAG_SUN_rtti_descriptor),
	      (_DW_TAG_SUN_dtor_info, = DW_TAG_SUN_dtor_info),
	      (_DW_TAG_SUN_dtor, = DW_TAG_SUN_dtor),
	      (_DW_TAG_SUN_f90_interface, = DW_TAG_SUN_f90_interface),
	      (_DW_TAG_SUN_fortran_vax_structure, = DW_TAG_SUN_fortran_vax_structure),
	      (_DW_TAG_SUN_hi, = DW_TAG_SUN_hi),
	      (_DW_TAG_ALTIUM_circ_type, = DW_TAG_ALTIUM_circ_type),
	      (_DW_TAG_ALTIUM_mwa_circ_type, = DW_TAG_ALTIUM_mwa_circ_type),
	      (_DW_TAG_ALTIUM_rev_carry_type, = DW_TAG_ALTIUM_rev_carry_type),
	      (_DW_TAG_ALTIUM_rom, = DW_TAG_ALTIUM_rom),
	      (_DW_TAG_LLVM_annotation, = DW_TAG_LLVM_annotation),
	      (_DW_TAG_ghs_namespace, = DW_TAG_ghs_namespace),
	      (_DW_TAG_ghs_using_namespace, = DW_TAG_ghs_using_namespace),
	      (_DW_TAG_ghs_using_declaration, = DW_TAG_ghs_using_declaration),
	      (_DW_TAG_ghs_template_templ_param, = DW_TAG_ghs_template_templ_param),
	      (_DW_TAG_upc_shared_type, = DW_TAG_upc_shared_type),
	      (_DW_TAG_upc_strict_type, = DW_TAG_upc_strict_type),
	      (_DW_TAG_upc_relaxed_type, = DW_TAG_upc_relaxed_type),
	      (_DW_TAG_PGI_kanji_type, = DW_TAG_PGI_kanji_type),
	      (_DW_TAG_PGI_interface_block, = DW_TAG_PGI_interface_block),
	      (_DW_TAG_BORLAND_property, = DW_TAG_BORLAND_property),
	      (_DW_TAG_BORLAND_Delphi_string, = DW_TAG_BORLAND_Delphi_string),
	      (_DW_TAG_BORLAND_Delphi_dynamic_array, = DW_TAG_BORLAND_Delphi_dynamic_array),
	      (_DW_TAG_BORLAND_Delphi_set, = DW_TAG_BORLAND_Delphi_set),
	      (_DW_TAG_BORLAND_Delphi_variant, = DW_TAG_BORLAND_Delphi_variant),
	      (_DW_TAG_hi_user, = DW_TAG_hi_user),
	      )

TYPEDEF_ENUM (mr_dw_attribute_code_t,
	      (_DW_AT_sibling, = DW_AT_sibling),
	      (_DW_AT_location, = DW_AT_location),
	      (_DW_AT_name, = DW_AT_name),
	      (_DW_AT_ordering, = DW_AT_ordering),
	      (_DW_AT_subscr_data, = DW_AT_subscr_data),
	      (_DW_AT_byte_size, = DW_AT_byte_size),
	      (_DW_AT_bit_offset, = DW_AT_bit_offset),
	      (_DW_AT_bit_size, = DW_AT_bit_size),
	      (_DW_AT_element_list, = DW_AT_element_list),
	      (_DW_AT_stmt_list, = DW_AT_stmt_list),
	      (_DW_AT_low_pc, = DW_AT_low_pc),
	      (_DW_AT_high_pc, = DW_AT_high_pc),
	      (_DW_AT_language, = DW_AT_language),
	      (_DW_AT_member, = DW_AT_member),
	      (_DW_AT_discr, = DW_AT_discr),
	      (_DW_AT_discr_value, = DW_AT_discr_value),
	      (_DW_AT_visibility, = DW_AT_visibility),
	      (_DW_AT_import, = DW_AT_import),
	      (_DW_AT_string_length, = DW_AT_string_length),
	      (_DW_AT_common_reference, = DW_AT_common_reference),
	      (_DW_AT_comp_dir, = DW_AT_comp_dir),
	      (_DW_AT_const_value, = DW_AT_const_value),
	      (_DW_AT_containing_type, = DW_AT_containing_type),
	      (_DW_AT_default_value, = DW_AT_default_value),
	      (_DW_AT_inline, = DW_AT_inline),
	      (_DW_AT_is_optional, = DW_AT_is_optional),
	      (_DW_AT_lower_bound, = DW_AT_lower_bound),
	      (_DW_AT_producer, = DW_AT_producer),
	      (_DW_AT_prototyped, = DW_AT_prototyped),
	      (_DW_AT_return_addr, = DW_AT_return_addr),
	      (_DW_AT_start_scope, = DW_AT_start_scope),
	      (_DW_AT_bit_stride, = DW_AT_bit_stride),
	      (_DW_AT_stride_size, = DW_AT_stride_size),
	      (_DW_AT_upper_bound, = DW_AT_upper_bound),
	      (_DW_AT_abstract_origin, = DW_AT_abstract_origin),
	      (_DW_AT_accessibility, = DW_AT_accessibility),
	      (_DW_AT_address_class, = DW_AT_address_class),
	      (_DW_AT_artificial, = DW_AT_artificial),
	      (_DW_AT_base_types, = DW_AT_base_types),
	      (_DW_AT_calling_convention, = DW_AT_calling_convention),
	      (_DW_AT_count, = DW_AT_count),
	      (_DW_AT_data_member_location, = DW_AT_data_member_location),
	      (_DW_AT_decl_column, = DW_AT_decl_column),
	      (_DW_AT_decl_file, = DW_AT_decl_file),
	      (_DW_AT_decl_line, = DW_AT_decl_line),
	      (_DW_AT_declaration, = DW_AT_declaration),
	      (_DW_AT_discr_list, = DW_AT_discr_list),
	      (_DW_AT_encoding, = DW_AT_encoding),
	      (_DW_AT_external, = DW_AT_external),
	      (_DW_AT_frame_base, = DW_AT_frame_base),
	      (_DW_AT_friend, = DW_AT_friend),
	      (_DW_AT_identifier_case, = DW_AT_identifier_case),
	      (_DW_AT_macro_info, = DW_AT_macro_info),
	      (_DW_AT_namelist_item, = DW_AT_namelist_item),
	      (_DW_AT_priority, = DW_AT_priority),
	      (_DW_AT_segment, = DW_AT_segment),
	      (_DW_AT_specification, = DW_AT_specification),
	      (_DW_AT_static_link, = DW_AT_static_link),
	      (_DW_AT_type, = DW_AT_type),
	      (_DW_AT_use_location, = DW_AT_use_location),
	      (_DW_AT_variable_parameter, = DW_AT_variable_parameter),
	      (_DW_AT_virtuality, = DW_AT_virtuality),
	      (_DW_AT_vtable_elem_location, = DW_AT_vtable_elem_location),
	      (_DW_AT_allocated, = DW_AT_allocated),
	      (_DW_AT_associated, = DW_AT_associated),
	      (_DW_AT_data_location, = DW_AT_data_location),
	      (_DW_AT_byte_stride, = DW_AT_byte_stride),
	      (_DW_AT_stride, = DW_AT_stride),
	      (_DW_AT_entry_pc, = DW_AT_entry_pc),
	      (_DW_AT_use_UTF8, = DW_AT_use_UTF8),
	      (_DW_AT_extension, = DW_AT_extension),
	      (_DW_AT_ranges, = DW_AT_ranges),
	      (_DW_AT_trampoline, = DW_AT_trampoline),
	      (_DW_AT_call_column, = DW_AT_call_column),
	      (_DW_AT_call_file, = DW_AT_call_file),
	      (_DW_AT_call_line, = DW_AT_call_line),
	      (_DW_AT_description, = DW_AT_description),
	      (_DW_AT_binary_scale, = DW_AT_binary_scale),
	      (_DW_AT_decimal_scale, = DW_AT_decimal_scale),
	      (_DW_AT_small, = DW_AT_small),
	      (_DW_AT_decimal_sign, = DW_AT_decimal_sign),
	      (_DW_AT_digit_count, = DW_AT_digit_count),
	      (_DW_AT_picture_string, = DW_AT_picture_string),
	      (_DW_AT_mutable, = DW_AT_mutable),
	      (_DW_AT_threads_scaled, = DW_AT_threads_scaled),
	      (_DW_AT_explicit, = DW_AT_explicit),
	      (_DW_AT_object_pointer, = DW_AT_object_pointer),
	      (_DW_AT_endianity, = DW_AT_endianity),
	      (_DW_AT_elemental, = DW_AT_elemental),
	      (_DW_AT_pure, = DW_AT_pure),
	      (_DW_AT_recursive, = DW_AT_recursive),
	      (_DW_AT_signature, = DW_AT_signature),
	      (_DW_AT_main_subprogram, = DW_AT_main_subprogram),
	      (_DW_AT_data_bit_offset, = DW_AT_data_bit_offset),
	      (_DW_AT_const_expr, = DW_AT_const_expr),
	      (_DW_AT_enum_class, = DW_AT_enum_class),
	      (_DW_AT_linkage_name, = DW_AT_linkage_name),
	      (_DW_AT_string_length_bit_size, = DW_AT_string_length_bit_size),
	      (_DW_AT_string_length_byte_size, = DW_AT_string_length_byte_size),
	      (_DW_AT_rank, = DW_AT_rank),
	      (_DW_AT_str_offsets_base, = DW_AT_str_offsets_base),
	      (_DW_AT_addr_base, = DW_AT_addr_base),
	      (_DW_AT_rnglists_base, = DW_AT_rnglists_base),
	      (_DW_AT_dwo_id, = DW_AT_dwo_id),
	      (_DW_AT_dwo_name, = DW_AT_dwo_name),
	      (_DW_AT_reference, = DW_AT_reference),
	      (_DW_AT_rvalue_reference, = DW_AT_rvalue_reference),
	      (_DW_AT_macros, = DW_AT_macros),
	      (_DW_AT_call_all_calls, = DW_AT_call_all_calls),
	      (_DW_AT_call_all_source_calls, = DW_AT_call_all_source_calls),
	      (_DW_AT_call_all_tail_calls, = DW_AT_call_all_tail_calls),
	      (_DW_AT_call_return_pc, = DW_AT_call_return_pc),
	      (_DW_AT_call_value, = DW_AT_call_value),
	      (_DW_AT_call_origin, = DW_AT_call_origin),
	      (_DW_AT_call_parameter, = DW_AT_call_parameter),
	      (_DW_AT_call_pc, = DW_AT_call_pc),
	      (_DW_AT_call_tail_call, = DW_AT_call_tail_call),
	      (_DW_AT_call_target, = DW_AT_call_target),
	      (_DW_AT_call_target_clobbered, = DW_AT_call_target_clobbered),
	      (_DW_AT_call_data_location, = DW_AT_call_data_location),
	      (_DW_AT_call_data_value, = DW_AT_call_data_value),
	      (_DW_AT_noreturn, = DW_AT_noreturn),
	      (_DW_AT_alignment, = DW_AT_alignment),
	      (_DW_AT_export_symbols, = DW_AT_export_symbols),
	      (_DW_AT_deleted, = DW_AT_deleted),
	      (_DW_AT_defaulted, = DW_AT_defaulted),
	      (_DW_AT_loclists_base, = DW_AT_loclists_base),
	      (_DW_AT_ghs_namespace_alias, = DW_AT_ghs_namespace_alias),
	      (_DW_AT_ghs_using_namespace, = DW_AT_ghs_using_namespace),
	      (_DW_AT_ghs_using_declaration, = DW_AT_ghs_using_declaration),
	      (_DW_AT_HP_block_index, = DW_AT_HP_block_index),
	      (_DW_AT_lo_user, = DW_AT_lo_user),
	      (_DW_AT_ghs_mangled, = DW_AT_ghs_mangled),
	      (_DW_AT_ghs_rsm, = DW_AT_ghs_rsm),
	      (_DW_AT_ghs_frsm, = DW_AT_ghs_frsm),
	      (_DW_AT_ghs_frames, = DW_AT_ghs_frames),
	      (_DW_AT_ghs_rso, = DW_AT_ghs_rso),
	      (_DW_AT_ghs_subcpu, = DW_AT_ghs_subcpu),
	      (_DW_AT_ghs_lbrace_line, = DW_AT_ghs_lbrace_line),
	      (_DW_AT_INTEL_other_endian, = DW_AT_INTEL_other_endian),
	      (_DW_AT_sf_names, = DW_AT_sf_names),
	      (_DW_AT_src_info, = DW_AT_src_info),
	      (_DW_AT_mac_info, = DW_AT_mac_info),
	      (_DW_AT_src_coords, = DW_AT_src_coords),
	      (_DW_AT_body_begin, = DW_AT_body_begin),
	      (_DW_AT_body_end, = DW_AT_body_end),
	      (_DW_AT_GNU_vector, = DW_AT_GNU_vector),
	      (_DW_AT_GNU_guarded_by, = DW_AT_GNU_guarded_by),
	      (_DW_AT_GNU_pt_guarded_by, = DW_AT_GNU_pt_guarded_by),
	      (_DW_AT_GNU_guarded, = DW_AT_GNU_guarded),
	      (_DW_AT_GNU_pt_guarded, = DW_AT_GNU_pt_guarded),
	      (_DW_AT_GNU_locks_excluded, = DW_AT_GNU_locks_excluded),
	      (_DW_AT_GNU_exclusive_locks_required, = DW_AT_GNU_exclusive_locks_required),
	      (_DW_AT_GNU_shared_locks_required, = DW_AT_GNU_shared_locks_required),
	      (_DW_AT_GNU_odr_signature, = DW_AT_GNU_odr_signature),
	      (_DW_AT_GNU_template_name, = DW_AT_GNU_template_name),
	      (_DW_AT_GNU_call_site_value, = DW_AT_GNU_call_site_value),
	      (_DW_AT_GNU_call_site_data_value, = DW_AT_GNU_call_site_data_value),
	      (_DW_AT_GNU_call_site_target, = DW_AT_GNU_call_site_target),
	      (_DW_AT_GNU_call_site_target_clobbered, = DW_AT_GNU_call_site_target_clobbered),
	      (_DW_AT_GNU_tail_call, = DW_AT_GNU_tail_call),
	      (_DW_AT_GNU_all_tail_call_sites, = DW_AT_GNU_all_tail_call_sites),
	      (_DW_AT_GNU_all_call_sites, = DW_AT_GNU_all_call_sites),
	      (_DW_AT_GNU_all_source_call_sites, = DW_AT_GNU_all_source_call_sites),
	      (_DW_AT_GNU_macros, = DW_AT_GNU_macros),
	      (_DW_AT_GNU_deleted, = DW_AT_GNU_deleted),
	      (_DW_AT_GNU_dwo_name, = DW_AT_GNU_dwo_name),
	      (_DW_AT_GNU_dwo_id, = DW_AT_GNU_dwo_id),
	      (_DW_AT_GNU_ranges_base, = DW_AT_GNU_ranges_base),
	      (_DW_AT_GNU_addr_base, = DW_AT_GNU_addr_base),
	      (_DW_AT_GNU_pubnames, = DW_AT_GNU_pubnames),
	      (_DW_AT_GNU_pubtypes, = DW_AT_GNU_pubtypes),
	      (_DW_AT_GNU_discriminator, = DW_AT_GNU_discriminator),
	      (_DW_AT_GNU_locviews, = DW_AT_GNU_locviews),
	      (_DW_AT_GNU_entry_view, = DW_AT_GNU_entry_view),
	      (_DW_AT_GNU_bias, = DW_AT_GNU_bias),
	      (_DW_AT_SUN_template, = DW_AT_SUN_template),
	      (_DW_AT_VMS_rtnbeg_pd_address, = DW_AT_VMS_rtnbeg_pd_address),
	      (_DW_AT_ALTIUM_loclist, = DW_AT_ALTIUM_loclist),
	      (_DW_AT_use_GNAT_descriptive_type, = DW_AT_use_GNAT_descriptive_type),
	      (_DW_AT_GNAT_descriptive_type, = DW_AT_GNAT_descriptive_type),
	      (_DW_AT_GNU_numerator, = DW_AT_GNU_numerator),
	      (_DW_AT_GNU_denominator, = DW_AT_GNU_denominator),
	      (_DW_AT_go_kind, = DW_AT_go_kind),
	      (_DW_AT_go_key, = DW_AT_go_key),
	      (_DW_AT_go_elem, = DW_AT_go_elem),
	      (_DW_AT_go_embedded_field, = DW_AT_go_embedded_field),
	      (_DW_AT_go_runtime_type, = DW_AT_go_runtime_type),
	      (_DW_AT_upc_threads_scaled, = DW_AT_upc_threads_scaled),
	      (_DW_AT_IBM_wsa_addr, = DW_AT_IBM_wsa_addr),
	      (_DW_AT_IBM_home_location, = DW_AT_IBM_home_location),
	      (_DW_AT_IBM_alt_srcview, = DW_AT_IBM_alt_srcview),
	      (_DW_AT_PGI_lbase, = DW_AT_PGI_lbase),
	      (_DW_AT_PGI_soffset, = DW_AT_PGI_soffset),
	      (_DW_AT_PGI_lstride, = DW_AT_PGI_lstride),
	      (_DW_AT_LLVM_include_path, = DW_AT_LLVM_include_path),
	      (_DW_AT_LLVM_config_macros, = DW_AT_LLVM_config_macros),
	      (_DW_AT_LLVM_sysroot, = DW_AT_LLVM_sysroot),
	      (_DW_AT_LLVM_tag_offset, = DW_AT_LLVM_tag_offset),
	      (_DW_AT_LLVM_apinotes, = DW_AT_LLVM_apinotes),
	      (_DW_AT_LLVM_active_lane, = DW_AT_LLVM_active_lane),
	      (_DW_AT_LLVM_augmentation, = DW_AT_LLVM_augmentation),
	      (_DW_AT_LLVM_lanes, = DW_AT_LLVM_lanes),
	      (_DW_AT_LLVM_lane_pc, = DW_AT_LLVM_lane_pc),
	      (_DW_AT_LLVM_vector_size, = DW_AT_LLVM_vector_size),
	      (_DW_AT_APPLE_optimized, = DW_AT_APPLE_optimized),
	      (_DW_AT_APPLE_flags, = DW_AT_APPLE_flags),
	      (_DW_AT_APPLE_isa, = DW_AT_APPLE_isa),
	      (_DW_AT_APPLE_block, = DW_AT_APPLE_block),
	      (_DW_AT_APPLE_major_runtime_vers, = DW_AT_APPLE_major_runtime_vers),
	      (_DW_AT_APPLE_runtime_class, = DW_AT_APPLE_runtime_class),
	      (_DW_AT_APPLE_omit_frame_ptr, = DW_AT_APPLE_omit_frame_ptr),
	      (_DW_AT_APPLE_property_name, = DW_AT_APPLE_property_name),
	      (_DW_AT_APPLE_property_getter, = DW_AT_APPLE_property_getter),
	      (_DW_AT_APPLE_property_setter, = DW_AT_APPLE_property_setter),
	      (_DW_AT_APPLE_property_attribute, = DW_AT_APPLE_property_attribute),
	      (_DW_AT_APPLE_objc_complete_type, = DW_AT_APPLE_objc_complete_type),
	      (_DW_AT_APPLE_property, = DW_AT_APPLE_property),
	      (_DW_AT_APPLE_objc_direct, = DW_AT_APPLE_objc_direct),
	      (_DW_AT_APPLE_sdk, = DW_AT_APPLE_sdk),
	      (_DW_AT_hi_user, = DW_AT_hi_user),	      
	      )

TYPEDEF_ENUM (mr_dw_form_t,
	      (_DW_FORM_addr, = DW_FORM_addr, "dw_addr"),
	      (_DW_FORM_block2, = DW_FORM_block2),
	      (_DW_FORM_block4, = DW_FORM_block4),
	      (_DW_FORM_data2, = DW_FORM_data2, "dw_unsigned"),
	      (_DW_FORM_data4, = DW_FORM_data4, "dw_unsigned"),
	      (_DW_FORM_data8, = DW_FORM_data8, "dw_unsigned"),
	      (_DW_FORM_string, = DW_FORM_string, "dw_str"),
	      (_DW_FORM_block, = DW_FORM_block),
	      (_DW_FORM_block1, = DW_FORM_block1, "dw_unsigned"),
	      (_DW_FORM_data1, = DW_FORM_data1, "dw_unsigned"),
	      (_DW_FORM_flag, = DW_FORM_flag, "dw_flag"),
	      (_DW_FORM_sdata, = DW_FORM_sdata, "dw_signed"),
	      (_DW_FORM_strp, = DW_FORM_strp, "dw_str"),
	      (_DW_FORM_udata, = DW_FORM_udata, "dw_unsigned"),
	      (_DW_FORM_ref_addr, = DW_FORM_ref_addr, "dw_off"),
	      (_DW_FORM_ref1, = DW_FORM_ref1, "dw_off"),
	      (_DW_FORM_ref2, = DW_FORM_ref2, "dw_off"),
	      (_DW_FORM_ref4, = DW_FORM_ref4, "dw_off"),
	      (_DW_FORM_ref8, = DW_FORM_ref8, "dw_off"),
	      (_DW_FORM_ref_udata, = DW_FORM_ref_udata, "dw_off"),
	      (_DW_FORM_indirect, = DW_FORM_indirect),
	      (_DW_FORM_sec_offset, = DW_FORM_sec_offset, "dw_off"),
	      (_DW_FORM_exprloc, = DW_FORM_exprloc),
	      (_DW_FORM_flag_present, = DW_FORM_flag_present, "dw_flag"),
	      (_DW_FORM_strx, = DW_FORM_strx, "dw_str"),
	      (_DW_FORM_addrx, = DW_FORM_addrx),
	      (_DW_FORM_ref_sup4, = DW_FORM_ref_sup4),
	      (_DW_FORM_strp_sup, = DW_FORM_strp_sup, "dw_str"),
	      (_DW_FORM_data16, = DW_FORM_data16),
	      (_DW_FORM_line_strp, = DW_FORM_line_strp),
	      (_DW_FORM_ref_sig8, = DW_FORM_ref_sig8, "dw_off"),
	      (_DW_FORM_implicit_const, = DW_FORM_implicit_const, "dw_unsigned"),
	      (_DW_FORM_loclistx, = DW_FORM_loclistx),
	      (_DW_FORM_rnglistx, = DW_FORM_rnglistx),
	      (_DW_FORM_ref_sup8, = DW_FORM_ref_sup8),
	      (_DW_FORM_strx1, = DW_FORM_strx1, "dw_str"),
	      (_DW_FORM_strx2, = DW_FORM_strx2, "dw_str"),
	      (_DW_FORM_strx3, = DW_FORM_strx3, "dw_str"),
	      (_DW_FORM_strx4, = DW_FORM_strx4, "dw_str"),
	      (_DW_FORM_addrx1, = DW_FORM_addrx1),
	      (_DW_FORM_addrx2, = DW_FORM_addrx2),
	      (_DW_FORM_addrx3, = DW_FORM_addrx3),
	      (_DW_FORM_addrx4, = DW_FORM_addrx4),
	      (_DW_FORM_GNU_addr_index, = DW_FORM_GNU_addr_index),
	      (_DW_FORM_GNU_str_index, = DW_FORM_GNU_str_index),
	      (_DW_FORM_GNU_ref_alt, = DW_FORM_GNU_ref_alt),
	      (_DW_FORM_GNU_strp_alt, = DW_FORM_GNU_strp_alt),
	      )

#define DW_FORM_STRING (0 MR_FOREACH (MR_ONE_SHIFT, _DW_FORM_string, _DW_FORM_strp, _DW_FORM_strx, _DW_FORM_strp_sup, _DW_FORM_strx1, _DW_FORM_strx2, _DW_FORM_strx3, _DW_FORM_strx4))
#define DW_FORM_UNSIGNED (0 MR_FOREACH (MR_ONE_SHIFT, _DW_FORM_data1, _DW_FORM_data2, _DW_FORM_data4, _DW_FORM_data8, _DW_FORM_udata, _DW_FORM_implicit_const))

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
    case _DW_FORM_strp_sup:
    case _DW_FORM_strx:
    case _DW_FORM_strx1:
    case _DW_FORM_strx2:
    case _DW_FORM_strx3:
    case _DW_FORM_strx4:
      rv = dwarf_formstring (dw_attribute, &mr_attr->dw_str, NULL);
      assert (rv == DW_DLV_OK);
      if (mr_attr->dw_str)
	{
	  mr_attr->dw_str = mr_strdup (mr_attr->dw_str);
	  assert (mr_attr->dw_str != NULL);
	}
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
    case _DW_FORM_implicit_const:
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
	rv = dwarf_siblingof_b (debug, child_die, !0, &sibling_die, NULL);
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
      rv = dwarf_next_cu_header_d (debug, !0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &next_cu_header, NULL, NULL);
      if (rv == DW_DLV_NO_ENTRY)
	break;
      assert (rv == DW_DLV_OK);

      rv = dwarf_siblingof_b (debug, 0, !0, &die, NULL);
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
	  assert (mr_die->attributes[idx].dw_str != NULL);
	  attr = &mr_die->attributes[idx];
	}
      assert ((DW_FORM_STRING >> attr->form) & 1);
      assert (attr->dw_str != NULL);
      fdp->type = mr_strdup (attr->dw_str);
      assert (fdp->type != NULL);
    }
}

static void
push_mr_type (mr_fd_t * fdp, mr_type_t mr_type)
{
  if (fdp->mr_type != MR_TYPE_NONE)
    fprintf (stderr, "name '%s' type '%s' mr_type %d aux %d ptr %d new %d\n", fdp->name.str, fdp->type, fdp->mr_type, fdp->mr_type_aux, fdp->mr_type_ptr, mr_type);
  assert (fdp->mr_type == MR_TYPE_NONE);
  fdp->mr_type = fdp->mr_type_aux;
  fdp->mr_type_aux = fdp->mr_type_ptr;
  fdp->mr_type_ptr = mr_type;
}

static void
get_base_mr_type (mr_fd_t * fdp, mr_die_t * mr_die)
{
  mr_type_sign_t mr_type_sign;
  memset (&mr_type_sign, 0, sizeof (mr_type_sign));

  mr_dw_attribute_t * attr = die_attribute (mr_die, _DW_AT_byte_size);
  assert (attr != NULL);
  assert ((DW_FORM_UNSIGNED >> attr->form) & 1);
  mr_type_sign.size = attr->dw_unsigned;
	
  attr = die_attribute (mr_die, _DW_AT_name);
  assert (attr != NULL);
  assert ((DW_FORM_STRING >> attr->form) & 1);
  mr_type_sign.type.str = attr->dw_str;

  mr_ptr_t * find = mr_ic_find (&mr_type_sign_ic, &mr_type_sign);
  if (NULL == find)
    fprintf (stderr, "Type descriptor for '%s' is not found\n", mr_type_sign.type.str);
  assert (find != NULL);

  mr_type_sign_t * found_sign = find->ptr;
  push_mr_type (fdp, found_sign->mr_type);
  fdp->size = mr_type_sign.size;
  if (fdp->type == NULL)
    fdp->type = mr_strdup (mr_type_sign.type.str);
  assert (fdp->type != NULL);
}

static void
get_array_mr_type (mr_fd_t * fdp, mr_die_t * mr_die)
{
  unsigned count = 1, dimension = 1;
  int i;
  for (i = mr_die->children_size / sizeof (mr_die->children[0]) - 1; i >= 0; --i)
    {
      assert (mr_die->children[i].tag == _DW_TAG_subrange_type);
      count *= dimension;
      dimension = 0;
      mr_dw_attribute_t * attr = die_attribute (&mr_die->children[i], _DW_AT_count);
      if (attr != NULL)
	{
	  assert ((DW_FORM_UNSIGNED >> attr->form) & 1);
	  dimension = attr->dw_unsigned;
	}

      attr = die_attribute (&mr_die->children[i], _DW_AT_upper_bound);
      if (attr != NULL)
	{
	  assert ((DW_FORM_UNSIGNED >> attr->form) & 1);
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
	case _DW_TAG_const_type:
	case _DW_TAG_volatile_type:
	  continue;

	case _DW_TAG_array_type:
	  push_mr_type (fdp, MR_TYPE_ARRAY);
	  get_array_mr_type (fdp, mr_die);
	  continue;
	
	case _DW_TAG_pointer_type:
	  push_mr_type (fdp, MR_TYPE_POINTER);
	  continue;
	
	case _DW_TAG_typedef:
	  get_type_name (fdp, mr_die, die_off_ic);
	  continue;

	case _DW_TAG_base_type:
	  get_base_mr_type (fdp, mr_die);
	  break;

	case _DW_TAG_subroutine_type:
	  assert (fdp->mr_type_ptr == MR_TYPE_POINTER);
	  fdp->mr_type_ptr = MR_TYPE_FUNC_TYPE;
	  break;

	case _DW_TAG_structure_type:
	  push_mr_type (fdp, MR_TYPE_STRUCT);
	  get_type_name (fdp, mr_die, die_off_ic);
	  break;
	
	case _DW_TAG_union_type:
	  push_mr_type (fdp, MR_TYPE_UNION);
	  get_type_name (fdp, mr_die, die_off_ic);
	  break;
	
	case _DW_TAG_enumeration_type:
	  push_mr_type (fdp, MR_TYPE_ENUM);
	  get_type_name (fdp, mr_die, die_off_ic);
	  break;
	
	default:
	  assert (false);
	}
      break;
    }

  if (fdp->mr_type_ptr != MR_TYPE_NONE)
    while (fdp->mr_type == MR_TYPE_NONE)
      {
	fdp->mr_type = fdp->mr_type_aux;
	fdp->mr_type_aux = fdp->mr_type_ptr;
	fdp->mr_type_ptr = MR_TYPE_NONE;
      }
}

static void
load_enumerator (mr_fd_t * fdp, mr_die_t * mr_die, mr_ic_t * die_off_ic)
{
  memset (fdp, 0, sizeof (*fdp));
  fdp->mr_type = MR_TYPE_ENUM;
  
  mr_dw_attribute_t * attr = die_attribute (mr_die, _DW_AT_name);
  assert (attr != NULL);
  assert ((DW_FORM_STRING >> attr->form) & 1);
  assert (attr->dw_str != NULL);
  fdp->name.str = mr_strdup (attr->dw_str);
  assert (fdp->name.str != NULL);

  attr = die_attribute (mr_die, _DW_AT_const_value);
  assert (attr != NULL);
  assert (((DW_FORM_UNSIGNED >> attr->form) & 1) || (_DW_FORM_sdata == attr->form));

  if (_DW_FORM_sdata == attr->form)
    {
      fdp->param.enum_param._signed = attr->dw_signed;
      fdp->mr_type_aux = MR_TYPE_INT64;
    }
  else
    {
      fdp->param.enum_param._unsigned = attr->dw_unsigned;
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
      assert ((DW_FORM_STRING >> attr->form) & 1);
      assert (attr->dw_str != NULL);
      fdp->name.str = mr_strdup (attr->dw_str);
      assert (fdp->name.str != NULL);
    }
  if (fdp->name.str == NULL)
    fdp->unnamed = true;

  attr = die_attribute (mr_die, _DW_AT_byte_size);
  if (attr != NULL)
    {
      assert ((DW_FORM_UNSIGNED >> attr->form) & 1);
      fdp->size = attr->dw_unsigned;
    }
  
  attr = die_attribute (mr_die, _DW_AT_data_member_location);
  if (attr != NULL)
    {
      assert (((DW_FORM_UNSIGNED >> attr->form) & 1) || (_DW_FORM_block1 == attr->form));
      fdp->offset = attr->dw_unsigned;
    }

  attr = die_attribute (mr_die, _DW_AT_bit_size);
  if (attr != NULL)
    {
      assert ((DW_FORM_UNSIGNED >> attr->form) & 1);
      fdp->mr_type_ptr = MR_TYPE_BITFIELD;
      fdp->param.bitfield_param.width = attr->dw_unsigned;

      attr = die_attribute (mr_die, _DW_AT_data_bit_offset);
      if (attr != NULL)
	{
	  assert ((DW_FORM_UNSIGNED >> attr->form) & 1);
	  fdp->param.bitfield_param.shift = attr->dw_unsigned % __CHAR_BIT__;
	  fdp->offset = attr->dw_unsigned / __CHAR_BIT__;
	}

      attr = die_attribute (mr_die, _DW_AT_bit_offset);
      if (attr != NULL)
	{
	  assert ((DW_FORM_UNSIGNED >> attr->form) & 1);	  
	  fdp->param.bitfield_param.shift = fdp->size * __CHAR_BIT__ - fdp->param.bitfield_param.width - attr->dw_unsigned;
	  fdp->offset += fdp->param.bitfield_param.shift / __CHAR_BIT__;
	  fdp->param.bitfield_param.shift %= __CHAR_BIT__;
	}
    }

  get_mr_type (fdp, mr_die, die_off_ic);
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

  assert ((DW_FORM_STRING >> attr->form) & 1);
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
	mr_fd_t fd;
	memset (&fd, 0, sizeof (fd));
	fd.mr_type_ptr = MR_TYPE_POINTER;
	get_mr_type (&fd, mr_die, die_off_ic);
	MR_FREE_RECURSIVELY (mr_fd_t, &fd);
	
	if (MR_TYPE_CHAR == fd.mr_type)
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
  assert (tdp->type.str != NULL);
  if ((MR_TYPE_STRING == mr_type) || (MR_TYPE_FUNC_TYPE == mr_type))
    tdp->size = sizeof (void *);

  find = mr_ic_add (td_ic, tdp);
  assert (find != NULL);

  attr = die_attribute (mr_die, _DW_AT_byte_size);
  if (attr != NULL)
    {
      assert ((DW_FORM_UNSIGNED >> attr->form) & 1);	  
      tdp->size = attr->dw_unsigned;
    }

  ssize_t alloc_size = 0;
  if (load_child != NULL)
    {
      int i, count = mr_die->children_size / sizeof (mr_die->children[0]);
      for (i = 0; i < count; ++i)
	if (mr_die->children[i].tag == children_tag)
	  {
	    mr_fd_t ** fdp_ptr = mr_rarray_allocate_element ((void*)&tdp->fields, &tdp->fields_size, &alloc_size, sizeof (tdp->fields[0]));
	    assert (fdp_ptr != NULL);
	    *fdp_ptr = MR_CALLOC (1, sizeof (**fdp_ptr));
	    assert (*fdp_ptr != NULL);

	    load_child (*fdp_ptr, &mr_die->children[i], die_off_ic);
	  }
    }
  
  mr_fd_t ** fdp_ptr = mr_rarray_allocate_element ((void*)&tdp->fields, &tdp->fields_size, &alloc_size, sizeof (tdp->fields[0]));
  assert (fdp_ptr != NULL);
  *fdp_ptr = MR_CALLOC (1, sizeof (**fdp_ptr));
  assert (*fdp_ptr != NULL);
  (*fdp_ptr)->mr_type = MR_TYPE_LAST;
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

#define MR_BI_SIGN_(TYPE, TYPE_STR) (mr_type_sign_t[]){{ .type = { .str = TYPE_STR, .hash_value = 0, }, .size = sizeof (TYPE), .mr_type = MR_TYPE_DETECT (TYPE), }},
#define MR_BI_SIGN(TYPE, ...) MR_IF_ELSE (MR_IS_EMPTY (__VA_ARGS__)) (MR_BI_SIGN_ (TYPE, #TYPE)) (MR_BI_SIGN_ (TYPE, __VA_ARGS__)) 

static mr_type_sign_t * mr_type_sign[] =
{
  MR_BI_SIGN (typeof (sizeof (0)), "__ARRAY_SIZE_TYPE__")
  MR_BI_SIGN (complex float, "complex")
  MR_BI_SIGN (complex double, "complex")
  MR_BI_SIGN (complex long double, "complex")
  MR_FOREACH (MR_BI_SIGN, MR_BUILTIN_TYPES)
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
  status = mr_ic_index (&die_off_ic, ra_die_ptr.ra, ra_die_ptr.size);
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
		      (tdp->fields[0]->mr_type_aux == MR_TYPE_INT64));
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
	mr_fd_t * fdp = tdp->fields[i];

	switch (fdp->mr_type)
	  {
	  case MR_TYPE_ARRAY:
	    if (fdp->mr_type_aux == MR_TYPE_POINTER)
	      {
		fdp->param.array_param.pointer_param = MR_CALLOC (1, sizeof (*fdp->param.array_param.pointer_param));
		assert (fdp->param.array_param.pointer_param != NULL);
	      }

	    if ((fdp->mr_type_aux == MR_TYPE_POINTER) || (fdp->mr_type_aux == MR_TYPE_FUNC_TYPE))
	      fdp->size = sizeof (void*);

	    __attribute__ ((fallthrough));

	  case MR_TYPE_UNION:
	  case MR_TYPE_ENUM:
	  case MR_TYPE_STRUCT:
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

	  case MR_TYPE_POINTER:
	    fdp->size = sizeof (void *);
	    if (fdp->mr_type_aux == MR_TYPE_NONE)
	      fdp->mr_type_aux = MR_TYPE_VOID;
	    else if (fdp->mr_type_aux == MR_TYPE_CHAR)
	      {
		fdp->mr_type = MR_TYPE_STRING;
		assert (fdp->type != NULL);
		int length = strlen (fdp->type);
		fdp->type = MR_REALLOC (fdp->type, length + 2 * sizeof (fdp->type[0]));
		assert (fdp->type != NULL);
		fdp->type[length] = '*';
		fdp->type[length + 1] = 0;
	      }
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

		if (fdp->mr_type_aux == MR_TYPE_POINTER)
		  {
		    fdp->param.pointer_param.pointer_param = MR_CALLOC (1, sizeof (*fdp->param.pointer_param.pointer_param));
		    assert (fdp->param.pointer_param.pointer_param != NULL);
		  }
	      }
	    break;

	  default:
	    break;
	  }

	if (MR_TYPE_ARRAY == fdp->mr_type)
	  {
	    fdp->size *= fdp->param.array_param.count;
	    if (MR_TYPE_CHAR == fdp->mr_type_aux)
	      fdp->mr_type = MR_TYPE_CHAR_ARRAY;
	  }

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
  MR_PRINT ((mr_td_t, tdp), ",\n");
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
  mr_type_void_fields ("mr_td_t", "field_by_name", "meta", "res", "res_type", "mr_size", "is_dynamically_allocated");
  mr_type_void_fields ("mr_fd_t", "tdp", "self_ptr", "readonly", "mr_size", "non_persistent", "mr_type_class");
  mr_type_void_fields ("mr_enum_param_t", "is_bitmask");
  mr_type_void_fields ("mr_bitfield_param_t", "bitfield", "size", "initialized");
  mr_type_void_fields ("mr_hashed_string_t", "hash_value");
}

static void
free_die (mr_die_t * mr_die)
{
  int i;

  for (i = 0; i < mr_die->attributes_size / sizeof (mr_die->attributes[0]); ++i)
    if ((DW_FORM_STRING >> mr_die->attributes[i].form) & 1)
      {
	if (mr_die->attributes[i].dw_str)
	  MR_FREE (mr_die->attributes[i].dw_str);
      }
  
  if (mr_die->attributes)
    MR_FREE (mr_die->attributes);

  for (i = 0; i < mr_die->children_size / sizeof (mr_die->children[0]); ++i)
    free_die (&mr_die->children[i]);
  
  if (mr_die->children)
    MR_FREE (mr_die->children);
}

int
main (int argc, char * argv [])
{
  if (argc < 2)
    {
      fprintf (stderr, "Usage: %s filename\n", argv[0]);
      return (EXIT_FAILURE);
    }
  
  mr_status_t status = mr_ic_new (&mr_type_sign_ic, mr_type_sign_hash, mr_type_sign_cmp, "mr_type_sign_t", MR_IC_HASH, NULL);
  assert (status == MR_SUCCESS);
  status = mr_ic_index (&mr_type_sign_ic, (mr_ptr_t*)mr_type_sign, sizeof (mr_type_sign));
  assert (status == MR_SUCCESS);

  mr_ic_t td_ic;
  status = mr_ic_new (&td_ic, mr_td_name_get_hash, mr_td_name_cmp, "mr_td_t", MR_IC_HASH, NULL);
  assert (status == MR_SUCCESS);

  int i;
  for (i = 1; i < argc; ++i)
    {
      Dwarf_Debug debug;
      Dwarf_Error error;
      char path[1 << 13];
      int rv = dwarf_init_path_dl (argv[i], path, sizeof (path), DW_GROUPNUMBER_ANY, NULL, NULL, &debug, NULL, 0, NULL, &error);
      if (rv != DW_DLV_OK)
	{
	  fprintf (stderr, "libdwarf error (%d): failed to open file '%s'\n", rv, argv[i]);
	  return (EXIT_FAILURE);
	}

      mr_die_t mr_die;
      memset (&mr_die, 0, sizeof (mr_die));
      dump_cu_list (debug, &mr_die);

      extract_type_descriptors (&td_ic, &mr_die);
      free_die (&mr_die);

      rv = dwarf_finish (debug);
      assert (rv == DW_DLV_OK);
    }

  tweak_mr_conf ();
  mr_ic_foreach (&td_ic, process_td, &td_ic);
  mr_ic_foreach (&td_ic, print_td, NULL);
  mr_ic_foreach (&td_ic, free_td, NULL);
  mr_ic_free (&td_ic);

  mr_ic_free (&mr_type_sign_ic);

  return (EXIT_SUCCESS);
}
