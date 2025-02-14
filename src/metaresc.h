/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

/* Metaresc prototypes */
#ifndef _METARESC_H_
#define _METARESC_H_

#include <mr_pp.h>
#include <mr_export.h>

#include <stdio.h> /* for FILE */
#include <stddef.h> /* for offsetof */
#include <stdbool.h> /* for bool */
#include <string.h> /* for strlen () & memset () */
#include <ctype.h> /* for isspace () */
#include <stdarg.h> /* for va_list */
#include <inttypes.h> /* for int8_t, int16_t, int32_t, int64_t, SCNxXX, etc */
#include <sys/types.h> /* for ssize_t */
#include <complex.h> /* for complex */
#include <setjmp.h> /* for jmp_buf and setjmp () */

#ifdef HAVE_LIBXML2
# include <libxml/xmlmemory.h>
# include <libxml/parser.h>
#endif /* HAVE_LIBXML2 */
#ifdef HAVE_RPC_TYPES_H
# include <rpc/types.h>
# include <rpc/xdr.h>
#endif /* HAVE_RPC_TYPES_H */

/* Library exports */
#define MR_NULL_IDX (0)
#define MR_MAX_STRING_LENGTH ((unsigned int)-1)

#define MR_INT_TO_STRING_BUF_SIZE (32)
#define MR_FLOAT_TO_STRING_BUF_SIZE (256)
#define MR_CHAR_TO_STRING_BUF_SIZE (8)

#define MR_MAX_INDENT_LEVEL (30) /* the same constant as in libxml2 */
#define MR_MIN(X,Y) ({ __typeof__ (X) _x_ = (X); __typeof__ (Y) _y_ = (Y); (_x_ < _y_) ? _x_ : _y_; })
#define MR_MAX(X,Y) ({ __typeof__ (X) _x_ = (X); __typeof__ (Y) _y_ = (Y); (_x_ > _y_) ? _x_ : _y_; })
#define MR_LIMIT_LEVEL(LEVEL) MR_MIN (LEVEL, MR_MAX_INDENT_LEVEL)

#define MR_BUILTIN_TYPES						\
  signed, unsigned, bool,						\
    char,								\
    signed char, char signed,						\
    unsigned char, char unsigned,					\
    short,								\
    signed short, short signed,						\
    unsigned short, short unsigned,					\
    short int, int short,						\
    signed short int, signed int short,					\
    short signed int, int signed short,					\
    short int signed, int short signed,					\
    unsigned short int, unsigned int short,				\
    short unsigned int, int unsigned short,				\
    short int unsigned, int short unsigned,				\
    int,								\
    signed int, int signed,						\
    unsigned int, int unsigned,						\
    long,								\
    signed long, long signed,						\
    unsigned long, long unsigned,					\
    long int, int long,							\
    signed long int, signed int long,					\
    long signed int, int signed long,					\
    long int signed, int long signed,					\
    unsigned long int, unsigned int long,				\
    long unsigned int, int unsigned long,				\
    long int unsigned, int long unsigned,				\
    long long,								\
    signed long long, long signed long, long long signed,		\
    unsigned long long, long unsigned long, long long unsigned,		\
    long long int, long int long, int long long,			\
    signed long long int, signed long int long, signed int long long,	\
    long signed long int, long signed int long, int signed long long,	\
    long long signed int, long int signed long, int long signed long,	\
    long long int signed, long int long signed, int long long signed,	\
    unsigned long long int, unsigned long int long, unsigned int long long, \
    long unsigned long int, long unsigned int long, int unsigned long long, \
    long long unsigned int, long int unsigned long, int long unsigned long, \
    long long int unsigned, long int long unsigned, int long long unsigned, \
    float, double, long double, double long,				\
    complex float, complex double, complex long double, complex double long, \
    float complex, double complex,					\
    long complex double, double complex long,				\
    long double complex, double long complex				\
    MR_BUILTIN_TYPES_INT128

/* each refereed structure will have REF_IDX property */
#define MR_REF_IDX "ref_idx"
/* references on already saved structures will be replaced with nodes that have only REF index property */
#define MR_REF "ref"
#define MR_REF_CONTENT "ref_content"

#define MR_YAML_ANCHOR_TMPLT MR_REF_IDX "_%" SCNu32
#define MR_YAML_REF_ANCHOR_TMPLT MR_YAML_ANCHOR_TMPLT
#define MR_YAML_REF_ANCHOR_CONTENT_TMPLT MR_REF_CONTENT "_%" SCNu32

#define MR_ANON_UNION_MAX_EMBEDED_LEVELS (16)
#define MR_ANONYMOUS_FIELD_NAME_TEMPLATE "%s_%d"
#define MR_ANONYMOUS_FIELD_TYPE_TEMPLATE MR_ANONYMOUS_FIELD_NAME_TEMPLATE "_t"

/* XML attribute for zero length strings */
#define MR_DEFAULT_NODE_NAME "entity"
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

#define MR_CALLOC(COUNT, SIZE) (mr_conf.mr_mem.calloc)(__FILE__, __FUNCTION__, __LINE__, COUNT, SIZE)
#define MR_REALLOC(PTR, SIZE) (mr_conf.mr_mem.realloc)(__FILE__, __FUNCTION__, __LINE__, PTR, SIZE)
#define MR_FREE(PTR) (mr_conf.mr_mem.free)(__FILE__, __FUNCTION__, __LINE__, PTR)

/*
  you can redefine this prefixes from outside before first include of metaresc.h
*/
#ifndef MR_DESCRIPTOR_PREFIX
#define MR_DESCRIPTOR_PREFIX(ID, MR_TYPE_NAME) mr_td_ ## ID
#endif /* MR_DESCRIPTOR_PREFIX */
#ifndef MR_TYPEDEF_STUB_PREFIX
#define MR_KETWORD_struct struct,
#define MR_KETWORD_union union,
#define MR_DROP_KEYWORDS_(MR_TYPE_NAME, KEYWORD, ...) MR_IF_ELSE (MR_IS_EMPTY (__VA_ARGS__)) (MR_TYPE_NAME) (__VA_ARGS__)
#define MR_DROP_KEYWORDS(...) MR_DROP_KEYWORDS_ (__VA_ARGS__)
#define MR_TYPEDEF_STUB_PREFIX(MR_TYPE_NAME) MR_PASTE2 (mr_stub_, MR_DROP_KEYWORDS (MR_TYPE_NAME, MR_PASTE2 (MR_KETWORD_, MR_TYPE_NAME)))
#endif /* MR_TYPEDEF_STUB_PREFIX */
#ifndef MR_TYPEDEF_PREFIX
#define MR_TYPEDEF_PREFIX(MR_TYPE_NAME) MR_TYPE_NAME
#endif /* MR_TYPEDEF_PREFIX */
#ifndef MR_CONSTRUCTOR_PREFIX
#define MR_CONSTRUCTOR_PREFIX(ID, MR_TYPE_NAME) mr_init_ ## ID
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
  | (__builtin_types_compatible_p (char SUFFIX, TYPE) ? MR_TYPE_CHAR : 0) \
  | (__builtin_types_compatible_p (signed char SUFFIX, TYPE) ? MR_PASTE2 (MR_TYPE_INT, MR_SIZEOF_CHAR) : 0) \
  | (__builtin_types_compatible_p (unsigned char SUFFIX, TYPE) ? MR_PASTE2 (MR_TYPE_UINT, MR_SIZEOF_CHAR) : 0) \
  | (__builtin_types_compatible_p (signed short SUFFIX, TYPE) ? MR_PASTE2 (MR_TYPE_INT, MR_SIZEOF_SHORT) : 0) \
  | (__builtin_types_compatible_p (unsigned short SUFFIX, TYPE) ? MR_PASTE2 (MR_TYPE_UINT, MR_SIZEOF_SHORT) : 0) \
  | (__builtin_types_compatible_p (signed int SUFFIX, TYPE) ? MR_PASTE2 (MR_TYPE_INT, MR_SIZEOF_INT) : 0) \
  | (__builtin_types_compatible_p (unsigned int SUFFIX, TYPE) ? MR_PASTE2 (MR_TYPE_UINT, MR_SIZEOF_INT) : 0) \
  | (__builtin_types_compatible_p (signed long SUFFIX, TYPE) ? MR_PASTE2 (MR_TYPE_INT, MR_SIZEOF_LONG) : 0) \
  | (__builtin_types_compatible_p (unsigned long SUFFIX, TYPE) ? MR_PASTE2 (MR_TYPE_UINT, MR_SIZEOF_LONG) : 0) \
  | (__builtin_types_compatible_p (signed long long SUFFIX, TYPE) ? MR_PASTE2 (MR_TYPE_INT, MR_SIZEOF_LONG_LONG) : 0) \
  | (__builtin_types_compatible_p (unsigned long long SUFFIX, TYPE) ? MR_PASTE2 (MR_TYPE_UINT, MR_SIZEOF_LONG_LONG) : 0) \
  | (__builtin_types_compatible_p (mr_int128_t SUFFIX, TYPE) ? MR_TYPE_INT128 : 0) \
  | (__builtin_types_compatible_p (mr_uint128_t SUFFIX, TYPE) ? MR_TYPE_UINT128 : 0) \
  | (__builtin_types_compatible_p (float SUFFIX, TYPE) ? MR_TYPE_FLOAT : 0) \
  | (__builtin_types_compatible_p (complex float SUFFIX, TYPE) ? MR_TYPE_COMPLEX_FLOAT : 0) \
  | (__builtin_types_compatible_p (double SUFFIX, TYPE) ? MR_TYPE_DOUBLE : 0) \
  | (__builtin_types_compatible_p (complex double SUFFIX, TYPE) ? MR_TYPE_COMPLEX_DOUBLE : 0) \
  | (__builtin_types_compatible_p (long double SUFFIX, TYPE) ? MR_TYPE_LONG_DOUBLE : 0) \
  | (__builtin_types_compatible_p (complex long double SUFFIX, TYPE) ? MR_TYPE_COMPLEX_LONG_DOUBLE : 0) \
  | ((__builtin_types_compatible_p (__typeof__ (char []) SUFFIX, TYPE)	\
      | __builtin_types_compatible_p (__typeof__ (const char []) SUFFIX, TYPE) \
      | __builtin_types_compatible_p (__typeof__ (volatile char []) SUFFIX, TYPE) \
      | __builtin_types_compatible_p (__typeof__ (const volatile char []) SUFFIX, TYPE) \
      ) ? MR_TYPE_CHAR_ARRAY : 0)					\
  | ((__builtin_types_compatible_p (__typeof__ (char *) SUFFIX, TYPE)	\
      | __builtin_types_compatible_p (__typeof__ (const char *) SUFFIX, TYPE) \
      | __builtin_types_compatible_p (__typeof__ (volatile char *) SUFFIX, TYPE) \
      | __builtin_types_compatible_p (__typeof__ (const volatile char *) SUFFIX, TYPE) \
      ) ? MR_TYPE_STRING : 0)						\
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
#define TYPEDEF_STRUCT(...) P00_TYPEDEF (STUB, __VA_ARGS__) P00_TYPEDEF (STRUCT, __VA_ARGS__)
#define TYPEDEF_UNION(...) P00_TYPEDEF (STUB, __VA_ARGS__) P00_TYPEDEF (UNION, __VA_ARGS__)
#define TYPEDEF_ENUM(...) P00_TYPEDEF (ENUM, __VA_ARGS__)
#define TYPEDEF_FUNC(...) P00_TYPEDEF (FUNC, __VA_ARGS__)

