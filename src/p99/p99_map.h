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
#ifndef   	P99_MAP_H_
# define   	P99_MAP_H_

/**
 ** @file
 ** @brief macros to produce lists of statements or declarations.
 ** @see statement_lists
 **/

#include "p99_c99.h"
#include "p99_type.h"
#include "p99_for.h"

/** @addtogroup statement_lists Produce C99 statements or expression lists
 **
 ** This provides tools to produce a list of declarations (e.g @c
 ** typedef), list of initializers or perform a sequence of additions
 ** or other operations. The input usually is just a list of names, e.g.
 **
 ** @{
 **/


#define P00_ACCESSOR(NAME, X, I) (NAME)[I]
#define P00_VASSIGN(NAME, X, I) X = P00_ACCESSOR(NAME, X, I)
#define P00_STRLEN(NAME, X, I) strlen(X)
#define P00_SIZEOF(NAME, X, I) sizeof(X)
#define P00_TYPD(NAME, X, I) typedef X P99_PASTE2(NAME, I)
#define P00_ADD(NAME, I, REC, RES) P99_ADD(RES, REC)

#define P00_STRLENS(N, ...) P99_FOR(,N, P00_SUM, P00_STRLEN, __VA_ARGS__)
#define P00_SIZEOFS(N, ...) P99_FOR(,N, P00_SUM, P00_SIZEOF, __VA_ARGS__)
#define P00_ADDS(N, ...) P99_FOR(, N, P00_ADD, P00_IDT, __VA_ARGS__)

#define P00_POW0(X, _1, _2) (X)
#define P00_POW(X, _1, REC, _3) (X) * REC

/**
 ** @brief Compute the @a N<sup>th</sup> multiplicative integer power of @a X.
 **
 ** @a N must be a decimal constant without suffixes. The value @c 0
 ** is special in that it evaluates to a @c 1 that is promoted to the
 ** promoted type of @a X.
 **/
P00_DOCUMENT_NUMBER_ARGUMENT(P99_IPOW, 0)
P00_DOCUMENT_MULTIPLE_ARGUMENT(P99_IPOW, 1)
#define P99_IPOW(N, X) P99_IF_EQ(N,0)(P99_SIGN_PROMOTE(1, X))((P99_FOR(X, N, P00_POW, P00_POW0, P99_REP(N,))))

/**
 ** @brief Return an expression that returns the sum of the lengths of
 ** all strings that are given as arguments.
 **/
#define P99_STRLENS(...) P00_STRLENS(P99_NARG(__VA_ARGS__),__VA_ARGS__)

/**
 ** @brief Return an expression that returns the sum of the size of
 ** all arguments.
 **/
#define P99_SIZEOFS(...) P00_SIZEOFS(P99_NARG(__VA_ARGS__),__VA_ARGS__)

/**
 ** @brief Return a token that is the sum of all arguments.
 **/
#define P99_ADDS(...) P00_ADDS(P99_NARG(__VA_ARGS__),__VA_ARGS__)

P99_DECLARE_STRUCT(p00_strcat_state);

struct p00_strcat_state {
  char* buffer;
  size_t pos;
};

p99_inline
p00_strcat_state* p00_strcat(p00_strcat_state *restrict dest, char const*src) {
  if (!dest->pos) dest->pos = strlen(dest->buffer);
  size_t len = strlen(src);
  memcpy(dest->buffer + dest->pos, src, len);
  dest->pos += len;
  return dest;
}

p99_inline
char* p00_strcat_terminate(p00_strcat_state *restrict dest) {
  dest->buffer[dest->pos] = '\0';
  return dest->buffer;
}

/**
 ** @brief Append all argument strings after @a TARG to @a TARG.
 **
 ** @a TARG should be compatible with @c char* and must provide enough
 ** space to hold the concatenation of all strings. The remaining
 ** arguments must be compatible with @c const char*.
 **
 ** This macro ensures a linear complexity of the operation. In
 ** particular any position in the argument strings is accessed at
 ** most 2 times, once by a call to strlen and once by a call to
 ** memcpy.
 **
 ** This uses some small inlined helper functions, since the repeated
 ** use of bare @c strcat would have quadratic complexity.
 **
 ** The resulting replacement that is produced by this macro evaluates
 ** each of the arguments at most once.
 **/
