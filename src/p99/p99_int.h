/* This may look like nonsense, but it really is -*- C -*-                   */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* (C) copyright  2010-2011 Jens Gustedt, INRIA, France                      */
/*                                                                           */
/* This file is free software; it is part of the P99 project.                */
/* You can redistribute it and/or modify it under the terms of the QPL as    */
/* given in the file LICENSE. It is distributed without any warranty;        */
/* without even the implied warranty of merchantability or fitness for a     */
/* particular purpose.                                                       */
/*                                                                           */
#ifndef   	P99_INT_H_
# define   	P99_INT_H_

/**
 ** @file
 ** @brief Macros handling integer types and initialization.
 **
 ** The initialization features in this file might produce many
 ** warnings by your compiler of "missing braces in initializer" or
 ** something similar. This is due to the fact that we have to use a
 ** catch all initializer of the form <code>{ 0 }</code>. Such a
 ** flattened initializer is explicitly covered by the standard but
 ** seems to be considered bad style by some people.
 **
 ** If you would use the features of this file and switch all warnings
 ** on for such a compiler you probably would have a hard time to
 ** detect real and interesting warning between all these initializer
 ** warnings. Therefore we switch such warnings off for compiler that
 ** we know causing such a problem.
 **/

#include "p99_c99.h"
#include "p99_id.h"

#if P99_COMPILER & (P99_COMPILER_CLANG | P99_COMPILER_GNU | P99_COMPILER_OPEN64)
# if P99_GCC_VERSION >= 40200UL
#   pragma GCC diagnostic ignored "-Wmissing-braces"
# endif
#endif

/**
 ** @addtogroup integers Macros to handle integer type expressions
 **
 ** The macros here help to integers more easily, e.g to detect if a
 ** type or expression is signed, what the maximum and minimum values
 ** are etc.
 **
 ** @{
 */

#define P00_DOCUMENT_C2(WIDTH) /*! @brief Combine two integer expressions of half the width into one of width WIDTH*/

#ifdef UINT16_MAX
P00_DOCUMENT_C2(16)
#define P99X_UINT16_C2(H, L) ((((uint16_t)(uint8_t)H) << 8u) | (uint16_t)(uint8_t)L)
#endif
#ifdef INT16_MAX
P00_DOCUMENT_C2(16)
#define P99X_INT16_C2(H, L) ((((int16_t)(int8_t)H) << 8u) | (int16_t)(int8_t)L)
#endif
#ifdef UINT32_MAX
P00_DOCUMENT_C2(32)
#define P99X_UINT32_C2(H, L) ((((uint32_t)(uint16_t)H) << 16u) | (uint32_t)(uint16_t)L)
#endif
#ifdef INT32_MAX
P00_DOCUMENT_C2(32)
#define P99X_INT32_C2(H, L) ((((int32_t)(int16_t)H) << 16u) | (int32_t)(int16_t)L)
#endif
#ifdef UINT64_MAX
P00_DOCUMENT_C2(64)
#define P99X_UINT64_C2(H, L) ((((uint64_t)(uint32_t)H) << 32u) | (uint64_t)(uint32_t)L)
#endif
#ifdef INT64_MAX
P00_DOCUMENT_C2(64)
#define P99X_INT64_C2(H, L) ((((int64_t)(int32_t)H) << 32u) | (int64_t)(int32_t)L)
#endif

#ifdef p99x_uint128
P00_DOCUMENT_C2(128)
#define P99X_UINT128_C2(H, L) ((((p99x_uint128)H) << 64u) | (p99x_uint128)L)
#define P99X_UINT128_0 P99X_UINT128_C2(0x0000000000000000, 0x0000000000000000)
#define P99X_UINT128_MAX P99X_UINT128_C2(0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF)
#endif

#ifdef p99x_int128
P00_DOCUMENT_C2(128)
#define P99X_INT128_C2(H, L) ((((p99x_int128)H) << 64u) | (p99x_int128)L)
#define P99X_INT128_MAX P99X_INT128_C2(0x7FFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF)
#define P99X_INT128_MIN P99X_INT128_C2(0x8000000000000000, 0x0000000000000000)
#endif

/**
 ** @brief An unsigned integer type of maximal width that extends the
 ** C99 specifications.
 **
 ** Typically this might be of width 128 where the whole tool chain,
 ** in particular the preprocessor only supports 64.
 **
 ** This @c typedef is always defined, in the best of all worlds it
 ** coincides with @c uintmax_t. If it is different from that can be
 ** checked for with a macro of the same name.
 ** @see p99x_uint128
 **/
#ifndef p99x_uint128
typedef uintmax_t p99x_uintmax;
#endif

/**
 ** @brief A signed integer type of maximal width that extends the
 ** C99 specifications.
 **
 ** Typically this might be of width 128 where the whole tool chain,
 ** in particular the preprocessor only supports 64.
 **
 ** This @c typedef is always defined, in the best of all worlds it
 ** coincides with @c intmax_t. If it is different from that can be
 ** checked for with a macro of the same name.
 ** @see p99x_int128
 **/
#ifndef p99x_int128
typedef intmax_t p99x_intmax;
#endif

#ifdef P00_DOXYGEN
/**
 ** @brief An unsigned integer type of width 128 that extends the
 ** C99 specifications.
 **
 ** Typically this might be defined when the whole tool chain,
 ** in particular the preprocessor only supports 64 bits.
 **
 ** This @c typedef does not necessary exist. It can be checked for with a
 ** macro of the same name.
 ** @see p99x_uintmax
 **/
