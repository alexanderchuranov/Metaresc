/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <metaresc.h>

TYPEDEF_UNION (mr_ptr_t, ATTRIBUTES (__attribute__((transparent_union)), "pointer on any type"),
	       (void *, ptr, , , { MR_SIZE_STR }, "string", .unnamed = true),
	       (void *, MR_OPAQUE_DATA, , , { MR_SIZE_STR }, "string"),
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
	      (MR_MESSAGE_UNBALANCED_TAGS, , "Unbalanced tags."),
	      (MR_MESSAGE_XDR_WRONG_ENCODING_MODE, , "XDR wrong encoding mode."),
	      (MR_MESSAGE_XDR_SAVE_FAILED, , "XDR save to resizable array failed."),
	      (MR_MESSAGE_XML_SAVE_FAILED, , "XML save failed."),
	      (MR_MESSAGE_LOAD_STRUCT_FAILED, , "Structure load failed."),
	      (MR_MESSAGE_LOAD_METHOD_MISSED, , "Load method missed."),
	      (MR_MESSAGE_XDR_LOAD_FAILED, , "XDR load failed."),
	      (MR_MESSAGE_NO_TYPE_DESCRIPTOR, , "Can't find type descriptor for '%s'."),
	      (MR_MESSAGE_TYPE_NOT_MATCHED, , "Field '%s' of type '%s' mismatched mr_type %d:%d != %d."),
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
	      (MR_MESSAGE_UNSUPPORTED_NODE_TYPE, , "Unsupported node type: %d."),
	      (MR_MESSAGE_RANGE_CHECK, , "Range check error in array '%s'."),
	      (MR_MESSAGE_READ_REF, , "Can't read '%s' as index."),
	      (MR_MESSAGE_SAVE_ENUM, , "Can't find enum name for value %" SCNu64 " type '%s' field '%s'."),
	      (MR_MESSAGE_CONFLICTED_ENUMS, , "Literal enum `%s` has a different value in types '%s' (%" SCNu64") and '%s' (%" SCNu64 ")."),
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
	      (MR_MESSAGE_CANT_READ_PROPERTY, , "Can't read %s property."),
	      (MR_MESSAGE_WRONG_SIZE_FOR_DYNAMIC_ARRAY, , "Wrong size (%zd) for dynamics array."),
	      (MR_MESSAGE_UNEXPECTED_MR_TYPE, , "Unexpected mr_type for serialized node."),
	      (MR_MESSAGE_UNEXPECTED_NUMBER_OF_ITEMS, , "Unexpected number of items in static array collection %d."),
	      (MR_MESSAGE_FIELD_NOT_FOUND, , "Field '%s' is not found in type '%s'."),
	      (MR_MESSAGE_LAST, , "Last message ID."),
	      )

TYPEDEF_ENUM (mr_type_t, ATTRIBUTES (__attribute__ ((packed)) , "Metaresc types"),
	      (MR_TYPE_NONE, = 0),
	      MR_TYPE_STRING,
	      MR_TYPE_CHAR_ARRAY,
	      MR_TYPE_CHAR,
	      MR_TYPE_VOID,
	      MR_TYPE_BOOL,
	      (MR_TYPE_INT8, , "_signed"),
	      (MR_TYPE_UINT8, , "_unsigned"),
	      (MR_TYPE_INT16, , "_signed"),
	      (MR_TYPE_UINT16, , "_unsigned"),
	      (MR_TYPE_INT32, , "_signed"),
	      (MR_TYPE_UINT32, , "_unsigned"),
	      (MR_TYPE_INT64, , "_signed"),
	      (MR_TYPE_UINT64, , "_unsigned"),
	      MR_TYPE_FLOAT,
	      MR_TYPE_COMPLEX_FLOAT,
	      MR_TYPE_DOUBLE,
	      MR_TYPE_COMPLEX_DOUBLE,
	      MR_TYPE_LONG_DOUBLE,
	      MR_TYPE_COMPLEX_LONG_DOUBLE,
	      MR_TYPE_STRUCT,
	      (MR_TYPE_ENUM, , "enum_param"),
	      MR_TYPE_FUNC_TYPE,
	      (MR_TYPE_FUNC, , "func_param"),
	      (MR_TYPE_BITFIELD, , "bitfield_param"),
	      (MR_TYPE_ARRAY, , "array_param"),
	      (MR_TYPE_POINTER, , "pointer_param"),
	      (MR_TYPE_UNION, , "union_param"),
	      (MR_TYPE_ANON_UNION, , "union_param"),
	      (MR_TYPE_NAMED_ANON_UNION, , "union_param"),
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
	      /* MR_LANG_TYPE_CLASS, */
	      )
    
