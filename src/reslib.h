/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

/* ResLib prototypes */
#ifndef _RESLIB_H_
#define _RESLIB_H_

/*
  you can redefine this prefixes from outside before first include of reslib.h
*/
#ifndef RL_DESCRIPTOR_PREFIX
#define RL_DESCRIPTOR_PREFIX(RL_TYPE_NAME) rl_td_ ## RL_TYPE_NAME
#endif /* RL_DESCRIPTOR_PREFIX */
#ifndef RL_TYPEDEF_PREFIX
#define RL_TYPEDEF_PREFIX(RL_TYPE_NAME) rl_s_ ## RL_TYPE_NAME
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

#define RL_STRINGIFY(STR) #STR

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
   | (__builtin_types_compatible_p (PREFIX float SUFFIX, TYPE) ? RL_TYPE_FLOAT : 0) \
   | (__builtin_types_compatible_p (PREFIX double SUFFIX, TYPE) ? RL_TYPE_DOUBLE : 0) \
   | (__builtin_types_compatible_p (PREFIX long double SUFFIX, TYPE) ? RL_TYPE_LONG_DOUBLE : 0) \
   | (__builtin_types_compatible_p (PREFIX char SUFFIX, TYPE) ? RL_TYPE_CHAR : 0) \
   | (__builtin_types_compatible_p (PREFIX char PAREN_SUFFIX [], TYPE) ? RL_TYPE_CHAR_ARRAY : 0) \
   | (__builtin_types_compatible_p (PREFIX char * SUFFIX, TYPE) ? RL_TYPE_STRING : 0) \
   | (__builtin_types_compatible_p (const char * SUFFIX, TYPE) ? RL_TYPE_STRING : 0) \
   )
#define RL_TYPE_DETECT(TYPE) RL_TYPE_DETECT_INNER ( , TYPE, ,)
#define RL_TYPE_DETECT_PTR(TYPE) (RL_TYPE_DETECT_INNER ( , TYPE, *, (*)) | RL_TYPE_DETECT_INNER (const, TYPE, *, (*)))
#define RL_TYPE_EXT_DETECT(TYPE, S_PTR) (__builtin_types_compatible_p (TYPE[], typeof (S_PTR)) ? RL_TYPE_EXT_ARRAY : RL_TYPE_EXT_NONE)

/* P99 interface */
#include <p99/p99_classification.h>

#define TYPEDEF_STRUCT(...) P00_TYPEDEF (STRUCT, __VA_ARGS__)
#define TYPEDEF_UNION(...) P00_TYPEDEF (UNION, __VA_ARGS__)
#define TYPEDEF_ENUM(...) P00_TYPEDEF (ENUM, __VA_ARGS__)
#define TYPEDEF_CHAR_ARRAY(...) P00_TYPEDEF (CHAR_ARRAY, __VA_ARGS__)

#define P00_TYPEDEF(...) P99_PASTE2 (P00_MODE_, RL_MODE) (__VA_ARGS__)
#define P00_MODE_PROTO(...) P00_TYPEDEF_MODE (PROTO, __VA_ARGS__)
#define P00_MODE_DESC(...) P00_TYPEDEF_MODE (DESC, __VA_ARGS__)
#define P00_MODE_RL_MODE(...)  P00_TYPEDEF_MODE (PROTO, __VA_ARGS__) P00_TYPEDEF_MODE (DESC, __VA_ARGS__)

#define P00_TYPEDEF_MODE(P00_MODE, P00_TYPE, ...) P99_PASTE2 (P00_TYPEDEF_, P00_TYPE) (P00_MODE, P00_TYPE, __VA_ARGS__)
#define P00_TYPEDEF_STRUCT P00_TYPEDEF_COMPAUND
#define P00_TYPEDEF_UNION P00_TYPEDEF_COMPAUND
#define P00_TYPEDEF_ENUM P00_TYPEDEF_COMPAUND
#define P00_TYPEDEF_CHAR_ARRAY(P00_MODE, P00_TYPE, ...) P99_PASTE2 (RL_TYPEDEF_CHAR_ARRAY_, P00_MODE) (__VA_ARGS__)

/*
  TYPEDEF_{STRUCT|UNION|ENUM} have first agrument type name.
  Second argument might be ATTRIBUTES(...) with typedef attributes, comments and extended meta information. The rest is list of fields/enums declarations.
  Next macro checks that ATTRIBUTES are presented.
 */
#define P00_TYPEDEF_COMPAUND(P00_MODE, P00_TYPE, P00_TYPE_NAME, ...)	\
  P99_IF_ELSE (P99_IS_EMPTY (__VA_ARGS__))				\
  (TYPEDEF_ATTR (P00_MODE, P00_TYPE, P00_TYPE_NAME, ATTRIBUTES (), ))	\
  (P00_TYPEDEF_COMPAUND_ (P00_MODE, P00_TYPE, P00_TYPE_NAME, __VA_ARGS__))
#define P00_TYPEDEF_COMPAUND_(P00_MODE, P00_TYPE, P00_TYPE_NAME, FIRST, ...) \
  P99_IF_ELSE (P99_HAS_NO_PAREN (FIRST))				\
  (P99_IF_ELSE (P99_PASTE2 (P00_IS_ATTRIBUTES_EQ_, FIRST))		\
   (TYPEDEF_ATTR (P00_MODE, P00_TYPE, P00_TYPE_NAME, ATTRIBUTES (), FIRST, __VA_ARGS__)) \
   (TYPEDEF_ATTR (P00_MODE, P00_TYPE, P00_TYPE_NAME, FIRST, __VA_ARGS__))) \
  (TYPEDEF_ATTR (P00_MODE, P00_TYPE, P00_TYPE_NAME, ATTRIBUTES (), FIRST, __VA_ARGS__))

#define P00_IS_ATTRIBUTES_EQ_ATTRIBUTES(...) 0 /* help macro for ATTRIBUTES test IF clause */
#define P00_UNFOLD(PREFIX, P00_TYPE, P00_MODE, ...) P99_PASTE4 (PREFIX, P00_TYPE, _, P00_MODE) (__VA_ARGS__)

#define P00_GET_FIRST_ATTRIBUTES(FIRST, ...) FIRST /* extract typedef attributes */
#define P00_GET_LAST_ATTRIBUTES(FIRST, ...) __VA_ARGS__ /* extract typedef comments and extended meta information */
/*
  Next macro produces typedef prolog, body and epilog.
  Prolog is RL_TYPEDEF_{STRUCT|UNION|ENUM}_{PROTO|DESC} (P00_TYPE_NAME, ATTR...)
  Body is a list of fields or enums definition. Structs and union fields are handled with P00_FIELD. Enums definitions are handled with P00_ENUM_DEF.
  Epilog is RL_END_{STRUCT|UNION|ENUM}_{PROTO|DESC} (P00_TYPE_NAME, COM...)
 */