/* Macroses for builtin types meta data registration */
#define MR_TYPEDEF_DESC_BI_(ID, TYPE, MR_TYPE, ...) MR_TYPEDEF_DESC (ID, TYPE, MR_TYPE) MR_TYPEDEF_END_DESC (ID, TYPE, __VA_ARGS__);
#define MR_TYPEDEF_DESC_BI(TYPE, ...) MR_TYPEDEF_DESC_BI_ (__COUNTER__, TYPE, MR_TYPE_DETECT (TYPE), __VA_ARGS__)

/*
  Checks if MR_MODE was defined. If it was undefined automtically produce prototypes and descriptors.
  Library could be unintrusively extended for other modes. Defined MR_MODE transparently passed to lower level.
*/
#define MR_IS_MR_MODE_EQ_MR_MODE 0
#define P00_TYPEDEF(...) P00_TYPEDEF_ (__COUNTER__, __VA_ARGS__)
#define P00_TYPEDEF_(...)						\
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
  TYPEDEF_{STRUCT|UNION|ENUM|FUNC} might have one of arguments with prefix ATTRIBUTES (...).
  This key word denotes type attributes and meta information.
  Next macro moves ATTRIBUTES argument to first position in arguments list.
*/
#define P00_TYPEDEF_MODE(P00_MODE, P00_ID, P00_TYPE, ...)		\
  P00_TYPEDEF_MODE_ (P00_MODE, P00_ID, P00_TYPE,			\
		     ATTRIBUTES (P00_GET_ATTRIBUTES (__VA_ARGS__)),	\
		     P00_GET_NON_ATTRIBUTES (__VA_ARGS__))
#define P00_TYPEDEF_MODE_(...) P00_TYPEDEF_MODE__ (__VA_ARGS__)
#define P00_TYPEDEF_MODE__(P00_MODE, P00_ID, P00_TYPE, ATTR_META_RES, ...) \
  MR_PASTE2 (P00_TYPEDEF_ATTR_, P00_TYPE) (P00_MODE, P00_ID, P00_TYPE, ATTR_META_RES, __VA_ARGS__)

/*
  Here is switch on definition type.
  FUNC should be directly expanded to MR_TYPEDEF_FUNC respectively.
  STUB applies only for DESC mode. It declares a stub union type with fields of type int8_t and names as in original structure/union.
  This type is required for bitfield/array descriptor definition.
*/
#define P00_TYPEDEF_ATTR_STRUCT TYPEDEF_ATTR
#define P00_TYPEDEF_ATTR_UNION TYPEDEF_ATTR
#define P00_TYPEDEF_ATTR_ENUM TYPEDEF_ATTR

#define MR_DESC_EQ_DESC 0
#define P00_TYPEDEF_ATTR_STUB(P00_MODE, ...) MR_IF_ELSE (MR_PASTE2 (MR_DESC_EQ_, P00_MODE)) () (TYPEDEF_ATTR (STUB, __VA_ARGS__))

#define P00_TYPEDEF_ATTR_FUNC(P00_MODE, P00_ID, P00_TYPE, ATTR_META_RES, RET_TYPE, P00_TYPE_NAME, ARGS, ...) MR_PASTE2 (MR_TYPEDEF_FUNC_, P00_MODE) (P00_ID, RET_TYPE, P00_TYPE_NAME, ARGS, MR_PASTE2 (P00_REMOVE_, ATTR_META_RES), __VA_ARGS__)

#define P00_UNFOLD(PREFIX, P00_TYPE, P00_MODE, ...) MR_PASTE4 (PREFIX, P00_TYPE, _, P00_MODE) (__VA_ARGS__)

/*
  Next macro produces typedef prolog, body and epilog.
  Prolog is MR_TYPEDEF_{STRUCT|UNION|ENUM}_{PROTO|DESC} (P00_ID, P00_TYPE_NAME, ATTR...)
  Body is a list of fields or enums definition. Structs and union fields are handled with P00_FIELD. Enums definitions are handled with P00_ENUM_DEF.
  Epilog is MR_END_{STRUCT|UNION|ENUM}_{PROTO|DESC} (P00_ID, P00_TYPE_NAME, META...)
*/
#define MR_SER(NAME, I, REC, X) REC X

#define TYPEDEF_ATTR(P00_MODE, P00_ID, P00_TYPE, ATTR_META_RES, P00_TYPE_NAME, ...) \
  P00_UNFOLD (MR_TYPEDEF_, P00_TYPE, P00_MODE, P00_ID, P00_TYPE_NAME)	\
  MR_FOR ((P00_MODE, P00_TYPE_NAME), MR_NARG (__VA_ARGS__), MR_SER, MR_PASTE3 (P00_, P00_TYPE, _HANDLER), __VA_ARGS__) \
  P00_UNFOLD (MR_END_, P00_TYPE, P00_MODE, P00_ID, P00_TYPE_NAME, MR_PASTE2 (P00_REMOVE_, ATTR_META_RES))

#define P00_STUB_HANDLER P00_FIELD
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
   (P00_FIELD_UNFOLD (P00_MODE_TYPE_NAME, ARRAY_OR_BITFIELD FIELD)))

#define P00_GET_MODE(P00_MODE, P00_TYPE_NAME) P00_MODE
#define P00_GET_TYPE_NAME(P00_MODE, P00_TYPE_NAME) P00_TYPE_NAME
/*
  Field type prefix should be extracted as separate macro argument. So we add prefix P00_COMMA_ and expect that in next macro field prefix will be substituted on comma delimitted MR_ type prefix.
  The 3rd macro unfolds to MR_{VOID|BITFIELD|...}_{PROTO|DESC} (P00_TYPE_NAME, ARGS...)
  Last one detects unknown field qualifiers.
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
#define P00_COMMA_ARRAY ARRAY,
#define P00_COMMA_ARRAY_OR_BITFIELD ARRAY_OR_BITFIELD,
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
#define MR_COMPILETIME_ASSERT(CONDITION, ...) MR_COMPILETIME_ASSERT_ (__COUNTER__, CONDITION, __VA_ARGS__)
#define MR_COMPILETIME_ASSERT_(ID, CONDITION, ...) __attribute__ ((unused)) static char * MR_UNIQ_NAME (ID) = &"Assert: " __VA_ARGS__ [0 / (CONDITION)];
/*
  For types defined using standard language approach you will need to create analog types with metaresc.
  For double checking of types costincency you will need the following macro. It compares size and offset of fields in two types.
  Usage: MR_COMPARE_COMPAUND_TYPES (system_type, metaresc_type, commonly_named_field, (field_for_system_type, field_for_metaresc_type), ...)
  Macro evaluates to 0 at compile time if all fields are compatible. Otherwise it is non-zero.
*/
#define MR_LOGICAL_AND(NAME, I, REC, X) ((REC) && (X))
#define MR_COMPARE_COMPAUND_TYPES(TYPE1, TYPE2, ...) ((sizeof (TYPE1) == sizeof (TYPE2)) && MR_FOR ((TYPE1, TYPE2), MR_NARG (__VA_ARGS__), MR_LOGICAL_AND, P00_COMPARE_FIELDS, __VA_ARGS__))
#define P00_GET_FIRST(_1, _2) _1
#define P00_GET_SECOND(_1, _2) _2
#define P00_COMPARE_FIELDS(TYPE1_TYPE2, NAME, I)			\
  MR_IF_ELSE (MR_IS_IN_PAREN (NAME))					\
  (MR_COMPARE_FIELDS (P00_GET_FIRST TYPE1_TYPE2, P00_GET_FIRST NAME, P00_GET_SECOND TYPE1_TYPE2, P00_GET_SECOND NAME)) \
  (MR_COMPARE_FIELDS (P00_GET_FIRST TYPE1_TYPE2, NAME, P00_GET_SECOND TYPE1_TYPE2, NAME))

#ifndef MR_COMPARE_FIELDS_EXT
#define MR_COMPARE_FIELDS_EXT(...) true
/*
  if your types contains only builtin types then you can do more precies comparation.
  #undef MR_COMPARE_FIELDS_EXT
  #define MR_COMPARE_FIELDS_EXT(TYPE1, NAME1, TYPE2, NAME2) __builtin_types_compatible_p (__typeof__ (((TYPE1*)0)->NAME1), __typeof__ (((TYPE2*)0)->NAME2))
*/
#endif /* MR_COMPARE_FIELDS_EXT */
#define MR_COMPARE_FIELDS(TYPE1, NAME1, TYPE2, NAME2) (offsetof (TYPE1, NAME1) == offsetof (TYPE2, NAME2)) && (sizeof (((TYPE1*)0)->NAME1) == sizeof (((TYPE2*)0)->NAME2)) && MR_COMPARE_FIELDS_EXT (TYPE1, NAME1, TYPE2, NAME2)

/* Macroses for prototypes generation mode */
#define MR_TYPEDEF_STRUCT_PROTO(ID, MR_TYPE_NAME) typedef struct MR_TYPEDEF_PREFIX (MR_TYPE_NAME) MR_TYPE_NAME; struct MR_TYPEDEF_PREFIX (MR_TYPE_NAME) {
#define MR_END_STRUCT_PROTO(ID, MR_TYPE_NAME, ATTR, ...) } ATTR;
#define MR_TYPEDEF_UNION_PROTO(ID, MR_TYPE_NAME) typedef union MR_TYPEDEF_PREFIX (MR_TYPE_NAME) {
#define MR_END_UNION_PROTO(ID, MR_TYPE_NAME, ATTR, ...) } ATTR MR_TYPE_NAME;
#define MR_TYPEDEF_ENUM_PROTO(ID, MR_TYPE_NAME) typedef enum MR_TYPEDEF_PREFIX (MR_TYPE_NAME) {
#define MR_END_ENUM_PROTO(ID, MR_TYPE_NAME, ATTR, ...) } ATTR MR_TYPE_NAME;

/* next macro adds empty argument. Required for MR_AUTO, MR_VOID with two parameters. It adds 3rd parameter (suffix) for them. */
#define MR_AUTO_PROTO(...) MR_FIELD_PROTO (__VA_ARGS__, )
#define MR_VOID_PROTO(...) MR_VOID_PROTO_ (__VA_ARGS__, )
#define MR_VOID_PROTO_(MR_TYPE_NAME, TYPE, NAME, SUFFIX, ...)		\
  MR_IF_ELSE (MR_IS_IN_PAREN (SUFFIX))					\
    (MR_FUNC_PROTO (MR_TYPE_NAME, TYPE, NAME, SUFFIX, __VA_ARGS__))	\
    (MR_FIELD_PROTO (MR_TYPE_NAME, TYPE, NAME, SUFFIX, __VA_ARGS__))

#define MR_FIELD_PROTO(MR_TYPE_NAME, TYPE, NAME, SUFFIX, ...) TYPE NAME SUFFIX;
#define MR_BITFIELD_PROTO MR_FIELD_PROTO
#define MR_ARRAY_PROTO MR_FIELD_PROTO
#define MR_ARRAY_OR_BITFIELD_PROTO MR_FIELD_PROTO
#define MR_FUNC_PROTO(MR_TYPE_NAME, TYPE, NAME, ARGS, ...) MR_FIELD_PROTO (MR_TYPE_NAME, TYPE, (*NAME), ARGS)

#define MR_ANON_UNION_PROTO(MR_TYPE_NAME, NAME, ... /* ATTR */) MR_IF_ELSE (MR_IS_EMPTY (NAME)) () (char NAME[0];) union __VA_ARGS__ {
#define MR_END_ANON_UNION_PROTO(MR_TYPE_NAME, ...) };

#define MR_ENUM_DEF_PROTO(MR_TYPE_NAME, NAME, ...) MR_ENUM_DEF_PROTO_ (MR_TYPE_NAME, NAME, __VA_ARGS__)
#define MR_ENUM_DEF_PROTO_(MR_TYPE_NAME, NAME, RHS, ...) NAME RHS,

