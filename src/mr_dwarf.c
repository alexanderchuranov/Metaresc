#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <libelf.h>
#include <libdwarf/dwarf.h>
#include <libdwarf/libdwarf.h>

#include <metaresc.h>

/* grep "#define DW_TAG_" /usr/include/libdwarf/dwarf.h | awk '{print "(_"$2", = "$2"),"}' */
TYPEDEF_ENUM (mr_dw_tag_t, ATTRIBUTES ( , "grep '#define DW_TAG_' /usr/include/libdwarf/dwarf.h | awk '{print \"(_\"$2\", = \"$2\"),\"}'"),
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
	      (_DW_TAG_ALTIUM_circ_type, = DW_TAG_ALTIUM_circ_type),
	      (_DW_TAG_ALTIUM_mwa_circ_type, = DW_TAG_ALTIUM_mwa_circ_type),
	      (_DW_TAG_ALTIUM_rev_carry_type, = DW_TAG_ALTIUM_rev_carry_type),
	      (_DW_TAG_ALTIUM_rom, = DW_TAG_ALTIUM_rom),
	      (_DW_TAG_upc_shared_type, = DW_TAG_upc_shared_type),
	      (_DW_TAG_upc_strict_type, = DW_TAG_upc_strict_type),
	      (_DW_TAG_upc_relaxed_type, = DW_TAG_upc_relaxed_type),
	      (_DW_TAG_PGI_kanji_type, = DW_TAG_PGI_kanji_type),
	      (_DW_TAG_PGI_interface_block, = DW_TAG_PGI_interface_block),
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
	      (_DW_AT_HP_block_index, = DW_AT_HP_block_index),
	      (_DW_AT_lo_user, = DW_AT_lo_user),
	      (_DW_AT_MIPS_fde, = DW_AT_MIPS_fde),
	      (_DW_AT_MIPS_loop_begin, = DW_AT_MIPS_loop_begin),
	      (_DW_AT_MIPS_tail_loop_begin, = DW_AT_MIPS_tail_loop_begin),
	      (_DW_AT_MIPS_epilog_begin, = DW_AT_MIPS_epilog_begin),
	      (_DW_AT_MIPS_loop_unroll_factor, = DW_AT_MIPS_loop_unroll_factor),
	      (_DW_AT_MIPS_software_pipeline_depth, = DW_AT_MIPS_software_pipeline_depth),
	      (_DW_AT_MIPS_linkage_name, = DW_AT_MIPS_linkage_name),
	      (_DW_AT_MIPS_stride, = DW_AT_MIPS_stride),
	      (_DW_AT_MIPS_abstract_name, = DW_AT_MIPS_abstract_name),
	      (_DW_AT_MIPS_clone_origin, = DW_AT_MIPS_clone_origin),
	      (_DW_AT_MIPS_has_inlines, = DW_AT_MIPS_has_inlines),
	      (_DW_AT_MIPS_stride_byte, = DW_AT_MIPS_stride_byte),
	      (_DW_AT_MIPS_stride_elem, = DW_AT_MIPS_stride_elem),
	      (_DW_AT_MIPS_ptr_dopetype, = DW_AT_MIPS_ptr_dopetype),
	      (_DW_AT_MIPS_allocatable_dopetype, = DW_AT_MIPS_allocatable_dopetype),
	      (_DW_AT_MIPS_assumed_shape_dopetype, = DW_AT_MIPS_assumed_shape_dopetype),
	      (_DW_AT_MIPS_assumed_size, = DW_AT_MIPS_assumed_size),
	      (_DW_AT_HP_unmodifiable, = DW_AT_HP_unmodifiable),
	      (_DW_AT_HP_actuals_stmt_list, = DW_AT_HP_actuals_stmt_list),
	      (_DW_AT_HP_proc_per_section, = DW_AT_HP_proc_per_section),
	      (_DW_AT_HP_raw_data_ptr, = DW_AT_HP_raw_data_ptr),
	      (_DW_AT_HP_pass_by_reference, = DW_AT_HP_pass_by_reference),
	      (_DW_AT_HP_opt_level, = DW_AT_HP_opt_level),
	      (_DW_AT_HP_prof_version_id, = DW_AT_HP_prof_version_id),
	      (_DW_AT_HP_opt_flags, = DW_AT_HP_opt_flags),
	      (_DW_AT_HP_cold_region_low_pc, = DW_AT_HP_cold_region_low_pc),
	      (_DW_AT_HP_cold_region_high_pc, = DW_AT_HP_cold_region_high_pc),
	      (_DW_AT_HP_all_variables_modifiable, = DW_AT_HP_all_variables_modifiable),
	      (_DW_AT_HP_linkage_name, = DW_AT_HP_linkage_name),
	      (_DW_AT_HP_prof_flags, = DW_AT_HP_prof_flags),
	      (_DW_AT_CPQ_discontig_ranges, = DW_AT_CPQ_discontig_ranges),
	      (_DW_AT_CPQ_semantic_events, = DW_AT_CPQ_semantic_events),
	      (_DW_AT_CPQ_split_lifetimes_var, = DW_AT_CPQ_split_lifetimes_var),
	      (_DW_AT_CPQ_split_lifetimes_rtn, = DW_AT_CPQ_split_lifetimes_rtn),
	      (_DW_AT_CPQ_prologue_length, = DW_AT_CPQ_prologue_length),
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
	      (_DW_AT_ALTIUM_loclist, = DW_AT_ALTIUM_loclist),
	      (_DW_AT_SUN_template, = DW_AT_SUN_template),
	      (_DW_AT_VMS_rtnbeg_pd_address, = DW_AT_VMS_rtnbeg_pd_address),
	      (_DW_AT_SUN_alignment, = DW_AT_SUN_alignment),
	      (_DW_AT_SUN_vtable, = DW_AT_SUN_vtable),
	      (_DW_AT_SUN_count_guarantee, = DW_AT_SUN_count_guarantee),
	      (_DW_AT_SUN_command_line, = DW_AT_SUN_command_line),
	      (_DW_AT_SUN_vbase, = DW_AT_SUN_vbase),
	      (_DW_AT_SUN_compile_options, = DW_AT_SUN_compile_options),
	      (_DW_AT_SUN_language, = DW_AT_SUN_language),
	      (_DW_AT_SUN_browser_file, = DW_AT_SUN_browser_file),
	      (_DW_AT_SUN_vtable_abi, = DW_AT_SUN_vtable_abi),
	      (_DW_AT_SUN_func_offsets, = DW_AT_SUN_func_offsets),
	      (_DW_AT_SUN_cf_kind, = DW_AT_SUN_cf_kind),
	      (_DW_AT_SUN_vtable_index, = DW_AT_SUN_vtable_index),
	      (_DW_AT_SUN_omp_tpriv_addr, = DW_AT_SUN_omp_tpriv_addr),
	      (_DW_AT_SUN_omp_child_func, = DW_AT_SUN_omp_child_func),
	      (_DW_AT_SUN_func_offset, = DW_AT_SUN_func_offset),
	      (_DW_AT_SUN_memop_type_ref, = DW_AT_SUN_memop_type_ref),
	      (_DW_AT_SUN_profile_id, = DW_AT_SUN_profile_id),
	      (_DW_AT_SUN_memop_signature, = DW_AT_SUN_memop_signature),
	      (_DW_AT_SUN_obj_dir, = DW_AT_SUN_obj_dir),
	      (_DW_AT_SUN_obj_file, = DW_AT_SUN_obj_file),
	      (_DW_AT_SUN_original_name, = DW_AT_SUN_original_name),
	      (_DW_AT_SUN_hwcprof_signature, = DW_AT_SUN_hwcprof_signature),
	      (_DW_AT_SUN_amd64_parmdump, = DW_AT_SUN_amd64_parmdump),
	      (_DW_AT_SUN_part_link_name, = DW_AT_SUN_part_link_name),
	      (_DW_AT_SUN_link_name, = DW_AT_SUN_link_name),
	      (_DW_AT_SUN_pass_with_const, = DW_AT_SUN_pass_with_const),
	      (_DW_AT_SUN_return_with_const, = DW_AT_SUN_return_with_const),
	      (_DW_AT_SUN_import_by_name, = DW_AT_SUN_import_by_name),
	      (_DW_AT_SUN_f90_pointer, = DW_AT_SUN_f90_pointer),
	      (_DW_AT_SUN_pass_by_ref, = DW_AT_SUN_pass_by_ref),
	      (_DW_AT_SUN_f90_allocatable, = DW_AT_SUN_f90_allocatable),
	      (_DW_AT_SUN_f90_assumed_shape_array, = DW_AT_SUN_f90_assumed_shape_array),
	      (_DW_AT_SUN_c_vla, = DW_AT_SUN_c_vla),
	      (_DW_AT_SUN_return_value_ptr, = DW_AT_SUN_return_value_ptr),
	      (_DW_AT_SUN_dtor_start, = DW_AT_SUN_dtor_start),
	      (_DW_AT_SUN_dtor_length, = DW_AT_SUN_dtor_length),
	      (_DW_AT_SUN_dtor_state_initial, = DW_AT_SUN_dtor_state_initial),
	      (_DW_AT_SUN_dtor_state_final, = DW_AT_SUN_dtor_state_final),
	      (_DW_AT_SUN_dtor_state_deltas, = DW_AT_SUN_dtor_state_deltas),
	      (_DW_AT_SUN_import_by_lname, = DW_AT_SUN_import_by_lname),
	      (_DW_AT_SUN_f90_use_only, = DW_AT_SUN_f90_use_only),
	      (_DW_AT_SUN_namelist_spec, = DW_AT_SUN_namelist_spec),
	      (_DW_AT_SUN_is_omp_child_func, = DW_AT_SUN_is_omp_child_func),
	      (_DW_AT_SUN_fortran_main_alias, = DW_AT_SUN_fortran_main_alias),
	      (_DW_AT_SUN_fortran_based, = DW_AT_SUN_fortran_based),
	      (_DW_AT_use_GNAT_descriptive_type, = DW_AT_use_GNAT_descriptive_type),
	      (_DW_AT_GNAT_descriptive_type, = DW_AT_GNAT_descriptive_type),
	      (_DW_AT_upc_threads_scaled, = DW_AT_upc_threads_scaled),
	      (_DW_AT_PGI_lbase, = DW_AT_PGI_lbase),
	      (_DW_AT_PGI_soffset, = DW_AT_PGI_soffset),
	      (_DW_AT_PGI_lstride, = DW_AT_PGI_lstride),
	      (_DW_AT_APPLE_optimized, = DW_AT_APPLE_optimized),
	      (_DW_AT_APPLE_flags, = DW_AT_APPLE_flags),
	      (_DW_AT_APPLE_isa, = DW_AT_APPLE_isa),
	      (_DW_AT_APPLE_block, = DW_AT_APPLE_block),
	      (_DW_AT_APPLE_major_runtime_vers, = DW_AT_APPLE_major_runtime_vers),
	      (_DW_AT_APPLE_runtime_class, = DW_AT_APPLE_runtime_class),
	      (_DW_AT_APPLE_omit_frame_ptr, = DW_AT_APPLE_omit_frame_ptr),
	      (_DW_AT_APPLE_closure, = DW_AT_APPLE_closure),
	      (_DW_AT_hi_user, = DW_AT_hi_user),
	      )