#define TYPEDEF_ATTR(P00_MODE, P00_TYPE, P00_TYPE_NAME, ATTR_COM_EXT, ...) \
  P00_UNFOLD (RL_TYPEDEF_, P00_TYPE, P00_MODE, P00_TYPE_NAME, P99_PASTE2 (P00_GET_FIRST_, ATTR_COM_EXT)) \
  P99_FOR ((P00_MODE, P00_TYPE_NAME), P99_NARG (__VA_ARGS__), P00_SER, P99_PASTE3 (P00_, P00_TYPE, _HANDLER), __VA_ARGS__) \
  P00_UNFOLD (RL_END_, P00_TYPE, P00_MODE, P00_TYPE_NAME, P99_PASTE2 (P00_GET_LAST_, ATTR_COM_EXT))

#define P00_STRUCT_HANDLER P00_FIELD
#define P00_UNION_HANDLER P00_FIELD
#define P00_ENUM_HANDLER P00_ENUM_DEF

/* field handler checks for trailing empty field */
#define P00_FIELD(P00_MODE_TYPE_NAME, FIELD, I) P99_IF_ELSE (P99_IS_EMPTY (FIELD)) () (P00_FIELD_ (P00_MODE_TYPE_NAME, FIELD))
/*
  field descritions might be in two forms.
  1. RL_TYPE_MACRO (ARGS) for type scpecific declarations like INT32 (x)
  2. (TYPE, NAME, SUFFIX..., COMMENT..., EXT...) for auto detection declarations.
 */
#define P00_FIELD_(P00_MODE_TYPE_NAME, FIELD)				\
  P99_IF_ELSE (P99_HAS_NO_PAREN (FIELD))				\
  (P00_FIELD_UNFOLD (P00_MODE_TYPE_NAME, FIELD))			\
  (P00_FIELD_DETECT (P00_MODE_TYPE_NAME, FIELD, P99_SUB (2, 1, P99_REMOVE_PAREN (FIELD))))

/*
  There are 3 options for auto-detection:
    a. Suffix with parentheses goes to FUNC as function.
    b. Non-empty suffix without parentheses goes to ARRAY as array.
    c. Everything else goes to AUTO.
 */
