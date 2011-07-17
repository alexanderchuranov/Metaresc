/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

#include <reslib.h>

TYPEDEF_UNION (rl_ptr_t, ATTRIBUTES ( , "pointer on any type"),
		(void *, ptr, , "default void pointer"),
		)

TYPEDEF_CHAR_ARRAY (rl_char_array_t, 1)

TYPEDEF_ENUM (rl_log_level_t, ATTRIBUTES ( ,"Log levels enum"),
	      (RL_LL_ALL, = 0),
	      RL_LL_TRACE,
	      RL_LL_DEBUG,
	      RL_LL_INFO,
	      RL_LL_WARN,
	      RL_LL_ERROR,
	      RL_LL_FATAL,
	      RL_LL_OFF,
	      )

TYPEDEF_ENUM (rl_message_id_t, ATTRIBUTES ( , "Messages enum. Message string saved in .comment field of meta info"),
	      (RL_MESSAGE_OUT_OF_MEMORY, , "Out of memory."),
	      (RL_MESSAGE_UNEXPECTED_END, , "Unexpected end of file or end of string."),
	      (RL_MESSAGE_UNEXPECTED_DATA, , "Unexpected data in stream."),
	      (RL_MESSAGE_UNBALANCED_TAGS, , "Unbalanced tags."),
	      (RL_MESSAGE_XDR_WRONG_ENCODING_MODE, , "XDR wrong encoding mode."),
	      (RL_MESSAGE_XDR_SAVE_FAILED, , "XDR save to resizable array failed."),
	      (RL_MESSAGE_XML_SAVE_FAILED, , "XML save failed."),
	      (RL_MESSAGE_LOAD_STRUCT_FAILED, , "Structure load failed."),
	      (RL_MESSAGE_LOAD_METHOD_MISSED, , "Load method missed."),
	      (RL_MESSAGE_XDR_LOAD_FAILED, , "XDR load failed."),
	      (RL_MESSAGE_NO_TYPE_DESCRIPTOR, , "Can't find type descriptor for '%s'."),
	      (RL_MESSAGE_NOT_A_UNION, , "Type '%s' is not a union."),
	      (RL_MESSAGE_STRING_IS_NULL, , "Input string is NULL."),
	      (RL_MESSAGE_NULL_POINTER, , "Can't save/load to/from NULL pointer."),
	      (RL_MESSAGE_IDS_COLLISION, , "Two XML nodes have the same %s=%" SCNd32 "."),
	      (RL_MESSAGE_UNDEFINED_REF_IDX, , "Reference on a node with %s=%" SCNd32 " could not be found."),
	      (RL_MESSAGE_UNKNOWN_ENUM, , "Unknown enum value '%s'."),
	      (RL_MESSAGE_READ_INT, , "Can't read integer value from string `%s`."),
	      (RL_MESSAGE_READ_ENUM, , "Can't read enum value from string `%s`."),
	      (RL_MESSAGE_READ_BITMASK, , "Can't read bitmask from string `%s`."),
	      (RL_MESSAGE_READ_FLOAT, , "Can't read float from string `%s`."),
	      (RL_MESSAGE_READ_DOUBLE, , "Can't read double from string `%s`."),
	      (RL_MESSAGE_READ_LONG_DOUBLE, , "Can't read long double from string `%s`."),
	      (RL_MESSAGE_READ_CHAR, , "Can't read char from string `%s`."),
	      (RL_MESSAGE_STRING_TRUNCATED, , "Char array range check: string truncated."),
	      (RL_MESSAGE_UNKNOWN_SUBNODE, , "Struct type '%s': unknown subnode '%s'."),
	      (RL_MESSAGE_SAVE_IDX_RANGE_CHECK, , "Save node index is out of range."),
	      (RL_MESSAGE_NODE_NAME_MISSMATCH, , "Node name mismatch: expected '%s' got '%s'."),
	      (RL_MESSAGE_NODE_TYPE_MISSMATCH, , "Node type mismatch: expected '%s' got '%s'."),
	      (RL_MESSAGE_UNSUPPORTED_NODE_TYPE, , "Unsupported node type: %s(%d),:%s(%d)."),
	      (RL_MESSAGE_RANGE_CHECK, , "Range check error in array '%s'."),
	      (RL_MESSAGE_READ_REF, , "Can't read '%s' as index."),
	      (RL_MESSAGE_READ_RARRAY_SIZE, , "Can't read '%s' as resizable array size."),
	      (RL_MESSAGE_TYPE_NOT_ENUM, , "Type '%s' is not a enum."),
	      (RL_MESSAGE_SAVE_ENUM, , "Can't find enum name for value %" SCNd64 " type '%s'."),
	      (RL_MESSAGE_SAVE_BITMASK, , "Can't decompose on bitmask %llu."),
	      (RL_MESSAGE_DUPLICATED_ENUMS, , "Duplicated enum definition for `%s` in enum '%s'."),
	      (RL_MESSAGE_DUPLICATED_FIELDS, , "Duplicated field definition for `%s` in type '%s'."),
	      (RL_MESSAGE_INT_OF_UNKNOWN_SIZE, , "Failed to stringify integer of unknown size: %" SCNd32 "."),
	      (RL_MESSAGE_PARSE_ERROR, , "Parser error: '%s'. Position: %" SCNd32 ":%" SCNd32 "-%" SCNd32 ":%" SCNd32 "."),
	      (RL_MESSAGE_ANON_UNION_TYPE_ERROR, , "Can't create type for anonymous union due to type name collision: '%s'."),
	      (RL_MESSAGE_UNKNOWN_XML_ESC, , "Unknown XML escape sequence '%s'."),
	      (RL_MESSAGE_WRONG_XML_ESC, , "Wrong XML escape sequence '%s'."),
	      (RL_MESSAGE_TYPES_HASHES_MATCHED, , "For types '%s' and '%s' hashes matched. Last type is not available."),
	      (RL_MESSAGE_RARRAY_LOAD_FAILED, , "Failed to find field .data in C-style data."),
	      (RL_MESSAGE_LAST, , "Last message ID."),
	      )

