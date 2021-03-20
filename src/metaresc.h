/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

/* Metaresc prototypes */
#ifndef _METARESC_H_
#define _METARESC_H_

#include <mr_pp.h>
#include <mr_export.h>

#ifndef __USE_XOPEN2K8
#define __USE_XOPEN2K8
#define __USE_XOPEN2K8_UNDEFINED
#endif /* __USE_XOPEN2K8 */

#ifndef __USE_GNU
#define __USE_GNU
#define __USE_GNU_UNDEFINED
#endif /* __USE_GNU */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#define _GNU_SOURCE_UNDEFINED
#endif /* _GNU_SOURCE */

#include <stdio.h> /* for FILE */
#include <stddef.h> /* for offsetof */
#include <stdbool.h> /* for bool */
#include <string.h> /* for strlen () & memset () */
#include <ctype.h> /* for isspace () */
#include <stdarg.h> /* for va_list */
#include <inttypes.h> /* for int8_t, int16_t, int32_t, int64_t, SCNxXX, etc */
#include <sys/types.h> /* for ssize_t */
#include <complex.h> /* for complex */
#ifdef HAVE_LIBXML2
# include <libxml/xmlmemory.h>
# include <libxml/parser.h>
#endif /* HAVE_LIBXML2 */
#ifdef HAVE_RPC_TYPES_H
#include <rpc/types.h>
#include <rpc/xdr.h>
#endif /* HAVE_RPC_TYPES_H */

#ifdef _GNU_SOURCE_UNDEFINED
#undef _GNU_SOURCE_UNDEFINED
#undef _GNU_SOURCE
#endif /* _GNU_SOURCE_UNDEFINED */

#ifdef __USE_GNU_UNDEFINED
#undef __USE_GNU_UNDEFINED
#undef __USE_GNU
#endif /* __USE_GNU_UNDEFINED */

#ifdef __USE_XOPEN2K8_UNDEFINED
#undef __USE_XOPEN2K8_UNDEFINED
#undef __USE_XOPEN2K8
#endif /* __USE_XOPEN2K8_UNDEFINED */

/* Library exports */
#define MR_MAX_STRING_LENGTH ((unsigned int)-1)

#define MR_TYPE_ANONYMOUS_UNION_TEMPLATE "mr_type_anonymous_union_%d_t"

#define MR_INT_TO_STRING_BUF_SIZE (32)
#define MR_FLOAT_TO_STRING_BUF_SIZE (256)
#define MR_CHAR_TO_STRING_BUF_SIZE (8)

#define MR_MAX_INDENT_LEVEL (30) /* the same constant as in libxml2 */
#define MR_MIN(X,Y) ({ __typeof__ (X) _x_ = (X); __typeof__ (Y) _y_ = (Y); (_x_ < _y_) ? _x_ : _y_; })
#define MR_MAX(X,Y) ({ __typeof__ (X) _x_ = (X); __typeof__ (Y) _y_ = (Y); (_x_ > _y_) ? _x_ : _y_; })
#define MR_LIMIT_LEVEL(LEVEL) MR_MIN (LEVEL, MR_MAX_INDENT_LEVEL)

/* each refereed structure will have REF_IDX property */
#define MR_REF_IDX "ref_idx"
/* references on already saved structures will be replaced with nodes that have only REF index property */
#define MR_REF "ref"
#define MR_REF_CONTENT "ref_content"
/* XML attribute for zero length strings */
#define MR_ISNULL "isnull"
#define MR_ISNULL_VALUE "true"
#define MR_OPAQUE_DATA mr_opaque_data
#define MR_OPAQUE_DATA_STR MR_STRINGIFY_READONLY (MR_OPAQUE_DATA)
#define MR_SIZE mr_size
#define MR_SIZE_STR MR_STRINGIFY_READONLY (MR_SIZE)
#define MR_VOIDP_T void*
#define MR_VOIDP_T_STR MR_STRINGIFY_READONLY (MR_VOIDP_T)

#define MR_STRINGIFY_READONLY(...) MR_STRINGIFY_READONLY_ (__VA_ARGS__)
#define MR_STRINGIFY_READONLY_(...) #__VA_ARGS__

#define MR_BITMASK_OR_DELIMITER " | "

#define MR_MESSAGE(LOG_LEVEL, /* MSG_ID */ ...) mr_message (__FILE__, __FUNCTION__, __LINE__, LOG_LEVEL, __VA_ARGS__)
#define MR_MESSAGE_UNSUPPORTED_NODE_TYPE_(FDP)				\
  ({									\
    mr_fd_t * __fdp__ = FDP;						\
    mr_td_t * mr_type_td = mr_get_td_by_name ("mr_type_t");		\
    mr_fd_t * mr_type_fd = mr_type_td ? mr_get_enum_by_value (mr_type_td, __fdp__->mr_type) : NULL; \
    MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNSUPPORTED_NODE_TYPE, (mr_type_fd ? mr_type_fd->name.str : "unknown"), __fdp__->mr_type); \
  })

/* make a string from argument in writable memory. #STR itself is in read-only memory */
#define MR_STRINGIFY(STR) (char []) { #STR }

#define MR_MEM_INIT(FUNC, /* ATTR */ ...) __VA_ARGS__ void mr_mem_init (void) { FUNC; }

#define MR_CALLOC(COUNT, SIZE) (mr_conf.mr_mem.calloc)(__FILE__, __FUNCTION__, __LINE__, COUNT, SIZE)
#define MR_REALLOC(PTR, SIZE) (mr_conf.mr_mem.realloc)(__FILE__, __FUNCTION__, __LINE__, PTR, SIZE)
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
#define MR_TYPE_DETECT(TYPE, ...) MR_TYPE_DETECT_ (TYPE, __VA_ARGS__)
#define MR_TYPE_DETECT_(TYPE, SUFFIX)					\
  (0 /* MR_TYPE_NONE */							\
   | (__builtin_types_compatible_p (void SUFFIX, TYPE) ? MR_TYPE_VOID : 0) \
   | (__builtin_types_compatible_p (bool SUFFIX, TYPE) ? MR_TYPE_BOOL : 0) \
   | (__builtin_types_compatible_p (int8_t SUFFIX, TYPE) ? MR_TYPE_INT8 : 0) \
   | (__builtin_types_compatible_p (uint8_t SUFFIX, TYPE) ? MR_TYPE_UINT8 : 0) \
   | (__builtin_types_compatible_p (int16_t SUFFIX, TYPE) ? MR_TYPE_INT16 : 0) \
   | (__builtin_types_compatible_p (uint16_t SUFFIX, TYPE) ? MR_TYPE_UINT16 : 0) \
   | (__builtin_types_compatible_p (int32_t SUFFIX, TYPE) ? MR_TYPE_INT32 : 0) \
   | (__builtin_types_compatible_p (uint32_t SUFFIX, TYPE) ? MR_TYPE_UINT32 : 0) \
   | (__builtin_types_compatible_p (int64_t SUFFIX, TYPE) ? MR_TYPE_INT64 : 0) \
   | (__builtin_types_compatible_p (uint64_t SUFFIX, TYPE) ? MR_TYPE_UINT64 : 0) \
   | (__builtin_types_compatible_p (MR_UNCOVERED_TYPE SUFFIX, TYPE) ? MR_PASTE2 (MR_TYPE_INT, MR_SIZEOF_UNCOVERED_TYPE) : 0) \
   | (__builtin_types_compatible_p (unsigned MR_UNCOVERED_TYPE SUFFIX, TYPE) ? MR_PASTE2 (MR_TYPE_UINT, MR_SIZEOF_UNCOVERED_TYPE) : 0) \
   | (__builtin_types_compatible_p (float SUFFIX, TYPE) ? MR_TYPE_FLOAT : 0) \
   | (__builtin_types_compatible_p (complex float SUFFIX, TYPE) ? MR_TYPE_COMPLEX_FLOAT : 0) \
   | (__builtin_types_compatible_p (double SUFFIX, TYPE) ? MR_TYPE_DOUBLE : 0) \
   | (__builtin_types_compatible_p (complex double SUFFIX, TYPE) ? MR_TYPE_COMPLEX_DOUBLE : 0) \
   | (__builtin_types_compatible_p (long double SUFFIX, TYPE) ? MR_TYPE_LONG_DOUBLE : 0) \
   | (__builtin_types_compatible_p (complex long double SUFFIX, TYPE) ? MR_TYPE_COMPLEX_LONG_DOUBLE : 0) \
   | (__builtin_types_compatible_p (char SUFFIX, TYPE) ? MR_TYPE_CHAR : 0) \
   | (__builtin_types_compatible_p (__typeof__ (char []) SUFFIX, TYPE) ? MR_TYPE_CHAR_ARRAY : 0) \
   | ((__builtin_types_compatible_p (const volatile __typeof__ (char *) SUFFIX, const volatile TYPE) \
       | __builtin_types_compatible_p (const volatile char * SUFFIX, const volatile TYPE)) ? MR_TYPE_STRING : 0)						\
   )