#define P00_FIELD_DETECT(P00_MODE_TYPE_NAME, FIELD, SUFFIX)	\
  P99_IF_ELSE (P99_HAS_NO_PAREN (SUFFIX))			\
  (P99_IF_ELSE (P99_IS_EMPTY (SUFFIX))				\
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
#define P00_FIELD_UNFOLD(P00_MODE_TYPE_NAME, FIELD) P00_FIELD_UNFOLD_ (P00_MODE_TYPE_NAME, FIELD, P99_PASTE2 (P00_COMMA_, FIELD))
#define P00_FIELD_UNFOLD_(P00_MODE_TYPE_NAME, FIELD, P00_FIELD_COMMA) P00_FIELD_UNFOLD__ (P00_MODE_TYPE_NAME, FIELD, P00_FIELD_COMMA)
#define P00_FIELD_UNFOLD__(P00_MODE_TYPE_NAME, FIELD, P00_FIELD_COMMA,  ...)	\
  P99_IF_ELSE (P99_IS_EMPTY (__VA_ARGS__))				\
  (P00_UNFOLD (RL_, UNKNOWN, P00_GET_MODE P00_MODE_TYPE_NAME, , FIELD))	\
  (P00_UNFOLD (RL_, P00_FIELD_COMMA, P00_GET_MODE P00_MODE_TYPE_NAME, P00_GET_TYPE_NAME P00_MODE_TYPE_NAME, P99_REMOVE_PAREN (__VA_ARGS__)))

/* produce compilation error for unkown field qualifiers */
#define RL_UNKNOWN_PROTO(P00_TYPE_NAME, ...) int _1[RL_STRINGIFY(__VA_ARGS__)()];
#define RL_UNKNOWN_DESC(P00_TYPE_NAME, ...) { RL_STRINGIFY(__VA_ARGS__)(), },

/*
  Check for empty trailing enum definition
  Enums definition might be in two forms.
  1. just a enum name
  2. (ENUM_NAME, RIGHT_HAND_SIDE..., COMMENT..., EXT...) enum and some extra information in parentheses
  both unfolds into RL_ENUM_DEF_{PROTO|DESC} (P00_TYPE_NAME, ARGS)
*/
#define P00_ENUM_DEF(P00_MODE_TYPE_NAME, FIELD, I)			\
  P99_IF_ELSE (P99_IS_EMPTY (FIELD))					\
  ()									\
  (P00_UNFOLD (RL_, ENUM_DEF, P00_GET_MODE P00_MODE_TYPE_NAME, P00_GET_TYPE_NAME P00_MODE_TYPE_NAME, P99_REMOVE_PAREN (FIELD)))

/* list of substitutions for P00_FIELD_UNFOLD_ */
#define P00_COMMA_FIELD FIELD,
#define P00_COMMA_AUTO AUTO,
#define P00_COMMA_NONE NONE,
#define P00_COMMA_ENUM ENUM,
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

#define P00_COMMA_ANON_UNION ANON_UNION,
#define P00_COMMA_NAMED_ANON_UNION NAMED_ANON_UNION,
#define P00_COMMA_END_ANON_UNION END_ANON_UNION,

/* P99 interface end */

#define RL_UNFOLD(NODE, ...) RL_UNFOLD_ (NODE, RL_MODE, __VA_ARGS__)
#define RL_UNFOLD_(NODE, MODE, ...) RL_UNFOLD__ (NODE, MODE, __VA_ARGS__)
#define RL_UNFOLD__(NODE, MODE, ...) NODE ## _ ## MODE (RL_TYPE_NAME, __VA_ARGS__)

#define RL_TYPE_ARGN_(RL_TYPE, RL_TYPE_NAME, TYPE, NAME...) RL_TYPE_ARGN (RL_TYPE, RL_TYPE_NAME, TYPE, NAME, 5, 4, 3, 2)
#define RL_TYPE_ARGN(RL_TYPE, RL_TYPE_NAME, TYPE, NAME, SUFFIX, COM, EXT, N, ...) RL_TYPE_ARG ## N (RL_TYPE, RL_TYPE_NAME, TYPE, NAME, SUFFIX, COM, EXT)
#define RL_TYPE_ARG2(RL_TYPE, RL_TYPE_NAME, TYPE, NAME, SUFFIX, COM, EXT) RL_TYPE (RL_TYPE_NAME, TYPE, NAME, ,)
#define RL_TYPE_ARG3(RL_TYPE, RL_TYPE_NAME, TYPE, NAME, SUFFIX, COM, EXT) RL_TYPE (RL_TYPE_NAME, TYPE, NAME, SUFFIX,)
#define RL_TYPE_ARG4(RL_TYPE, RL_TYPE_NAME, TYPE, NAME, SUFFIX, COM, EXT) RL_TYPE (RL_TYPE_NAME, TYPE, NAME, SUFFIX, COM)
#define RL_TYPE_ARG5(RL_TYPE, RL_TYPE_NAME, TYPE, NAME, SUFFIX, COM, EXT) RL_TYPE (RL_TYPE_NAME, TYPE, NAME, SUFFIX, COM, EXT)

#define RL_ENUM_DEF_(RL_TYPE, RL_TYPE_NAME, NAME...) RL_ENUM_DEF_ARGN (RL_TYPE, RL_TYPE_NAME, NAME, 4, 3, 2, 1)
#define RL_ENUM_DEF_ARGN(RL_TYPE, RL_TYPE_NAME, NAME, RHS, COM, EXT, N, ...) RL_ENUM_DEF_ARG ## N (RL_TYPE, RL_TYPE_NAME, NAME, RHS, COM, EXT)
#define RL_ENUM_DEF_ARG1(RL_TYPE, RL_TYPE_NAME, NAME, RHS, COM, EXT) RL_TYPE (RL_TYPE_NAME, NAME, ,)
#define RL_ENUM_DEF_ARG2(RL_TYPE, RL_TYPE_NAME, NAME, RHS, COM, EXT) RL_TYPE (RL_TYPE_NAME, NAME, RHS,)
#define RL_ENUM_DEF_ARG3(RL_TYPE, RL_TYPE_NAME, NAME, RHS, COM, EXT) RL_TYPE (RL_TYPE_NAME, NAME, RHS, COM)
#define RL_ENUM_DEF_ARG4(RL_TYPE, RL_TYPE_NAME, NAME, RHS, COM, EXT) RL_TYPE (RL_TYPE_NAME, NAME, RHS, COM, EXT)

#define RL_FOR1(X, _0, ...) X (_0)
#define RL_FOR2(X, _0, ...) X (_0) RL_FOR1 (X, __VA_ARGS__)
#define RL_FOR3(X, _0, ...) X (_0) RL_FOR2 (X, __VA_ARGS__)
#define RL_FOR4(X, _0, ...) X (_0) RL_FOR3 (X, __VA_ARGS__)
#define RL_FOR5(X, _0, ...) X (_0) RL_FOR4 (X, __VA_ARGS__)
#define RL_FOR6(X, _0, ...) X (_0) RL_FOR5 (X, __VA_ARGS__)
#define RL_FOR7(X, _0, ...) X (_0) RL_FOR6 (X, __VA_ARGS__)
#define RL_FOR8(X, _0, ...) X (_0) RL_FOR7 (X, __VA_ARGS__)
#define RL_FOR9(X, _0, ...) X (_0) RL_FOR8 (X, __VA_ARGS__)
#define RL_NARG(...) RL_NARG_ (__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define RL_NARG_(_1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...) N
#define RL_PASTE2(_0, _1) RL_PASTE2_ (_0, _1)
#define RL_PASTE2_(_0, _1) _0 ## _1
#define RL_FOR(X, ...) RL_PASTE2 (RL_FOR, RL_NARG (__VA_ARGS__)) (X, __VA_ARGS__)
#define RL_REMOVE_PAREN_(...) __VA_ARGS__
#define RL_REMOVE_PAREN(...) RL_REMOVE_PAREN_ __VA_ARGS__

#define RL_TYPEDEF_STRUCT(...) RL_UNFOLD (RL_TYPEDEF_STRUCT, __VA_ARGS__)

#define RL_AUTO(...) RL_UNFOLD (RL_AUTO, __VA_ARGS__)
#define RL_NONE(...) RL_UNFOLD (RL_NONE, __VA_ARGS__)
#define RL_ENUM(...) RL_UNFOLD (RL_ENUM, __VA_ARGS__)
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
#define RL_NAMED_ANON_UNION(...) RL_UNFOLD (RL_NAMED_ANON_UNION, __VA_ARGS__)
#define RL_END_ANON_UNION(...) RL_UNFOLD (RL_END_ANON_UNION, __VA_ARGS__)

#define RL_TYPEDEF_ENUM(...) RL_UNFOLD (RL_TYPEDEF_ENUM, __VA_ARGS__)
#define RL_ENUM_DEF(...) RL_UNFOLD (RL_ENUM_DEF, __VA_ARGS__)
#define RL_END_ENUM(...) RL_UNFOLD (RL_END_ENUM, __VA_ARGS__)

#define RL_TYPEDEF_CHAR_ARRAY(...) RL_UNFOLD (RL_TYPEDEF_CHAR_ARRAY, __VA_ARGS__)

/* Macroses for prototypes generation mode */
#define RL_TYPEDEF_STRUCT_PROTO(RL_TYPE_NAME, ATTR...) typedef struct RL_TYPEDEF_PREFIX (RL_TYPE_NAME) RL_TYPE_NAME; struct ATTR RL_TYPEDEF_PREFIX (RL_TYPE_NAME) {

#define RL_AUTO_PROTO(...) RL_TYPE_ARGN_ (RL_FIELD_PROTO, __VA_ARGS__)
#define RL_NONE_PROTO(...) RL_TYPE_ARGN_ (RL_FIELD_PROTO, __VA_ARGS__)
#define RL_CHAR_ARRAY_PROTO(...) RL_TYPE_ARGN_ (RL_FIELD_PROTO, __VA_ARGS__)

#define RL_FIELD_PROTO(RL_TYPE_NAME, TYPE, NAME, SUFFIX, COM...) TYPE NAME SUFFIX;
#define RL_ENUM_PROTO(RL_TYPE_NAME, TYPE, NAME, COM...) RL_FIELD_PROTO (RL_TYPE_NAME, TYPE, NAME, )
#define RL_BITMASK_PROTO(RL_TYPE_NAME, TYPE, NAME, COM...) RL_FIELD_PROTO (RL_TYPE_NAME, TYPE, NAME, )
#define RL_INT8_PROTO(RL_TYPE_NAME, NAME, COM...) RL_FIELD_PROTO (RL_TYPE_NAME, int8_t, NAME, )
#define RL_UINT8_PROTO(RL_TYPE_NAME, NAME, COM...) RL_FIELD_PROTO (RL_TYPE_NAME, uint8_t, NAME, )
#define RL_INT16_PROTO(RL_TYPE_NAME, NAME, COM...) RL_FIELD_PROTO (RL_TYPE_NAME, int16_t, NAME, )
#define RL_UINT16_PROTO(RL_TYPE_NAME, NAME, COM...) RL_FIELD_PROTO (RL_TYPE_NAME, uint16_t, NAME, )
#define RL_INT32_PROTO(RL_TYPE_NAME, NAME, COM...) RL_FIELD_PROTO (RL_TYPE_NAME, int32_t, NAME, )
#define RL_UINT32_PROTO(RL_TYPE_NAME, NAME, COM...) RL_FIELD_PROTO (RL_TYPE_NAME, uint32_t, NAME, )
#define RL_INT64_PROTO(RL_TYPE_NAME, NAME, COM...) RL_FIELD_PROTO (RL_TYPE_NAME, int64_t, NAME, )
#define RL_UINT64_PROTO(RL_TYPE_NAME, NAME, COM...) RL_FIELD_PROTO (RL_TYPE_NAME, uint64_t, NAME, )
#define RL_FLOAT_PROTO(RL_TYPE_NAME, NAME, COM...) RL_FIELD_PROTO (RL_TYPE_NAME, float, NAME, )
#define RL_DOUBLE_PROTO(RL_TYPE_NAME, NAME, COM...) RL_FIELD_PROTO (RL_TYPE_NAME, double, NAME, )
#define RL_LONG_DOUBLE_PROTO(RL_TYPE_NAME, NAME, COM...) RL_FIELD_PROTO (RL_TYPE_NAME, long double, NAME, )
#define RL_CHAR_PROTO(RL_TYPE_NAME, NAME, COM...) RL_FIELD_PROTO (RL_TYPE_NAME, char, NAME, )
#define RL_STRING_PROTO(RL_TYPE_NAME, NAME, COM...) RL_FIELD_PROTO (RL_TYPE_NAME, char *, NAME, )
#define RL_STRUCT_PROTO(RL_TYPE_NAME, TYPE, NAME, COM...) RL_FIELD_PROTO (RL_TYPE_NAME, TYPE, NAME, )
#define RL_UNION_PROTO(RL_TYPE_NAME, TYPE, NAME, COM...) RL_FIELD_PROTO (RL_TYPE_NAME, TYPE, NAME, )
#define RL_ARRAY_PROTO RL_FIELD_PROTO
#define RL_POINTER_PROTO(RL_TYPE_NAME, TYPE, NAME, COM...) RL_FIELD_PROTO (RL_TYPE_NAME, TYPE *, NAME, )
#define RL_POINTER_STRUCT_PROTO(RL_TYPE_NAME, TYPE, NAME, COM...) RL_FIELD_PROTO (RL_TYPE_NAME, struct RL_TYPEDEF_PREFIX (TYPE) *, NAME, )
/* rarray defenition should be syncroonized with rl_rarray_t type definition */
#define RL_RARRAY_PROTO(RL_TYPE_NAME, TYPE, NAME, COM...) RL_FIELD_PROTO (RL_TYPE_NAME, struct __attribute__((packed)) {TYPE * data; int32_t size; int32_t alloc_size; void * ext;}, NAME, )
#define RL_FUNC_PROTO(RL_TYPE_NAME, TYPE, NAME, ARGS, COM...) RL_FIELD_PROTO (RL_TYPE_NAME, TYPE, (*NAME), ARGS)
#define RL_END_STRUCT_PROTO(RL_TYPE_NAME, COM...) };

#define RL_TYPEDEF_UNION_PROTO(RL_TYPE_NAME, ATTR...) typedef union ATTR RL_TYPEDEF_PREFIX (RL_TYPE_NAME) {
#define RL_END_UNION_PROTO(RL_TYPE_NAME, COM...) } RL_TYPE_NAME;
#define RL_ANON_UNION_PROTO(RL_TYPE_NAME, ATTR...) RL_ANON_UNION_PROTO_ (RL_TYPE_NAME, __LINE__, ATTR)
#define RL_ANON_UNION_PROTO_(RL_TYPE_NAME, LINE, ATTR...) RL_NAMED_ANON_UNION_PROTO (RL_TYPE_NAME, RL_ANON_UNION_NAME (LINE), ATTR)
#define RL_NAMED_ANON_UNION_PROTO(RL_TYPE_NAME, NAME, ATTR...) char NAME[0]; union ATTR {
#define RL_END_ANON_UNION_PROTO(RL_TYPE_NAME, COM...) };

#define RL_TYPEDEF_ENUM_PROTO(RL_TYPE_NAME, ATTR...) typedef enum ATTR RL_TYPEDEF_PREFIX (RL_TYPE_NAME) {
#define RL_ENUM_DEF_PROTO_(RL_TYPE_NAME, NAME, RHS, COM...) NAME RHS,
#define RL_ENUM_DEF_PROTO(...) RL_ENUM_DEF_ (RL_ENUM_DEF_PROTO_, __VA_ARGS__)
#define RL_END_ENUM_PROTO(RL_TYPE_NAME, COM...) } RL_TYPE_NAME;

#define RL_TYPEDEF_CHAR_ARRAY_PROTO(RL_TYPE_NAME, SIZE, ...) typedef char RL_TYPE_NAME[SIZE];

/* Macroses for descriptors generation mode */

#define RL_TYPEDEF_STRUCT_DESC(RL_TYPE_NAME, ATTR...) RL_TYPEDEF_DESC (RL_TYPE_NAME, RL_TYPE_STRUCT, ATTR)

#define RL_FIELD_DESC(RL_TYPE_NAME, TYPE, NAME, SUFFIX, RL_TYPE, RL_TYPE_EXT, COM...)  { \
    .type = #TYPE,							\
      .name = #NAME,							\
      .size = sizeof (TYPE),						\
      .offset = offsetof (RL_TYPE_NAME, NAME),				\
      .rl_type = RL_TYPE,						\
      .rl_type_ext = RL_TYPE_EXT,					\
      .count = sizeof (TYPE) == 0 ? 0 :					\
        sizeof (((RL_TYPE_NAME*)NULL)->NAME) / sizeof (TYPE),           \
      .row_count = 1,							\
      .value = 0,							\
      .ext = NULL,							\
      .comment = "" COM,						\
      },

