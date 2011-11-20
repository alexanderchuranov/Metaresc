/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

/* ResLib prototypes */
#ifndef _RESLIB_H_
#define _RESLIB_H_

#include <rlpp.h>
#include <rlexport.h>

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
#define RL_MAX_TYPES (256)
#define RL_MAX_STRING_LENGTH ((unsigned int)-1)

#define RL_TYPE_ANONYMOUS_UNION_TEMPLATE "rl_type_anonymous_union_%d_t"

#define RL_INT_TO_STRING_BUF_SIZE (32)
#define RL_FLOAT_TO_STRING_BUF_SIZE (256)
#define RL_CHAR_TO_STRING_BUF_SIZE (8)

#define RL_MAX_INDENT_LEVEL (30) /* the same constant as in libxml2 */
#define RL_MIN(X,Y) ({ typeof(X) _x_ = (X); typeof(Y) _y_ = (Y); (_x_ < _y_) ? _x_ : _y_; })
#define RL_MAX(X,Y) ({ typeof(X) _x_ = (X); typeof(Y) _y_ = (Y); (_x_ > _y_) ? _x_ : _y_; })
#define RL_LIMIT_LEVEL(LEVEL) RL_MIN (LEVEL, RL_MAX_INDENT_LEVEL)

/* each refereed structure will have REF_IDX property */
#define RL_REF_IDX "ref_idx"
/* references on already saved structures will be replaced with nodes that have only REF index property */
#define RL_REF "ref"
#define RL_REF_CONTENT "ref_content"
#define RL_RARRAY_SIZE "size"
/* XML attribute for zero length strings */
#define RL_ISNULL "isnull"

#define RL_BITMASK_OR_DELIMITER " | "

#define RL_MESSAGE(LOG_LEVEL, /* MSG_ID */ ...) rl_message (__FILE__, __FUNCTION__, __LINE__, LOG_LEVEL, __VA_ARGS__)
#define RL_MESSAGE_UNSUPPORTED_NODE_TYPE_(FDP)				\
  ({									\
    rl_fd_t * __fdp__ = FDP;						\
    rl_td_t * rl_type_td = rl_get_td_by_name ("rl_type_t");		\
    rl_td_t * rl_type_ext_td = rl_get_td_by_name ("rl_type_ext_t");	\
    rl_fd_t * rl_type_fd = rl_type_td ? rl_get_enum_by_value (rl_type_td, __fdp__->rl_type) : NULL; \
    rl_fd_t * rl_type_ext_fd = rl_type_ext_td ? rl_get_enum_by_value (rl_type_ext_td, __fdp__->rl_type_ext) : NULL; \
    RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_UNSUPPORTED_NODE_TYPE, (rl_type_fd ? rl_type_fd->name : "unknown"), __fdp__->rl_type, (rl_type_ext_fd ? rl_type_ext_fd->name : "unknown"), __fdp__->rl_type_ext); \
  })

/* make a string from argument in writable memory. #STR itself is in read-only memory */
#define RL_STRINGIFY(STR) (char []) { #STR }

#define RL_MEM_INIT(FUNC, /* ATTR */ ...) void __VA_ARGS__ rl_mem_init (void) { FUNC; }

#define RL_MALLOC(SIZE) (rl_conf.rl_mem.malloc)(__FILE__, __FUNCTION__, __LINE__, SIZE)
#define RL_REALLOC(PTR, SIZE) (rl_conf.rl_mem.realloc)(__FILE__, __FUNCTION__, __LINE__, PTR, SIZE)
#define RL_STRDUP(STR) (rl_conf.rl_mem.strdup)(__FILE__, __FUNCTION__, __LINE__, STR)
#define RL_FREE(PTR) (rl_conf.rl_mem.free)(__FILE__, __FUNCTION__, __LINE__, PTR)

/*
  you can redefine this prefixes from outside before first include of reslib.h
*/
#ifndef RL_DESCRIPTOR_PREFIX
#define RL_DESCRIPTOR_PREFIX(RL_TYPE_NAME) rl_td_ ## RL_TYPE_NAME
#endif /* RL_DESCRIPTOR_PREFIX */
#ifndef RL_TYPEDEF_PREFIX
#define RL_TYPEDEF_PREFIX(RL_TYPE_NAME) RL_TYPE_NAME
#endif /* RL_TYPEDEF_PREFIX */
#ifndef RL_ANON_UNION_NAME
#define RL_ANON_UNION_NAME(LINE) anon_union_ ## LINE
#endif /* RL_ANON_UNION_NAME */
#ifndef RL_CONSTRUCTOR_PREFIX
#define RL_CONSTRUCTOR_PREFIX(RL_TYPE_NAME) rl_init_ ## RL_TYPE_NAME
#endif /* RL_CONSTRUCTOR_PREFIX */
/* this attribute can be redefined from outside */
#ifndef RL_DESCRIPTOR_ATTR
#define RL_DESCRIPTOR_ATTR static
#endif /* RL_DESCRIPTOR_ATTR */

/*
  Help macro for internal type detection. It compares variable with all known builin types.
  Compaund types are detected in runtime.
*/
#define RL_TYPE_DETECT_INNER(PREFIX, TYPE, SUFFIX, PAREN_SUFFIX)	\
  (0 /* RL_TYPE_NONE */							\
   | (__builtin_types_compatible_p (PREFIX void SUFFIX, TYPE) ? RL_TYPE_VOID : 0) \
   | (__builtin_types_compatible_p (PREFIX int8_t SUFFIX, TYPE) ? RL_TYPE_INT8 : 0) \
   | (__builtin_types_compatible_p (PREFIX uint8_t SUFFIX, TYPE) ? RL_TYPE_UINT8 : 0) \
   | (__builtin_types_compatible_p (PREFIX int16_t SUFFIX, TYPE) ? RL_TYPE_INT16 : 0) \
   | (__builtin_types_compatible_p (PREFIX uint16_t SUFFIX, TYPE) ? RL_TYPE_UINT16 : 0) \
   | (__builtin_types_compatible_p (PREFIX int32_t SUFFIX, TYPE) ? RL_TYPE_INT32 : 0) \
   | (__builtin_types_compatible_p (PREFIX uint32_t SUFFIX, TYPE) ? RL_TYPE_UINT32 : 0) \
   | (__builtin_types_compatible_p (PREFIX int64_t SUFFIX, TYPE) ? RL_TYPE_INT64 : 0) \
   | (__builtin_types_compatible_p (PREFIX uint64_t SUFFIX, TYPE) ? RL_TYPE_UINT64 : 0) \
   | (__builtin_types_compatible_p (PREFIX RL_UNCOVERED_TYPE SUFFIX, TYPE) ? RL_PASTE2 (RL_TYPE_INT, RL_SIZEOF_UNCOVERED_TYPE) : 0) \
   | (__builtin_types_compatible_p (PREFIX unsigned RL_UNCOVERED_TYPE SUFFIX, TYPE) ? RL_PASTE2 (RL_TYPE_UINT, RL_SIZEOF_UNCOVERED_TYPE) : 0) \
   | (__builtin_types_compatible_p (PREFIX float SUFFIX, TYPE) ? RL_TYPE_FLOAT : 0) \
   | (__builtin_types_compatible_p (PREFIX double SUFFIX, TYPE) ? RL_TYPE_DOUBLE : 0) \
   | (__builtin_types_compatible_p (PREFIX long double SUFFIX, TYPE) ? RL_TYPE_LONG_DOUBLE : 0) \
   | (__builtin_types_compatible_p (PREFIX char SUFFIX, TYPE) ? RL_TYPE_CHAR : 0) \
   | (__builtin_types_compatible_p (PREFIX char PAREN_SUFFIX [], TYPE) ? RL_TYPE_CHAR_ARRAY : 0) \
   | (__builtin_types_compatible_p (PREFIX char * SUFFIX, TYPE) ? RL_TYPE_STRING : 0) \
   | (__builtin_types_compatible_p (const char * SUFFIX, TYPE) ? RL_TYPE_STRING : 0) \
   | (__builtin_types_compatible_p (volatile char * SUFFIX, TYPE) ? RL_TYPE_STRING : 0) \
   | (__builtin_types_compatible_p (const volatile char * SUFFIX, TYPE) ? RL_TYPE_STRING : 0) \
   )
#define RL_TYPE_DETECT(TYPE) RL_TYPE_DETECT_INNER ( , TYPE, ,)
#define RL_TYPE_DETECT_PTR(TYPE) (RL_TYPE_DETECT_INNER ( , TYPE, *, (*)) | RL_TYPE_DETECT_INNER (const, TYPE, *, (*)) | RL_TYPE_DETECT_INNER (volatile, TYPE, *, (*)) | RL_TYPE_DETECT_INNER (const volatile, TYPE, *, (*)))
/* Help macro for arrays auto-detection */
#define RL_TYPE_EXT_DETECT(TYPE, S_PTR) (__builtin_types_compatible_p (TYPE [], typeof (S_PTR)) ? RL_TYPE_EXT_ARRAY : RL_TYPE_EXT_NONE)

/* internal macros for arguments evaluation and concatination */
#define RL_PASTE2(...) RL_PASTE2_ (__VA_ARGS__)
#define RL_PASTE2_(_0, _1) _0 ## _1
#define RL_PASTE3(...) RL_PASTE3_ (__VA_ARGS__)
#define RL_PASTE3_(_0, _1, _2) _0 ## _1 ## _2
#define RL_PASTE4(...) RL_PASTE4_ (__VA_ARGS__)
#define RL_PASTE4_(_0, _1, _2, _3) _0 ## _1 ## _2 ## _3

/* Interface macros for unrolled loops from rlpp.h */
#define RL_FOREACH(X, ...) RL_PASTE2 (RL_FOREACH, RL_NARG (__VA_ARGS__)) (X, __VA_ARGS__)
#define RL_FOR(NAME, N, OP, FUNC, ...) RL_PASTE2 (RL_FOR, N) (NAME, OP, FUNC, __VA_ARGS__)

/*
  Next group of macroses detects empty argument list.
*/
#define P00_IS__EQ__(...) ,
#define P00_IS_EMPTY_CASE_0001 ,

#define RL_IS_EMPTY(...)						\
  RL_HAS_COMMA								\
  (RL_PASTE2								\
   (P00_IS_EMPTY_CASE_,							\
    RL_PASTE4 (								\
	       /* test if there is just one argument, eventually an empty one */ \
	       RL_HAS_COMMA (__VA_ARGS__),				\
	       /* test if P99_IS__EQ__ together with the argument adds a comma */ \
	       RL_HAS_COMMA (P00_IS__EQ__ __VA_ARGS__),			\
	       /* test if the argument together with a parenthesis adds a comma */ \
	       RL_HAS_COMMA (__VA_ARGS__ (~)),				\
	       /* test if placing it between P99_IS__EQ__ and the parenthesis adds a comma */ \
	       RL_HAS_COMMA (P00_IS__EQ__ __VA_ARGS__ (~))		\
									)))

/* Next group of macroses checks that it has only one argument and it is 0 */
#define RL_IS_0_EQ_0 ,
#define RL_IS_0_EQ_0_CASE_11 ,

#define RL_IS_EQ_0(...) RL_PASTE2 (RL_IS_EQ_0_ARG, RL_HAS_COMMA (__VA_ARGS__)) (__VA_ARGS__)
#define RL_IS_EQ_0_ARG1(...) 0
#define RL_IS_EQ_0_ARG0(X) RL_IS_EQ_0_ARG0_ (RL_PASTE2 (RL_IS_0_EQ_, X))
#define RL_IS_EQ_0_ARG0_(...) RL_IS_EQ_0_ARG0__ (__VA_ARGS__)
#define RL_IS_EQ_0_ARG0__(X, ...) RL_HAS_COMMA (RL_PASTE3 (RL_IS_0_EQ_0_CASE_, RL_IS_EMPTY (X), RL_IS_EMPTY (__VA_ARGS__)))