#define MR_TYPEDEF_FUNC_PROTO(ID, RET_TYPE, MR_TYPE_NAME, ARGS, /* ATTR */ ...) MR_TYPEDEF_FUNC_PROTO_ (RET_TYPE, MR_TYPE_NAME, ARGS, __VA_ARGS__)
#define MR_TYPEDEF_FUNC_PROTO_(RET_TYPE, MR_TYPE_NAME, ARGS, ATTR, /* META */ ...) typedef ATTR RET_TYPE (*MR_TYPE_NAME) ARGS;

/* Macroses for descriptors generation mode */

#define MR_FIELD_DESC(MR_TYPE_NAME, TYPE, NAME, SUFFIX, MR_TYPE, /* META */ ...) \
  (mr_fd_t[]){ {							\
      .name.str = #NAME,						\
	.stype.type = #TYPE,						\
	.stype.size = sizeof (((MR_TYPE_NAME*)0)->NAME),		\
	.stype.mr_type = MR_TYPE,					\
	.stype.mr_type_class = __builtin_classify_type (((MR_TYPE_NAME*)0)->NAME), \
	.offset = offsetof (MR_TYPE_NAME, NAME),			\
	.meta = "" __VA_ARGS__,						\
	} },

#define MR_ARRAY_SLICE0(TYPE, ARRAY) (__builtin_choose_expr (__builtin_types_compatible_p (TYPE, __typeof__ (ARRAY[0])), (TYPE[]){}, ARRAY[0]))
#define MR_ARRAY_SLICE1(TYPE, ARRAY) (__builtin_choose_expr (__builtin_types_compatible_p (TYPE, __typeof__ (ARRAY[0])), (TYPE[]){}, ARRAY[0]))

#define MR_ARRAY_DIMENSIONS_(TYPE, A0, A1, A2, A3) {			\
    .dim = {								\
      sizeof (A0) / sizeof (A1),					\
      sizeof (A1) / sizeof (A2),					\
      sizeof (A2) / sizeof (A3),					\
      sizeof (A3) / sizeof (TYPE),					\
    },									\
    .size = sizeof (((mr_array_dimensions_t*)0)->dim) -			\
    sizeof (((mr_array_dimensions_t*)0)->dim[0]) *			\
    (__builtin_types_compatible_p (TYPE, __typeof__ (A1)) +		\
     __builtin_types_compatible_p (TYPE, __typeof__ (A2)) +		\
     __builtin_types_compatible_p (TYPE, __typeof__ (A3))		\
     ),									\
  }

#define MR_ARRAY_DIMENSIONS(TYPE, ARRAY) MR_ARRAY_DIMENSIONS_ (TYPE, ARRAY, ARRAY[0], MR_ARRAY_SLICE0 (TYPE, ARRAY)[0], MR_ARRAY_SLICE1 (TYPE, MR_ARRAY_SLICE0 (TYPE, ARRAY))[0])

#define MR_ARRAY_DESC(MR_TYPE_NAME, TYPE, NAME, SUFFIX, /* META */ ...) \
  (mr_fd_t[]){ {							\
      .name.str = #NAME,						\
	.stype.type = #TYPE,						\
	.stype.size = sizeof (((MR_TYPE_NAME*)0)->NAME),		\
	.stype.mr_type = MR_TYPE_DETECT (TYPE),				\
	.stype.mr_type_aux = MR_TYPE_DETECT_PTR (TYPE),			\
	.stype.mr_type_class = __builtin_classify_type (((MR_TYPE_NAME*)0)->NAME), \
	.stype.is_array = true,						\
	.stype.dim = MR_ARRAY_DIMENSIONS (TYPE, ((MR_TYPE_NAME*)0)->NAME), \
	.offset = offsetof (MR_TYPE_NAME, NAME),			\
	.meta = "" __VA_ARGS__,						\
	} },

#define MR_BITFIELD_DESC(MR_TYPE_NAME, TYPE, NAME, SUFFIX, /* META */ ...) \
  (mr_fd_t[]){ {							\
      .name.str = #NAME,						\
	.stype.type = #TYPE,						\
	.stype.size = sizeof (TYPE),					\
	.stype.mr_type = MR_TYPE_BITFIELD,				\
	.stype.mr_type_aux = MR_TYPE_DETECT (TYPE),			\
	.stype.mr_type_class = __builtin_classify_type (((MR_TYPE_NAME*)0)->NAME), \
	.bitfield_param.size = sizeof (MR_TYPE_NAME),			\
	.bitfield_param.bitfield = (uint8_t*)((MR_TYPE_NAME[]){ { .NAME = -1 } }), \
	.meta = "" __VA_ARGS__,						\
	} },

#define MR_IS_AN_ARRAY(X) (__builtin_classify_type (X) == MR_POINTER_TYPE_CLASS)

#define MR_AOB_OBJECT(MR_TYPE_NAME, NAME, BITFIELD_OBJ)			\
  __builtin_choose_expr (MR_IS_AN_ARRAY (((MR_TYPE_NAME*)0)->NAME),	\
			 ((MR_TYPE_NAME*)0)->NAME, BITFIELD_OBJ)

#define MR_AOB_TYPE(MR_TYPE_NAME, NAME, SWAP)				\
  __typeof__ (__builtin_choose_expr (SWAP ^ MR_IS_AN_ARRAY (((MR_TYPE_NAME*)0)->NAME), \
				     *(MR_TYPE_NAME*)0, *(MR_TYPEDEF_STUB_PREFIX (MR_TYPE_NAME)*)0))

#define MR_AOB_BITFIELD(MR_TYPE_NAME, NAME)				\
  __builtin_choose_expr (MR_IS_AN_ARRAY (((MR_TYPE_NAME*)0)->NAME),	\
			 NULL, (uint8_t*)((MR_AOB_TYPE (MR_TYPE_NAME, NAME, true)[]){ { .NAME = -1 } }))


#define MR_ARRAY_OR_BITFIELD_DESC(MR_TYPE_NAME, TYPE, NAME, SUFFIX, /* META */ ...) \
  (mr_fd_t[]){ {							\
      .name.str = #NAME,						\
	.stype.type = #TYPE,						\
	.stype.size = sizeof (MR_AOB_OBJECT (MR_TYPE_NAME, NAME, *(TYPE*)0)), \
	.stype.mr_type = MR_TYPE_DETECT (TYPE),				\
	.stype.mr_type_aux = MR_TYPE_DETECT_PTR (TYPE),			\
	.stype.mr_type_class = __builtin_classify_type (((MR_TYPE_NAME*)0)->NAME), \
	.stype.is_array = MR_IS_AN_ARRAY (((MR_TYPE_NAME*)0)->NAME),	\
	.stype.is_bitfield = !MR_IS_AN_ARRAY (((MR_TYPE_NAME*)0)->NAME), \
	.stype.dim = MR_ARRAY_DIMENSIONS (TYPE, MR_AOB_OBJECT (MR_TYPE_NAME, NAME, (TYPE[]){})), \
	.offset = offsetof (MR_AOB_TYPE (MR_TYPE_NAME, NAME, false), NAME), \
	.bitfield_param.size = sizeof (MR_TYPE_NAME),			\
	.bitfield_param.bitfield = MR_AOB_BITFIELD (MR_TYPE_NAME, NAME), \
	.meta = "" __VA_ARGS__,						\
	} },

#define MR_VOID_DESC_(MR_TYPE_NAME, TYPE, NAME, SUFFIX, /* META */ ...) \
  (mr_fd_t[]){ {							\
      .name.str = (char []) { #NAME },					\
	.stype.mr_type = MR_TYPE_VOID,					\
	MR_IF_ELSE (MR_IS_IN_PAREN (SUFFIX))				\
	(.stype.type = #TYPE " (*) " #SUFFIX,				\
	 .stype.size = sizeof (void*),					\
	 .stype.mr_type_aux = MR_TYPE_FUNC,				\
	 .stype.mr_type_class = MR_FUNCTION_TYPE_CLASS,			\
	 .offset = offsetof (MR_TYPE_NAME, NAME),			\
	 )								\
	(.stype.type = #TYPE,						\
	 .stype.mr_type_aux = MR_TYPE_DETECT (TYPE),			\
	 .stype.mr_type_class = __builtin_classify_type (((MR_TYPE_NAME*)0)->NAME), \
	 MR_IF_ELSE (MR_IS_EMPTY (SUFFIX))				\
	 (.offset = offsetof (MR_TYPE_NAME, NAME),			\
	  .stype.size = sizeof (((MR_TYPE_NAME*)0)->NAME),		\
	  )								\
	 (.offset = offsetof (MR_AOB_TYPE (MR_TYPE_NAME, NAME, false), NAME), \
	  .stype.size = sizeof (TYPE),					\
	  )								\
	 )								\
	.meta = "" __VA_ARGS__,						\
	} },

#define MR_AUTO_DESC_(MR_TYPE_NAME, TYPE, NAME, SUFFIX, /* META */ ...) \
  (mr_fd_t[]){ {							\
      .name.str = #NAME,						\
	.stype.type = __builtin_choose_expr				\
	(__builtin_types_compatible_p (MR_TYPE_NAME *, TYPE) |		\
	 __builtin_types_compatible_p (MR_TYPE_NAME const *, TYPE) |	\
	 __builtin_types_compatible_p (MR_TYPE_NAME volatile *, TYPE) | \
	 __builtin_types_compatible_p (MR_TYPE_NAME const volatile *, TYPE), \
	 #MR_TYPE_NAME "*",						\
	 #TYPE								\
	 ),								\
	.stype.size = sizeof (((MR_TYPE_NAME*)0)->NAME),		\
	.stype.mr_type = MR_TYPE_DETECT (TYPE),				\
	.stype.mr_type_aux = MR_TYPE_DETECT_PTR (TYPE),			\
	.stype.mr_type_class = __builtin_classify_type (((MR_TYPE_NAME*)0)->NAME), \
	.non_persistent = 0 / __builtin_types_compatible_p (TYPE, __typeof__ (((MR_TYPE_NAME*)0)->NAME)), \
	.offset = offsetof (MR_TYPE_NAME, NAME),			\
	.meta = "" __VA_ARGS__,						\
	} },

/* Generate division by zero error if type of the field mismatches
   with type provided in macro. This is possible for descriptors
   generated for external types */

/* ensure that name is a token without parentheses or braces at the end */
#define MR_AUTO_DESC(MR_TYPE_NAME, TYPE, NAME, ...) MR_AUTO_DESC__ (NAME ## _, MR_TYPE_NAME, TYPE, NAME, __VA_ARGS__)
#define MR_AUTO_DESC__(VALIDATED_NAME, MR_TYPE_NAME, TYPE, NAME, ...) MR_IF_ELSE (MR_IS_EMPTY (TYPE)) \
    (MR_AUTO_DESC_ (MR_TYPE_NAME, __typeof__ (((MR_TYPE_NAME*)0)->NAME), NAME, __VA_ARGS__)) \
    (MR_AUTO_DESC_ (MR_TYPE_NAME, TYPE, NAME, __VA_ARGS__))

#define MR_VOID_DESC(MR_TYPE_NAME, TYPE, NAME, ...) MR_VOID_DESC_ (MR_TYPE_NAME, TYPE, NAME, __VA_ARGS__)
#define MR_FUNC_DESC(MR_TYPE_NAME, TYPE, NAME, ARGS, /* META */ ...) MR_FIELD_DESC (MR_TYPE_NAME, TYPE, NAME, , MR_TYPE_FUNC, __VA_ARGS__, .func_param = { .args = (mr_stype_t*[]){ MR_FUNC_ARG (TYPE) MR_FOREACH (MR_FUNC_ARG, MR_REMOVE_PAREN (ARGS)) NULL, }, })

/*
  MR_OBJ_OF_TYPE returns an object of specified type. It could be as simple as (TYPE){},
  but will not work for type void, that's why it's wrapped into __builtin_choose_expr.
  Type cast require additional typeof wrapper for array types, otherwise construct is invalid.
*/
#define MR_OBJ_OF_TYPE(TYPE) *__builtin_choose_expr (__builtin_types_compatible_p (TYPE, void), "", (__typeof__ (TYPE) *)0)

#define MR_FUNC_ARG(TYPE) (mr_stype_t[]){ {				\
      .type = #TYPE,							\
	.size = sizeof (TYPE),						\
	.mr_type = MR_TYPE_DETECT (TYPE),				\
	.mr_type_aux = MR_TYPE_DETECT_PTR (TYPE),			\
	.mr_type_class = __builtin_choose_expr (__builtin_types_compatible_p (TYPE, void), MR_VOID_TYPE_CLASS, \
						__builtin_classify_type (MR_OBJ_OF_TYPE (TYPE))), \
	} },