#define MR_TYPE_DETECT_PTR(TYPE) (MR_TYPE_DETECT (TYPE, *) | MR_TYPE_DETECT (TYPE, const *) | MR_TYPE_DETECT (TYPE, volatile *) | MR_TYPE_DETECT (TYPE, const volatile *))

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

/* Macroses for builtin types meta data registration */
#define MR_TYPEDEF_DESC_BI_(TYPE, MR_TYPE, ...) MR_TYPEDEF_DESC (TYPE, MR_TYPE) MR_TYPEDEF_END_DESC (TYPE, __VA_ARGS__);
#define MR_TYPEDEF_DESC_BI(TYPE, ...) MR_TYPEDEF_DESC_BI_ (TYPE, MR_TYPE_DETECT (TYPE), __VA_ARGS__)

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
  This key word denotes type attributes and meta information.
  Next macro moves ATTRIBUTES argument to first position in arguments list.
*/
#define P00_TYPEDEF_MODE(P00_MODE, P00_TYPE, ...)			\
  P00_TYPEDEF_MODE_ (P00_MODE, P00_TYPE,				\
		     ATTRIBUTES (P00_GET_ATTRIBUTES (__VA_ARGS__)),	\
		     P00_GET_NON_ATTRIBUTES (__VA_ARGS__))
#define P00_TYPEDEF_MODE_(...) P00_TYPEDEF_MODE__ (__VA_ARGS__)
#define P00_TYPEDEF_MODE__(P00_MODE, P00_TYPE, ATTR_META_RES, ...)	\
  MR_PASTE2 (P00_TYPEDEF_ATTR_, P00_TYPE) (P00_MODE, P00_TYPE, ATTR_META_RES, __VA_ARGS__)

/*
  Here is switch on definition type.
  CHAR_ARRAY and FUNC should be directly expanded to MR_TYPEDEF_CHAR_ARRAY and MR_TYPEDEF_FUNC respectively.
*/
#define P00_TYPEDEF_ATTR_STRUCT TYPEDEF_ATTR
#define P00_TYPEDEF_ATTR_UNION TYPEDEF_ATTR
#define P00_TYPEDEF_ATTR_ENUM TYPEDEF_ATTR
#define P00_TYPEDEF_ATTR_CHAR_ARRAY(P00_MODE, P00_TYPE, ATTR_META_RES, P00_TYPE_NAME, SIZE, ...) MR_PASTE2 (MR_TYPEDEF_CHAR_ARRAY_, P00_MODE) (P00_TYPE_NAME, SIZE, MR_PASTE2 (P00_REMOVE_, ATTR_META_RES), __VA_ARGS__)
#define P00_TYPEDEF_ATTR_FUNC(P00_MODE, P00_TYPE, ATTR_META_RES, RET_TYPE, P00_TYPE_NAME, ARGS, ...) MR_PASTE2 (MR_TYPEDEF_FUNC_, P00_MODE) (RET_TYPE, P00_TYPE_NAME, ARGS, MR_PASTE2 (P00_REMOVE_, ATTR_META_RES), __VA_ARGS__)

#define P00_UNFOLD(PREFIX, P00_TYPE, P00_MODE, ...) MR_PASTE4 (PREFIX, P00_TYPE, _, P00_MODE) (__VA_ARGS__)

/*
  Next macro produces typedef prolog, body and epilog.
  Prolog is MR_TYPEDEF_{STRUCT|UNION|ENUM}_{PROTO|DESC} (P00_TYPE_NAME, ATTR...)
  Body is a list of fields or enums definition. Structs and union fields are handled with P00_FIELD. Enums definitions are handled with P00_ENUM_DEF.
  Epilog is MR_END_{STRUCT|UNION|ENUM}_{PROTO|DESC} (P00_TYPE_NAME, META...)
*/
#define MR_SER(NAME, I, REC, X) REC X

#define TYPEDEF_ATTR(P00_MODE, P00_TYPE, ATTR_META_RES, P00_TYPE_NAME, ...) \
  P00_UNFOLD (MR_TYPEDEF_, P00_TYPE, P00_MODE, P00_TYPE_NAME)		\
  MR_FOR ((P00_MODE, P00_TYPE_NAME), MR_NARG (__VA_ARGS__), MR_SER, MR_PASTE3 (P00_, P00_TYPE, _HANDLER), __VA_ARGS__) \
  P00_UNFOLD (MR_END_, P00_TYPE, P00_MODE, P00_TYPE_NAME, MR_PASTE2 (P00_REMOVE_, ATTR_META_RES))

#define P00_STRUCT_HANDLER P00_FIELD
#define P00_UNION_HANDLER P00_FIELD
#define P00_ENUM_HANDLER P00_ENUM_DEF

/* field handler checks for trailing empty field */
#define P00_FIELD(P00_MODE_TYPE_NAME, FIELD, I) MR_IF_ELSE (MR_IS_EMPTY (FIELD)) () (P00_FIELD_ (P00_MODE_TYPE_NAME, FIELD))
/*
  field descriptions might be in two forms.
  1. MR_TYPE_MACRO (ARGS) for type scpecific declarations like VOID (TYPE, NAME)
  2. (TYPE, NAME, SUFFIX..., META..., RES..., RES_TYPE..., RES_SIZE...) for auto detection declarations.
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
  (P00_UNFOLD (MR_, UNKNOWN, P00_GET_MODE P00_MODE_TYPE_NAME, P00_GET_TYPE_NAME P00_MODE_TYPE_NAME, FIELD))	\
  (P00_UNFOLD (MR_, P00_FIELD_COMMA, P00_GET_MODE P00_MODE_TYPE_NAME, P00_GET_TYPE_NAME P00_MODE_TYPE_NAME, MR_REMOVE_PAREN (__VA_ARGS__)))

/* produce compilation error for unkown field qualifiers */
#define MR_UNKNOWN_PROTO(P00_TYPE_NAME, ...) int _1[#__VA_ARGS__()];
/* auto generate descriptor based on field name */
#define MR_UNKNOWN_DESC(P00_TYPE_NAME, ...) MR_AUTO_DESC (P00_TYPE_NAME, /* auto detect type */, __VA_ARGS__)

/*
  Check for empty trailing enum definition
  Enums definition might be in two forms.
  1. just a enum name
  2. (ENUM_NAME, RIGHT_HAND_SIDE..., META..., RESOURCE...) enum and some extra information in parentheses
  both unfolds into MR_ENUM_DEF_{PROTO|DESC} (P00_TYPE_NAME, ARGS)
*/
#define P00_ENUM_DEF(P00_MODE_TYPE_NAME, FIELD, I)			\
  MR_IF_ELSE (MR_IS_EMPTY (FIELD))					\
  ()									\
  (P00_UNFOLD (MR_, ENUM_DEF, P00_GET_MODE P00_MODE_TYPE_NAME, P00_GET_TYPE_NAME P00_MODE_TYPE_NAME, MR_REMOVE_PAREN (FIELD)))

/* list of substitutions for P00_FIELD_UNFOLD_ */
#define P00_COMMA_AUTO AUTO,
#define P00_COMMA__ AUTO,
#define P00_COMMA_VOID VOID,
#define P00_COMMA_BITFIELD BITFIELD,
#define P00_COMMA_CHAR_ARRAY CHAR_ARRAY,
#define P00_COMMA_ARRAY ARRAY,
#define P00_COMMA_POINTER POINTER,
#define P00_COMMA_FUNC FUNC,