TYPEDEF_ENUM (rl_type_t, ATTRIBUTES ( , "ResLib types"),
	      (RL_TYPE_NONE, = 0),
	      (RL_TYPE_STRING, = 1),
	      (RL_TYPE_CHAR_ARRAY, = 2),
	      (RL_TYPE_CHAR, = 3, "RL_TYPE_CHAR | RL_TYPE_STRING should be the same as RL_TYPE_CHAR for proper char* auto-detection"),
	      RL_TYPE_VOID,
	      RL_TYPE_INT8,
	      RL_TYPE_UINT8,
	      RL_TYPE_INT16,
	      RL_TYPE_UINT16,
	      RL_TYPE_INT32,
	      RL_TYPE_UINT32,
	      RL_TYPE_INT64,
	      RL_TYPE_UINT64,
	      RL_TYPE_FLOAT,
	      RL_TYPE_DOUBLE,
	      RL_TYPE_LONG_DOUBLE,
	      RL_TYPE_STRUCT,
	      RL_TYPE_ENUM,
	      RL_TYPE_FUNC_TYPE,
	      (RL_TYPE_ENUM_VALUE, , "enum_value"), /* comment refers to union member in rl_param_t */
	      (RL_TYPE_FUNC, , "func_param"),
	      (RL_TYPE_BITFIELD, , "bitfield_param"),
	      RL_TYPE_BITMASK,
	      RL_TYPE_UNION,
	      RL_TYPE_ANON_UNION,
	      RL_TYPE_END_ANON_UNION,
	      RL_TYPE_TRAILING_RECORD,
	      RL_TYPE_LAST, /* keep it last */
	      )

TYPEDEF_ENUM (rl_type_ext_t, ATTRIBUTES ( , "ResLib types extension"),
	      RL_TYPE_EXT_NONE,
	      RL_TYPE_EXT_ARRAY,
	      RL_TYPE_EXT_RARRAY,
	      RL_TYPE_EXT_POINTER,
	      RL_TYPE_EXT_LAST,  /* keep it last */
	      )

