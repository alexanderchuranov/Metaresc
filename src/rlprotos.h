/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

#include <reslib.h>

#undef RL_TYPE_NAME
#define RL_TYPE_NAME rl_log_level_t
RL_TYPEDEF_ENUM ()
  RL_ENUM_DEF (RL_LL_ALL, = 0)
  RL_ENUM_DEF (RL_LL_TRACE)
  RL_ENUM_DEF (RL_LL_DEBUG)
  RL_ENUM_DEF (RL_LL_INFO)
  RL_ENUM_DEF (RL_LL_WARN)
  RL_ENUM_DEF (RL_LL_ERROR)
  RL_ENUM_DEF (RL_LL_FATAL)
  RL_ENUM_DEF (RL_LL_OFF)
RL_END_ENUM ("Log levels enum")

#undef RL_TYPE_NAME
#define RL_TYPE_NAME rl_message_id_t
RL_TYPEDEF_ENUM () /* Messages enum. Message string saved in .comment field of meta info */
  RL_ENUM_DEF (RL_MESSAGE_OUT_OF_MEMORY, , "Out of memory.")
  RL_ENUM_DEF (RL_MESSAGE_UNEXPECTED_END, , "Unexpected end of file or end of string.")
  RL_ENUM_DEF (RL_MESSAGE_UNEXPECTED_DATA, , "Unexpected data in stream.")
  RL_ENUM_DEF (RL_MESSAGE_UNBALANCED_TAGS, , "Unbalanced tags.")
  RL_ENUM_DEF (RL_MESSAGE_XDR_WRONG_ENCODING_MODE, , "XDR wrong encoding mode.")
  RL_ENUM_DEF (RL_MESSAGE_XDR_SAVE_FAILED, , "XDR save to resizable array failed.")
  RL_ENUM_DEF (RL_MESSAGE_XML_SAVE_FAILED, , "XML save failed.")
  RL_ENUM_DEF (RL_MESSAGE_LOAD_STRUCT_FAILED, , "Structure load failed.")
  RL_ENUM_DEF (RL_MESSAGE_LOAD_METHOD_MISSED, , "Load method missed.")
  RL_ENUM_DEF (RL_MESSAGE_XDR_LOAD_FAILED, , "XDR load failed.")
  RL_ENUM_DEF (RL_MESSAGE_NO_TYPE_DESCRIPTOR, , "Can't find type descriptor for '%s'.")
  RL_ENUM_DEF (RL_MESSAGE_NOT_A_UNION, , "Type '%s' is not a union.")
  RL_ENUM_DEF (RL_MESSAGE_NULL_POINTER, , "Can't save/load to/from NULL pointer.")
  RL_ENUM_DEF (RL_MESSAGE_IDS_COLLISION, , "Two XML nodes have the same %s=%" SCNd32 ".")
  RL_ENUM_DEF (RL_MESSAGE_UNDEFINED_REF_IDX, , "Reference on a node with %s=%" SCNd32 " could not be found.")
  RL_ENUM_DEF (RL_MESSAGE_UNKNOWN_ENUM, , "Unknown enum value '%s'.")
  RL_ENUM_DEF (RL_MESSAGE_READ_INT, , "Can't read integer value from string `%s`.")
  RL_ENUM_DEF (RL_MESSAGE_READ_ENUM, , "Can't read enum value from string `%s`.")
  RL_ENUM_DEF (RL_MESSAGE_READ_BITMASK, , "Can't read bitmask from string `%s`.")
  RL_ENUM_DEF (RL_MESSAGE_READ_FLOAT, , "Can't read float from string `%s`.")
  RL_ENUM_DEF (RL_MESSAGE_READ_DOUBLE, , "Can't read double from string `%s`.")
  RL_ENUM_DEF (RL_MESSAGE_READ_LONG_DOUBLE, , "Can't read long double from string `%s`.")
  RL_ENUM_DEF (RL_MESSAGE_READ_CHAR, , "Can't read char from string `%s`.")
  RL_ENUM_DEF (RL_MESSAGE_STRING_TRUNCATED, , "Char array range check: string truncated.")
  RL_ENUM_DEF (RL_MESSAGE_UNKNOWN_SUBNODE, , "Struct type '%s': unknown subnode '%s'.")
  RL_ENUM_DEF (RL_MESSAGE_SAVE_IDX_RANGE_CHECK, , "Save node index is out of range.")
  RL_ENUM_DEF (RL_MESSAGE_NODE_NAME_MISSMATCH, , "Node name mismatch: expected '%s' got '%s'.")
  RL_ENUM_DEF (RL_MESSAGE_NODE_TYPE_MISSMATCH, , "Node type mismatch: expected '%s' got '%s'.")
  RL_ENUM_DEF (RL_MESSAGE_UNSUPPORTED_NODE_TYPE, , "Unsupported node type: %s(%d):%s(%d).")
  RL_ENUM_DEF (RL_MESSAGE_RANGE_CHECK, , "Range check error in array '%s'.")
  RL_ENUM_DEF (RL_MESSAGE_READ_REF, , "Can't read '%s' ref index.")
  RL_ENUM_DEF (RL_MESSAGE_TYPE_NOT_ENUM, , "Type '%s' is not a enum.")
  RL_ENUM_DEF (RL_MESSAGE_SAVE_ENUM, , "Can't find enum name for value %" SCNd64 " type '%s'.")
  RL_ENUM_DEF (RL_MESSAGE_SAVE_BITMASK, , "Can't decompose on bitmask %llu.")
  RL_ENUM_DEF (RL_MESSAGE_DUPLICATED_ENUMS, , "Duplicated enum definition for `%s` in enum '%s'.")
  RL_ENUM_DEF (RL_MESSAGE_DUPLICATED_FIELDS, , "Duplicated field definition for `%s` in type '%s'. Incorrect results are possible!")
  RL_ENUM_DEF (RL_MESSAGE_INT_OF_UNKNOWN_SIZE, , "Failed to stringify integer of unknown size: %" SCNd32 ".")
  RL_ENUM_DEF (RL_MESSAGE_PARSE_ERROR, , "Parser error: '%s'. Position: %" SCNd32 ":%" SCNd32 "-%" SCNd32 ":%" SCNd32 ".")
  RL_ENUM_DEF (RL_MESSAGE_ANON_UNION_TYPE_ERROR, , "Can't create type for anonymous union due to type name collision: '%s'.")
  RL_ENUM_DEF (RL_MESSAGE_UNKNOWN_XML_ESC, , "Unknown XML escape sequence '%s'.")
  RL_ENUM_DEF (RL_MESSAGE_WRONG_XML_ESC, , "Wrong XML escape sequence '%s'.")
  RL_ENUM_DEF (RL_MESSAGE_TYPES_HASHES_MATCHED, , "For types '%s' and '%s' hashes matched. Last type is not available.")
  RL_ENUM_DEF (RL_MESSAGE_RARRAY_LOAD_FAILED, , "Failed to find field .data in C-style data.")
  RL_ENUM_DEF (RL_MESSAGE_LAST, , "Last message ID.")