#define RL_POINTER_STRUCT_DESC(RL_TYPE_NAME, TYPE, NAME, COM...)  { \
    .type = #TYPE,							\
      .name = #NAME,							\
      .size = 0,							\
      .offset = offsetof (RL_TYPE_NAME, NAME),				\
      .rl_type = RL_TYPE_STRUCT,					\
      .rl_type_ext = RL_TYPE_EXT_POINTER,				\
      .count = 1,							\
      .row_count = 1,							\
      .value = 0,							\
      .ext = NULL,							\
      .comment = "" COM,						\
      },

#define RL_ARRAY_DESC(RL_TYPE_NAME, TYPE, NAME, SUFFIX, COM...)  {	\
    .type = #TYPE,							\
      .name = #NAME,							\
      .size = sizeof (TYPE),						\
      .offset = offsetof (RL_TYPE_NAME, NAME),				\
      .rl_type = RL_TYPE_DETECT (TYPE),					\
      .rl_type_ext = RL_TYPE_EXT_ARRAY,					\
      .count = sizeof (TYPE) == 0 ? 0 :					\
        sizeof (((RL_TYPE_NAME*)NULL)->NAME) / sizeof (TYPE),           \
      .row_count = sizeof (((RL_TYPE_NAME*)NULL)->NAME[0]) / sizeof (TYPE), \
      .value = 0,							\
      .ext = NULL,							\
      .comment = "" COM,						\
      },

#define RL_NONE_DESC_(RL_TYPE_NAME, TYPE, NAME, SUFFIX, COM...)  {	\
    .type = #TYPE,							\
      .name = #NAME,							\
      .size = sizeof (TYPE),						\
      .offset = 0,							\
      .rl_type = RL_TYPE_VOID,						\
      .rl_type_ext = RL_TYPE_EXT_NONE,					\
      .count = 0,							\
      .row_count = 0,							\
      .ext = NULL,							\
      .comment = "" COM,						\
      },

#define RL_AUTO_DESC_(RL_TYPE_NAME, TYPE, NAME, SUFFIX, COM...) RL_FIELD_DESC (RL_TYPE_NAME, TYPE, NAME, SUFFIX, RL_TYPE_DETECT (TYPE), RL_TYPE_EXT_DETECT (TYPE, ((RL_TYPE_NAME*)NULL)->NAME), COM, .rl_type_ptr = RL_TYPE_DETECT_PTR (TYPE))

