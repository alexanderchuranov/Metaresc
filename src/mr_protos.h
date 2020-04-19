/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <metaresc.h>

TYPEDEF_UNION (mr_ptr_t, ATTRIBUTES (__attribute__((transparent_union)), "pointer on any type"),
	       (void *, ptr, , , { MR_SIZE_STR }, "char", .unnamed = true),
	       (void *, MR_OPAQUE_DATA, , , { MR_SIZE_STR }, "char"),
	       (char *, string),
	       (mr_offset_t, offset),
	       (uintptr_t, uintptr_t),
	       (intptr_t, intptr_t),
	       )

TYPEDEF_ENUM (mr_log_level_t, ATTRIBUTES ( ,"Log levels enum"),
	      (MR_LL_ALL, = 0),
	      MR_LL_TRACE,
	      MR_LL_DEBUG,
	      MR_LL_INFO,
	      MR_LL_WARN,
	      MR_LL_ERROR,
	      MR_LL_FATAL,
	      MR_LL_OFF,
	      )

TYPEDEF_ENUM (mr_message_id_t, ATTRIBUTES ( , "Messages enum. Message string saved in .meta field"),
	      (MR_MESSAGE_OUT_OF_MEMORY, , "Out of memory."),
	      (MR_MESSAGE_UNEXPECTED_END, , "Unexpected end of file or end of string."),
	      (MR_MESSAGE_UNEXPECTED_DATA, , "Unexpected data in stream."),
	      (MR_MESSAGE_UNBALANCED_TAGS, , "Unbalanced tags."),
	      (MR_MESSAGE_XDR_WRONG_ENCODING_MODE, , "XDR wrong encoding mode."),
	      (MR_MESSAGE_XDR_SAVE_FAILED, , "XDR save to resizable array failed."),
	      (MR_MESSAGE_XML_SAVE_FAILED, , "XML save failed."),
	      (MR_MESSAGE_LOAD_STRUCT_FAILED, , "Structure load failed."),
	      (MR_MESSAGE_LOAD_METHOD_MISSED, , "Load method missed."),
	      (MR_MESSAGE_XDR_LOAD_FAILED, , "XDR load failed."),
	      (MR_MESSAGE_NO_TYPE_DESCRIPTOR, , "Can't find type descriptor for '%s'."),
	      (MR_MESSAGE_TYPE_NOT_STRUCT, , "Type '%s' is not a struct."),
	      (MR_MESSAGE_TYPE_NOT_UNION, , "Type '%s' is not a union."),
	      (MR_MESSAGE_TYPE_NOT_ENUM, , "Type '%s' is not a enum."),
	      (MR_MESSAGE_NULL_POINTER, , "Can't save/load to/from NULL pointer."),
	      (MR_MESSAGE_IDS_COLLISION, , "Two XML nodes have the same %s = %" SCNd32 "."),
	      (MR_MESSAGE_UNDEFINED_REF_IDX, , "Reference on a node with %s = %" SCNd32 " could not be found."),
	      (MR_MESSAGE_UNKNOWN_ENUM, , "Unknown enum value '%s'."),
	      (MR_MESSAGE_READ_INT, , "Can't read integer value from string `%s`."),
	      (MR_MESSAGE_READ_LONG_DOUBLE, , "Can't read long double from string `%s`."),
	      (MR_MESSAGE_READ_COMPLEX_LONG_DOUBLE, , "Can't read complex long double from string `%s`."),
	      (MR_MESSAGE_READ_CHAR, , "Can't read char from string `%s`."),
	      (MR_MESSAGE_READ_FUNC, , "Can't read function address from string `%s`."),
	      (MR_MESSAGE_STRING_TRUNCATED, , "Char array range check: string truncated."),
	      (MR_MESSAGE_UNKNOWN_SUBNODE, , "Struct type '%s': unknown subnode '%s'."),
	      (MR_MESSAGE_SAVE_IDX_RANGE_CHECK, , "Save node index is out of range."),
	      (MR_MESSAGE_NODE_NAME_MISSMATCH, , "Node name mismatch: expected '%s' got '%s'."),
	      (MR_MESSAGE_NODE_TYPE_MISSMATCH, , "Node '%s' type mismatch: expected '%s' got '%s'."),
	      (MR_MESSAGE_UNSUPPORTED_NODE_TYPE, , "Unsupported node type: %s(%d)."),
	      (MR_MESSAGE_RANGE_CHECK, , "Range check error in array '%s'."),
	      (MR_MESSAGE_READ_REF, , "Can't read '%s' as index."),
	      (MR_MESSAGE_SAVE_ENUM, , "Can't find enum name for value %" SCNd64 " type '%s' field '%s'."),
	      (MR_MESSAGE_NOT_BITMASK, , "Type '%s' of field '%s' is not a bit mask."),
	      (MR_MESSAGE_SAVE_BITMASK, , "Can't decompose on bitmask %" SCNu64 "."),
	      (MR_MESSAGE_DUPLICATED_ENUMS, , "Duplicated enum definition for `%s` in enum '%s'."),
	      (MR_MESSAGE_INT_OF_UNKNOWN_SIZE, , "Failed to stringify integer of unknown size: %" SCNd32 "."),
	      (MR_MESSAGE_PARSE_ERROR, , "Parser error: '%s'. Position: %d:%d-%d:%d."),
	      (MR_MESSAGE_ANON_UNION_TYPE_ERROR, , "Can't create type for anonymous union in type: '%s'."),
	      (MR_MESSAGE_UNKNOWN_XML_ESC, , "Unknown XML escape sequence '%s'."),
	      (MR_MESSAGE_WRONG_XML_ESC, , "Wrong XML escape sequence '%s'."),
	      (MR_MESSAGE_UNION_DISCRIMINATOR_ERROR, , "Union failed to find field by discriminator '%s'."),
	      (MR_MESSAGE_XML_STRING_ENCODING_FAILED, , "XML2 encoding failed for string '%s'."),
	      (MR_MESSAGE_POINTER_NODE_CHILD_MISSING, , "Pointer node child missing: type '%s' name '%s'."),
	      (MR_MESSAGE_UNEXPECTED_NULL_POINTER, , "Unexpected NULL pointer."),
	      (MR_MESSAGE_UNEXPECTED_STRING_SAVE_DATA, , "Unexpected data for string save."),
	      (MR_MESSAGE_WRONG_RESULT_TYPE, , "Wrong result type."),
	      (MR_MESSAGE_DIVISION_BY_ZERO, , "Division by zero."),
	      (MR_MESSAGE_UNEXPECTED_TARGET_TYPE, , "Unexpected target type (%d)."),
	      (MR_MESSAGE_UNEXPECTED_DATA_AT_THE_END, , "Unexpected data at the end of the string '%s'."),
	      (MR_MESSAGE_SERIALIZATION_FAILED, , "Serialization failed."),
	      (MR_MESSAGE_UNEXPECTED_IC_TYPE, , "Unexpected indexed collection type."),
	      (MR_MESSAGE_UNEXPECTED_HASH_TABLE_ERROR, , "Unexpected error in hash table."),
	      (MR_MESSAGE_UNEXPECTED_CHARS_AFTER_CLOSING_TAG, , "Unexpected charecters after closing tag."),
	      (MR_MESSAGE_TAGS_DONT_MATCH, , "Open and close tags names do not match."),
	      (MR_MESSAGE_CANT_READ_PROPERTY, , "Can't read %s property."),
	      (MR_MESSAGE_WRONG_SIZE_FOR_DYNAMIC_ARRAY, , "Wrong size (%zd) for dynamics array."),
	      (MR_MESSAGE_UNEXPECTED_MR_TYPE, , "Unexpected mr_type for serialized node."),
	      (MR_MESSAGE_UNEXPECTED_NUMBER_OF_ITEMS, , "Unexpected number of items in static array collection %d."),
	      (MR_MESSAGE_LAST, , "Last message ID."),
	      )

