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
/*
** p99_type.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sat Nov 20 09:09:33 2010 Jens Gustedt
** Last update Sat Nov 20 09:09:33 2010 Jens Gustedt
*/

#ifndef   	P99_TYPE_H_
# define   	P99_TYPE_H_

#include "p99_paste.h"

/**
 ** @addtogroup types Type facilities
 ** @brief Macros that help you handle different kind of types,
 ** forward declaring @c struct of @c union, and handle @c enum.
 ** @{
 **/


/**
 ** @brief forward declaration of a @c struct @a NAME
 **
 ** This combines a @c struct forward declaration and a @c typedef
 ** that should allow you to use @c struct @a NAME and just @a NAME
 ** interchangeably.
 **
 ** This declaration should also be suitable to be included into a C++
 ** source.
 **/
P00_DOCUMENT_TYPE_ARGUMENT(P99_DECLARE_STRUCT, 0)
#define P99_DECLARE_STRUCT(NAME) typedef struct NAME NAME


/**
 ** @brief forward declaration of a @c union @a NAME
 **
 ** This combines a @c union forward declaration and a @c typedef
 ** that should allow you to use @c union @a NAME and just @a NAME
 ** interchangeably.
 **
 ** This declaration should also be suitable to be included into a C++
 ** source.
 **/
P00_DOCUMENT_TYPE_ARGUMENT(P99_DECLARE_UNION, 0)
#define P99_DECLARE_UNION(NAME) typedef union NAME NAME

/** @}
 **/

#endif 	    /* !P99_TYPE_H_ */