RL_END_ENUM ("RL_MESSAGE enum")

#undef RL_TYPE_NAME
#define RL_TYPE_NAME rl_type_t
RL_TYPEDEF_ENUM ()
  RL_ENUM_DEF (RL_TYPE_NONE, = 0)
  RL_ENUM_DEF (RL_TYPE_CHAR, = 1)
  RL_ENUM_DEF (RL_TYPE_CHAR_ARRAY, = 2)
  RL_ENUM_DEF (RL_TYPE_STRING, = 3, "RL_TYPE_CHAR | RL_TYPE_STRING should be the same as RL_TYPE_STRING for proper char* auto-detection")
  RL_ENUM_DEF (RL_TYPE_VOID)
  RL_ENUM_DEF (RL_TYPE_INT8)
  RL_ENUM_DEF (RL_TYPE_UINT8)
  RL_ENUM_DEF (RL_TYPE_INT16)
  RL_ENUM_DEF (RL_TYPE_UINT16)
  RL_ENUM_DEF (RL_TYPE_INT32)
  RL_ENUM_DEF (RL_TYPE_UINT32)
  RL_ENUM_DEF (RL_TYPE_INT64)
  RL_ENUM_DEF (RL_TYPE_UINT64)
  RL_ENUM_DEF (RL_TYPE_FLOAT)
  RL_ENUM_DEF (RL_TYPE_DOUBLE)
  RL_ENUM_DEF (RL_TYPE_LONG_DOUBLE)
  RL_ENUM_DEF (RL_TYPE_STRUCT)
  RL_ENUM_DEF (RL_TYPE_ENUM)
  RL_ENUM_DEF (RL_TYPE_BITMASK)
  RL_ENUM_DEF (RL_TYPE_FUNC)
  RL_ENUM_DEF (RL_TYPE_UNION)
  RL_ENUM_DEF (RL_TYPE_ANON_UNION)
  RL_ENUM_DEF (RL_TYPE_END_ANON_UNION)
  RL_ENUM_DEF (RL_TYPE_TRAILING_RECORD)
  RL_ENUM_DEF (RL_TYPE_LAST, , "keep it last")