/* If clause implementation. Usage RL_IF_ELSE (test_value) (expand_if_nonzero) (expand_if_zero) */
#define RL_IGNORE(...)
#define RL_IDENT(...) __VA_ARGS__
#define RL_IF_ELSE0(...) __VA_ARGS__ RL_IGNORE
#define RL_IF_ELSE1(...) RL_IDENT
#define RL_IF_ELSE(...) RL_PASTE2 (RL_IF_ELSE, RL_IS_EQ_0 (__VA_ARGS__))

/* Next macro RL_HAS_NO_PAREN(...) checks that arguments are in parents */
#define RL_DETECT_PAREN(...) 0
#define RL_HAS_NO_PAREN(...) RL_PASTE2 (RL_HAS_NO_PAREN_ARG, RL_HAS_COMMA (__VA_ARGS__)) (__VA_ARGS__)
#define RL_HAS_NO_PAREN_ARG0(X) RL_IF_ELSE (RL_IS_EQ_0 (RL_DETECT_PAREN X)) (0) (1)
#define RL_HAS_NO_PAREN_ARG1(...) 1

/* RL_REMOVE_PAREN(...) removes parents if they are presented */
#define RL_REMOVE_PAREN_(...) __VA_ARGS__
#define RL_REMOVE_PAREN(...) RL_IF_ELSE (RL_HAS_NO_PAREN (__VA_ARGS__)) (__VA_ARGS__) (RL_REMOVE_PAREN_ __VA_ARGS__)

/* Main macroses for types definition. It passes type of definition to next level. */
#define TYPEDEF_STRUCT(...) P00_TYPEDEF (STRUCT, __VA_ARGS__)
#define TYPEDEF_UNION(...) P00_TYPEDEF (UNION, __VA_ARGS__)
#define TYPEDEF_ENUM(...) P00_TYPEDEF (ENUM, __VA_ARGS__)
#define TYPEDEF_CHAR_ARRAY(...) P00_TYPEDEF (CHAR_ARRAY, __VA_ARGS__)
#define TYPEDEF_FUNC(...) P00_TYPEDEF (FUNC, __VA_ARGS__)

/*
  Checks if RL_MODE was defined. If it was undefined automtically produce prototypes and descriptors.
  Library could be unintrusively extended for other modes. Defined RL_MODE transparently passed to lower level.
*/
#define RL_IS_RL_MODE_EQ_RL_MODE 0
#define P00_TYPEDEF(...)						\
  RL_IF_ELSE (RL_PASTE2 (RL_IS_RL_MODE_EQ_, RL_MODE))			\
  (P00_TYPEDEF_MODE (RL_MODE, __VA_ARGS__))				\
  (P00_TYPEDEF_MODE (PROTO, __VA_ARGS__) P00_TYPEDEF_MODE (DESC, __VA_ARGS__))

/*
  Here is switch on definition type.
  CHAR_ARRAY and FUNC should be directly expanded to RL_TYPEDEF_CHAR_ARRAY and RL_TYPEDEF_FUNC respectively.
*/
#define P00_TYPEDEF_MODE(P00_MODE, P00_TYPE, ...) RL_PASTE2 (P00_TYPEDEF_, P00_TYPE) (P00_MODE, P00_TYPE, __VA_ARGS__)
#define P00_TYPEDEF_STRUCT P00_TYPEDEF_COMPAUND
#define P00_TYPEDEF_UNION P00_TYPEDEF_COMPAUND
#define P00_TYPEDEF_ENUM P00_TYPEDEF_COMPAUND
#define P00_TYPEDEF_CHAR_ARRAY(P00_MODE, P00_TYPE, ...) RL_PASTE2 (RL_TYPEDEF_CHAR_ARRAY_, P00_MODE) (__VA_ARGS__)
#define P00_TYPEDEF_FUNC(P00_MODE, P00_TYPE, ...) RL_PASTE2 (RL_TYPEDEF_FUNC_, P00_MODE) (__VA_ARGS__)

/*
  TYPEDEF_{STRUCT|UNION|ENUM} have first agrument type name.
  Second argument might be ATTRIBUTES(...) with typedef attributes, comments and extended meta information. The rest is list of fields/enums declarations.
  Next macro checks that ATTRIBUTES are presented.
*/
#define P00_TYPEDEF_COMPAUND(P00_MODE, P00_TYPE, P00_TYPE_NAME, ...)	\
  RL_IF_ELSE (RL_IS_EMPTY (__VA_ARGS__))				\
  (TYPEDEF_ATTR (P00_MODE, P00_TYPE, P00_TYPE_NAME, ATTRIBUTES (), ))	\
  (P00_TYPEDEF_COMPAUND_ (P00_MODE, P00_TYPE, P00_TYPE_NAME, __VA_ARGS__))
#define P00_TYPEDEF_COMPAUND_(P00_MODE, P00_TYPE, P00_TYPE_NAME, FIRST, ...) \
  RL_IF_ELSE (RL_HAS_NO_PAREN (FIRST))					\
  (RL_IF_ELSE (RL_PASTE2 (P00_IS_ATTRIBUTES_EQ_, FIRST))		\
   (TYPEDEF_ATTR (P00_MODE, P00_TYPE, P00_TYPE_NAME, ATTRIBUTES (), FIRST, __VA_ARGS__)) \
   (TYPEDEF_ATTR (P00_MODE, P00_TYPE, P00_TYPE_NAME, FIRST, __VA_ARGS__))) \
  (TYPEDEF_ATTR (P00_MODE, P00_TYPE, P00_TYPE_NAME, ATTRIBUTES (), FIRST, __VA_ARGS__))

#define P00_IS_ATTRIBUTES_EQ_ATTRIBUTES(...) 0 /* help macro for ATTRIBUTES test IF clause */
#define P00_UNFOLD(PREFIX, P00_TYPE, P00_MODE, ...) RL_PASTE4 (PREFIX, P00_TYPE, _, P00_MODE) (__VA_ARGS__)

#define P00_GET_FIRST_ATTRIBUTES(FIRST, ...) FIRST /* extract typedef attributes */
#define P00_GET_LAST_ATTRIBUTES(FIRST, ...) __VA_ARGS__ /* extract typedef comments and extended meta information */
/*
  Next macro produces typedef prolog, body and epilog.
  Prolog is RL_TYPEDEF_{STRUCT|UNION|ENUM}_{PROTO|DESC} (P00_TYPE_NAME, ATTR...)
  Body is a list of fields or enums definition. Structs and union fields are handled with P00_FIELD. Enums definitions are handled with P00_ENUM_DEF.
  Epilog is RL_END_{STRUCT|UNION|ENUM}_{PROTO|DESC} (P00_TYPE_NAME, COM...)
*/
#define RL_SER(NAME, I, REC, X) REC X

#define TYPEDEF_ATTR(P00_MODE, P00_TYPE, P00_TYPE_NAME, ATTR_COM_EXT, ...) \
  P00_UNFOLD (RL_TYPEDEF_, P00_TYPE, P00_MODE, P00_TYPE_NAME, RL_PASTE2 (P00_GET_FIRST_, ATTR_COM_EXT)) \
  RL_FOR ((P00_MODE, P00_TYPE_NAME), RL_NARG (__VA_ARGS__), RL_SER, RL_PASTE3 (P00_, P00_TYPE, _HANDLER), __VA_ARGS__) \
  P00_UNFOLD (RL_END_, P00_TYPE, P00_MODE, P00_TYPE_NAME, RL_PASTE2 (P00_GET_LAST_, ATTR_COM_EXT))

#define P00_STRUCT_HANDLER P00_FIELD
#define P00_UNION_HANDLER P00_FIELD
#define P00_ENUM_HANDLER P00_ENUM_DEF

/* field handler checks for trailing empty field */
#define P00_FIELD(P00_MODE_TYPE_NAME, FIELD, I) RL_IF_ELSE (RL_IS_EMPTY (FIELD)) () (P00_FIELD_ (P00_MODE_TYPE_NAME, FIELD))
/*
  field descritions might be in two forms.
  1. RL_TYPE_MACRO (ARGS) for type scpecific declarations like INT32 (x)
  2. (TYPE, NAME, SUFFIX..., COMMENT..., EXT...) for auto detection declarations.
*/
#define P00_FIELD_(P00_MODE_TYPE_NAME, FIELD)				\
  RL_IF_ELSE (RL_HAS_NO_PAREN (FIELD))					\
  (P00_FIELD_UNFOLD (P00_MODE_TYPE_NAME, FIELD))			\
  (P00_FIELD_DETECT (P00_MODE_TYPE_NAME, FIELD, RL_GET_SUFFIX FIELD))

#define RL_GET_SUFFIX(...) RL_GET_SUFFIX_ (__VA_ARGS__, ,)
#define RL_GET_SUFFIX_(_0, _1, _2, ...) _2
/*
  There are 3 options for auto-detection:
  a. Suffix with parentheses goes to FUNC as function.
  b. Non-empty suffix without parentheses goes to ARRAY as array.
  c. Everything else goes to AUTO.
*/
#define P00_FIELD_DETECT(P00_MODE_TYPE_NAME, FIELD, SUFFIX)	\
  RL_IF_ELSE (RL_HAS_NO_PAREN (SUFFIX))				\
  (RL_IF_ELSE (RL_IS_EMPTY (SUFFIX))				\
   (P00_FIELD_UNFOLD (P00_MODE_TYPE_NAME, AUTO FIELD))		\
   (P00_FIELD_UNFOLD (P00_MODE_TYPE_NAME, ARRAY FIELD)))	\
  (P00_FIELD_UNFOLD (P00_MODE_TYPE_NAME, FUNC FIELD))

#define P00_GET_MODE(P00_MODE, P00_TYPE_NAME) P00_MODE
#define P00_GET_TYPE_NAME(P00_MODE, P00_TYPE_NAME) P00_TYPE_NAME
/*
  Field type prefix should be extracted as separate macro argument. So we add prefix P00_COMMA_ and expect that in next macro field prefix will be substituted on comma delimitted RL_ type prefix.
  The 3rd macro unfolds to RL_{AUTO|INT32|...}_{PROTO|DESC} (P00_TYPE_NAME, ARGS...)
  Last one detects unkown field qualifiers.
*/
#define P00_FIELD_UNFOLD(P00_MODE_TYPE_NAME, FIELD) P00_FIELD_UNFOLD_ (P00_MODE_TYPE_NAME, FIELD, RL_PASTE2 (P00_COMMA_, FIELD))
#define P00_FIELD_UNFOLD_(P00_MODE_TYPE_NAME, FIELD, P00_FIELD_COMMA) P00_FIELD_UNFOLD__ (P00_MODE_TYPE_NAME, FIELD, P00_FIELD_COMMA)
#define P00_FIELD_UNFOLD__(P00_MODE_TYPE_NAME, FIELD, P00_FIELD_COMMA, ...) \
  RL_IF_ELSE (RL_IS_EMPTY (__VA_ARGS__))				\
  (P00_UNFOLD (RL_, UNKNOWN, P00_GET_MODE P00_MODE_TYPE_NAME, , FIELD))	\
  (P00_UNFOLD (RL_, P00_FIELD_COMMA, P00_GET_MODE P00_MODE_TYPE_NAME, P00_GET_TYPE_NAME P00_MODE_TYPE_NAME, RL_REMOVE_PAREN (__VA_ARGS__)))