TYPEDEF_ENUM (mr_dw_form_t,
	      (_DW_FORM_addr, = DW_FORM_addr, "dw_addr"),
	      (_DW_FORM_block, = DW_FORM_block),
	      (_DW_FORM_block1, = DW_FORM_block1),
	      (_DW_FORM_block2, = DW_FORM_block2),
	      (_DW_FORM_block4, = DW_FORM_block4),
	      (_DW_FORM_data1, = DW_FORM_data1, "dw_unsigned"),
	      (_DW_FORM_data2, = DW_FORM_data2, "dw_unsigned"),
	      (_DW_FORM_data4, = DW_FORM_data4, "dw_unsigned"),
	      (_DW_FORM_data8, = DW_FORM_data8, "dw_unsigned"),
	      (_DW_FORM_string, = DW_FORM_string, "dw_str"),
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
	      (_DW_FORM_ref_sig8, = DW_FORM_ref_sig8, "dw_off"),
	      )	      

TYPEDEF_STRUCT (mr_dw_attribute_t,
		(mr_dw_attribute_code_t, code),
		(mr_dw_form_t, form),
		ANON_UNION (),
		VOID (bool, dw_default),
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
		(mr_dw_attribute_t *, attributes, , "", { "attributes_size" }, "char"),
		VOID (ssize_t, attributes_size),
		(mr_die_t *, children, , "", { "children_size" }, "char"),
		VOID (ssize_t, children_size),
		VOID (ssize_t, children_alloc_size),
		)

