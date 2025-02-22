/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

#include <metaresc.h>

TYPEDEF_UNION (mr_ptr_t, ATTRIBUTES (__attribute__((transparent_union)), "pointer on any type"),
	       (void *, ptr, , "mr_ptr_t void pointer", { MR_SIZE_STR }, "string", .unnamed = true),
	       (void *, MR_OPAQUE_DATA, , "XDR opaque data", { MR_SIZE_STR }, "string"),
	       (char *, string),
	       (mr_offset_t, offset),
	       (uintptr_t, uintptr),
	       (intptr_t, intptr),
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
	      (MR_MESSAGE_UNEXPECTED_SUBNODES, , "Unexpected subnodes for pointer marked as a reference."),
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
	      (MR_MESSAGE_BAD_ENUM_TYPE, , "Bad enum type %d."),
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
	      (MR_MESSAGE_UNSUPPORTED_NODE_TYPE, , "Unsupported node type: %d."),
	      (MR_MESSAGE_RANGE_CHECK, , "Range check error in array '%s'."),
	      (MR_MESSAGE_READ_REF, , "Can't read '%s' as index."),
	      (MR_MESSAGE_SAVE_ENUM, , "Can't find enum name for value %" SCNu64 " type '%s' field '%s'."),
	      (MR_MESSAGE_CONFLICTED_ENUMS, , "Literal enum `%s` has a different value in type '%s': %" SCNu64" != %" SCNu64 "."),
	      (MR_MESSAGE_CONFLICTED_OVERRIDES, , "Overrides for value %" SCNu64 " has two different discriminators '%s' and '%s'."),
	      (MR_MESSAGE_INVALID_OVERRIDE, , "Override for value %" SCNu64 " has invalid discriminator '%s'."),
	      (MR_MESSAGE_PARSE_ERROR, , "Parser error: '%s'. Position: %d:%d-%d:%d."),
	      (MR_MESSAGE_UNKNOWN_XML_ESC, , "Unknown XML escape sequence '%s'."),
	      (MR_MESSAGE_WRONG_XML_ESC, , "Wrong XML escape sequence '%s'."),
	      (MR_MESSAGE_UNION_DISCRIMINATOR_ERROR, , "Union failed to find field by discriminator '%s'."),
	      (MR_MESSAGE_POINTER_NODE_CHILD_MISSING, , "Pointer node child missing: type '%s' name '%s'."),
	      (MR_MESSAGE_UNEXPECTED_NULL_POINTER, , "Unexpected NULL pointer."),
	      (MR_MESSAGE_WRONG_RESULT_TYPE, , "Wrong result type."),
	      (MR_MESSAGE_DIVISION_BY_ZERO, , "Division by zero."),
	      (MR_MESSAGE_UNEXPECTED_TARGET_TYPE, , "Unexpected target type (%d)."),
	      (MR_MESSAGE_UNEXPECTED_DATA_AT_THE_END, , "Unexpected data at the end of the string '%s'."),
	      (MR_MESSAGE_UNEXPECTED_HASH_TABLE_ERROR, , "Unexpected error in hash table."),
	      (MR_MESSAGE_UNEXPECTED_CHARS_AFTER_CLOSING_TAG, , "Unexpected charecters after closing tag."),
	      (MR_MESSAGE_TAGS_DONT_MATCH, , "Open and close tags names do not match."),
	      (MR_MESSAGE_WRONG_SIZE_FOR_DYNAMIC_ARRAY, , "Wrong size (%zd) for dynamics array."),
	      (MR_MESSAGE_UNEXPECTED_MR_TYPE, , "Unexpected mr_type for serialized node."),
	      (MR_MESSAGE_UNEXPECTED_NUMBER_OF_ITEMS, , "Unexpected number of items in static array collection %d."),
	      (MR_MESSAGE_TOO_MANY_UD, , "Serialization require more distinct union discriminators. Recompile Metaresc with MR_RA_UD_IDX_TYPE defined to a type bigger then '%s' (e.g. ./configure --enable-ra-ud-idx=uint16_t)"),
	      (MR_MESSAGE_FIELD_NOT_FOUND, , "Field '%s' is not found in type '%s'."),
	      (MR_MESSAGE_YAML_ERROR, , "YAML error '%s'."),
	      (MR_MESSAGE_UNKNOWN_FIELD_NAME, , "Field name '%s' is not valid."),
	      (MR_MESSAGE_INTERNAL_PARSER_ERROR, , "Internal parser error: load structure is corrupted."),
	      (MR_MESSAGE_ZERO_SIZE_FIELD, , "Field '%s' has zero size."),
	      (MR_MESSAGE_LAST, , "Last message ID."),
	      )