TYPEDEF_ENUM (mr_sign_t, (PLUS, = 0), (MINUS, = 1))

#define IEEE_754_HALF_MANTISSA (10)
#define IEEE_754_HALF_EXPONENT (5)
  
TYPEDEF_STRUCT (ieee_754_half_t, ATTRIBUTES (__attribute__ ((packed))),
		BITFIELD (uint32_t, mantissa, : IEEE_754_HALF_MANTISSA),
		BITFIELD (uint32_t, exponent, : IEEE_754_HALF_EXPONENT),
		BITFIELD (mr_sign_t, sign, : 1))

TYPEDEF_STRUCT (ieee_754_half_nan_t, ATTRIBUTES (__attribute__ ((packed))),
		BITFIELD (uint32_t, mantissa, : IEEE_754_HALF_MANTISSA - 1),
		BITFIELD (bool, quiet_nan, : 1),
		BITFIELD (uint32_t, exponent, : IEEE_754_HALF_EXPONENT),
		BITFIELD (mr_sign_t, sign, : 1))

TYPEDEF_ENUM (ieee_754_half_nan_enum_t, (IEEE_754_HALF_NAN_ENUM_T, = (1 << IEEE_754_HALF_EXPONENT) - 1, "ieee_754_half_nan"))

TYPEDEF_UNION (ieee_half_t, ATTRIBUTES (__attribute__ ((packed))),
	       ANON_UNION ( , __attribute__ ((packed))),
	       (ieee_754_half_t, ieee_half_float),
	       (ieee_754_half_nan_t, ieee_754_half_nan),
	       END_ANON_UNION ("is_nan"),
	       BITFIELD (ieee_754_half_nan_enum_t, is_nan, : IEEE_754_HALF_EXPONENT, ,
			 .offset = IEEE_754_HALF_MANTISSA / __CHAR_BIT__,
			 .param = { .bitfield_param = {
			     .bitfield = NULL, .size = 0,
			     .width = IEEE_754_HALF_EXPONENT,
			     .shift = IEEE_754_HALF_MANTISSA % __CHAR_BIT__,
			     .initialized = true, }, } ),
	       uint16_t uint16,
	       )

#define IEEE_754_FLOAT_MANTISSA (23)
#define IEEE_754_FLOAT_EXPONENT (8)

TYPEDEF_STRUCT (ieee_754_float_t,
		BITFIELD (uint32_t, mantissa, : IEEE_754_FLOAT_MANTISSA),
		BITFIELD (uint32_t, exponent, : IEEE_754_FLOAT_EXPONENT),
		BITFIELD (mr_sign_t, sign, : 1))

TYPEDEF_STRUCT (ieee_754_float_nan_t,
		BITFIELD (uint32_t, mantissa, : IEEE_754_FLOAT_MANTISSA - 1),
		BITFIELD (bool, quiet_nan, : 1),
		BITFIELD (uint32_t, exponent, : IEEE_754_FLOAT_EXPONENT),
		BITFIELD (mr_sign_t, sign, : 1))

TYPEDEF_ENUM (ieee_754_float_nan_enum_t, (IEEE_754_FLOAT_NAN_ENUM_T, = (1 << IEEE_754_FLOAT_EXPONENT) - 1, "ieee_754_float_nan"))