TYPEDEF_ENUM (mr_type_t, ATTRIBUTES ( , "Metaresc types"),
	      (MR_TYPE_NONE, = 0),
	      MR_TYPE_STRING,
	      MR_TYPE_CHAR_ARRAY,
	      MR_TYPE_CHAR,
	      MR_TYPE_VOID,
	      MR_TYPE_BOOL,
	      MR_TYPE_INT8,
	      MR_TYPE_UINT8,
	      MR_TYPE_INT16,
	      MR_TYPE_UINT16,
	      MR_TYPE_INT32,
	      MR_TYPE_UINT32,
	      MR_TYPE_INT64,
	      MR_TYPE_UINT64,
	      MR_TYPE_FLOAT,
	      MR_TYPE_COMPLEX_FLOAT,
	      MR_TYPE_DOUBLE,
	      MR_TYPE_COMPLEX_DOUBLE,
	      MR_TYPE_LONG_DOUBLE,
	      MR_TYPE_COMPLEX_LONG_DOUBLE,
	      (MR_TYPE_STRUCT, , "struct_param"),
	      (MR_TYPE_ENUM, , "enum_param"),
	      MR_TYPE_FUNC_TYPE,
	      (MR_TYPE_ENUM_VALUE, , "enum_value"), /* meta field refers to union member in mr_fd_param_t */
	      (MR_TYPE_FUNC, , "func_param"),
	      (MR_TYPE_BITFIELD, , "bitfield_param"),
	      MR_TYPE_BITMASK,
	      (MR_TYPE_ARRAY, , "array_param"),
	      MR_TYPE_POINTER,
	      MR_TYPE_UNION,
	      MR_TYPE_ANON_UNION,
	      MR_TYPE_NAMED_ANON_UNION,
	      MR_TYPE_END_ANON_UNION,
	      MR_TYPE_TRAILING_RECORD,
	      MR_TYPE_LAST, /* keep it last */
	      )