/* produce compilation error for unkown field qualifiers */
#define RL_UNKNOWN_PROTO(P00_TYPE_NAME, ...) int _1[#__VA_ARGS__()];
#define RL_UNKNOWN_DESC(P00_TYPE_NAME, ...) { #__VA_ARGS__(), },

/*
  Check for empty trailing enum definition
  Enums definition might be in two forms.
  1. just a enum name
  2. (ENUM_NAME, RIGHT_HAND_SIDE..., COMMENT..., EXT...) enum and some extra information in parentheses
  both unfolds into RL_ENUM_DEF_{PROTO|DESC} (P00_TYPE_NAME, ARGS)
*/
#define P00_ENUM_DEF(P00_MODE_TYPE_NAME, FIELD, I)			\
  RL_IF_ELSE (RL_IS_EMPTY (FIELD))					\
  ()									\
  (P00_UNFOLD (RL_, ENUM_DEF, P00_GET_MODE P00_MODE_TYPE_NAME, P00_GET_TYPE_NAME P00_MODE_TYPE_NAME, RL_REMOVE_PAREN (FIELD)))

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
#define P00_COMMA_volatile AUTO_BI, volatile,
#define P00_COMMA___volatile AUTO_BI, __volatile,
#define P00_COMMA___volatile__ AUTO_BI, __volatile__,
#define P00_COMMA_const AUTO_BI, const,
#define P00_COMMA___const AUTO_BI, __const,
#define P00_COMMA___const__ AUTO_BI, __const__,

#define RL_AUTO_BI_PROTO(...) RL_AUTO_BI_0 (PROTO, __VA_ARGS__)
#define RL_AUTO_BI_DESC(...) RL_AUTO_BI_0 (DESC, __VA_ARGS__)

#define RL_AUTO_BI_0(P00_MODE, RL_TYPE_NAME, TYPE, NAME) RL_IF_ELSE (RL_HAS_COMMA (RL_PASTE2 (RL_IS_BUILTIN_, NAME))) (RL_AUTO_BI_1_ (P00_MODE, RL_TYPE_NAME, TYPE RL_PASTE2 (RL_IS_BUILTIN_, NAME))) (RL_PASTE2 (RL_AUTO_, P00_MODE) (RL_TYPE_NAME, TYPE, NAME))
#define RL_AUTO_BI_1_(...) RL_AUTO_BI_1 (__VA_ARGS__)
#define RL_AUTO_BI_1(P00_MODE, RL_TYPE_NAME, TYPE, NAME) RL_IF_ELSE (RL_HAS_COMMA (RL_PASTE2 (RL_IS_BUILTIN_, NAME))) (RL_AUTO_BI_2_ (P00_MODE, RL_TYPE_NAME, TYPE RL_PASTE2 (RL_IS_BUILTIN_, NAME))) (RL_PASTE2 (RL_AUTO_, P00_MODE) (RL_TYPE_NAME, TYPE, NAME))
#define RL_AUTO_BI_2_(...) RL_AUTO_BI_2 (__VA_ARGS__)
#define RL_AUTO_BI_2(P00_MODE, RL_TYPE_NAME, TYPE, NAME) RL_IF_ELSE (RL_HAS_COMMA (RL_PASTE2 (RL_IS_BUILTIN_, NAME))) (RL_AUTO_BI_3_ (P00_MODE, RL_TYPE_NAME, TYPE RL_PASTE2 (RL_IS_BUILTIN_, NAME))) (RL_PASTE2 (RL_AUTO_, P00_MODE) (RL_TYPE_NAME, TYPE, NAME))
#define RL_AUTO_BI_3_(...) RL_AUTO_BI_3 (__VA_ARGS__)
#define RL_AUTO_BI_3(P00_MODE, RL_TYPE_NAME, TYPE, NAME) RL_IF_ELSE (RL_HAS_COMMA (RL_PASTE2 (RL_IS_BUILTIN_, NAME))) (RL_AUTO_BI_4_ (P00_MODE, RL_TYPE_NAME, TYPE RL_PASTE2 (RL_IS_BUILTIN_, NAME))) (RL_PASTE2 (RL_AUTO_, P00_MODE) (RL_TYPE_NAME, TYPE, NAME))
#define RL_AUTO_BI_4_(...) RL_AUTO_BI_4 (__VA_ARGS__)
#define RL_AUTO_BI_4(P00_MODE, RL_TYPE_NAME, TYPE, NAME) RL_IF_ELSE (RL_HAS_COMMA (RL_PASTE2 (RL_IS_BUILTIN_, NAME))) (RL_AUTO_BI_5_ (P00_MODE, RL_TYPE_NAME, TYPE RL_PASTE2 (RL_IS_BUILTIN_, NAME))) (RL_PASTE2 (RL_AUTO_, P00_MODE) (RL_TYPE_NAME, TYPE, NAME))
#define RL_AUTO_BI_5_(...) RL_AUTO_BI_5 (__VA_ARGS__)
#define RL_AUTO_BI_5(P00_MODE, RL_TYPE_NAME, TYPE, NAME) RL_PASTE2 (RL_AUTO_, P00_MODE) (RL_TYPE_NAME, TYPE, NAME)

#define RL_IS_BUILTIN_char char,
#define RL_IS_BUILTIN_short short,
#define RL_IS_BUILTIN_int int,
#define RL_IS_BUILTIN_unsigned unsigned,
#define RL_IS_BUILTIN_signed signed,
#define RL_IS_BUILTIN_long long,
#define RL_IS_BUILTIN_float float,
#define RL_IS_BUILTIN_double double,
#define RL_IS_BUILTIN_int8_t int8_t,
#define RL_IS_BUILTIN_uint8_t uint8_t,
#define RL_IS_BUILTIN_int16_t int16_t,
#define RL_IS_BUILTIN_uint16_t uint16_t,
#define RL_IS_BUILTIN_int32_t int32_t,
#define RL_IS_BUILTIN_uint32_t uint32_t,
#define RL_IS_BUILTIN_int64_t int64_t,
#define RL_IS_BUILTIN_uint64_t uint64_t,
#define RL_IS_BUILTIN_long_double_t long_double_t,
#define RL_IS_BUILTIN_string_t string_t,
#define RL_IS_BUILTIN_volatile volatile,
#define RL_IS_BUILTIN___volatile __volatile,
#define RL_IS_BUILTIN___volatile__ __volatile__,
#define RL_IS_BUILTIN_const const,
#define RL_IS_BUILTIN___const __const,
#define RL_IS_BUILTIN___const__ __const__,

/* NB! for p99 mode only one anonymous union in struct is possible and it has default name */
#define P00_COMMA_ANON_UNION ANON_UNION_DEF,
#define P00_COMMA_NAMED_ANON_UNION NAMED_ANON_UNION,
#define P00_COMMA_END_ANON_UNION END_ANON_UNION,

#define RL_COMPILETIME_ASSERT(X) (int)sizeof (struct { int:-!!(X); });
/*
  For types defined using standard language approach you will need to create analog types with metaresc.
  For double checking of types costincency you will need the following macro. It compares size and offset of fields in two types.
  Usage: RL_COMPARE_COMPAUND_TYPES (system_type, metaresc_type, commonly_named_field, (field_for_system_type, field_for_metaresc_type), ...)
  Macro evaluates to 0 at compile time if all fields are compatible. Otherwise it is non-zero.
*/
#define RL_BOR(NAME, I, REC, X) ((REC) | (X))
#define RL_COMPARE_COMPAUND_TYPES(TYPE1, TYPE2, ...) ((sizeof (TYPE1) != sizeof (TYPE2)) | RL_FOR ((TYPE1, TYPE2), RL_NARG (__VA_ARGS__), RL_BOR, P00_COMPARE_FIELDS, __VA_ARGS__))
#define P00_GET_FIRST(_1, _2) _1
#define P00_GET_SECOND(_1, _2) _2
#define P00_COMPARE_FIELDS(TYPE1_TYPE2, NAME, I) RL_IF_ELSE (RL_HAS_NO_PAREN (NAME)) (RL_COMPARE_FIELDS (P00_GET_FIRST TYPE1_TYPE2, NAME, P00_GET_SECOND TYPE1_TYPE2, NAME)) (RL_COMPARE_FIELDS (P00_GET_FIRST TYPE1_TYPE2, P00_GET_FIRST NAME, P00_GET_SECOND TYPE1_TYPE2, P00_GET_SECOND NAME))

#ifndef RL_COMPARE_FIELDS_EXT
#define RL_COMPARE_FIELDS_EXT(...) 0
/*
  if your types contains only builtin types then you can do more precies comparation.
  #undef RL_COMPARE_FIELDS_EXT
  #define RL_COMPARE_FIELDS_EXT(TYPE1, NAME1, TYPE2, NAME2) !__builtin_types_compatible_p (typeof (((TYPE1*)NULL)->NAME1), typeof (((TYPE2*)NULL)->NAME2))
*/
#endif /* RL_COMPARE_FIELDS_EXT */
#define RL_COMPARE_FIELDS(TYPE1, NAME1, TYPE2, NAME2) (offsetof (TYPE1, NAME1) != offsetof (TYPE2, NAME2)) | (sizeof (((TYPE1*)NULL)->NAME1) != sizeof (((TYPE2*)NULL)->NAME2)) | RL_COMPARE_FIELDS_EXT (TYPE1, NAME1, TYPE2, NAME2)

#define RL_UNFOLD(NODE, ...) RL_PASTE3 (NODE, _, RL_MODE) (RL_TYPE_NAME, __VA_ARGS__)

#define RL_TYPEDEF_STRUCT(...) RL_UNFOLD (RL_TYPEDEF_STRUCT, __VA_ARGS__)

#define RL_AUTO(...) RL_UNFOLD (RL_AUTO, __VA_ARGS__)
#define RL_NONE(...) RL_UNFOLD (RL_NONE, __VA_ARGS__)
#define RL_ENUM(...) RL_UNFOLD (RL_ENUM, __VA_ARGS__)
#define RL_BITFIELD(...) RL_UNFOLD (RL_BITFIELD, __VA_ARGS__)
#define RL_BITMASK(...) RL_UNFOLD (RL_BITMASK, __VA_ARGS__)
#define RL_INT8(...) RL_UNFOLD (RL_INT8, __VA_ARGS__)
#define RL_UINT8(...) RL_UNFOLD (RL_UINT8, __VA_ARGS__)
#define RL_INT16(...) RL_UNFOLD (RL_INT16, __VA_ARGS__)
#define RL_UINT16(...) RL_UNFOLD (RL_UINT16, __VA_ARGS__)
#define RL_INT32(...) RL_UNFOLD (RL_INT32, __VA_ARGS__)
#define RL_UINT32(...) RL_UNFOLD (RL_UINT32, __VA_ARGS__)
#define RL_INT64(...) RL_UNFOLD (RL_INT64, __VA_ARGS__)
#define RL_UINT64(...) RL_UNFOLD (RL_UINT64, __VA_ARGS__)
#define RL_FLOAT(...) RL_UNFOLD (RL_FLOAT, __VA_ARGS__)
#define RL_DOUBLE(...) RL_UNFOLD (RL_DOUBLE, __VA_ARGS__)
#define RL_LONG_DOUBLE(...) RL_UNFOLD (RL_LONG_DOUBLE, __VA_ARGS__)
#define RL_CHAR(...) RL_UNFOLD (RL_CHAR, __VA_ARGS__)
#define RL_STRING(...) RL_UNFOLD (RL_STRING, __VA_ARGS__)
#define RL_CHAR_ARRAY(...) RL_UNFOLD (RL_CHAR_ARRAY, __VA_ARGS__)
#define RL_STRUCT(...) RL_UNFOLD (RL_STRUCT, __VA_ARGS__)
#define RL_UNION(...) RL_UNFOLD (RL_UNION, __VA_ARGS__)
#define RL_ARRAY(...) RL_UNFOLD (RL_ARRAY, __VA_ARGS__)
#define RL_POINTER(...) RL_UNFOLD (RL_POINTER, __VA_ARGS__)
#define RL_POINTER_STRUCT(...) RL_UNFOLD (RL_POINTER_STRUCT, __VA_ARGS__)
#define RL_RARRAY(...) RL_UNFOLD (RL_RARRAY, __VA_ARGS__)
#define RL_FUNC(...) RL_UNFOLD (RL_FUNC, __VA_ARGS__)
#define RL_END_STRUCT(...) RL_UNFOLD (RL_END_STRUCT, __VA_ARGS__)