TYPEDEF_UNION (ieee_float_t,
	       ANON_UNION (),
	       (ieee_754_float_t, ieee_754_float),
	       (ieee_754_float_nan_t, ieee_754_float_nan),
	       END_ANON_UNION ("is_nan"),
	       BITFIELD (ieee_754_float_nan_enum_t, is_nan, : IEEE_754_FLOAT_EXPONENT, ,
			 .offset = IEEE_754_FLOAT_MANTISSA / __CHAR_BIT__,
			 .param = { .bitfield_param = {
			     .bitfield = NULL, .size = 0,
			     .width = IEEE_754_FLOAT_EXPONENT,
			     .shift = IEEE_754_FLOAT_MANTISSA % __CHAR_BIT__,
			     .initialized = true, }, } ),
	       float _float,
	       uint32_t uint32,
	       )

#define IEEE_754_DOUBLE_MANTISSA (52)
#define IEEE_754_DOUBLE_EXPONENT (11)

TYPEDEF_STRUCT (ieee_754_double_t,
		BITFIELD (uint64_t, mantissa, : IEEE_754_DOUBLE_MANTISSA),
		BITFIELD (uint32_t, exponent, : IEEE_754_DOUBLE_EXPONENT),
		BITFIELD (mr_sign_t, sign, : 1))

TYPEDEF_STRUCT (ieee_754_double_nan_t,
		BITFIELD (uint64_t, mantissa, : IEEE_754_DOUBLE_MANTISSA - 1),
		BITFIELD (bool, quiet_nan, : 1),
		BITFIELD (uint32_t, exponent, : IEEE_754_DOUBLE_EXPONENT),
		BITFIELD (mr_sign_t, sign, : 1))

  TYPEDEF_ENUM (ieee_754_double_nan_enum_t, (IEEE_754_DOUBLE_NAN_ENUM_T, = (1 << IEEE_754_DOUBLE_EXPONENT) - 1, "ieee_754_double_nan"))

TYPEDEF_UNION (ieee_double_t,
	       ANON_UNION (),
	       (ieee_754_double_t, ieee_754_double),
	       (ieee_754_double_nan_t, ieee_754_double_nan),
	       END_ANON_UNION ("is_nan"),
	       BITFIELD (ieee_754_double_nan_enum_t, is_nan, : IEEE_754_DOUBLE_EXPONENT, ,
			 .offset = IEEE_754_DOUBLE_MANTISSA / __CHAR_BIT__,
			 .param = { .bitfield_param = {
			     .bitfield = NULL, .size = 0,
			     .width = IEEE_754_DOUBLE_EXPONENT,
			     .shift = IEEE_754_DOUBLE_MANTISSA % __CHAR_BIT__,
			     .initialized = true, }, } ),
	       double _double,
	       uint64_t uint64,
	       )

#define IEEE_854_LONG_DOUBLE_MANTISSA (64)
#define IEEE_854_LONG_DOUBLE_EXPONENT (15)

TYPEDEF_STRUCT (ieee_854_long_double_t, ATTRIBUTES (__attribute__ ((packed))),
		(uint64_t, mantissa),
		BITFIELD (uint32_t, exponent, : IEEE_854_LONG_DOUBLE_EXPONENT),
		BITFIELD (mr_sign_t, sign, : 1),
		)

TYPEDEF_STRUCT (ieee_854_long_double_nan_t, ATTRIBUTES (__attribute__ ((packed))),
		BITFIELD (uint64_t, mantissa, : IEEE_854_LONG_DOUBLE_MANTISSA - 2),
		BITFIELD (bool, quiet_nan, : 1),
		BITFIELD (unsigned int, one, : 1),
		BITFIELD (uint32_t, exponent, : IEEE_854_LONG_DOUBLE_EXPONENT),
		BITFIELD (mr_sign_t, sign, : 1),
		)