typedef extendedInt p99x_uint128;

/**
 ** @brief A signed integer type of width 128 that extends the
 ** C99 specifications.
 **
 ** Typically this might be defined when the whole tool chain,
 ** in particular the preprocessor only supports 64 bits.
 **
 ** This @c typedef does not necessary exist. It can be checked for with a
 ** macro of the same name.
 ** @see p99x_intmax
 **/
typedef extendedInt p99x_int128;
#endif

#ifdef p99x_uintmax

#define P99X__SIGN_PROMOTE(EXPR)                                              \
((p99x_uintmax)P99_SIGN_PROMOTE(P99_UE_MAX(EXPR), (p99x_uintmax)UINTMAX_MAX))

#define P99X__SHIFT(EXPR)                                                      \
((P99_SIGN_PROMOTE(P99_UE_MAX(EXPR), UINTMAX_MAX) > (p99x_uintmax)UINTMAX_MAX) \
 ? 64u                                                                         \
 : 0u)
#endif

#ifndef P99_HIGH2
# if P99_UINTMAX_WIDTH == 64
#  define P99_HIGH2(X)                                         \
((((X) & P00_B0) ? P00_S0 : 0u)                                \
 | (((X) & P00_B1) ? P00_S1 : 0u)                              \
 | (((X) & P00_B2) ? P00_S2 : 0u)                              \
 | (((X) & P00_B3) ? P00_S3 : 0u)                              \
 | (((X) & P00_B4) ? P00_S4 : 0u)                              \
 | (((X) & P00_B5) ? P00_S5 : 0u))
# endif
#endif
#ifndef P99_HIGH2
# if P99_UINTMAX_WIDTH <= 128
#  define P99_HIGH2(X)                                         \
((((X) & P00_B0) ? P00_S0 : 0u)                                \
 | (((X) & P00_B1) ? P00_S1 : 0u)                              \
 | (((X) & P00_B2) ? P00_S2 : 0u)                              \
 | (((X) & P00_B3) ? P00_S3 : 0u)                              \
 | (((X) & P00_B4) ? P00_S4 : 0u)                              \
 | (((X) & P00_B5) ? P00_S5 : 0u)                              \
 | (((X) & P00_B6) ? P00_S6 : 0u))
# endif
#endif

#ifndef P99X__SHIFT
#define P99X__SHIFT(EXPR) 0
#endif

#ifndef P99X__SIGN_PROMOTE
#define P99X__SIGN_PROMOTE(EXPR) 0
#endif

#define P99_HIGH2_1(X) ((X) == P99_UINTMAX_MAX ? P99_UINTMAX_WIDTH : (P99_HIGH2((X) + UINTMAX_C(1))))

/**
 ** @brief Apply the type macro @a MACRO to an unsigned type that is
 ** compatible with type @a T.
 **
 ** The returning expression is of type @c uintmax_t
 **/
P00_DOCUMENT_MACRO_ARGUMENT(P99_TO_UNSIGNED, 1)
#define P99_TO_UNSIGNED(T, MACRO)                              \
((uintmax_t)                                                   \
 (sizeof(T) < sizeof(signed)                                   \
  ? (sizeof(T) == 1u                                           \
     ? MACRO(unsigned char)                                    \
     : MACRO(unsigned short))                                  \
  : (sizeof(T) < sizeof(unsigned long)                         \
     ? MACRO(unsigned)                                         \
     : (sizeof(T) < sizeof(unsigned long long)                 \
        ? MACRO(unsigned long)                                 \
        : MACRO(unsigned long long)))))

/**
 ** @brief Convert -1 to type @a T
 **
 ** If @a T is a signed type a representation of the value -1 should
 ** always exist, since the value 1 also exists.
 **
 ** If @a T is an unsigned type, the converted value is guaranteed to
 ** hold the maximum value that is representable in this type. This is
 ** guaranteed to work regardless of the platform's representation of
 ** negative values: integer conversion @b to an unsigned type is
 ** always defined to be modulo 2^N where N is the precision of the
 ** target type.
 **/
#define P99_M1(T) ((T)-1)

/**
 ** @brief Return an unsigned version of P99_M1.
 **
 ** The returning expression is of type @c uintmax_t
 **/
#define P99_M1U(T) P99_TO_UNSIGNED(T, P99_M1)

#define P00_DOCUMENT_SIGNED(X) /*! @brief Cast the @c int value @c X to type @a T */
#define P00_DOCUMENT_UNSIGNED(X) /*! @brief Cast the @c int value @c X to the unsigned type corresponding to @a T */

P00_DOCUMENT_SIGNED(0)
#define P99_0(T) ((T)0)

P00_DOCUMENT_UNSIGNED(0)
#define P99_0U(T) P99_TO_UNSIGNED(T, P99_0)

P00_DOCUMENT_SIGNED(1)
#define P99_1(T) ((T)1)

P00_DOCUMENT_UNSIGNED(1)
#define P99_1U(T)  P99_TO_UNSIGNED(T, P99_1)

P00_DOCUMENT_SIGNED(2)
#define P99_2(T) ((T)2)

P00_DOCUMENT_UNSIGNED(2)
#define P99_2U(T)  P99_TO_UNSIGNED(T, P99_2)

P00_DOCUMENT_SIGNED(3)
#define P99_3(T) ((T)3)

P00_DOCUMENT_UNSIGNED(3)
#define P99_3U(T)  P99_TO_UNSIGNED(T, P99_3)