/* rl_rarray_t defenition should be syncroonized with RL_RARRAY_PROTO macro */
TYPEDEF_STRUCT (rl_rarray_t, ATTRIBUTES (__attribute__((packed)), "resizable array type"),
		(void *, data, , "pointer on data array"),
		(int32_t, size, , "used space in bytes"),
		(int32_t, alloc_size, , "allocated space in bytes"),
		(rl_ptr_t, ext, , "ptr_type"), /* extra pointer for user data */
		(char *, ptr_type, , "union discriminator"),
		)

TYPEDEF_ENUM (rl_red_black_t, ATTRIBUTES (__attribute__ ((packed,aligned(sizeof (unsigned char)))), "Red/Black enum"),
	      (RL_BLACK, = 0),
	      (RL_RED, = 1),
	      )

TYPEDEF_STRUCT (rl_red_black_tree_node_t, ATTRIBUTES ( , "red/black tree node"),
		(const void *, key, , "void pointer"),
		(rl_red_black_tree_node_t *, left, , "left child"),
		(rl_red_black_tree_node_t *, right, , "right child"),
		(rl_red_black_t, red),
		)

TYPEDEF_STRUCT (rl_array_param_t, ATTRIBUTES ( , "array parameters"),
		(int, count, , "array size"),
		(int, row_count, , "row size"),
		)

TYPEDEF_STRUCT (rl_bitfield_param_t, ATTRIBUTES ( , "bit-field parameters"),
		(int, width, , "bit-field width in bits"),
		(int, shift, , "bit-field shift in first byte"),
		RARRAY (uint8_t, bitfield, "zero-struct with flagged bit-fields"),
		)

TYPEDEF_UNION (rl_param_t, ATTRIBUTES ( , "optional parameters for different types"),
	       (rl_array_param_t, array_param, , "array parameters - default for serialization"),
	       (int64_t, enum_value, , "enum value"),
	       (rl_bitfield_param_t, bitfield_param, , "bit-field parameters"),
	       RARRAY (struct rl_fd_t, func_param, "function arguments descriptors"),
	       )

TYPEDEF_STRUCT (rl_fd_t, ATTRIBUTES ( , "ResLib field descriptor"),
		(char *, type, , "stringified type name"),
		(char *, name, , "name of the field"),
		(uint64_t, hash_value, , "hash of the name"),
		(int, size, , "size of field"),
		(int, offset, , "offset in structure"),
		(rl_type_t, rl_type, , "ResLib type"),
		(rl_type_t, rl_type_aux, , "ResLib type if field is a pointer on builtin types or bit-field"),
		(rl_type_ext_t, rl_type_ext, , "ResLib type extension"),
		(rl_param_t, param, , "rl_type"),
		(char *, comment, , "field comments"),
		/*
		  ext field can be used by user for extended information
		  placed after comments for tricky intialization
		  sample:
		  RL_NONE (void*, ext, , "user extended info", { &((ext_info_t){ .field = XXX }) }, "ext_info_t")
		  or
		  RL_NONE (void*, ext, , "user extended info", { (ext_info_t[]){ {.field = XXX}, {.field = YYY} } }, "ext_info_t")
		  or
		  RL_NONE (void*, ext, , "user extended info", { "one more extra string" }, "rl_char_array_t")
		*/
		(rl_ptr_t, ext, , "ptr_type"), /* extra pointer for user data */
		(char *, ptr_type, , "union discriminator"),
		)

TYPEDEF_STRUCT (rl_fd_ptr_t, ATTRIBUTES ( , "rl_fd_t pointer wrapper"),
		(rl_fd_t *, fdp, , "pointer on field descriptor"),
		)

