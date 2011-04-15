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
#ifndef   	P99_ENUM_H_
# define   	P99_ENUM_H_

#include "p99_for.h"

/**
 ** @addtogroup types Type facilities
 ** @{
 **/

#define P00_ENUM_CASE(NAME, X, I) case X: return P99_STRINGIFY(X)

#ifdef P00_DOXYGEN
/**
 ** @brief Declare a simple inline function to return strings
 ** containing the names of enumeration constants.
 **/
#define P99_DECLARE_ENUM_GETNAME(T, ...)                            \
/*! @brief Get a string with the name of constant @a x of type T */ \
inline char const* P99_PASTE2(T, _getname)(T x)...
#else
P00_DOCUMENT_TYPE_ARGUMENT(P99_DECLARE_ENUM_GETNAME, 0)
P00_DOCUMENT_DECLARATION_ARGUMENT(P99_DECLARE_ENUM_GETNAME, 1)
P00_DOCUMENT_DECLARATION_ARGUMENT(P99_DECLARE_ENUM_GETNAME, 2)
P00_DOCUMENT_DECLARATION_ARGUMENT(P99_DECLARE_ENUM_GETNAME, 3)
#define P99_DECLARE_ENUM_GETNAME(T, ...)                                   \
p99_inline                                                                 \
char const* P99_PASTE2(T, _getname)(T x) {                                 \
  switch ((uintmax_t)x) {                                                  \
    P99_FOR(, P99_NARG(__VA_ARGS__), P00_SEP, P00_ENUM_CASE, __VA_ARGS__); \
  default: return "((" #T ")unknown value)";                               \
  }                                                                        \
}                                                                          \
P99_MACRO_END(declare_enum_getname, T)
#endif

/**
 ** @brief Declare a simple enumeration type.
 **
 ** This macro only works for enumeration types that have no
 ** assignments to the constants. To define an enumeration type @c
 ** color Use it in something as
 ** @code
 ** P99_DECLARE_ENUM(color, red, green, blue);
 ** @endcode
 **
 ** As additional benefits you obtain:
 **
 ** - constants @c color_min and @c color_max that in this example
 **   here would correspond to @c red and @c blue, respectively
 ** - a function @c color_getname that returns a string containing the
 **   name of its argument.
 **
 ** To have this functional, you have to put an line
 ** @code
 ** P99_DEFINE_ENUM(color);
 ** @endcode
 **
 ** in one of your object files.
 **
 ** Unfortunately you can't use doxygen like comments inside the
 ** argument list to document the enumeration constants. You have to
 ** document the constants separately. For the above example you would
 ** do
 ** @code
 **   /// @var color red
 **   /// the most redish color of all colors
 ** @endcode
 ** Observe the bizarre naming convention, here. Although in C (and
 ** C++) `red' is declared in the same scope as `enum color' you have
 ** to prefix it with `color' such that the documentation lands inside
 ** the one for `color'.
 **/
P00_DOCUMENT_TYPE_ARGUMENT(P99_DECLARE_ENUM, 0)
P00_DOCUMENT_DECLARATION_ARGUMENT(P99_DECLARE_ENUM, 1)
P00_DOCUMENT_DECLARATION_ARGUMENT(P99_DECLARE_ENUM, 2)
P00_DOCUMENT_DECLARATION_ARGUMENT(P99_DECLARE_ENUM, 3)
#define P99_DECLARE_ENUM(T, ...)                                                   \
/*! @see P99_DECLARE_ENUM was used for the declaration of this enumeration type */ \
/*! @see T ## _getname for access to the names of the constants as strings */      \
typedef enum T { __VA_ARGS__ ,                                                     \
               /*! upper bound of the @ref T constants */                          \
               P99_PASTE2(T, _amount),                                             \
               /*! the largest @ref T constant */                                  \
               P99_PASTE2(T, _max) = ((size_t)(P99_PASTE2(T, _amount)) - 1u),      \
               /*! the smallest @ref T constant */                                 \
               P99_PASTE2(T, _min) = 0                                             \
} T;                                                                               \
P99_DECLARE_ENUM_GETNAME(T, __VA_ARGS__);                                          \
P99_MACRO_END(declare_enum_, T)

/**
 ** @brief Define the necessary symbols for a simple enumeration type.
 **
 ** Use this with P99_DECLARE_ENUM(), which see.
 **/
#define P99_DEFINE_ENUM(T) P99_INSTANTIATE(char const*, P99_PASTE2(T, _getname), T)

P99_DECLARE_ENUM_GETNAME(bool, false, true);

/** @}
 **/


#endif 	    /* !P99_ENUM_H_ */