RL_END_ENUM ("ResLib types enum")

#undef RL_TYPE_NAME
#define RL_TYPE_NAME rl_type_ext_t
RL_TYPEDEF_ENUM ()
  RL_ENUM_DEF (RL_TYPE_EXT_NONE)
  RL_ENUM_DEF (RL_TYPE_EXT_ARRAY)
  RL_ENUM_DEF (RL_TYPE_EXT_RARRAY)
  RL_ENUM_DEF (RL_TYPE_EXT_POINTER)
  RL_ENUM_DEF (RL_TYPE_EXT_LAST, , "keep it last")
RL_END_ENUM ("ResLib type extension enum")

/* rl_rarray_t defenition should be syncroonized with RL_RARRAY_PROTO macro */
#undef RL_TYPE_NAME
#define RL_TYPE_NAME rl_rarray_t
RL_TYPEDEF_STRUCT (__attribute__((packed)))
  RL_POINTER (void, data, "pointer on data array")
  RL_INT32 (size, "used space in bytes")
  RL_INT32 (alloc_size, "allocated space in bytes")
  RL_POINTER (void, ext, "extra pointer for user data")
RL_END_STRUCT ("resizable array type")

#undef RL_TYPE_NAME
#define RL_TYPE_NAME rl_red_black_t
RL_TYPEDEF_ENUM (__attribute__ ((packed,aligned(sizeof (unsigned char)))))
  RL_ENUM_DEF (RL_BLACK, = 0)
  RL_ENUM_DEF (RL_RED, = 1)
RL_END_ENUM ("Red/Black enum")

#undef RL_TYPE_NAME
#define RL_TYPE_NAME rl_red_black_tree_node_t
RL_TYPEDEF_STRUCT ()
  RL_POINTER (const void, key, "void pointer")
  RL_POINTER (rl_red_black_tree_node_t, left, "left child")
  RL_POINTER (rl_red_black_tree_node_t, right, "right child")
  RL_ENUM (rl_red_black_t, red)
RL_END_STRUCT ("red/black tree node")

#undef RL_TYPE_NAME
#define RL_TYPE_NAME rl_fd_t
RL_TYPEDEF_STRUCT ()
  RL_STRING (type, "stringified type name")
  RL_STRING (name, "name of the field")
  RL_UINT64 (hash_value, "hash of the name")
  RL_UINT32 (size, "size of field")
  RL_UINT32 (offset, "offset in structure")
  RL_ENUM (rl_type_t, rl_type, "ResLib type")
  RL_ENUM (rl_type_t, rl_type_ptr, "ResLib type if field is a pointer on builtin types")
  RL_ENUM (rl_type_ext_t, rl_type_ext, "ResLib type extension")
  RL_INT32 (count, "array size")
  RL_INT32 (row_count, "row size")
  RL_INT64 (value, "enum value")
  RL_RARRAY (rl_fd_t, args, "function arguments descriptors")
  RL_STRING (comment, "field comments")
     /*
       ext field can be used by user for extended information
       placed after comments for tricky intialization
       sample:
       RL_NONE (void*, ext, , "user extended info", &ext_info_struct)
       or
       RL_NONE (void*, ext, , "user extended info", .ext = &ext_info_struct)
       or
       RL_NONE (void*, ext, , "user extended info", &((ext_info_t){ .field = XXX }))
       or
       RL_NONE (void*, ext, , "user extended info", (ext_info_t[]){ {.field = XXX}, {.field = YYY} })
       or
       RL_NONE (void*, ext, , "user extended info", "one more extra string")
      */
  RL_POINTER (void, ext, "extra pointer for user data")
RL_END_STRUCT ("ResLib field descriptor") /* field descriptor */

#undef RL_TYPE_NAME
#define RL_TYPE_NAME rl_fd_ptr_t
RL_TYPEDEF_STRUCT ()
  RL_POINTER (rl_fd_t, fdp, "pointer on field descriptor")
RL_END_STRUCT ("rl_fd_t pointer wrapper")

#undef RL_TYPE_NAME
#define RL_TYPE_NAME rl_td_t
RL_TYPEDEF_STRUCT ()
  RL_ENUM (rl_type_t, rl_type, "ResLib type") /* possible variants RL_TYPE_ENUM, RL_TYPE_STRUCT, RL_TYPE_UNION */
  RL_STRING (type, "stringified type name") 
  RL_POINTER (rl_red_black_tree_node_t, lookup_by_value, "B-tree for enums values lookup")
  RL_RARRAY (rl_fd_ptr_t, lookup_by_name, "hash for lookup by field name")
