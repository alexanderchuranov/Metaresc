/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

/* Metaresc prototypes */
#ifndef _METARESC_H_
#define _METARESC_H_

#include <mr_pp.h>
#include <mr_export.h>

#ifndef __USE_GNU
#define __USE_GNU
#endif /* __USE_GNU */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif /* _GNU_SOURCE */
#include <stdio.h> /* for FILE */
#include <stddef.h> /* for offsetof */
#include <string.h> /* for strlen () & memset () */
#include <ctype.h> /* for isspace () */
#include <stdarg.h> /* for va_list */
#include <inttypes.h> /* for int8_t, int16_t, int32_t, int64_t, SCNxXX, etc */
#ifdef HAVE_LIBXML2
# include <libxml/xmlmemory.h>
# include <libxml/parser.h>
#endif /* HAVE_LIBXML2 */
#include <rpc/types.h>
#include <rpc/xdr.h>

/* Library exports */
#define MR_MAX_STRING_LENGTH ((unsigned int)-1)

#define MR_TYPE_ANONYMOUS_UNION_TEMPLATE "mr_type_anonymous_union_%d_t"

#define MR_INT_TO_STRING_BUF_SIZE (32)
#define MR_FLOAT_TO_STRING_BUF_SIZE (256)
#define MR_CHAR_TO_STRING_BUF_SIZE (8)

#define MR_SIZEOF_LONG_DOUBLE (10)

#define MR_MAX_INDENT_LEVEL (30) /* the same constant as in libxml2 */
#define MR_MIN(X,Y) ({ typeof(X) _x_ = (X); typeof(Y) _y_ = (Y); (_x_ < _y_) ? _x_ : _y_; })
#define MR_MAX(X,Y) ({ typeof(X) _x_ = (X); typeof(Y) _y_ = (Y); (_x_ > _y_) ? _x_ : _y_; })
#define MR_LIMIT_LEVEL(LEVEL) MR_MIN (LEVEL, MR_MAX_INDENT_LEVEL)

/* each refereed structure will have REF_IDX property */
#define MR_REF_IDX "ref_idx"
/* references on already saved structures will be replaced with nodes that have only REF index property */
#define MR_REF "ref"
#define MR_REF_CONTENT "ref_content"
/* XML attribute for zero length strings */
#define MR_ISNULL "isnull"
#define MR_ISNULL_VALUE "true"
#define MR_IC_NONE_TYPE_T "mr_ic_none_type_t"
#define MR_RARRAY_OPAQUE_DATA_T mr_rarray_opaque_data_t
#define MR_RARRAY_OPAQUE_DATA_T_STR MR_STRINGIFY_READONLY (MR_RARRAY_OPAQUE_DATA_T)
#define MR_STRINGIFY_READONLY(...) MR_STRINGIFY_READONLY_ (__VA_ARGS__)
#define MR_STRINGIFY_READONLY_(...) #__VA_ARGS__

#define MR_BITMASK_OR_DELIMITER " | "

#define MR_MESSAGE(LOG_LEVEL, /* MSG_ID */ ...) mr_message (__FILE__, __FUNCTION__, __LINE__, LOG_LEVEL, __VA_ARGS__)
#define MR_MESSAGE_UNSUPPORTED_NODE_TYPE_(FDP)				\
  ({									\
    mr_fd_t * __fdp__ = FDP;						\
    mr_td_t * mr_type_td = mr_get_td_by_name ("mr_type_t");		\
    mr_td_t * mr_type_ext_td = mr_get_td_by_name ("mr_type_ext_t");	\
    mr_fd_t * mr_type_fd = mr_type_td ? mr_get_enum_by_value (mr_type_td, __fdp__->mr_type) : NULL; \
    mr_fd_t * mr_type_ext_fd = mr_type_ext_td ? mr_get_enum_by_value (mr_type_ext_td, __fdp__->mr_type_ext) : NULL; \
    MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNSUPPORTED_NODE_TYPE, (mr_type_fd ? mr_type_fd->name.str : "unknown"), __fdp__->mr_type, (mr_type_ext_fd ? mr_type_ext_fd->name.str : "unknown"), __fdp__->mr_type_ext); \
  })

/* make a string from argument in writable memory. #STR itself is in read-only memory */
#define MR_STRINGIFY(STR) (char []) { #STR }

#define MR_MEM_INIT(FUNC, /* ATTR */ ...) __VA_ARGS__ void mr_mem_init (void) { FUNC; }

#define MR_MALLOC(SIZE) (mr_conf.mr_mem.malloc)(__FILE__, __FUNCTION__, __LINE__, SIZE)
#define MR_REALLOC(PTR, SIZE) (mr_conf.mr_mem.realloc)(__FILE__, __FUNCTION__, __LINE__, PTR, SIZE)
#define MR_STRDUP(STR) (mr_conf.mr_mem.strdup)(__FILE__, __FUNCTION__, __LINE__, STR)
#define MR_FREE(PTR) (mr_conf.mr_mem.free)(__FILE__, __FUNCTION__, __LINE__, PTR)

/*
  you can redefine this prefixes from outside before first include of metaresc.h
*/
#ifndef MR_DESCRIPTOR_PREFIX
#define MR_DESCRIPTOR_PREFIX(MR_TYPE_NAME) mr_td_ ## MR_TYPE_NAME
#endif /* MR_DESCRIPTOR_PREFIX */
#ifndef MR_TYPEDEF_PREFIX
#define MR_TYPEDEF_PREFIX(MR_TYPE_NAME) MR_TYPE_NAME
#endif /* MR_TYPEDEF_PREFIX */
#ifndef MR_CONSTRUCTOR_PREFIX
#define MR_CONSTRUCTOR_PREFIX(MR_TYPE_NAME) mr_init_ ## MR_TYPE_NAME
#endif /* MR_CONSTRUCTOR_PREFIX */
/* this attribute can be redefined from outside */
#ifndef MR_DESCRIPTOR_ATTR
#define MR_DESCRIPTOR_ATTR static
#endif /* MR_DESCRIPTOR_ATTR */

/*
  Help macro for internal type detection. It compares variable with all known builin types.
  Compaund types are detected in runtime.
*/
#define MR_TYPE_DETECT(TYPE, ...) MR_TYPE_DETECT1 (TYPE, __VA_ARGS__)
#define MR_TYPE_DETECT1(TYPE, SUFFIX, ...) MR_TYPE_DETECT2 (TYPE, SUFFIX, __VA_ARGS__)
#define MR_TYPE_DETECT2(TYPE, SUFFIX, PREFIX) MR_TYPE_DETECT3 (PREFIX, TYPE, SUFFIX)

#define MR_TYPE_DETECT3(PREFIX, TYPE, SUFFIX)				\
  (0 /* MR_TYPE_NONE */							\
   | (__builtin_types_compatible_p (PREFIX void SUFFIX, TYPE) ? MR_TYPE_VOID : 0) \
   | (__builtin_types_compatible_p (PREFIX int8_t SUFFIX, TYPE) ? MR_TYPE_INT8 : 0) \
   | (__builtin_types_compatible_p (PREFIX uint8_t SUFFIX, TYPE) ? MR_TYPE_UINT8 : 0) \
   | (__builtin_types_compatible_p (PREFIX int16_t SUFFIX, TYPE) ? MR_TYPE_INT16 : 0) \
   | (__builtin_types_compatible_p (PREFIX uint16_t SUFFIX, TYPE) ? MR_TYPE_UINT16 : 0) \
   | (__builtin_types_compatible_p (PREFIX int32_t SUFFIX, TYPE) ? MR_TYPE_INT32 : 0) \
   | (__builtin_types_compatible_p (PREFIX uint32_t SUFFIX, TYPE) ? MR_TYPE_UINT32 : 0) \
   | (__builtin_types_compatible_p (PREFIX int64_t SUFFIX, TYPE) ? MR_TYPE_INT64 : 0) \
   | (__builtin_types_compatible_p (PREFIX uint64_t SUFFIX, TYPE) ? MR_TYPE_UINT64 : 0) \
   | (__builtin_types_compatible_p (PREFIX MR_UNCOVERED_TYPE SUFFIX, TYPE) ? MR_PASTE2 (MR_TYPE_INT, MR_SIZEOF_UNCOVERED_TYPE) : 0) \
   | (__builtin_types_compatible_p (PREFIX unsigned MR_UNCOVERED_TYPE SUFFIX, TYPE) ? MR_PASTE2 (MR_TYPE_UINT, MR_SIZEOF_UNCOVERED_TYPE) : 0) \
   | (__builtin_types_compatible_p (PREFIX float SUFFIX, TYPE) ? MR_TYPE_FLOAT : 0) \
   | (__builtin_types_compatible_p (PREFIX double SUFFIX, TYPE) ? MR_TYPE_DOUBLE : 0) \
   | (__builtin_types_compatible_p (PREFIX long double SUFFIX, TYPE) ? MR_TYPE_LONG_DOUBLE : 0) \
   | (__builtin_types_compatible_p (PREFIX char SUFFIX, TYPE) ? MR_TYPE_CHAR : 0) \
   | (__builtin_types_compatible_p (PREFIX char MR_IF_ELSE (MR_IS_EMPTY (SUFFIX)) () ((SUFFIX)) [], TYPE) ? MR_TYPE_CHAR_ARRAY : 0) \
   | ((__builtin_types_compatible_p (PREFIX char * SUFFIX, TYPE)	\
       MR_IF_ELSE (MR_IS_EMPTY (PREFIX)) (				\
					  | __builtin_types_compatible_p (const char * SUFFIX, TYPE) \
					  | __builtin_types_compatible_p (volatile char * SUFFIX, TYPE)	\
					  | __builtin_types_compatible_p (const volatile char * SUFFIX, TYPE)) ()) ? MR_TYPE_STRING : 0) \
   )
#define MR_TYPE_DETECT_PTR(TYPE) (MR_TYPE_DETECT (TYPE, *) | MR_TYPE_DETECT (TYPE, *, const) | MR_TYPE_DETECT (TYPE, *, volatile) | MR_TYPE_DETECT (TYPE, *, const volatile))
/* Help macro for arrays auto-detection */
#define MR_TYPE_EXT_DETECT(TYPE, S_PTR) (__builtin_types_compatible_p (TYPE [], typeof (S_PTR)) ? MR_TYPE_EXT_ARRAY : MR_TYPE_EXT_NONE)

/* internal macros for arguments evaluation and concatination */
#define MR_PASTE2(...) MR_PASTE2_ (__VA_ARGS__)
#define MR_PASTE2_(_0, _1) _0 ## _1
#define MR_PASTE3(...) MR_PASTE3_ (__VA_ARGS__)
#define MR_PASTE3_(_0, _1, _2) _0 ## _1 ## _2
#define MR_PASTE4(...) MR_PASTE4_ (__VA_ARGS__)
#define MR_PASTE4_(_0, _1, _2, _3) _0 ## _1 ## _2 ## _3
#define MR_PASTE5(...) MR_PASTE5_ (__VA_ARGS__)
#define MR_PASTE5_(_0, _1, _2, _3, _4) _0 ## _1 ## _2 ## _3 ## _4

/* Interface macros for unrolled loops from mr_pp.h */
#define MR_FOREACH(X, ...) MR_PASTE2 (MR_FOREACH, MR_NARG (__VA_ARGS__)) (X, __VA_ARGS__)
#define MR_FOR(NAME, N, OP, FUNC, ...) MR_PASTE2 (MR_FOR, N) (NAME, OP, FUNC, __VA_ARGS__)

/*
  Next group of macroses detects empty argument list.
*/
#define P00_IS__EQ__(...) ,
#define P00_IS_EMPTY_CASE_0001 ,

#define MR_IS_EMPTY(...)						\
  MR_HAS_COMMA								\
  (MR_PASTE5								\
   (P00_IS_EMPTY_CASE_,							\
    /* test if there is just one argument, eventually an empty one */	\
    MR_HAS_COMMA (__VA_ARGS__),						\
    /* test if P00_IS__EQ__ together with the argument adds a comma */	\
    MR_HAS_COMMA (P00_IS__EQ__ __VA_ARGS__),				\
    /* test if the argument together with a parenthesis adds a comma */ \
    MR_HAS_COMMA (__VA_ARGS__ ()),					\
    /* test if placing it between P00_IS__EQ__ and the parenthesis adds a comma */ \
    MR_HAS_COMMA (P00_IS__EQ__ __VA_ARGS__ ())				\
    ))