/**
 ** @brief The maximum representable value of the unsigned type
 ** corresponding to @a T
 **
 ** The returning expression is of type @c uintmax_t
 **/
#define P99_UT_MAX(T) (P99_M1U(T))

/**
 ** @brief Half of the maximum representable value of the unsigned type
 ** corresponding to @a T
 **
 ** This is the maximum representable value of the signed type
 ** corresponding to @a T.
 **
 ** The returning expression is of type @c uintmax_t.
 **
 ** @see P99_TMAX
 **/
#define P99_UT_MAX1(T) (P99_UT_MAX(T)/2u)

/**
 ** @brief The negative of the half of the maximum representable value
 ** of the unsigned type corresponding to @a T
 **
 ** This is generally not the minimum representable value of the signed type
 ** corresponding to @a T, it might deviate by one from that value.
 **
 ** The returning expression is of type @a T.
 **
 ** @see P99_TMIN
 **/
#define P00_ST_MIN1(T) (-(T)P99_UT_MAX1(T))

/**
 ** @brief Determine if @a T is an unsigned or signed integral type
 **
 ** This works as follows:
 ** - If @a T is signed then -1 in that type is always less than 0 in
 **    that type
 ** - If @a T is unsigned then conversion of -1 to that type gives the
 **    maximally encodable value for that type. This is always greater
 **    than 0 in that type,
 ** - If @a T is @c _Bool or equivalent, -1 converted to it results in
 **    1 and 0 is also mapped to 0. Thus it is detected as unsigned.
 **
 ** @see P99_SIGNED for a similar macro that takes an expression as an
 ** argument
 **/
P00_DOCUMENT_TYPE_ARGUMENT(P99_ISSIGNED, 0)
#define P99_ISSIGNED(T) (P99_M1(T) < P99_0(T))

/**
 ** @brief Compute expression @a A and apply integer promotion rules
 ** under the constraint of expression @a B.
 **
 ** Expression @a A is evaluated exactly once. Expression @a B is only
 ** used for its type and never evaluated.
 **
 ** The result can always be determined at compile time, if @a A can
 ** be determined then, regardless of the actual value of @a B.  But
 ** it is only a constant expression in the strict sense of C99 if
 ** both @a A and @a B are so.
 **
 ** @warning Due to the integer promotion rules the resulting
 ** expression has a width that is at least that of @c int, even if @a
 ** A and @a B have smaller width than that.
 **/
#define P99_SIGN_PROMOTE(A, B) (1 ? (A) : (B))


#define P00_SEE_PROMOTE /*! @see P99_SIGN_PROMOTE for rules about integer sign promotion */

#define P00_DOCUMENT_PROMOTE(X) /*! @brief Promote the integer value @c X under the constraint of expression @a EXPR */

P00_DOCUMENT_PROMOTE(0)
P00_SEE_PROMOTE
#define P99_PROMOTE_0(EXPR) P99_SIGN_PROMOTE(0, (EXPR))

P00_DOCUMENT_PROMOTE(1)
P00_SEE_PROMOTE
#define P99_PROMOTE_1(EXPR) P99_SIGN_PROMOTE(1, (EXPR))

P00_DOCUMENT_PROMOTE(2)
P00_SEE_PROMOTE
#define P99_PROMOTE_2(EXPR) P99_SIGN_PROMOTE(2, (EXPR))

P00_DOCUMENT_PROMOTE(3)
P00_SEE_PROMOTE
#define P99_PROMOTE_3(EXPR) P99_SIGN_PROMOTE(3, (EXPR))

P00_DOCUMENT_PROMOTE(0U)
P00_SEE_PROMOTE
#define P99_PROMOTE_0U(EXPR) P99_SIGN_PROMOTE(0U, (EXPR))

P00_DOCUMENT_PROMOTE(1u)
P00_SEE_PROMOTE
#define P99_PROMOTE_1U(EXPR) P99_SIGN_PROMOTE(1U, (EXPR))

P00_DOCUMENT_PROMOTE(2u)
P00_SEE_PROMOTE
#define P99_PROMOTE_2U(EXPR) P99_SIGN_PROMOTE(2U, (EXPR))


P00_DOCUMENT_PROMOTE(-1)
P00_SEE_PROMOTE
#define P99_PROMOTE_M1(EXPR) P99_SIGN_PROMOTE(-1, (EXPR))

/**
 ** @brief Promote the integer value @c -1 under the constraint of the
 ** unsigned promoted expression @a EXPR
 ** */
P00_SEE_PROMOTE
#define P99_PROMOTE_M1U(EXPR) P99_SIGN_PROMOTE(P99_PROMOTE_M1(EXPR), P99_PROMOTE_0U(EXPR))

/**
 ** @brief The maximum representable value of the unsigned type
 ** promoted with respect to expression @a EXPR.
 **
 ** The returning expression is of unsigned integer type.
 **/
P00_SEE_PROMOTE
#define P99_UE_MAX(EXPR) (P99_PROMOTE_M1U(EXPR))

/**
 ** @brief Half of the maximum representable value of the unsigned type
 ** promoted with respect to expression @a EXPR
 **
 ** This is the maximum representable value of the signed type
 ** promoted with respect to @a EXPR
 **
 ** The returning expression is of unsigned integer type.
 **
 ** @see P99_TMAX
 **/
P00_SEE_PROMOTE
#define P99_UE_MAX1(EXPR) (P99_UE_MAX(EXPR)/P99_PROMOTE_2U(EXPR))