#define P99_STRCATS(TARG, ...)                                 \
p00_strcat_terminate                                           \
(P99_BIGFUNC                                                   \
 (p00_strcat,                                                  \
  P99_NARG(TARG, __VA_ARGS__),                                 \
  (&(p00_strcat_state){ .buffer = (TARG), .pos = 0  }),        \
   __VA_ARGS__))

/**
 ** @brief Concatenate all arguments.
 **
 ** All arguments must be strings for which the length can be computed
 ** at compile time.
 **
 ** @return a temporary string that is valid in the containing block
 ** of the expression holding the call to this macro.
 **
 ** @see P99_STRDUP for a variant that returns a @c malloc'ed string and
 ** thus can be called with any type of @c char* arguments.
 **/
#define P99_JOIN(...) P99_STRCATS((char[P99_SIZEOFS(__VA_ARGS__) + 1]){ 0 }, __VA_ARGS__)

/**
 ** @brief Concatenate all arguments.
 **
 ** @return a string that must be freed by @c free
 **
 ** The resulting replacement that is produced by this macro evaluates
 ** each of the arguments twice; once to compute the overall length of
 ** the new string and then for the duplication operation.
 **/
P00_DOCUMENT_MULTIPLE_ARGUMENT(P99_STRDUP, 0)
P00_DOCUMENT_MULTIPLE_ARGUMENT(P99_STRDUP, 1)
P00_DOCUMENT_MULTIPLE_ARGUMENT(P99_STRDUP, 2)
#define P99_STRDUP(...) P99_STRCATS(memset(malloc(P99_STRLENS(__VA_ARGS__) + 1), 0, 1), __VA_ARGS__)

/**
 ** @brief Produce a list of length @a N that has the contents of 0,
 ** 1, , @a N-1
 **/
P00_DOCUMENT_NUMBER_ARGUMENT(P99_POSS, 0)
#define P99_POSS(N) P99_FOR(,N, P00_SEQ, P00_POS,)

/**
 ** Produce a list of length @a N that has the contents of @a X[0], @a
 ** X [1], ,
 ** @a X[@a N-1]
 **/
P00_DOCUMENT_MULTIPLE_ARGUMENT(P99_ACCESSORS, 0)
P00_DOCUMENT_NUMBER_ARGUMENT(P99_ACCESSORS, 1)
#define P99_ACCESSORS(X, N) P99_FOR(X, N, P00_SEQ, P00_ACCESSOR, )


/**
 ** @brief Vector-assign to a list
 **
 ** Produce a list of length @c N that has the contents of @c V0 = @a
 ** NAME[0], @c V1 = @a NAME[1], ..., @c VN-1 = @a NAME[@a N-1], where
 ** V0, etc are the remaining arguments.
 **/
P00_DOCUMENT_MULTIPLE_ARGUMENT(P99_VASSIGNS, 0)
#define P99_VASSIGNS(NAME, ...)                                            \
P99_IF_LT(P99_NARG(__VA_ARGS__),2)                                         \
(P99_IF_VOID(__VA_ARGS__)((void)0)(__VA_ARGS__ = (NAME)[0]))               \
  (P99_FOR(NAME, P00_NARG(__VA_ARGS__),P00_SEP, P00_VASSIGN, __VA_ARGS__))

#define P00_TYPEDEFS(NAME, N, ...)                             \
  P99_IF_VOID(__VA_ARGS__)                                     \
  (P99_MACRO_END(NAME, _eat_the_semicolon_, N))                \
  (P99_FOR(NAME, N, P00_SEP, P00_TYPD, __VA_ARGS__))

/**
 ** @brief Take each argument of the list and transform it into a
 ** @c typedef of name NAME_0, NAME_1, etc.
 **
 ** Because of syntax problems this can't be used for function or
 ** array type derivatives.
 **/
#define P99_TYPEDEFS(NAME, ...)                                \
P00_TYPEDEFS(NAME, P99_NARG(__VA_ARGS__), __VA_ARGS__)



/** @}
 **/

#endif 	    /* !P99_MAP_H_ */