/* Next group of macroses checks that it has only one argument and it is 0 */
#define MR_IS_0_EQ_0 ,
#define MR_IS_EQ_0_CASE_011 ,

#define MR_GET_SECOND(_0, ...) __VA_ARGS__
#define MR_IS_EQ_0(...) MR_IS_EQ_0_ (__VA_ARGS__) /* evaluate arguments */
#define MR_IS_EQ_0_(...) MR_IS_EQ_0__ ((__VA_ARGS__), (MR_PASTE2 (MR_IS_0_EQ_, __VA_ARGS__)))
#define MR_IS_EQ_0__(ARGS, ARGS_EQ_0)					\
  MR_HAS_COMMA (MR_PASTE4 (MR_IS_EQ_0_CASE_,				\
			   /* test if there is just one argument, eventually a zero */ \
			   MR_HAS_COMMA ARGS,				\
			   /* test if MR_IS_0_EQ_ together with the argument adds a comma */ \
			   MR_HAS_COMMA ARGS_EQ_0,			\
			   /* test that there is nothing after comma */ \
			   MR_IS_EMPTY (MR_GET_SECOND ARGS_EQ_0)))

/* If clause implementation. Usage MR_IF_ELSE (test_value) (expand_if_nonzero) (expand_if_zero) */
#define MR_IGNORE(...)
#define MR_IDENT(...) __VA_ARGS__
#define MR_IF_ELSE_CASE_0(...) __VA_ARGS__ MR_IGNORE
#define MR_IF_ELSE_CASE_1(...) MR_IDENT
#define MR_IF_ELSE(...) MR_PASTE2 (MR_IF_ELSE_CASE_, MR_IS_EQ_0 (__VA_ARGS__))

/* Next macro MR_IS_IN_PAREN(...) checks that argument is in paren */
#define MR_IS_IN_PAREN_CASE_01 ,
#define MR_DETECT_PAREN(...) ,

#define MR_IS_IN_PAREN(...) MR_IS_IN_PAREN_ (__VA_ARGS__) /* evaluate agruments */
#define MR_IS_IN_PAREN_(...)						\
  MR_HAS_COMMA (MR_PASTE3 (MR_IS_IN_PAREN_CASE_,			\
			   /* test if there is just one argument, eventually in paren */ \
			   MR_HAS_COMMA (__VA_ARGS__),			\
			   /* test if MR_DETECT_PAREN_ together with the argument adds a comma */ \
			   MR_HAS_COMMA (MR_DETECT_PAREN __VA_ARGS__)))

/* MR_REMOVE_PAREN(...) removes parents if they are presented */
#define MR_REMOVE_PAREN_(...) __VA_ARGS__
#define MR_REMOVE_PAREN(...) MR_IF_ELSE (MR_IS_IN_PAREN (__VA_ARGS__)) (MR_REMOVE_PAREN_ __VA_ARGS__) (__VA_ARGS__)

/* Main macroses for types definition. It passes type of definition to next level. */
#define TYPEDEF_STRUCT(...) P00_TYPEDEF (STRUCT, __VA_ARGS__)
#define TYPEDEF_UNION(...) P00_TYPEDEF (UNION, __VA_ARGS__)
#define TYPEDEF_ENUM(...) P00_TYPEDEF (ENUM, __VA_ARGS__)
#define TYPEDEF_CHAR_ARRAY(...) P00_TYPEDEF (CHAR_ARRAY, __VA_ARGS__)
#define TYPEDEF_FUNC(...) P00_TYPEDEF (FUNC, __VA_ARGS__)

/*
  Checks if MR_MODE was defined. If it was undefined automtically produce prototypes and descriptors.
  Library could be unintrusively extended for other modes. Defined MR_MODE transparently passed to lower level.
*/
#define MR_IS_MR_MODE_EQ_MR_MODE 0
#define P00_TYPEDEF(...)						\
  MR_IF_ELSE (MR_PASTE2 (MR_IS_MR_MODE_EQ_, MR_MODE))			\
  (P00_TYPEDEF_MODE (MR_MODE, __VA_ARGS__))				\
  (P00_TYPEDEF_MODE (PROTO, __VA_ARGS__) P00_TYPEDEF_MODE (DESC, __VA_ARGS__))

#define P00_IS_ATTRIBUTES_EQ_ATTRIBUTES(...) 0 /* help macro for ATTRIBUTES test IF clause */
#define P00_REMOVE_ATTRIBUTES(...) __VA_ARGS__
#define P00_GET_FIRST_ATTRIBUTES(FIRST, ...) FIRST /* extract typedef attributes */
#define P00_GET_OTHER_ATTRIBUTES(FIRST, ...) __VA_ARGS__ /* extract typedef comments and extended meta information */

/* Outputs only arguments that start with ATTRIBUTES. Removes key word ATTRIBUTES and parenthesis. */
#define P00_EXTRACT_ATTRIBUTES(ARG)					\
  MR_IF_ELSE (MR_HAS_COMMA (P00_IS__EQ__ ARG)) ()			\
  (MR_IF_ELSE (MR_PASTE2 (P00_IS_ATTRIBUTES_EQ_, ARG)) () (MR_PASTE2 (P00_REMOVE_, ARG)))

/* Outputs arguments that do not start with ATTRIBUTES. Delemit them with comma */
#define P00_EXTRACT_NON_ATTRIBUTES(ARG)					\
  MR_IF_ELSE (MR_HAS_COMMA (P00_IS__EQ__ ARG)) (ARG, )			\
  (MR_IF_ELSE (MR_PASTE2 (P00_IS_ATTRIBUTES_EQ_, ARG)) (ARG, ) ())

#define P00_GET_ATTRIBUTES(...) MR_FOREACH (P00_EXTRACT_ATTRIBUTES, __VA_ARGS__)
#define P00_GET_NON_ATTRIBUTES(...) MR_FOREACH (P00_EXTRACT_NON_ATTRIBUTES, __VA_ARGS__)

/*
  TYPEDEF_{STRUCT|UNION|ENUM|CHAR_ARRAY|FUNC} might have one of arguments with prefix ATTRIBUTES (...).
  This key word denotes type attributes, comments and extended meta information.
  Next macro moves ATTRIBUTES argument to first position in arguments list.
*/
#define P00_TYPEDEF_MODE(P00_MODE, P00_TYPE, ...)			\
  P00_TYPEDEF_MODE_ (P00_MODE, P00_TYPE,				\
		     ATTRIBUTES (P00_GET_ATTRIBUTES (__VA_ARGS__)),	\
		     P00_GET_NON_ATTRIBUTES (__VA_ARGS__))
#define P00_TYPEDEF_MODE_(...) P00_TYPEDEF_MODE__ (__VA_ARGS__)
#define P00_TYPEDEF_MODE__(P00_MODE, P00_TYPE, ATTR_COM_EXT, ...)	\
  MR_PASTE2 (P00_TYPEDEF_ATTR_, P00_TYPE) (P00_MODE, P00_TYPE, ATTR_COM_EXT, __VA_ARGS__)

/*
  Here is switch on definition type.
  CHAR_ARRAY and FUNC should be directly expanded to MR_TYPEDEF_CHAR_ARRAY and MR_TYPEDEF_FUNC respectively.
*/
#define P00_TYPEDEF_ATTR_STRUCT TYPEDEF_ATTR
#define P00_TYPEDEF_ATTR_UNION TYPEDEF_ATTR
#define P00_TYPEDEF_ATTR_ENUM TYPEDEF_ATTR
#define P00_TYPEDEF_ATTR_CHAR_ARRAY(P00_MODE, P00_TYPE, ATTR_COM_EXT, P00_TYPE_NAME, SIZE, ...) MR_PASTE2 (MR_TYPEDEF_CHAR_ARRAY_, P00_MODE) (P00_TYPE_NAME, SIZE, MR_PASTE2 (P00_REMOVE_, ATTR_COM_EXT), __VA_ARGS__)
#define P00_TYPEDEF_ATTR_FUNC(P00_MODE, P00_TYPE, ATTR_COM_EXT, RET_TYPE, P00_TYPE_NAME, ARGS, ...) MR_PASTE2 (MR_TYPEDEF_FUNC_, P00_MODE) (RET_TYPE, P00_TYPE_NAME, ARGS, MR_PASTE2 (P00_REMOVE_, ATTR_COM_EXT), __VA_ARGS__)

#define P00_UNFOLD(PREFIX, P00_TYPE, P00_MODE, ...) MR_PASTE4 (PREFIX, P00_TYPE, _, P00_MODE) (__VA_ARGS__)

/*
  Next macro produces typedef prolog, body and epilog.
  Prolog is MR_TYPEDEF_{STRUCT|UNION|ENUM}_{PROTO|DESC} (P00_TYPE_NAME, ATTR...)
  Body is a list of fields or enums definition. Structs and union fields are handled with P00_FIELD. Enums definitions are handled with P00_ENUM_DEF.
  Epilog is MR_END_{STRUCT|UNION|ENUM}_{PROTO|DESC} (P00_TYPE_NAME, COM...)
*/
#define MR_SER(NAME, I, REC, X) REC X

#define TYPEDEF_ATTR(P00_MODE, P00_TYPE, ATTR_COM_EXT, P00_TYPE_NAME, ...) \
  P00_UNFOLD (MR_TYPEDEF_, P00_TYPE, P00_MODE, P00_TYPE_NAME, MR_PASTE2 (P00_GET_FIRST_, ATTR_COM_EXT)) \
  MR_FOR ((P00_MODE, P00_TYPE_NAME), MR_NARG (__VA_ARGS__), MR_SER, MR_PASTE3 (P00_, P00_TYPE, _HANDLER), __VA_ARGS__) \
  P00_UNFOLD (MR_END_, P00_TYPE, P00_MODE, P00_TYPE_NAME, MR_PASTE2 (P00_GET_OTHER_, ATTR_COM_EXT))

#define P00_STRUCT_HANDLER P00_FIELD
#define P00_UNION_HANDLER P00_FIELD
#define P00_ENUM_HANDLER P00_ENUM_DEF

/* field handler checks for trailing empty field */
#define P00_FIELD(P00_MODE_TYPE_NAME, FIELD, I) MR_IF_ELSE (MR_IS_EMPTY (FIELD)) () (P00_FIELD_ (P00_MODE_TYPE_NAME, FIELD))
/*
  field descriptions might be in two forms.
  1. MR_TYPE_MACRO (ARGS) for type scpecific declarations like INT32 (x)
  2. (TYPE, NAME, SUFFIX..., COMMENT..., EXT...) for auto detection declarations.
*/
#define P00_FIELD_(P00_MODE_TYPE_NAME, FIELD)				\
  MR_IF_ELSE (MR_IS_IN_PAREN (FIELD))					\
  (P00_FIELD_DETECT (P00_MODE_TYPE_NAME, FIELD, MR_GET_SUFFIX FIELD))	\
  (P00_FIELD_UNFOLD (P00_MODE_TYPE_NAME, FIELD))

#define MR_GET_SUFFIX(...) MR_GET_SUFFIX_ (__VA_ARGS__, ,)
#define MR_GET_SUFFIX_(_0, _1, _2, ...) _2
/*
  There are 3 options for auto-detection:
  a. Suffix with parentheses goes to FUNC as function.
  b. Non-empty suffix without parentheses goes to ARRAY as array.
  c. Everything else goes to AUTO.
*/
#define P00_FIELD_DETECT(P00_MODE_TYPE_NAME, FIELD, SUFFIX)	\
  MR_IF_ELSE (MR_IS_IN_PAREN (SUFFIX))				\
  (P00_FIELD_UNFOLD (P00_MODE_TYPE_NAME, FUNC FIELD))		\
  (MR_IF_ELSE (MR_IS_EMPTY (SUFFIX))				\
   (P00_FIELD_UNFOLD (P00_MODE_TYPE_NAME, AUTO FIELD))		\
   (P00_FIELD_UNFOLD (P00_MODE_TYPE_NAME, ARRAY FIELD)))