#define P00_COMMA_char AUTO_BI, char,
#define P00_COMMA_short AUTO_BI, short,
#define P00_COMMA_int AUTO_BI, int,
#define P00_COMMA_unsigned AUTO_BI, unsigned,
#define P00_COMMA_signed AUTO_BI, signed,
#define P00_COMMA_long AUTO_BI, long,
#define P00_COMMA_float AUTO_BI, float,
#define P00_COMMA_double AUTO_BI, double,
#define P00_COMMA_bool AUTO_BI, bool,
#define P00_COMMA__Bool AUTO_BI, _Bool,
#define P00_COMMA_complex AUTO_BI, complex,
#define P00_COMMA___complex__ AUTO_BI, __complex__,
#define P00_COMMA__Complex AUTO_BI, _Complex,
#define P00_COMMA_int8_t AUTO_BI, int8_t,
#define P00_COMMA_uint8_t AUTO_BI, uint8_t,
#define P00_COMMA_int16_t AUTO_BI, int16_t,
#define P00_COMMA_uint16_t AUTO_BI, uint16_t,
#define P00_COMMA_int32_t AUTO_BI, int32_t,
#define P00_COMMA_uint32_t AUTO_BI, uint32_t,
#define P00_COMMA_int64_t AUTO_BI, int64_t,
#define P00_COMMA_uint64_t AUTO_BI, uint64_t,
#define P00_COMMA_size_t AUTO_BI, size_t,
#define P00_COMMA_ssize_t AUTO_BI, ssize_t,
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
#define MR_IS_BUILTIN_bool bool,
#define MR_IS_BUILTIN__Bool _Bool,
#define MR_IS_BUILTIN_complex complex,
#define MR_IS_BUILTIN___complex__ __complex__,
#define MR_IS_BUILTIN__Complex _Complex,
#define MR_IS_BUILTIN_int8_t int8_t,
#define MR_IS_BUILTIN_uint8_t uint8_t,
#define MR_IS_BUILTIN_int16_t int16_t,
#define MR_IS_BUILTIN_uint16_t uint16_t,
#define MR_IS_BUILTIN_int32_t int32_t,
#define MR_IS_BUILTIN_uint32_t uint32_t,
#define MR_IS_BUILTIN_int64_t int64_t,
#define MR_IS_BUILTIN_uint64_t uint64_t,
#define MR_IS_BUILTIN_size_t size_t,
#define MR_IS_BUILTIN_ssize_t ssize_t,
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

#define MR_UNIQ_NAME(ID) name_ ## ID
#define MR_COMPILETIME_ASSERT(X) MR_COMPILETIME_ASSERT_ (X, __COUNTER__)
#define MR_COMPILETIME_ASSERT_(X, ID) typedef struct { int:-!!(X); } MR_UNIQ_NAME (ID)
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
  #define MR_COMPARE_FIELDS_EXT(TYPE1, NAME1, TYPE2, NAME2) !__builtin_types_compatible_p (__typeof__ (((TYPE1*)0)->NAME1), __typeof__ (((TYPE2*)0)->NAME2))
*/
#endif /* MR_COMPARE_FIELDS_EXT */
#define MR_COMPARE_FIELDS(TYPE1, NAME1, TYPE2, NAME2) (offsetof (TYPE1, NAME1) != offsetof (TYPE2, NAME2)) | (sizeof (((TYPE1*)0)->NAME1) != sizeof (((TYPE2*)0)->NAME2)) | MR_COMPARE_FIELDS_EXT (TYPE1, NAME1, TYPE2, NAME2)

#define MR_UNFOLD(NODE, ...) MR_PASTE3 (NODE, _, MR_MODE) (MR_TYPE_NAME, __VA_ARGS__)

#define MR_TYPEDEF_STRUCT(...) MR_UNFOLD (MR_TYPEDEF_STRUCT, __VA_ARGS__)

#define MR_AUTO(...) MR_UNFOLD (MR_AUTO, __VA_ARGS__)
#define MR_VOID(...) MR_UNFOLD (MR_VOID, __VA_ARGS__)
#define MR_BITFIELD(...) MR_UNFOLD (MR_BITFIELD, __VA_ARGS__)
#define MR_CHAR_ARRAY(...) MR_UNFOLD (MR_CHAR_ARRAY, __VA_ARGS__)
#define MR_ARRAY(...) MR_UNFOLD (MR_ARRAY, __VA_ARGS__)
#define MR_POINTER(...) MR_UNFOLD (MR_POINTER, __VA_ARGS__)
#define MR_FUNC(...) MR_UNFOLD (MR_FUNC, __VA_ARGS__)
#define MR_END_STRUCT(...) MR_UNFOLD (MR_END_STRUCT, __VA_ARGS__)

#define MR_TYPEDEF_UNION(...) MR_UNFOLD (MR_TYPEDEF_UNION, __VA_ARGS__)
#define MR_END_UNION(...) MR_UNFOLD (MR_END_UNION, __VA_ARGS__)
#define MR_ANON_UNION(...) MR_UNFOLD (MR_ANON_UNION, __VA_ARGS__)
#define MR_END_ANON_UNION(...) MR_UNFOLD (MR_END_ANON_UNION, __VA_ARGS__)

#define MR_TYPEDEF_ENUM(...) MR_UNFOLD (MR_TYPEDEF_ENUM, __VA_ARGS__)
#define MR_END_ENUM(...) MR_UNFOLD (MR_END_ENUM, __VA_ARGS__)

#define MR_TYPEDEF_CHAR_ARRAY(...) MR_UNFOLD (MR_TYPEDEF_CHAR_ARRAY, __VA_ARGS__)
#define MR_TYPEDEF_FUNC(...) MR_UNFOLD (MR_TYPEDEF_FUNC, __VA_ARGS__)

/* Macroses for prototypes generation mode */
#define MR_TYPEDEF_STRUCT_PROTO(MR_TYPE_NAME) typedef struct MR_TYPEDEF_PREFIX (MR_TYPE_NAME) MR_TYPE_NAME; struct MR_TYPEDEF_PREFIX (MR_TYPE_NAME) {
#define MR_END_STRUCT_PROTO(MR_TYPE_NAME, ATTR, ...) } ATTR;
#define MR_TYPEDEF_UNION_PROTO(MR_TYPE_NAME) typedef union MR_TYPEDEF_PREFIX (MR_TYPE_NAME) {
#define MR_END_UNION_PROTO(MR_TYPE_NAME, ATTR, ...) } ATTR MR_TYPE_NAME;
#define MR_TYPEDEF_ENUM_PROTO(MR_TYPE_NAME) typedef enum MR_TYPEDEF_PREFIX (MR_TYPE_NAME) {
#define MR_END_ENUM_PROTO(MR_TYPE_NAME, ATTR, ...) } ATTR MR_TYPE_NAME;

/* next macro adds empty argument. Required for MR_AUTO, MR_VOID, MR_CHAR_ARRAY with two parameters. It adds 3rd parameter (suffix) for them. */
#define MR_AUTO_PROTO(...) MR_FIELD_PROTO (__VA_ARGS__, )
#define MR_VOID_PROTO(...) MR_FIELD_PROTO (__VA_ARGS__, )
#define MR_CHAR_ARRAY_PROTO(...) MR_FIELD_PROTO (__VA_ARGS__, )

#define MR_FIELD_PROTO(MR_TYPE_NAME, TYPE, NAME, SUFFIX, ...) TYPE NAME SUFFIX;
#define MR_BITFIELD_PROTO MR_FIELD_PROTO
#define MR_ARRAY_PROTO MR_FIELD_PROTO
#define MR_POINTER_PROTO(MR_TYPE_NAME, TYPE, NAME, ...) MR_FIELD_PROTO (MR_TYPE_NAME, TYPE *, NAME, )
#define MR_FUNC_PROTO(MR_TYPE_NAME, TYPE, NAME, ARGS, ...) MR_FIELD_PROTO (MR_TYPE_NAME, TYPE, (*NAME), ARGS)

#define MR_ANON_UNION_PROTO(MR_TYPE_NAME, NAME, ... /* ATTR */) MR_IF_ELSE (MR_IS_EMPTY (NAME)) () (char NAME[0];) union __VA_ARGS__ {
#define MR_END_ANON_UNION_PROTO(MR_TYPE_NAME, ...) };

#define MR_ENUM_DEF_PROTO(MR_TYPE_NAME, NAME, ...) MR_ENUM_DEF_PROTO_ (MR_TYPE_NAME, NAME, __VA_ARGS__)
#define MR_ENUM_DEF_PROTO_(MR_TYPE_NAME, NAME, RHS, ...) NAME RHS,