/**
 ** @brief The width of the integral type of expression @a EXPR.
 **
 ** This is the precision plus eventually a sign bit, if the type is
 ** signed.
 **
 ** The resulting expression is evaluated at compile time and maybe
 ** used int constant expressions.
 ** @warning These are not necessarily all bits that are @em used by
 ** the type, there might be padding bits.
 ** @see P99_TWIDTH
 ** @see P99_EPREC
 ** @see P99_EPADDING
 **/
P00_SEE_PROMOTE
#define P99_EWIDTH(EXPR)                                                               \
  (P99X__SHIFT(EXPR)                                                                   \
 ? (P99_HIGH2_1((uintmax_t)(P99_UE_MAX(EXPR)>>P99X__SHIFT(EXPR))) + P99X__SHIFT(EXPR)) \
 : P99_HIGH2_1(P99_UE_MAX(EXPR))                                                       \
 )

/**
 ** @brief The precision, i.e the number of significant bits the
 ** integral type of expression @a EXPR.
 **
 ** The resulting expression is evaluated at compile time and maybe
 ** used int constant expressions.
 ** @warning this is not necessarily the width of @a T
 ** @see P99_TPREC
 ** @see P99_EWIDTH
 ** @see P99_EPADDING
 **/
P00_SEE_PROMOTE
#define P99_EPREC(EXPR) (P99_EWIDTH(EXPR) - P99_SIGNED(EXPR))

/**
 ** @brief The padding bits of the integral type of expression @a EXPR.
 **
 ** These are the bits that are not used for the numerical
 ** representation of the values. On most architectures and for most
 ** types this will be 0. But for e.g for @c _Bool this will be at
 ** least 7.
 **
 ** The resulting expression is evaluated at compile time and maybe
 ** used int constant expressions.
 ** @see P99_TPADDING
 ** @see P99_EWIDTH
 ** @see P99_EPREC
 **/
P00_SEE_PROMOTE
#define P99_EPADDING(EXPR) (sizeof(P99_PROMOTE_0(EXPR))*CHAR_BIT - P99_EWIDTH(EXPR))

#define P99_SE_MAX(EXPR)                                       \
((((P99_PROMOTE_1(EXPR)                                        \
    << (P99_EWIDTH(EXPR) - 2U))                                \
   - P99_PROMOTE_1(EXPR))                                      \
  << 1U)                                                       \
 + P99_PROMOTE_1(EXPR))


/**
 ** @brief The negative of the half of the maximum representable value
 ** of the promoted integral type of expression @a EXPR
 **
 ** This is generally not the minimum representable value of the signed type
 ** corresponding to @a EXPR, it might deviate by one from that value.
 **
 ** The returning expression is of type @a T.
 **
 ** @see P99_TMIN
 **/
#define P00_SE_MIN1(EXPR) (-P99_SE_MAX(EXPR))

/**
 ** @brief Determine if @a EXPR has an unsigned or signed integral type
 **
 ** Note that this macro does not evaluate @a EXPR but uses it only to
 ** determine its signedness. There are no side effects.
 **
 ** The result can always be determined at compile time, but it is not
 ** a constant expression in the strict sense of C99 if @a EXPR is not
 ** so.
 **
 ** Beware, the result of this expression is not the sign of @a EXPR,
 ** but whether or not the type of it could hold a signed value.
 **
 ** @see P99_ISSIGNED for a similar macro that takes a type as an argument
 **/
P00_SEE_PROMOTE
#define P99_SIGNED(EXPR) (P99_PROMOTE_M1(EXPR) < P99_PROMOTE_0(EXPR))


p99_inline
uintmax_t p00_abs_signed(intmax_t a) {
  uintmax_t aa = a;
  /* The minus is taken on the unsigned value, so it gives the
     correct result with -INTMAX_MAX, namely INTMAX_MAX, which might
     not be representable for two's complement representation. */
  return (a < INTMAX_C(0)) ? -aa : a;
}

/**
 ** @brief Compute the absolute value of an integral expression.
 **
 ** @return The return type of this macro is @c uintmax_t. It must be
 ** so, since if we have a two's complement representation the value
 ** @c -INTMAX_MIN is not representable in @c intmax_t.
 **
 ** @a EXPR is guaranteed to be evaluated exactly once.
 **
 ** This macro is preferable to the C99 function @c imaxabs. That
 ** function forcibly returns a value of type @c intmax_t, and might
 ** thus fail for @c INTMAX_MIN.
 **/
#define P99_ABS(EXPR) (P99_SIGNED(EXPR) ? p00_abs_signed(EXPR) : (EXPR))

#if defined(p99x_uintmax) && defined(p99x_intmax)
p99_inline
p99x_uintmax p99x__abs_signed(p99x_intmax a) {
  p99x_uintmax aa = a;
  /* The minus is taken on the unsigned value, so it gives the
     correct result with -INTMAX_MAX, namely INTMAX_MAX, which might
     not be representable for two's complement representation. */
  return (a < (p99x_intmax)0) ? -aa : a;
}
#define P99X_ABS(EXPR) (P99_SIGNED(EXPR) ? p99x__abs_signed(EXPR) : (EXPR))
#endif

#ifndef P99X_ABS
/**
 ** @brief Compute the absolute value of an integral expression, using
 ** integers with extended width.
 **
 ** @return The return type of this macro is p99x_uintmax. It must be
 ** so, since if we have a two's complement representation the minimum
 ** signed value of p99x_intmax is not representable in p99x_intmax.
 **
 ** @a EXPR is guaranteed to be evaluated exactly once.
 **/
# define P99X_ABS P00_ABS
#endif

