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
#ifndef   	P99_NEW_H_
# define   	P99_NEW_H_

#include "p99_c99.h"

/**
 ** @file
 **
 ** @brief Macros for initialization and allocation
 ** @see preprocessor_initialization
 **/

/**
 ** @addtogroup preprocessor_initialization Iniitalization facilities throught the preprocessor
 **
 ** @{
 **/

/**
 ** @brief Zero out all bits in the object that @a X points to.
 **
 ** @a X can be a pointer or an array
 ** @code
 ** T *A;
 ** T B[3];
 ** PZERO(A, 29);
 ** PZERO(B, 2);
 ** @endcode
 **
 ** Here the first call zeroes out 29 elements of type T starting at
 ** the memory where @c A points to. The second zeroes out the first
 ** two elements of the array @c B.
 **
 ** This is a macro not a function, but anyhow @a X is evaluated only
 ** once.
 **/
#define P99_PZERO(X, N) (memset((X), 0, sizeof(X[0]) * N))

/**
 ** @brief Zero out all bits in the object @a X.
 **
 ** @a X must be an object from which the address can be taken, such
 ** as a normal variable or an array.
 **
 ** This is a macro not a function, but anyhow @a X is evaluated only
 ** once.
 **/
#define P99_TZERO(...) (memset(&(__VA_ARGS__), 0, sizeof(__VA_ARGS__)))


/**
 ** @}
 **/


#define P00_NEW(T) P99_PASTE2(T, _init)((T*)malloc(sizeof(T)))

#define P00_NEW_ARGS(T, ...) P99_PASTE2(T, _init)((T*)malloc(sizeof(T)), __VA_ARGS__)


/**
 ** @addtogroup preprocessor_allocation Allocation facilities throught the preprocessor
 **
 ** @{
 **/

/**
 ** @brief Allocate an element of type @c T as given by the first
 ** argument and initialize it with the remaining arguments, if any.
 **
 ** This supposes that the argument list is of the form
 ** @code
 ** T [, OTHER_PARAMETERS]
 ** @endcode
 ** and that a function or macro named @c T_init exists where
 ** @c T_init is just the concatenation of the type name @c T and
 ** `_init'. Thus @c T must be a typename that consists of just one
 ** token: `signed' would work but `signed long' would not.
 **
 ** The @c T_init function must have a signature that is
 ** compatible to one of
 ** @code
 ** T* T_init(T* p);
 ** T* T_init(T*p, OTHER_PARAMETERS);
 ** @endcode
 **
 ** - it must accept a pointer to @c T as a first argument
 ** - it must be robust to the fact of receiving a null pointer through that
 **     first argument (indicating an error condition)
 ** - it should return the @em same pointer @c p
 ** - it may accept additional arguments which may then be passed
 **     through from the call to ::P99_NEW
 **
 ** @return value is a pointer to a newly allocated and initialized object of type
 ** @c T, or @c (T*)0 if the allocation failed.
 **
 ** @see P99_CALL_DEFARG if you want to provide default arguments to
 ** the @c T_init function.
 **/
#define P99_NEW(...) P99_IF_LT(P99_NARG(__VA_ARGS__), 2)(P00_NEW(__VA_ARGS__))(P00_NEW_ARGS(__VA_ARGS__))

/**
 ** @}
 **/

#endif 	    /* !P99_NEW_H_ */