#define RL_AUTO_DESC(...) RL_TYPE_ARGN_ (RL_AUTO_DESC_, __VA_ARGS__)
#define RL_NONE_DESC(...) RL_TYPE_ARGN_ (RL_NONE_DESC_, __VA_ARGS__)
#define RL_CHAR_ARRAY_DESC(...) RL_TYPE_ARGN_ (RL_CHAR_ARRAY_DESC_, __VA_ARGS__)

#define RL_ENUM_DESC(RL_TYPE_NAME, TYPE, NAME, COM...) RL_FIELD_DESC (RL_TYPE_NAME, TYPE, NAME, , RL_TYPE_ENUM, RL_TYPE_EXT_NONE, COM)
#define RL_BITMASK_DESC(RL_TYPE_NAME, TYPE, NAME, COM...) RL_FIELD_DESC (RL_TYPE_NAME, TYPE, NAME, , RL_TYPE_BITMASK, RL_TYPE_EXT_NONE, COM)
#define RL_INT8_DESC(RL_TYPE_NAME, NAME, COM...) RL_FIELD_DESC (RL_TYPE_NAME, int8_t, NAME, , RL_TYPE_INT8, RL_TYPE_EXT_NONE, COM)
#define RL_UINT8_DESC(RL_TYPE_NAME, NAME, COM...) RL_FIELD_DESC (RL_TYPE_NAME, uint8_t, NAME, , RL_TYPE_UINT8, RL_TYPE_EXT_NONE, COM)
#define RL_INT16_DESC(RL_TYPE_NAME, NAME, COM...) RL_FIELD_DESC (RL_TYPE_NAME, int16_t, NAME, , RL_TYPE_INT16, RL_TYPE_EXT_NONE, COM)
#define RL_UINT16_DESC(RL_TYPE_NAME, NAME, COM...) RL_FIELD_DESC (RL_TYPE_NAME, uint16_t, NAME, , RL_TYPE_UINT16, RL_TYPE_EXT_NONE, COM)
#define RL_INT32_DESC(RL_TYPE_NAME, NAME, COM...) RL_FIELD_DESC (RL_TYPE_NAME, int32_t, NAME, , RL_TYPE_INT32, RL_TYPE_EXT_NONE, COM)
#define RL_UINT32_DESC(RL_TYPE_NAME, NAME, COM...) RL_FIELD_DESC (RL_TYPE_NAME, uint32_t, NAME, , RL_TYPE_UINT32, RL_TYPE_EXT_NONE, COM)
#define RL_INT64_DESC(RL_TYPE_NAME, NAME, COM...) RL_FIELD_DESC (RL_TYPE_NAME, int64_t, NAME, , RL_TYPE_INT64, RL_TYPE_EXT_NONE, COM)
#define RL_UINT64_DESC(RL_TYPE_NAME, NAME, COM...) RL_FIELD_DESC (RL_TYPE_NAME, uint64_t, NAME, , RL_TYPE_UINT64, RL_TYPE_EXT_NONE, COM)
#define RL_FLOAT_DESC(RL_TYPE_NAME, NAME, COM...) RL_FIELD_DESC (RL_TYPE_NAME, float, NAME, , RL_TYPE_FLOAT, RL_TYPE_EXT_NONE, COM)
#define RL_DOUBLE_DESC(RL_TYPE_NAME, NAME, COM...) RL_FIELD_DESC (RL_TYPE_NAME, double, NAME, , RL_TYPE_DOUBLE, RL_TYPE_EXT_NONE, COM)
#define RL_LONG_DOUBLE_DESC(RL_TYPE_NAME, NAME, COM...) RL_FIELD_DESC (RL_TYPE_NAME, long double, NAME, , RL_TYPE_LONG_DOUBLE, RL_TYPE_EXT_NONE, COM)
#define RL_CHAR_DESC(RL_TYPE_NAME, NAME, COM...) RL_FIELD_DESC (RL_TYPE_NAME, char, NAME, , RL_TYPE_CHAR, RL_TYPE_EXT_NONE, COM)
#define RL_STRING_DESC(RL_TYPE_NAME, NAME, COM...) RL_FIELD_DESC (RL_TYPE_NAME, char *, NAME, , RL_TYPE_STRING, RL_TYPE_EXT_NONE, COM)
#define RL_CHAR_ARRAY_DESC_(RL_TYPE_NAME, TYPE, NAME, SUFFIX, COM...) RL_FIELD_DESC (RL_TYPE_NAME, TYPE, NAME, SUFFIX, RL_TYPE_CHAR_ARRAY, RL_TYPE_EXT_NONE, COM)
#define RL_STRUCT_DESC(RL_TYPE_NAME, TYPE, NAME, COM...) RL_FIELD_DESC (RL_TYPE_NAME, TYPE, NAME, , RL_TYPE_STRUCT, RL_TYPE_EXT_NONE, COM)
#define RL_UNION_DESC(RL_TYPE_NAME, TYPE, NAME, COM...) RL_FIELD_DESC (RL_TYPE_NAME, TYPE, NAME, , RL_TYPE_UNION, RL_TYPE_EXT_NONE, COM)
#define RL_POINTER_DESC(RL_TYPE_NAME, TYPE, NAME, COM...) RL_FIELD_DESC (RL_TYPE_NAME, TYPE, NAME, , RL_TYPE_DETECT (TYPE), RL_TYPE_EXT_POINTER, COM)
#define RL_RARRAY_DESC(RL_TYPE_NAME, TYPE, NAME, COM...) RL_FIELD_DESC (RL_TYPE_NAME, TYPE, NAME, , RL_TYPE_DETECT (TYPE), RL_TYPE_EXT_RARRAY, COM)
#define RL_FUNC_DESC(RL_TYPE_NAME, TYPE, NAME, ARGS, COM...) RL_FIELD_DESC (RL_TYPE_NAME, TYPE, NAME, , RL_TYPE_FUNC, RL_TYPE_EXT_NONE, COM, .args = { .alloc_size = -1, .size = 0, .data = (rl_fd_t[]){ RL_FUNC_ARG (TYPE, "return value") RL_FOR (RL_FUNC_ARG, RL_REMOVE_PAREN (ARGS)) { .rl_type = RL_TYPE_TRAILING_RECORD, }, }, })
#define RL_FUNC_ARG(TYPE, COM...) { .type = #TYPE, .size = sizeof (TYPE), .rl_type = RL_TYPE_DETECT (TYPE), .rl_type_ptr = RL_TYPE_DETECT_PTR (TYPE), .rl_type_ext = RL_TYPE_EXT_NONE, .comment = "" COM, },
#define RL_END_STRUCT_DESC(RL_TYPE_NAME, COM...) RL_TYPEDEF_END_DESC (RL_TYPE_NAME, COM)