TYPEDEF_ENUM (ieee_854_long_double_nan_enum_t,
	      (IEEE_854_LONG_DOUBLE_NAN_ENUM_T, = (1 << IEEE_854_LONG_DOUBLE_EXPONENT) - 1, "ieee_854_long_double_nan"),
	      (IEEE_854_LONG_DOUBLE_NAN_ZERO_ONE_ENUM_T, = ((1 << IEEE_854_LONG_DOUBLE_EXPONENT) - 1) ^ (1 << 1), "ieee_854_long_double_nan"),
	      )

TYPEDEF_UNION (ieee_long_double_t,
	       ANON_UNION (),
	       (ieee_854_long_double_t, ieee_854_long_double),
	       (ieee_854_long_double_nan_t, ieee_854_long_double_nan),
	       END_ANON_UNION ("is_nan"),
	       BITFIELD (ieee_854_long_double_nan_enum_t, is_nan, : IEEE_854_LONG_DOUBLE_EXPONENT, ,
			 .offset = IEEE_854_LONG_DOUBLE_MANTISSA / __CHAR_BIT__,
			 .param = { .bitfield_param = {
			     .bitfield = NULL, .size = 0,
			     .width = IEEE_854_LONG_DOUBLE_EXPONENT,
			     .shift = IEEE_854_LONG_DOUBLE_MANTISSA % __CHAR_BIT__,
			     .initialized = true, }, } ),
	       long double long_double,
	       )