TYPEDEF_ENUM (mr_type_t, ATTRIBUTES (__attribute__ ((packed)) , "Metaresc types"),
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
	      MR_TYPE_INT128,
	      MR_TYPE_UINT128,
	      MR_TYPE_FLOAT,
	      MR_TYPE_COMPLEX_FLOAT,
	      MR_TYPE_DOUBLE,
	      MR_TYPE_COMPLEX_DOUBLE,
	      MR_TYPE_LONG_DOUBLE,
	      MR_TYPE_COMPLEX_LONG_DOUBLE,
	      MR_TYPE_STRUCT,
	      MR_TYPE_ENUM,
	      MR_TYPE_FUNC_TYPE,
	      MR_TYPE_FUNC,
	      MR_TYPE_BITFIELD,
	      MR_TYPE_ARRAY,
	      MR_TYPE_POINTER,
	      MR_TYPE_UNION,
	      MR_TYPE_ANON_UNION,
	      MR_TYPE_NAMED_ANON_UNION,
	      MR_TYPE_END_ANON_UNION,
	      MR_TYPE_LAST, /* keep it last */
	      )

TYPEDEF_ENUM (mr_type_class_t, ATTRIBUTES (__attribute__ ((packed)) , "Classification provided by __builtin_classify_type"),
	      (MR_NO_TYPE_CLASS, = -1),
	      MR_VOID_TYPE_CLASS,
	      MR_INTEGER_TYPE_CLASS,
	      MR_CHAR_TYPE_CLASS,
	      MR_ENUMERAL_TYPE_CLASS,
	      MR_BOOLEAN_TYPE_CLASS,
	      MR_POINTER_TYPE_CLASS,
	      MR_REFERENCE_TYPE_CLASS,
	      MR_OFFSET_TYPE_CLASS,
	      MR_REAL_TYPE_CLASS,
	      MR_COMPLEX_TYPE_CLASS,
	      MR_FUNCTION_TYPE_CLASS,
	      MR_METHOD_TYPE_CLASS,
	      MR_RECORD_TYPE_CLASS,
	      MR_UNION_TYPE_CLASS,
	      MR_ARRAY_TYPE_CLASS,
	      MR_STRING_TYPE_CLASS,
	      MR_LANG_TYPE_CLASS,
	      MR_OPAQUE_TYPE_CLASS,
	      MR_BITINT_TYPE_CLASS,
	      MR_VECTOR_TYPE_CLASS,
	      )

TYPEDEF_ENUM (mr_td_producer_t, ATTRIBUTES (__attribute__ ((packed)) , "Type descriptor producer"),
	      MR_TDP_MACRO,
	      MR_TDP_DWARF,
	      MR_TDP_DUMP_STRUCT,
	      MR_TDP_ANON_UNION,
	      MR_TDP_LAST,
	      )

TYPEDEF_ENUM (mr_status_t, ATTRIBUTES ( , "return status"),
	      MR_SUCCESS,
	      MR_FAILURE,
	      )

TYPEDEF_ENUM (mr_ic_type_t, ATTRIBUTES ( , "types of indexed collections"),
	      (MR_IC_UNINITIALIZED, = 0, "void_ptr"),
	      (MR_IC_UNSORTED_ARRAY, , "rarray"),
	      (MR_IC_SORTED_ARRAY, , "rarray"),
	      (MR_IC_STATIC_ARRAY, , "static_array"),
	      (MR_IC_HASH, , "hash"),
	      (MR_IC_RBTREE, , "rb"),
	      (MR_IC_AVLTREE, , "avl"),
	      )

TYPEDEF_FUNC (int, mr_compar_fn_t, (__const mr_ptr_t /* x */, __const mr_ptr_t /* y */, __const void * /* context */))

TYPEDEF_FUNC (mr_status_t, mr_visit_fn_t, (mr_ptr_t /* nodep */, void * /* context */))

TYPEDEF_FUNC (mr_hash_value_t, mr_hash_fn_t, (mr_ptr_t /* nodep */, __const void * /* context */))

TYPEDEF_STRUCT (mr_rarray_t, ATTRIBUTES ( , "resizable array type"),
		(mr_ptr_t, data, , "type"),
		(ssize_t, MR_SIZE, , "used space in bytes"),
		VOID (ssize_t, alloc_size, , "allocated space in bytes"),
		(char *, type, , "type of 'data' pointer"),
		)

TYPEDEF_STRUCT (mr_ic_rarray_t, ATTRIBUTES ( , "resizable array with pointers for indexed collections"),
		(mr_ptr_t *, ra, , "key_type", { .offset = offsetof (mr_ic_rarray_t, size) }, "offset"), 
		(ssize_t, size, , "size of array"),
		VOID (ssize_t, alloc_size, , "allocated size for array"),
		)

TYPEDEF_STRUCT (mr_ic_hash_t, ATTRIBUTES ( , "private fields for indexed collections based on hash table"),
		(mr_hash_fn_t, hash_fn),
		/* resizable array for hash table sized by field 'size' mr_ptr_t typed by 'key_type' in mr_ic_t */
		(mr_ptr_t *, hash_table, , "key_type", { "size" }, "string"),
		(size_t, size, : sizeof (size_t) * __CHAR_BIT__ - 1, "size of hash table"),
		(bool, zero_key, : 1),
		)