#define RL_TYPEDEF_UNION_DESC(RL_TYPE_NAME, ATTR...) RL_TYPEDEF_DESC (RL_TYPE_NAME, RL_TYPE_UNION, ATTR)
#define RL_END_UNION_DESC(RL_TYPE_NAME, COM...) RL_TYPEDEF_END_DESC (RL_TYPE_NAME, COM)
#define RL_ANON_UNION_DESC(RL_TYPE_NAME, ATTR...) RL_ANON_UNION_DESC_ (RL_TYPE_NAME, __LINE__, ATTR)
#define RL_ANON_UNION_DESC_(RL_TYPE_NAME, LINE, ATTR...) RL_NAMED_ANON_UNION_DESC (RL_TYPE_NAME, RL_ANON_UNION_NAME (LINE), ATTR)

#define RL_NAMED_ANON_UNION_DESC(RL_TYPE_NAME, NAME, ATTR...) {		\
    .type = "anon_union_t",						\
      .name = RL_STRINGIFY (NAME),					\
      .size = 0,							\
      .offset = offsetof (RL_TYPE_NAME, NAME),				\
      .rl_type = RL_TYPE_ANON_UNION,					\
      .rl_type_ext = RL_TYPE_EXT_NONE,					\
      .count = 0,							\
      .row_count = 0,							\
      .comment = #ATTR,							\
      .ext = NULL							\
      },
#define RL_END_ANON_UNION_DESC(RL_TYPE_NAME, COM...) {	\
    .type = "anon_union_t",			\
      .name = NULL,				\
      .size = 0,				\
      .offset = 0,				\
      .rl_type = RL_TYPE_END_ANON_UNION,	\
      .rl_type_ext = RL_TYPE_EXT_NONE,		\
      .count = 0,				\
      .row_count = 0,				\
      .ext = NULL,				\
      .comment = "" COM,			\
      },

#define RL_TYPEDEF_ENUM_DESC(RL_TYPE_NAME, ATTR...) RL_TYPEDEF_DESC (RL_TYPE_NAME, RL_TYPE_ENUM, ATTR)
#define RL_ENUM_DEF_DESC_(RL_TYPE_NAME, NAME, RHS, COM...) { .type = RL_STRINGIFY (RL_TYPE_NAME), .name = #NAME, .value = NAME, .rl_type = RL_TYPE_ENUM, .ext = NULL, .comment = "" COM, },
#define RL_ENUM_DEF_DESC(...) RL_ENUM_DEF_ (RL_ENUM_DEF_DESC_, __VA_ARGS__)
#define RL_END_ENUM_DESC(RL_TYPE_NAME, COM...) RL_TYPEDEF_END_DESC (RL_TYPE_NAME, COM)

#define RL_TYPEDEF_CHAR_ARRAY_DESC(RL_TYPE_NAME, SIZE, COM...) RL_TYPEDEF_DESC (RL_TYPE_NAME, RL_TYPE_CHAR_ARRAY) RL_TYPEDEF_END_DESC (RL_TYPE_NAME, COM)