TYPEDEF_STRUCT (mr_rarray_t, ATTRIBUTES ( , "resizable array type"),
		(mr_ptr_t, data, , "type"),
		(ssize_t, MR_SIZE, , "used space in bytes"),
		VOID (ssize_t, alloc_size, , "allocated space in bytes"),
		(char *, type, , "type of 'data' pointer"),
		)

TYPEDEF_ENUM (mr_status_t, ATTRIBUTES ( , "return status"),
	      MR_SUCCESS,
	      MR_FAILURE,
	      )

TYPEDEF_STRUCT (mr_array_param_t, ATTRIBUTES ( , "array parameters"),
		(unsigned int, count, , "array size"),
		(unsigned int, row_count, , "row size"),
		)

TYPEDEF_STRUCT (mr_bitfield_param_t, ATTRIBUTES ( , "bit-field parameters"),
		(uint8_t * , bitfield, , "flagged bit-fields saved as resizable array of bytes",
		{ .offset = offsetof (mr_bitfield_param_t, size) }, "offset"), 
		(unsigned int, size, , "size of bitfield array"),
		(uint8_t, width, , "bit-field width in bits"),
		(uint8_t, shift, , "bit-field shift in first byte"),
		)

TYPEDEF_STRUCT (mr_func_param_t, ATTRIBUTES ( , "types descriptors for function return value and all arguments"),
		(struct mr_fd_t *, args, , "function arguments descriptors saved as resizable array of field descriptors",
		{ .offset = offsetof (mr_func_param_t, size) }, "offset"), 
		(unsigned int, size, , "size of args array"),
		)

TYPEDEF_UNION (mr_fd_param_t, ATTRIBUTES ( , "optional parameters for different types"),
	       VOID (uint8_t, dummy, , "default serialization is empty"),
	       (mr_array_param_t, array_param, , "array parameters"),
	       (uint64_t, enum_value, , "enum value"),
	       (mr_bitfield_param_t, bitfield_param, , "bit-field parameters"),
	       (mr_func_param_t, func_param, , "types of function arguments"),
	       )

TYPEDEF_FUNC (int, mr_compar_fn_t, (__const mr_ptr_t /* x */, __const mr_ptr_t /* y */, __const void * /* context */))