#define P00_GET_MODE(P00_MODE, P00_TYPE_NAME) P00_MODE
#define P00_GET_TYPE_NAME(P00_MODE, P00_TYPE_NAME) P00_TYPE_NAME
/*
  Field type prefix should be extracted as separate macro argument. So we add prefix P00_COMMA_ and expect that in next macro field prefix will be substituted on comma delimitted MR_ type prefix.
  The 3rd macro unfolds to MR_{AUTO|INT32|...}_{PROTO|DESC} (P00_TYPE_NAME, ARGS...)
  Last one detects unkown field qualifiers.
*/
#define P00_FIELD_UNFOLD(P00_MODE_TYPE_NAME, FIELD) P00_FIELD_UNFOLD_ (P00_MODE_TYPE_NAME, FIELD, MR_PASTE2 (P00_COMMA_, FIELD))
#define P00_FIELD_UNFOLD_(...) P00_FIELD_UNFOLD__ (__VA_ARGS__)
#define P00_FIELD_UNFOLD__(P00_MODE_TYPE_NAME, FIELD, P00_FIELD_COMMA, ...) \
  MR_IF_ELSE (MR_IS_EMPTY (__VA_ARGS__))				\
  (P00_UNFOLD (MR_, UNKNOWN, P00_GET_MODE P00_MODE_TYPE_NAME, , FIELD))	\
  (P00_UNFOLD (MR_, P00_FIELD_COMMA, P00_GET_MODE P00_MODE_TYPE_NAME, P00_GET_TYPE_NAME P00_MODE_TYPE_NAME, MR_REMOVE_PAREN (__VA_ARGS__)))

/* produce compilation error for unkown field qualifiers */
#define MR_UNKNOWN_PROTO(P00_TYPE_NAME, ...) int _1[#__VA_ARGS__()];
#define MR_UNKNOWN_DESC(P00_TYPE_NAME, ...) { #__VA_ARGS__(), },

/*
  Check for empty trailing enum definition
  Enums definition might be in two forms.
  1. just a enum name
  2. (ENUM_NAME, RIGHT_HAND_SIDE..., COMMENT..., EXT...) enum and some extra information in parentheses
  both unfolds into MR_ENUM_DEF_{PROTO|DESC} (P00_TYPE_NAME, ARGS)
*/
#define P00_ENUM_DEF(P00_MODE_TYPE_NAME, FIELD, I)			\
  MR_IF_ELSE (MR_IS_EMPTY (FIELD))					\
  ()									\
  (P00_UNFOLD (MR_, ENUM_DEF, P00_GET_MODE P00_MODE_TYPE_NAME, P00_GET_TYPE_NAME P00_MODE_TYPE_NAME, MR_REMOVE_PAREN (FIELD)))

/* list of substitutions for P00_FIELD_UNFOLD_ */
#define P00_COMMA_FIELD FIELD,
#define P00_COMMA_AUTO AUTO,
#define P00_COMMA_NONE NONE,
#define P00_COMMA_ENUM ENUM,
#define P00_COMMA_BITFIELD BITFIELD,
#define P00_COMMA_BITMASK BITMASK,
#define P00_COMMA_INT8 INT8,
#define P00_COMMA_UINT8 UINT8,
#define P00_COMMA_INT16 INT16,
#define P00_COMMA_UINT16 UINT16,
#define P00_COMMA_INT32 INT32,
#define P00_COMMA_UINT32 UINT32,
#define P00_COMMA_INT64 INT64,
#define P00_COMMA_UINT64 UINT64,
#define P00_COMMA_FLOAT FLOAT,
#define P00_COMMA_DOUBLE DOUBLE,
#define P00_COMMA_LONG_DOUBLE LONG_DOUBLE,
#define P00_COMMA_CHAR CHAR,
#define P00_COMMA_STRING STRING,
#define P00_COMMA_CHAR_ARRAY CHAR_ARRAY,
#define P00_COMMA_STRUCT STRUCT,
#define P00_COMMA_UNION UNION,
#define P00_COMMA_ARRAY ARRAY,
#define P00_COMMA_POINTER POINTER,
#define P00_COMMA_POINTER_STRUCT POINTER_STRUCT,
#define P00_COMMA_RARRAY RARRAY,
#define P00_COMMA_FUNC FUNC,

#define P00_COMMA_char AUTO_BI, char,
#define P00_COMMA_short AUTO_BI, short,
#define P00_COMMA_int AUTO_BI, int,
#define P00_COMMA_unsigned AUTO_BI, unsigned,
#define P00_COMMA_signed AUTO_BI, signed,
#define P00_COMMA_long AUTO_BI, long,
#define P00_COMMA_float AUTO_BI, float,
#define P00_COMMA_double AUTO_BI, double,
#define P00_COMMA_int8_t AUTO_BI, int8_t,
#define P00_COMMA_uint8_t AUTO_BI, uint8_t,
#define P00_COMMA_int16_t AUTO_BI, int16_t,
#define P00_COMMA_uint16_t AUTO_BI, uint16_t,
#define P00_COMMA_int32_t AUTO_BI, int32_t,
#define P00_COMMA_uint32_t AUTO_BI, uint32_t,
#define P00_COMMA_int64_t AUTO_BI, int64_t,
#define P00_COMMA_uint64_t AUTO_BI, uint64_t,
#define P00_COMMA_long_double_t AUTO_BI, long_double_t,
#define P00_COMMA_string_t AUTO_BI, string_t,
#define P00_COMMA_mr_ptr_t AUTO_BI, mr_ptr_t,
#define P00_COMMA_volatile AUTO_BI, volatile,
#define P00_COMMA___volatile AUTO_BI, __volatile,
#define P00_COMMA___volatile__ AUTO_BI, __volatile__,
#define P00_COMMA_const AUTO_BI, const,
#define P00_COMMA___const AUTO_BI, __const,
#define P00_COMMA___const__ AUTO_BI, __const__,

#define MR_AUTO_BI_PROTO(...) MR_AUTO_BI_0 (PROTO, __VA_ARGS__)
#define MR_AUTO_BI_DESC(...) MR_AUTO_BI_0 (DESC, __VA_ARGS__)

#define MR_AUTO_BI_0(P00_MODE, MR_TYPE_NAME, TYPE, NAME) MR_IF_ELSE (MR_HAS_COMMA (MR_PASTE2 (MR_IS_BUILTIN_, NAME))) (MR_AUTO_BI_1_ (P00_MODE, MR_TYPE_NAME, TYPE MR_PASTE2 (MR_IS_BUILTIN_, NAME))) (MR_PASTE2 (MR_AUTO_, P00_MODE) (MR_TYPE_NAME, TYPE, NAME))
#define MR_AUTO_BI_1_(...) MR_AUTO_BI_1 (__VA_ARGS__)
#define MR_AUTO_BI_1(P00_MODE, MR_TYPE_NAME, TYPE, NAME) MR_IF_ELSE (MR_HAS_COMMA (MR_PASTE2 (MR_IS_BUILTIN_, NAME))) (MR_AUTO_BI_2_ (P00_MODE, MR_TYPE_NAME, TYPE MR_PASTE2 (MR_IS_BUILTIN_, NAME))) (MR_PASTE2 (MR_AUTO_, P00_MODE) (MR_TYPE_NAME, TYPE, NAME))
#define MR_AUTO_BI_2_(...) MR_AUTO_BI_2 (__VA_ARGS__)
#define MR_AUTO_BI_2(P00_MODE, MR_TYPE_NAME, TYPE, NAME) MR_IF_ELSE (MR_HAS_COMMA (MR_PASTE2 (MR_IS_BUILTIN_, NAME))) (MR_AUTO_BI_3_ (P00_MODE, MR_TYPE_NAME, TYPE MR_PASTE2 (MR_IS_BUILTIN_, NAME))) (MR_PASTE2 (MR_AUTO_, P00_MODE) (MR_TYPE_NAME, TYPE, NAME))
#define MR_AUTO_BI_3_(...) MR_AUTO_BI_3 (__VA_ARGS__)
#define MR_AUTO_BI_3(P00_MODE, MR_TYPE_NAME, TYPE, NAME) MR_IF_ELSE (MR_HAS_COMMA (MR_PASTE2 (MR_IS_BUILTIN_, NAME))) (MR_AUTO_BI_4_ (P00_MODE, MR_TYPE_NAME, TYPE MR_PASTE2 (MR_IS_BUILTIN_, NAME))) (MR_PASTE2 (MR_AUTO_, P00_MODE) (MR_TYPE_NAME, TYPE, NAME))
#define MR_AUTO_BI_4_(...) MR_AUTO_BI_4 (__VA_ARGS__)
#define MR_AUTO_BI_4(P00_MODE, MR_TYPE_NAME, TYPE, NAME) MR_IF_ELSE (MR_HAS_COMMA (MR_PASTE2 (MR_IS_BUILTIN_, NAME))) (MR_AUTO_BI_5_ (P00_MODE, MR_TYPE_NAME, TYPE MR_PASTE2 (MR_IS_BUILTIN_, NAME))) (MR_PASTE2 (MR_AUTO_, P00_MODE) (MR_TYPE_NAME, TYPE, NAME))
#define MR_AUTO_BI_5_(...) MR_AUTO_BI_5 (__VA_ARGS__)
#define MR_AUTO_BI_5(P00_MODE, MR_TYPE_NAME, TYPE, NAME) MR_PASTE2 (MR_AUTO_, P00_MODE) (MR_TYPE_NAME, TYPE, NAME)

#define MR_IS_BUILTIN_char char,
#define MR_IS_BUILTIN_short short,
#define MR_IS_BUILTIN_int int,
#define MR_IS_BUILTIN_unsigned unsigned,
#define MR_IS_BUILTIN_signed signed,
#define MR_IS_BUILTIN_long long,
#define MR_IS_BUILTIN_float float,
#define MR_IS_BUILTIN_double double,
#define MR_IS_BUILTIN_int8_t int8_t,
#define MR_IS_BUILTIN_uint8_t uint8_t,
#define MR_IS_BUILTIN_int16_t int16_t,
#define MR_IS_BUILTIN_uint16_t uint16_t,
#define MR_IS_BUILTIN_int32_t int32_t,
#define MR_IS_BUILTIN_uint32_t uint32_t,
#define MR_IS_BUILTIN_int64_t int64_t,
#define MR_IS_BUILTIN_uint64_t uint64_t,
#define MR_IS_BUILTIN_long_double_t long_double_t,
#define MR_IS_BUILTIN_string_t string_t,
#define MR_IS_BUILTIN_mr_ptr_t mr_ptr_t,
#define MR_IS_BUILTIN_volatile volatile,
#define MR_IS_BUILTIN___volatile __volatile,
#define MR_IS_BUILTIN___volatile__ __volatile__,
#define MR_IS_BUILTIN_const const,
#define MR_IS_BUILTIN___const __const,
#define MR_IS_BUILTIN___const__ __const__,

/* NB! for p99 mode only one anonymous union in struct is possible and it has default name */
#define P00_COMMA_ANON_UNION ANON_UNION,
#define P00_COMMA_END_ANON_UNION END_ANON_UNION,

#define MR_UNIQ_NAME(LINE) name_ ## LINE
#define MR_COMPILETIME_ASSERT(X) MR_COMPILETIME_ASSERT_ (X, __LINE__)
#define MR_COMPILETIME_ASSERT_(X, LINE) typedef struct { int:-!!(X); } MR_UNIQ_NAME (LINE)
/*
  For types defined using standard language approach you will need to create analog types with metaresc.
  For double checking of types costincency you will need the following macro. It compares size and offset of fields in two types.
  Usage: MR_COMPARE_COMPAUND_TYPES (system_type, metaresc_type, commonly_named_field, (field_for_system_type, field_for_metaresc_type), ...)
  Macro evaluates to 0 at compile time if all fields are compatible. Otherwise it is non-zero.
*/
#define MR_BOR(NAME, I, REC, X) ((REC) | (X))
#define MR_COMPARE_COMPAUND_TYPES(TYPE1, TYPE2, ...) ((sizeof (TYPE1) != sizeof (TYPE2)) | MR_FOR ((TYPE1, TYPE2), MR_NARG (__VA_ARGS__), MR_BOR, P00_COMPARE_FIELDS, __VA_ARGS__))
#define P00_GET_FIRST(_1, _2) _1
#define P00_GET_SECOND(_1, _2) _2
#define P00_COMPARE_FIELDS(TYPE1_TYPE2, NAME, I)			\
  MR_IF_ELSE (MR_IS_IN_PAREN (NAME))					\
  (MR_COMPARE_FIELDS (P00_GET_FIRST TYPE1_TYPE2, P00_GET_FIRST NAME, P00_GET_SECOND TYPE1_TYPE2, P00_GET_SECOND NAME)) \
  (MR_COMPARE_FIELDS (P00_GET_FIRST TYPE1_TYPE2, NAME, P00_GET_SECOND TYPE1_TYPE2, NAME))