TYPEDEF_STRUCT (rl_td_t, ATTRIBUTES ( , "ResLib type descriptor"),
		(rl_type_t, rl_type, , "ResLib type"), /* possible variants RL_TYPE_ENUM, RL_TYPE_STRUCT, RL_TYPE_UNION */
		(char *, type, , "stringified type name"),
		(rl_red_black_tree_node_t *, lookup_by_value, , "B-tree for enums values lookup"),
		RARRAY (rl_fd_ptr_t, lookup_by_name, "hash for lookup by field name"),
#ifndef RL_TREE_LOOKUP
		(uint64_t, hash_value, , "type name hash value"),
#endif /* RL_TREE_LOOKUP */
		(char *, attr, , "stringified typedef attributes"),
		(int, size, , "size of type"),
		RARRAY (rl_fd_t, fields, "fields or enums descriptors"),
		(char *, comment, , "type comments"),
		(rl_ptr_t, ext, , "ptr_type"), /* extra pointer for user data */
		(char *, ptr_type, , "union discriminator"),
		) /* type descriptor */

TYPEDEF_STRUCT (rl_td_ptr_t, ATTRIBUTES ( , "rl_td_t pointer wrapper"),
		(rl_td_t *, tdp, , "pointer on type descriptor"),
		)

TYPEDEF_STRUCT (rl_mem_t, ATTRIBUTES ( , "ResLib memory operations"),
		(float, mem_alloc_strategy, , "memory allocation strategy"),
		(void*, malloc, (const char *, const char *, int, size_t), "pointer on malloc() function"),
		(void*, realloc, (const char *, const char *, int, void*, size_t), "pointer on realloc() function"),
		(char*, strdup, (const char *, const char *, int, const char*), "pointer on strdup() function"),
		(void, free, (const char *, const char *, int, void*), "pointer on free() function"),
		)

TYPEDEF_STRUCT (rl_ra_rl_td_ptr_t,
		RARRAY (rl_td_ptr_t, ra, "non-collision hash table"),
		)

TYPEDEF_ENUM (rl_ptrdes_flags_t, ATTRIBUTES ( , "bitmask values"),
	      (RL_PDF_NONE, = 0),
	      (RL_PDF_IS_NULL, = (1 << 0)),
	      (RL_PDF_IS_REFERENCED, = (1 << 1)),
	      (RL_PDF_RARRAY_SIZE, = (1 << 2)),
	      )

TYPEDEF_STRUCT (rl_ptrdes_t, ATTRIBUTES ( , "pointer descriptor type"),
		(void *, data, , "pointer on binary data"),
		(rl_fd_t, fd, , "field descriptor"),
		(int, level, , "level from the root element"),
		(int, idx, , "public index"),
		(int, ref_idx, , "reference index (internal enumeration)"),
		(int, parent, , "parent index"),
		(int, first_child, , "first child index"),
		(int, last_child, , "last child index"),
		(int, prev, , "previous sibling index"),
		(int, next, , "next sibling index"),
		BITMASK (rl_ptrdes_flags_t, flags),
		(int32_t, rarray_size, , "size of resizable array"),
		(int, union_field_idx, , "field descriptor for unions"),
		(char *, value, , "stringified value"),
		(rl_ptr_t, ext, , "ptr_type"), /* extra pointer for user data */
		(char *, ptr_type, , "union discriminator"),
		) /* pointer descriptor */

TYPEDEF_STRUCT (rl_ra_rl_ptrdes_t, ATTRIBUTES ( , "rl_ptrdes_t resizable array"),
		RARRAY (rl_ptrdes_t, ra, "resizable array with rl_ptrdes_t"),
		)

TYPEDEF_STRUCT (rl_save_type_data_t, ATTRIBUTES ( , "serialization of the node"),
		(char *, prefix, , "statically allocated string"),
		(char *, content, , "dynamically formed string. Need to be freed."),
		(char *, suffix, , "statically allocated string"),
		)

TYPEDEF_STRUCT (rl_lloc_t, ATTRIBUTES ( , "parser location"),
		(int, lineno, , "parser location - line number"),
		(int, column, , "parser location - column number"),
		(int, offset, , "parser location - offset in string"),
		)
  