TYPEDEF_STRUCT (mr_ic_static_array_t, ATTRIBUTES ( , "indexed collection for small sets"),
		(mr_ptr_t, static_array, [sizeof (mr_ic_hash_t) / sizeof (mr_ptr_t)], "key_type"),
		)

TYPEDEF_ENUM (mr_child_idx_t, ATTRIBUTES (__attribute__ ((packed)), "tree traverse to the left/right"),
	      (MR_LEFT, = 0),
	      (MR_RIGHT, = 1),
	      )

TYPEDEF_STRUCT (mr_rbtree_node_t, ATTRIBUTES ( , "node of the red/black tree"),
		(unsigned int, left, : sizeof (unsigned int) * __CHAR_BIT__ - 1, "index in the pool"),
		VOID (bool, unused, : 1),
		(unsigned int, right, : sizeof (unsigned int) * __CHAR_BIT__ - 1, "index in the pool"),
		(bool, red, : 1),
		)

TYPEDEF_STRUCT (mr_avltree_node_t, ATTRIBUTES ( , "node of the avl tree"),
		(unsigned int, left, : sizeof (unsigned int) * __CHAR_BIT__ - 1, "index in the pool"),
		(bool, balanced, : 1),
		(unsigned int, right, : sizeof (unsigned int) * __CHAR_BIT__ - 1, "index in the pool"),
		(mr_child_idx_t, longer, : 1),
		)