#ifndef MR_COMPARE_FIELDS_EXT
#define MR_COMPARE_FIELDS_EXT(...) 0
/*
  if your types contains only builtin types then you can do more precies comparation.
  #undef MR_COMPARE_FIELDS_EXT
  #define MR_COMPARE_FIELDS_EXT(TYPE1, NAME1, TYPE2, NAME2) !__builtin_types_compatible_p (typeof (((TYPE1*)NULL)->NAME1), typeof (((TYPE2*)NULL)->NAME2))
*/
#endif /* MR_COMPARE_FIELDS_EXT */
#define MR_COMPARE_FIELDS(TYPE1, NAME1, TYPE2, NAME2) (offsetof (TYPE1, NAME1) != offsetof (TYPE2, NAME2)) | (sizeof (((TYPE1*)NULL)->NAME1) != sizeof (((TYPE2*)NULL)->NAME2)) | MR_COMPARE_FIELDS_EXT (TYPE1, NAME1, TYPE2, NAME2)

#define MR_UNFOLD(NODE, ...) MR_PASTE3 (NODE, _, MR_MODE) (MR_TYPE_NAME, __VA_ARGS__)

#define MR_TYPEDEF_STRUCT(...) MR_UNFOLD (MR_TYPEDEF_STRUCT, __VA_ARGS__)

#define MR_AUTO(...) MR_UNFOLD (MR_AUTO, __VA_ARGS__)
#define MR_NONE(...) MR_UNFOLD (MR_NONE, __VA_ARGS__)
#define MR_ENUM(...) MR_UNFOLD (MR_ENUM, __VA_ARGS__)
#define MR_BITFIELD(...) MR_UNFOLD (MR_BITFIELD, __VA_ARGS__)
#define MR_BITMASK(...) MR_UNFOLD (MR_BITMASK, __VA_ARGS__)
#define MR_INT8(...) MR_UNFOLD (MR_INT8, __VA_ARGS__)
#define MR_UINT8(...) MR_UNFOLD (MR_UINT8, __VA_ARGS__)
#define MR_INT16(...) MR_UNFOLD (MR_INT16, __VA_ARGS__)
#define MR_UINT16(...) MR_UNFOLD (MR_UINT16, __VA_ARGS__)
#define MR_INT32(...) MR_UNFOLD (MR_INT32, __VA_ARGS__)
#define MR_UINT32(...) MR_UNFOLD (MR_UINT32, __VA_ARGS__)
#define MR_INT64(...) MR_UNFOLD (MR_INT64, __VA_ARGS__)
#define MR_UINT64(...) MR_UNFOLD (MR_UINT64, __VA_ARGS__)
#define MR_FLOAT(...) MR_UNFOLD (MR_FLOAT, __VA_ARGS__)
#define MR_DOUBLE(...) MR_UNFOLD (MR_DOUBLE, __VA_ARGS__)
#define MR_LONG_DOUBLE(...) MR_UNFOLD (MR_LONG_DOUBLE, __VA_ARGS__)
#define MR_CHAR(...) MR_UNFOLD (MR_CHAR, __VA_ARGS__)
#define MR_STRING(...) MR_UNFOLD (MR_STRING, __VA_ARGS__)
#define MR_CHAR_ARRAY(...) MR_UNFOLD (MR_CHAR_ARRAY, __VA_ARGS__)
#define MR_STRUCT(...) MR_UNFOLD (MR_STRUCT, __VA_ARGS__)
#define MR_UNION(...) MR_UNFOLD (MR_UNION, __VA_ARGS__)
#define MR_ARRAY(...) MR_UNFOLD (MR_ARRAY, __VA_ARGS__)
#define MR_POINTER(...) MR_UNFOLD (MR_POINTER, __VA_ARGS__)
#define MR_POINTER_STRUCT(...) MR_UNFOLD (MR_POINTER_STRUCT, __VA_ARGS__)
#define MR_RARRAY(...) MR_UNFOLD (MR_RARRAY, __VA_ARGS__)
#define MR_FUNC(...) MR_UNFOLD (MR_FUNC, __VA_ARGS__)
#define MR_END_STRUCT(...) MR_UNFOLD (MR_END_STRUCT, __VA_ARGS__)

#define MR_TYPEDEF_UNION(...) MR_UNFOLD (MR_TYPEDEF_UNION, __VA_ARGS__)
#define MR_END_UNION(...) MR_UNFOLD (MR_END_UNION, __VA_ARGS__)
#define MR_ANON_UNION(...) MR_UNFOLD (MR_ANON_UNION, __VA_ARGS__)
#define MR_END_ANON_UNION(...) MR_UNFOLD (MR_END_ANON_UNION, __VA_ARGS__)

#define MR_TYPEDEF_ENUM(...) MR_UNFOLD (MR_TYPEDEF_ENUM, __VA_ARGS__)
#define MR_ENUM_DEF(...) MR_UNFOLD (MR_ENUM_DEF, __VA_ARGS__)
#define MR_END_ENUM(...) MR_UNFOLD (MR_END_ENUM, __VA_ARGS__)

#define MR_TYPEDEF_CHAR_ARRAY(...) MR_UNFOLD (MR_TYPEDEF_CHAR_ARRAY, __VA_ARGS__)
#define MR_TYPEDEF_FUNC(...) MR_UNFOLD (MR_TYPEDEF_FUNC, __VA_ARGS__)

/* Macroses for prototypes generation mode */
#define MR_TYPEDEF_STRUCT_PROTO(MR_TYPE_NAME, /* ATTR */ ...) typedef struct MR_TYPEDEF_PREFIX (MR_TYPE_NAME) MR_TYPE_NAME; struct __VA_ARGS__ MR_TYPEDEF_PREFIX (MR_TYPE_NAME) {
#define MR_END_STRUCT_PROTO(MR_TYPE_NAME, ...) };
#define MR_TYPEDEF_UNION_PROTO(MR_TYPE_NAME, /* ATTR */ ...) typedef __VA_ARGS__ union MR_TYPEDEF_PREFIX (MR_TYPE_NAME) {
#define MR_END_UNION_PROTO(MR_TYPE_NAME, ...) } MR_TYPE_NAME;

/* next macro adds empty argument. Required for MR_AUTO, MR_NONE, MR_CHAR_ARRAY with two parameters. It adds 3rd parameter (suffix) for them. */
#define MR_AUTO_PROTO(...) MR_FIELD_PROTO (__VA_ARGS__, )
#define MR_NONE_PROTO(...) MR_FIELD_PROTO (__VA_ARGS__, )
#define MR_CHAR_ARRAY_PROTO(...) MR_FIELD_PROTO (__VA_ARGS__, )

#define MR_FIELD_PROTO(MR_TYPE_NAME, TYPE, NAME, SUFFIX, ...) TYPE NAME SUFFIX;
#define MR_ENUM_PROTO(MR_TYPE_NAME, TYPE, NAME, ...) MR_FIELD_PROTO (MR_TYPE_NAME, TYPE, NAME, )
#define MR_BITFIELD_PROTO(MR_TYPE_NAME, TYPE, NAME, SUFFIX, ...) MR_FIELD_PROTO (MR_TYPE_NAME, TYPE, NAME, SUFFIX)
#define MR_BITMASK_PROTO(MR_TYPE_NAME, TYPE, NAME, ...) MR_FIELD_PROTO (MR_TYPE_NAME, TYPE, NAME, )
#define MR_INT8_PROTO(MR_TYPE_NAME, NAME, ...) MR_FIELD_PROTO (MR_TYPE_NAME, int8_t, NAME, )
#define MR_UINT8_PROTO(MR_TYPE_NAME, NAME, ...) MR_FIELD_PROTO (MR_TYPE_NAME, uint8_t, NAME, )
#define MR_INT16_PROTO(MR_TYPE_NAME, NAME, ...) MR_FIELD_PROTO (MR_TYPE_NAME, int16_t, NAME, )
#define MR_UINT16_PROTO(MR_TYPE_NAME, NAME, ...) MR_FIELD_PROTO (MR_TYPE_NAME, uint16_t, NAME, )
#define MR_INT32_PROTO(MR_TYPE_NAME, NAME, ...) MR_FIELD_PROTO (MR_TYPE_NAME, int32_t, NAME, )
#define MR_UINT32_PROTO(MR_TYPE_NAME, NAME, ...) MR_FIELD_PROTO (MR_TYPE_NAME, uint32_t, NAME, )
#define MR_INT64_PROTO(MR_TYPE_NAME, NAME, ...) MR_FIELD_PROTO (MR_TYPE_NAME, int64_t, NAME, )
#define MR_UINT64_PROTO(MR_TYPE_NAME, NAME, ...) MR_FIELD_PROTO (MR_TYPE_NAME, uint64_t, NAME, )
#define MR_FLOAT_PROTO(MR_TYPE_NAME, NAME, ...) MR_FIELD_PROTO (MR_TYPE_NAME, float, NAME, )
#define MR_DOUBLE_PROTO(MR_TYPE_NAME, NAME, ...) MR_FIELD_PROTO (MR_TYPE_NAME, double, NAME, )
#define MR_LONG_DOUBLE_PROTO(MR_TYPE_NAME, NAME, ...) MR_FIELD_PROTO (MR_TYPE_NAME, long double, NAME, )
#define MR_CHAR_PROTO(MR_TYPE_NAME, NAME, ...) MR_FIELD_PROTO (MR_TYPE_NAME, char, NAME, )
#define MR_STRING_PROTO(MR_TYPE_NAME, NAME, ...) MR_FIELD_PROTO (MR_TYPE_NAME, char *, NAME, )
#define MR_STRUCT_PROTO(MR_TYPE_NAME, TYPE, NAME, ...) MR_FIELD_PROTO (MR_TYPE_NAME, TYPE, NAME, )
#define MR_UNION_PROTO(MR_TYPE_NAME, TYPE, NAME, ...) MR_FIELD_PROTO (MR_TYPE_NAME, TYPE, NAME, )
#define MR_ARRAY_PROTO MR_FIELD_PROTO
#define MR_POINTER_PROTO(MR_TYPE_NAME, TYPE, NAME, ...) MR_FIELD_PROTO (MR_TYPE_NAME, TYPE *, NAME, )
#define MR_POINTER_STRUCT_PROTO(MR_TYPE_NAME, TYPE, NAME, ...) MR_FIELD_PROTO (MR_TYPE_NAME, struct MR_TYPEDEF_PREFIX (TYPE) *, NAME, )
/* rarray defenition should be syncroonized with mr_rarray_t type definition */
#define MR_RARRAY_PROTO(MR_TYPE_NAME, TYPE, NAME, ...) MR_FIELD_PROTO (MR_TYPE_NAME, struct __attribute__((packed)) {TYPE * data; typeof (((mr_rarray_t*)NULL)->size) size; typeof (((mr_rarray_t*)NULL)->alloc_size) alloc_size; typeof (((mr_rarray_t*)NULL)->ext) ext; typeof (((mr_rarray_t*)NULL)->ptr_type) ptr_type;}, NAME, )
#define MR_FUNC_PROTO(MR_TYPE_NAME, TYPE, NAME, ARGS, ...) MR_FIELD_PROTO (MR_TYPE_NAME, TYPE, (*NAME), ARGS)

#define MR_ANON_UNION_PROTO(MR_TYPE_NAME, NAME, ... /* ATTR */) MR_IF_ELSE (MR_IS_EMPTY (NAME)) () (char NAME[0];) union __VA_ARGS__ {
#define MR_END_ANON_UNION_PROTO(MR_TYPE_NAME, ...) };

#define MR_TYPEDEF_ENUM_PROTO(MR_TYPE_NAME, /* ATTR */ ...) typedef enum __VA_ARGS__ MR_TYPEDEF_PREFIX (MR_TYPE_NAME) {
#define MR_ENUM_DEF_PROTO(MR_TYPE_NAME, NAME, ...) MR_ENUM_DEF_PROTO_ (MR_TYPE_NAME, NAME, __VA_ARGS__)
#define MR_ENUM_DEF_PROTO_(MR_TYPE_NAME, NAME, RHS, ...) NAME RHS,
#define MR_END_ENUM_PROTO(MR_TYPE_NAME, ...) } MR_TYPE_NAME;