#ifndef RL_TREE_LOOKUP
  RL_UINT64 (hash_value, "type name hash value")
#endif /* RL_TREE_LOOKUP */
  RL_STRING (attr, "stringified typedef attributes")
  RL_UINT32 (size, "size of type")
  RL_RARRAY (rl_fd_t, fields, "fields or enums descriptors")
  RL_STRING (comment, "type comments")
  RL_POINTER (void, ext, "extra pointer for user data")
RL_END_STRUCT ("ResLib type descriptor") /* type descriptor */

#undef RL_TYPE_NAME
#define RL_TYPE_NAME rl_td_ptr_t
RL_TYPEDEF_STRUCT ()
  RL_POINTER (rl_td_t, tdp, "pointer on type descriptor")
RL_END_STRUCT ("rl_td_t pointer wrapper")

#undef RL_TYPE_NAME
#define RL_TYPE_NAME rl_mem_t
RL_TYPEDEF_STRUCT ()
  RL_FLOAT (mem_alloc_strategy, "memory allocation strategy")
  RL_FUNC (void*, malloc, (const char *, const char *, int, size_t), "pointer on malloc() function")
  RL_FUNC (void*, realloc, (const char *, const char *, int, void*, size_t), "pointer on realloc() function")
  RL_FUNC (char*, strdup, (const char *, const char *, int, const char*), "pointer on strdup() function")
  RL_FUNC (void, free, (const char *, const char *, int, void*), "pointer on free() function")
RL_END_STRUCT ("ResLib memory operations")

#undef RL_TYPE_NAME
#define RL_TYPE_NAME rl_void_ptr_t
RL_TYPEDEF_STRUCT ()
  RL_POINTER (void, ptr)
RL_END_STRUCT ()

#undef RL_TYPE_NAME
#define RL_TYPE_NAME rl_ra_rl_td_ptr_t
RL_TYPEDEF_STRUCT ()
  RL_RARRAY (rl_td_ptr_t, ra, "non-collision hash table")
RL_END_STRUCT ()

#undef RL_TYPE_NAME
#define RL_TYPE_NAME rl_conf_t
RL_TYPEDEF_STRUCT ()
  RL_STRUCT (rl_mem_t, rl_mem, "memory operations")
  RL_ENUM (rl_log_level_t, log_level)
  RL_FUNC (void, msg_handler, (const char*, const char*, int, rl_log_level_t, rl_message_id_t, va_list), "handler for error messages")
#ifndef RL_TREE_LOOKUP
  RL_STRUCT (rl_ra_rl_td_ptr_t, hash, "hash for type descriptors lookup")
#else /* RL_TREE_LOOKUP */
  RL_POINTER (rl_red_black_tree_node_t, tree, "B-tree for type descriptors lookup")
#endif /* RL_TREE_LOOKUP */
  RL_RARRAY (rl_td_ptr_t, des, "types descriptors")
  RL_POINTER (rl_red_black_tree_node_t, enum_by_name, "B-tree with enums mapping")
  RL_RARRAY (rl_void_ptr_t, allocated_mem, "memory to deallocate")
RL_END_STRUCT ("ResLib configuration")

#undef RL_TYPE_NAME
#define RL_TYPE_NAME rl_ptrdes_flags_t
RL_TYPEDEF_ENUM ()
  RL_ENUM_DEF (RL_PDF_NONE, = 0)
  RL_ENUM_DEF (RL_PDF_IS_NULL, = (1 << 0))
  RL_ENUM_DEF (RL_PDF_IS_REFERENCED, = (1 << 1))
RL_END_ENUM ("bitmask values")
  
#undef RL_TYPE_NAME
#define RL_TYPE_NAME rl_ptrdes_t
RL_TYPEDEF_STRUCT ()
  RL_POINTER (void, data, "pointer on binary data")
  RL_STRUCT (rl_fd_t, fd, "field descriptor")
  RL_INT32 (level, "level from the root element")
  RL_INT32 (idx, "public index")
  RL_INT32 (ref_idx, "reference index (internal enumeration)")
  RL_INT32 (parent, "parent index")
  RL_INT32 (first_child, "first child index")
  RL_INT32 (last_child, "last child index")
  RL_INT32 (prev, "previous sibling index")
  RL_INT32 (next, "next sibling index")
  RL_BITMASK (rl_ptrdes_flags_t, flags)
  RL_INT32 (union_field_idx, "field descriptor for unions")
  RL_STRING (value, "stringified value")
  RL_POINTER (void, ext, "extra pointer for user data")