TYPEDEF_FUNC (mr_status_t, mr_visit_fn_t, (mr_ptr_t /* nodep */, __const void * /* context */))

TYPEDEF_FUNC (mr_hash_value_t, mr_hash_fn_t, (mr_ptr_t /* nodep */, __const void * /* context */))

TYPEDEF_STRUCT (mr_hashed_string_t, ATTRIBUTES ( , "basic type for hash lookup over field 'name'"),
		(char *, str, , "key field"),
		(mr_hash_value_t, hash_value, , "hash value of 'str'"),
		)

TYPEDEF_ENUM (mr_ic_type_t, ATTRIBUTES ( , "types of indexed collections"),
	      (MR_IC_UNINITIALIZED, = 0, "void_ptr"),
	      (MR_IC_CUSTOM, , "custom"),
	      (MR_IC_UNSORTED_ARRAY, , "rarray"),
	      (MR_IC_SORTED_ARRAY, , "rarray"),
	      (MR_IC_HASH_NEXT, , "hash_next"),
	      (MR_IC_STATIC_ARRAY, , "static_array"),
	      (MR_IC_RBTREE, , "rb"),
	      (MR_IC_AVLTREE, , "avl"),
	      )

TYPEDEF_STRUCT (mr_ic_rarray_t, ATTRIBUTES ( , "resizable array with pointers for indexed collections"),
		(mr_ptr_t *, ra, , "key_type", { .offset = offsetof (mr_ic_rarray_t, size) }, "offset"), 
		(ssize_t, size, , "size of array"),
		VOID (ssize_t, alloc_size, , "allocated size for array"),
		)

TYPEDEF_STRUCT (mr_ic_hash_next_t, ATTRIBUTES ( , "private fields for indexed collections based on hash table"),
		(mr_hash_fn_t, hash_fn),
		/* resizable array for hash table sized by field 'size' mr_ptr_t typed by 'key_type' in mr_ic_t */
		(mr_ptr_t *, hash_table, , "key_type", { .offset = offsetof (mr_ic_hash_next_t, size) }, "offset"),
		(mr_ptr_t, zero, , "pre allocated element for zero key"),
		(unsigned, size, , "size of hash table"),
		BITFIELD (unsigned, resize_count, : sizeof (unsigned) * __CHAR_BIT__ - 1, "Next resize after this amount"),
		BITFIELD (bool, zero_key, : 1),
		)

TYPEDEF_STRUCT (mr_ic_static_array_t, ATTRIBUTES ( , "indexed collection for small sets"),
		(mr_hash_fn_t, hash_fn),
		(mr_ptr_t, static_array, [(sizeof (mr_ic_hash_next_t) - sizeof (mr_hash_fn_t)) / sizeof (mr_ptr_t)], "key_type"),
		)

TYPEDEF_ENUM (mr_child_idx_t,
	      (MR_LEFT, = 0),
	      (MR_RIGHT, = 1),
	      )

TYPEDEF_STRUCT (mr_tree_path_t, ATTRIBUTES ( , "traverse index and discent direction"), 
		(unsigned int, idx, , "index in the pool"),
		BITFIELD (mr_child_idx_t, child_idx, : 1),
		BITFIELD (bool, equal, : 1),
		)

TYPEDEF_STRUCT (mr_rbtree_node_t, ATTRIBUTES ( , "node of the red/black tree"),
		BITFIELD (unsigned int, left, : sizeof (unsigned int) * __CHAR_BIT__ - 1, "index in the pool"),
		VOID (bool, unused, : 1),
		BITFIELD (unsigned int, right, : sizeof (unsigned int) * __CHAR_BIT__ - 1, "index in the pool"),
		BITFIELD (bool, red, : 1),
		)

TYPEDEF_STRUCT (mr_avltree_node_t, ATTRIBUTES ( , "node of the avl tree"),
		BITFIELD (unsigned int, left, : sizeof (unsigned int) * __CHAR_BIT__ - 1, "index in the pool"),
		BITFIELD (bool, balanced, : 1),
		BITFIELD (unsigned int, right, : sizeof (unsigned int) * __CHAR_BIT__ - 1, "index in the pool"),
		BITFIELD (mr_child_idx_t, longer, : 1),
		)