#define MR_TYPEDEF_CHAR_ARRAY_PROTO(MR_TYPE_NAME, SIZE, /* ATTR */...) MR_TYPEDEF_CHAR_ARRAY_PROTO_ (MR_TYPE_NAME, SIZE, __VA_ARGS__)
#define MR_TYPEDEF_CHAR_ARRAY_PROTO_(MR_TYPE_NAME, SIZE, ATTR, /* META */ ...) typedef ATTR char MR_TYPE_NAME[SIZE];
#define MR_TYPEDEF_FUNC_PROTO(RET_TYPE, MR_TYPE_NAME, ARGS, /* ATTR */ ...) MR_TYPEDEF_FUNC_PROTO_ (RET_TYPE, MR_TYPE_NAME, ARGS, __VA_ARGS__)
#define MR_TYPEDEF_FUNC_PROTO_(RET_TYPE, MR_TYPE_NAME, ARGS, ATTR, /* META */ ...) typedef ATTR RET_TYPE (*MR_TYPE_NAME) ARGS;

/* Macroses for descriptors generation mode */

#define MR_FIELD_DESC(MR_TYPE_NAME, TYPE, NAME, SUFFIX, MR_TYPE, /* META */ ...) { \
    (mr_fd_t[]){ {							\
	.name = { .str = MR_STRINGIFY (NAME), .hash_value = 0, },	\
	  .type = MR_STRINGIFY (TYPE),					\
	     .size = sizeof (((MR_TYPE_NAME*)0)->NAME),			\
	     .offset = offsetof (MR_TYPE_NAME, NAME),			\
	     .mr_type = MR_TYPE,					\
	     .meta = "" __VA_ARGS__,					\
		} } },

#define MR_ARRAY_DESC(MR_TYPE_NAME, TYPE, NAME, SUFFIX, /* META */ ...) { \
    (mr_fd_t[]){ {							\
	.name = { .str = #NAME, .hash_value = 0, },			\
	  .type = MR_STRINGIFY (TYPE),					\
	     .size = sizeof (((MR_TYPE_NAME*)0)->NAME),			\
	     .offset = offsetof (MR_TYPE_NAME, NAME),			\
	     .mr_type = MR_TYPE_ARRAY,					\
	     .mr_type_aux = MR_TYPE_DETECT (TYPE),			\
	     .param =							\
	     {								\
	       .array_param = {						\
		 .count = (sizeof (TYPE) == 0 ? 0 :			\
			   sizeof (((MR_TYPE_NAME*)0)->NAME) / sizeof (TYPE)), \
		 .row_count = (sizeof (TYPE) == 0 ? 1 :			\
			       sizeof (((MR_TYPE_NAME*)0)->NAME[0]) / sizeof (TYPE)), \
	       },							\
	     },								\
	     .meta = "" __VA_ARGS__,					\
		} } },

#define MR_VOID_DESC_(MR_TYPE_NAME, TYPE, NAME, SUFFIX, /* META */ ...) { \
    (mr_fd_t[]){ {							\
	.name = { .str = MR_STRINGIFY (NAME), .hash_value = 0, },	\
	  .type = MR_STRINGIFY (TYPE),					\
	     .size = sizeof (TYPE),					\
	     MR_IF_ELSE (MR_IS_EMPTY (SUFFIX)) (.offset = offsetof (MR_TYPE_NAME, NAME),) () \
	     .mr_type = MR_TYPE_VOID,					\
	     MR_IF_ELSE (MR_IS_EMPTY (SUFFIX)) (.mr_type_aux = MR_TYPE_DETECT (TYPE),) () \
	     .meta = "" __VA_ARGS__,					\
	     } } },

#define MR_BITFIELD_DESC(MR_TYPE_NAME, TYPE, NAME, SUFFIX, /* META */ ...) { \
    (mr_fd_t[]){ {							\
	.name = { .str = MR_STRINGIFY (NAME), .hash_value = 0, },	\
	  .type = MR_STRINGIFY (TYPE),					\
	     .size = sizeof (TYPE),					\
	     .mr_type = MR_TYPE_BITFIELD,				\
	     .mr_type_aux = MR_TYPE_DETECT (TYPE),			\
	     .param = {							\
	  .bitfield_param = {						\
	    .size = sizeof (MR_TYPE_NAME),				\
	    .bitfield = (uint8_t*)((MR_TYPE_NAME[]){ { .NAME = -1 } }), \
	  }, },								\
	     .meta = "" __VA_ARGS__,					\
		} } },

#define MR_AUTO_DESC_(MR_TYPE_NAME, TYPE, NAME, SUFFIX, /* META */ ...) \
  MR_FIELD_DESC (MR_TYPE_NAME, TYPE, NAME, SUFFIX,			\
		 MR_TYPE_DETECT (TYPE), __VA_ARGS__,			\
		 .self_ptr = __builtin_types_compatible_p (const volatile __typeof__ (MR_TYPE_NAME *), const volatile __typeof__ (((MR_TYPE_NAME*)0)->NAME)) | \
		 __builtin_types_compatible_p (const volatile __typeof__ (MR_TYPE_NAME const *), const volatile __typeof__ (((MR_TYPE_NAME*)0)->NAME)) | \
		 __builtin_types_compatible_p (const volatile __typeof__ (MR_TYPE_NAME volatile *), const volatile __typeof__ (((MR_TYPE_NAME*)0)->NAME)) | \
		 __builtin_types_compatible_p (const volatile __typeof__ (MR_TYPE_NAME const volatile *), const volatile __typeof__ (((MR_TYPE_NAME*)0)->NAME)), \
		 .mr_type_aux = MR_TYPE_DETECT_PTR (TYPE)		\
		 + 0 / __builtin_types_compatible_p (TYPE, __typeof__ (((MR_TYPE_NAME*)0)->NAME)))
/* Generate division by zero error if type of the field mismatches
   with type provided in macro. This is possible for descriptors
   generated for external types */

/* ensure that name is a token without parentheses of braces at the end */
#define MR_AUTO_DESC(MR_TYPE_NAME, TYPE, NAME, ...) MR_AUTO_DESC__ (NAME ## _, MR_TYPE_NAME, TYPE, NAME, __VA_ARGS__)
#define MR_AUTO_DESC__(VALIDATED_NAME, MR_TYPE_NAME, TYPE, NAME, ...) MR_IF_ELSE (MR_IS_EMPTY (TYPE)) \
    (MR_AUTO_DESC_ (MR_TYPE_NAME, __typeof__ (((MR_TYPE_NAME*)0)->NAME), NAME, __VA_ARGS__)) \
    (MR_AUTO_DESC_ (MR_TYPE_NAME, TYPE, NAME, __VA_ARGS__))

#define MR_VOID_DESC(MR_TYPE_NAME, TYPE, NAME, ...) MR_VOID_DESC_ (MR_TYPE_NAME, TYPE, NAME, __VA_ARGS__)
#define MR_CHAR_ARRAY_DESC(MR_TYPE_NAME, TYPE, NAME, ...) MR_CHAR_ARRAY_DESC_ (MR_TYPE_NAME, TYPE, NAME, __VA_ARGS__)

#define MR_CHAR_ARRAY_DESC_(MR_TYPE_NAME, TYPE, NAME, SUFFIX, /* META */ ...) MR_FIELD_DESC (MR_TYPE_NAME, TYPE, NAME, SUFFIX, MR_TYPE_CHAR_ARRAY, __VA_ARGS__)
#define MR_POINTER_DESC(MR_TYPE_NAME, TYPE, NAME, /* META */ ...) MR_FIELD_DESC (MR_TYPE_NAME, TYPE, NAME, , MR_TYPE_POINTER, __VA_ARGS__, .mr_type_aux = MR_TYPE_DETECT (TYPE))
#define MR_FUNC_DESC(MR_TYPE_NAME, TYPE, NAME, ARGS, /* META */ ...) MR_FIELD_DESC (MR_TYPE_NAME, TYPE, NAME, , MR_TYPE_FUNC, __VA_ARGS__, .param = { .func_param = { .size = 0, .args = (mr_fd_t []){ MR_FUNC_ARG (TYPE, "return value") MR_FOREACH (MR_FUNC_ARG, MR_REMOVE_PAREN (ARGS)) { .mr_type = MR_TYPE_LAST, }, }, }, })
#define MR_FUNC_ARG(TYPE, /* META */ ...) {			\
    .name = { .str = MR_STRINGIFY (TYPE), .hash_value = 0, },	\
      .type = MR_STRINGIFY (TYPE),				\
	 .size = sizeof (TYPE),					\
	 .mr_type = MR_TYPE_DETECT (TYPE),			\
	 .mr_type_aux = MR_TYPE_DETECT_PTR (TYPE),		\
	 .meta = "" __VA_ARGS__,				\
	 },