#define MR_TYPEDEF_STUB_STUB(ID, MR_TYPE_NAME) typedef union {
#define MR_END_STUB_STUB(ID, MR_TYPE_NAME, ATTR, ...) } MR_TYPEDEF_STUB_PREFIX (MR_TYPE_NAME);
#define MR_ANON_UNION_STUB(...)
#define MR_END_ANON_UNION_STUB(...)
#define MR_AUTO_BI_STUB(...) MR_AUTO_BI_0 (STUB, __VA_ARGS__)
#define MR_FIELD_STUB(MR_TYPE_NAME, TYPE, NAME, ...) int8_t NAME;
#define MR_UNKNOWN_STUB(MR_TYPE_NAME, NAME, ...) int8_t NAME;
#define MR_AUTO_STUB MR_FIELD_STUB
#define MR_ARRAY_STUB MR_FIELD_STUB
#define MR_BITFIELD_STUB MR_FIELD_STUB
#define MR_ARRAY_OR_BITFIELD_STUB MR_FIELD_STUB
#define MR_VOID_STUB MR_FIELD_STUB
#define MR_FUNC_STUB MR_FIELD_STUB

#define MR_TYPEDEF_STRUCT_DESC(ID, MR_TYPE_NAME) MR_TYPEDEF_DESC (ID, MR_TYPE_NAME, MR_TYPE_STRUCT)
#define MR_END_STRUCT_DESC(ID, MR_TYPE_NAME, /* META */ ...) MR_TYPEDEF_END_DESC (ID, MR_TYPE_NAME, __VA_ARGS__)

#define MR_TYPEDEF_UNION_DESC(ID, MR_TYPE_NAME) MR_TYPEDEF_DESC (ID, MR_TYPE_NAME, MR_TYPE_UNION)
#define MR_END_UNION_DESC(ID, MR_TYPE_NAME, /* META */ ...) MR_TYPEDEF_END_DESC (ID, MR_TYPE_NAME, __VA_ARGS__)

#define MR_ANONYMOUS_FIELD_TYPE_SIZE(MR_TYPE_NAME) (sizeof (#MR_TYPE_NAME) + MR_ANON_UNION_MAX_EMBEDED_LEVELS * sizeof (MR_ANONYMOUS_FIELD_TYPE_TEMPLATE))

#define MR_ANON_UNION_DESC(MR_TYPE_NAME, NAME, /* ATTR */ ...)		\
  (mr_fd_t[]){ {							\
      .name.str = (char [MR_ANONYMOUS_FIELD_TYPE_SIZE (MR_TYPE_NAME)]) { #NAME }, \
	.stype.type = "",						\
	.stype.mr_type = MR_IF_ELSE (MR_IS_EMPTY (NAME)) (MR_TYPE_ANON_UNION) (MR_TYPE_NAMED_ANON_UNION), \
	.unnamed = MR_IF_ELSE (MR_IS_EMPTY (NAME)) (true) (false),	\
	.offset = 0,							\
	.res = { (mr_td_t[]){ {						\
	    .type.str =							\
	    (char [MR_ANONYMOUS_FIELD_TYPE_SIZE (MR_TYPE_NAME)]) {	\
	      [0 ... MR_ANONYMOUS_FIELD_TYPE_SIZE (MR_TYPE_NAME) - 2] = ' ', \
	    }, } } },							\
	.res_type = "mr_td_t",						\
	} },
#define MR_END_ANON_UNION_DESC(MR_TYPE_NAME, /* META */ ...)		\
  (mr_fd_t[]){ {							\
      .stype.type = "",							\
	.stype.mr_type = MR_TYPE_END_ANON_UNION,			\
	.meta = "" __VA_ARGS__,						\
	} },

#define MR_TYPEDEF_ENUM_DESC(ID, MR_TYPE_NAME, ...)			\
  MR_DESCRIPTOR_ATTR mr_td_t MR_DESCRIPTOR_PREFIX (ID, MR_TYPE_NAME) = { \
  .type = { .str = #MR_TYPE_NAME, },					\
    .mr_type = MR_TYPE_ENUM,						\
    .td_producer = MR_TDP_MACRO,					\
    .size = sizeof (MR_TYPE_NAME),					\
    .param = {								\
      .enum_param = {							\
	.mr_type_effective = MR_TYPE_DETECT (MR_TYPE_NAME),		\
	.enums = (mr_ed_t*[]){

#define MR_END_ENUM_DESC(ID, MR_TYPE_NAME, ATTR, /* META */ ...)	\
  NULL, }, }, },							\
    .meta = "" __VA_ARGS__ };						\
    static inline void __attribute__((constructor))			\
    MR_CONSTRUCTOR_PREFIX (ID, MR_TYPE_NAME) (void) {			\
      mr_add_type (&MR_DESCRIPTOR_PREFIX (ID, MR_TYPE_NAME));		\
    }

#define MR_ENUM_DEF_DESC(MR_TYPE_NAME, NAME, ...) MR_ENUM_DEF_DESC_(MR_TYPE_NAME, NAME, __VA_ARGS__)
#define MR_ENUM_DEF_DESC_(MR_TYPE_NAME, NAME, RHS, /* META */ ...)   \
  (mr_ed_t[]){ {						     \
      .name = { .str =  #NAME, },				     \
	.value = { ._unsigned = NAME, },			     \
	.meta = "" __VA_ARGS__,					     \
	} },

#define MR_TYPEDEF_FUNC_DESC(ID, RET_TYPE, MR_TYPE_NAME, ARGS, /* ATTR */ ...) MR_TYPEDEF_FUNC_DESC_ (ID, RET_TYPE, MR_TYPE_NAME, ARGS, __VA_ARGS__)
#define MR_TYPEDEF_FUNC_DESC_(ID, RET_TYPE, MR_TYPE_NAME, ARGS, ATTR, /* META */ ...) \
  MR_DESCRIPTOR_ATTR mr_td_t MR_DESCRIPTOR_PREFIX (ID, MR_TYPE_NAME) = { \
    .type = { .str = #MR_TYPE_NAME, },					\
    .mr_type = MR_TYPE_FUNC_TYPE,					\
    .td_producer = MR_TDP_MACRO,					\
    .size = sizeof (MR_TYPE_NAME),					\
    .param = {								\
      .func_param = {							\
	.args = (mr_stype_t*[]){					\
	  MR_FUNC_ARG (RET_TYPE)					\
	  MR_FOREACH (MR_FUNC_ARG, MR_REMOVE_PAREN (ARGS))		\
	  NULL, }, }, },						\
    .meta = "" __VA_ARGS__ };						\
  static inline void __attribute__((constructor))			\
  MR_CONSTRUCTOR_PREFIX (ID, MR_TYPE_NAME) (void) {			\
    mr_add_type (&MR_DESCRIPTOR_PREFIX (ID, MR_TYPE_NAME));		\
  }

#define MR_TYPEDEF_DESC(ID, MR_TYPE_NAME, MR_TYPE, ...)			\
  MR_DESCRIPTOR_ATTR mr_td_t MR_DESCRIPTOR_PREFIX (ID, MR_TYPE_NAME) = { \
  .type = { .str = #MR_TYPE_NAME, },					\
    .mr_type = MR_TYPE,							\
    .td_producer = MR_TDP_MACRO,					\
    .size = sizeof (MR_TYPE_NAME),					\
    .param = { .struct_param = { .fields = (mr_fd_t*[]){
#define MR_TYPEDEF_END_DESC(ID, MR_TYPE_NAME, ATTR, /* META */ ...) 	\
  NULL, }, }, },							\
    .meta = "" __VA_ARGS__ };						\
    static inline void __attribute__((constructor))			\
    MR_CONSTRUCTOR_PREFIX (ID, MR_TYPE_NAME) (void) {			\
      mr_add_type (&MR_DESCRIPTOR_PREFIX (ID, MR_TYPE_NAME));		\
    }

#ifdef HAVE_BUILTIN_DUMP_STRUCT_EXTRA_ARGS

#define MR_SINGLE_FD(CONTEXT, ARG, I) (mr_fd_t[]){{}},

#define MR_ADD_TYPE(MR_TYPE_NAME) MR_ADD_TYPE_ (__COUNTER__, MR_TYPE_NAME)
#define MR_ADD_TYPE_(ID, MR_TYPE_NAME)					\
  MR_DESCRIPTOR_ATTR mr_td_t MR_DESCRIPTOR_PREFIX (ID, MR_TYPE_NAME) = { \
    .type = { .str = #MR_TYPE_NAME, },					\
    .mr_type = (MR_RECORD_TYPE_CLASS == __builtin_classify_type (*(MR_TYPE_NAME*)0)) ? MR_TYPE_STRUCT : MR_TYPE_UNION, \
    .td_producer = MR_TDP_DUMP_STRUCT,					\
    .size = sizeof (MR_TYPE_NAME),					\
    .param = { .struct_param = { .fields = (mr_fd_t*[]){		\
	  MR_FOR ( , MR_PP_DEPTH, MR_SER, MR_SINGLE_FD)			\
	  NULL, }, }, }, };						\
  static inline void __attribute__((constructor))			\
  MR_CONSTRUCTOR_PREFIX (ID, MR_TYPE_NAME) (void) {			\
    mr_dump_struct_type_ctx_t dst_ctx;					\
    MR_TYPE_NAME value;							\
    uint8_t * ptr = (uint8_t*)&value;					\
    size_t block_size, i;						\
    for (i = 0; i < sizeof (value); ++i)				\
      *ptr++ = i;							\
    memset (&dst_ctx, 0, sizeof (dst_ctx));				\
    dst_ctx.struct_ptr = &value;					\
    dst_ctx.tdp = &MR_DESCRIPTOR_PREFIX (ID, MR_TYPE_NAME);		\
    for (dst_ctx.offset_byte = 0; ; ++dst_ctx.offset_byte)		\
      {									\
	dst_ctx.field_idx = 0;						\
	if (0 == setjmp (dst_ctx._jmp_buf))				\
	  __builtin_dump_struct (&value, mr_dump_struct_type_detection, &dst_ctx); \
	block_size = 1 << (__CHAR_BIT__ * (1 + dst_ctx.offset_byte));	\
	if (sizeof (value) < block_size)				\
	  break;							\
	i = 0;								\
	for (ptr = (uint8_t*)&value;					\
	     ptr - (uint8_t*)&value < sizeof (value) - (block_size - 1); \
	     ptr += block_size)						\
	  memset (ptr, i++, block_size);				\
	memset (ptr, i, sizeof (value) & (block_size - 1));		\
      }									\
    mr_struct_param_t * struct_param = &dst_ctx.tdp->param.struct_param; \
    size_t count = struct_param->fields_size / sizeof (struct_param->fields[0]); \
    int size = sizeof (value) - 1;					\
    block_size = 1 << (sizeof (int) * __CHAR_BIT__ -			\
		       __builtin_clz ((size <= 0) ? 1 : size) - 1);	\
    while (block_size != 0)						\
      {									\
	i = 0;								\
	for (ptr = (uint8_t*)&value;					\
	     ptr - (uint8_t*)&value < sizeof (value) - (block_size - 1); \
	     ptr += block_size)						\
	  memset (ptr, -(i++ & 1), block_size);				\
	memset (ptr, -(i++ & 1), sizeof (value) & (block_size - 1));	\
	dst_ctx.field_idx = count;					\
	if (0 == setjmp (dst_ctx._jmp_buf))				\
	  __builtin_dump_struct (&value, mr_dump_struct_bitfield_detection, &dst_ctx); \
	if (dst_ctx.field_idx == count)					\
	  break;							\
	block_size >>= 1;						\
      }									\
    if (dst_ctx.field_idx != count)					\
      {									\
	memset (&value, 0b11110000, sizeof (value));			\
	dst_ctx.field_idx = count;					\
	if (0 == setjmp (dst_ctx._jmp_buf))				\
	  __builtin_dump_struct (&value, mr_dump_struct_bitfield_detection, &dst_ctx); \
	memset (&value, 0b11001100, sizeof (value));			\
	dst_ctx.field_idx = count;					\
	if (0 == setjmp (dst_ctx._jmp_buf))				\
	  __builtin_dump_struct (&value, mr_dump_struct_bitfield_detection, &dst_ctx); \
	memset (&value, 0b10101010, sizeof (value));			\
	dst_ctx.field_idx = count;					\
	if (0 == setjmp (dst_ctx._jmp_buf))				\
	  __builtin_dump_struct (&value, mr_dump_struct_bitfield_detection, &dst_ctx); \
	i = count;							\
	count = struct_param->fields_size / sizeof (struct_param->fields[0]); \
	for ( ; i < count; ++i)						\
	  {								\
	    struct_param->fields[i]->bitfield_param.initialized = true;	\
	    struct_param->fields[i]->bitfield_param.shift =		\
	      struct_param->fields[i]->offset % __CHAR_BIT__;		\
	    struct_param->fields[i]->offset /= __CHAR_BIT__;		\
	  }								\
      }									\
    struct_param->fields[MR_PP_DEPTH] = struct_param->fields[count];	\
    struct_param->fields[count] = NULL;					\
    mr_add_type (dst_ctx.tdp);						\
  }

#else /* ! HAVE_BUILTIN_DUMP_STRUCT_EXTRA_ARGS */
#define MR_ADD_TYPE(MR_TYPE_NAME) "This feature require __builtin_dump_struct with support of extra args"[0 / 0]
#endif /* HAVE_BUILTIN_DUMP_STRUCT_EXTRA_ARGS */
/*
  User can turn off strict types checking for Metaresc macroses, so compilation will produce only warnings.
  MR_CHECK_TYPES macro should be predefined as
  #define MR_CHECK_TYPES(...)
*/
#ifndef MR_CHECK_TYPES
#define MR_CHECK_TYPES(MR_TYPE_NAME, S_PTR) MR_IF_ELSE (MR_IS_EMPTY (MR_TYPE_NAME)) () (MR_COMPILETIME_ASSERT (__builtin_types_compatible_p (MR_TYPE_NAME, __typeof__ (*(S_PTR)))))
#endif /* MR_CHECK_TYPES */

#define MR_COPY_RECURSIVELY(...) MR_PASTE2 (MR_COPY_RECURSIVELY_ARGS, MR_NARG (__VA_ARGS__)) (__VA_ARGS__)
#define MR_COPY_RECURSIVELY_ARGS3(MR_TYPE_NAME, S_PTR, D_PTR) ({	\
      mr_status_t ___status = MR_FAILURE;				\
      MR_CHECK_TYPES (MR_TYPE_NAME, D_PTR);				\
      MR_CHECK_TYPES (MR_TYPE_NAME, S_PTR);				\
      MR_COMPILETIME_ASSERT (__builtin_types_compatible_p (__typeof__ (D_PTR), __typeof__ (S_PTR))); \
      mr_ra_ptrdes_t ___ptrs = MR_SAVE (MR_TYPE_NAME, S_PTR);		\
      if (___ptrs.ra != NULL)						\
	{								\
	  ___status = mr_copy_recursively (&___ptrs, D_PTR);		\
	  MR_FREE (___ptrs.ra);						\
	}								\
      ___status;							\
    })
#define MR_COPY_RECURSIVELY_ARGS2(MR_TYPE_NAME, S_PTR) ({		\
      __typeof__ (*(S_PTR)) dst;					\
      memset (&dst, 0, sizeof (dst));					\
      MR_COPY_RECURSIVELY_ARGS3 (MR_TYPE_NAME, S_PTR, &dst);		\
      dst;								\
    })
#define MR_COPY_RECURSIVELY_ARGS1(S_PTR) MR_COPY_RECURSIVELY_ARGS2 ( , S_PTR)

#define MR_FREE_RECURSIVELY(...) MR_PASTE2 (MR_FREE_RECURSIVELY_ARGS, MR_NARG (__VA_ARGS__)) (__VA_ARGS__)
#define MR_FREE_RECURSIVELY_ARGS2(MR_TYPE_NAME, S_PTR) ({	\
      mr_ra_ptrdes_t ptrs = MR_SAVE (MR_TYPE_NAME, S_PTR);	\
      mr_status_t status = MR_SUCCESS;				\
      mr_free_recursively (&ptrs);				\
      if (NULL == ptrs.ra)					\
	status = MR_FAILURE;					\
      else							\
	MR_FREE (ptrs.ra);					\
      status;							\
    })
