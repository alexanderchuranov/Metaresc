/* This may look like nonsense, but it really is -*- C -*-                   */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* (C) copyright  2010 Jens Gustedt, INRIA, France                           */
/*                                                                           */
/* This file is free software; it is part of the P99 project.                */
/* You can redistribute it and/or modify it under the terms of the QPL as    */
/* given in the file LICENSE. It is distributed without any warranty;        */
/* without even the implied warranty of merchantability or fitness for a     */
/* particular purpose.                                                       */
/*                                                                           */
/*
** p99_str.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Thu Oct  7 14:57:47 2010 Jens Gustedt
** Last update Thu Oct  7 14:57:47 2010 Jens Gustedt
*/

#ifndef   	P99_STR_H_
# define   	P99_STR_H_

/**
 ** @file
 ** @brief Provide a family of functions @c str2XX that provide a
 ** safer end check with an position in the string instead of the
 ** bogus @c char pointer.
 **/

#include "p99_classification.h"
#include "p99_for.h"

#define P00_DEFINE_STR3(CODE)                                                                 \
p99_inline                                                                                    \
P99_BUILTIN_TYPE(CODE) P99_PASTE2(str2, CODE)(char const* nptr, size_t* pos, unsigned base) { \
  char *endptr;                                                                               \
  P99_BUILTIN_TYPE(CODE) ret = (strto ## CODE)(nptr, (pos ? &endptr : 0), base);              \
  if (pos) *pos = endptr - nptr;                                                              \
  return ret;                                                                                 \
}                                                                                             \
P99_MACRO_END(P00_DEFINE_STR3, CODE)

#define P00_STR3_0(NAME, X, I) P00_DEFINE_STR3(X)
#define P00_STR3_1(...) P99_FOR(, P99_NARG(__VA_ARGS__), P00_SEP, P00_STR3_0, __VA_ARGS__)

#ifndef P00_DOXYGEN
P00_STR3_1(l, ll, ul, ull, uz, t, j, uj);
#endif

P00_C99_DEFARG_DOCU(str2l, long, char const *nptr, size_t *pos, unsigned base)
/*! @param nptr is required */
/*! @param pos defaults to 0, meaning that the successfully parsed string is not kept track */
/*! @param base is 0, numbers are hex if they start with 0x, octal if they start with 0 and decimal otherwise */
#define str2l(...) P99_CALL_DEFARG(str2l, 3, __VA_ARGS__)
#define str2l_defarg_1() 0
#define str2l_defarg_2() 0

P00_C99_DEFARG_DOCU(str2ll, long long, char const *nptr, size_t *pos, unsigned base)
/*! @param nptr is required */
/*! @param pos defaults to 0, meaning that the successfully parsed string is not kept track */
/*! @param base is 0, numbers are hex if they start with 0x, octal if they start with 0 and decimal otherwise */
#define str2ll(...) P99_CALL_DEFARG(str2ll, 3, __VA_ARGS__)
#define str2ll_defarg_1() 0
#define str2ll_defarg_2() 0

P00_C99_DEFARG_DOCU(str2ul, unsigned long, char const *nptr, size_t *pos, unsigned base)
/*! @param nptr is required */
/*! @param pos defaults to 0, meaning that the successfully parsed string is not kept track */
/*! @param base is 0, numbers are hex if they start with 0x, octal if they start with 0 and decimal otherwise */
#define str2ul(...) P99_CALL_DEFARG(str2ul, 3, __VA_ARGS__)
#define str2ul_defarg_1() 0
#define str2ul_defarg_2() 0

P00_C99_DEFARG_DOCU(str2ull, unsigned long long, char const *nptr, size_t *pos, unsigned base)
/*! @param nptr is required */
/*! @param pos defaults to 0, meaning that the successfully parsed string is not kept track */
/*! @param base is 0, numbers are hex if they start with 0x, octal if they start with 0 and decimal otherwise */
#define str2ull(...) P99_CALL_DEFARG(str2ull, 3, __VA_ARGS__)
#define str2ull_defarg_1() 0
#define str2ull_defarg_2() 0

P00_C99_DEFARG_DOCU(str2uz, size_t, char const *nptr, size_t *pos, unsigned base)
/*! @param nptr is required */
/*! @param pos defaults to 0, meaning that the successfully parsed string is not kept track */
/*! @param base is 0, numbers are hex if they start with 0x, octal if they start with 0 and decimal otherwise */
#define str2uz(...) P99_CALL_DEFARG(str2uz, 3, __VA_ARGS__)
#define str2uz_defarg_1() 0
#define str2uz_defarg_2() 0

P00_C99_DEFARG_DOCU(str2t, ptrdiff_t, char const *nptr, size_t *pos, unsigned base)
/*! @param nptr is required */
/*! @param pos defaults to 0, meaning that the successfully parsed string is not kept track */
/*! @param base is 0, numbers are hex if they start with 0x, octal if they start with 0 and decimal otherwise */
#define str2t(...) P99_CALL_DEFARG(str2t, 3, __VA_ARGS__)
#define str2t_defarg_1() 0
#define str2t_defarg_2() 0

P00_C99_DEFARG_DOCU(str2j, intmax_t, char const *nptr, size_t *pos, unsigned base)
/*! @param nptr is required */
/*! @param pos defaults to 0, meaning that the successfully parsed string is not kept track */
/*! @param base is 0, numbers are hex if they start with 0x, octal if they start with 0 and decimal otherwise */
#define str2j(...) P99_CALL_DEFARG(str2j, 3, __VA_ARGS__)
#define str2j_defarg_1() 0
#define str2j_defarg_2() 0

P00_C99_DEFARG_DOCU(str2uj, uintmax_t, char const *nptr, size_t *pos, unsigned base)
/*! @param nptr is required */
/*! @param pos defaults to 0, meaning that the successfully parsed string is not kept track */
/*! @param base is 0, numbers are hex if they start with 0x, octal if they start with 0 and decimal otherwise */
#define str2uj(...) P99_CALL_DEFARG(str2uj, 3, __VA_ARGS__)
#define str2uj_defarg_1() 0
#define str2uj_defarg_2() 0

#define P00_DEFINE_STR2(CODE)                                                  \
p99_inline                                                                     \
P99_BUILTIN_TYPE(CODE) P99_PASTE2(str2, CODE)(char const* nptr, size_t* pos) { \
  char *endptr;                                                                \
  P99_BUILTIN_TYPE(CODE) ret = (strto ## CODE)(nptr, (pos ? &endptr : 0));     \
  if (pos) *pos = endptr - nptr;                                               \
  return ret;                                                                  \
}                                                                              \
P99_MACRO_END(P00_DEFINE_STR2, CODE)

#define P00_STR2_0(NAME, X, I) P00_DEFINE_STR2(X)
#define P00_STR2_1(...) P99_FOR(, P99_NARG(__VA_ARGS__), P00_SEP, P00_STR2_0, __VA_ARGS__)

P00_STR2_1(f, d, ld);

P00_C99_DEFARG_DOCU(str2f, float, char const *nptr, size_t *pos)
/*! @param nptr is required */
/*! @param pos defaults to 0, meaning that the successfully parsed string is not kept track */
#define str2f(...) P99_CALL_DEFARG(str2f, 2, __VA_ARGS__)
#define str2f_defarg_1() 0

P00_C99_DEFARG_DOCU(str2d, double, char const *nptr, size_t *pos)
/*! @param nptr is required */
/*! @param pos defaults to 0, meaning that the successfully parsed string is not kept track */
#define str2d(...) P99_CALL_DEFARG(str2d, 2, __VA_ARGS__)
#define str2d_defarg_1() 0

P00_C99_DEFARG_DOCU(str2ld, long double, char const *nptr, size_t *pos)
/*! @param nptr is required */
/*! @param pos defaults to 0, meaning that the successfully parsed string is not kept track */
#define str2ld(...) P99_CALL_DEFARG(str2ld, 2, __VA_ARGS__)
#define str2ld_defarg_1() 0



#endif 	    /* !P99_STR_H_ */