/**
 ** @brief C99 allows for exactly three different possibilities to
 ** encode negative values of integer types.
 **
 ** @see ::P99_SIGNED_REPRESENTATION
 **/
typedef enum {
  p99_signed_representation_invalid = 0,
  p99_signed_representation_magnitude = 1,
  p99_signed_representation_ones = 2,
  p99_signed_representation_twos = 3,
} p99_signed_representation;

/**
 ** @brief C99 allows for exactly three different possibilities to
 ** encode negative values of integer types.
 **
 ** The representation can be found by looking at the two least
 ** significant bits of -1 represented in @a T.
 ** @see ::p99_signed_representation
 **/
P00_DOCUMENT_TYPE_ARGUMENT(P99_SIGNED_REPRESENTATION, 0)
#define P99_SIGNED_REPRESENTATION(T) ((p99_signed_representation)(P99_M1(T) & P99_3(T)))


/**
 ** @brief If the sign representation of a type is two's complement
 ** the type has no @em negative zero and can thus represent one more
 ** value.
 **/
#define P99_2COMPLEMENT(T) ((T)(P99_SIGNED_REPRESENTATION(T) == p99_signed_representation_twos))

/**
 ** @brief Give the maximum representable value of type @a T
 **/
P00_DOCUMENT_TYPE_ARGUMENT(P99_TMAX, 0)
#define P99_TMAX(T)                                                     \
((T)                                                                    \
 (P99X__SHIFT((T)0)                                                     \
  ? (P99_ISSIGNED(T) ? P99X__SIGN_PROMOTE((T)-1)/2u : P99_UE_MAX((T)0)) \
  : (P99_ISSIGNED(T) ? P99_UT_MAX1(T) : P99_UT_MAX(T))))

/**
 ** @brief Give the minimum representable value of type @a T
 **/
P00_DOCUMENT_TYPE_ARGUMENT(P99_TMIN, 0)
#define P99_TMIN(T)                                                                   \
((T)                                                                                  \
 (P99X__SHIFT((T)0)                                                                   \
  ? (P99_ISSIGNED(T) ? (-(P99X__SIGN_PROMOTE((T)-1)/2u)) - P99_2COMPLEMENT(T) : (T)0) \
  : (P99_ISSIGNED(T) ? (P00_ST_MIN1(T) - P99_2COMPLEMENT(T)) : P99_0(T))))

/**
 ** @brief C99 allows for exactly three different possibilities to
 ** encode negative values of integer types.
 **
 ** The representation can be found by looking at the two least
 ** significant bits of -1 represented in the type of the expression.
 ** @see ::p99_signed_representation
 **/
#define P99_E_REPRESENTATION(EXPR) ((p99_signed_representation)(P99_PROMOTE_M1(EXPR) & P99_PROMOTE_3(EXPR)))


/**
 ** @brief If the sign representation of a type is two's complement
 ** the type has no @em negative zero and can thus represent one more
 ** value.
 **/
#define P99_E_2COMPLEMENT(EXPR)                                                        \
P99_SIGN_PROMOTE(P99_E_REPRESENTATION(EXPR) == p99_signed_representation_twos, (EXPR))

/**
 ** @brief Give the maximum representable value of the type of
 ** expression @a EXPR
 **/
#define P99_EMAX(EXPR) (P99_SIGNED(EXPR) ? P99_SE_MAX(EXPR) : P99_PROMOTE_M1(EXPR))

/**
 ** @brief Give the minimum representable value of the type of the
 ** expression @a EXPR
 **/
#define P99_EMIN(EXPR) (P99_SIGNED(EXPR) ? (P00_SE_MIN1(EXPR) - P99_E_2COMPLEMENT(EXPR)) : P99_PROMOTE_0(EXPR))

/**
 ** @brief The precision, i.e the number of significant bits of integral type
 ** @a T.
 **
 ** The resulting expression is evaluated at compile time and maybe
 ** used int constant expressions.
 ** @warning this is not necessarily the width of @a T
 ** @see P99_TWIDTH
 ** @see P99_TPADDING
 **/
#define P99_TPREC(T)                                           \
(P99X__SHIFT((T)-1)                                            \
 ? P99_EPREC((T)-1)                                            \
 : P99_HIGH2_1(P99_TMAX(T)))

/**
 ** @brief The width of integral type @a T.
 **
 ** This is the precision plus eventually a sign bit, if @a T is signed.
 **
 ** The resulting expression is evaluated at compile time and maybe
 ** used int constant expressions.
 ** @warning These are not necessarily all bits that are @em used by
 ** the type, there might be padding bits.
 ** @see P99_TPREC
 ** @see P99_TPADDING
 **/
#define P99_TWIDTH(T) (P99_TPREC(T) + P99_ISSIGNED(T))

/**
 ** @brief The padding bits of integral type @a T.
 **
 ** These are the bits that are not used for the numerical
 ** representation of the values. On most architectures and for most
 ** types this will be 0. But for e.g for @c _Bool this will be at
 ** least 7.
 **
 ** The resulting expression is evaluated at compile time and maybe
 ** used int constant expressions.
 ** @see P99_TWIDTH
 ** @see P99_TPREC
 **/
#define P99_TPADDING(T) ((sizeof(T)*CHAR_BIT) - P99_TWIDTH(T))