#define RL_TYPEDEF_UNION(...) RL_UNFOLD (RL_TYPEDEF_UNION, __VA_ARGS__)
#define RL_END_UNION(...) RL_UNFOLD (RL_END_UNION, __VA_ARGS__)
#define RL_ANON_UNION(...) RL_UNFOLD (RL_ANON_UNION, __VA_ARGS__)
#define RL_ANON_UNION_DEF(...) RL_UNFOLD (RL_ANON_UNION_DEF, __VA_ARGS__)
#define RL_NAMED_ANON_UNION(...) RL_UNFOLD (RL_NAMED_ANON_UNION, __VA_ARGS__)
#define RL_END_ANON_UNION(...) RL_UNFOLD (RL_END_ANON_UNION, __VA_ARGS__)

#define RL_TYPEDEF_ENUM(...) RL_UNFOLD (RL_TYPEDEF_ENUM, __VA_ARGS__)
#define RL_ENUM_DEF(...) RL_UNFOLD (RL_ENUM_DEF, __VA_ARGS__)
#define RL_END_ENUM(...) RL_UNFOLD (RL_END_ENUM, __VA_ARGS__)

#define RL_TYPEDEF_CHAR_ARRAY(...) RL_UNFOLD (RL_TYPEDEF_CHAR_ARRAY, __VA_ARGS__)
#define RL_TYPEDEF_FUNC(...) RL_UNFOLD (RL_TYPEDEF_FUNC, __VA_ARGS__)

/* Macroses for prototypes generation mode */
#define RL_TYPEDEF_STRUCT_PROTO(RL_TYPE_NAME, /* ATTR */ ...) typedef struct RL_TYPEDEF_PREFIX (RL_TYPE_NAME) RL_TYPE_NAME; struct __VA_ARGS__ RL_TYPEDEF_PREFIX (RL_TYPE_NAME) {

/* next macro adds empty argument. Required for RL_AUTO, RL_NONE, RL_CHAR_ARRAY with two parameters. It adds 3rd parameter (suffix) for them. */
#define RL_AUTO_PROTO(...) RL_FIELD_PROTO (__VA_ARGS__, )
#define RL_NONE_PROTO(...) RL_FIELD_PROTO (__VA_ARGS__, )
#define RL_CHAR_ARRAY_PROTO(...) RL_FIELD_PROTO (__VA_ARGS__, )

#define RL_FIELD_PROTO(RL_TYPE_NAME, TYPE, NAME, SUFFIX, ...) TYPE NAME SUFFIX;
#define RL_ENUM_PROTO(RL_TYPE_NAME, TYPE, NAME, ...) RL_FIELD_PROTO (RL_TYPE_NAME, TYPE, NAME, )
#define RL_BITFIELD_PROTO(RL_TYPE_NAME, TYPE, NAME, SUFFIX, ...) RL_FIELD_PROTO (RL_TYPE_NAME, TYPE, NAME, : SUFFIX)
#define RL_BITMASK_PROTO(RL_TYPE_NAME, TYPE, NAME, ...) RL_FIELD_PROTO (RL_TYPE_NAME, TYPE, NAME, )
#define RL_INT8_PROTO(RL_TYPE_NAME, NAME, ...) RL_FIELD_PROTO (RL_TYPE_NAME, int8_t, NAME, )
#define RL_UINT8_PROTO(RL_TYPE_NAME, NAME, ...) RL_FIELD_PROTO (RL_TYPE_NAME, uint8_t, NAME, )
#define RL_INT16_PROTO(RL_TYPE_NAME, NAME, ...) RL_FIELD_PROTO (RL_TYPE_NAME, int16_t, NAME, )
#define RL_UINT16_PROTO(RL_TYPE_NAME, NAME, ...) RL_FIELD_PROTO (RL_TYPE_NAME, uint16_t, NAME, )
#define RL_INT32_PROTO(RL_TYPE_NAME, NAME, ...) RL_FIELD_PROTO (RL_TYPE_NAME, int32_t, NAME, )
#define RL_UINT32_PROTO(RL_TYPE_NAME, NAME, ...) RL_FIELD_PROTO (RL_TYPE_NAME, uint32_t, NAME, )
#define RL_INT64_PROTO(RL_TYPE_NAME, NAME, ...) RL_FIELD_PROTO (RL_TYPE_NAME, int64_t, NAME, )
#define RL_UINT64_PROTO(RL_TYPE_NAME, NAME, ...) RL_FIELD_PROTO (RL_TYPE_NAME, uint64_t, NAME, )
#define RL_FLOAT_PROTO(RL_TYPE_NAME, NAME, ...) RL_FIELD_PROTO (RL_TYPE_NAME, float, NAME, )
#define RL_DOUBLE_PROTO(RL_TYPE_NAME, NAME, ...) RL_FIELD_PROTO (RL_TYPE_NAME, double, NAME, )
#define RL_LONG_DOUBLE_PROTO(RL_TYPE_NAME, NAME, ...) RL_FIELD_PROTO (RL_TYPE_NAME, long double, NAME, )
#define RL_CHAR_PROTO(RL_TYPE_NAME, NAME, ...) RL_FIELD_PROTO (RL_TYPE_NAME, char, NAME, )
#define RL_STRING_PROTO(RL_TYPE_NAME, NAME, ...) RL_FIELD_PROTO (RL_TYPE_NAME, char *, NAME, )
#define RL_STRUCT_PROTO(RL_TYPE_NAME, TYPE, NAME, ...) RL_FIELD_PROTO (RL_TYPE_NAME, TYPE, NAME, )
#define RL_UNION_PROTO(RL_TYPE_NAME, TYPE, NAME, ...) RL_FIELD_PROTO (RL_TYPE_NAME, TYPE, NAME, )
#define RL_ARRAY_PROTO RL_FIELD_PROTO
#define RL_POINTER_PROTO(RL_TYPE_NAME, TYPE, NAME, ...) RL_FIELD_PROTO (RL_TYPE_NAME, TYPE *, NAME, )
#define RL_POINTER_STRUCT_PROTO(RL_TYPE_NAME, TYPE, NAME, ...) RL_FIELD_PROTO (RL_TYPE_NAME, struct RL_TYPEDEF_PREFIX (TYPE) *, NAME, )
/* rarray defenition should be syncroonized with rl_rarray_t type definition */
#define RL_RARRAY_PROTO(RL_TYPE_NAME, TYPE, NAME, ...) RL_FIELD_PROTO (RL_TYPE_NAME, struct __attribute__((packed)) {TYPE * data; int32_t size; int32_t alloc_size; rl_ptr_t ext; char * ptr_type;}, NAME, )
#define RL_FUNC_PROTO(RL_TYPE_NAME, TYPE, NAME, ARGS, ...) RL_FIELD_PROTO (RL_TYPE_NAME, TYPE, (*NAME), ARGS)
#define RL_END_STRUCT_PROTO(RL_TYPE_NAME, ...) };

#define RL_TYPEDEF_UNION_PROTO(RL_TYPE_NAME, /* ATTR */ ...) typedef union __VA_ARGS__ RL_TYPEDEF_PREFIX (RL_TYPE_NAME) {
#define RL_END_UNION_PROTO(RL_TYPE_NAME, ...) } RL_TYPE_NAME;
#define RL_ANON_UNION_PROTO(RL_TYPE_NAME, /* ATTR */ ...) RL_ANON_UNION_PROTO_ (RL_TYPE_NAME, __LINE__, __VA_ARGS__)
#define RL_ANON_UNION_PROTO_(RL_TYPE_NAME, LINE, ATTR) RL_NAMED_ANON_UNION_PROTO (RL_TYPE_NAME, RL_ANON_UNION_NAME (LINE), ATTR)
#define RL_ANON_UNION_DEF_PROTO(RL_TYPE_NAME, /* ATTR */ ...) RL_ANON_UNION_PROTO_ (RL_TYPE_NAME, , __VA_ARGS__)
#define RL_NAMED_ANON_UNION_PROTO(RL_TYPE_NAME, NAME, /* ATTR */ ...) char NAME[0]; union __VA_ARGS__ {
#define RL_END_ANON_UNION_PROTO(RL_TYPE_NAME, ...) };

#define RL_TYPEDEF_ENUM_PROTO(RL_TYPE_NAME, /* ATTR */ ...) typedef enum __VA_ARGS__ RL_TYPEDEF_PREFIX (RL_TYPE_NAME) {
#define RL_ENUM_DEF_PROTO(RL_TYPE_NAME, NAME, ...) RL_ENUM_DEF_PROTO_ (RL_TYPE_NAME, NAME, __VA_ARGS__)
#define RL_ENUM_DEF_PROTO_(RL_TYPE_NAME, NAME, RHS, ...) NAME RHS,
#define RL_END_ENUM_PROTO(RL_TYPE_NAME, ...) } RL_TYPE_NAME;

#define RL_TYPEDEF_CHAR_ARRAY_PROTO(RL_TYPE_NAME, SIZE, ...) typedef char RL_TYPE_NAME[SIZE];
#define RL_TYPEDEF_FUNC_PROTO(RL_TYPE_NAME, RET_TYPE, ARGS, ...) typedef RET_TYPE (*RL_TYPE_NAME) ARGS;

/* Macroses for descriptors generation mode */

#define RL_TYPEDEF_STRUCT_DESC(RL_TYPE_NAME, /* ATTR */ ...) RL_TYPEDEF_DESC (RL_TYPE_NAME, RL_TYPE_STRUCT, __VA_ARGS__)

#define RL_FIELD_DESC(RL_TYPE_NAME, TYPE, NAME, SUFFIX, RL_TYPE, RL_TYPE_EXT, /* COMMENT */ ...)  { \
    .type = RL_STRINGIFY (TYPE),					\
      .name = #NAME,							\
      .size = sizeof (TYPE),						\
      .offset = offsetof (RL_TYPE_NAME, NAME),				\
      .rl_type = RL_TYPE,						\
      .rl_type_ext = RL_TYPE_EXT,					\
      .param =								\
      {									\
	.array_param = {						\
	  .count = sizeof (TYPE) == 0 ? 0 :				\
	  sizeof (((RL_TYPE_NAME*)NULL)->NAME) / sizeof (TYPE),		\
	  .row_count = 1,						\
	},								\
      },								\
      .comment = "" __VA_ARGS__,					\
	 },

#define RL_POINTER_STRUCT_DESC(RL_TYPE_NAME, TYPE, NAME, /* COMMENT */ ...)  { \
    .type = RL_STRINGIFY (TYPE),					\
      .name = #NAME,							\
      .offset = offsetof (RL_TYPE_NAME, NAME),				\
      .rl_type = RL_TYPE_STRUCT,					\
      .rl_type_ext = RL_TYPE_EXT_POINTER,				\
      .comment = "" __VA_ARGS__,					\
      },