#define MR_TYPEDEF_STRUCT_DESC(MR_TYPE_NAME) MR_TYPEDEF_DESC (MR_TYPE_NAME, MR_TYPE_STRUCT)
#define MR_END_STRUCT_DESC(MR_TYPE_NAME, /* META */ ...) MR_TYPEDEF_END_DESC (MR_TYPE_NAME, __VA_ARGS__)

#define MR_TYPEDEF_UNION_DESC(MR_TYPE_NAME) MR_TYPEDEF_DESC (MR_TYPE_NAME, MR_TYPE_UNION)
#define MR_END_UNION_DESC(MR_TYPE_NAME, /* META */ ...) MR_TYPEDEF_END_DESC (MR_TYPE_NAME, __VA_ARGS__)

#define MR_ANON_UNION_DESC(MR_TYPE_NAME, NAME, /* ATTR */ ...) {	\
    (mr_fd_t[]){ {							\
	.name = { .str = MR_STRINGIFY (NAME), .hash_value = 0, },	\
	  .type = "",							\
	     .offset = 0,						\
	     .unnamed = MR_IF_ELSE (MR_IS_EMPTY (NAME)) (true) (false), \
	     .mr_type = MR_IF_ELSE (MR_IS_EMPTY (NAME)) (MR_TYPE_ANON_UNION) (MR_TYPE_NAMED_ANON_UNION), \
	     .res = { (mr_td_t[]){ { .type = { .str = (char []) {MR_TYPE_ANONYMOUS_UNION_TEMPLATE "9999"}, .hash_value = 0, }, } } }, \
	     .res_type = "mr_td_t",					\
		} } },
#define MR_END_ANON_UNION_DESC(MR_TYPE_NAME, /* META */ ...) {	\
    (mr_fd_t[]){ {							\
	.type = "",							\
	  .mr_type = MR_TYPE_END_ANON_UNION,				\
	  .meta = "" __VA_ARGS__,					\
	  } } },

#define MR_TYPEDEF_ENUM_DESC(MR_TYPE_NAME) MR_TYPEDEF_DESC (MR_TYPE_NAME, MR_TYPE_ENUM)

#define MR_ENUM_DEF_DESC(MR_TYPE_NAME, NAME, ...) MR_ENUM_DEF_DESC_(MR_TYPE_NAME, NAME, __VA_ARGS__)
#define MR_ENUM_DEF_DESC_(MR_TYPE_NAME, NAME, RHS, /* META */ ...) { \
    (mr_fd_t[]){ {							\
	.type = MR_STRINGIFY (MR_TYPE_NAME),				\
	  .name = { .str = MR_STRINGIFY (NAME), .hash_value = 0, },	\
	  .mr_type = MR_TYPE_ENUM,					\
	  .mr_type_aux = MR_TYPE_DETECT (MR_TYPE_NAME),			\
	     .param = { .enum_param = { NAME }, },			\
	     .meta = "" __VA_ARGS__,					\
		} } },
#define MR_END_ENUM_DESC(MR_TYPE_NAME, /* META */ ...) MR_TYPEDEF_END_DESC (MR_TYPE_NAME, __VA_ARGS__)

#define MR_FUNC_ARG_PTR(...) { (mr_fd_t[]){ MR_FUNC_ARG (__VA_ARGS__) } },

#define MR_TYPEDEF_CHAR_ARRAY_DESC(MR_TYPE_NAME, SIZE, /* ATTR */ ...)	\
  MR_TYPEDEF_DESC (MR_TYPE_NAME, MR_TYPE_CHAR_ARRAY)			\
    MR_TYPEDEF_END_DESC (MR_TYPE_NAME, __VA_ARGS__)

#define MR_TYPEDEF_FUNC_DESC(RET_TYPE, MR_TYPE_NAME, ARGS, /* ATTR */ ...) \
  MR_TYPEDEF_DESC (MR_TYPE_NAME, MR_TYPE_FUNC_TYPE)			\
  MR_FUNC_ARG_PTR (RET_TYPE, "return value")				\
  MR_FOREACH (MR_FUNC_ARG_PTR, MR_REMOVE_PAREN (ARGS))			\
  MR_TYPEDEF_END_DESC (MR_TYPE_NAME, __VA_ARGS__)

#define MR_TYPEDEF_DESC(MR_TYPE_NAME, MR_TYPE)				\
  MR_DESCRIPTOR_ATTR mr_td_t MR_DESCRIPTOR_PREFIX (MR_TYPE_NAME) = {	\
    .type = { .str = #MR_TYPE_NAME, .hash_value = 0, },			\
    .mr_type = MR_TYPE,							\
    .param = { .enum_param = { .mr_type_effective = MR_TYPE_DETECT (MR_TYPE_NAME), }, }, \
    .size = sizeof (MR_TYPE_NAME),					\
    .fields_size = 0,							\
    .fields = (mr_fd_ptr_t[]){
#define MR_TYPEDEF_END_DESC(MR_TYPE_NAME, ATTR, /* META */ ...) {	\
    (mr_fd_t[]){ {							\
	.type = #MR_TYPE_NAME,						\
	.mr_type = MR_TYPE_LAST,					\
	  } } } },							\
    .meta = "" __VA_ARGS__ };						\
  static inline void __attribute__((constructor)) MR_CONSTRUCTOR_PREFIX (MR_TYPE_NAME) (void) { mr_add_type (&MR_DESCRIPTOR_PREFIX (MR_TYPE_NAME)); }

/*
  User can turn off strict types checking for Metaresc macroses, so compilation will produce only warnings.
  MR_CHECK_TYPES macro should be predefined as
  #define MR_CHECK_TYPES(...)
*/
#ifndef MR_CHECK_TYPES
#define MR_CHECK_TYPES(MR_TYPE_NAME, ...) (void) ((MR_TYPE_NAME*)0 - (__typeof__ (__VA_ARGS__ + 0))0)
#endif /* MR_CHECK_TYPES */

#define MR_COPY_RECURSIVELY(MR_TYPE_NAME, ...) MR_COPY_RECURSIVELY_ (MR_TYPE_NAME, __VA_ARGS__, 3, 2)
#define MR_COPY_RECURSIVELY_(MR_TYPE_NAME, S_PTR, D_PTR, N, ...) MR_PASTE2 (MR_COPY_RECURSIVELY_ARG, N) (MR_TYPE_NAME, S_PTR, D_PTR)
#define MR_COPY_RECURSIVELY_ARG3(MR_TYPE_NAME, S_PTR, D_PTR) ({		\
      mr_status_t ___status = MR_FAILURE;				\
      MR_CHECK_TYPES (MR_TYPE_NAME, D_PTR);				\
      mr_ra_ptrdes_t ___ptrs = MR_SAVE (MR_TYPE_NAME, S_PTR);		\
      if (___ptrs.ra != NULL)						\
	{								\
	  ___status = mr_copy_recursively (&___ptrs, D_PTR);		\
	  MR_FREE (___ptrs.ra);						\
	}								\
      ___status;							\
    })
#define MR_COPY_RECURSIVELY_ARG2(MR_TYPE_NAME, S_PTR, D_PTR) ({ \
      MR_TYPE_NAME dst;						\
      memset (&dst, 0, sizeof (dst));				\
      MR_COPY_RECURSIVELY_ARG3 (MR_TYPE_NAME, S_PTR, &dst);	\
      dst;							\
    })

#define MR_FREE_RECURSIVELY(MR_TYPE_NAME, S_PTR) ({		\
      mr_ra_ptrdes_t ptrs = MR_SAVE (MR_TYPE_NAME, S_PTR);	\
      mr_status_t status = MR_SUCCESS;				\
      mr_free_recursively (&ptrs);				\
      if (NULL == ptrs.ra)					\
	status = MR_FAILURE;					\
      else							\
	MR_FREE (ptrs.ra);					\
      status;							\
    })

#define MR_HASH_STRUCT(MR_TYPE_NAME, S_PTR) ({			\
      mr_ra_ptrdes_t _s_ptr_ = MR_SAVE (MR_TYPE_NAME, S_PTR);	\
      mr_hash_value_t _hash_value_ = mr_hash_struct (&_s_ptr_);	\
      if (_s_ptr_.ra)						\
	MR_FREE (_s_ptr_.ra);					\
      _hash_value_;						\
    })

#define MR_CMP_STRUCTS(MR_TYPE_NAME, X, Y) ({			\
      mr_ra_ptrdes_t _x_ = MR_SAVE (MR_TYPE_NAME, X);		\
      mr_ra_ptrdes_t _y_ = MR_SAVE (MR_TYPE_NAME, Y);		\
      int _cmp_ = mr_cmp_structs (&_x_, &_y_);			\
      if (_x_.ra)						\
	MR_FREE (_x_.ra);					\
      if (_y_.ra)						\
	MR_FREE (_y_.ra);					\
      _cmp_;							\
    })
    