#define RL_TYPEDEF_DESC(RL_TYPE_NAME, RL_TYPE, ATTR...) RL_DESCRIPTOR_ATTR rl_td_t RL_DESCRIPTOR_PREFIX (RL_TYPE_NAME) = { \
    .rl_type = RL_TYPE,							\
    .type = RL_STRINGIFY (RL_TYPE_NAME),				\
    .size = sizeof (RL_TYPE_NAME),					\
    .attr = #ATTR,							\
    .ext = NULL,							\
    .fields = { .alloc_size = -1, .size = 0, .data = (rl_fd_t[]){
#define RL_TYPEDEF_END_DESC(RL_TYPE_NAME, COM...) {.rl_type = RL_TYPE_TRAILING_RECORD} } }, .comment = "" COM }; static inline void __attribute__((constructor)) RL_CONSTRUCTOR_PREFIX (RL_TYPE_NAME) (void) { RL_ADD_TYPE (RL_TYPE_NAME); }

/* Library exports */
#define RL_MAX_TYPES (256)
#define RL_MAX_STRING_LENGTH ((unsigned int)-1)

#define RL_INT_TO_STRING_BUF_SIZE (32)
#define RL_FLOAT_TO_STRING_BUF_SIZE (256)
#define RL_CHAR_TO_STRING_BUF_SIZE (8)

#define RL_MAX_INDENT_LEVEL (30) /* the same constant as in libxml2 */
#define RL_MIN(X,Y) ({ typeof(X) _x_ = (X); typeof(Y) _y_ = (Y); (_x_ < _y_) ? _x_ : _y_; })
#define RL_MAX(X,Y) ({ typeof(X) _x_ = (X); typeof(Y) _y_ = (Y); (_x_ > _y_) ? _x_ : _y_; })
#define RL_LIMIT_LEVEL(LEVEL) RL_MIN (LEVEL, RL_MAX_INDENT_LEVEL)

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
#include <inttypes.h> /* for int32_t */
#ifdef HAVE_LIBXML2
# include <libxml/xmlmemory.h>
# include <libxml/parser.h>
#endif /* HAVE_LIBXML2 */
#include <rpc/types.h>
#include <rpc/xdr.h>

#ifndef RL_MODE
#define RL_MODE_UNDEFINED
#define RL_MODE PROTO
#endif
#include <rlprotos.h>
#ifdef RL_MODE_UNDEFINED
#undef RL_MODE_UNDEFINED
#undef RL_MODE
#endif

extern rl_conf_t rl_conf;

typedef char * (*rl_output_format_t) (rl_ptrdes_t*);

typedef long double long_double_t;

extern rl_io_handler_t rl_io_handlers[RL_MAX_TYPES];
extern rl_io_handler_t rl_io_ext_handlers[RL_MAX_TYPES];
extern rl_output_format_t rl_output_format[RL_MAX_TYPES];

extern int __attribute__ ((sentinel(0))) rl_add_type (rl_td_t*, char*, ...);
extern char * rl_read_xml_doc (FILE*);

extern void rl_save (void*, rl_fd_t*, rl_save_data_t*);
extern int rl_load (void*, rl_fd_t*, int, rl_ra_rl_ptrdes_t*);
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
extern void rl_free_ptrs (rl_ra_rl_ptrdes_t*);
extern rl_fd_t * rl_get_fd_by_name (rl_td_t*, char*);
extern rl_fd_t * rl_get_enum_by_value (rl_td_t*, int64_t);
extern int rl_get_enum_by_name (int64_t*, char*);
extern int rl_parse_add_node (rl_load_t*);

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
extern char * rl_stringify_bitmask (rl_ptrdes_t*, char*);
extern char * rl_stringify_float (rl_ptrdes_t*);
extern char * rl_stringify_double (rl_ptrdes_t*);
extern char * rl_stringify_long_double_t (rl_ptrdes_t*);

extern char * xml_quote_string (char*);
extern char * xml_unquote_string (char*);

/* each refereed structure will have REF_IDX property */
#define RL_REF_IDX "ref_idx"
/* references on already saved structures will be replaced with nodes that have only REF index property */
#define RL_REF "ref"
/* XML attribute for zero length strings */
#define RL_ISNULL "isnull"

#define RL_BITMASK_OR_DELIMITER " | "

#define RL_MESSAGE(LOG_LEVEL, MSG_ID...) rl_message (__FILE__, __PRETTY_FUNCTION__, __LINE__, LOG_LEVEL, MSG_ID)
#define RL_MESSAGE_UNSUPPORTED_NODE_TYPE_(FDP)				\
  ({									\
    rl_fd_t * __fdp__ = FDP;						\
    rl_td_t * rl_type_td = rl_get_td_by_name ("rl_type_t");		\
    rl_td_t * rl_type_ext_td = rl_get_td_by_name ("rl_type_ext_t");	\
    rl_fd_t * rl_type_fd = rl_type_td ? rl_get_enum_by_value (rl_type_td, __fdp__->rl_type) : NULL; \
    rl_fd_t * rl_type_ext_fd = rl_type_ext_td ? rl_get_enum_by_value (rl_type_ext_td, __fdp__->rl_type_ext) : NULL; \
    RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_UNSUPPORTED_NODE_TYPE, (rl_type_fd ? rl_type_fd->name : "unknown"), __fdp__->rl_type, (rl_type_ext_fd ? rl_type_ext_fd->name : "unknown"), __fdp__->rl_type_ext); \
  })

#define RL_MEM_INIT(FUNC, ATTR...) void ATTR rl_mem_init (void) { FUNC; }

#define RL_MALLOC(SIZE) (rl_conf.rl_mem.malloc)(__FILE__, __PRETTY_FUNCTION__, __LINE__, SIZE)
#define RL_REALLOC(PTR, SIZE) (rl_conf.rl_mem.realloc)(__FILE__, __PRETTY_FUNCTION__, __LINE__, PTR, SIZE)
#define RL_STRDUP(STR) (rl_conf.rl_mem.strdup)(__FILE__, __PRETTY_FUNCTION__, __LINE__, STR)
#define RL_FREE(PTR) (rl_conf.rl_mem.free)(__FILE__, __PRETTY_FUNCTION__, __LINE__, PTR)

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
#define RL_ADD_TYPE(RL_TYPE_NAME, COM...) rl_add_type (&RL_DESCRIPTOR_PREFIX (RL_TYPE_NAME), "" COM, NULL)

/*
  User can turn off strict types checking for ResLib macroses, so compilation will produce only warnings.
  RL_CHECK_TYPES macro should be predefined as
  #define RL_CHECK_TYPES(...)
*/
#ifndef RL_CHECK_TYPES
#define RL_CHECK_TYPES(RL_TYPE_NAME, S_PTR...) ({ (void) ((RL_TYPE_NAME*)0 - (typeof (S_PTR + 0))0); })
#endif /* RL_CHECK_TYPES */

#define RL_SAVE_RL(RL_TYPE_NAME, S_PTR) ({				\
      char __name__[] = #S_PTR;						\
      rl_fd_t __fd__ =							\
	{								\
	  .name = __name__,						\
	  .type = #RL_TYPE_NAME,					\
	  .rl_type = RL_TYPE_DETECT (RL_TYPE_NAME),			\
	  .rl_type_ext = RL_TYPE_EXT_DETECT (RL_TYPE_NAME, S_PTR),	\
	  .size = sizeof (RL_TYPE_NAME),				\
	  .count = sizeof (RL_TYPE_NAME) == 0 ? 0 :                     \
            sizeof (S_PTR) / sizeof (RL_TYPE_NAME),                     \
	  .row_count = 1,						\
	};								\
      RL_TYPE_NAME * check_type = S_PTR;				\
      rl_save_data_t __rl_save_data__ =					\
	{								\
	  .parent = -1,							\
	  .typed_ptrs_tree = NULL,					\
	  .untyped_ptrs_tree = NULL,					\
	  .rl_ra_idx = { .data = NULL, .size = 0, .alloc_size = 0, .ext = NULL, }, \
	  .ptrs = { .ra = { .data = NULL, .size = 0, .alloc_size = 0, .ext = NULL } } \
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
      rl_rarray_t _ra_ = { .alloc_size = 0, .size = 0, .data = NULL, .ext = NULL }; \
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
      rl_rarray_t _ra_ = { .alloc_size = 0, .size = 0, .data = NULL, .ext = NULL }; \
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
	    .count = sizeof (RL_TYPE_NAME) == 0 ? 0 :                   \
              (sizeof S_PTR + 0) / sizeof (RL_TYPE_NAME),               \
	    .row_count = 1,						\
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

#define RL_LOAD_XDR(RL_TYPE_NAME, XDRS...) RL_LOAD_XDR_ARGN (RL_TYPE_NAME, XDRS, 3, 2)
#define RL_LOAD_XDR_ARGN(RL_TYPE_NAME, XDRS, S_PTR, N, ...) RL_LOAD_XDR_ARG ## N (RL_TYPE_NAME, XDRS, S_PTR)
#define RL_LOAD_XDR_ARG2(RL_TYPE_NAME, XDRS, S_PTR) RL_LOAD_XDR_ARG2_ (RL_TYPE_NAME, XDRS)

#define RL_LOAD_XDR_RA(RL_TYPE_NAME, RA...) RL_LOAD_XDR_RA_ARGN (RL_TYPE_NAME, RA, 3, 2)
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
      rl_ra_rl_ptrdes_t __ptrs__ = { .ra = { .alloc_size = 0, .size = 0, .data = NULL, .ext = NULL } }; \
      rl_fd_t __fd__ = {						\
	.type = #RL_TYPE_NAME,						\
	.name = NULL,							\
	.rl_type = RL_TYPE_DETECT (RL_TYPE_NAME),			\
	.rl_type_ext = RL_TYPE_EXT_NONE,				\
	.size = sizeof (RL_TYPE_NAME),					\
	.count = sizeof (RL_TYPE_NAME) == 0 ? 0 :                       \
          (sizeof S_PTR + 0) / sizeof (RL_TYPE_NAME),                   \
	.row_count = 1,							\
      };								\
      RL_TYPE_NAME * __check_type__ = S_PTR + 0;			\
      rl_td_t * __tdp__ = rl_get_td_by_name (__fd__.type);		\
      if (__tdp__)							\
	__fd__.rl_type = __tdp__->rl_type;				\
      RL_CHECK_TYPES (RL_TYPE_NAME, S_PTR);				\
      if (NULL != __check_type__) 					\
	__idx__ = xml2_load (XML, &__ptrs__);				\
      else								\
	RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_NULL_POINTER);		\
      if (__idx__ >= 0)							\
	__status__ = rl_load (__check_type__, &__fd__, __idx__, &__ptrs__); \
      rl_free_ptrs (&__ptrs__);						\
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

#define RL_LOAD_XML2_NODE(RL_TYPE_NAME, XML...) RL_LOAD_XML2_NODE_ARGN (RL_TYPE_NAME, XML, 3, 2)
#define RL_LOAD_XML2_NODE_ARGN(RL_TYPE_NAME, XML, S_PTR, N, ...) RL_LOAD_XML2_NODE_ARG ## N (RL_TYPE_NAME, XML, S_PTR)
#define RL_LOAD_XML2_NODE_ARG2(RL_TYPE_NAME, XML, S_PTR) RL_LOAD_XML2_NODE_ARG2_ (RL_TYPE_NAME, XML)

#define RL_LOAD_XML2_ARG3(RL_TYPE_NAME, STR, S_PTR) ({			\
      int __status__ = 0;						\
      RL_TYPE_NAME * _check_type_ = S_PTR + 0;				\
      RL_CHECK_TYPES (RL_TYPE_NAME, S_PTR);				\
      if (NULL == _check_type_)						\
	RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_NULL_POINTER);		\
      else								\
	{								\
	  char * __str__ = (STR);					\
	  xmlDocPtr __doc__ = xmlParseMemory (__str__, strlen (__str__)); \
	  if (__doc__)							\
	    {								\
	      xmlNodePtr __node__ = xmlDocGetRootElement (__doc__);	\
	      __status__ = RL_LOAD_XML2_NODE_ARG3 (RL_TYPE_NAME, __node__, _check_type_); \
	      xmlFreeDoc (__doc__);					\
	    }								\
	}								\
      __status__;							\
    })