#define RL_ARRAY_DESC(RL_TYPE_NAME, TYPE, NAME, SUFFIX, /* COMMENT */ ...)  { \
    .type = RL_STRINGIFY (TYPE),					\
      .name = #NAME,							\
      .size = sizeof (TYPE),						\
      .offset = offsetof (RL_TYPE_NAME, NAME),				\
      .rl_type = RL_TYPE_DETECT (TYPE),					\
      .rl_type_ext = RL_TYPE_EXT_ARRAY,					\
      .param =								\
      {									\
	.array_param = {						\
	  .count = sizeof (TYPE) == 0 ? 0 :				\
	  sizeof (((RL_TYPE_NAME*)NULL)->NAME) / sizeof (TYPE),		\
	  .row_count = sizeof (TYPE) == 0 ? 0 :				\
	  sizeof (((RL_TYPE_NAME*)NULL)->NAME[0]) / sizeof (TYPE),	\
	},								\
      },								\
      .comment = "" __VA_ARGS__,					\
	 },

#define RL_NONE_DESC_(RL_TYPE_NAME, TYPE, NAME, SUFFIX, /* COMMENT */ ...)  { \
    .type = RL_STRINGIFY (TYPE),					\
      .name = RL_STRINGIFY (NAME),					\
      .size = sizeof (TYPE),						\
      .rl_type = RL_TYPE_VOID,						\
      .rl_type_ext = RL_TYPE_EXT_NONE,					\
      .comment = "" __VA_ARGS__,					\
      },

#define RL_BITFIELD_DESC(RL_TYPE_NAME, TYPE, NAME, SUFFIX, /* COMMENT */ ...) { \
    .type = RL_STRINGIFY (TYPE),					\
      .name = RL_STRINGIFY (NAME),					\
      .size = sizeof (TYPE),						\
      .rl_type = RL_TYPE_BITFIELD,					\
      .rl_type_aux = RL_TYPE_DETECT (TYPE),				\
      .rl_type_ext = RL_TYPE_EXT_NONE,					\
      .param = {							\
      .bitfield_param = {						\
	.width = SUFFIX,						\
	.bitfield = {							\
	  .size = sizeof (RL_TYPE_NAME),				\
	  .alloc_size = -1,						\
	  .data = (uint8_t*)((RL_TYPE_NAME[]){ { .NAME = -1 } }), },	\
      }, },								\
      .comment = "" __VA_ARGS__,					\
	 },

#define RL_AUTO_DESC_(RL_TYPE_NAME, TYPE, NAME, SUFFIX, /* COMMENTS */ ...) RL_FIELD_DESC (RL_TYPE_NAME, TYPE, NAME, SUFFIX, RL_TYPE_DETECT (TYPE), RL_TYPE_EXT_DETECT (TYPE, ((RL_TYPE_NAME*)NULL)->NAME), __VA_ARGS__, .rl_type_aux = RL_TYPE_DETECT_PTR (TYPE))

#define RL_AUTO_DESC(RL_TYPE_NAME, TYPE, NAME, ...) RL_AUTO_DESC_ (RL_TYPE_NAME, TYPE, NAME, __VA_ARGS__)
#define RL_NONE_DESC(RL_TYPE_NAME, TYPE, NAME, ...) RL_NONE_DESC_ (RL_TYPE_NAME, TYPE, NAME, __VA_ARGS__)
#define RL_CHAR_ARRAY_DESC(RL_TYPE_NAME, TYPE, NAME, ...) RL_CHAR_ARRAY_DESC_ (RL_TYPE_NAME, TYPE, NAME, __VA_ARGS__)

#define RL_ENUM_DESC(RL_TYPE_NAME, TYPE, NAME, /* COMMENTS */ ...) RL_FIELD_DESC (RL_TYPE_NAME, TYPE, NAME, , RL_TYPE_ENUM, RL_TYPE_EXT_NONE, __VA_ARGS__)
#define RL_BITMASK_DESC(RL_TYPE_NAME, TYPE, NAME, /* COMMENTS */ ...) RL_FIELD_DESC (RL_TYPE_NAME, TYPE, NAME, , RL_TYPE_BITMASK, RL_TYPE_EXT_NONE, __VA_ARGS__)
#define RL_INT8_DESC(RL_TYPE_NAME, NAME, /* COMMENTS */ ...) RL_FIELD_DESC (RL_TYPE_NAME, int8_t, NAME, , RL_TYPE_INT8, RL_TYPE_EXT_NONE, __VA_ARGS__)
#define RL_UINT8_DESC(RL_TYPE_NAME, NAME, /* COMMENTS */ ...) RL_FIELD_DESC (RL_TYPE_NAME, uint8_t, NAME, , RL_TYPE_UINT8, RL_TYPE_EXT_NONE, __VA_ARGS__)
#define RL_INT16_DESC(RL_TYPE_NAME, NAME, /* COMMENTS */ ...) RL_FIELD_DESC (RL_TYPE_NAME, int16_t, NAME, , RL_TYPE_INT16, RL_TYPE_EXT_NONE, __VA_ARGS__)
#define RL_UINT16_DESC(RL_TYPE_NAME, NAME, /* COMMENTS */ ...) RL_FIELD_DESC (RL_TYPE_NAME, uint16_t, NAME, , RL_TYPE_UINT16, RL_TYPE_EXT_NONE, __VA_ARGS__)
#define RL_INT32_DESC(RL_TYPE_NAME, NAME, /* COMMENTS */ ...) RL_FIELD_DESC (RL_TYPE_NAME, int32_t, NAME, , RL_TYPE_INT32, RL_TYPE_EXT_NONE, __VA_ARGS__)
#define RL_UINT32_DESC(RL_TYPE_NAME, NAME, /* COMMENTS */ ...) RL_FIELD_DESC (RL_TYPE_NAME, uint32_t, NAME, , RL_TYPE_UINT32, RL_TYPE_EXT_NONE, __VA_ARGS__)
#define RL_INT64_DESC(RL_TYPE_NAME, NAME, /* COMMENTS */ ...) RL_FIELD_DESC (RL_TYPE_NAME, int64_t, NAME, , RL_TYPE_INT64, RL_TYPE_EXT_NONE, __VA_ARGS__)
#define RL_UINT64_DESC(RL_TYPE_NAME, NAME, /* COMMENTS */ ...) RL_FIELD_DESC (RL_TYPE_NAME, uint64_t, NAME, , RL_TYPE_UINT64, RL_TYPE_EXT_NONE, __VA_ARGS__)
#define RL_FLOAT_DESC(RL_TYPE_NAME, NAME, /* COMMENTS */ ...) RL_FIELD_DESC (RL_TYPE_NAME, float, NAME, , RL_TYPE_FLOAT, RL_TYPE_EXT_NONE, __VA_ARGS__)
#define RL_DOUBLE_DESC(RL_TYPE_NAME, NAME, /* COMMENTS */ ...) RL_FIELD_DESC (RL_TYPE_NAME, double, NAME, , RL_TYPE_DOUBLE, RL_TYPE_EXT_NONE, __VA_ARGS__)
#define RL_LONG_DOUBLE_DESC(RL_TYPE_NAME, NAME, /* COMMENTS */ ...) RL_FIELD_DESC (RL_TYPE_NAME, long double, NAME, , RL_TYPE_LONG_DOUBLE, RL_TYPE_EXT_NONE, __VA_ARGS__)
#define RL_CHAR_DESC(RL_TYPE_NAME, NAME, /* COMMENTS */ ...) RL_FIELD_DESC (RL_TYPE_NAME, char, NAME, , RL_TYPE_CHAR, RL_TYPE_EXT_NONE, __VA_ARGS__)
#define RL_STRING_DESC(RL_TYPE_NAME, NAME, /* COMMENTS */ ...) RL_FIELD_DESC (RL_TYPE_NAME, char *, NAME, , RL_TYPE_STRING, RL_TYPE_EXT_NONE, __VA_ARGS__)
#define RL_CHAR_ARRAY_DESC_(RL_TYPE_NAME, TYPE, NAME, SUFFIX, /* COMMENTS */ ...) RL_FIELD_DESC (RL_TYPE_NAME, TYPE, NAME, SUFFIX, RL_TYPE_CHAR_ARRAY, RL_TYPE_EXT_NONE, __VA_ARGS__)
#define RL_STRUCT_DESC(RL_TYPE_NAME, TYPE, NAME, /* COMMENTS */ ...) RL_FIELD_DESC (RL_TYPE_NAME, TYPE, NAME, , RL_TYPE_STRUCT, RL_TYPE_EXT_NONE, __VA_ARGS__)
#define RL_UNION_DESC(RL_TYPE_NAME, TYPE, NAME, /* COMMENTS */ ...) RL_FIELD_DESC (RL_TYPE_NAME, TYPE, NAME, , RL_TYPE_UNION, RL_TYPE_EXT_NONE, __VA_ARGS__)
#define RL_POINTER_DESC(RL_TYPE_NAME, TYPE, NAME, /* COMMENTS */ ...) RL_FIELD_DESC (RL_TYPE_NAME, TYPE, NAME, , RL_TYPE_DETECT (TYPE), RL_TYPE_EXT_POINTER, __VA_ARGS__)
#define RL_RARRAY_DESC(RL_TYPE_NAME, TYPE, NAME, /* COMMENTS */ ...) RL_FIELD_DESC (RL_TYPE_NAME, TYPE, NAME, , RL_TYPE_DETECT (TYPE), RL_TYPE_EXT_RARRAY, __VA_ARGS__)
#define RL_FUNC_DESC(RL_TYPE_NAME, TYPE, NAME, ARGS, /* COMMENTS */ ...) RL_FIELD_DESC (RL_TYPE_NAME, TYPE, NAME, , RL_TYPE_FUNC, RL_TYPE_EXT_NONE, __VA_ARGS__, .param = { .func_param = { .alloc_size = -1, .size = 0, .data = (rl_fd_t []){ RL_FUNC_ARG (TYPE, "return value") RL_FOREACH (RL_FUNC_ARG, RL_REMOVE_PAREN (ARGS)) { .rl_type = RL_TYPE_TRAILING_RECORD, }, }, }, })
#define RL_FUNC_ARG(TYPE, /* COMMENTS */ ...) { .type = RL_STRINGIFY (TYPE), .size = sizeof (TYPE), .rl_type = RL_TYPE_DETECT (TYPE), .rl_type_aux = RL_TYPE_DETECT_PTR (TYPE), .rl_type_ext = RL_TYPE_EXT_NONE, .comment = "" __VA_ARGS__, },
#define RL_END_STRUCT_DESC(RL_TYPE_NAME, /* COMMENTS */ ...) RL_TYPEDEF_END_DESC (RL_TYPE_NAME, __VA_ARGS__)

#define RL_TYPEDEF_UNION_DESC(RL_TYPE_NAME, /* ATTR */ ...) RL_TYPEDEF_DESC (RL_TYPE_NAME, RL_TYPE_UNION, __VA_ARGS__)
#define RL_END_UNION_DESC(RL_TYPE_NAME, /* COMMENTS */ ...) RL_TYPEDEF_END_DESC (RL_TYPE_NAME, __VA_ARGS__)
#define RL_ANON_UNION_DESC(RL_TYPE_NAME, /* ATTR */ ...) RL_ANON_UNION_DESC_ (RL_TYPE_NAME, __LINE__, __VA_ARGS__)
#define RL_ANON_UNION_DESC_(RL_TYPE_NAME, LINE, ATTR) RL_ANON_UNION_DESC__ (RL_TYPE_NAME, RL_ANON_UNION_NAME (LINE), ATTR)
#define RL_ANON_UNION_DESC__(RL_TYPE_NAME, NAME, ATTR) RL_NAMED_ANON_UNION_DESC (RL_TYPE_NAME, NAME, ATTR)
#define RL_ANON_UNION_DEF_DESC(RL_TYPE_NAME, /* ATTR */ ...) RL_ANON_UNION_DESC_ (RL_TYPE_NAME, , __VA_ARGS__)