#define MR_SAVE MR_SAVE_TYPED

#define MR_SAVE_TYPED(MR_TYPE_NAME, S_PTR)		\
  MR_IF_ELSE (MR_IS_EMPTY (MR_TYPE_NAME))		\
  (MR_SAVE_TYPED_ (__typeof__ (*(S_PTR)), S_PTR))	\
  (MR_SAVE_TYPED_ (MR_TYPE_NAME, S_PTR))

#define MR_SAVE_TYPED_(MR_TYPE_NAME, S_PTR) ({				\
      mr_fd_t __fd__ =							\
	{								\
	  .name = { .str = MR_STRINGIFY (S_PTR), .hash_value = 0, },	\
	  .type = MR_STRINGIFY (MR_TYPE_NAME),				\
	  .non_persistent = true,					\
	  .mr_type = MR_TYPE_DETECT (MR_TYPE_NAME),			\
	  .size = sizeof (MR_TYPE_NAME),				\
	};								\
      MR_TYPE_NAME * check_type = S_PTR;				\
      mr_save_data_t __mr_save_data__;					\
      mr_detect_type (&__fd__);						\
      if (__builtin_types_compatible_p (MR_TYPE_NAME[], __typeof__ (S_PTR)) && \
	  (sizeof (MR_TYPE_NAME) > 0) &&				\
	  (sizeof (S_PTR) >= sizeof (MR_TYPE_NAME)))			\
	{								\
	  __fd__.mr_type_aux = __fd__.mr_type;				\
	  __fd__.mr_type = MR_TYPE_ARRAY;				\
	  __fd__.size = sizeof (S_PTR);					\
	  __fd__.param.array_param.count = (0 + sizeof (S_PTR)) / sizeof (MR_TYPE_NAME); \
	  __fd__.param.array_param.row_count = 1;			\
	}								\
      __fd__.name.str = mr_normalize_name (__fd__.name.str);		\
      MR_CHECK_TYPES (MR_TYPE_NAME, S_PTR);				\
      (void)(0 / (uintptr_t)S_PTR);					\
      if (check_type == NULL)						\
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_NULL_POINTER);		\
      else								\
	mr_save (check_type, &__fd__, &__mr_save_data__);		\
      __mr_save_data__.ptrs;						\
    })

#define MR_SAVE_STR_TYPED(MR_TYPE_NAME_STR, S_PTR) ({			\
      mr_fd_t __fd__ =							\
	{								\
	  .name = { .str = MR_STRINGIFY (S_PTR), .hash_value = 0, },	\
	  .non_persistent = true,					\
	  .type = MR_TYPE_NAME_STR,					\
	  .mr_type = MR_TYPE_NONE,					\
	  .size = 0,							\
	};								\
      void * __ptr__ = S_PTR;						\
      mr_save_data_t __mr_save_data__;					\
      mr_detect_type (&__fd__);						\
      __fd__.name.str = mr_normalize_name (__fd__.name.str);		\
      if (__ptr__ == NULL)						\
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_NULL_POINTER);		\
      else								\
	mr_save (__ptr__, &__fd__, &__mr_save_data__);			\
      __mr_save_data__.ptrs;						\
    })

#define MR_SAVE_XDR(MR_TYPE_NAME, XDRS, S_PTR) ({			\
      mr_status_t __status__ = MR_FAILURE;				\
      XDR * __xdrs__ = (XDRS);						\
      if (XDR_ENCODE != __xdrs__->x_op)					\
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_XDR_WRONG_ENCODING_MODE);	\
      else								\
	{								\
	  mr_ra_ptrdes_t __ptrs__ = MR_SAVE (MR_TYPE_NAME, S_PTR);	\
	  if (__ptrs__.ra != NULL)					\
	    {								\
	      __status__ = xdr_save (__xdrs__, &__ptrs__);		\
	      MR_FREE (__ptrs__.ra);					\
	    }								\
	}								\
      __status__;							\
    })

#define MR_SAVE_XDR_RA(MR_TYPE_NAME, S_PTR) ({				\
      XDR _xdrs_;							\
      mr_rarray_t _ra_ = { .alloc_size = 0, .MR_SIZE = 0, .data = { NULL }, .type = "uint8_t" }; \
      xdrra_create (&_xdrs_, &_ra_, XDR_ENCODE);			\
      if (MR_SUCCESS != MR_SAVE_XDR (MR_TYPE_NAME, &_xdrs_, S_PTR))	\
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_XDR_SAVE_FAILED);		\
      _ra_;								\
    })

#define MR_SAVE_METHOD(METHOD, MR_TYPE_NAME, S_PTR) ({			\
      mr_ra_ptrdes_t __ptrs__ = MR_SAVE (MR_TYPE_NAME, S_PTR);		\
      char * __str__ = NULL;						\
      if (__ptrs__.ra != NULL)						\
	{								\
	  mr_remove_empty_nodes (&__ptrs__);				\
	  __str__ = METHOD (&__ptrs__);					\
	  MR_FREE (__ptrs__.ra);					\
	}								\
      __str__;								\
    })

#define MR_SAVE_XML1(MR_TYPE_NAME, S_PTR) MR_SAVE_METHOD (xml1_save, MR_TYPE_NAME, S_PTR)
#define MR_SAVE_CINIT(MR_TYPE_NAME, S_PTR) MR_SAVE_METHOD (cinit_save, MR_TYPE_NAME, S_PTR)
#define MR_SAVE_JSON(MR_TYPE_NAME, S_PTR) MR_SAVE_METHOD (json_save, MR_TYPE_NAME, S_PTR)
#define MR_SAVE_SCM(MR_TYPE_NAME, S_PTR) MR_SAVE_METHOD (scm_save, MR_TYPE_NAME, S_PTR)

#define MR_SAVE_METHOD_RA(METHOD, MR_TYPE_NAME, S_PTR) ({		\
      mr_rarray_t _ra_ = { .alloc_size = 0, .MR_SIZE = 0, .data = { NULL }, .type = "string" }; \
      _ra_.data.ptr = METHOD (MR_TYPE_NAME, S_PTR);			\
      if (_ra_.data.ptr)						\
	_ra_.MR_SIZE = _ra_.alloc_size = strlen (_ra_.data.ptr) + 1;	\
      _ra_;								\
    })

#define MR_SAVE_XML_RA(MR_TYPE_NAME, S_PTR) MR_SAVE_METHOD_RA (MR_SAVE_XML, MR_TYPE_NAME, S_PTR)
#define MR_SAVE_XML1_RA(MR_TYPE_NAME, S_PTR) MR_SAVE_METHOD_RA (MR_SAVE_XML1, MR_TYPE_NAME, S_PTR)
#define MR_SAVE_CINIT_RA(MR_TYPE_NAME, S_PTR) MR_SAVE_METHOD_RA (MR_SAVE_CINIT, MR_TYPE_NAME, S_PTR)
#define MR_SAVE_JSON_RA(MR_TYPE_NAME, S_PTR) MR_SAVE_METHOD_RA (MR_SAVE_JSON, MR_TYPE_NAME, S_PTR)
#define MR_SAVE_SCM_RA(MR_TYPE_NAME, S_PTR) MR_SAVE_METHOD_RA (MR_SAVE_SCM, MR_TYPE_NAME, S_PTR)

#define MR_LOAD_XDR_ARG3(MR_TYPE_NAME, XDRS, S_PTR) ({			\
      mr_status_t __status__ = MR_FAILURE;				\
      XDR * __xdrs__ = (XDRS);						\
      if (XDR_DECODE != __xdrs__->x_op)					\
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_XDR_WRONG_ENCODING_MODE);	\
      else								\
	{								\
	  mr_fd_t __fd__ = {						\
	    .type = MR_STRINGIFY (MR_TYPE_NAME),			\
	    .name = { .str = NULL, .hash_value = 0, },			\
	    .non_persistent = true,					\
	    .mr_type = MR_TYPE_DETECT (MR_TYPE_NAME),			\
	    .size = sizeof (MR_TYPE_NAME),				\
	  };								\
	  MR_TYPE_NAME * __check_type__ = S_PTR + 0;			\
	  mr_detect_type (&__fd__);					\
	  MR_CHECK_TYPES (MR_TYPE_NAME, S_PTR);				\
	  if (__check_type__ != NULL)					\
	    __status__ = xdr_load (__check_type__, &__fd__, __xdrs__);	\
	  else								\
	    MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_NULL_POINTER);		\
	}								\
      __status__;							\
    })