static void
dump_attribute (Dwarf_Attribute dw_attribute, mr_dw_attribute_t * mr_attr)
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
	dump_attribute (dw_attributes[i], &mr_die->attributes[i]);
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
      rv = dwarf_next_cu_header (debug, NULL, NULL, NULL, NULL,
				  &next_cu_header, NULL);
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

int
main (int argc, char * argv [])
{
  if (argc < 2)
    {
      fprintf (stderr, "Usage: %s filename\n", argv[0]);
      return (EXIT_FAILURE);
    }
  
  FILE * fp = fopen (argv[1], "r");
  if (fp == NULL)
    {
      perror (argv[1]);
      return (EXIT_FAILURE);
    }
  
  Dwarf_Debug debug;
  int rv = dwarf_init (fileno (fp), DW_DLC_READ, NULL, NULL, &debug, NULL);
  assert (rv == DW_DLV_OK);

  mr_die_t mr_die;
  memset (&mr_die, 0, sizeof (mr_die));
  dump_cu_list (debug, &mr_die);

  char * dump = MR_SAVE_CINIT (mr_die_t, &mr_die);
  if (dump)
    {
      printf ("%s", dump);
      MR_FREE (dump);
    }

  MR_FREE_RECURSIVELY (mr_die_t, &mr_die);
  
  rv = dwarf_finish (debug, NULL);
  assert (rv == DW_DLV_OK);
  fclose (fp);
  
  return (EXIT_SUCCESS);
}