RL_END_STRUCT ("pointer descriptor type") /* pointer descriptor */

#undef RL_TYPE_NAME
#define RL_TYPE_NAME rl_ra_rl_ptrdes_t
RL_TYPEDEF_STRUCT ()
  RL_RARRAY (rl_ptrdes_t, ra, "resizable array with rl_ptrdes_t")
RL_END_STRUCT ("rl_ptrdes_t resizable array")

#undef RL_TYPE_NAME
#define RL_TYPE_NAME rl_save_type_data_t
RL_TYPEDEF_STRUCT ()
  RL_STRING (prefix, "statically allocated string")
  RL_STRING (content, "dynamically formed string. Need to be freed.")
  RL_STRING (suffix, "statically allocated string")
RL_END_STRUCT ("serialization of the node")

#undef RL_TYPE_NAME
#define RL_TYPE_NAME rl_lloc_t
RL_TYPEDEF_STRUCT ()
  RL_INT32 (lineno, "parser location - line number")
  RL_INT32 (column, "parser location - column number")
  RL_INT32 (offset, "parser location - offset in string")
RL_END_STRUCT ("parser location")
  
#undef RL_TYPE_NAME
#define RL_TYPE_NAME rl_token_lloc_t
RL_TYPEDEF_STRUCT ()
  RL_STRUCT (rl_lloc_t, start, "start of the token")
  RL_STRUCT (rl_lloc_t, end, "end of the token")
RL_END_STRUCT ("token location")
  
#undef RL_TYPE_NAME
#define RL_TYPE_NAME rl_load_t
RL_TYPEDEF_STRUCT ()
  RL_STRUCT (rl_lloc_t, lloc, "current location of parser")
  RL_STRING (str, "string to parse")
  RL_STRING (buf, "parser internal buffer")
  RL_INT32 (parent, "index of current parent")
  RL_POINTER (rl_ra_rl_ptrdes_t, ptrs, "resizable array with rl_ptrdes_t")
RL_END_STRUCT ("ResLib load parser data")

#undef RL_TYPE_NAME
#define RL_TYPE_NAME rl_save_data_t
RL_TYPEDEF_STRUCT ()
  RL_STRUCT (rl_ra_rl_ptrdes_t, ptrs, "internal representation of saved tree")
  RL_INT32 (parent, "index of current parent")
  RL_POINTER (rl_red_black_tree_node_t, typed_ptrs_tree, "B-tree with typed nodes")
  RL_POINTER (rl_red_black_tree_node_t, untyped_ptrs_tree, "B-tree with untyped nodes")
  RL_RARRAY (int32_t, rl_ra_idx, "indexes of postponed nodes")
RL_END_STRUCT ("save routines data and lookup structures")

#undef RL_TYPE_NAME
#define RL_TYPE_NAME rl_load_io_t
RL_TYPEDEF_STRUCT ()
  RL_FUNC (int, rl, (int, rl_ra_rl_ptrdes_t*), "handler for internal format parsing")
  RL_FUNC (int, xdr, (XDR*, int, rl_ra_rl_ptrdes_t*), "handler for XDR parsing")
RL_END_STRUCT ("load handlers")

#undef RL_TYPE_NAME
#define RL_TYPE_NAME rl_save_io_t
RL_TYPEDEF_STRUCT ()
  RL_FUNC (void, rl, (rl_save_data_t*), "handler for internal format")
  RL_FUNC (int, xdr, (XDR*, int, rl_ra_rl_ptrdes_t*), "handler for XDR")
  RL_FUNC (char *, xml, (int, rl_ra_rl_ptrdes_t*), "handler for XML")
  RL_FUNC (int, cinit, (int, rl_ra_rl_ptrdes_t*, rl_save_type_data_t*), "handler for CINIT")
  RL_FUNC (int, json, (int, rl_ra_rl_ptrdes_t*, rl_save_type_data_t*), "handler for JSON")
  RL_FUNC (char *, scm, (int, rl_ra_rl_ptrdes_t*), "handler for SCHEME")
RL_END_STRUCT ("save handlers")

#undef RL_TYPE_NAME
#define RL_TYPE_NAME rl_io_handler_t
RL_TYPEDEF_STRUCT ()
  RL_STRUCT (rl_load_io_t, load, "load handlers")
  RL_STRUCT (rl_save_io_t, save, "save handlers")
RL_END_STRUCT ("input/ouput handlers")