#define MR_FREE_RECURSIVELY_ARGS1(S_PTR) MR_FREE_RECURSIVELY_ARGS2 ( , S_PTR)

#define MR_HASH_STRUCT(...) MR_PASTE2 (MR_HASH_STRUCT_ARGS, MR_NARG (__VA_ARGS__)) (__VA_ARGS__)
#define MR_HASH_STRUCT_ARGS2(MR_TYPE_NAME, S_PTR) ({		\
      mr_ra_ptrdes_t _s_ptr_ = MR_SAVE (MR_TYPE_NAME, S_PTR);	\
      mr_hash_value_t _hash_value_ = mr_hash_struct (&_s_ptr_);	\
      if (_s_ptr_.ra)						\
	MR_FREE (_s_ptr_.ra);					\
      _hash_value_;						\
    })
#define MR_HASH_STRUCT_ARGS1(S_PTR) MR_HASH_STRUCT_ARGS2 ( , S_PTR)

#define MR_CMP_STRUCTS(...) MR_PASTE2 (MR_CMP_STRUCTS_ARGS, MR_NARG (__VA_ARGS__)) (__VA_ARGS__)
#define MR_CMP_STRUCTS_ARGS3(MR_TYPE_NAME, X, Y) ({		\
      mr_ra_ptrdes_t _x_ = MR_SAVE (MR_TYPE_NAME, X);		\
      mr_ra_ptrdes_t _y_ = MR_SAVE (MR_TYPE_NAME, Y);		\
      int _cmp_ = mr_cmp_structs (&_x_, &_y_);			\
      if (_x_.ra)						\
	MR_FREE (_x_.ra);					\
      if (_y_.ra)						\
	MR_FREE (_y_.ra);					\
      _cmp_;							\
    })
#define MR_CMP_STRUCTS_ARGS2(X, Y) MR_CMP_STRUCTS_ARGS3 ( , X, Y)
    
#define MR_GENERIC_SORT(...) MR_PASTE2 (MR_GENERIC_SORT_ARGS, MR_NARG (__VA_ARGS__)) (__VA_ARGS__)
#define MR_GENERIC_SORT_ARGS3(ARRAY, COUNT, TYPE) mr_generic_sort (ARRAY, COUNT, TYPE)
#define MR_GENERIC_SORT_ARGS2(ARRAY, COUNT) mr_basic_types_sort (ARRAY, COUNT, MR_OBJ_TYPE (ARRAY), MR_TYPE_DETECT (__typeof__ (ARRAY[0])), sizeof (ARRAY[0]))
#define MR_GENERIC_SORT_ARGS1(ARRAY) MR_GENERIC_SORT_ARGS2 (ARRAY, sizeof (ARRAY) / sizeof (ARRAY[0]))

#ifndef MR_OBJ_TYPE
# ifdef HAVE_BUILTIN_DUMP_STRUCT
#  ifdef HAVE_BUILTIN_DUMP_STRUCT_EXTRA_ARGS
#   define MR_OBJ_TYPE MR_OBJ_TYPE_DUMP_EXTRA
#  else /* HAVE_BUILTIN_DUMP_STRUCT_EXTRA_ARGS */
#   define MR_OBJ_TYPE MR_OBJ_TYPE_DUMP
#  endif /* HAVE_BUILTIN_DUMP_STRUCT_EXTRA_ARGS */
# else /* HAVE_BUILTIN_DUMP_STRUCT */
#  define MR_OBJ_TYPE MR_OBJ_TYPE_DWARF
# endif /* HAVE_BUILTIN_DUMP_STRUCT */
#endif /* MR_OBJ_TYPE */

#define MR_IS_STRUCT_OR_UNION(OBJ)				\
  ((((1 << MR_UNION_TYPE_CLASS) | (1 << MR_RECORD_TYPE_CLASS))	\
    >> __builtin_classify_type (OBJ)) & 1)

#define MR_STRUCT_POINTER(S_PTR) ({				\
      __typeof__ (&*MR_CAST_TO_PTR (S_PTR)) _mr = NULL;		\
      __builtin_choose_expr (MR_IS_STRUCT_OR_UNION (*_mr),	\
			     _mr, (mr_dummy_struct_t*)0);	\
    })

#define MR_OBJ_TYPE_DUMP_EXTRA(S_PTR) ({				\
      mr_conf_init ();							\
      mr_get_struct_type_name_t ctx;					\
      ctx.type_name = MR_OBJ_TYPE_DWARF (S_PTR);			\
      if (ctx.type_name == NULL)					\
	if (0 == setjmp (ctx._jmp_buf))					\
	  __builtin_dump_struct (MR_STRUCT_POINTER (S_PTR),		\
				 &mr_get_struct_type_name_extra,	\
				 &ctx);					\
      ctx.type_name;							\
    })

#define MR_OBJ_TYPE_DUMP(S_PTR) ({					\
      mr_conf_init ();							\
      mr_get_struct_type_name_ctx.type_name = MR_OBJ_TYPE_DWARF (S_PTR); \
      if (mr_get_struct_type_name_ctx.type_name == NULL)		\
	if (0 == setjmp (mr_get_struct_type_name_ctx._jmp_buf))		\
	  __builtin_dump_struct (MR_STRUCT_POINTER (S_PTR),		\
				 &mr_get_struct_type_name);		\
      mr_get_struct_type_name_ctx.type_name;				\
    })

#define MR_OBJ_TYPE_VAR mr_ptr_detect_type_var_
#define MR_OBJ_TYPE_DWARF(S_PTR) MR_OBJ_TYPE_DWARF_ (S_PTR, MR_PASTE2 (MR_OBJ_TYPE_VAR, __COUNTER__))
#define MR_OBJ_TYPE_DWARF_(...) MR_OBJ_TYPE_DWARF__ (__VA_ARGS__)
#define MR_OBJ_TYPE_DWARF__(S_PTR, VAR) ({		\
      __typeof__ (S_PTR) VAR[0];			\
      mr_ptr_detect_type (__FILE__, #VAR, VAR);		\
    })