TYPEDEF_STRUCT (mr_tree_node_idx_t, ATTRIBUTES ( , "index of the binary tree with 1 bit extra property"),
		BITFIELD (unsigned int, idx, : sizeof (unsigned int) * __CHAR_BIT__ - 1, "index in the pool"),
		BITFIELD (bool, bit, : 1),
		)

TYPEDEF_STRUCT (mr_tree_node_t, ATTRIBUTES ( , "node of the red/black or avl tree"),
		(mr_ptr_t, key, , "key_type"),
		ANON_UNION (),
		(mr_tree_node_idx_t, next, [2], "left and right children"),
		(mr_tree_node_idx_t, root),
		(mr_avltree_node_t, avl),
		(mr_rbtree_node_t, rb),
		END_ANON_UNION ("ic_type"),
		)

TYPEDEF_STRUCT (mr_tree_t, ATTRIBUTES ( , "indexed collection for red/black tree"),
		(mr_tree_node_t *, pool, , "mr_tree_node_t allocation pool", { .offset = offsetof (mr_tree_t, size) }, "offset"),
		(ssize_t, size),
		VOID (ssize_t, alloc_size),
		)

TYPEDEF_STRUCT (mr_res_t,
		(mr_ptr_t, data, , "type"), 
		(char *, type, , "union discriminator"),
		(ssize_t, MR_SIZE, , "size of data"),
		)

TYPEDEF_STRUCT (mr_ic_t, ATTRIBUTES ( , "indexed collection"),
		(mr_ic_type_t, ic_type),
		(unsigned int, items_count),
		(mr_res_t, context),
		(char *, key_type),
		(mr_compar_fn_t, compar_fn),
		(struct mr_ic_virt_func_t *, virt_func),
		
		ANON_UNION (type_specific),
		(void *, void_ptr, , "default serialization"),
		(mr_res_t, custom, , "pointer on custom data for extended IC types"),
		(mr_ic_rarray_t, rarray),
		(mr_ic_hash_next_t, hash_next),
		(mr_ic_static_array_t, static_array),
		(mr_tree_t, rb),
		(mr_tree_t, avl),
		END_ANON_UNION ("ic_type"),
		)

TYPEDEF_STRUCT (mr_ic_virt_func_t, ATTRIBUTES ( , "virtual functions table for indexed collections"),
		(mr_ptr_t *, add, (mr_ic_t * /* ic */, mr_ptr_t /* key */)),
		(mr_status_t, del, (mr_ic_t * /* ic */, mr_ptr_t /* key */)),
		(mr_ptr_t *, find, (mr_ic_t * /* ic */, mr_ptr_t /* key */)),
		(mr_status_t, foreach, (mr_ic_t * /* ic */, mr_visit_fn_t /* visit_fn */, __const void * /* context */)),
		(mr_status_t, index, (mr_ic_t * /* ic */, mr_ic_rarray_t * /* rarray */)),
		(void, free, (mr_ic_t * /* ic */)),
		)

TYPEDEF_STRUCT (mr_fd_t, ATTRIBUTES ( , "Metaresc field descriptor"),
		(mr_type_t, mr_type, , "Metaresc type"),
		BITFIELD (mr_type_t, mr_type_aux, : sizeof (mr_type_t) * __CHAR_BIT__ - 1, "Metaresc type if field is a pointer on builtin types or bit-field"),
		BITFIELD (bool, unnamed, : 1 , "by default all fields are named, but anonymous unions and fields in mr_ptr_t should be unnamed"),
		(mr_hashed_string_t, name, , "hashed name of the field"),
		(char *, type, , "stringified type name"),
		(unsigned int, offset, , "offset in structure"),
		(unsigned int, size, , "size of field"),
		(mr_fd_param_t, param, , "mr_type"),
		(char *, meta, , "field meta info"),
		/*
		  res field can be used by user for extended information
		  placed after meta field for tricky intialization
		  sample:
		  VOID (void *, ptr, , "user extended info", { &((ext_info_t){ .field = XXX }) }, "ext_info_t")
		  or
		  VOID (void *, ptr, , "user extended info", { (ext_info_t[]){ {.field = XXX}, {.field = YYY} } }, "ext_info_t", 2 * sizeof (ext_info_t))
		  or
		  VOID (void *, ptr, , "user extended info", { "one more extra string" }, "char")
		*/
		(mr_ptr_t, res, , "res_type"), /* extra pointer for user data */
		(char *, res_type, , "union discriminator"),
		(ssize_t, MR_SIZE, , "size of array pointed by 'res'"),
		)