#define RL_NAMED_ANON_UNION_DESC(RL_TYPE_NAME, NAME, /* ATTR */ ...) {	\
    .type = "",								\
      .name = #NAME,							\
      .offset = offsetof (RL_TYPE_NAME, NAME),				\
      .rl_type = RL_TYPE_ANON_UNION,					\
      .rl_type_ext = RL_TYPE_EXT_NONE,					\
      .comment = #__VA_ARGS__,						\
      .ext = {(rl_td_t[]){ { .type = (char []) {RL_TYPE_ANONYMOUS_UNION_TEMPLATE "9999"}, } }}, \
      .ptr_type = "rl_td_t",						\
	 },
#define RL_END_ANON_UNION_DESC(RL_TYPE_NAME, /* COMMENTS */ ...) {	\
    .type = "",								\
      .rl_type = RL_TYPE_END_ANON_UNION,				\
      .rl_type_ext = RL_TYPE_EXT_NONE,					\
      .comment = "" __VA_ARGS__,					\
      },

#define RL_TYPEDEF_ENUM_DESC(RL_TYPE_NAME, /* ATTR */ ...) RL_TYPEDEF_DESC (RL_TYPE_NAME, RL_TYPE_ENUM, __VA_ARGS__)
#define RL_ENUM_DEF_DESC(RL_TYPE_NAME, NAME, ...) RL_ENUM_DEF_DESC_(RL_TYPE_NAME, NAME, __VA_ARGS__)
#define RL_ENUM_DEF_DESC_(RL_TYPE_NAME, NAME, RHS, /* COMMENTS */ ...) { \
    .type = RL_STRINGIFY (RL_TYPE_NAME),				\
      .name = #NAME,							\
      .rl_type = RL_TYPE_ENUM_VALUE,					\
      .rl_type_ext = RL_TYPE_EXT_NONE,					\
      .param =								\
      {									\
	.enum_value = NAME,						\
      },								\
      .comment = "" __VA_ARGS__,					\
	 },
#define RL_END_ENUM_DESC(RL_TYPE_NAME, /* COMMENTS */ ...) RL_TYPEDEF_END_DESC (RL_TYPE_NAME, __VA_ARGS__)

#define RL_TYPEDEF_CHAR_ARRAY_DESC(RL_TYPE_NAME, SIZE, /* COMMENTS */ ...) RL_TYPEDEF_DESC (RL_TYPE_NAME, RL_TYPE_CHAR_ARRAY) RL_TYPEDEF_END_DESC (RL_TYPE_NAME, __VA_ARGS__)
#define RL_TYPEDEF_FUNC_DESC(RL_TYPE_NAME, RET_TYPE, ARGS, /* COMMENTS */ ...) \
  RL_TYPEDEF_DESC (RL_TYPE_NAME, RL_TYPE_FUNC_TYPE)			\
  RL_FUNC_ARG (RET_TYPE, "return value")				\
  RL_FOREACH (RL_FUNC_ARG, RL_REMOVE_PAREN (ARGS))			\
  RL_TYPEDEF_END_DESC (RL_TYPE_NAME, __VA_ARGS__)

#define RL_TYPEDEF_DESC(RL_TYPE_NAME, RL_TYPE, /* ATTR */ ...)		\
  RL_DESCRIPTOR_ATTR rl_td_t RL_DESCRIPTOR_PREFIX (RL_TYPE_NAME) = {	\
    .rl_type = RL_TYPE,							\
    .type = #RL_TYPE_NAME,						\
    .size = sizeof (RL_TYPE_NAME),					\
    .attr = #__VA_ARGS__,						\
    .fields = { .alloc_size = -1, .size = 0, .data = (rl_fd_t []){
#define RL_TYPEDEF_END_DESC(RL_TYPE_NAME, /* COMMENTS */ ...)		\
  {.rl_type = RL_TYPE_TRAILING_RECORD} } },				\
    .comment = "" __VA_ARGS__ };					\
  static inline void __attribute__((constructor)) RL_CONSTRUCTOR_PREFIX (RL_TYPE_NAME) (void) { RL_ADD_TYPE (RL_TYPE_NAME); }

/*
  Macro for type registration.
  Usage:
  RL_ADD_TYPE(foo_t);
  RL_ADD_TYPE(foo_t, "Text Meta Info");
  RL_ADD_TYPE(foo_t, "TMI", "One more TMI");
  RL_ADD_TYPE(foo_t, "TMI", &meta_info_struct);
  RL_ADD_TYPE(foo_t, "TMI", &meta_info_struct, unused, unused, unused);
  By default comment and .ext void pointer could initialized in RL_END_STRUCT & RL_END_ENUM.
  Arguments in RL_ADD_TYPE will override those settings.
*/
#define RL_ADD_TYPE(RL_TYPE_NAME, /* COMMENTS */ ...) rl_add_type (&RL_DESCRIPTOR_PREFIX (RL_TYPE_NAME), "" __VA_ARGS__, NULL)

/*
  User can turn off strict types checking for ResLib macroses, so compilation will produce only warnings.
  RL_CHECK_TYPES macro should be predefined as
  #define RL_CHECK_TYPES(...)
*/
#ifndef RL_CHECK_TYPES
#define RL_CHECK_TYPES(RL_TYPE_NAME, ...) ({ (void) ((RL_TYPE_NAME*)0 - (typeof (__VA_ARGS__ + 0))0); })
#endif /* RL_CHECK_TYPES */

#define RL_FREE_RECURSIVELY(RL_TYPE_NAME, S_PTR) rl_free_recursively (RL_SAVE_RL (RL_TYPE_NAME, S_PTR))

#define RL_SAVE_RL(RL_TYPE_NAME, S_PTR) ({				\
      char * __name__ = RL_STRINGIFY (S_PTR);				\
      rl_fd_t __fd__ =							\
	{								\
	  .name = __name__,						\
	  .type = #RL_TYPE_NAME,					\
	  .rl_type = RL_TYPE_DETECT (RL_TYPE_NAME),			\
	  .rl_type_ext = RL_TYPE_EXT_DETECT (RL_TYPE_NAME, S_PTR),	\
	  .size = sizeof (RL_TYPE_NAME),				\
	  .param = 							\
	  {								\
	    .array_param = {						\
	      .count = sizeof (RL_TYPE_NAME) == 0 ? 0 :			\
	      sizeof (S_PTR) / sizeof (RL_TYPE_NAME),			\
	      .row_count = 1,						\
	    },								\
	  },								\
	};								\
      RL_TYPE_NAME * check_type = S_PTR;				\
      rl_save_data_t __rl_save_data__ =					\
	{								\
	  .parent = -1,							\
	  .typed_ptrs_tree = NULL,					\
	  .untyped_ptrs_tree = NULL,					\
	  .rl_ra_idx = { .data = NULL, .size = 0, .alloc_size = 0, },	\
	  .ptrs = { .ra = { .data = NULL, .size = 0, .alloc_size = 0, } } \
	};								\
      char * __ptr__ = strchr (__name__, '[');				\
      rl_td_t * __tdp__ = rl_get_td_by_name (__fd__.type);		\
      if (__tdp__)							\
	__fd__.rl_type = __tdp__->rl_type;				\
      if (NULL == __ptr__)						\
	__ptr__ = strchr (__name__, 0);					\
      --__ptr__;							\
      while ((__ptr__ >= __name__) && !(isalnum (*__ptr__) || ('_' == *__ptr__))) \
	--__ptr__;							\
      *(__ptr__ + 1) = 0;						\
      while ((__ptr__ >= __name__) && (isalnum (*__ptr__) || ('_' == *__ptr__))) \
	--__ptr__;							\
      __fd__.name = ++__ptr__;						\
      RL_CHECK_TYPES (RL_TYPE_NAME, S_PTR);				\
      if (check_type == NULL)						\
	RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_NULL_POINTER);		\
      else								\
	rl_save (check_type, &__fd__, &__rl_save_data__);		\
      __rl_save_data__.ptrs;						\
    })

#define RL_SAVE_XDR(RL_TYPE_NAME, XDRS, S_PTR) ({			\
      int __status__ = 0;						\
      XDR * __xdrs__ = (XDRS);						\
      if (XDR_ENCODE != __xdrs__->x_op)					\
	RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_XDR_WRONG_ENCODING_MODE);	\
      else								\
	{								\
	  rl_ra_rl_ptrdes_t __ptrs__ = RL_SAVE_RL (RL_TYPE_NAME, S_PTR); \
	  if (__ptrs__.ra.data)						\
	    {								\
	      __status__ = xdr_save (__xdrs__, &__ptrs__);		\
	      RL_FREE (__ptrs__.ra.data);				\
	    }								\
	}								\
      __status__;							\
    })

#define RL_SAVE_XDR_RA(RL_TYPE_NAME, S_PTR) ({				\
      XDR _xdrs_;							\
      rl_rarray_t _ra_ = { .alloc_size = 0, .size = 0, .data = NULL, }; \
      xdrra_create (&_xdrs_, &_ra_, XDR_ENCODE);			\
      if (0 == RL_SAVE_XDR (RL_TYPE_NAME, &_xdrs_, S_PTR))		\
	RL_MESSAGE (RL_LL_WARN, RL_MESSAGE_XDR_SAVE_FAILED);		\
      _ra_;								\
    })

#define RL_SAVE_METHOD(METHOD, RL_TYPE_NAME, S_PTR) ({			\
      rl_ra_rl_ptrdes_t __ptrs__ = RL_SAVE_RL (RL_TYPE_NAME, S_PTR);	\
      char * __str__ = NULL;						\
      if (__ptrs__.ra.data)						\
	{								\
	  __str__ = METHOD (&__ptrs__);					\
	  RL_FREE (__ptrs__.ra.data);					\
	}								\
      __str__;								\
    })

#define RL_SAVE_XML1(RL_TYPE_NAME, S_PTR) RL_SAVE_METHOD (xml1_save, RL_TYPE_NAME, S_PTR)
#define RL_SAVE_CINIT(RL_TYPE_NAME, S_PTR) RL_SAVE_METHOD (cinit_save, RL_TYPE_NAME, S_PTR)
#define RL_SAVE_JSON(RL_TYPE_NAME, S_PTR) RL_SAVE_METHOD (json_save, RL_TYPE_NAME, S_PTR)
#define RL_SAVE_SCM(RL_TYPE_NAME, S_PTR) RL_SAVE_METHOD (scm_save, RL_TYPE_NAME, S_PTR)

#define RL_SAVE_METHOD_RA(METHOD, RL_TYPE_NAME, S_PTR) ({		\
      rl_rarray_t _ra_ = { .alloc_size = 0, .size = 0, .data = NULL, }; \
      _ra_.data = METHOD (RL_TYPE_NAME, S_PTR);				\
      if (_ra_.data) _ra_.size = _ra_.alloc_size = strlen (_ra_.data) + 1; \
      _ra_;								\
    })