#define MR_SAVE MR_SAVE_TYPED

#define MR_SAVE_TYPED(...) MR_PASTE2 (MR_SAVE_TYPED_ARGS, MR_NARG (__VA_ARGS__)) (__VA_ARGS__)
#define MR_SAVE_TYPED_ARGS1(S_PTR) MR_SAVE_TYPED_ARGS2 ( , S_PTR)
#define MR_SAVE_TYPED_ARGS2(MR_TYPE_NAME, S_PTR)		\
  MR_IF_ELSE (MR_IS_EMPTY (MR_TYPE_NAME))			\
       (MR_SAVE_STR_TYPED (MR_OBJ_TYPE (S_PTR), S_PTR))		\
       (({ MR_CHECK_TYPES (MR_TYPE_NAME, S_PTR);		\
	   MR_SAVE_STR_TYPED (#MR_TYPE_NAME, S_PTR); }))

#define MR_SAVE_STR_TYPED(MR_TYPE_NAME_STR, S_PTR) ({			\
      __typeof__ (&*(S_PTR)) __src__ = (S_PTR);				\
      mr_fd_t __fd__;							\
      memset (&__fd__, 0, sizeof (__fd__));				\
      __fd__.stype.type = MR_TYPE_NAME_STR;				\
      if (__fd__.stype.type == NULL)					\
	__fd__.stype.type = "";						\
      __fd__.name.str = MR_DEFAULT_NODE_NAME;				\
      __fd__.name.hash_value = 0;					\
      __fd__.unnamed = true;						\
      __fd__.non_persistent = true;					\
      __fd__.stype.mr_type = MR_TYPE_DETECT (__typeof__ (*__src__));	\
      __fd__.stype.mr_type_aux = MR_TYPE_DETECT_PTR (__typeof__ (*__src__)); \
      __fd__.stype.size = sizeof (*__src__);				\
      if (!__builtin_types_compatible_p (__typeof__ (__src__), __typeof__ (S_PTR))) \
	{								\
	  __fd__.stype.is_array = true;					\
	  __fd__.stype.size = sizeof (S_PTR);				\
	  __fd__.stype.dim.dim[0] = (0 + sizeof (S_PTR)) / sizeof (*__src__); \
	  __fd__.stype.dim.size = sizeof (__fd__.stype.dim.dim[0]);	\
	}								\
      mr_detect_type (&__fd__);						\
      mr_save (__src__, &__fd__);					\
    })

#define MR_SAVE_XDR(XDRS, ...) ({					\
      mr_status_t __status__ = MR_FAILURE;				\
      XDR * __xdrs__ = (XDRS);						\
      if (XDR_ENCODE != __xdrs__->x_op)					\
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_XDR_WRONG_ENCODING_MODE);	\
      else								\
	{								\
	  mr_ra_ptrdes_t __ptrs__ = MR_SAVE (__VA_ARGS__);		\
	  if (__ptrs__.ra != NULL)					\
	    {								\
	      __status__ = mr_xdr_save (__xdrs__, &__ptrs__);		\
	      MR_FREE (__ptrs__.ra);					\
	    }								\
	}								\
      __status__;							\
    })

#define MR_SAVE_XDR_RA(...) ({						\
      XDR _xdrs_;							\
      mr_rarray_t _ra_ = { .alloc_size = 0, .MR_SIZE = 0, .data = { NULL }, .type = "uint8_t" }; \
      mr_xdrra_create (&_xdrs_, &_ra_, XDR_ENCODE);			\
      if (MR_SUCCESS != MR_SAVE_XDR (&_xdrs_, __VA_ARGS__))		\
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_XDR_SAVE_FAILED);		\
      _ra_;								\
    })

#define MR_SAVE_METHOD(METHOD, ...) ({					\
      mr_ra_ptrdes_t __ptrs__ = MR_SAVE (__VA_ARGS__);			\
      char * __str__ = NULL;						\
      if (__ptrs__.ra != NULL)						\
	{								\
	  mr_remove_empty_nodes (&__ptrs__);				\
	  __str__ = METHOD (&__ptrs__);					\
	  MR_FREE (__ptrs__.ra);					\
	}								\
      __str__;								\
    })

#define MR_SAVE_METHOD_RA(STR) ({					\
      mr_rarray_t _ra_ = { .alloc_size = 0, .MR_SIZE = 0, .type = "string" }; \
      _ra_.data.string = STR;						\
      if (_ra_.data.ptr)						\
	_ra_.MR_SIZE = _ra_.alloc_size = strlen (_ra_.data.string) + 1;	\
      _ra_;								\
    })

#define MR_SAVE_XML1(...) MR_SAVE_METHOD (mr_xml1_save, __VA_ARGS__)
#define MR_SAVE_CINIT(...) MR_SAVE_METHOD (mr_cinit_save, __VA_ARGS__)
#define MR_SAVE_JSON(...) MR_SAVE_METHOD (mr_json_save, __VA_ARGS__)
#define MR_SAVE_SCM(...) MR_SAVE_METHOD (mr_scm_save, __VA_ARGS__)

#define MR_SAVE_XML_RA(...) MR_SAVE_METHOD_RA (MR_SAVE_XML (__VA_ARGS__))
#define MR_SAVE_XML1_RA(...) MR_SAVE_METHOD_RA (MR_SAVE_XML1 (__VA_ARGS__))
#define MR_SAVE_CINIT_RA(...) MR_SAVE_METHOD_RA (MR_SAVE_CINIT (__VA_ARGS__))
#define MR_SAVE_JSON_RA(...) MR_SAVE_METHOD_RA (MR_SAVE_JSON (__VA_ARGS__))
#define MR_SAVE_SCM_RA(...) MR_SAVE_METHOD_RA (MR_SAVE_SCM (__VA_ARGS__))

#ifdef HAVE_LIBYAML
#define MR_SAVE_YAML(...) MR_SAVE_METHOD (mr_yaml_save, __VA_ARGS__)
#define MR_SAVE_YAML_RA(...) MR_SAVE_METHOD_RA (MR_SAVE_YAML (__VA_ARGS__))
#endif /* HAVE_LIBYAML */

#define MR_LOAD_XDR_ARGS3(MR_TYPE_NAME, XDRS, D_PTR)			\
  MR_IF_ELSE (MR_IS_EMPTY (MR_TYPE_NAME))				\
       (MR_LOAD_XDR_ARGS3_ (MR_OBJ_TYPE (D_PTR), XDRS, D_PTR))		\
       (({ MR_CHECK_TYPES (MR_TYPE_NAME, D_PTR);			\
	   MR_LOAD_XDR_ARGS3_ (#MR_TYPE_NAME, XDRS, D_PTR); }))

#define MR_LOAD_XDR_ARGS3_(MR_TYPE_NAME, XDRS, D_PTR) ({		\
      mr_status_t __status__ = MR_FAILURE;				\
      XDR * __xdrs__ = (XDRS);						\
      if (XDR_DECODE != __xdrs__->x_op)					\
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_XDR_WRONG_ENCODING_MODE);	\
      else								\
	{								\
	  __typeof__ (&*(D_PTR)) _dst_ = (D_PTR);			\
	  mr_fd_t __fd__ =						\
	    {								\
	      .stype.type = MR_TYPE_NAME,				\
	      .name = { .str = NULL, .hash_value = 0, },		\
	      .non_persistent = true,					\
	      .stype.mr_type = MR_TYPE_DETECT (__typeof__ (*_dst_)),	\
	      .stype.size = sizeof (*_dst_),				\
	    };								\
	  memset (_dst_, 0, sizeof (*_dst_));				\
	  mr_detect_type (&__fd__);					\
	  __status__ = mr_xdr_load (_dst_, &__fd__, __xdrs__);		\
	}								\
      __status__;							\
    })

#define MR_LOAD_XDR_ARGS2(MR_TYPE_NAME, XDRS) ({			\
      mr_status_t _status_ = MR_FAILURE;				\
      MR_TYPE_NAME _result_;						\
      memset (&_result_, 0, sizeof (_result_));				\
      _status_ = MR_LOAD_XDR_ARGS3 (MR_TYPE_NAME, XDRS, &_result_);	\
      if (MR_SUCCESS != _status_)					\
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_XDR_LOAD_FAILED);		\
      _result_;								\
    })

#define MR_LOAD_XDR(...) MR_PASTE2 (MR_LOAD_XDR_ARGS, MR_NARG (__VA_ARGS__)) (__VA_ARGS__)

#define MR_LOAD_XDR_RA(MR_TYPE_NAME, RA, ...) ({			\
      XDR _xdrs_;							\
      mr_xdrra_create (&_xdrs_, RA, XDR_DECODE);			\
      MR_IF_ELSE (MR_IS_EMPTY (__VA_ARGS__))				\
	(MR_LOAD_XDR (MR_TYPE_NAME, &_xdrs_))				\
	(MR_LOAD_XDR (MR_TYPE_NAME, &_xdrs_, __VA_ARGS__));		\
    })

#ifdef HAVE_LIBXML2

#define MR_SAVE_XML2_RA(...) MR_SAVE_METHOD_RA (MR_SAVE_XML2 (__VA_ARGS__))

#define MR_SAVE_XML2(...) ({						\
      int __size__;							\
      char * __str__ = NULL;						\
      xmlChar * __xml_str__ = NULL;					\
      mr_ra_ptrdes_t __ptrs__ = MR_SAVE (__VA_ARGS__);			\
      if (__ptrs__.ra != NULL)						\
	{								\
	  mr_remove_empty_nodes (&__ptrs__);				\
	  xmlDocPtr __doc__ = mr_xml2_save (&__ptrs__);			\
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

#define MR_LOAD_XML2_NODE_ARGS3(MR_TYPE_NAME, XML, D_PTR)		\
    MR_IF_ELSE (MR_IS_EMPTY (MR_TYPE_NAME))				\
	 (MR_LOAD_XML2_NODE_ARGS3_ (MR_OBJ_TYPE (D_PTR), XML, D_PTR))	\
	 (({ MR_CHECK_TYPES (MR_TYPE_NAME, D_PTR);			\
	     MR_LOAD_XML2_NODE_ARGS3_ (#MR_TYPE_NAME, XML, D_PTR); }))

#define MR_LOAD_XML2_NODE_ARGS3_(MR_TYPE_NAME, XML, D_PTR) ({		\
      __typeof__ (&*(D_PTR)) _dst_ = (D_PTR);				\
      mr_status_t __status__ = MR_FAILURE;				\
      mr_ra_ptrdes_t __ptrs__ =						\
	{								\
	  .ra = NULL,							\
	  .size = 0,							\
	  .alloc_size = 0,						\
	  .ptrdes_type = MR_PD_LOAD,					\
	};								\
      mr_fd_t __fd__ =							\
	{								\
	  .stype.type = MR_TYPE_NAME,					\
	    .name = { .str = NULL, .hash_value = 0, },			\
	    .non_persistent = true,					\
	    .stype.mr_type = MR_TYPE_DETECT (__typeof__ (*_dst_)),	\
	    .stype.size = sizeof (*_dst_),				\
	};								\
      xmlNodePtr __xml__ = (XML);					\
      memset (_dst_, 0, sizeof (*_dst_));				\
      if (NULL == __xml__)						\
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_NULL_POINTER);		\
      else								\
	{								\
	  mr_detect_type (&__fd__);					\
	  mr_add_ptr_to_list (&__ptrs__);				\
	  if (__ptrs__.ra)						\
	    {								\
	      mr_idx_t __idx__ = mr_xml2_load (__xml__, &__ptrs__);	\
	      if (__idx__ != MR_NULL_IDX)				\
		__status__ = mr_load (_dst_, &__fd__, __idx__, &__ptrs__); \
	      mr_free_load_values (&__ptrs__);				\
	    }								\
	}								\
      __status__;							\
    })

#define MR_LOAD_XML2_NODE_ARGS2(MR_TYPE_NAME, XML) ({			\
      mr_status_t _status_ = MR_FAILURE;				\
      MR_TYPE_NAME _result_;						\
      memset (&_result_, 0, sizeof (_result_));				\
      _status_ = MR_LOAD_XML2_NODE_ARGS3 (MR_TYPE_NAME, XML, &_result_); \
      if (MR_SUCCESS != _status_)					\
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_LOAD_STRUCT_FAILED);		\
      _result_;								\
    })

