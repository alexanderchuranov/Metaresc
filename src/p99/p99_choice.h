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
#ifndef   	P99_CHOICE_H_
# define   	P99_CHOICE_H_

/**
 ** @file
 ** @brief Choice macros that implement `static inline' table lookup.
 **/

#include "p99_c99.h"
#include "p99_id.h"
#include "p99_for.h"

#define P00_CASE_RETURN(NAME, X, I) case I: return X

#define P99_CASE_RETURN(...) P99_FOR(,P99_NARG(__VA_ARGS__), P00_SEP, P00_CASE_RETURN, __VA_ARGS__)

#define P99_CHOICE_FUNCTION(TYPE, NAME, DEFAULT, ...)                                \
/*! This is a function implementing a ``@c static @c inline'' case table          */ \
/*! Default value if @a x is out of range is DEFAULT.                             */ \
/*! The table values are __VA_ARGS__.                                             */ \
/*! @param x indicates the value to choose, it must be compatible with @c size_t. */ \
p99_inline                                                                           \
TYPE NAME(size_t x) {                                                                \
  switch (x) {                                                                       \
    P99_CASE_RETURN(__VA_ARGS__);                                                    \
  default: return DEFAULT;                                                           \
  }                                                                                  \
}                                                                                    \
P99_MACRO_END(_choice_function_, NAME)

/** @brief Return a small prime number or 0 if @a x is too big **/
P99_CHOICE_FUNCTION(uint8_t, p99_small_primes, 0,
                    2, 3, 5, 7, 11, 13, 17, 19, 23, 31, 37, 41, 43, 47,
                    53, 57, 59, 61, 67, 71);


#define P00_UNIQUE_BIT_MULT_3 0x7C
#define P00_UNIQUE_BIT_MULT_4 0x13F1
#define P00_UNIQUE_BIT_MULT_5 0x077CB531
#define P00_UNIQUE_BIT_MULT_6 0X022FDD63CC95386D

#define P00_UNIQUE_BIT_MULT_(WIDTH, MULT) P99_PASTE3(UINT, WIDTH, _C)(MULT)
#define P00_UNIQUE_BIT_MULT(BITS, WIDTH) P00_UNIQUE_BIT_MULT_(WIDTH, P99_PASTE2(P00_UNIQUE_BIT_MULT_, BITS))

#define P00_UNIQUE_BIT__(BIT, BITS, WIDTH, MULT)               \
(((P99_PASTE3(UINT, WIDTH, _C)(1) << BIT)                      \
  * MULT)                                                      \
 >> (WIDTH - BITS))

#define P00_UNIQUE_BIT_(BIT, BITS, WIDTH)                            \
P00_UNIQUE_BIT__(BIT, BITS, WIDTH, P00_UNIQUE_BIT_MULT(BITS, WIDTH))


#define P00_UNIQUE_BIT_RETURN(NAME, X, I) case P00_UNIQUE_BIT_(I, X, NAME): return I


#ifdef P00_DOXYGEN
/* doxygen can't handle the P99_FOR */
#define P00_UNIQUE_BIT_FUNCTION(TYPE, NAME, DEFAULT, BITS, WIDTH) \
p99_inline                                                        \
TYPE P99_PASTE2(NAME, BITS)(size_t x)
#else
#define P00_UNIQUE_BIT_FUNCTION(TYPE, NAME, DEFAULT, BITS, WIDTH)                 \
p99_inline                                                                        \
TYPE P99_PASTE2(NAME, BITS)(size_t x) {                                           \
  switch (x) {                                                                    \
    P99_FOR(WIDTH, WIDTH, P00_SEP, P00_UNIQUE_BIT_RETURN, P99_DUPL(WIDTH, BITS)); \
  default: return DEFAULT;                                                        \
  }                                                                               \
}                                                                                 \
P99_MACRO_END(_unique_bit)
#endif

P00_UNIQUE_BIT_FUNCTION(unsigned, p00_unique_bit_hash_, -1, 3, 8);
P00_UNIQUE_BIT_FUNCTION(unsigned, p00_unique_bit_hash_, -1, 4, 16);
P00_UNIQUE_BIT_FUNCTION(unsigned, p00_unique_bit_hash_, -1, 5, 32);
P00_UNIQUE_BIT_FUNCTION(unsigned, p00_unique_bit_hash_, -1, 6, 64);


/**
 ** The ideas for this where taken from "R's" solution to this
 ** question on stackoverflow:
 ** http://stackoverflow.com/questions/3465098/bit-twiddling-which-bit-is-set
 **/
#define P00_UNIQUE_BIT(BITS, WIDTH)                                                  \
/*! @brief Find the one unique bit that is set in @a x                     */        \
/*! @warning this function doesn't check if the precondition is fulfilled. */        \
/*! @see p99_unique_bit_checked ## WIDTH                                   */        \
p99_inline                                                                           \
 unsigned P99_PASTE2(p99_unique_bit_, WIDTH)(P99_PASTE3(uint, WIDTH, _t) x) {        \
  /* the index now only has BITS significant bits, so the default case of            \
     P99_PASTE2(p00_unique_bit_hash_, BITS) will never trigger.*/                    \
  return                                                                             \
    P99_PASTE2(p00_unique_bit_hash_, BITS)                                           \
    ((x * P00_UNIQUE_BIT_MULT(BITS, WIDTH))                                          \
     >> (WIDTH - BITS));                                                             \
}                                                                                    \
/*! @brief Find the one unique bit that is set in @a x                 */            \
/*! if @a x has 0 or more than one bits set this returns a big number. */            \
/*! @see p99_unique_bit_ ## WIDTH                                      */            \
p99_inline                                                                           \
unsigned P99_PASTE2(p99_unique_bit_checked_, WIDTH)(P99_PASTE3(uint, WIDTH, _t) x) { \
  unsigned ret = P99_PASTE2(p99_unique_bit_, WIDTH)(x);                              \
  return ((P99_PASTE3(UINT, WIDTH, _C)(1) << ret) == x) ? ret : -1;                  \
}                                                                                    \
P99_MACRO_END(_unique_bit)

P00_UNIQUE_BIT(3, 8);
P00_UNIQUE_BIT(4, 16);
P00_UNIQUE_BIT(5, 32);
P00_UNIQUE_BIT(6, 64);


#endif 	    /* !P99_CHOICE_H_ */