#define RL_SAVE_XML_RA(RL_TYPE_NAME, S_PTR) RL_SAVE_METHOD_RA (RL_SAVE_XML, RL_TYPE_NAME, S_PTR)
#define RL_SAVE_XML1_RA(RL_TYPE_NAME, S_PTR) RL_SAVE_METHOD_RA (RL_SAVE_XML1, RL_TYPE_NAME, S_PTR)
#define RL_SAVE_CINIT_RA(RL_TYPE_NAME, S_PTR) RL_SAVE_METHOD_RA (RL_SAVE_CINIT, RL_TYPE_NAME, S_PTR)
#define RL_SAVE_JSON_RA(RL_TYPE_NAME, S_PTR) RL_SAVE_METHOD_RA (RL_SAVE_JSON, RL_TYPE_NAME, S_PTR)
#define RL_SAVE_SCM_RA(RL_TYPE_NAME, S_PTR) RL_SAVE_METHOD_RA (RL_SAVE_SCM, RL_TYPE_NAME, S_PTR)
  
#define RL_LOAD_XDR_ARG3(RL_TYPE_NAME, XDRS, S_PTR) ({			\
      int __status__ = 0;						\
      XDR * __xdrs__ = (XDRS);						\
      if (XDR_DECODE != __xdrs__->x_op)					\
	RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_XDR_WRONG_ENCODING_MODE);	\
      else								\
	{								\
	  rl_fd_t __fd__ = {						\
	    .type = #RL_TYPE_NAME,					\
	    .name = NULL,						\
	    .rl_type = RL_TYPE_DETECT (RL_TYPE_NAME),			\
	    .rl_type_ext = RL_TYPE_EXT_NONE,				\
	    .size = sizeof (RL_TYPE_NAME),				\
	    .param = 							\
	    {								\
	      .array_param = {						\
		.count = sizeof (RL_TYPE_NAME) == 0 ? 0 :		\
		(sizeof S_PTR + 0) / sizeof (RL_TYPE_NAME),		\
		.row_count = 1,						\
	      },							\
	    },								\
	  };								\
	  RL_TYPE_NAME * __check_type__ = S_PTR + 0;			\
	  rl_td_t * __tdp__ = rl_get_td_by_name (__fd__.type);		\
	  if (__tdp__)							\
	    __fd__.rl_type = __tdp__->rl_type;				\
	  RL_CHECK_TYPES (RL_TYPE_NAME, S_PTR);				\
	  if (__check_type__ != NULL)					\
	    __status__ = xdr_load (__check_type__, &__fd__, __xdrs__, NULL); \
	  else								\
	    RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_NULL_POINTER);		\
	}								\
      __status__;							\
    })

#define RL_LOAD_XDR_ARG2_(RL_TYPE_NAME, XDRS) ({			\
      int _status_ = 0;							\
      RL_TYPE_NAME __result__;						\
      memset (&__result__, 0, sizeof (__result__));			\
      _status_ = RL_LOAD_XDR_ARG3 (RL_TYPE_NAME, XDRS, &__result__);	\
      if (0 == _status_)						\
	RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_XDR_LOAD_FAILED);		\
      __result__;							\
    })

#define RL_LOAD_XDR(RL_TYPE_NAME, ...) RL_LOAD_XDR_ARGN (RL_TYPE_NAME, __VA_ARGS__, 3, 2)
#define RL_LOAD_XDR_ARGN(RL_TYPE_NAME, XDRS, S_PTR, N, ...) RL_LOAD_XDR_ARG ## N (RL_TYPE_NAME, XDRS, S_PTR)
#define RL_LOAD_XDR_ARG2(RL_TYPE_NAME, XDRS, S_PTR) RL_LOAD_XDR_ARG2_ (RL_TYPE_NAME, XDRS)

#define RL_LOAD_XDR_RA(RL_TYPE_NAME, ...) RL_LOAD_XDR_RA_ARGN (RL_TYPE_NAME, __VA_ARGS__, 3, 2)
#define RL_LOAD_XDR_RA_ARGN(RL_TYPE_NAME, RA, S_PTR, N, ...) RL_LOAD_XDR_RA_ARG ## N (RL_TYPE_NAME, RA, S_PTR)
#define RL_LOAD_XDR_RA_ARG2(RL_TYPE_NAME, RA, S_PTR) ({ XDR _xdrs_; xdrra_create (&_xdrs_, RA, XDR_DECODE); RL_LOAD_XDR (RL_TYPE_NAME, &_xdrs_); })
#define RL_LOAD_XDR_RA_ARG3(RL_TYPE_NAME, RA, S_PTR) ({ XDR _xdrs_; xdrra_create (&_xdrs_, RA, XDR_DECODE); RL_LOAD_XDR (RL_TYPE_NAME, &_xdrs_, S_PTR); })

#ifdef HAVE_LIBXML2

#define RL_SAVE_XML2_NODE(RL_TYPE_NAME, S_PTR) ({			\
      xmlNodePtr __node__ = NULL;					\
      rl_ra_rl_ptrdes_t __ptrs__ = RL_SAVE_RL (RL_TYPE_NAME, S_PTR);	\
      if (__ptrs__.ra.data)						\
	{								\
	  __node__ = xml2_save (&__ptrs__);				\
	  RL_FREE (__ptrs__.ra.data);					\
	}								\
      __node__;								\
    })

#define RL_SAVE_XML2_RA(RL_TYPE_NAME, S_PTR) RL_SAVE_METHOD_RA (RL_SAVE_XML2, RL_TYPE_NAME, S_PTR)

#define RL_SAVE_XML2(RL_TYPE_NAME, S_PTR) ({				\
      int __size__;							\
      char * __str__ = NULL;						\
      xmlChar * __xml_str__;						\
      xmlDocPtr __doc__ = xmlNewDoc ((unsigned char*)"1.0");		\
      if (NULL == __doc__)						\
	RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_XML_SAVE_FAILED);		\
      else								\
	{								\
	  xmlNodePtr _node_ = RL_SAVE_XML2_NODE(RL_TYPE_NAME, S_PTR);	\
	  if (NULL == _node_)						\
	    RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_XML_SAVE_FAILED);	\
	  else								\
	    xmlDocSetRootElement (__doc__, _node_);			\
	  xmlDocDumpFormatMemory (__doc__, &__xml_str__, &__size__, 1);	\
	  xmlFreeDoc (__doc__);						\
	  if (__xml_str__)						\
	    {								\
	      __str__ = RL_STRDUP ((char*)__xml_str__);			\
	      xmlFree (__xml_str__);					\
	    }								\
	}								\
      __str__;								\
    })

#define RL_LOAD_XML2_NODE_ARG3(RL_TYPE_NAME, XML, S_PTR) ({		\
      int __status__ = 0;						\
      int __idx__ = -1;							\
      rl_load_data_t __load_data__ = {					\
	.ptrs = { .ra = { .alloc_size = 0, .size = 0, .data = NULL, } }, \
	.rl_ra_idx = { .alloc_size = 0, .size = 0, .data = NULL, }, };	\
      rl_fd_t __fd__ = {						\
	.type = #RL_TYPE_NAME,						\
	.name = NULL,							\
	.rl_type = RL_TYPE_DETECT (RL_TYPE_NAME),			\
	.rl_type_ext = RL_TYPE_EXT_NONE,				\
	.size = sizeof (RL_TYPE_NAME),					\
	.param = 							\
	{								\
	  .array_param = {						\
	    .count = sizeof (RL_TYPE_NAME) == 0 ? 0 :			\
	    (sizeof S_PTR + 0) / sizeof (RL_TYPE_NAME),			\
	    .row_count = 1,						\
	  },								\
	},								\
      };								\
      RL_TYPE_NAME * __check_type__ = S_PTR + 0;			\
      rl_td_t * __tdp__ = rl_get_td_by_name (__fd__.type);		\
      xmlNodePtr __xml__ = (XML);					\
      if (NULL == __xml__)						\
	RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_NULL_POINTER);		\
      else								\
	{								\
	  if (__tdp__)							\
	    __fd__.rl_type = __tdp__->rl_type;				\
	  RL_CHECK_TYPES (RL_TYPE_NAME, S_PTR);				\
	  if (NULL != __check_type__) 					\
	    __idx__ = xml2_load (__xml__, &__load_data__.ptrs);		\
	  else								\
	    RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_NULL_POINTER);		\
	  if (__idx__ >= 0)						\
	    __status__ = rl_load (__check_type__, &__fd__, __idx__, &__load_data__); \
	  rl_free_ptrs (__load_data__.ptrs);				\
	}								\
      __status__;							\
    })

#define RL_LOAD_XML2_NODE_ARG2_(RL_TYPE_NAME, XML) ({			\
      int _status_ = 0;							\
      RL_TYPE_NAME __result__;						\
      memset (&__result__, 0, sizeof (__result__));			\
      _status_ = RL_LOAD_XML2_NODE_ARG3 (RL_TYPE_NAME, XML, &__result__); \
      if (0 == _status_)						\
	RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_LOAD_STRUCT_FAILED);	\
      __result__;							\
    })

#define RL_LOAD_XML2_NODE(RL_TYPE_NAME, ...) RL_LOAD_XML2_NODE_ARGN (RL_TYPE_NAME, __VA_ARGS__, 3, 2)
#define RL_LOAD_XML2_NODE_ARGN(RL_TYPE_NAME, XML, S_PTR, N, ...) RL_LOAD_XML2_NODE_ARG ## N (RL_TYPE_NAME, XML, S_PTR)
#define RL_LOAD_XML2_NODE_ARG2(RL_TYPE_NAME, XML, S_PTR) RL_LOAD_XML2_NODE_ARG2_ (RL_TYPE_NAME, XML)

#define RL_LOAD_XML2_ARG3(RL_TYPE_NAME, STR, S_PTR) ({			\
      int _status_ = 0;							\
      RL_TYPE_NAME * _check_type_ = S_PTR + 0;				\
      char * __str__ = (STR);						\
      RL_CHECK_TYPES (RL_TYPE_NAME, S_PTR);				\
      if (NULL == _check_type_)						\
	RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_NULL_POINTER);		\
      else if (NULL == __str__)						\
	RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_STRING_IS_NULL);		\
      else								\
	{								\
	  xmlDocPtr __doc__ = xmlParseMemory (__str__, strlen (__str__)); \
	  if (__doc__)							\
	    {								\
	      xmlNodePtr __node__ = xmlDocGetRootElement (__doc__);	\
	      _status_ = RL_LOAD_XML2_NODE_ARG3 (RL_TYPE_NAME, __node__, _check_type_); \
	      xmlFreeDoc (__doc__);					\
	    }								\
	}								\
      _status_;								\
    })

#define RL_LOAD_XML2_ARG2_(RL_TYPE_NAME, STR) ({			\
      int ___status___ = 0;						\
      RL_TYPE_NAME __result__;						\
      memset (&__result__, 0, sizeof (__result__));			\
      ___status___ = RL_LOAD_XML2_ARG3 (RL_TYPE_NAME, STR, &__result__); \
      if (0 == ___status___)						\
	RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_LOAD_STRUCT_FAILED);	\
      __result__;							\
    })

#define RL_LOAD_XML2(RL_TYPE_NAME, ...) RL_LOAD_XML2_N (RL_TYPE_NAME, __VA_ARGS__, 3, 2)
#define RL_LOAD_XML2_N(RL_TYPE_NAME, STR, S_PTR, N, ...) RL_LOAD_XML2_ARG ## N (RL_TYPE_NAME, STR, S_PTR)
#define RL_LOAD_XML2_ARG2(RL_TYPE_NAME, STR, S_PTR) RL_LOAD_XML2_ARG2_ (RL_TYPE_NAME, STR)

#endif /* HAVE_LIBXML2 */

#define RL_LOAD_XML2_RA(RL_TYPE_NAME, ...) RL_LOAD_XML2_RA_ARGN (RL_TYPE_NAME, __VA_ARGS__, 3, 2)
#define RL_LOAD_XML2_RA_ARGN(RL_TYPE_NAME, RA, S_PTR, N, ...) RL_LOAD_XML2_ARG ## N (RL_TYPE_NAME, (char*)((RA)->data), S_PTR)