#define MR_LOAD_XML2_NODE(...) MR_PASTE2 (MR_LOAD_XML2_NODE_ARGS, MR_NARG (__VA_ARGS__)) (__VA_ARGS__)

#define MR_LOAD_XML2_ARGS3(MR_TYPE_NAME, STR, S_PTR) ({			\
      mr_status_t _status_ = MR_FAILURE;				\
      char * __str__ = (STR);						\
      if (NULL == __str__)						\
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_NULL_POINTER);	\
      else								\
	{								\
	  xmlDocPtr __doc__ = xmlParseMemory (__str__, strlen (__str__)); \
	  if (__doc__)							\
	    {								\
	      xmlNodePtr __node__ = xmlDocGetRootElement (__doc__);	\
	      _status_ = MR_LOAD_XML2_NODE_ARGS3 (MR_TYPE_NAME, __node__, (S_PTR)); \
	      xmlFreeDoc (__doc__);					\
	    }								\
	}								\
      _status_;								\
    })

#define MR_LOAD_XML2_ARGS2(MR_TYPE_NAME, STR) ({			\
      mr_status_t ___status___ = MR_FAILURE;				\
      MR_TYPE_NAME __result__;						\
      memset (&__result__, 0, sizeof (__result__));			\
      ___status___ = MR_LOAD_XML2_ARGS3 (MR_TYPE_NAME, STR, &__result__); \
      if (MR_SUCCESS != ___status___)					\
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_LOAD_STRUCT_FAILED);		\
      __result__;							\
    })

#define MR_LOAD_XML2(...) MR_PASTE2 (MR_LOAD_XML2_ARGS, MR_NARG (__VA_ARGS__)) (__VA_ARGS__)

#define MR_LOAD_XML2_RA(MR_TYPE_NAME, RA, ...)				\
  MR_IF_ELSE (MR_IS_EMPTY (__VA_ARGS__))				\
    (MR_LOAD_XML2_ARGS2 (MR_TYPE_NAME, (char*)((RA)->data.ptr)))	\
    (MR_LOAD_XML2_ARGS3 (MR_TYPE_NAME, (char*)((RA)->data.ptr), __VA_ARGS__))

#endif /* HAVE_LIBXML2 */

#define MR_LOAD_METHOD_ARGS4(METHOD, MR_TYPE_NAME, STR, D_PTR)		\
  MR_IF_ELSE (MR_IS_EMPTY (MR_TYPE_NAME))				\
       (MR_LOAD_METHOD_ARGS4_ (METHOD, MR_OBJ_TYPE (D_PTR), STR, D_PTR)) \
       (({ MR_CHECK_TYPES (MR_TYPE_NAME, D_PTR);			\
	   MR_LOAD_METHOD_ARGS4_ (METHOD, #MR_TYPE_NAME, STR, D_PTR); }))

#define MR_LOAD_METHOD_ARGS4_(METHOD, MR_TYPE_NAME, STR, D_PTR) ({	\
      mr_ra_ptrdes_t _ptrs_ =						\
	{ .ra = NULL, .size = 0, .alloc_size = 0, .ptrdes_type = MR_PD_LOAD, }; \
      __typeof__ (&*(D_PTR)) _dst_ = (D_PTR);				\
      memset (_dst_, 0, sizeof (*_dst_));				\
      mr_conf_init ();							\
      mr_add_ptr_to_list (&_ptrs_);					\
      mr_status_t _status_ = MR_FAILURE;				\
      if (_ptrs_.ra)							\
	{								\
	  _status_ = METHOD ((STR), &_ptrs_);				\
	  if (MR_SUCCESS == _status_)					\
	    {								\
	      mr_fd_t _fd_ =						\
		{							\
		  .stype.type = MR_TYPE_NAME,				\
		  .name = { .str = NULL, .hash_value = 0, },		\
		  .stype.mr_type = MR_TYPE_DETECT (__typeof__ (*_dst_)), \
		  .stype.size = sizeof (*_dst_),			\
		};							\
	      mr_detect_type (&_fd_);					\
	      _status_ = mr_load (_dst_, &_fd_, 1, &_ptrs_);		\
	    }								\
	  mr_free_load_values (&_ptrs_);				\
	}								\
      _status_;								\
    })

#define MR_LOAD_METHOD_ARGS3(METHOD, MR_TYPE_NAME, STR) ({		\
      mr_status_t __status__ = MR_FAILURE;				\
      MR_TYPE_NAME __result__;						\
      memset (&__result__, 0, sizeof (__result__));			\
      __status__ = MR_LOAD_METHOD_ARGS4 (METHOD, MR_TYPE_NAME, STR, &__result__); \
      if (MR_SUCCESS != __status__)					\
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_LOAD_STRUCT_FAILED);		\
      __result__;							\
    })

#define MR_LOAD_METHOD(...) MR_PASTE2 (MR_LOAD_METHOD_ARGS, MR_NARG (__VA_ARGS__)) (__VA_ARGS__)

#define MR_LOAD_METHOD_RA(METHOD, MR_TYPE_NAME, RA, ...)		\
  MR_IF_ELSE (MR_IS_EMPTY (__VA_ARGS__))				\
    (MR_LOAD_METHOD_ARGS3 (METHOD, MR_TYPE_NAME, (RA)->data.string))	\
    (MR_LOAD_METHOD_ARGS4 (METHOD, MR_TYPE_NAME, (RA)->data.string, __VA_ARGS__))

#ifdef HAVE_LIBYAML
#define MR_LOAD_YAML(...) MR_LOAD_METHOD (mr_yaml_load, __VA_ARGS__)
#define MR_LOAD_YAML_RA(...) MR_LOAD_METHOD_RA (mr_yaml_load, __VA_ARGS__)
#endif /* HAVE_LIBYAML */

#ifdef HAVE_BISON_FLEX

#define MR_LOAD_XML1(...) MR_LOAD_METHOD (mr_xml1_load, __VA_ARGS__)
#define MR_LOAD_CINIT(...) MR_LOAD_METHOD (mr_cinit_load, __VA_ARGS__)
#define MR_LOAD_JSON(...) MR_LOAD_METHOD (mr_json_load, __VA_ARGS__)
#define MR_LOAD_SCM(...) MR_LOAD_METHOD (mr_scm_load, __VA_ARGS__)

#define MR_LOAD_XML1_RA(...) MR_LOAD_METHOD_RA (mr_xml1_load, __VA_ARGS__)
#define MR_LOAD_CINIT_RA(...) MR_LOAD_METHOD_RA (mr_cinit_load, __VA_ARGS__)
#define MR_LOAD_JSON_RA(...) MR_LOAD_METHOD_RA (mr_json_load, __VA_ARGS__)
#define MR_LOAD_SCM_RA(...) MR_LOAD_METHOD_RA (mr_scm_load, __VA_ARGS__)

#else /* ! HAVE_BISON_FLEX */

#define MR_LOAD_STUB(...) MR_PASTE2 (MR_LOAD_STUB_ARGS, MR_NARG (__VA_ARGS__)) (__VA_ARGS__)
#define MR_LOAD_STUB_ARGS2(MR_TYPE_NAME, STR) ({		\
      MR_TYPE_NAME __result__;					\
      memset (&__result__, 0, sizeof (__result__));		\
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_LOAD_METHOD_MISSED);	\
      __result__;						\
    })
#define MR_LOAD_STUB_ARGS3(MR_TYPE_NAME, STR, D_PTR) ({ MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_LOAD_METHOD_MISSED); 0;})

#define MR_LOAD_XML1 MR_LOAD_STUB
#define MR_LOAD_CINIT MR_LOAD_STUB
#define MR_LOAD_JSON MR_LOAD_STUB
#define MR_LOAD_SCM MR_LOAD_STUB

#endif /* HAVE_BISON_FLEX */

#ifdef HAVE_LIBXML2

#define MR_LOAD_XML MR_LOAD_XML2
#define MR_SAVE_XML MR_SAVE_XML2
#define MR_LOAD_XML_RA MR_LOAD_XML2_RA

#else /* ! HAVE_LIBXML2 */

#define MR_SAVE_XML MR_SAVE_XML1
#define MR_LOAD_XML MR_LOAD_XML1
#define MR_LOAD_XML_RA MR_LOAD_XML1_RA

#endif /* HAVE_LIBXML2 */

#define MR_EQ_2_2 0
#define MR_EQ_3_3 0
#define MR_UNDEF_MR_PRINT_SERIALIZATION_METHOD 0

#define MR_PRINT_SERIALIZATION_METHOD_GET()				\
  MR_IF_ELSE (MR_PASTE2 (MR_UNDEF_, MR_PRINT_SERIALIZATION_METHOD))	\
    (MR_PRINT_SERIALIZATION_METHOD)					\
    (CINIT)

#define MR_PRINT_IN_PAREN(COUNT, FD, VALUE)		\
  MR_IF_ELSE (MR_PASTE2 (MR_EQ_2_, COUNT))		\
       (MR_PRINT_IN_PAREN_ (COUNT, FD, VALUE))		\
       (MR_PRINT_STRUCT_ (FD, MR_REMOVE_PAREN_ VALUE, MR_PRINT_SERIALIZATION_METHOD_GET ()))

#define MR_PRINT_IN_PAREN_(COUNT, FD, VALUE)		\
  MR_IF_ELSE (MR_PASTE2 (MR_EQ_3_, COUNT))		\
       (MR_PRINT_VALUE (FD, VALUE))			\
       (MR_PRINT_STRUCT_ (FD, MR_REMOVE_PAREN_ VALUE))

#define MR_PRINT_STRUCT_(...) MR_PRINT_STRUCT (__VA_ARGS__)
#define MR_PRINT_STRUCT(FD, TYPE, PTR, METHOD) ({	\
      char * _dump_ = MR_SAVE_ ## METHOD (TYPE, PTR);	\
	int _rv_ = 0;					\
	if (_dump_)					\
	  {						\
	    _rv_ = fprintf (FD, "%s", _dump_);		\
	    MR_FREE (_dump_);				\
	  }						\
	_rv_;						\
    })

#define MR_CAST_TO_PTR(OBJ) __builtin_choose_expr ((MR_POINTER_TYPE_CLASS == __builtin_classify_type (OBJ)) && \
						   !__builtin_types_compatible_p (__typeof__ (OBJ), void *) && \
						   !__builtin_types_compatible_p (__typeof__ (OBJ), void const *) && \
						   !__builtin_types_compatible_p (__typeof__ (OBJ), void volatile *) && \
						   !__builtin_types_compatible_p (__typeof__ (OBJ), void const volatile *) \
						   , (OBJ), (void**)0)

#define MR_ARRAY_SIZE(OBJ) ({						\
      __typeof__ (OBJ) _mr[0];						\
      __builtin_choose_expr (__builtin_types_compatible_p (__typeof__ (0 + MR_CAST_TO_PTR (*_mr)), __typeof__ (*_mr)), -1, sizeof (*_mr)); \
    })