TYPEDEF_STRUCT (mr_complex_long_double_t, ATTRIBUTES ( , "complex long double packed to 20 bytes"),
		(ieee_854_long_double_t, real, , "__real__ complex long double"),
		(ieee_854_long_double_t, imag, , "__imag__ complex long double"),
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

TYPEDEF_STRUCT (mr_ud_override_t, ATTRIBUTES ( , "key value pair for union discriminator override"),
		(uint64_t, value, , "discriminator value that has a custom override"),
		(char *, discriminator, , "name of union branch for this value"),
		(struct mr_fd_t *, fdp, , "descriptor of union branch"),
		)

TYPEDEF_UNION (mr_pointer_param_t, ATTRIBUTES ( , "pointer parameters"),
		VOID (void *, default_serialization),
		(struct mr_ic_t *, union_param, , "IC for union discriminator overrides"),
		)

TYPEDEF_STRUCT (mr_array_param_t, ATTRIBUTES ( , "array parameters"),
		ANON_UNION (),
		VOID (void *, default_serialization),
		(struct mr_fd_t *, pointer_param, , "statically allocated field descriptor for array of pointers case"),
		(struct mr_ic_t *, union_param, , "IC for union discriminator overrides"),
		END_ANON_UNION ("mr_type_aux"),
		(unsigned int, count, , "array size"),
		(unsigned int, row_count, , "row size"),
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
		(struct mr_fd_t *, args, , "function arguments descriptors saved as resizable array of field descriptors",
		{ .offset = offsetof (mr_func_param_t, size) }, "offset"), 
		(size_t, size, , "size of args array"),
		)

TYPEDEF_UNION (mr_enum_value_t, ATTRIBUTES ( , "signed/unsigned value of the enum"),
	       VOID (uint64_t, default_serialization),
	       (uint64_t, _unsigned),
	       (int64_t, _signed),
	       )

TYPEDEF_FUNC (int, mr_compar_fn_t, (__const mr_ptr_t /* x */, __const mr_ptr_t /* y */, __const void * /* context */))

TYPEDEF_FUNC (mr_status_t, mr_visit_fn_t, (mr_ptr_t /* nodep */, __const void * /* context */))

TYPEDEF_FUNC (mr_hash_value_t, mr_hash_fn_t, (mr_ptr_t /* nodep */, __const void * /* context */))

TYPEDEF_STRUCT (mr_hashed_string_t, ATTRIBUTES (__attribute__ ((packed)) , "hashed string"),
		(char *, str, , "key field"),
		(mr_hash_value_t, hash_value, , "hash value of 'str'"),
		)

TYPEDEF_ENUM (mr_ic_type_t, ATTRIBUTES ( , "types of indexed collections"),
	      (MR_IC_UNINITIALIZED, = 0, "void_ptr"),
	      (MR_IC_UNSORTED_ARRAY, , "rarray"),
	      (MR_IC_SORTED_ARRAY, , "rarray"),
	      (MR_IC_HASH, , "hash"),
	      (MR_IC_STATIC_ARRAY, , "static_array"),
	      (MR_IC_RBTREE, , "rb"),
	      (MR_IC_AVLTREE, , "avl"),
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
		BITFIELD (size_t, size, : sizeof (size_t) * __CHAR_BIT__ - 1, "size of hash table"),
		BITFIELD (bool, zero_key, : 1),
		)

TYPEDEF_STRUCT (mr_ic_static_array_t, ATTRIBUTES ( , "indexed collection for small sets"),
		(mr_ptr_t, static_array, [sizeof (mr_ic_hash_t) / sizeof (mr_ptr_t)], "key_type"),
		)

TYPEDEF_ENUM (mr_child_idx_t, ATTRIBUTES (__attribute__ ((packed)), "tree traverse to the left/right"),
	      (MR_LEFT, = 0),
	      (MR_RIGHT, = 1),
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

TYPEDEF_STRUCT (mr_tree_t, ATTRIBUTES ( , "indexed collection for binary tree"),
		(mr_tree_node_t *, pool, , "mr_tree_node_t allocation pool", { .offset = offsetof (mr_tree_t, size) }, "offset"),
		(ssize_t, size),
		VOID (ssize_t, alloc_size),
		)

TYPEDEF_STRUCT (mr_tree_path_t, ATTRIBUTES ( , "element of traverse index and discent direction"), 
		(mr_child_idx_t, child_idx, , "descent direction"),
		(unsigned int, idx, , "index in the pool"),
		)

TYPEDEF_STRUCT (mr_tree_traverse_t, ATTRIBUTES ( , "tree traverse and zero flag for the last comparison"),
		(unsigned int, size, , "size of tree traverse"),
		(bool, equal, , "equal flag for the last comparison in the traverse"),
		(mr_tree_path_t, path, [(sizeof (mr_tree_node_idx_t) * __CHAR_BIT__ << 1) - 1], "tree traverse path"),
		)

TYPEDEF_STRUCT (mr_res_t,
		(mr_ptr_t, data, , "type"), 
		(char *, type, , "union discriminator"),
		(ssize_t, MR_SIZE, , "size of data"),
		)

TYPEDEF_STRUCT (mr_ic_t, ATTRIBUTES ( , "indexed collection"),
		(mr_res_t, context),
		(mr_ic_type_t, ic_type),
		(unsigned int, items_count),
		(char *, key_type),
		(mr_compar_fn_t, compar_fn),
		(struct mr_ic_virt_func_t *, virt_func),
		
		ANON_UNION (),
		VOID (void *, void_ptr, , "default serialization"),
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
		(mr_status_t, foreach, (mr_ic_t * /* ic */, mr_visit_fn_t /* visit_fn */, __const void * /* context */)),
		(mr_status_t, index, (mr_ic_t * /* ic */, mr_ic_rarray_t * /* rarray */)),
		(void, free, (mr_ic_t * /* ic */)),
		)

TYPEDEF_UNION (mr_fd_param_t, ATTRIBUTES ( , "optional parameters for different types"),
	       VOID (uint8_t, default_serialization, , "default serialization is empty"),
	       (mr_array_param_t, array_param, , "array parameters"),
	       (mr_enum_value_t, enum_param, , "mr_type_aux"),
	       (mr_bitfield_param_t, bitfield_param, , "bit-field parameters"),
	       (mr_func_param_t, func_param, , "types of function arguments"),
	       (mr_pointer_param_t, pointer_param, , "mr_type_aux"),
	       (mr_ic_t *, union_param, , "indexed collection with union descriminators overrides"),
	       )

TYPEDEF_STRUCT (mr_fd_t, ATTRIBUTES ( , "Metaresc field descriptor"),
		(struct mr_td_t *, tdp, , "type descriptor"),
		(char *, type, , "stringified type name"),
		(mr_hashed_string_t, name, , "hashed name of the field"),
		(mr_type_t, mr_type, , "Metaresc type"),
		(mr_type_t, mr_type_aux, , "Metaresc type if field is a pointer on builtin types or bit-field"),
		(mr_type_t, mr_type_ptr, , "Metaresc type to detect pointers on basic type"),
		BITFIELD (bool, self_ptr, : 1, "true if field is a pointer on a base type"),
		BITFIELD (bool, non_persistent, : 1, "true if field descriptor is allocated on stack"),
		BITFIELD (bool, unnamed, : 1, "by default all fields are named, but anonymous unions and fields in mr_ptr_t should be unnamed"),
		BITFIELD (mr_type_class_t, mr_type_class, : 5, "required to distinguish records and unions from scalar types"),
		(mr_offset_t, offset, , "offset in structure"),
		(mr_size_t, size, , "size of field"),
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
		  VOID (void *, ptr, , "user extended info", { "one more extra string" }, "string")
		*/
		(mr_ptr_t, res, , "res_type"), /* extra pointer for user data */
		(char *, res_type, , "union discriminator"),
		(ssize_t, MR_SIZE, , "size of array pointed by 'res'"),
		)

TYPEDEF_STRUCT (mr_fd_ptr_t, ATTRIBUTES ( , "wrapper for mr_fd_t*"),
		(mr_fd_t *, fdp, , "wrapper for mr_fd_t pointer type"),
		)

TYPEDEF_STRUCT (mr_enum_param_t,
		(uint8_t, size_effective, , "effective size"),
		BITFIELD (mr_type_t, mr_type_effective, : __CHAR_BIT__ - 1, "automatic type detection is required for enums size adjustment"),
		BITFIELD (bool, is_bitmask, : 1, "set to true if all enum values are power of 2"),
		)

TYPEDEF_UNION (mr_td_param_t,
	       VOID (uint8_t, default_serialization, , "default serialization"),
	       (mr_enum_param_t, enum_param, , "parameters specific for enums"),
	       )

TYPEDEF_STRUCT (mr_td_t, ATTRIBUTES ( , "Metaresc type descriptor"),
		(mr_hashed_string_t, type, , "hashed name of the type"),
		(mr_type_t, mr_type, , "Metaresc type"), /* possible variants MR_TYPE_ENUM, MR_TYPE_STRUCT, MR_TYPE_UNION */
		(bool, is_dynamically_allocated, , "mark types that require free at exit"),
		(mr_td_param_t, param, , "mr_type"),
		(mr_size_t, size, , "size of type"),
		(mr_ic_t, field_by_name, , "lookup by field names"),
		(mr_fd_ptr_t *, fields, , "fields or enums descriptors", { .offset = offsetof (mr_td_t, fields_size) }, "offset"),
		(ssize_t, fields_size, , "size of 'fields' array"),
		(char *, meta, , "type meta info"),
		(mr_ptr_t, res, , "res_type"), /* extra pointer for user data */
		(char *, res_type, , "union discriminator"),
		(ssize_t, MR_SIZE, , "size of array pointed by 'res'"),
		) /* type descriptor */

TYPEDEF_STRUCT (mr_mem_t, ATTRIBUTES ( , "Metaresc memory operations"),
		(void *, calloc, (const char *, const char *, int, size_t, size_t), "pointer on malloc() function"),
		(void *, realloc, (const char *, const char *, int, void *, size_t), "pointer on realloc() function"),
		(void, free, (const char *, const char *, int, void *), "pointer on free() function"),
		)

TYPEDEF_STRUCT (mr_ptrdes_flags_t, ATTRIBUTES (__attribute__ ((packed)), "ponter descriptor flag bitfield values"),
		BITFIELD (bool, is_null, : 1, "is a null pointer"),
		BITFIELD (bool, is_referenced, : 1, "pointer is already serialized somewhere else"),
		BITFIELD (bool, is_content_reference, : 1, "pointers on string content may reffer only on string pointer"),
		BITFIELD (bool, is_opaque_data, : 1, "opaque serialization of resizeable pointer to XDR"),
		)

TYPEDEF_STRUCT (mr_union_discriminator_t, ATTRIBUTES ( , "cache for union discriminator resolution"),
		(mr_fd_t *, union_fdp, , "union field descriptor"),
		(mr_fd_t *, discriminated_fdp, , "discriminated union field descriptor"),
		)

TYPEDEF_STRUCT (mr_substr_t, ATTRIBUTES (__attribute__ ((packed)), "substring"),
		POINTER (char, str, "pointer on substring", { .offset = offsetof (mr_substr_t, length) }, "offset"),
		(unsigned int, length, , "length of the substring"),
		)

TYPEDEF_STRUCT (mr_quoted_substr_t, ATTRIBUTES (__attribute__ ((packed)), "quoted substring"),
		(void, unquote, (mr_substr_t *, char *), "unquote function"),
		(mr_substr_t, substr, , "substring pointer and length"),
		)

TYPEDEF_FUNC (mr_status_t, mr_process_quoted_str_t, (char * /* src */, void * /* arg */))

TYPEDEF_ENUM (mr_value_type_t, ATTRIBUTES ( , "type of values from lexer"),
	      (MR_VT_VOID, = 0, "vt_void"),
	      (MR_VT_CHAR, , "vt_char"),
	      (MR_VT_QUOTED_SUBSTR, , "vt_quoted_substr"),
	      (MR_VT_ID, , "vt_quoted_substr"),
	      (MR_VT_INT, , "vt_int"),
	      (MR_VT_FLOAT, , "vt_float"),
	      (MR_VT_COMPLEX, , "vt_complex"),
	      )

TYPEDEF_STRUCT (mr_value_t, ATTRIBUTES ( , "value for expressions calculation"),
		(mr_value_type_t, value_type),
		ANON_UNION ( , __attribute__ ((packed))),
		VOID (uint8_t, default_serialization, , "no serialization by default"),
		(mr_complex_long_double_t, vt_complex),
		(mr_quoted_substr_t, vt_quoted_substr),
		long long int vt_int,
		long double vt_float,
		string_t vt_string,
		char vt_char,
		END_ANON_UNION ("value_type"),
		)

TYPEDEF_STRUCT (mr_load_params_t, ATTRIBUTES ( , "attributes specific for loading"),
		(mr_value_t, mr_value, , "loaded value"),
		)

TYPEDEF_STRUCT (mr_save_params_t, ATTRIBUTES ( , "attributes specific for saving"),
		ANON_UNION (),
		(int, static_array, [sizeof (mr_load_params_t) / sizeof (int) - 2], "in place list of union discriminators"),
		(mr_ic_t *, union_discriminator, , "index over unions discriminator"),
		END_ANON_UNION ("ud_as_ic"),
		(int, next_typed, , "linked list of nodes with same type and pointer"),
		(int, next_untyped, , "linked list of nodes with same pointer"),
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
		(mr_td_t *, tdp, , "type descriptor"),
		(char *, name, , "name of the field"),
		(mr_type_t, mr_type, , "Metaresc type"),
		(mr_type_t, mr_type_aux, , "Metaresc type if field is a pointer on builtin types or bit-field"),
		(mr_ptrdes_flags_t, flags, , "packed flags"),
		BITFIELD (bool, unnamed, : 1, "for CINIT serialization all fields are named, but anonymous union and arrays elements don't need name"),
		BITFIELD (bool, non_persistent, : 1, "true if field descriptor is allocated on stack"),
		BITFIELD (bool, ud_is_ic, : 1, "true if union discriminator is an indexed collection"),
		BITFIELD (unsigned, ud_count, : 3, "size of union discriminator in place list"),
		(int32_t, idx, , "public index"),
		(int32_t, ref_idx, , "reference index (internal enumeration)"),
		(int, parent, , "parent index"),
		(int, first_child, , "first child index"),
		(int, last_child, , "last child index"),
		(int, prev, , "previous sibling index"),
		(int, next, , "next sibling index"),
		(ssize_t, MR_SIZE, , "size of 'data' resizable array"),
		ANON_UNION (),
		(void *, _data_, , "by default try to resolve pointer as void *"),
		(mr_ptr_t, data, , "tdp"), /* serialize for subset of mr_type */
		END_ANON_UNION ("mr_type", { MR_DATA_UDO }, "mr_ud_override_t", sizeof (MR_DATA_UDO)),
		ANON_UNION (),
		(mr_fd_t *, fdp, , "serializable field descriptor"),
		VOID (mr_fd_t *, fdp_, , "field descriptor on stack"),
		END_ANON_UNION ("non_persistent"),
		ANON_UNION (),
		VOID (uint8_t, default_serialization),
		(mr_save_params_t, save_params, , "attributes specific for saving"),
		(mr_load_params_t, load_params, , "attributes specific for loading"),
		(mr_res_t, res, , "extra pointer for user data"),
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
		(mr_ptrdes_type_t, ptrdes_type, , "discriminator for anonymous union in mr_ptrdes_t"),
		(mr_res_t, res, , "extra pointer for user data"),
		)

TYPEDEF_ENUM (mr_dfs_order_t,
	      MR_DFS_PRE_ORDER,
	      MR_DFS_POST_ORDER,
	      )

TYPEDEF_FUNC (mr_status_t, mr_ptrdes_processor_t, (mr_ra_ptrdes_t * /* ptrs */, int /* idx */, int /* level */, mr_dfs_order_t /* order */, mr_ptr_t /* context */))

TYPEDEF_STRUCT (mr_load_data_t, ATTRIBUTES ( , "state for objects loading"),
		(mr_ra_ptrdes_t, ptrs, , "internal representation of a loaded tree"),
		(ssize_t, mr_ra_idx_size, , "size of 'mr_ra_idx'"),
		(unsigned int *, mr_ra_idx, , "indexes of postponed nodes", { .offset = offsetof (mr_load_data_t, mr_ra_idx_size) }, "offset"),
		VOID (ssize_t, mr_ra_idx_alloc_size, , "allocated size of 'mr_ra_idx'"),
		)

TYPEDEF_STRUCT (mr_save_data_t, ATTRIBUTES ( , "save routines data and lookup structures"),
		(mr_ra_ptrdes_t, ptrs, , "internal representation of a saved tree"),
		(mr_ic_t, typed_ptrs, , "index over typed nodes"),
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
		(mr_lloc_t, lloc, , "current location of parser"),
		(char *, str, , "string to parse"),
		(char *, buf, , "parser internal buffer"),
		(int, parent, , "index of current parent"),
		(mr_ra_ptrdes_t *, ptrs, , "resizable array with mr_ptrdes_t"),
		)

TYPEDEF_STRUCT (mr_conf_t, ATTRIBUTES ( , "Metaresc configuration"),
		(mr_mem_t, mr_mem, , "memory operations"),
		(mr_log_level_t, log_level),
		(bool, cache_func_resolve, , "global knob to enable dladdr caching"),
		(mr_msg_handler_t, msg_handler),
		(mr_ic_t, enum_by_name, , "index over all enum names"),
		(mr_ic_t, fields_names, , "index of all fields names"),
		(mr_ic_t, type_by_name, , "index over types descriptors"),
		(mr_ra_printf_t, output_format, [MR_TYPE_LAST], "formaters"),
		)