TYPEDEF_STRUCT (mr_fd_ptr_t, ATTRIBUTES ( , "wrapper for mr_fd_t*"),
		(mr_fd_t *, fdp, , "wrapper for mr_fd_t pointer type"),
		)

TYPEDEF_STRUCT (mr_enum_param_t,
		(mr_ic_t, enum_by_value, , "lookup by enum values"),
		(mr_size_t, size_effective, , "effective size"),
		(mr_type_t, mr_type_effective, , "automatic type detection is required for enums size adjustment"),
		(bool, is_bitmask, , "set to true if all enum values are power of 2"),
		)

TYPEDEF_STRUCT (mr_struct_param_t,
		(mr_ic_t, field_by_offset, , "lookup by field offset"),
		)

TYPEDEF_UNION (mr_td_param_t,
	       VOID (uint8_t, void_param, , "default serialization"),
	       (mr_enum_param_t, enum_param, , "parameters specific for enums"),
	       (mr_struct_param_t, struct_param, , "parameters specific for structures"),
	       )

TYPEDEF_STRUCT (mr_td_t, ATTRIBUTES ( , "Metaresc type descriptor"),
		(mr_type_t, mr_type, , "Metaresc type"), /* possible variants MR_TYPE_ENUM, MR_TYPE_STRUCT, MR_TYPE_UNION */
		(mr_hashed_string_t, type, , "hashed name of the type"),
		(mr_size_t, size, , "size of type"),
		(char *, attr, , "stringified typedef attributes"),
		(mr_td_param_t, param, , "mr_type"),
		(mr_ic_t, field_by_name, , "lookup by field names"),
		(ssize_t, fields_size, , "size of 'fields' array"),
		(mr_fd_ptr_t *, fields, , "fields or enums descriptors", { .offset = offsetof (mr_td_t, fields_size) }, "offset"),
		(char *, meta, , "type meta info"),
		(mr_ptr_t, res, , "res_type"), /* extra pointer for user data */
		(char *, res_type, , "union discriminator"),
		(ssize_t, MR_SIZE, , "size of array pointed by 'res'"),
		) /* type descriptor */

TYPEDEF_STRUCT (mr_mem_t, ATTRIBUTES ( , "Metaresc memory operations"),
		(float, mem_alloc_strategy, , "memory allocation strategy"),
		(void *, calloc, (const char *, const char *, int, size_t, size_t), "pointer on malloc() function"),
		(void *, realloc, (const char *, const char *, int, void *, size_t), "pointer on realloc() function"),
		(void, free, (const char *, const char *, int, void *), "pointer on free() function"),
		)

TYPEDEF_STRUCT (mr_ptrdes_flags_t, ATTRIBUTES (__attribute__ ((packed, aligned (sizeof (uint32_t)))), "ponter descriptor flag bitfield values"),
		BITFIELD (bool, is_null, :1),
		BITFIELD (bool, is_referenced, :1),
		BITFIELD (bool, is_content_reference, :1),
		BITFIELD (bool, is_opaque_data, :1),
		)

TYPEDEF_STRUCT (mr_union_discriminator_t, ATTRIBUTES ( , "cache for union discriminator resolution"),
		(mr_hashed_string_t, type, , "union type name"),
		(mr_hashed_string_t, discriminator, , "union discriminator"),
		(mr_fd_t *, fdp, , "discriminated union field descriptor"),
		)