#ifdef P00_DOXYGEN
#define P00_DECLARE_OVERFLOW(SUFF)                                                                                                            \
/*! @brief Cast an unsigned type into a signed one as would do a two's complement representation of the signed type. */                       \
/*! If the signed type is in usual two's complement this should be the identity. For other cases this is supposed to do the best possible. */ \
/*! @warning For bizarrely encoded cases this might result in a negative zero or so. */                                                       \
p99_inline                                                                                                                                    \
P99_BUILTIN_TYPE(SUFF,)                                                                                                                       \
P99_PASTE2(p99_twos, SUFF)(P99_BUILTIN_TYPE(u, SUFF) a);                                                                                      \
p99_inline                                                                                                                                    \
P99_BUILTIN_TYPE(SUFF,)                                                                                                                       \
P99_PASTE2(p99_add, SUFF)(P99_BUILTIN_TYPE(SUFF,) a, P99_BUILTIN_TYPE(SUFF,) b, int* err)
#else
#define P00_DECLARE_OVERFLOW(SUFF)                                                                      \
p99_inline                                                                                              \
P99_BUILTIN_TYPE(SUFF)                                                                                  \
P99_PASTE2(p99_twos, SUFF)(P99_BUILTIN_TYPE(u, SUFF) a) {                                               \
  P99_BUILTIN_TYPE(u, SUFF) const type_max = P99_BUILTIN_MAX(SUFF);                                     \
  P99_BUILTIN_TYPE(u, SUFF) const type_max1 = (P99_BUILTIN_TYPE(u, SUFF))(type_max + 1);                \
  /* the unsigned max, as if it had just one value bit more */                                          \
  P99_BUILTIN_TYPE(u, SUFF) const utype_max = (P99_BUILTIN_TYPE(u, SUFF))((2 * type_max) + 1);          \
  return                                                                                                \
    /* for positive values there is nothing to do, this includes the                                    \
       case where the unsigned type has the same number of value bits                                   \
       as the signed type */                                                                            \
    (a <= type_max)                                                                                     \
    ? a                                                                                                 \
    /* Capture the special case where type_max1 is a trap                                               \
       representation for the signed type */                                                            \
    : (((P99_BUILTIN_MIN(SUFF) == -P99_BUILTIN_MAX(SUFF)) && (a == type_max1))                          \
       ? (P99_BUILTIN_TYPE(SUFF))(-0)                                                                   \
       /* otherwise compute the negative modulo utype_max + 1. for                                      \
          the case that the unsigned type is much wider than the                                        \
          signed type we mask the higher order bits away. */                                            \
       : (P99_BUILTIN_TYPE(SUFF))((-(P99_BUILTIN_TYPE(SUFF))(utype_max - (utype_max & a))) - 1));       \
}                                                                                                       \
p99_inline                                                                                              \
P99_BUILTIN_TYPE(u, SUFF)                                                                               \
P99_PASTE2(p99_unsig, SUFF)(P99_BUILTIN_TYPE(SUFF) a) {                                                 \
  /* needed to avoid spurious compiler warnings */                                                      \
  register P99_BUILTIN_TYPE(SUFF) const type_null = 0;                                                  \
  register P99_BUILTIN_TYPE(u, SUFF) const type_max = P99_BUILTIN_MAX(SUFF);                            \
  register P99_BUILTIN_TYPE(u, SUFF) const type_max1 = (P99_BUILTIN_TYPE(u, SUFF))(type_max + 1);       \
  /* the unsigned max, as if it had just one value bit more */                                          \
  register P99_BUILTIN_TYPE(u, SUFF) const utype_max = (P99_BUILTIN_TYPE(u, SUFF))((2 * type_max) + 1); \
  return                                                                                                \
    a >= type_null                                                                                      \
    ? a                                                                                                 \
    /* Capture the special case where -INTMAX_MIN can not represented                                   \
       in the signed type */                                                                            \
    : (((P99_BUILTIN_MIN(SUFF) < -P99_BUILTIN_MAX(SUFF)) && (a == P99_BUILTIN_MIN(SUFF)))               \
       ? type_max1                                                                                      \
       /* otherwise compute the negative modulo utype_max + 1. */                                       \
       : (P99_BUILTIN_TYPE(u, SUFF))((utype_max - (P99_BUILTIN_TYPE(u, SUFF))-a) + 1));                 \
}                                                                                                       \
p99_inline                                                                                              \
P99_BUILTIN_TYPE(u, SUFF)                                                                               \
P99_PASTE2(p00_add0, SUFF)(P99_BUILTIN_TYPE(SUFF) a, P99_BUILTIN_TYPE(SUFF) b) {                        \
  register P99_BUILTIN_TYPE(u, SUFF) ua = P99_PASTE2(p99_unsig, SUFF)(a);                               \
  register P99_BUILTIN_TYPE(u, SUFF) ub = P99_PASTE2(p99_unsig, SUFF)(b);                               \
  register P99_BUILTIN_TYPE(u, SUFF) res = (P99_BUILTIN_TYPE(u, SUFF))(ua + ub);                        \
  return res;                                                                                           \
}                                                                                                       \
p99_inline                                                                                              \
P99_BUILTIN_TYPE(SUFF)                                                                                  \
P99_PASTE2(p00_add, SUFF)(P99_BUILTIN_TYPE(SUFF) a, P99_BUILTIN_TYPE(SUFF) b, int* err) {               \
  /* needed to avoid spurious compiler warnings */                                                      \
  register P99_BUILTIN_TYPE(SUFF) const type_null = 0;                                                  \
  register _Bool const a0 = (a < type_null);                                                            \
  register _Bool const b0 = (b < type_null);                                                            \
  register P99_BUILTIN_TYPE(u, SUFF) uc                                                                 \
    = P99_PASTE2(p00_add0, SUFF)(a, b);                                                                 \
  register P99_BUILTIN_TYPE(SUFF) c                                                                     \
    = P99_PASTE2(p99_twos, SUFF)(uc);                                                                   \
  register _Bool const c0 = (c < type_null);                                                            \
  if ((a0 == b0) && P99_UNLIKELY(a0 != c0))                                                             \
    if (err) *err = ERANGE;                                                                             \
  return c;                                                                                             \
}                                                                                                       \
p99_inline                                                                                              \
P99_BUILTIN_TYPE(SUFF)                                                                                  \
P99_PASTE2(p99_add, SUFF)(P99_BUILTIN_TYPE(SUFF) a, P99_BUILTIN_TYPE(SUFF) b, int* err) {               \
  /* needed to avoid spurious compiler warnings */                                                      \
  register P99_BUILTIN_TYPE(SUFF) const type_null = 0;                                                  \
  return                                                                                                \
    (P99_BUILTIN_MAX(SUFF) < P99_BUILTIN_MAX(u, SUFF))                                                  \
    ? P99_PASTE2(p00_add, SUFF)(a, b, err)                                                              \
    : ((a >= type_null)                                                                                 \
       ? (P99_BUILTIN_MAX(SUFF) - a < b)                                                                \
       : (P99_BUILTIN_MIN(SUFF) - a > b));                                                              \
}                                                                                                       \
P99_MACRO_END(p99_overflow_, SUFF)
#endif