#define MR_TYPEDEF_CHAR_ARRAY_PROTO(MR_TYPE_NAME, SIZE, /* ATTR */...) MR_TYPEDEF_CHAR_ARRAY_PROTO_ (MR_TYPE_NAME, SIZE, __VA_ARGS__)
#define MR_TYPEDEF_CHAR_ARRAY_PROTO_(MR_TYPE_NAME, SIZE, ATTR, /* COMMENT */ ...) typedef ATTR char MR_TYPE_NAME[SIZE];
#define MR_TYPEDEF_FUNC_PROTO(RET_TYPE, MR_TYPE_NAME, ARGS, /* ATTR */ ...) MR_TYPEDEF_FUNC_PROTO_ (RET_TYPE, MR_TYPE_NAME, ARGS, __VA_ARGS__)
#define MR_TYPEDEF_FUNC_PROTO_(RET_TYPE, MR_TYPE_NAME, ARGS, ATTR, /* COMMENT */ ...) typedef ATTR RET_TYPE (*MR_TYPE_NAME) ARGS;

/* Macroses for descriptors generation mode */

#define MR_TYPEDEF_STRUCT_DESC(MR_TYPE_NAME, /* ATTR */ ...) MR_TYPEDEF_DESC (MR_TYPE_NAME, MR_TYPE_STRUCT, __VA_ARGS__)

#define MR_FIELD_DESC(MR_TYPE_NAME, TYPE, NAME, SUFFIX, MR_TYPE, MR_TYPE_EXT, /* COMMENT */ ...) { \
    (mr_fd_t[]){ {							\
	.name = { .str = MR_STRINGIFY (NAME), .hash_value = 0, },	\
	  .type = MR_STRINGIFY (TYPE),					\
	     .size = sizeof (TYPE),					\
	     .offset = offsetof (MR_TYPE_NAME, NAME),			\
	     .mr_type = MR_TYPE,					\
	     .mr_type_ext = MR_TYPE_EXT,				\
	     .param =							\
	     {								\
	       .array_param = {						\
		 .count = sizeof (TYPE) == 0 ? 0 :			\
		 sizeof (((MR_TYPE_NAME*)NULL)->NAME) / sizeof (TYPE),	\
		 .row_count = 1,					\
	       },							\
	     },								\
	     .comment = "" __VA_ARGS__,					\
		} } },

#define MR_POINTER_STRUCT_DESC(MR_TYPE_NAME, TYPE, NAME, /* COMMENT */ ...) { \
    (mr_fd_t[]){ {							\
	.name = { .str = #NAME, .hash_value = 0, },			\
	  .type = MR_STRINGIFY (TYPE),					\
	     .offset = offsetof (MR_TYPE_NAME, NAME),			\
	     .mr_type = MR_TYPE_STRUCT,					\
	     .mr_type_ext = MR_TYPE_EXT_POINTER,			\
	     .comment = "" __VA_ARGS__,					\
	     } } },

#define MR_ARRAY_DESC(MR_TYPE_NAME, TYPE, NAME, SUFFIX, /* COMMENT */ ...) { \
    (mr_fd_t[]){ {							\
	.name = { .str = #NAME, .hash_value = 0, },			\
	  .type = MR_STRINGIFY (TYPE),					\
	     .size = sizeof (TYPE),					\
	     .offset = offsetof (MR_TYPE_NAME, NAME),			\
	     .mr_type = MR_TYPE_DETECT (TYPE),				\
	     .mr_type_ext = MR_TYPE_EXT_ARRAY,				\
	     .param =							\
	     {								\
	       .array_param = {						\
		 .count = sizeof (TYPE) == 0 ? 0 :			\
		 sizeof (((MR_TYPE_NAME*)NULL)->NAME) / sizeof (TYPE),	\
		 .row_count = sizeof (TYPE) == 0 ? 0 :			\
		 sizeof (((MR_TYPE_NAME*)NULL)->NAME[0]) / sizeof (TYPE), \
	       },							\
	     },								\
	     .comment = "" __VA_ARGS__,					\
		} } },

#define MR_NONE_DESC_(MR_TYPE_NAME, TYPE, NAME, SUFFIX, /* COMMENT */ ...) { \
    (mr_fd_t[]){ {							\
	.name = { .str = MR_STRINGIFY (NAME), .hash_value = 0, },	\
	  .type = MR_STRINGIFY (TYPE),					\
	     .size = sizeof (TYPE),					\
	     .mr_type = MR_TYPE_VOID,					\
	     .mr_type_ext = MR_TYPE_EXT_NONE,				\
	     .comment = "" __VA_ARGS__,					\
	     } } },

#define MR_BITFIELD_DESC(MR_TYPE_NAME, TYPE, NAME, SUFFIX, /* COMMENT */ ...) { \
    (mr_fd_t[]){ {							\
	.name = { .str = MR_STRINGIFY (NAME), .hash_value = 0, },	\
	  .type = MR_STRINGIFY (TYPE),					\
	     .size = sizeof (TYPE),					\
	     .mr_type = MR_TYPE_BITFIELD,				\
	     .mr_type_aux = MR_TYPE_DETECT (TYPE),			\
	     .mr_type_ext = MR_TYPE_EXT_NONE,				\
	     .param = {							\
	  .bitfield_param = {						\
	    .bitfield = {						\
	      .size = sizeof (MR_TYPE_NAME),				\
	      .alloc_size = -1,						\
	      .data = (uint8_t*)((MR_TYPE_NAME[]){ { .NAME = -1 } }),	\
	      .ptr_type = MR_RARRAY_OPAQUE_DATA_T_STR,			\
	    }, }, },							\
	     .comment = "" __VA_ARGS__,					\
		} } },

#define MR_AUTO_DESC_(MR_TYPE_NAME, TYPE, NAME, SUFFIX, /* COMMENTS */ ...) MR_FIELD_DESC (MR_TYPE_NAME, TYPE, NAME, SUFFIX, MR_TYPE_DETECT (TYPE), MR_TYPE_EXT_DETECT (TYPE, ((MR_TYPE_NAME*)NULL)->NAME), __VA_ARGS__, .mr_type_aux = MR_TYPE_DETECT_PTR (TYPE))

#define MR_AUTO_DESC(MR_TYPE_NAME, TYPE, NAME, ...) MR_AUTO_DESC_ (MR_TYPE_NAME, TYPE, NAME, __VA_ARGS__)
#define MR_NONE_DESC(MR_TYPE_NAME, TYPE, NAME, ...) MR_NONE_DESC_ (MR_TYPE_NAME, TYPE, NAME, __VA_ARGS__)
#define MR_CHAR_ARRAY_DESC(MR_TYPE_NAME, TYPE, NAME, ...) MR_CHAR_ARRAY_DESC_ (MR_TYPE_NAME, TYPE, NAME, __VA_ARGS__)

#define MR_ENUM_DESC(MR_TYPE_NAME, TYPE, NAME, /* COMMENTS */ ...) MR_FIELD_DESC (MR_TYPE_NAME, TYPE, NAME, , MR_TYPE_ENUM, MR_TYPE_EXT_NONE, __VA_ARGS__)
#define MR_BITMASK_DESC(MR_TYPE_NAME, TYPE, NAME, /* COMMENTS */ ...) MR_FIELD_DESC (MR_TYPE_NAME, TYPE, NAME, , MR_TYPE_BITMASK, MR_TYPE_EXT_NONE, __VA_ARGS__)
#define MR_INT8_DESC(MR_TYPE_NAME, NAME, /* COMMENTS */ ...) MR_FIELD_DESC (MR_TYPE_NAME, int8_t, NAME, , MR_TYPE_INT8, MR_TYPE_EXT_NONE, __VA_ARGS__)
#define MR_UINT8_DESC(MR_TYPE_NAME, NAME, /* COMMENTS */ ...) MR_FIELD_DESC (MR_TYPE_NAME, uint8_t, NAME, , MR_TYPE_UINT8, MR_TYPE_EXT_NONE, __VA_ARGS__)
#define MR_INT16_DESC(MR_TYPE_NAME, NAME, /* COMMENTS */ ...) MR_FIELD_DESC (MR_TYPE_NAME, int16_t, NAME, , MR_TYPE_INT16, MR_TYPE_EXT_NONE, __VA_ARGS__)
#define MR_UINT16_DESC(MR_TYPE_NAME, NAME, /* COMMENTS */ ...) MR_FIELD_DESC (MR_TYPE_NAME, uint16_t, NAME, , MR_TYPE_UINT16, MR_TYPE_EXT_NONE, __VA_ARGS__)
#define MR_INT32_DESC(MR_TYPE_NAME, NAME, /* COMMENTS */ ...) MR_FIELD_DESC (MR_TYPE_NAME, int32_t, NAME, , MR_TYPE_INT32, MR_TYPE_EXT_NONE, __VA_ARGS__)
#define MR_UINT32_DESC(MR_TYPE_NAME, NAME, /* COMMENTS */ ...) MR_FIELD_DESC (MR_TYPE_NAME, uint32_t, NAME, , MR_TYPE_UINT32, MR_TYPE_EXT_NONE, __VA_ARGS__)
#define MR_INT64_DESC(MR_TYPE_NAME, NAME, /* COMMENTS */ ...) MR_FIELD_DESC (MR_TYPE_NAME, int64_t, NAME, , MR_TYPE_INT64, MR_TYPE_EXT_NONE, __VA_ARGS__)
#define MR_UINT64_DESC(MR_TYPE_NAME, NAME, /* COMMENTS */ ...) MR_FIELD_DESC (MR_TYPE_NAME, uint64_t, NAME, , MR_TYPE_UINT64, MR_TYPE_EXT_NONE, __VA_ARGS__)
#define MR_FLOAT_DESC(MR_TYPE_NAME, NAME, /* COMMENTS */ ...) MR_FIELD_DESC (MR_TYPE_NAME, float, NAME, , MR_TYPE_FLOAT, MR_TYPE_EXT_NONE, __VA_ARGS__)
#define MR_DOUBLE_DESC(MR_TYPE_NAME, NAME, /* COMMENTS */ ...) MR_FIELD_DESC (MR_TYPE_NAME, double, NAME, , MR_TYPE_DOUBLE, MR_TYPE_EXT_NONE, __VA_ARGS__)
#define MR_LONG_DOUBLE_DESC(MR_TYPE_NAME, NAME, /* COMMENTS */ ...) MR_FIELD_DESC (MR_TYPE_NAME, long double, NAME, , MR_TYPE_LONG_DOUBLE, MR_TYPE_EXT_NONE, __VA_ARGS__)
#define MR_CHAR_DESC(MR_TYPE_NAME, NAME, /* COMMENTS */ ...) MR_FIELD_DESC (MR_TYPE_NAME, char, NAME, , MR_TYPE_CHAR, MR_TYPE_EXT_NONE, __VA_ARGS__)
#define MR_STRING_DESC(MR_TYPE_NAME, NAME, /* COMMENTS */ ...) MR_FIELD_DESC (MR_TYPE_NAME, char *, NAME, , MR_TYPE_STRING, MR_TYPE_EXT_NONE, __VA_ARGS__)
#define MR_CHAR_ARRAY_DESC_(MR_TYPE_NAME, TYPE, NAME, SUFFIX, /* COMMENTS */ ...) MR_FIELD_DESC (MR_TYPE_NAME, TYPE, NAME, SUFFIX, MR_TYPE_CHAR_ARRAY, MR_TYPE_EXT_NONE, __VA_ARGS__)
#define MR_STRUCT_DESC(MR_TYPE_NAME, TYPE, NAME, /* COMMENTS */ ...) MR_FIELD_DESC (MR_TYPE_NAME, TYPE, NAME, , MR_TYPE_STRUCT, MR_TYPE_EXT_NONE, __VA_ARGS__)
#define MR_UNION_DESC(MR_TYPE_NAME, TYPE, NAME, /* COMMENTS */ ...) MR_FIELD_DESC (MR_TYPE_NAME, TYPE, NAME, , MR_TYPE_UNION, MR_TYPE_EXT_NONE, __VA_ARGS__)
#define MR_POINTER_DESC(MR_TYPE_NAME, TYPE, NAME, /* COMMENTS */ ...) MR_FIELD_DESC (MR_TYPE_NAME, TYPE, NAME, , MR_TYPE_DETECT (TYPE), MR_TYPE_EXT_POINTER, __VA_ARGS__)
#define MR_RARRAY_DESC(MR_TYPE_NAME, TYPE, NAME, /* COMMENTS */ ...) MR_FIELD_DESC (MR_TYPE_NAME, TYPE, NAME, , MR_TYPE_DETECT (TYPE), MR_TYPE_EXT_RARRAY, __VA_ARGS__)
#define MR_FUNC_DESC(MR_TYPE_NAME, TYPE, NAME, ARGS, /* COMMENTS */ ...) MR_FIELD_DESC (MR_TYPE_NAME, TYPE, NAME, , MR_TYPE_FUNC, MR_TYPE_EXT_NONE, __VA_ARGS__, .param = { .func_param = { .alloc_size = -1, .size = 0, .data = (mr_fd_t []){ MR_FUNC_ARG (TYPE, "return value") MR_FOREACH (MR_FUNC_ARG, MR_REMOVE_PAREN (ARGS)) { .mr_type = MR_TYPE_TRAILING_RECORD, }, }, }, })
#define MR_FUNC_ARG(TYPE, /* COMMENTS */ ...) {			\
    .name = { .str = MR_STRINGIFY (TYPE), .hash_value = 0, },	\
      .type = MR_STRINGIFY (TYPE),				\
	 .size = sizeof (TYPE),					\
	 .mr_type = MR_TYPE_DETECT (TYPE),			\
	 .mr_type_aux = MR_TYPE_DETECT_PTR (TYPE),		\
	 .mr_type_ext = MR_TYPE_EXT_NONE,			\
	 .comment = "" __VA_ARGS__,				\
	 },