TYPEDEF_STRUCT (mr_substr_t, ATTRIBUTES ( , "substring"),
		POINTER (char, str, , { .offset = offsetof (mr_substr_t, length) }, "offset"),
		(size_t, length),
		)

TYPEDEF_STRUCT (mr_quoted_substr_t, ATTRIBUTES ( , "quoted substring"),
		(mr_substr_t, substr),
		(void, unquote, (mr_substr_t *, char *)),
		)

TYPEDEF_ENUM (mr_value_type_t, ATTRIBUTES ( , "type of values from lexer"),
	      (MR_VT_VOID, = 0, "vt_void"),
	      (MR_VT_QUOTED_SUBSTR, , "vt_quoted_substr"),
	      (MR_VT_INT, , "vt_int"),
	      (MR_VT_FLOAT, , "vt_float"),
	      (MR_VT_COMPLEX, , "vt_complex"),
	      (MR_VT_CHAR, , "vt_char"),
	      (MR_VT_ID, , "vt_quoted_substr"),
	      )

TYPEDEF_STRUCT (mr_value_t, ATTRIBUTES ( , "value for expressions calculation"),
		(mr_value_type_t, value_type),
		ANON_UNION (type_specific),
		VOID (uint8_t, vt_void, , "no serialization by default"),
		long long int vt_int,
		long double vt_float,
		complex long double vt_complex,
		string_t vt_string,
		char vt_char,
		(mr_quoted_substr_t, vt_quoted_substr),
		END_ANON_UNION ("value_type"),
		)

TYPEDEF_STRUCT (mr_save_params_t, ATTRIBUTES ( , "attributes specific for saving"),
		(mr_ic_t, union_discriminator, , "index over unions discriminator"),
		(int, next_typed, , "linked list of nodes with same type and pointer"),
		(int, next_untyped, , "linked list of nodes with same pointer"),
		(int, level, , "level from the root element"),
		)

TYPEDEF_STRUCT (mr_load_params_t, ATTRIBUTES ( , "attributes specific for loading"),
		(mr_value_t, mr_value, , "loaded value"),
		)

TYPEDEF_STRUCT (mr_ptrdes_t, ATTRIBUTES ( , "pointer descriptor type"),
		(mr_ptr_t, data, , "type"),
		(char *, type, , "copy of fd.type for all basic types and structures"),
		(ssize_t, MR_SIZE, , "size of dynamic array"),
		(mr_fd_t, fd, , "field descriptor"),
		(int32_t, idx, , "public index"),
		(int32_t, ref_idx, , "reference index (internal enumeration)"),
		(int, parent, , "parent index"),
		(int, first_child, , "first child index"),
		(int, last_child, , "last child index"),
		(int, prev, , "previous sibling index"),
		(int, next, , "next sibling index"),
		(mr_ptrdes_flags_t, flags),
		ANON_UNION (type_specific),
		(mr_save_params_t, save_params, , "attributes specific for saving"),
		(mr_load_params_t, load_params, , "attributes specific for loading"),
		END_ANON_UNION ("ptrdes_type"),
		(mr_res_t, res, , "extra pointer for user data"),
		)

TYPEDEF_ENUM (mr_ptrdes_type_t,
	      (MR_PD_SAVE, , "save_params"),
	      (MR_PD_LOAD, , "load_params"),
	      )

TYPEDEF_STRUCT (mr_ra_ptrdes_t, ATTRIBUTES ( , "mr_ptrdes_t resizable array"),
		(mr_ptrdes_t *, ra, , "resizable array with descriptors of saved elements",
		{ .offset = offsetof (mr_ra_ptrdes_t, size) }, "offset"),
		(ssize_t, size, , "size of resizable array"),
		VOID (ssize_t, alloc_size, , "allocated size of resizable array"),
		(mr_ptrdes_type_t, ptrdes_type, , "discriminator for anonymous union in mr_ptrdes_t"),
		(mr_res_t, res, , "extra pointer for user data"),
		)