P00_DECLARE_OVERFLOW(c);
P00_DECLARE_OVERFLOW(hh);
P00_DECLARE_OVERFLOW(h);
P00_DECLARE_OVERFLOW();
P00_DECLARE_OVERFLOW(l);
P00_DECLARE_OVERFLOW(ll);

/**
 ** @brief A catch all 0-initializer
 **
 ** Variables of static storage are initialized as such as if all
 ** components recursively are initialized from 0. Use this macro here
 ** to achieve the same effect for @c auto or @c register variables.
 ** All the following declarations are legal:
 ** @code
 ** unsigned a        = P99_INIT;
 ** unsigned b[2]     = P99_INIT;
 ** unsigned *c       = P99_INIT;
 ** unsigned (*)d[3]  = P99_INIT;
 ** unsigned e[4][5]  = P99_INIT;
 ** @endcode
 ** Here depending on your compiler the last for @c e (and other
 ** deeply nested types) may produce spurious "missing braces"
 ** warnings. These can safely be ignored.
 ** @see P99_LVAL
 ** @see p99_int.h
 **/
#define P99_INIT { 0 }

#define P00_LVAL(T, ...) ((T){ __VA_ARGS__ })

/**
 ** @brief Define an lvalue of type @c T, where @c T is the first
 ** parameter in the variable parameter list.
 **
 ** If only @c T is given as a parameter, @c { 0 } is used to
 ** initialize the lvalue. Otherwise the remaining parameters are used
 ** for initialization.
 **/
#define P99_LVAL(...) P99_IF_LE(P99_NARG(__VA_ARGS__),1)(P00_LVAL(__VA_ARGS__, 0))(P00_LVAL(__VA_ARGS__))


/**
 ** @brief Define an rvalue of type @a T and value @a VAL
 **
 ** In a way this is a "safer" cast operation. E.g
 ** @code
 ** void f(double* x);
 ** void f(double x);
 ** int a = 32;
 **
 ** g((double)a);
 ** g(P99_RVAL(double, a));   // essentially the same as previous
 **
 ** f((double*)&a);           // no warning is produced
 ** f(P99_RVAL(double*, &a)); // warning: assignment from incompatible pointer type
 ** @endcode
 **
 ** @pre @a T must be a type that can be initialized with @c
 ** {0}. These are all the built-in types (integral, floating point,
 ** pointers, enumerations, ...) plus all composite types for which
 ** the first element can be initialized with @c 0.
 **
 ** @pre @a VAL must not necessarily be of type @a T, but it must be
 ** of a type that is assignment compatible with @a T.
 **/
#define P99_RVAL(T, ...) (P99_LVAL(T) = (__VA_ARGS__))

#define P99_CHOOSE5(xT, cc, cs, ci, cl, cll)                   \
((sizeof(xT) < sizeof(int))                                    \
 ? ((sizeof(xT) < sizeof(short))                               \
    ? cc                                                       \
    : cs)                                                      \
 : ((sizeof(xT) <= sizeof(long))                               \
    ? ((sizeof(xT) == sizeof(int))                             \
       ? ci                                                    \
       : cl)                                                   \
    : cll))