#define MR_END_STRUCT_DESC(MR_TYPE_NAME, /* COMMENTS */ ...) MR_TYPEDEF_END_DESC (MR_TYPE_NAME, __VA_ARGS__)

#define MR_TYPEDEF_UNION_DESC(MR_TYPE_NAME, /* ATTR */ ...) MR_TYPEDEF_DESC (MR_TYPE_NAME, MR_TYPE_UNION, __VA_ARGS__)
#define MR_END_UNION_DESC(MR_TYPE_NAME, /* COMMENTS */ ...) MR_TYPEDEF_END_DESC (MR_TYPE_NAME, __VA_ARGS__)

#define MR_ANON_UNION_DESC(MR_TYPE_NAME, NAME, ... /* ATTR */) {	\
    (mr_fd_t[]){ {							\
	.name = { .str = MR_STRINGIFY (NAME), .hash_value = 0, },	\
	  .type = "",							\
	     .offset = 0,						\
	     .mr_type = MR_IF_ELSE (MR_IS_EMPTY (NAME)) (MR_TYPE_ANON_UNION) (MR_TYPE_NAMED_ANON_UNION), \
	     .mr_type_ext = MR_TYPE_EXT_NONE,				\
	     .comment = #__VA_ARGS__,					\
	     .ext = { (mr_td_t[]){ { .name = { .str = (char []) {MR_TYPE_ANONYMOUS_UNION_TEMPLATE "9999"}, .hash_value = 0, }, } } }, \
	     .ptr_type = "mr_td_t",					\
		} } },
#define MR_END_ANON_UNION_DESC(MR_TYPE_NAME, /* COMMENTS */ ...) {	\
    (mr_fd_t[]){ {							\
	.type = "",							\
	  .mr_type = MR_TYPE_END_ANON_UNION,				\
	  .mr_type_ext = MR_TYPE_EXT_NONE,				\
	  .comment = "" __VA_ARGS__,					\
	  } } },

#define MR_TYPEDEF_ENUM_DESC(MR_TYPE_NAME, /* ATTR */ ...) MR_TYPEDEF_DESC (MR_TYPE_NAME, MR_TYPE_ENUM, __VA_ARGS__)
#define MR_ENUM_DEF_DESC(MR_TYPE_NAME, NAME, ...) MR_ENUM_DEF_DESC_(MR_TYPE_NAME, NAME, __VA_ARGS__)
#define MR_ENUM_DEF_DESC_(MR_TYPE_NAME, NAME, RHS, /* COMMENTS */ ...) { \
    (mr_fd_t[]){ {							\
	.type = MR_STRINGIFY (MR_TYPE_NAME),				\
	  .name = { .str = MR_STRINGIFY (NAME), .hash_value = 0, },	\
	  .mr_type = MR_TYPE_ENUM_VALUE,				\
	     .mr_type_ext = MR_TYPE_EXT_NONE,				\
	     .param = { .enum_value = NAME, },				\
	     .comment = "" __VA_ARGS__,					\
		} } },
#define MR_END_ENUM_DESC(MR_TYPE_NAME, /* COMMENTS */ ...) MR_TYPEDEF_END_DESC (MR_TYPE_NAME, __VA_ARGS__)

#define MR_FUNC_ARG_PTR(...) { (mr_fd_t[]){ MR_FUNC_ARG (__VA_ARGS__) } }, 

#define MR_TYPEDEF_CHAR_ARRAY_DESC(MR_TYPE_NAME, SIZE, /* ATTR */ ...) MR_TYPEDEF_CHAR_ARRAY_DESC_ (MR_TYPE_NAME, SIZE, __VA_ARGS__)
#define MR_TYPEDEF_CHAR_ARRAY_DESC_(MR_TYPE_NAME, SIZE, ATTR, /* COMMENTS */ ...) MR_TYPEDEF_DESC (MR_TYPE_NAME, MR_TYPE_CHAR_ARRAY, ATTR) MR_TYPEDEF_END_DESC (MR_TYPE_NAME, __VA_ARGS__)
#define MR_TYPEDEF_FUNC_DESC(RET_TYPE, MR_TYPE_NAME, ARGS, /* ATTR */ ...) MR_TYPEDEF_FUNC_DESC_ (RET_TYPE, MR_TYPE_NAME, ARGS, __VA_ARGS__)
#define MR_TYPEDEF_FUNC_DESC_(RET_TYPE, MR_TYPE_NAME, ARGS, ATTR, /* COMMENTS */ ...) \
  MR_TYPEDEF_DESC (MR_TYPE_NAME, MR_TYPE_FUNC_TYPE, ATTR)		\
  MR_FUNC_ARG_PTR (RET_TYPE, "return value")				\
  MR_FOREACH (MR_FUNC_ARG_PTR, MR_REMOVE_PAREN (ARGS))			\
  MR_TYPEDEF_END_DESC (MR_TYPE_NAME, __VA_ARGS__)

#define MR_TYPEDEF_DESC(MR_TYPE_NAME, MR_TYPE, /* ATTR */ ...)		\
  MR_DESCRIPTOR_ATTR mr_td_t MR_DESCRIPTOR_PREFIX (MR_TYPE_NAME) = {	\
    .name = { .str = #MR_TYPE_NAME, .hash_value = 0, },		\
    .mr_type = MR_TYPE,							\
    .mr_type_effective = MR_TYPE_DETECT (MR_TYPE_NAME),			\
    .size = sizeof (MR_TYPE_NAME),					\
    .attr = #__VA_ARGS__,						\
    .fields = { .alloc_size = -1, .size = 0, .data = (mr_fd_ptr_t[]){
#define MR_TYPEDEF_END_DESC(MR_TYPE_NAME, /* COMMENTS */ ...){		\
    (mr_fd_t[]){ {							\
	.type = #MR_TYPE_NAME,						\
	  .mr_type = MR_TYPE_TRAILING_RECORD,				\
	  } } } } },							\
    .comment = "" __VA_ARGS__ };					\
  static inline void __attribute__((constructor)) MR_CONSTRUCTOR_PREFIX (MR_TYPE_NAME) (void) { MR_ADD_TYPE (MR_TYPE_NAME); }

/*
  Macro for type registration.
  Usage:
  MR_ADD_TYPE(foo_t);
  MR_ADD_TYPE(foo_t, "Text Meta Info");
  MR_ADD_TYPE(foo_t, "TMI", "One more TMI");
  MR_ADD_TYPE(foo_t, "TMI", &meta_info_struct);
  MR_ADD_TYPE(foo_t, "TMI", &meta_info_struct, unused, unused, unused);
  By default comment and .ext void pointer could initialized in MR_END_STRUCT & MR_END_ENUM.
  Arguments in MR_ADD_TYPE will override those settings.
*/
#define MR_ADD_TYPE(MR_TYPE_NAME, /* COMMENTS */ ...) mr_add_type (&MR_DESCRIPTOR_PREFIX (MR_TYPE_NAME), "" __VA_ARGS__, NULL)

/*
  User can turn off strict types checking for Metaresc macroses, so compilation will produce only warnings.
  MR_CHECK_TYPES macro should be predefined as
  #define MR_CHECK_TYPES(...)
*/
#ifndef MR_CHECK_TYPES
#define MR_CHECK_TYPES(MR_TYPE_NAME, ...) ({ (void) ((MR_TYPE_NAME*)0 - (typeof (__VA_ARGS__ + 0))0); })
#endif /* MR_CHECK_TYPES */

#define MR_COPY_RECURSIVELY(MR_TYPE_NAME, ...) MR_COPY_RECURSIVELY_ (MR_TYPE_NAME, __VA_ARGS__, 3, 2)
#define MR_COPY_RECURSIVELY_(MR_TYPE_NAME, S_PTR, D_PTR, N, ...) MR_PASTE2 (MR_COPY_RECURSIVELY_ARG, N) (MR_TYPE_NAME, S_PTR, D_PTR)
#define MR_COPY_RECURSIVELY_ARG3(MR_TYPE_NAME, S_PTR, D_PTR) ({ MR_CHECK_TYPES (MR_TYPE_NAME, D_PTR); mr_copy_recursively (MR_SAVE (MR_TYPE_NAME, S_PTR), D_PTR); })
#define MR_COPY_RECURSIVELY_ARG2(MR_TYPE_NAME, S_PTR, D_PTR) ({ MR_TYPE_NAME dst; mr_copy_recursively (MR_SAVE (MR_TYPE_NAME, S_PTR), &dst); dst; })
#define MR_FREE_RECURSIVELY(MR_TYPE_NAME, S_PTR) mr_free_recursively (MR_SAVE (MR_TYPE_NAME, S_PTR))

#define MR_SAVE(MR_TYPE_NAME, S_PTR) ({					\
      mr_fd_t __fd__ =							\
	{								\
	  .name = { .str = MR_STRINGIFY (S_PTR), .hash_value = 0, },	\
	  .type = #MR_TYPE_NAME,					\
	  .mr_type = MR_TYPE_DETECT (MR_TYPE_NAME),			\
	  .mr_type_ext = MR_TYPE_EXT_DETECT (MR_TYPE_NAME, S_PTR),	\
	  .size = sizeof (MR_TYPE_NAME),				\
	  .param = 							\
	  {								\
	    .array_param = {						\
	      .count = sizeof (S_PTR) / (sizeof (MR_TYPE_NAME) ? sizeof (MR_TYPE_NAME) : 1), \
	      .row_count = 1,						\
	    },								\
	  },								\
	};								\
      MR_TYPE_NAME * check_type = S_PTR;				\
      mr_save_data_t __mr_save_data__;					\
      mr_detect_type (&__fd__);						\
      __fd__.name.str = mr_normalize_name (__fd__.name.str);		\
      MR_CHECK_TYPES (MR_TYPE_NAME, S_PTR);				\
      if (check_type == NULL)						\
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_NULL_POINTER);		\
      else								\
	mr_save (check_type, &__fd__, &__mr_save_data__);		\
      __mr_save_data__.ptrs;						\
    })

#define MR_SAVE_XDR(MR_TYPE_NAME, XDRS, S_PTR) ({			\
      int __status__ = 0;						\
      XDR * __xdrs__ = (XDRS);						\
      if (XDR_ENCODE != __xdrs__->x_op)					\
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_XDR_WRONG_ENCODING_MODE);	\
      else								\
	{								\
	  mr_ra_mr_ptrdes_t __ptrs__ = MR_SAVE (MR_TYPE_NAME, S_PTR);	\
	  if (__ptrs__.ra.data)						\
	    {								\
	      __status__ = xdr_save (__xdrs__, &__ptrs__);		\
	      MR_FREE (__ptrs__.ra.data);				\
	    }								\
	}								\
      __status__;							\
    })

#define MR_SAVE_XDR_RA(MR_TYPE_NAME, S_PTR) ({				\
      XDR _xdrs_;							\
      mr_rarray_t _ra_ = { .alloc_size = 0, .size = 0, .data = NULL, }; \
      xdrra_create (&_xdrs_, &_ra_, XDR_ENCODE);			\
      if (0 == MR_SAVE_XDR (MR_TYPE_NAME, &_xdrs_, S_PTR))		\
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_XDR_SAVE_FAILED);		\
      _ra_;								\
    })