#define MR_LOAD_XDR_ARG2_(MR_TYPE_NAME, XDRS) ({			\
      mr_status_t _status_ = MR_FAILURE;				\
      MR_TYPE_NAME __result__;						\
      memset (&__result__, 0, sizeof (__result__));			\
      _status_ = MR_LOAD_XDR_ARG3 (MR_TYPE_NAME, XDRS, &__result__);	\
      if (MR_SUCCESS != _status_)					\
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
      mr_ra_ptrdes_t __ptrs__ = MR_SAVE (MR_TYPE_NAME, S_PTR);		\
      if (__ptrs__.ra != NULL)						\
	{								\
	  mr_remove_empty_nodes (&__ptrs__);				\
	  xmlDocPtr __doc__ = xml2_save (&__ptrs__);			\
	  MR_FREE (__ptrs__.ra);					\
	  if (__doc__)							\
	    {								\
	      xmlDocDumpFormatMemory (__doc__, &__xml_str__, &__size__, 1); \
	      xmlFreeDoc (__doc__);					\
	    }								\
	  if (__xml_str__)						\
	    {								\
	      __str__ = mr_strdup ((char*)__xml_str__);			\
	      xmlFree (__xml_str__);					\
	    }								\
	}								\
      __str__;								\
    })

#define MR_LOAD_XML2_NODE_ARG3(MR_TYPE_NAME, XML, S_PTR) ({		\
      mr_status_t __status__ = MR_FAILURE;				\
      int __idx__ = -1;							\
      mr_load_data_t __load_data__ = {					\
	.ptrs = { .ra = NULL, .size = 0, .alloc_size = 0, .ptrdes_type = MR_PD_LOAD, }, \
	.mr_ra_idx = NULL,						\
	.mr_ra_idx_size = 0,						\
	.mr_ra_idx_alloc_size = 0,					\
      };								\
      mr_fd_t __fd__ = {						\
	.type = MR_STRINGIFY (MR_TYPE_NAME),				\
	.name = { .str = NULL, .hash_value = 0, },			\
	.non_persistent = true,						\
	.mr_type = MR_TYPE_DETECT (MR_TYPE_NAME),			\
	.size = sizeof (MR_TYPE_NAME),					\
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
	  if (__load_data__.ptrs.ra)					\
	    MR_FREE (__load_data__.ptrs.ra);				\
	}								\
      __status__;							\
    })

#define MR_LOAD_XML2_NODE_ARG2_(MR_TYPE_NAME, XML) ({			\
      mr_status_t _status_ = MR_FAILURE;				\
      MR_TYPE_NAME __result__;						\
      memset (&__result__, 0, sizeof (__result__));			\
      _status_ = MR_LOAD_XML2_NODE_ARG3 (MR_TYPE_NAME, XML, &__result__); \
      if (MR_SUCCESS != _status_)					\
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_LOAD_STRUCT_FAILED);	\
      __result__;							\
    })

#define MR_LOAD_XML2_NODE(MR_TYPE_NAME, ...) MR_LOAD_XML2_NODE_ARGN (MR_TYPE_NAME, __VA_ARGS__, 3, 2)
#define MR_LOAD_XML2_NODE_ARGN(MR_TYPE_NAME, XML, S_PTR, N, ...) MR_LOAD_XML2_NODE_ARG ## N (MR_TYPE_NAME, XML, S_PTR)
#define MR_LOAD_XML2_NODE_ARG2(MR_TYPE_NAME, XML, S_PTR) MR_LOAD_XML2_NODE_ARG2_ (MR_TYPE_NAME, XML)

#define MR_LOAD_XML2_ARG3(MR_TYPE_NAME, STR, S_PTR) ({			\
      mr_status_t _status_ = MR_FAILURE;				\
      MR_TYPE_NAME * _check_type_ = S_PTR + 0;				\
      char * __str__ = (STR);						\
      MR_CHECK_TYPES (MR_TYPE_NAME, S_PTR);				\
      if (NULL == _check_type_)						\
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_NULL_POINTER);		\
      else if (NULL == __str__)						\
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_NULL_POINTER);	\
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
      mr_status_t ___status___ = MR_FAILURE;				\
      MR_TYPE_NAME __result__;						\
      memset (&__result__, 0, sizeof (__result__));			\
      ___status___ = MR_LOAD_XML2_ARG3 (MR_TYPE_NAME, STR, &__result__); \
      if (MR_SUCCESS != ___status___)					\
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_LOAD_STRUCT_FAILED);	\
      __result__;							\
    })

#define MR_LOAD_XML2(MR_TYPE_NAME, ...) MR_LOAD_XML2_N (MR_TYPE_NAME, __VA_ARGS__, 3, 2)
#define MR_LOAD_XML2_N(MR_TYPE_NAME, STR, S_PTR, N, ...) MR_LOAD_XML2_ARG ## N (MR_TYPE_NAME, STR, S_PTR)
#define MR_LOAD_XML2_ARG2(MR_TYPE_NAME, STR, S_PTR) MR_LOAD_XML2_ARG2_ (MR_TYPE_NAME, STR)

#endif /* HAVE_LIBXML2 */

#define MR_LOAD_XML2_RA(MR_TYPE_NAME, ...) MR_LOAD_XML2_RA_ARGN (MR_TYPE_NAME, __VA_ARGS__, 3, 2)
#define MR_LOAD_XML2_RA_ARGN(MR_TYPE_NAME, RA, S_PTR, N, ...) MR_LOAD_XML2_ARG ## N (MR_TYPE_NAME, (char*)((RA)->data.ptr), S_PTR)

#ifdef HAVE_BISON_FLEX

#define MR_LOAD_METHOD_ARG3(METHOD, MR_TYPE_NAME, STR, S_PTR) ({	\
      mr_status_t _status_ = MR_FAILURE;				\
      MR_TYPE_NAME * _check_type_ = S_PTR + 0;				\
      char * _str_ = (STR);						\
      MR_CHECK_TYPES (MR_TYPE_NAME, S_PTR);				\
      if (NULL == _check_type_)						\
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_NULL_POINTER);		\
      else if (NULL == _str_)						\
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_NULL_POINTER);	\
      else								\
	{								\
	  mr_load_data_t _load_data_ = {				\
	    .ptrs = { .ra = NULL, .size = 0, .alloc_size = 0, .ptrdes_type = MR_PD_LOAD, }, \
	    .mr_ra_idx = NULL,						\
	    .mr_ra_idx_size = 0,					\
	    .mr_ra_idx_alloc_size = 0,					\
	  };								\
	  _status_ = METHOD (_str_, &_load_data_.ptrs);			\
	  if (MR_SUCCESS == _status_)					\
	    {								\
	      mr_fd_t _fd_ = {						\
		.type = MR_STRINGIFY (MR_TYPE_NAME),			\
		.name = { .str = NULL, .hash_value = 0, },		\
		.mr_type = MR_TYPE_DETECT (MR_TYPE_NAME),		\
		.size = sizeof (MR_TYPE_NAME),				\
	      };							\
	      mr_detect_type (&_fd_);					\
	      _status_ = mr_load (_check_type_, &_fd_, 0, &_load_data_); \
	    }								\
	  if (_load_data_.ptrs.ra)					\
	    MR_FREE (_load_data_.ptrs.ra);				\
	}								\
      _status_;								\
    })

#define MR_LOAD_METHOD_ARG2_(METHOD, MR_TYPE_NAME, STR) ({		\
      mr_status_t _status_ = MR_FAILURE;				\
      MR_TYPE_NAME __result__;						\
      memset (&__result__, 0, sizeof (__result__));			\
      _status_ = MR_LOAD_METHOD_ARG3 (METHOD, MR_TYPE_NAME, STR, &__result__); \
      if (MR_SUCCESS != _status_)					\
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
#define MR_LOAD_METHOD_RA_ARGN(METHOD, MR_TYPE_NAME, RA, S_PTR, N, ...) MR_LOAD_METHOD_ARG ## N (METHOD, MR_TYPE_NAME, (char*)((RA)->data.ptr), S_PTR)

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
#define MR_LOAD_XML MR_LOAD_STUB
#define MR_LOAD_XML_RA MR_LOAD_STUB
#endif /* HAVE_BISON_FLEX */

#endif /* HAVE_LIBXML2 */

#ifndef MR_PRINT_FD
#define MR_PRINT_FD stdout
#endif