#define P99_PRI(xT, F, LEN)                                    \
P99_CHOOSE5(xT,                                                \
            "%" #LEN "hh" #F,                                  \
            "%" #LEN "h" #F,                                   \
            "%" #LEN "" #F,                                    \
            "%" #LEN "l" #F,                                   \
            "%" #LEN "ll" #F)

/**
 ** @brief Promote integer expression @a x to the width of @c
 ** uintmax_t but keep signedness if possible.
 **/
#define P00_J(x) (0 ? P99_0(uintmax_t) : (x))

/**
 ** @addtogroup bitfiddling
 ** @brief Bit fiddling of low order bits
 **
 ** The smart expression that are used in this group are taken as
 ** where summarized on <a
 ** href="http://realtimecollisiondetection.net/blog/?p=78"> Christer
 ** Ericson's blog</a>.
 **
 ** They work because of the special features of unsigned integer
 ** arithmetic that is an arithmetic modulo a power of @c 2.
 **
 ** The notation of the macro and function names has some menomics:
 **
 ** - @c low2 stands for the least significant bit resulting in the
 **   corresponding power of 2, something like <code>1 <<
 **   bitposition</code>
 ** - @c mask stands for a mask that is constructed from this, that is
 **   a sequence of bits that are all one. When @c mask comes after @c
 **   low as in @c low2mask it means that the bit mask follows that
 **   least significant bit. When @c mask comes before @c low as in @c
 **   mask2low it means that the bit mask precedes that least
 **   significant bit.
 ** - The last digit of @c 0 or @c 1 tells if that least significant
 **   bit is cleared or left untouched.
 **
 ** Be careful when using one of the macros in this group:
 ** - they may evaluate their arguments several times
 ** - they @b must be only used with expressions that can be
 **   guaranteed to be unsigned
 **
 ** On the other hand prefer the macro when you known that you have
 ** compile time expressions. Then you may even use them as part of an
 ** @c #if preprocessing conditional.
 **
 ** @{
 **/

/**
 ** @brief extract the least significant bit that is non zero
 **
 ** Example: 01001110 ->  00000010
 **/
#define P99_LOW2(X) ((X) & -(X))

/**
 ** @brief function equivalent to ::P99_LOW2
 **/
p99_inline
uintmax_t p99_low2(uintmax_t x) { return P99_LOW2(x); }

/**
 ** @brief mask consisting of bits strictly below the least
 ** significant bit that is non zero
 **
 ** Example: 10011100 ->  00000011
 **/
#define P99_LOW2MASK1(X) ((X) ^ ((X) - 1))

/**
 ** @brief function equivalent to ::P99_LOW2MASK1
 **/
p99_inline
uintmax_t p99_low2mask1(uintmax_t x) { return P99_LOW2MASK1(x); }

/**
 ** @brief mask consisting of bits below and including the least
 ** significant bit that is non zero
 **
 ** Example: 10011100 ->  00000111
 **/
#define P99_LOW2MASK0(X) (~(X) & ((X) - 1))

/**
 ** @brief function equivalent to ::P99_LOW2MASK0
 **/
p99_inline
uintmax_t p99_low2mask0(uintmax_t x) { return P99_LOW2MASK0(x); }

/**
 ** @brief mask consisting of bits above and including the least
 ** significant bit that is non zero
 **
 ** Example: 10011100 ->  11111100
 **/
#define P99_MASK2LOW1(X) ((X) | -(X))

/**
 ** @brief function equivalent to ::P99_MASK2LOW1
 **/
p99_inline
uintmax_t p99_mask2low1(uintmax_t x) { return P99_MASK2LOW1(x); }

/**
 ** @brief mask consisting of bits strictly above the least
 ** significant bit that is non zero
 **
 ** Example: 10011100 ->  11111000
 **/
#define P99_MASK2LOW0(X) ((X) ^ -(X))

/**
 ** @brief function equivalent to ::P99_MASK2LOW0
 **/
p99_inline
uintmax_t p99_mask2low0(uintmax_t x) { return P99_MASK2LOW0(x); }

/**
 ** @brief clear the least significant bit that is non zero
 **
 ** Example: 01001110 ->  01001100
 **/
#define P99_LOW2CLEAR(X) ((X) & ((X) - 1))

/**
 ** @brief function equivalent to ::P99_LOW2CLEAR
 **/
p99_inline
uintmax_t p99_low2clear(uintmax_t x) { return P99_LOW2CLEAR(x); }

/**
 ** @brief fill all bits below least significant bit that is non zero
 **
 ** Example: 01001100 ->  01001111
 **/
#define P99_LOW2FILL(X) ((X) | ((X) - 1))

/**
 ** @brief function equivalent to ::P99_LOW2FILL
 **/
p99_inline
uintmax_t p99_low2fill(uintmax_t x) { return P99_LOW2FILL(x); }

/**
 ** @brief set least significant zero bit
 **
 ** Example: 01001011 ->  01001111
 **/
#define P99_LOW0SET(X) ((X) | ((X) + 1))

/**
 ** @brief function equivalent to ::P99_LOW0SET
 **/
p99_inline
uintmax_t p99_low0set(uintmax_t x) { return P99_LOW0SET(x); }

/**
 ** @brief shift number right such that the least significant bit
 ** that is non zero ends up at bit position 0
 **
 ** Example: 10011100 ->  00100111
 **/
#define P99_LOW2SHIFT(X) (P99_LIKELY(!!(X)) ? ((X) / ((X) & -(X))) : 0u)

/**
 ** @brief function equivalent to ::P99_LOW2SHIFT
 **/
p99_inline
uintmax_t p99_low2shift(uintmax_t x) { return P99_LOW2SHIFT(x); }

/**
 ** @brief function that returns the next higher value that has
 ** exactly the same popcount as @a x.
 **
 ** That is if @a x has @c K bits set in total, the result will be the
 ** smallest number that also has @c K bits set and that is greater
 ** than @a x.
 **/
p99_inline
uintmax_t p99_next_popcount(uintmax_t x) {
  uintmax_t ret = 0;
  if (x) {
    uintmax_t b = P99_LOW2(x);
    uintmax_t t = x + b;
    uintmax_t c = x ^ t;
    uintmax_t m = (c >> 2) / b;
    ret = t | m;
  }
  return ret;
}

/**
 ** @}
 **/

/**
 ** @}
 **/


#endif 	    /* !P99_INT_H_ */