#ifdef HAVE_BISON_FLEX

#define RL_LOAD_METHOD_ARG3(METHOD, RL_TYPE_NAME, STR, S_PTR) ({	\
      int _status_ = 0;							\
      RL_TYPE_NAME * _check_type_ = S_PTR + 0;				\
      char * _str_ = (STR);						\
      RL_CHECK_TYPES (RL_TYPE_NAME, S_PTR);				\
      if (NULL == _check_type_)						\
	RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_NULL_POINTER);		\
      else if (NULL == _str_)						\
	RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_STRING_IS_NULL);		\
      else								\
	{								\
	  rl_load_data_t _load_data_ = {				\
	    .ptrs = { .ra = { .alloc_size = 0, .size = 0, .data = NULL, } }, \
	    .rl_ra_idx = { .alloc_size = 0, .size = 0, .data = NULL, }, }; \
	  _status_ = METHOD (_str_, &_load_data_.ptrs);			\
	  if (_status_)							\
	    {								\
	      rl_fd_t _fd_ = {						\
		.type = #RL_TYPE_NAME,					\
		.name = NULL,						\
		.rl_type = RL_TYPE_DETECT (RL_TYPE_NAME),		\
		.rl_type_ext = RL_TYPE_EXT_DETECT (RL_TYPE_NAME, S_PTR), \
		.size = sizeof (RL_TYPE_NAME),				\
		.param = 						\
		{							\
		  .array_param = {					\
		    .count = sizeof (RL_TYPE_NAME) == 0 ? 0 :		\
		    (sizeof S_PTR + 0) / sizeof (RL_TYPE_NAME),		\
		    .row_count = 1,					\
		  },							\
		},							\
	      };							\
	      rl_td_t * _tdp_ = rl_get_td_by_name (_fd_.type);		\
	      if (_tdp_)						\
		_fd_.rl_type = _tdp_->rl_type;				\
	      _status_ = rl_load (_check_type_, &_fd_, 0, &_load_data_); \
	    }								\
	  rl_free_ptrs (_load_data_.ptrs);				\
	}								\
      _status_;								\
    })

#define RL_LOAD_METHOD_ARG2_(METHOD, RL_TYPE_NAME, STR) ({		\
      int _status_ = 0;							\
      RL_TYPE_NAME __result__;						\
      memset (&__result__, 0, sizeof (__result__));			\
      _status_ = RL_LOAD_METHOD_ARG3 (METHOD, RL_TYPE_NAME, STR, &__result__); \
      if (0 == _status_)						\
	RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_LOAD_STRUCT_FAILED);	\
      __result__;							\
    })

#define RL_LOAD_METHOD(METHOD, RL_TYPE_NAME, ...) RL_LOAD_METHOD_ARGN (METHOD, RL_TYPE_NAME, __VA_ARGS__, 3, 2)
#define RL_LOAD_METHOD_ARGN(METHOD, RL_TYPE_NAME, STR, S_PTR, N, ...) RL_LOAD_METHOD_ARG ## N (METHOD, RL_TYPE_NAME, STR, S_PTR)
#define RL_LOAD_METHOD_ARG2(METHOD, RL_TYPE_NAME, STR, S_PTR) RL_LOAD_METHOD_ARG2_ (METHOD, RL_TYPE_NAME, STR)

#define RL_LOAD_XML1(RL_TYPE_NAME, /* STR */ ...) RL_LOAD_METHOD (xml1_load, RL_TYPE_NAME, __VA_ARGS__)
#define RL_LOAD_CINIT(RL_TYPE_NAME, /* STR */ ...) RL_LOAD_METHOD (cinit_load, RL_TYPE_NAME, __VA_ARGS__)
#define RL_LOAD_JSON RL_LOAD_CINIT
#define RL_LOAD_SCM(RL_TYPE_NAME, /* STR */ ...) RL_LOAD_METHOD (scm_load, RL_TYPE_NAME, __VA_ARGS__)

#define RL_LOAD_METHOD_RA(METHOD, RL_TYPE_NAME, ...) RL_LOAD_METHOD_RA_ARGN (METHOD, RL_TYPE_NAME, __VA_ARGS__, 3, 2)
#define RL_LOAD_METHOD_RA_ARGN(METHOD, RL_TYPE_NAME, RA, S_PTR, N, ...) RL_LOAD_METHOD_ARG ## N (METHOD, RL_TYPE_NAME, (char*)((RA)->data), S_PTR)

#define RL_LOAD_XML1_RA(RL_TYPE_NAME, /* RA */ ...) RL_LOAD_METHOD_RA (xml1_load, RL_TYPE_NAME, __VA_ARGS__)
#define RL_LOAD_CINIT_RA(RL_TYPE_NAME, /* RA */ ...) RL_LOAD_METHOD_RA (cinit_load, RL_TYPE_NAME, __VA_ARGS__)
#define RL_LOAD_JSON_RA RL_LOAD_CINIT_RA
#define RL_LOAD_SCM_RA(RL_TYPE_NAME, /* RA */ ...) RL_LOAD_METHOD_RA (scm_load, RL_TYPE_NAME, __VA_ARGS__)

#else /* ! HAVE_BISON_FLEX */

#define RL_ZERO_RESULT(RL_TYPE_NAME) ({				\
      RL_TYPE_NAME __result__;					\
      memset (&__result__, 0, sizeof (__result__));		\
      RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_LOAD_METHOD_MISSED);	\
      __result__;						\
    })

#define RL_LOAD_STUB(RL_TYPE_NAME, ...) RL_LOAD_STUB_ARGN (RL_TYPE_NAME, __VA_ARGS__, 3, 2)
#define RL_LOAD_STUB_ARGN(RL_TYPE_NAME, STR, S_PTR, N, ...) RL_LOAD_STUB_ARG ## N (RL_TYPE_NAME, STR, S_PTR)
#define RL_LOAD_STUB_ARG2(RL_TYPE_NAME, STR, S_PTR) (RL_ZERO_RESULT (RL_TYPE_NAME))
#define RL_LOAD_STUB_ARG3(RL_TYPE_NAME, STR, S_PTR) ({ RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_LOAD_METHOD_MISSED); 0;})

#define RL_LOAD_XML1 RL_LOAD_STUB
#define RL_LOAD_CINIT RL_LOAD_STUB
#define RL_LOAD_SCM RL_LOAD_STUB

#endif /* HAVE_BISON_FLEX */

#ifdef HAVE_LIBXML2
#define RL_LOAD_XML RL_LOAD_XML2
#define RL_LOAD_XML_RA RL_LOAD_XML2_RA
#define RL_SAVE_XML RL_SAVE_XML2

#else /* ! HAVE_LIBXML2 */

#define RL_SAVE_XML RL_SAVE_XML1

#ifdef HAVE_BISON_FLEX
#define RL_LOAD_XML RL_LOAD_XML1
#define RL_LOAD_XML_RA RL_LOAD_XML1_RA
#else /* ! HAVE_BISON_FLEX */
#warning There is no support for XML load.
#define RL_LOAD_XML RL_LOAD_STUB
#define RL_LOAD_XML_RA RL_LOAD_STUB
#endif /* HAVE_BISON_FLEX */

#endif /* HAVE_LIBXML2 */

#ifndef RL_MODE
#define RL_MODE_UNDEFINED
#define RL_MODE PROTO
#endif
#include <rlprotos.h>
#ifdef RL_MODE_UNDEFINED
#undef RL_MODE_UNDEFINED
#undef RL_MODE
#endif

typedef long double long_double_t;

extern rl_conf_t rl_conf;

extern int __attribute__ ((sentinel(0))) rl_add_type (rl_td_t*, char*, ...);
extern char * rl_read_xml_doc (FILE*);

extern void rl_save (void*, rl_fd_t*, rl_save_data_t*);
extern int rl_load (void*, rl_fd_t*, int, rl_load_data_t*);
#ifdef HAVE_LIBXML2
extern xmlNodePtr xml2_save (rl_ra_rl_ptrdes_t*);
extern int xml2_load (xmlNodePtr, rl_ra_rl_ptrdes_t*);
#endif /* HAVE_LIBXML2 */
extern int xdr_save (XDR*, rl_ra_rl_ptrdes_t*);
extern int xdr_load (void*, rl_fd_t*, XDR*, rl_ra_rl_ptrdes_t*);
extern void xdrra_create (XDR*, rl_rarray_t*, enum xdr_op);

extern char * xml1_save (rl_ra_rl_ptrdes_t*);
#ifdef HAVE_BISON_FLEX
extern int xml1_load (char*, rl_ra_rl_ptrdes_t*);
#endif /* HAVE_BISON_FLEX */
extern char * cinit_save (rl_ra_rl_ptrdes_t*);
#ifdef HAVE_BISON_FLEX
extern int cinit_load (char*, rl_ra_rl_ptrdes_t*);
#endif /* HAVE_BISON_FLEX */
extern char * json_save (rl_ra_rl_ptrdes_t*);

extern char * scm_save (rl_ra_rl_ptrdes_t*);
#ifdef HAVE_BISON_FLEX
extern int scm_load (char*, rl_ra_rl_ptrdes_t*);
#endif /* HAVE_BISON_FLEX */

extern int rl_add_ptr_to_list (rl_ra_rl_ptrdes_t*);
extern void rl_add_child (int, int, rl_ra_rl_ptrdes_t*);
extern int rl_free_recursively (rl_ra_rl_ptrdes_t);
extern int rl_free_ptrs (rl_ra_rl_ptrdes_t);
extern rl_fd_t * rl_get_fd_by_name (rl_td_t*, char*);
extern rl_fd_t * rl_get_enum_by_value (rl_td_t*, int64_t);
extern int rl_get_enum_by_name (uint64_t*, char*);
extern int rl_parse_add_node (rl_load_t*);
extern int rl_load_bitfield_value (rl_ptrdes_t*, uint64_t*);
extern int rl_save_bitfield_value (rl_ptrdes_t*, uint64_t*);
extern int rl_td_foreach (int (*func) (rl_td_t*, void*), void*);
extern rl_td_t * rl_get_td_by_name (char*);
extern void rl_message_format (void (*output_handler) (char*), rl_message_id_t, va_list);
extern void rl_message (const char*, const char*, int, rl_log_level_t, rl_message_id_t, ...);
extern void rl_message_unsupported_node_type (rl_fd_t*);
extern void * rl_rarray_append (rl_rarray_t*, int);
extern int __attribute__ ((format (printf, 2, 3))) rl_ra_printf (rl_rarray_t*, const char*, ...);

extern char * rl_stringify_int8 (rl_ptrdes_t*);
extern char * rl_stringify_uint8 (rl_ptrdes_t*);
extern char * rl_stringify_int16 (rl_ptrdes_t*);
extern char * rl_stringify_uint16 (rl_ptrdes_t*);
extern char * rl_stringify_int32 (rl_ptrdes_t*);
extern char * rl_stringify_uint32 (rl_ptrdes_t*);
extern char * rl_stringify_int64 (rl_ptrdes_t*);
extern char * rl_stringify_uint64 (rl_ptrdes_t*);
extern char * rl_stringify_enum (rl_ptrdes_t*);
extern char * rl_stringify_bitfield (rl_ptrdes_t*);
extern char * rl_stringify_bitmask (rl_ptrdes_t*, char*);
extern char * rl_stringify_float (rl_ptrdes_t*);
extern char * rl_stringify_double (rl_ptrdes_t*);
extern char * rl_stringify_long_double_t (rl_ptrdes_t*);

extern char * xml_quote_string (char*);
extern char * xml_unquote_string (char*);

#endif /* _RESLIB_H_ */