TYPEDEF_STRUCT (rl_token_lloc_t, ATTRIBUTES ( , "token location"),
		(rl_lloc_t, start, , "start of the token"),
		(rl_lloc_t, end, , "end of the token"),
		)

TYPEDEF_STRUCT (rl_load_t, ATTRIBUTES ( , "ResLib load parser data"),
		(rl_lloc_t, lloc, , "current location of parser"),
		(char *, str, , "string to parse"),
		(char *, buf, , "parser internal buffer"),
		(int, parent, , "index of current parent"),
		(rl_ra_rl_ptrdes_t *, ptrs, , "resizable array with rl_ptrdes_t"),
		)

TYPEDEF_STRUCT (rl_load_data_t,
		(rl_ra_rl_ptrdes_t, ptrs, , "internal representation of a loaded tree"),
		RARRAY (int, rl_ra_idx, "indexes of postponed nodes"),
		)

TYPEDEF_STRUCT (rl_save_data_t, ATTRIBUTES ( , "save routines data and lookup structures"),
		(rl_ra_rl_ptrdes_t, ptrs, , "internal representation of a saved tree"),
		(int, parent, , "index of current parent"),
		(rl_red_black_tree_node_t *, typed_ptrs_tree, , "B-tree with typed nodes"),
		(rl_red_black_tree_node_t *, untyped_ptrs_tree, , "B-tree with untyped nodes"),
		RARRAY (int, rl_ra_idx, "indexes of postponed nodes"),
		)

TYPEDEF_STRUCT (rl_load_io_t, ATTRIBUTES ( , "load handlers"),
		(int, rl, (int, rl_load_data_t*), "handler for internal format parsing"),
		(int, xdr, (XDR*, int, rl_ra_rl_ptrdes_t*), "handler for XDR parsing"),
		)

TYPEDEF_STRUCT (rl_save_io_t, ATTRIBUTES ( , "save handlers"),
		(void, rl, (rl_save_data_t*), "handler for internal format"),
		(int, xdr, (XDR*, int, rl_ra_rl_ptrdes_t*), "handler for XDR"),
		(char *, xml, (int, rl_ra_rl_ptrdes_t*), "handler for XML"),
		(int, cinit, (int, rl_ra_rl_ptrdes_t*, rl_save_type_data_t*), "handler for CINIT"),
		(int, json, (int, rl_ra_rl_ptrdes_t*, rl_save_type_data_t*), "handler for JSON"),
		(char *, scm, (int, rl_ra_rl_ptrdes_t*), "handler for SCHEME"),
		)

TYPEDEF_STRUCT (rl_io_handler_t, ATTRIBUTES ( , "input/ouput handlers"),
		(rl_load_io_t, load, , "load handlers"),
		(rl_save_io_t, save, , "save handlers"),
		)

TYPEDEF_FUNC (rl_output_format_t, char*, (rl_ptrdes_t*), "formater handler")

TYPEDEF_STRUCT (rl_conf_t, ATTRIBUTES ( , "ResLib configuration"),
		(rl_mem_t, rl_mem, , "memory operations"),
		(rl_log_level_t, log_level),
		(void, msg_handler, (const char*, const char*, int, rl_log_level_t, rl_message_id_t, va_list), "handler for error messages"),
#ifndef RL_TREE_LOOKUP
		(rl_ra_rl_td_ptr_t, hash, , "hash for type descriptors lookup"),
#else /* RL_TREE_LOOKUP */
		(rl_red_black_tree_node_t *, tree, , "B-tree for type descriptors lookup"),
#endif /* RL_TREE_LOOKUP */
		RARRAY (rl_td_ptr_t, des, "types descriptors"),
		(rl_red_black_tree_node_t *, enum_by_name, , "B-tree with enums mapping"),
		(rl_output_format_t, output_format, [RL_MAX_TYPES], "formaters"),
		(rl_io_handler_t, io_handlers, [RL_MAX_TYPES], "io handlers"),
		(rl_io_handler_t, io_ext_handlers, [RL_MAX_TYPES], "io handlers"),
		)