TYPEDEF_STRUCT (mr_tree_node_idx_t, ATTRIBUTES ( , "index of the binary tree with 1 bit extra property"),
		(unsigned int, idx, : sizeof (unsigned int) * __CHAR_BIT__ - 1, "index in the pool"),
		(bool, bit, : 1),
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

TYPEDEF_STRUCT (mr_tree_t, ATTRIBUTES ( , "indexed collection for binary tree"),
		(mr_tree_node_t *, pool, , "mr_tree_node_t allocation pool", { .offset = offsetof (mr_tree_t, size) }, "offset"),
		(ssize_t, size),
		VOID (ssize_t, alloc_size),
		)

TYPEDEF_STRUCT (mr_tree_path_t, ATTRIBUTES ( , "element of traverse index and discent direction"), 
		(unsigned int, idx, : sizeof (unsigned int) * __CHAR_BIT__ - 1, "index in the pool"),
		(mr_child_idx_t, child_idx, : 1, "descent direction"),
		)

TYPEDEF_STRUCT (mr_tree_traverse_t, ATTRIBUTES ( , "tree traverse and zero flag for the last comparison"),
		(uint16_t, size, , "size of tree traverse"),
		(bool, equal, , "equal flag for the last comparison in the traverse"),
		(mr_tree_path_t, path, [(sizeof (mr_tree_node_idx_t) * __CHAR_BIT__ << 1) - 1], "tree traverse path", { .offset = offsetof (mr_tree_traverse_t, size) }, "offset"),
		)

TYPEDEF_STRUCT (mr_typed_ptr_t,
		(char *, type, , "union discriminator"),
		(mr_ptr_t, data, , "type"),
		)

TYPEDEF_STRUCT (mr_res_t,
		(char *, type, , "union discriminator"),
		(ssize_t, MR_SIZE, , "size of data"),
		(mr_ptr_t, data, , "type"),
		)

TYPEDEF_STRUCT (mr_ic_t, ATTRIBUTES ( , "indexed collection"),
		(mr_res_t, context),
		(mr_ic_type_t, ic_type),
		(unsigned int, items_count),
		(char *, key_type),
		(mr_compar_fn_t, compar_fn),
		(struct mr_ic_virt_func_t *, virt_func),
		
		ANON_UNION (),
		(void *, void_ptr, [0], "default serialization"),
		(mr_ic_rarray_t, rarray),
		(mr_ic_hash_t, hash),
		(mr_ic_static_array_t, static_array),
		(mr_tree_t, rb),
		(mr_tree_t, avl),
		(mr_tree_t, tree),
		END_ANON_UNION ("ic_type"),
		)

TYPEDEF_STRUCT (mr_ic_virt_func_t, ATTRIBUTES ( , "virtual functions table for indexed collections"),
		(mr_ptr_t *, add, (mr_ic_t * /* ic */, mr_ptr_t /* key */)),
		(mr_status_t, del, (mr_ic_t * /* ic */, mr_ptr_t /* key */)),
		(mr_ptr_t *, find, (mr_ic_t * /* ic */, mr_ptr_t /* key */)),
		(mr_status_t, foreach, (mr_ic_t * /* ic */, mr_visit_fn_t /* visit_fn */, void * /* context */)),
		(mr_status_t, index, (mr_ic_t * /* ic */, mr_ptr_t * /* rarray */, size_t /* size */)),
		(void, free, (mr_ic_t * /* ic */)),
		)

TYPEDEF_STRUCT (mr_ud_override_t, ATTRIBUTES ( , "key value pair for union discriminator override"),
		(int64_t, value, , "discriminator value that has a custom override"),
		(char *, discriminator, , "name of union branch for this value"),
		(char *, type, , "discriminator for typed_value"),
		(mr_ptr_t, typed_value, , "type"),
		(struct mr_fd_t *, fdp, , "descriptor of union branch"),
		)

TYPEDEF_STRUCT (mr_array_dimensions_t, ATTRIBUTES ( , "all array's dimensions"),
		(uint32_t, dim, [4], "up to 4 dimensions", { .offset = offsetof (mr_array_dimensions_t, size) }, "offset"),
		(uint32_t, size, , "size of 'dim' array"),
		)

TYPEDEF_STRUCT (mr_stype_t, ATTRIBUTES ( , "Metaresc structured type"),
		(struct mr_td_t *, tdp, , "type descriptor"),
		(char *, type, , "stringified type name"),
		(mr_type_t, mr_type, , "Metaresc type"),
		(mr_type_t, mr_type_aux, , "Extra mr_type for pointers, arrays and bit fields"),
		(mr_type_t, mr_type_ptr, , "Extra mr_type for pointers and arrays"),
		(mr_type_class_t, mr_type_class, : __CHAR_BIT__ - 2, "required to distinguish records and unions from scalar types"),
		(bool, is_array, : 1, "true if field is an array"),
		(bool, is_bitfield, : 1, "true if field is a bitfield"),
		(mr_size_t, size, , "size of type"),
		ANON_UNION (),
		(uint8_t, default_serialization, [0], "default serialization"),
		(mr_array_dimensions_t, dim, , "array dimensions"),
		END_ANON_UNION ("mr_type", { (mr_ud_override_t[]){{ MR_TYPE_ARRAY, "dim" }} }, "mr_ud_override_t"),
		)

TYPEDEF_STRUCT (mr_hashed_string_t, ATTRIBUTES (__attribute__ ((packed)) , "hashed string"),
		(char *, str, , "key field"),
		(mr_hash_value_t, hash_value, , "hash value of 'str'"),
		)

TYPEDEF_STRUCT (mr_bitfield_param_t, ATTRIBUTES ( , "bit-field parameters"),
		(uint8_t * , bitfield, , "flagged bit-fields saved as resizable array of bytes",
		{ .offset = offsetof (mr_bitfield_param_t, size) }, "offset"), 
		(unsigned int, size, , "size of bitfield array"),
		(uint8_t, width, , "bit-field width in bits"),
		(uint8_t, shift, , "bit-field shift in first byte"),
		(bool, initialized, , "flag that width and shift are initialized"),
		)

TYPEDEF_STRUCT (mr_func_param_t, ATTRIBUTES ( , "types descriptors for function return value and all arguments"),
		(mr_stype_t **, args, , "function arguments saved as resizable array of pointers on structured types",
		{ .offset = offsetof (mr_func_param_t, size) }, "offset"), 
		(ssize_t, size, , "size of args array"),
		)

#define MR_FIELD_PARAM_UNION_UDO				\
  (mr_ud_override_t[]) {					\
      { MR_TYPE_UNION, "union_param" },				\
      { MR_TYPE_ANON_UNION, "union_param" },			\
      { MR_TYPE_NAMED_ANON_UNION, "union_param" },		\
      }

#define MR_FIELD_PARAM_NON_UNION_UDO				\
  (mr_ud_override_t[]) {					\
      { MR_TYPE_BITFIELD, "bitfield_param" },			\
      { MR_TYPE_FUNC, "func_param" },				\
      }

TYPEDEF_STRUCT (mr_fd_t, ATTRIBUTES ( , "Metaresc field descriptor"),
		(mr_stype_t, stype, , "structured type"),
		(mr_hashed_string_t, name, , "hashed name of the field"),
		(mr_type_t, mr_type, [0], "alias of mr_type for union resolution", .offset = offsetof (mr_fd_t, stype.mr_type)),
		(mr_type_t, mr_type_base, , "copy of mr_type form base type"),
		(bool, unnamed, : 1, "by default all fields are named, but anonymous unions and fields in mr_ptr_t should be unnamed"),
		(bool, non_persistent, : 1, "true if field descriptor is allocated on stack"),
		(mr_offset_t, offset, , "offset in structure"),

		ANON_UNION (),
		ANON_UNION (),
		(uint8_t, default_serialization, [0], "default serialization is empty"),
		(mr_bitfield_param_t, bitfield_param, , "bit-field parameters"),
		(mr_func_param_t, func_param, , "types of function arguments"),
		END_ANON_UNION ("mr_type", { MR_FIELD_PARAM_NON_UNION_UDO }, "mr_ud_override_t", sizeof (MR_FIELD_PARAM_NON_UNION_UDO)),
		(mr_ic_t, union_param, , "indexed collection with union discriminators overrides"),
		END_ANON_UNION ("mr_type_base", { MR_FIELD_PARAM_UNION_UDO }, "mr_ud_override_t", sizeof (MR_FIELD_PARAM_UNION_UDO)),

		(char *, meta, , "field meta info"),
		/*
		  res field can be used by user for extended information
		  placed after meta field for tricky intialization
		  sample:
		  VOID (void *, ptr, , "user extended info", { &((ext_info_t){ .field = XXX }) }, "ext_info_t")
		  or
		  VOID (void *, ptr, , "user extended info", { (ext_info_t[]){ {.field = XXX}, {.field = YYY} } }, "ext_info_t", 2 * sizeof (ext_info_t))
		  or
		  VOID (void *, ptr, , "user extended info", { "one more extra string" }, "string")
		*/
		(mr_ptr_t, res, , "res_type"), /* extra pointer for user data */
		(char *, res_type, , "union discriminator"),
		(ssize_t, MR_SIZE, , "size of array pointed by 'res'"),
		)

#define MR_ENUM_VALUE_UDO					\
  (mr_ud_override_t[]) {					\
    { MR_TYPE_INT8, "_signed" },				\
      { MR_TYPE_UINT8, "_unsigned" },				\
      { MR_TYPE_INT16, "_signed" },				\
      { MR_TYPE_UINT16, "_unsigned" },				\
      { MR_TYPE_INT32, "_signed" },				\
      { MR_TYPE_UINT32, "_unsigned" },				\
      { MR_TYPE_INT64, "_signed" },				\
      { MR_TYPE_UINT64, "_unsigned" },				\
      { MR_TYPE_INT128, "_signed" },				\
      { MR_TYPE_UINT128, "_unsigned" },				\
      }

TYPEDEF_UNION (mr_enum_value_t, ATTRIBUTES ( , "signed/unsigned value of the enum"),
	       (uint64_t, default_serialization, [0]),
	       (uint64_t, _unsigned),
	       (int64_t, _signed),
	       )

TYPEDEF_STRUCT (mr_ed_t, ATTRIBUTES ( , "Metaresc enum descriptor"),
		(mr_hashed_string_t, name, , "hashed name of the field"),
		(mr_type_t, mr_type, , "how many bits and signedness of the value"),
		(mr_enum_value_t, value, , "mr_type", { MR_ENUM_VALUE_UDO }, "mr_ud_override_t", sizeof (MR_ENUM_VALUE_UDO)),
		(char *, meta, , "field meta info"),
		(mr_ptr_t, res, , "res_type"), /* extra pointer for user data */
		(char *, res_type, , "union discriminator"),
		(ssize_t, MR_SIZE, , "size of array pointed by 'res'"),
		)

TYPEDEF_STRUCT (mr_enum_param_t,
		(mr_ed_t **, enums, , "fields or enums descriptors", { .offset = offsetof (mr_enum_param_t, enums_size) }, "offset"),
		(ssize_t, enums_size, , "size of 'enums' array"),
		(uint8_t, size_effective, , "effective size"),
		(mr_type_t, mr_type_effective, , "automatic type detection is required for enums size adjustment"),
		(bool, is_bitmask, , "set to true if all enum values are power of 2"),
		)

TYPEDEF_STRUCT (mr_struct_param_t,
		(mr_fd_t **, fields, , "fields or enums descriptors", { .offset = offsetof (mr_struct_param_t, fields_size) }, "offset"),
		(ssize_t, fields_size, , "size of 'fields' array"),
		(mr_ic_t, field_by_name, , "lookup by field names"),
		)

TYPEDEF_UNION (mr_td_param_t,
	       (uint8_t, default_serialization, [0], "default serialization"),
	       (mr_enum_param_t, enum_param, , "parameters specific for enums"),
	       (mr_struct_param_t, struct_param, , "parameters specific for structures/unions"),
	       (mr_struct_param_t, union_param, , "parameters specific for structures/unions"),
	       (mr_func_param_t, func_param, , "types of function arguments"),
	       )

#define MR_TYPE_PARAM_UDO					\
  (mr_ud_override_t[]) {					\
    { MR_TYPE_ENUM, "enum_param" },				\
      { MR_TYPE_STRUCT, "struct_param" },			\
      { MR_TYPE_UNION, "union_param" },				\
      { MR_TYPE_ANON_UNION, "union_param" },			\
      { MR_TYPE_NAMED_ANON_UNION, "union_param" },		\
      { MR_TYPE_FUNC_TYPE, "func_param" },			\
      }

TYPEDEF_STRUCT (mr_td_t, ATTRIBUTES ( , "Metaresc type descriptor"),
		(mr_hashed_string_t, type, , "hashed name of the type"),
		(mr_type_t, mr_type, , "Metaresc type"),
		(mr_td_producer_t, td_producer, , "producer of type descriptor"),
		(bool, is_union_discriminator, : 1, "type could be a union disriminator"),
		(bool, is_union_discriminator_set, : 1, "flags that is_union_discriminator is set"),
		(mr_size_t, size, , "size of type"),
		(mr_fd_t, mr_ptr_fd, , "field descriptor for mr_ptr_t"),
		(mr_td_param_t, param, , "mr_type", { MR_TYPE_PARAM_UDO }, "mr_ud_override_t", sizeof (MR_TYPE_PARAM_UDO)),
		(char *, meta, , "type meta info"),
		(mr_ptr_t, res, , "res_type"), /* extra pointer for user data */
		(char *, res_type, , "union discriminator"),
		(ssize_t, MR_SIZE, , "size of array pointed by 'res'"),
		VOID (mr_td_t *, next, , "single linked list"),
		) /* type descriptor */

TYPEDEF_STRUCT (mr_mem_t, ATTRIBUTES ( , "Metaresc memory operations"),
		(void *, calloc, (const char *, const char *, int, size_t, size_t), "pointer on malloc() function"),
		(void *, realloc, (const char *, const char *, int, void *, size_t), "pointer on realloc() function"),
		(void, free, (const char *, const char *, int, void *), "pointer on free() function"),
		)

TYPEDEF_ENUM (mr_ptrdes_flags_t, ATTRIBUTES (__attribute__ ((packed)), "ponter descriptor flag bitfield values"),
	      (MR_NO_FLAGS, = 0, "fancy serialization of empty flags"),
	      (MR_IS_NULL, = 1 << 0, "is a null pointer"),
	      (MR_IS_REFERENCE, = 1 << 1, "first_child is an index of a referenced node"),
	      (MR_IS_REFERENCED, = 1 << 2, "there is a reference on this node"),
	      (MR_IS_CONTENT_REFERENCE, = 1 << 3, "pointers on string content (may reffer only on string nodes)"),
	      (MR_IS_OPAQUE_DATA, = 1 << 4, "opaque serialization of resizeable pointer for XDR"),
	      (MR_IS_UNNAMED, = 1 << 5, "for CINIT serialization some nodes are unnamed (anonymous union and arrays elements)"),
	      (MR_IS_TYPED, = 1 << 6, "for CINIT deserialization pointer node has a type"),
	      )

TYPEDEF_STRUCT (mr_union_discriminator_t, ATTRIBUTES ( , "cache for union discriminator resolution"),
		(mr_fd_t *, union_fdp, , "union field descriptor"),
		(mr_fd_t *, discriminated_fdp, , "discriminated union field descriptor"),
		)

TYPEDEF_STRUCT (mr_substr_t, ATTRIBUTES (__attribute__ ((packed)), "substring"),
		(char *, str, , "pointer on substring", { .offset = offsetof (mr_substr_t, length) }, "offset", .stype.mr_type = MR_TYPE_POINTER),
		(size_t, length, , "length of the substring"),
		)

TYPEDEF_ENUM (mr_value_type_t, ATTRIBUTES (__attribute__ ((packed)), "type of values from lexer"),
	      (MR_VT_VOID, = 0),
	      (MR_VT_CHAR, , "vt_char"),
	      (MR_VT_STRING, , "vt_string"),
	      (MR_VT_SUBSTR, , "vt_substr"),
	      (MR_VT_ID, , "vt_substr"),
	      (MR_VT_INT, , "vt_int"),
	      (MR_VT_FLOAT, , "vt_float"),
	      (MR_VT_COMPLEX, , "vt_complex"),
	      )

TYPEDEF_STRUCT (mr_value_t, ATTRIBUTES ( , "value for expressions calculation"),
		(mr_value_type_t, value_type),
		ANON_UNION (),
		(uint8_t, default_serialization, [0], "no serialization by default"),
		(complex_long_double_t, vt_complex),
		(mr_substr_t, vt_substr),
		(mr_intmax_t, vt_int),
		long double vt_float,
		string_t vt_string,
		char vt_char,
		END_ANON_UNION ("value_type"),
		)

TYPEDEF_UNION (mr_load_params_t, ATTRIBUTES ( , "attributes specific for loading"),
	       (uint8_t, default_serialization, [0], "no serialization by default"),
	       (complex_long_double_t *, vt_complex, , "Macos on M1 has long double the same as double, so pointer on this type is stored as double* in DWARF"),
	       (mr_substr_t, vt_substr),
	       (uint8_t, vt_int, [sizeof (mr_intmax_t)]),
	       (uint8_t, vt_float, [sizeof (long double)]),
	       string_t vt_string,
	       char vt_char,
	       )

TYPEDEF_STRUCT (mr_ud_set_t, ATTRIBUTES (__attribute__ ((packed)), "set union discriminator indexes"),
		ANON_UNION ( , __attribute__ ((packed))),
		/* to make mr_ptrdes_t more compact we need to align size of mr_save_params_t with size of mr_load_params_t */
		(MR_RA_UD_IDX_TYPE, idx, [(sizeof (mr_load_params_t) - sizeof (mr_idx_t) - sizeof (uint8_t)) / sizeof (MR_RA_UD_IDX_TYPE)],
		 "in place list of union discriminators", { "size" }, "string"),
		(mr_ic_t *, union_discriminator, , "index over unions discriminator"),
		END_ANON_UNION ("ud_is_ic"),
		(unsigned, size, : __CHAR_BIT__ - 1, "size of union discriminator in place list"),
		(bool, is_ic, : 1, "true if union discriminator is an indexed collection"),
		)

TYPEDEF_STRUCT (mr_save_params_t, ATTRIBUTES ( , "attributes specific for saving"),
		(mr_ud_set_t, ud_set, , "set union discriminator indexes"),
		(mr_idx_t, next_untyped, , "linked list of nodes with same pointer"),
		)

#define MR_DATA_UDO						\
  (mr_ud_override_t[]) {					\
    { MR_TYPE_STRING, "data" },					\
    { MR_TYPE_CHAR, "data" },					\
    { MR_TYPE_BOOL, "data" },					\
    { MR_TYPE_INT8, "data" },					\
    { MR_TYPE_UINT8, "data" },					\
    { MR_TYPE_INT16, "data" },					\
    { MR_TYPE_UINT16, "data" },					\
    { MR_TYPE_INT32, "data" },					\
    { MR_TYPE_UINT32, "data" },					\
    { MR_TYPE_INT64, "data" },					\
    { MR_TYPE_UINT64, "data" },					\
    { MR_TYPE_INT128, "data" },					\
    { MR_TYPE_UINT128, "data" },				\
    { MR_TYPE_FLOAT, "data" },					\
    { MR_TYPE_COMPLEX_FLOAT, "data" },				\
    { MR_TYPE_DOUBLE, "data" },					\
    { MR_TYPE_COMPLEX_DOUBLE, "data" },				\
    { MR_TYPE_LONG_DOUBLE, "data" },				\
    { MR_TYPE_COMPLEX_LONG_DOUBLE, "data" },			\
    { MR_TYPE_STRUCT, "data" },					\
    { MR_TYPE_ENUM, "data" },					\
  }

TYPEDEF_STRUCT (mr_ptrdes_t, ATTRIBUTES ( , "pointer descriptor type"),
		(mr_fd_t *, fdp, , "serializable field descriptor"),

		(mr_type_t, mr_type, , "Metaresc type"),
		(mr_type_t, mr_type_aux, , "Metaresc type if field is a pointer on builtin types or bit-field"),
		(mr_ptrdes_flags_t, flags, , "packed flags"),
		(mr_value_type_t, value_type, , "value type for load_params"),
		(mr_size_t, MR_SIZE, , "size of 'data' resizable array"),

		(mr_idx_t, idx, , "public index"),
		(mr_idx_t, parent, , "parent index"),
		(mr_idx_t, next, , "next sibling index"),
		(mr_idx_t, first_child, , "first child index"),

		ANON_UNION (),
		(void *, _data_, , "by default try to resolve pointer as void *"),
		(mr_ptr_t, data, , "fdp"), /* serialize for subset of mr_type */
		END_ANON_UNION ("mr_type", { MR_DATA_UDO }, "mr_ud_override_t", sizeof (MR_DATA_UDO)),

		ANON_UNION (),
		(uint8_t, default_serialization, [0]),
		(mr_save_params_t, save_params, , "attributes specific for saving"),
		(mr_load_params_t, load_params, , "value_type"),
		(mr_typed_ptr_t, res, , "extra pointer for user data"),
		END_ANON_UNION ("ptrdes_type"),
		)

TYPEDEF_ENUM (mr_ptrdes_type_t,
	      (MR_PD_UNINITIALIZED, = 0, "default_serialization"),
	      (MR_PD_SAVE, , "save_params"),
	      (MR_PD_LOAD, , "load_params"),
	      (MR_PD_CUSTOM, , "res"),
	      )

TYPEDEF_STRUCT (mr_ra_ptrdes_t, ATTRIBUTES ( , "mr_ptrdes_t resizable array"),
		(mr_ptrdes_t *, ra, , "resizable array with descriptors of saved elements",
		{ .offset = offsetof (mr_ra_ptrdes_t, size) }, "offset"),
		(ssize_t, size, , "size of resizable array"),
		VOID (ssize_t, alloc_size, , "allocated size of resizable array"),
		(mr_idx_t, last_child, , "last added child index"),
		(mr_ptrdes_type_t, ptrdes_type, , "discriminator for anonymous union in mr_ptrdes_t"),
		(mr_res_t, res, , "extra pointer for user data"),
		)

TYPEDEF_ENUM (mr_dfs_order_t,
	      MR_DFS_PRE_ORDER,
	      MR_DFS_POST_ORDER,
	      )

TYPEDEF_FUNC (mr_status_t, mr_ptrdes_processor_t, (mr_ra_ptrdes_t * /* ptrs */, mr_idx_t /* idx */, int /* level */, mr_dfs_order_t /* order */, mr_ptr_t /* context */))

TYPEDEF_STRUCT (mr_save_data_t, ATTRIBUTES ( , "save routines data and lookup structures"),
		(mr_ra_ptrdes_t, ptrs, , "internal representation of a saved tree"),
		(mr_ic_t, untyped_ptrs, , "index over untyped nodes"),
		(mr_ic_t, union_discriminators, , "index over all union discriminators"),
		(ssize_t, mr_ra_ud_size, , "size of 'mr_ra_ud'"),
		(mr_union_discriminator_t *, mr_ra_ud, , "allocation of union discriminators",
		{ .offset = offsetof (mr_save_data_t, mr_ra_ud_size) }, "offset"),
		VOID (ssize_t, mr_ra_ud_alloc_size, , "allocated size of 'mr_ra_ud'"),
		)

TYPEDEF_FUNC (int, mr_ra_printf_t, (mr_rarray_t *, mr_ptrdes_t *))

TYPEDEF_FUNC (void, mr_msg_handler_t, (const char *, const char *, int, mr_log_level_t, mr_message_id_t, va_list), ATTRIBUTES ( , "handler for error messages"))

TYPEDEF_STRUCT (mr_lloc_t, ATTRIBUTES ( , "parser location"),
		(int, lineno, , "parser location - line number"),
		(int, column, , "parser location - column number"),
		(int, offset, , "parser location - offset in string"),
		)

TYPEDEF_STRUCT (mr_load_t, ATTRIBUTES ( , "Metaresc load parser data"),
		(mr_ra_ptrdes_t *, ptrs, , "resizable array with mr_ptrdes_t"),
		(char *, str, , "string to parse"),
		(char *, buf, , "parser internal buffer"),
		(mr_lloc_t, lloc, , "current location of parser"),
		(mr_idx_t, parent, , "index of current parent"),
		)

TYPEDEF_STRUCT (mr_get_struct_type_name_t, ATTRIBUTES ( , "long jump buffer and type name"),
		VOID (jmp_buf, _jmp_buf, , "long jump buffer"),
		(char *, type_name, , "type name"),
		)

TYPEDEF_STRUCT (mr_dump_struct_type_ctx_t, ATTRIBUTES ( , "context for type detection with __builtin_dump_struct"),
		VOID (jmp_buf, _jmp_buf, , "long jump buffer"),
		(void *, struct_ptr, , "pointer on a sample struct variable"),
		(mr_td_t *, tdp, , "memory for type desctiptor"),
		(char *, type, , "embeded struct/union type"),
		(char *, name, , "embeded struct/union name"),
		(int, offset_byte, , "which byte of offset is set in struct"),
		(int, field_idx, , "index of the next detected field for structures bigger then (1 << __CHAR_BIT__)"),
		)

TYPEDEF_STRUCT (mr_var_t, ATTRIBUTES ( , "record to identify type of serialize expression"),
		(char *, filename, , "source file name"),
		(char *, varname, , "unique variable name to identify type of serialized expression"),
		(char *, type, , "type of expression"),
		)

TYPEDEF_STRUCT (mr_dwarf_t, ATTRIBUTES ( , "record to identify type of serialize expression"),
		(mr_td_t **, tdps, , "array of pointers on type descriptors", { .offset = offsetof (mr_dwarf_t, tdps_size), }, "offset"),
		(size_t, tdps_size, , "size of tdps array"),
		(mr_var_t **, vars, , "variables types", { .offset = offsetof (mr_dwarf_t, vars_size), }, "offset"),
		(size_t, vars_size, , "size of vars array"),
		(mr_dwarf_t *, next, , "linked list"),
		)

TYPEDEF_STRUCT (mr_conf_t, ATTRIBUTES ( , "Metaresc configuration"),
		(mr_mem_t, mr_mem, , "memory operations"),
		(mr_log_level_t, log_level),
		(bool, cache_func_resolve, , "global knob to enable dladdr caching"),
		(mr_td_t *, td_list, , "linked list of all type descriptors"),
		(mr_dwarf_t *, dwarf_list, , "linked list of all DWARF detected types"),
		(mr_msg_handler_t, msg_handler),
		(mr_ic_t, var_types, , "index over uniquely named variables to identify type"),
		(mr_ic_t, enum_by_name, , "index over all enum names"),
		(mr_ic_t, type_by_name, , "index over types descriptors"),
		(mr_ic_t, field_by_name, , "index of all fields names"),
		(mr_ic_t, field_by_name_and_type, , "index of all fields by name and type"),
		(mr_ra_printf_t, output_format, [MR_TYPE_LAST], "formaters"),
		)