#define MR_SAVE_METHOD(METHOD, MR_TYPE_NAME, S_PTR) ({			\
      mr_ra_mr_ptrdes_t __ptrs__ = MR_SAVE (MR_TYPE_NAME, S_PTR);	\
      char * __str__ = NULL;						\
      if (__ptrs__.ra.data)						\
	{								\
	  __str__ = METHOD (&__ptrs__);					\
	  MR_FREE (__ptrs__.ra.data);					\
	}								\
      __str__;								\
    })

#define MR_SAVE_XML1(MR_TYPE_NAME, S_PTR) MR_SAVE_METHOD (xml1_save, MR_TYPE_NAME, S_PTR)
#define MR_SAVE_CINIT(MR_TYPE_NAME, S_PTR) MR_SAVE_METHOD (cinit_save, MR_TYPE_NAME, S_PTR)
#define MR_SAVE_JSON(MR_TYPE_NAME, S_PTR) MR_SAVE_METHOD (json_save, MR_TYPE_NAME, S_PTR)
#define MR_SAVE_SCM(MR_TYPE_NAME, S_PTR) MR_SAVE_METHOD (scm_save, MR_TYPE_NAME, S_PTR)

#define MR_SAVE_METHOD_RA(METHOD, MR_TYPE_NAME, S_PTR) ({		\
      mr_rarray_t _ra_ = { .alloc_size = 0, .size = 0, .data = NULL, }; \
      _ra_.data = METHOD (MR_TYPE_NAME, S_PTR);				\
      if (_ra_.data) _ra_.size = _ra_.alloc_size = strlen (_ra_.data) + 1; \
      _ra_;								\
    })

#define MR_SAVE_XML_RA(MR_TYPE_NAME, S_PTR) MR_SAVE_METHOD_RA (MR_SAVE_XML, MR_TYPE_NAME, S_PTR)
#define MR_SAVE_XML1_RA(MR_TYPE_NAME, S_PTR) MR_SAVE_METHOD_RA (MR_SAVE_XML1, MR_TYPE_NAME, S_PTR)
#define MR_SAVE_CINIT_RA(MR_TYPE_NAME, S_PTR) MR_SAVE_METHOD_RA (MR_SAVE_CINIT, MR_TYPE_NAME, S_PTR)
#define MR_SAVE_JSON_RA(MR_TYPE_NAME, S_PTR) MR_SAVE_METHOD_RA (MR_SAVE_JSON, MR_TYPE_NAME, S_PTR)
#define MR_SAVE_SCM_RA(MR_TYPE_NAME, S_PTR) MR_SAVE_METHOD_RA (MR_SAVE_SCM, MR_TYPE_NAME, S_PTR)
  
#define MR_LOAD_XDR_ARG3(MR_TYPE_NAME, XDRS, S_PTR) ({			\
      int __status__ = 0;						\
      XDR * __xdrs__ = (XDRS);						\
      if (XDR_DECODE != __xdrs__->x_op)					\
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_XDR_WRONG_ENCODING_MODE);	\
      else								\
	{								\
	  mr_fd_t __fd__ = {						\
	    .type = #MR_TYPE_NAME,					\
	    .name = { .str = NULL, .hash_value = 0, },			\
	    .mr_type = MR_TYPE_DETECT (MR_TYPE_NAME),			\
	    .mr_type_ext = MR_TYPE_EXT_NONE,				\
	    .size = sizeof (MR_TYPE_NAME),				\
	    .param = 							\
	    {								\
	      .array_param = {						\
		.count = sizeof (S_PTR) / (sizeof (MR_TYPE_NAME) ? sizeof (MR_TYPE_NAME) : 1), \
		.row_count = 1,						\
	      },							\
	    },								\
	  };								\
	  MR_TYPE_NAME * __check_type__ = S_PTR + 0;			\
	  mr_detect_type (&__fd__);					\
	  MR_CHECK_TYPES (MR_TYPE_NAME, S_PTR);				\
	  if (__check_type__ != NULL)					\
	    __status__ = xdr_load (__check_type__, &__fd__, __xdrs__, NULL); \
	  else								\
	    MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_NULL_POINTER);		\
	}								\
      __status__;							\
    })

#define MR_LOAD_XDR_ARG2_(MR_TYPE_NAME, XDRS) ({			\
      int _status_ = 0;							\
      MR_TYPE_NAME __result__;						\
      memset (&__result__, 0, sizeof (__result__));			\
      _status_ = MR_LOAD_XDR_ARG3 (MR_TYPE_NAME, XDRS, &__result__);	\
      if (0 == _status_)						\
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_XDR_LOAD_FAILED);		\
      __result__;							\
    })

#define MR_LOAD_XDR(MR_TYPE_NAME, ...) MR_LOAD_XDR_ARGN (MR_TYPE_NAME, __VA_ARGS__, 3, 2)
#define MR_LOAD_XDR_ARGN(MR_TYPE_NAME, XDRS, S_PTR, N, ...) MR_LOAD_XDR_ARG ## N (MR_TYPE_NAME, XDRS, S_PTR)
#define MR_LOAD_XDR_ARG2(MR_TYPE_NAME, XDRS, S_PTR) MR_LOAD_XDR_ARG2_ (MR_TYPE_NAME, XDRS)

#define MR_LOAD_XDR_RA(MR_TYPE_NAME, ...) MR_LOAD_XDR_RA_ARGN (MR_TYPE_NAME, __VA_ARGS__, 3, 2)
#define MR_LOAD_XDR_RA_ARGN(MR_TYPE_NAME, RA, S_PTR, N, ...) MR_LOAD_XDR_RA_ARG ## N (MR_TYPE_NAME, RA, S_PTR)
#define MR_LOAD_XDR_RA_ARG2(MR_TYPE_NAME, RA, S_PTR) ({ XDR _xdrs_; xdrra_create (&_xdrs_, RA, XDR_DECODE); MR_LOAD_XDR (MR_TYPE_NAME, &_xdrs_); })
#define MR_LOAD_XDR_RA_ARG3(MR_TYPE_NAME, RA, S_PTR) ({ XDR _xdrs_; xdrra_create (&_xdrs_, RA, XDR_DECODE); MR_LOAD_XDR (MR_TYPE_NAME, &_xdrs_, S_PTR); })

#ifdef HAVE_LIBXML2

#define MR_SAVE_XML2_RA(MR_TYPE_NAME, S_PTR) MR_SAVE_METHOD_RA (MR_SAVE_XML2, MR_TYPE_NAME, S_PTR)

#define MR_SAVE_XML2(MR_TYPE_NAME, S_PTR) ({				\
      int __size__;							\
      char * __str__ = NULL;						\
      xmlChar * __xml_str__ = NULL;					\
      mr_ra_mr_ptrdes_t __ptrs__ = MR_SAVE (MR_TYPE_NAME, S_PTR);	\
      if (__ptrs__.ra.data)						\
	{								\
	  xmlDocPtr __doc__ = xml2_save (&__ptrs__);			\
	  MR_FREE (__ptrs__.ra.data);					\
	  if (__doc__)							\
	    {								\
	      xmlDocDumpFormatMemory (__doc__, &__xml_str__, &__size__, 1); \
	      xmlFreeDoc (__doc__);					\
	    }								\
	  if (__xml_str__)						\
	    {								\
	      __str__ = MR_STRDUP ((char*)__xml_str__);			\
	      xmlFree (__xml_str__);					\
	    }								\
	}								\
      __str__;								\
    })

#define MR_LOAD_XML2_NODE_ARG3(MR_TYPE_NAME, XML, S_PTR) ({		\
      int __status__ = 0;						\
      int __idx__ = -1;							\
      mr_load_data_t __load_data__ = {					\
	.ptrs = { .ra = { .alloc_size = 0, .size = 0, .data = NULL, } }, \
	.mr_ra_idx = { .alloc_size = 0, .size = 0, .data = NULL, }, };	\
      mr_fd_t __fd__ = {						\
	.type = #MR_TYPE_NAME,						\
	.name = { .str = NULL, .hash_value = 0, },			\
	.mr_type = MR_TYPE_DETECT (MR_TYPE_NAME),			\
	.mr_type_ext = MR_TYPE_EXT_NONE,				\
	.size = sizeof (MR_TYPE_NAME),					\
	.param = 							\
	{								\
	  .array_param = {						\
	    .count = sizeof (S_PTR) / (sizeof (MR_TYPE_NAME) ? sizeof (MR_TYPE_NAME) : 1), \
	    .row_count = 1,						\
	  },								\
	},								\
      };								\
      MR_TYPE_NAME * __check_type__ = S_PTR + 0;			\
      xmlNodePtr __xml__ = (XML);					\
      if (NULL == __xml__)						\
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_NULL_POINTER);		\
      else								\
	{								\
	  MR_CHECK_TYPES (MR_TYPE_NAME, S_PTR);				\
	  mr_detect_type (&__fd__);					\
	  if (NULL != __check_type__) 					\
	    __idx__ = xml2_load (__xml__, &__load_data__.ptrs);		\
	  else								\
	    MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_NULL_POINTER);		\
	  if (__idx__ >= 0)						\
	    __status__ = mr_load (__check_type__, &__fd__, __idx__, &__load_data__); \
	  mr_free_ptrs (__load_data__.ptrs);				\
	}								\
      __status__;							\
    })

#define MR_LOAD_XML2_NODE_ARG2_(MR_TYPE_NAME, XML) ({			\
      int _status_ = 0;							\
      MR_TYPE_NAME __result__;						\
      memset (&__result__, 0, sizeof (__result__));			\
      _status_ = MR_LOAD_XML2_NODE_ARG3 (MR_TYPE_NAME, XML, &__result__); \
      if (0 == _status_)						\
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_LOAD_STRUCT_FAILED);	\
      __result__;							\
    })

#define MR_LOAD_XML2_NODE(MR_TYPE_NAME, ...) MR_LOAD_XML2_NODE_ARGN (MR_TYPE_NAME, __VA_ARGS__, 3, 2)
#define MR_LOAD_XML2_NODE_ARGN(MR_TYPE_NAME, XML, S_PTR, N, ...) MR_LOAD_XML2_NODE_ARG ## N (MR_TYPE_NAME, XML, S_PTR)
#define MR_LOAD_XML2_NODE_ARG2(MR_TYPE_NAME, XML, S_PTR) MR_LOAD_XML2_NODE_ARG2_ (MR_TYPE_NAME, XML)

#define MR_LOAD_XML2_ARG3(MR_TYPE_NAME, STR, S_PTR) ({			\
      int _status_ = 0;							\
      MR_TYPE_NAME * _check_type_ = S_PTR + 0;				\
      char * __str__ = (STR);						\
      MR_CHECK_TYPES (MR_TYPE_NAME, S_PTR);				\
      if (NULL == _check_type_)						\
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_NULL_POINTER);		\
      else if (NULL == __str__)						\
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_STRING_IS_NULL);		\
      else								\
	{								\
	  xmlDocPtr __doc__ = xmlParseMemory (__str__, strlen (__str__)); \
	  if (__doc__)							\
	    {								\
	      xmlNodePtr __node__ = xmlDocGetRootElement (__doc__);	\
	      _status_ = MR_LOAD_XML2_NODE_ARG3 (MR_TYPE_NAME, __node__, _check_type_); \
	      xmlFreeDoc (__doc__);					\
	    }								\
	}								\
      _status_;								\
    })

#define MR_LOAD_XML2_ARG2_(MR_TYPE_NAME, STR) ({			\
      int ___status___ = 0;						\
      MR_TYPE_NAME __result__;						\
      memset (&__result__, 0, sizeof (__result__));			\
      ___status___ = MR_LOAD_XML2_ARG3 (MR_TYPE_NAME, STR, &__result__); \
      if (0 == ___status___)						\
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_LOAD_STRUCT_FAILED);	\
      __result__;							\
    })

#define MR_LOAD_XML2(MR_TYPE_NAME, ...) MR_LOAD_XML2_N (MR_TYPE_NAME, __VA_ARGS__, 3, 2)
#define MR_LOAD_XML2_N(MR_TYPE_NAME, STR, S_PTR, N, ...) MR_LOAD_XML2_ARG ## N (MR_TYPE_NAME, STR, S_PTR)
#define MR_LOAD_XML2_ARG2(MR_TYPE_NAME, STR, S_PTR) MR_LOAD_XML2_ARG2_ (MR_TYPE_NAME, STR)