TYPEDEF_STRUCT (mr_load_data_t, ATTRIBUTES ( , "state for objects loading"),
		(mr_ra_ptrdes_t, ptrs, , "internal representation of a loaded tree"),
		(ssize_t, mr_ra_idx_size, , "size of 'mr_ra_idx'"),
		(unsigned int *, mr_ra_idx, , "indexes of postponed nodes", { .offset = offsetof (mr_load_data_t, mr_ra_idx_size) }, "offset"),
		VOID (ssize_t, mr_ra_idx_alloc_size, , "allocated size of 'mr_ra_idx'"),
		)

TYPEDEF_STRUCT (mr_load_node_context_t,
		int idx,
		(mr_ra_ptrdes_t *, ptrs)
		)

TYPEDEF_FUNC (mr_status_t, mr_load_handler_t, (int /* idx */, mr_load_data_t * /* mr_load_data */))

TYPEDEF_FUNC (mr_status_t, mr_process_quoted_str_t, (char * /* src */, void * /* arg */))

TYPEDEF_STRUCT (mr_lloc_t, ATTRIBUTES ( , "parser location"),
		(int, lineno, , "parser location - line number"),
		(int, column, , "parser location - column number"),
		(int, offset, , "parser location - offset in string"),
		)

TYPEDEF_STRUCT (mr_token_lloc_t, ATTRIBUTES ( , "token location"),
		(mr_lloc_t, start, , "start of the token"),
		(mr_lloc_t, end, , "end of the token"),
		)

TYPEDEF_STRUCT (mr_load_t, ATTRIBUTES ( , "Metaresc load parser data"),
		(mr_lloc_t, lloc, , "current location of parser"),
		(char *, str, , "string to parse"),
		(char *, buf, , "parser internal buffer"),
		(int, parent, , "index of current parent"),
		(mr_ra_ptrdes_t *, ptrs, , "resizable array with mr_ptrdes_t"),
		)

TYPEDEF_STRUCT (mr_save_data_t, ATTRIBUTES ( , "save routines data and lookup structures"),
		(mr_ra_ptrdes_t, ptrs, , "internal representation of a saved tree"),
		(mr_ic_t, typed_ptrs, , "index over typed nodes"),
		(mr_ic_t, untyped_ptrs, , "index over untyped nodes"),
		(ssize_t, mr_ra_idx_size, , "size of 'mr_ra_idx'"),
		(unsigned int *, mr_ra_idx, , "indexes of postponed nodes", { .offset = offsetof (mr_save_data_t, mr_ra_idx_size) }, "offset"),
		VOID (ssize_t, mr_ra_idx_alloc_size, , "allocated size of 'mr_ra_idx'"),
		(ssize_t, mr_ra_ud_size, , "size of 'mr_ra_ud'"),
		(mr_union_discriminator_t *, mr_ra_ud, , "allocation of union discriminators",
		{ .offset = offsetof (mr_save_data_t, mr_ra_ud_size) }, "offset"),
		VOID (ssize_t, mr_ra_ud_alloc_size, , "allocated size of 'mr_ra_ud'"),
		)

TYPEDEF_FUNC (int, mr_ra_printf_t, (mr_rarray_t *, mr_ptrdes_t *))

TYPEDEF_FUNC (void, mr_msg_handler_t, (const char *, const char *, int, mr_log_level_t, mr_message_id_t, va_list), ATTRIBUTES ( , "handler for error messages"))

TYPEDEF_STRUCT (mr_conf_t, ATTRIBUTES ( , "Metaresc configuration"),
		(mr_mem_t, mr_mem, , "memory operations"),
		(mr_log_level_t, log_level),
		(mr_msg_handler_t, msg_handler),
		(bool, cache_func_resolve, , "global knob to enable dladdr caching"),
		(mr_ic_t, enum_by_name, , "index over all enum names"),
		(mr_ic_t, fields_names, , "index of all fields names"),
		(mr_ic_t, type_by_name, , "index over types descriptors"),
		(mr_ra_printf_t, output_format, [MR_TYPE_LAST], "formaters"),
		)