#define RL_LOAD_XML2_ARG2_(RL_TYPE_NAME, STR) ({			\
      int _status_ = 0;							\
      RL_TYPE_NAME __result__;						\
      memset (&__result__, 0, sizeof (__result__));			\
      _status_ = RL_LOAD_XML2_ARG3 (RL_TYPE_NAME, STR, &__result__);	\
      if (0 == _status_)						\
	RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_LOAD_STRUCT_FAILED);	\
      __result__;							\
    })

#define RL_LOAD_XML2(RL_TYPE_NAME, STR...) RL_LOAD_XML2_N (RL_TYPE_NAME, STR, 3, 2)
#define RL_LOAD_XML2_N(RL_TYPE_NAME, STR, S_PTR, N, ...) RL_LOAD_XML2_ARG ## N (RL_TYPE_NAME, STR, S_PTR)
#define RL_LOAD_XML2_ARG2(RL_TYPE_NAME, STR, S_PTR) RL_LOAD_XML2_ARG2_ (RL_TYPE_NAME, STR)

#endif /* HAVE_LIBXML2 */

#define RL_LOAD_XML2_RA(RL_TYPE_NAME, RA...) RL_LOAD_XML2_RA_ARGN (RL_TYPE_NAME, RA, 3, 2)
#define RL_LOAD_XML2_RA_ARGN(RL_TYPE_NAME, RA, S_PTR, N, ...) RL_LOAD_XML2_ARG ## N (RL_TYPE_NAME, (char*)((RA)->data), S_PTR)

#ifdef HAVE_BISON_FLEX

#define RL_LOAD_METHOD_ARG3(METHOD, RL_TYPE_NAME, STR, S_PTR) ({	\
      int _status_ = 0;							\
      RL_TYPE_NAME * _check_type_ = S_PTR + 0;				\
      RL_CHECK_TYPES (RL_TYPE_NAME, S_PTR);				\
      if (NULL == _check_type_)						\
	RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_NULL_POINTER);		\
      else								\
	{								\
	  char * _str_ = (STR);						\
	  rl_ra_rl_ptrdes_t _ptrs_ = { .ra = { .alloc_size = 0, .size = 0, .data = NULL, .ext = NULL } }; \
	  _status_ = METHOD (_str_, &_ptrs_);				\
	  if (0 == _status_)						\
	    {								\
	      rl_fd_t _fd_ = {						\
		.type = #RL_TYPE_NAME,					\
		.name = NULL,						\
		.rl_type = RL_TYPE_DETECT (RL_TYPE_NAME),		\
		.rl_type_ext = RL_TYPE_EXT_DETECT (RL_TYPE_NAME, S_PTR), \
		.size = sizeof (RL_TYPE_NAME),				\
		.count = sizeof (RL_TYPE_NAME) == 0 ? 0 :               \
                  (sizeof S_PTR + 0) / sizeof (RL_TYPE_NAME),           \
		.row_count = 1,						\
	      };							\
	      rl_td_t * _tdp_ = rl_get_td_by_name (_fd_.type);		\
	      if (_tdp_)						\
		_fd_.rl_type = _tdp_->rl_type;				\
	      _status_ = rl_load (_check_type_, &_fd_, 0, &_ptrs_);	\
	    }								\
	  rl_free_ptrs (&_ptrs_);					\
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

#define RL_LOAD_METHOD(METHOD, RL_TYPE_NAME, STR...) RL_LOAD_METHOD_ARGN (METHOD, RL_TYPE_NAME, STR, 3, 2)
#define RL_LOAD_METHOD_ARGN(METHOD, RL_TYPE_NAME, STR, S_PTR, N, ...) RL_LOAD_METHOD_ARG ## N (METHOD, RL_TYPE_NAME, STR, S_PTR)
#define RL_LOAD_METHOD_ARG2(METHOD, RL_TYPE_NAME, STR, S_PTR) RL_LOAD_METHOD_ARG2_ (METHOD, RL_TYPE_NAME, STR)

#define RL_LOAD_XML1(RL_TYPE_NAME, STR...) RL_LOAD_METHOD (xml1_load, RL_TYPE_NAME, STR)
#define RL_LOAD_CINIT(RL_TYPE_NAME, STR...) RL_LOAD_METHOD (cinit_load, RL_TYPE_NAME, STR)
#define RL_LOAD_JSON RL_LOAD_CINIT
#define RL_LOAD_SCM(RL_TYPE_NAME, STR...) RL_LOAD_METHOD (scm_load, RL_TYPE_NAME, STR)

#define RL_LOAD_METHOD_RA(METHOD, RL_TYPE_NAME, RA...) RL_LOAD_METHOD_RA_ARGN (METHOD, RL_TYPE_NAME, RA, 3, 2)
#define RL_LOAD_METHOD_RA_ARGN(METHOD, RL_TYPE_NAME, RA, S_PTR, N, ...) RL_LOAD_METHOD_ARG ## N (METHOD, RL_TYPE_NAME, (char*)((RA)->data), S_PTR)

#define RL_LOAD_XML1_RA(RL_TYPE_NAME, RA...) RL_LOAD_METHOD_RA (xml1_load, RL_TYPE_NAME, RA)
#define RL_LOAD_CINIT_RA(RL_TYPE_NAME, RA...) RL_LOAD_METHOD_RA (cinit_load, RL_TYPE_NAME, RA)
#define RL_LOAD_JSON_RA RL_LOAD_CINIT_RA
#define RL_LOAD_SCM_RA(RL_TYPE_NAME, RA...) RL_LOAD_METHOD_RA (scm_load, RL_TYPE_NAME, RA)

#else /* ! HAVE_BISON_FLEX */

#define RL_ZERO_RESULT(RL_TYPE_NAME) ({					\
      RL_TYPE_NAME __result__;						\
      memset (&__result__, 0, sizeof (__result__));			\
      RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_LOAD_METHOD_MISSED);		\
      __result__;							\
    })

#define RL_LOAD_STUB(RL_TYPE_NAME, STR...) RL_LOAD_STUB_ARGN (RL_TYPE_NAME, STR, 3, 2)
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

#endif /* _RESLIB_H_ */