#define MR_TYPE_DETECT_OBJ(OBJ) ({		\
      __typeof__ (OBJ) _mr[0];			\
      MR_TYPE_DETECT (__typeof__ (*_mr));	\
    })

#define MR_PRINT_ARG(OBJ, TYPE_OBJ)					\
  __builtin_choose_expr (MR_IS_STRUCT_OR_UNION (TYPE_OBJ),		\
			 &__builtin_choose_expr (MR_IS_STRUCT_OR_UNION (TYPE_OBJ), \
						 (OBJ), ""),		\
			 (OBJ))

#define MR_PRINT_VALUE(FD, X) ({					\
      __typeof__ (X) __mr[0];						\
      mr_print_value (FD,						\
		      MR_TYPE_DETECT (__typeof__ (*__mr)),		\
		      MR_TYPE_DETECT_OBJ (*MR_CAST_TO_PTR (*__mr)),	\
		      __builtin_classify_type (*__mr),			\
		      MR_ARRAY_SIZE (*__mr),				\
		      MR_OBJ_TYPE (MR_PRINT_ARG (*__mr, *__mr)),	\
		      MR_STRINGIFY_READONLY (MR_PRINT_SERIALIZATION_METHOD_GET ()), \
		      MR_PRINT_ARG (X, *__mr));				\
    })

#define MR_PRINT_ONE_ELEMENT(FD, X, I)				\
  MR_IF_ELSE (MR_IS_IN_PAREN (X))				\
       (MR_PRINT_IN_PAREN (MR_NARG X, FD, X))			\
       (MR_PRINT_VALUE (FD, X))

#define MR_ADD(NAME, I, REC, X) REC + X
#define MR_PRINT(...) (void)MR_FPRINT (stdout, __VA_ARGS__)
#define MR_FPRINT(FD, ...) (MR_FOR (FD, MR_NARG (__VA_ARGS__), MR_ADD, MR_PRINT_ONE_ELEMENT, __VA_ARGS__))

typedef char * string_t;
typedef char mr_static_string_t[1];
typedef uint32_t mr_idx_t;
typedef uint32_t mr_size_t;
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

#ifdef MR_HAVE_INT128

#define MR_BUILTIN_TYPES_INT128 , __int128, signed __int128, __int128 signed, unsigned __int128, __int128 unsigned
typedef unsigned __int128 mr_uintmax_t;
typedef signed __int128 mr_intmax_t;
typedef unsigned __int128 mr_uint128_t;
typedef signed __int128 mr_int128_t;

#else /* HAVE_INT128 */

#define MR_BUILTIN_TYPES_INT128
typedef unsigned long long int mr_uintmax_t;
typedef signed long long int mr_intmax_t;
typedef uint64_t mr_uint128_t[2];
typedef int64_t mr_int128_t[2];

#endif

#ifndef MR_MODE
#define MR_MODE_UNDEFINED
#define MR_MODE PROTO
#endif
#include <mr_protos.h>
#ifdef MR_MODE_UNDEFINED
#undef MR_MODE_UNDEFINED
#undef MR_MODE
#endif

typedef __typeof__ (((mr_enum_value_t*)0)->_unsigned) mr_enum_value_type_t;

extern mr_conf_t mr_conf;

#ifdef HAVE_BUILTIN_DUMP_STRUCT_EXTRA_ARGS
extern int mr_get_struct_type_name_extra (mr_get_struct_type_name_t * ctx, const char * fmt, ...);
extern int mr_dump_struct_type_detection (mr_dump_struct_type_ctx_t * ctx, const char * fmt, ...);
extern int mr_dump_struct_bitfield_detection (mr_dump_struct_type_ctx_t * ctx, const char * fmt, ...);
#else /* ! HAVE_BUILTIN_DUMP_STRUCT_EXTRA_ARGS */
extern __thread mr_get_struct_type_name_t mr_get_struct_type_name_ctx;
extern int mr_get_struct_type_name (const char * fmt, ...);
#endif /* HAVE_BUILTIN_DUMP_STRUCT_EXTRA_ARGS */

extern void mr_add_type (mr_td_t * tdp);
extern void mr_add_dwarf (mr_dwarf_t * mr_dwarf);
extern mr_uintmax_t mr_strtouintmax (char * s, char ** endptr, int base);
extern char * mr_read_xml_doc (FILE * fd);

extern mr_ra_ptrdes_t mr_save (void * data, mr_fd_t * fdp);
extern void mr_reorder_strings (mr_ra_ptrdes_t * ptrs);
extern void mr_free_load_values (mr_ra_ptrdes_t * ptrs);
extern mr_status_t mr_load (void * data, mr_fd_t * fdp, mr_idx_t idx, mr_ra_ptrdes_t * ptrs);
#ifdef HAVE_LIBXML2
extern xmlDocPtr mr_xml2_save (mr_ra_ptrdes_t * ptrs);
extern mr_idx_t mr_xml2_load (xmlNodePtr, mr_ra_ptrdes_t * ptrs);
#endif /* HAVE_LIBXML2 */
#ifdef HAVE_LIBYAML
extern char * mr_yaml_save (mr_ra_ptrdes_t * ptrs);
extern mr_status_t mr_yaml_load (char * str, mr_ra_ptrdes_t * ptrs);
#endif /* HAVE_LIBYAML */
#ifdef HAVE_RPC_TYPES_H
extern mr_status_t mr_xdr_save (XDR * xdrs, mr_ra_ptrdes_t * ptrs);
extern mr_status_t mr_xdr_load (void * data, mr_fd_t * fdp, XDR * xdrs);
extern void mr_xdrra_create (XDR * xdrs, mr_rarray_t * rarray, enum xdr_op op);
#endif /* HAVE_RPC_TYPES_H */

extern char * mr_xml1_save (mr_ra_ptrdes_t * ptrs);
extern char * mr_cinit_save (mr_ra_ptrdes_t * ptrs);
extern char * mr_json_save (mr_ra_ptrdes_t * ptrs);
extern char * mr_scm_save (mr_ra_ptrdes_t * ptrs);

#ifdef HAVE_BISON_FLEX
extern mr_status_t mr_xml1_load (char * str, mr_ra_ptrdes_t * ptrs);
extern mr_status_t mr_cinit_load (char * str, mr_ra_ptrdes_t * ptrs);
extern mr_status_t mr_json_load (char * str, mr_ra_ptrdes_t * ptrs);
extern mr_status_t mr_scm_load (char * str, mr_ra_ptrdes_t * ptrs);
#endif /* HAVE_BISON_FLEX */

extern void * mr_calloc (const char * filename, const char * function, int line, size_t count, size_t size);
extern void * mr_realloc (const char * filename, const char * function, int line, void * ptr, size_t size);
extern void mr_free (const char * filename, const char * function, int line, void * ptr);
extern char * mr_strdup (const char * str);
extern char * mr_strndup (const char * str, size_t size);

extern mr_idx_t mr_add_ptr_to_list (mr_ra_ptrdes_t * ptrs);
extern mr_idx_t mr_last_child_for_parent (mr_ra_ptrdes_t * ptrs, mr_idx_t parent);
extern void mr_add_child (mr_ra_ptrdes_t * ptrs, mr_idx_t parent, mr_idx_t child);
extern void mr_detect_type (mr_fd_t * fdp);
extern char * mr_ptr_detect_type (char * filename, char * varname, ...);
extern mr_hash_value_t mr_var_get_hash (mr_ptr_t x, const void * context);
extern int mr_var_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context);

#define MR_AND_IS_STRING(X) && __builtin_types_compatible_p (char, __typeof__ (*(X)))
#define MR_VALIDATE_ALL_ARGS_ARE_STRINGS(...) (true MR_FOREACH (MR_AND_IS_STRING, __VA_ARGS__))
#define mr_type_void_fields(type, ...) MR_COMPILETIME_ASSERT (MR_VALIDATE_ALL_ARGS_ARE_STRINGS (__VA_ARGS__)); mr_type_void_fields_impl (type, __VA_ARGS__, NULL)
extern void __attribute__ ((sentinel(0))) mr_type_void_fields_impl (char * type, char * name, ...);
extern mr_size_t mr_type_size (mr_type_t mr_type);
extern void mr_conf_init ();
extern mr_status_t mr_free_recursively (mr_ra_ptrdes_t * ptrs);
extern mr_status_t mr_copy_recursively (mr_ra_ptrdes_t * ptrs, void * data);
extern mr_hash_value_t mr_hash_struct (mr_ra_ptrdes_t * ptrs);
extern int mr_cmp_structs (mr_ra_ptrdes_t * x, mr_ra_ptrdes_t * y);
extern mr_fd_t * mr_get_fd_by_name (mr_td_t * tdp, char * name);
extern mr_enum_value_type_t mr_get_enum_value (mr_td_t * tdp, void * data);
extern mr_ed_t * mr_get_enum_by_value (mr_td_t * tdp, mr_enum_value_type_t value);
extern mr_ed_t * mr_get_enum_by_name (char * name);
extern mr_status_t mr_load_bitfield_value (mr_ptrdes_t * ptrdes, mr_uintmax_t * value);
extern mr_status_t mr_save_bitfield_value (mr_ptrdes_t * ptrdes, mr_uintmax_t * value);
extern mr_td_t * mr_get_td_by_name (char * type);
extern mr_td_t * mr_get_td_by_name_internal (char * type);
extern char * mr_message_format (mr_message_id_t message_id, va_list args);
extern void mr_message (const char * file_name, const char * func_name, int line, mr_log_level_t log_level, mr_message_id_t message_id, ...);
extern void * mr_rarray_append (mr_rarray_t * rarray, ssize_t size);
extern void * mr_rarray_allocate_element (void ** data, ssize_t * size, ssize_t * alloc_size, ssize_t element_size);
extern int __attribute__ ((format (printf, 2, 3))) mr_ra_printf (mr_rarray_t * rarray, const char * format, ...);
extern int mr_print_value (FILE * fd, mr_type_t mr_type, mr_type_t mr_type_aux, mr_type_class_t mr_type_class, ssize_t size, char * type, char * method, ...);

extern mr_fd_t * mr_get_any_fd_by_name (const char * name, mr_td_t * tdp);
extern void xml_unquote_string (mr_substr_t * substr, char * dst);
#define mr_ptrs_dfs(ptrs, processor, context, ...) mr_ptrs_dfs_impl (ptrs, processor, context, MR_IF_ELSE (MR_IS_EMPTY (__VA_ARGS__)) (1) (__VA_ARGS__))
extern mr_status_t mr_ptrs_dfs_impl (mr_ra_ptrdes_t * ptrs, mr_ptrdes_processor_t processor, mr_ptr_t context, mr_idx_t start);
extern void mr_remove_empty_nodes (mr_ra_ptrdes_t * ptrs);
extern mr_status_t mr_generic_sort (void * data, size_t count, char * key_type);
extern mr_hash_value_t mr_generic_hash (const mr_ptr_t x, const void * context);
extern int mr_generic_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context);
extern mr_status_t mr_basic_types_sort (void * data, size_t count, char * key_type, mr_type_t mr_type, size_t element_size);

extern mr_hash_value_t mr_hash_block (void * block, mr_size_t size);
extern mr_hash_value_t mr_hash_str (char * str);
extern int mr_fd_name_and_type_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context);
extern mr_hash_value_t mr_fd_name_and_type_get_hash (mr_ptr_t x, const void * context);
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
extern mr_hash_value_t mr_ud_override_hash (mr_ptr_t x, const void * context);
extern int mr_ud_override_cmp (mr_ptr_t x, mr_ptr_t y, const void * context);
extern mr_hash_value_t mr_ed_name_get_hash (mr_ptr_t x, const void * context);
extern int mr_ed_name_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context);

#endif /* _METARESC_H_ */