#endif /* HAVE_LIBXML2 */

#define MR_LOAD_XML2_RA(MR_TYPE_NAME, ...) MR_LOAD_XML2_RA_ARGN (MR_TYPE_NAME, __VA_ARGS__, 3, 2)
#define MR_LOAD_XML2_RA_ARGN(MR_TYPE_NAME, RA, S_PTR, N, ...) MR_LOAD_XML2_ARG ## N (MR_TYPE_NAME, (char*)((RA)->data), S_PTR)

#ifdef HAVE_BISON_FLEX

#define MR_LOAD_METHOD_ARG3(METHOD, MR_TYPE_NAME, STR, S_PTR) ({	\
      int _status_ = 0;							\
      MR_TYPE_NAME * _check_type_ = S_PTR + 0;				\
      char * _str_ = (STR);						\
      MR_CHECK_TYPES (MR_TYPE_NAME, S_PTR);				\
      if (NULL == _check_type_)						\
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_NULL_POINTER);		\
      else if (NULL == _str_)						\
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_STRING_IS_NULL);		\
      else								\
	{								\
	  mr_load_data_t _load_data_ = {				\
	    .ptrs = { .ra = { .alloc_size = 0, .size = 0, .data = NULL, } }, \
	    .mr_ra_idx = { .alloc_size = 0, .size = 0, .data = NULL, }, }; \
	  _status_ = METHOD (_str_, &_load_data_.ptrs);			\
	  if (_status_)							\
	    {								\
	      mr_fd_t _fd_ = {						\
		.type = #MR_TYPE_NAME,					\
		.name = { .str = NULL, .hash_value = 0, },		\
		.mr_type = MR_TYPE_DETECT (MR_TYPE_NAME),		\
		.mr_type_ext = MR_TYPE_EXT_DETECT (MR_TYPE_NAME, S_PTR), \
		.size = sizeof (MR_TYPE_NAME),				\
		.param = 						\
		{							\
		  .array_param = {					\
		    .count = sizeof (S_PTR) / (sizeof (MR_TYPE_NAME) ? sizeof (MR_TYPE_NAME) : 1), \
		    .row_count = 1,					\
		  },							\
		},							\
	      };							\
	      mr_detect_type (&_fd_);					\
	      _status_ = mr_load (_check_type_, &_fd_, 0, &_load_data_); \
	    }								\
	  mr_free_ptrs (_load_data_.ptrs);				\
	}								\
      _status_;								\
    })

#define MR_LOAD_METHOD_ARG2_(METHOD, MR_TYPE_NAME, STR) ({		\
      int _status_ = 0;							\
      MR_TYPE_NAME __result__;						\
      memset (&__result__, 0, sizeof (__result__));			\
      _status_ = MR_LOAD_METHOD_ARG3 (METHOD, MR_TYPE_NAME, STR, &__result__); \
      if (0 == _status_)						\
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_LOAD_STRUCT_FAILED);	\
      __result__;							\
    })

#define MR_LOAD_METHOD(METHOD, MR_TYPE_NAME, ...) MR_LOAD_METHOD_ARGN (METHOD, MR_TYPE_NAME, __VA_ARGS__, 3, 2)
#define MR_LOAD_METHOD_ARGN(METHOD, MR_TYPE_NAME, STR, S_PTR, N, ...) MR_LOAD_METHOD_ARG ## N (METHOD, MR_TYPE_NAME, STR, S_PTR)
#define MR_LOAD_METHOD_ARG2(METHOD, MR_TYPE_NAME, STR, S_PTR) MR_LOAD_METHOD_ARG2_ (METHOD, MR_TYPE_NAME, STR)

#define MR_LOAD_XML1(MR_TYPE_NAME, /* STR */ ...) MR_LOAD_METHOD (xml1_load, MR_TYPE_NAME, __VA_ARGS__)
#define MR_LOAD_CINIT(MR_TYPE_NAME, /* STR */ ...) MR_LOAD_METHOD (cinit_load, MR_TYPE_NAME, __VA_ARGS__)
#define MR_LOAD_JSON MR_LOAD_CINIT
#define MR_LOAD_SCM(MR_TYPE_NAME, /* STR */ ...) MR_LOAD_METHOD (scm_load, MR_TYPE_NAME, __VA_ARGS__)

#define MR_LOAD_METHOD_RA(METHOD, MR_TYPE_NAME, ...) MR_LOAD_METHOD_RA_ARGN (METHOD, MR_TYPE_NAME, __VA_ARGS__, 3, 2)
#define MR_LOAD_METHOD_RA_ARGN(METHOD, MR_TYPE_NAME, RA, S_PTR, N, ...) MR_LOAD_METHOD_ARG ## N (METHOD, MR_TYPE_NAME, (char*)((RA)->data), S_PTR)

#define MR_LOAD_XML1_RA(MR_TYPE_NAME, /* RA */ ...) MR_LOAD_METHOD_RA (xml1_load, MR_TYPE_NAME, __VA_ARGS__)
#define MR_LOAD_CINIT_RA(MR_TYPE_NAME, /* RA */ ...) MR_LOAD_METHOD_RA (cinit_load, MR_TYPE_NAME, __VA_ARGS__)
#define MR_LOAD_JSON_RA MR_LOAD_CINIT_RA
#define MR_LOAD_SCM_RA(MR_TYPE_NAME, /* RA */ ...) MR_LOAD_METHOD_RA (scm_load, MR_TYPE_NAME, __VA_ARGS__)

#else /* ! HAVE_BISON_FLEX */

#define MR_ZERO_RESULT(MR_TYPE_NAME) ({				\
      MR_TYPE_NAME __result__;					\
      memset (&__result__, 0, sizeof (__result__));		\
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_LOAD_METHOD_MISSED);	\
      __result__;						\
    })

#define MR_LOAD_STUB(MR_TYPE_NAME, ...) MR_LOAD_STUB_ARGN (MR_TYPE_NAME, __VA_ARGS__, 3, 2)
#define MR_LOAD_STUB_ARGN(MR_TYPE_NAME, STR, S_PTR, N, ...) MR_LOAD_STUB_ARG ## N (MR_TYPE_NAME, STR, S_PTR)
#define MR_LOAD_STUB_ARG2(MR_TYPE_NAME, STR, S_PTR) (MR_ZERO_RESULT (MR_TYPE_NAME))
#define MR_LOAD_STUB_ARG3(MR_TYPE_NAME, STR, S_PTR) ({ MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_LOAD_METHOD_MISSED); 0;})

#define MR_LOAD_XML1 MR_LOAD_STUB
#define MR_LOAD_CINIT MR_LOAD_STUB
#define MR_LOAD_SCM MR_LOAD_STUB

#endif /* HAVE_BISON_FLEX */

#ifdef HAVE_LIBXML2
#define MR_LOAD_XML MR_LOAD_XML2
#define MR_LOAD_XML_RA MR_LOAD_XML2_RA
#define MR_SAVE_XML MR_SAVE_XML2

#else /* ! HAVE_LIBXML2 */

#define MR_SAVE_XML MR_SAVE_XML1

#ifdef HAVE_BISON_FLEX
#define MR_LOAD_XML MR_LOAD_XML1
#define MR_LOAD_XML_RA MR_LOAD_XML1_RA
#else /* ! HAVE_BISON_FLEX */
#warning There is no support for XML load.
#define MR_LOAD_XML MR_LOAD_STUB
#define MR_LOAD_XML_RA MR_LOAD_STUB
#endif /* HAVE_BISON_FLEX */

#endif /* HAVE_LIBXML2 */

#ifndef MR_MODE
#define MR_MODE_UNDEFINED
#define MR_MODE PROTO
#endif
#include <mr_protos.h>
#ifdef MR_MODE_UNDEFINED
#undef MR_MODE_UNDEFINED
#undef MR_MODE
#endif

#include <mr_tsearch.h>

typedef long double long_double_t;

extern mr_conf_t mr_conf;

extern int __attribute__ ((sentinel(0))) mr_add_type (mr_td_t*, char*, ...);
extern char * mr_read_xml_doc (FILE*);

extern void mr_save (void*, mr_fd_t*, mr_save_data_t*);
extern int mr_load (void*, mr_fd_t*, int, mr_load_data_t*);
#ifdef HAVE_LIBXML2
extern xmlDocPtr xml2_save (mr_ra_mr_ptrdes_t*);
extern int xml2_load (xmlNodePtr, mr_ra_mr_ptrdes_t*);
#endif /* HAVE_LIBXML2 */
extern int xdr_save (XDR*, mr_ra_mr_ptrdes_t*);
extern int xdr_load (void*, mr_fd_t*, XDR*, mr_ra_mr_ptrdes_t*);
extern void xdrra_create (XDR*, mr_rarray_t*, enum xdr_op);

extern char * xml1_save (mr_ra_mr_ptrdes_t*);
#ifdef HAVE_BISON_FLEX
extern int xml1_load (char*, mr_ra_mr_ptrdes_t*);
#endif /* HAVE_BISON_FLEX */
extern char * cinit_save (mr_ra_mr_ptrdes_t*);
#ifdef HAVE_BISON_FLEX
extern int cinit_load (char*, mr_ra_mr_ptrdes_t*);
#endif /* HAVE_BISON_FLEX */
extern char * json_save (mr_ra_mr_ptrdes_t*);

extern char * scm_save (mr_ra_mr_ptrdes_t*);
#ifdef HAVE_BISON_FLEX
extern int scm_load (char*, mr_ra_mr_ptrdes_t*);
#endif /* HAVE_BISON_FLEX */

extern int mr_add_ptr_to_list (mr_ra_mr_ptrdes_t*);
extern void mr_add_child (int, int, mr_ra_mr_ptrdes_t*);
extern void mr_detect_type (mr_fd_t*);
extern char * mr_normalize_name (char*);
extern int mr_free_recursively (mr_ra_mr_ptrdes_t);
extern int mr_copy_recursively (mr_ra_mr_ptrdes_t, void*);
extern int mr_free_ptrs (mr_ra_mr_ptrdes_t);
extern mr_fd_t * mr_get_fd_by_name (mr_td_t*, char*);
extern mr_fd_t * mr_get_enum_by_value (mr_td_t*, int64_t);
extern mr_fd_t * mr_get_enum_by_name (char*);
extern int mr_parse_add_node (mr_load_t*);
extern int mr_load_bitfield_value (mr_ptrdes_t*, uint64_t*);
extern int mr_save_bitfield_value (mr_ptrdes_t*, uint64_t*);
extern mr_td_t * mr_get_td_by_name (char*);
extern char * mr_message_format (mr_message_id_t, va_list);
extern void mr_message (const char*, const char*, int, mr_log_level_t, mr_message_id_t, ...);
extern void mr_message_unsupported_node_type (mr_fd_t*);
extern void * mr_rarray_append (mr_rarray_t*, int);
extern int __attribute__ ((format (printf, 2, 3))) mr_ra_printf (mr_rarray_t*, const char*, ...);

extern char * mr_stringify_int8_t (mr_ptrdes_t*);
extern char * mr_stringify_uint8_t (mr_ptrdes_t*);
extern char * mr_stringify_int16_t (mr_ptrdes_t*);
extern char * mr_stringify_uint16_t (mr_ptrdes_t*);
extern char * mr_stringify_int32_t (mr_ptrdes_t*);
extern char * mr_stringify_uint32_t (mr_ptrdes_t*);
extern char * mr_stringify_int64_t (mr_ptrdes_t*);
extern char * mr_stringify_uint64_t (mr_ptrdes_t*);
extern char * mr_stringify_float (mr_ptrdes_t*);
extern char * mr_stringify_double (mr_ptrdes_t*);
extern char * mr_stringify_long_double_t (mr_ptrdes_t*);
extern char * mr_stringify_enum (mr_ptrdes_t*);
extern char * mr_stringify_bitfield (mr_ptrdes_t*);
extern char * mr_stringify_bitmask (mr_ptrdes_t*, char*);
extern char * mr_stringify_func (mr_ptrdes_t*);

extern char * xml_quote_string (char*);
extern char * xml_unquote_string (char*, int);
extern void mr_init_save_xml (void);

extern unsigned int mr_hash_str (char*);
extern int mr_hashed_string_cmp (const mr_ptr_t, const mr_ptr_t, const void *);
extern unsigned int mr_hashed_string_get_hash (mr_ptr_t, const void *);

#endif /* _METARESC_H_ */