#define EQ_2_2 0

#define MR_PRINT_STRUCT(FD, ...)			\
  MR_IF_ELSE (MR_PASTE2 (EQ_2_, MR_NARG (__VA_ARGS__)))	\
    (MR_PRINT_VALUE (FD, __VA_ARGS__))			\
    (MR_PRINT_STRUCT_ (FD, __VA_ARGS__))

#define MR_PRINT_STRUCT_(FD, TYPE, PTR) ({		\
      char * _dump_ = MR_SAVE_CINIT (TYPE, PTR);	\
      int rv = 0;					\
      if (_dump_)					\
	{						\
	  rv = fprintf (FD, "%s", _dump_);		\
	  MR_FREE (_dump_);				\
	}						\
      rv;						\
    })

#define MR_PRINT_VALUE(FD, X) mr_print_value (FD, MR_TYPE_DETECT (__typeof__ (X)), X)

#define MR_PRINT_ONE_ELEMENT(FD, X, I)		\
  MR_IF_ELSE (MR_IS_IN_PAREN (X))		\
    (MR_PRINT_STRUCT (FD, MR_REMOVE_PAREN (X)))	\
    (MR_PRINT_VALUE (FD, X))

#define MR_ADD(NAME, I, REC, X) REC + X
#define MR_PRINT(...) (void)MR_FPRINT (stdout, __VA_ARGS__)
#define MR_FPRINT(FD, ...) (MR_FOR (FD, MR_NARG (__VA_ARGS__), MR_ADD, MR_PRINT_ONE_ELEMENT, __VA_ARGS__))

typedef char * string_t;
typedef char mr_string_t[1];
typedef long int long_int_t;
typedef long long int long_long_int_t;
typedef long double long_double_t;
typedef complex float complex_float_t;
typedef complex double complex_double_t;
typedef complex long double complex_long_double_t;
typedef unsigned mr_hash_value_t;
typedef struct mr_ic_t mr_ic_t_; /* required for forward definition of struct mr_ic_t which is used in mr_ic_hash_t */
typedef struct mr_dummy_struct_t {
  char dummy_field[0];
} mr_dummy_struct_t;
typedef __typeof__ (offsetof (mr_dummy_struct_t, dummy_field)) mr_offset_t;
typedef __typeof__ (sizeof (0)) mr_size_t;

#ifndef MR_MODE
#define MR_MODE_UNDEFINED
#define MR_MODE PROTO
#endif
#include <mr_protos.h>
#ifdef MR_MODE_UNDEFINED
#undef MR_MODE_UNDEFINED
#undef MR_MODE
#endif

extern mr_conf_t mr_conf;

extern mr_status_t mr_add_type (mr_td_t * tdp);
extern char * mr_read_xml_doc (FILE * fd);

extern void mr_save (void * data, mr_fd_t * fdp, mr_save_data_t * mr_save_data);
extern mr_status_t mr_load (void * data, mr_fd_t * fdp, int idx, mr_load_data_t * mr_load_data);
#ifdef HAVE_LIBXML2
extern xmlDocPtr xml2_save (mr_ra_ptrdes_t * ptrs);
extern int xml2_load (xmlNodePtr, mr_ra_ptrdes_t * ptrs);
#endif /* HAVE_LIBXML2 */
#ifdef HAVE_RPC_TYPES_H
extern mr_status_t xdr_save (XDR * xdrs, mr_ra_ptrdes_t * ptrs);
extern mr_status_t xdr_load (void * data, mr_fd_t * fdp, XDR * xdrs);
extern void xdrra_create (XDR * xdrs, mr_rarray_t * rarray, enum xdr_op op);
#endif /* HAVE_RPC_TYPES_H */

extern char * xml1_save (mr_ra_ptrdes_t * ptrs);
extern char * cinit_save (mr_ra_ptrdes_t * ptrs);
extern char * json_save (mr_ra_ptrdes_t * ptrs);
extern char * scm_save (mr_ra_ptrdes_t * ptrs);

#ifdef HAVE_BISON_FLEX
extern mr_status_t xml1_load (char * str, mr_ra_ptrdes_t * ptrs);
extern mr_status_t cinit_load (char * str, mr_ra_ptrdes_t * ptrs);
extern mr_status_t scm_load (char * str, mr_ra_ptrdes_t * ptrs);
#endif /* HAVE_BISON_FLEX */

extern void * mr_calloc (const char * filename, const char * function, int line, size_t count, size_t size);
extern void * mr_realloc (const char * filename, const char * function, int line, void * ptr, size_t size);
extern void mr_free (const char * filename, const char * function, int line, void * ptr);
extern char * mr_strdup (const char * str);
extern char * mr_strndup (const char * str, size_t size);

extern void mr_assign_int (mr_ptrdes_t * dst, mr_ptrdes_t * src);
extern bool mr_is_valid_field_name (char * name);
extern void mr_pointer_get_size_ptrdes (mr_ptrdes_t * ptrdes, int idx, mr_ra_ptrdes_t * ptrs);
extern void mr_pointer_set_size (int idx, mr_ra_ptrdes_t * ptrs);
extern int mr_add_ptr_to_list (mr_ra_ptrdes_t * ptrs);
extern void mr_add_child (int parent, int child, mr_ptrdes_t * ra);
extern void mr_detect_type (mr_fd_t * fdp);
extern void mr_detect_fields_types (mr_td_t * tdp);
extern void __attribute__ ((sentinel(0))) mr_type_void_fields (char * type, char * name, ...);
extern mr_size_t mr_type_size (mr_type_t mr_type, char * type);
extern char * mr_normalize_name (char * name);
extern mr_status_t mr_free_recursively (mr_ra_ptrdes_t * ptrs);
extern mr_status_t mr_copy_recursively (mr_ra_ptrdes_t * ptrs, void * data);
extern mr_hash_value_t mr_hash_struct (mr_ra_ptrdes_t * ptrs);
extern int mr_cmp_structs (mr_ra_ptrdes_t * x, mr_ra_ptrdes_t * y);
extern mr_fd_t * mr_get_fd_by_name (mr_td_t * tdp, char * name);
extern mr_fd_t * mr_get_enum_by_value (mr_td_t * tdp, int64_t value);
extern mr_fd_t * mr_get_enum_by_name (char * name);
extern mr_status_t mr_load_bitfield_value (mr_ptrdes_t * ptrdes, uint64_t * value);
extern mr_status_t mr_save_bitfield_value (mr_ptrdes_t * ptrdes, uint64_t * value);
extern mr_td_t * mr_get_td_by_name (char * type);
extern char * mr_message_format (mr_message_id_t message_id, va_list args);
extern void mr_message (const char * file_name, const char * func_name, int line, mr_log_level_t log_level, mr_message_id_t message_id, ...);
extern void * mr_rarray_append (mr_rarray_t * rarray, ssize_t size);
extern void * mr_rarray_allocate_element (void ** data, ssize_t * size, ssize_t * alloc_size, ssize_t element_size);
extern int __attribute__ ((format (printf, 2, 3))) mr_ra_printf (mr_rarray_t * rarray, const char * format, ...);
extern int mr_print_value (FILE * fd, int mr_type, ...);

extern char * mr_get_static_field_name (mr_substr_t * substr);
extern void xml_unquote_string (mr_substr_t * substr, char * dst);
extern void mr_remove_empty_nodes (mr_ra_ptrdes_t * ptrs);

extern int64_t mr_get_enum_value (mr_td_t * tdp, void * data);
extern mr_hash_value_t mr_hash_str (char * str);
extern int mr_fd_name_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context);
extern mr_hash_value_t mr_fd_name_get_hash (const mr_ptr_t x, const void * context);
extern int mr_td_name_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context);
extern mr_hash_value_t mr_td_name_get_hash (const mr_ptr_t x, const void * context);
extern int mr_hashed_string_cmp (const mr_hashed_string_t * x, const mr_hashed_string_t * y);
extern mr_hash_value_t mr_hashed_string_get_hash (const mr_hashed_string_t * x);
extern mr_hash_value_t mr_enumfd_get_hash (mr_ptr_t x, const void * context);
extern int cmp_enums_by_value (mr_ptr_t x, mr_ptr_t y, const void * context);
extern int mr_hashed_string_cmp_ic (const mr_ptr_t x, const mr_ptr_t y, const void * context);
extern mr_hash_value_t mr_hashed_string_get_hash_ic (mr_ptr_t x, const void * context);
extern int mr_ud_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context);
extern int mr_fd_offset_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context);

#endif /* _METARESC_H_ */
