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
#ifndef   	P99_PASTE_H_
# define   	P99_PASTE_H_

/**
 ** @file
 ** @brief macros that paste tokens in a predictive way
 ** @see P99_PASTE
 **/

#include "p99_generated.h"

/** @addtogroup preprocessor_text Preprocessor text and token manipulation
 ** @{
 **/


/* This was inspired by BOOT's PP_CAT macro. Using such a thing avoid
   to define multiple levels of expansion for each macro. */
#define P99_PASTE0()
#define P99_PASTE1(_1) _1

/**
 ** @brief Paste two token sequences at their junction.
 **
 ** This macro does the paste operation first, and then an evaluation
 ** of the result. Thus
 ** @code
 ** #define B(x) toto(x)
 ** P99_CAT2(A, B(6));
 ** @endcode
 ** will result in
 ** @code
 ** AB(6);
 ** @endcode
 ** and not in
 ** @code
 ** Atoto(6)
 ** @endcode
 **/
#define P99_CAT2(_1, _2) _1 ## _2

/**
 ** @brief Paste two token sequences at their junction.
 **
 ** This macro does the evaluation of the arguments first and then
 ** proceeds at the concatenation
 ** of the results. Thus
 ** @code
 ** #define B(x) toto(x)
 ** P99_CAT2(A, B(6));
 ** @endcode
 ** will result in
 ** @code
 ** Atoto(6)
 ** @endcode
 ** and not in
 ** @code
 ** AB(6);
 ** @endcode
 **/
#define P99_PASTE2(_1, _2) P99_CAT2(_1, _2)
#define P99_PASTE3(_1,	_2,	_3)                                 \
	P99_PASTE2(P99_PASTE2(_1,	_2), _3)
#define P99_PASTE4(_1,	_2,	_3,	_4)                             \
	P99_PASTE2(P99_PASTE3(_1,	_2,	_3), _4)
#define P99_PASTE5(_1,	_2,	_3,	_4,	_5)                         \
	P99_PASTE2(P99_PASTE4(_1,	_2,	_3,	_4), _5)
#define P99_PASTE6(_1,	_2,	_3,	_4,	_5,	_6)                     \
	P99_PASTE2(P99_PASTE5(_1,	_2,	_3,	_4,	_5), _6)

#ifndef P00_DOCUMENT_MULTIPLE_ARGUMENT
#define P00_DOCUMENT_TYPE_ARGUMENT(MACRO, N) /*! @remark argument N of MACRO should correspond to a type */
#define P00_DOCUMENT_MULTIPLE_ARGUMENT(MACRO, N) /*! @warning argument N of MACRO may be evaluated multiple times */
#define P00_DOCUMENT_PERMITTED_ARGUMENT(MACRO, N)
#define P00_DOCUMENT_STATEMENT_ARGUMENT(MACRO, N)
#define P00_DOCUMENT_DECLARATION_ARGUMENT(MACRO, N)
#define P00_DOCUMENT_MACRO_ARGUMENT(MACRO, N) /*! @remark argument N of MACRO should correspond to a macro name */
#define P00_DOCUMENT_NUMBER_ARGUMENT(MACRO, N) /*! @remark argument N of MACRO must expand to a decimal number */
#endif

/**
 ** @}
 **/


#endif 	    /* !P99_PASTE_H_ */
