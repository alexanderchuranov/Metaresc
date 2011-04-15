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
#ifndef   	P99_FOR_H_
# define   	P99_FOR_H_

/**
 ** @file
 ** @brief A prepprocessor for loop implementation and some derived
 ** list handling macros.
 ** @see P99_FOR
 **/

#include "p99_block.h"



/** @addtogroup preprocessor_for Preprocessor FOR loops
 ** @{
 **/

#define P00_FOR0(NAME, OP, FUNC, ...)
#define P00_FOR1(NAME, OP, FUNC, ...) FUNC(NAME, P00_PRE1(__VA_ARGS__,), 0)

/**
 ** @brief A preprocessor pseudo iterator.
 **
 ** @param NAME a name or other data that is visible to all iterations
 ** @param N the number of times the iteration is to be done
 ** @param OP an operation that is performed to glue the different
 **        results of the individual iterations together
 ** @param FUNC a function that is applied in each iteration
 **
 ** In each iteration, @a FUNC will be called as @c FUNC(NAME, X, I).
 ** Here @a NAME is the same as the argument to ::P99_FOR. @c I is the
 ** ordinal number of this iteration step, starting from 0. @c X is
 ** the @c Ith argument from the remaining argument list of this
 ** invocation of ::P99_FOR.
 **
 ** @a OP is called for iterations 1 to @a N - 1 to glue the results
 ** of the @a N iterations together. It is called in the form @a
 ** OP(NAME, I, REC, RES), where @a NAME and @c I are as before. @c
 ** REC is the result of all iterations with index less than @c I, @c
 ** RES is the result of the current iteration.
 **
 ** @code
 ** #define P00_SEP(NAME, I, REC, RES) REC; RES
 ** #define P00_VASSIGN(NAME, X, I) X = (NAME)[I]
 **
 ** P99_FOR(A, 2, P00_SEQ, P00_VASSIGN, toto, tutu);
 ** @endcode
 **
 ** Will result in
 ** @code
 ** toto = (A)[0]; tutu = (A)[1];
 ** @endcode
 **
 ** To understand the associativity of the @a OP argument 
 ** @code
 ** #define P00_SUM(NAME, I, REC, RES) ((REC) + (RES))
 ** #define P00_IDT(NAME, X, I) X
 **
 ** P99_FOR(A, 2, P00_SUM, P00_IDT, a, b, c)
 ** @endcode
 ** Will result in
 ** @code
 ** ((((a) + (b))) + (c))
 ** @endcode
 **/
P00_DOCUMENT_MULTIPLE_ARGUMENT(P99_FOR, 0)
P00_DOCUMENT_NUMBER_ARGUMENT(P99_FOR, 1)
P00_DOCUMENT_MACRO_ARGUMENT(P99_FOR, 3)
#define P99_FOR(NAME, N, OP, FUNC, ...) P99_PASTE2(P00_FOR, N)(NAME, OP, FUNC, __VA_ARGS__)

#define P00_IGN(NAME, X, I)
#define P00_IDT(NAME, X, I) X
#define P00_POS(NAME, X, I) I
#define P00_NAM(NAME, X, I) NAME
#define P00_NAME_I(NAME, X, I) P99_PASTE2(NAME, I)
#define P00_STR(NAME, X, I) P99_STRINGIFY(X)

#define P00_SUM(NAME, I, X, Y) ((X) + (Y))
#define P00_PROD(NAME, I, X, Y) ((X) * (Y))
#define P00_QUOT(NAME, I, X, Y) ((X) / (Y))
#define P00_XOR(NAME, I, X, Y) ((X) ^ (Y))
#define P00_BOR(NAME, I, X, Y) ((X) | (Y))
#define P00_BAND(NAME, I, X, Y) ((X) & (Y))
#define P00_OR(NAME, I, X, Y) ((X) || (Y))
#define P00_AND(NAME, I, X, Y) ((X) && (Y))

#define P00_SEQ(NAME, I, REC, X) REC, X
#define P00_SEP(NAME, I, REC, X) REC; X
#define P00_SER(NAME, I, REC, X) REC X
#define P00_REV(NAME, I, REC, X) X, REC

/**
 ** @brief generate lists of names of the form <code>NAME0, NAME1, ...</code>
 **/
P00_DOCUMENT_NUMBER_ARGUMENT(P99_NAME, 0)
#define P99_NAME(N, NAME) P99_FOR(NAME, N, P00_SEQ, P00_NAME_I, P99_REP(N,))

#define P00_FUNC(NAME, I, REC, X) NAME(REC, X)

/**
 ** @brief Realize the right associative operation @a OP of all the arguments.
 **
 ** Here @a OP should receive four arguments <code>NAME, X, N,
 ** REC</code> out of which @c NAME and @c N are ignored and @c X and
 ** @c REC should be interpreted as the left and right hand of the
 ** operator action, respectively.
 **
 ** @a M is the length of the list that follows it.
 **/
P00_DOCUMENT_NUMBER_ARGUMENT(P99_BIGOP, 1)
#define P99_BIGOP(OP, M, ...) P99_FOR( , M, OP, P00_IDT, __VA_ARGS__)

/**
 ** @brief Realize the right associative call of binary function @a FUNC of all the arguments.
 **
 ** Here @a FUNC should be a function of two arguments.
 **
 ** @a M is the length of the list that follows it.
 **/
P00_DOCUMENT_NUMBER_ARGUMENT(P99_BIGFUNC, 1)
#define P99_BIGFUNC(FUNC, M, ...) P99_FOR(FUNC, M, P00_FUNC, P00_IDT, __VA_ARGS__)

/**
 ** @}
 **/



/** @addtogroup statement_lists
 ** @{
 **/

/**
 ** @brief Realize the right associative sum of all the arguments.
 **/
#define P99_SUMS(...) P99_BIGOP(P00_SUM, P99_NARG(__VA_ARGS__),__VA_ARGS__)
/**
 ** @brief Realize the right associative product of all the arguments.
 **/
#define P99_PRODS(...) P99_BIGOP(P00_PROD, P99_NARG(__VA_ARGS__),__VA_ARGS__)
/**
 ** @brief Realize the right associative quotient of all the arguments.
 **/
#define P99_QUOTS(...) P99_BIGOP(P00_QUOT, P99_NARG(__VA_ARGS__),__VA_ARGS__)
/**
 ** @brief Realize the right associative bitwise exclusive or of all the arguments.
 **/
#define P99_XORS(...) P99_BIGOP(P00_XOR, P99_NARG(__VA_ARGS__),__VA_ARGS__)
/**
 ** @brief Realize the right associative bitwise or of all the arguments.
 **/
#define P99_BORS(...) P99_BIGOP(P00_BOR, P99_NARG(__VA_ARGS__),__VA_ARGS__)
/**
 ** @brief Realize the right associative bitwise and of all the arguments.
 **/
#define P99_BANDS(...) P99_BIGOP(P00_BAND, P99_NARG(__VA_ARGS__),__VA_ARGS__)
/**
 ** @brief Realize the right associative logical or of all the arguments.
 **/
#define P99_ORS(...) P99_BIGOP(P00_OR, P99_NARG(__VA_ARGS__),__VA_ARGS__)
/**
 ** @brief Realize the right associative logical and of all the arguments.
 **/
#define P99_ANDS(...) P99_BIGOP(P00_AND, P99_NARG(__VA_ARGS__),__VA_ARGS__)

/**
 ** @}
 **/


/**
 ** @ingroup preprocessor_for
 ** @brief Revert the argument list
 **/
#define P99_REVS(...) P00_REVS(P99_NARG(__VA_ARGS__),__VA_ARGS__)

#define P00_REVS(N, ...) P99_PASTE2(P00_REVS_, P99_IS_LT(N, 2))(N, __VA_ARGS__)

#define P00_REVS_0(N, ...) P00_REVS_(N,__VA_ARGS__)
#define P00_REVS_1(N, ...) __VA_ARGS__

/* the general case for an argument list of at least two elements */
#define P00_REVS_(N, ...) P99_FOR(,N, P00_REV, P00_IDT, __VA_ARGS__)

/**
 ** @ingroup preprocessor_arithmetic
 ** @brief Generate the quotient of non-negative decimal numbers @a A and @a B at
 ** preprocessing time.
 **
 ** @warning Both arguments must be less than the maximum argument list number that
 ** is supported, currently 64.
 **/
#define P99_DIV(A, B) P99_CHS(A, P99_FOR(B, 32, P00_SEQ, P00_IDI, P00_ALL_ONES()))

#define P00_IDI(B, X, I) P99_DUPL(B, I)


#define P00_CDIM_OP(NAME, I, REC, X) (X + ((NAME)[I] * REC))
#define P00_CDIM_FUNC(NAME, X, I) (X)
#define P00_CDIM(N, NAME, ...) P99_FOR(NAME, N, P00_CDIM_OP, P00_CDIM_FUNC, __VA_ARGS__)

/**
 ** @ingroup preprocessor_arithmetic
 ** @brief Compute an absolute index in a multidimensional array with
 ** the same function as C would do that.
 **
 ** If we have @c N arguments after @a NAME, @a NAME must be an array
 ** of type
 ** @code
 ** size_t NAME[N];
 ** @endcode
 ** that hold the side lengths of an @c N dimensional cube. E.g
 ** @code
 ** size_t const D[] = { 3, 4, 7 };
 ** @endcode
 ** corresponds to a cube of side length 3 in dimension 0, 4 in
 ** dimension 1 and 7 in dimension 2, with in total 84 elements.
 ** With that, we get
 ** @code
 ** P99_CDIM(D, 1, 2, 3)  =>  ((3) + ((D)[2] * ((2) + ((D)[1] * (1)))))
 ** P99_CDIM(D, k0, k1, k2)  =>  ((k2) + ((D)[2] * ((k1) + ((D)[1] * (k0)))))
 ** @endcode
 **/
P00_DOCUMENT_MULTIPLE_ARGUMENT(P99_CDIM, 0)
#define P99_CDIM(NAME, ...) P00_CDIM(P99_NARG(__VA_ARGS__), NAME, __VA_ARGS__)


/**
 ** @addtogroup VLA Variable Length Arrays
 ** @{
 **/

#define P00_SUBSCRIPT(NAME, X, I) [X]

/**
 ** @brief Transform the list of arguments <code>x0, x1, ...</code> to array
 ** subscripts <code>[x0][x1]...</code>
 **/
#define P99_SUBSCRIPT(...) P99_FOR(, P99_NARG(__VA_ARGS__), P00_SER, P00_SUBSCRIPT, __VA_ARGS__)

#define P99_ARRAY(ARR, ...) (ARR)P99_SUBSCRIPT(__VA_ARGS__)
#define P99_AREF(T, ARR, ...) T P99_ARRAY(*const ARR, __VA_ARGS__)
#define P99_AREF1(T, ARR, ...) T P99_ARRAY(ARR, static const 1, __VA_ARGS__)

#define P00_ALEN0(NAME)                                         \
  ((sizeof(NAME)/sizeof((NAME)[0]))                             \
   /sizeof(char[((!(sizeof(NAME) % sizeof((NAME)[0])))<<1)-1]))

#define P00_ALEN(NAME, _1, I) P99_IF_EQ_0(I)(P00_ALEN0(NAME))(P00_ALEN0((NAME)P99_REP(I,[0])))
#define P00_ALEN2_(NAME, I, ...) P00_ALEN(NAME,,I)
#define P00_ALEN2(NAME, ...) P00_ALEN2_(NAME, __VA_ARGS__,)

#define P00_ALENS0(NAME, I, REC, _3) REC, P00_ALEN(NAME,,I)
/**
 ** @brief Produce a list of the lengths of the argument array @a ARR in terms of number
 ** of elements in the first @a N dimensions.
 **/
P00_DOCUMENT_NUMBER_ARGUMENT(P99_ALENS, 1)
#define P99_ALENS(X, N) P99_FOR(X, N, P00_ALENS0, P00_ALEN, P99_REP(N,))

#define P00_ACALL1(ARR) P99_ALENS(*ARR, 1), (ARR)
#define P00_ACALL2(ARR, N) P99_ALENS(*ARR, N), (ARR)
/* The three argument form asserts that pointers to the elements of
   the matrix are assignment compatible to pointers of the indicated type.
   Then we do the cast to the pointer to matrix type that would
   otherwise be dangerous and could hide incompatibilities. */
#define P00_ACALL3(ARR, N, TYPE) P99_ALENS(*ARR, N), ((TYPE (*const)P99_REP(N,[1]))(TYPE*const){ &((*ARR)P99_REP(N,[0])) })

/* transform a list of names into size_t declarations */
#define P00_AARG_DECL(NAME, X, I) size_t const X
#define P00_AARG_LIST(N, ...) P99_FOR(, N, P00_SEQ, P00_AARG_DECL, __VA_ARGS__)

/* generate a list of size_t's and the declaration of the array
   pointer */
#define P00_AARG_0(T, ARR, DIM, ...) P00_AARG_LIST(DIM, __VA_ARGS__), P99_AREF1(T, ARR, __VA_ARGS__)
#define P00_AARG(T, ARR, DIM, INAME) P00_AARG_0(T, ARR, DIM, P99_NAME(DIM, INAME))

/* capture the special cases do implement default arguments */
#define P00_AARG_3(T, ARR, DIM) P00_AARG(T, ARR, DIM, P99_PASTE(p00_aarg_, ARR))
#define P00_AARG_2(T, ARR) P00_AARG_3(T, ARR, 1)

#ifdef P00_DOXYGEN
/**
 ** @brief Produce the length of the argument array @a ARR in terms of number
 ** of elements.
 **
 ** If a second argument @a N is given, it refers to the size of the
 ** Nth dimension of the array.
 **
 ** If @a ARR is actually just a pointer to an array, P99_ALEN(ARR, 0)
 ** is meaningless.
 **/
#define P99_ALEN(ARR, N)

/**
 ** @brief Pass a pointer to an @a N dimensional array @a ARR to a function.
 **
 ** This is not supposed to be used directly but for defining a macro
 ** interface to a function:
 ** @code
 ** double dotproductFunc(P99_AARG(double const, A, 1),
 **                       P99_AARG(double const, B, 1));
 **
 ** #define dotproduct(VA, VB)                       \
 **   dotproductFunc(P99_ACALL(VA, 1, double const), \
 **                  P99_ACALL(VB, 1, double const))
 ** .
 ** double Ar[5];
 ** double Br[5];
 ** .
 ** double result = dotproduct(&Ar, &Br);
 ** @endcode
 ** Here the expression with @c dotproduct in the last line will
 ** first result in a macro expansion that will place the pointers as
 ** well as the array sizes to a call of the function @c
 ** dotproductFunc.
 **
 ** If the argument @a N is omitted it will default to 1, supposing
 ** that the array is just one dimensional. If it is greater than 1, a
 ** list of length in the first @a N dimension of @a ARR is
 ** transmitted to the function call.
 **
 ** @a TYPE can be omitted in which case no attempt to conform types
 ** will be made. Specifying @a TYPE is particularly helpful if the
 ** type is qualified, that is has a @c const or @c volatile
 ** qualification as in the above example. If you don't use the tool
 ** that is provided here, ensuring <code>const</code>ness of multidimensional
 ** arrays is particularly tedious.
 **
 ** To be more precise, the three argument form asserts that pointers
 ** to the elements of the matrix are assignment compatible to
 ** pointers of the indicated type.  Then we do the cast to the
 ** pointer to matrix type that would otherwise be dangerous and could
 ** hide incompatibilities.
 ** @see P99_AARG
 **/
#define P99_ACALL(ARR, N, TYPE)

/**
 ** @brief Declare a pointer to array function argument of basetype @a
 ** TYPE, with name @a NAME, dimension @a DIM and naming scheme for
 ** the length variables @a VAR{0}, ... @a VAR{@a DIM - 1}.
 **
 ** Parameter @a VAR may be omitted such that you will not have access
 ** to the length variables. But you most probably don't need them
 ** since you may use ::P99_ALEN to have these values.
 **
 ** @a DIM defaults to @c 1.
 **
 ** @warning The pointer such declared has the @c const attribute
 ** and may thus not be modified.
 **
 ** If @a TYPE has qualifiers (@c const, @c volatile, @c restrict or
 ** @c _Atomic), the corresponding call to ::P99_ACALL @em must
 ** contain the same qualifiers in the 3rd argument.
 **
 ** @see P99_ACALL
 ** @see P99_ALEN
 **/
#define P99_AARG(TYPE, NAME, DIM, VAR)

#else
P00_DOCUMENT_NUMBER_ARGUMENT(P99_ALENS, 1)
#define P99_ALEN(...) P99_IF_EQ_1(P99_NARG(__VA_ARGS__))(P00_ALEN(__VA_ARGS__, ,0))(P00_ALEN2(__VA_ARGS__))
P00_DOCUMENT_NUMBER_ARGUMENT(P99_ACALL, 1)
P00_DOCUMENT_TYPE_ARGUMENT(P99_ACALL, 2)
#define P99_ACALL(...)  P99_PASTE2(P00_ACALL, P99_NARG(__VA_ARGS__))(__VA_ARGS__)
P00_DOCUMENT_TYPE_ARGUMENT(P99_AARG, 0)
P00_DOCUMENT_NUMBER_ARGUMENT(P99_AARG, 2)
#define P99_AARG(...) P99_IF_GT(P99_NARG(__VA_ARGS__),3)(P00_AARG(__VA_ARGS__))(P99_PASTE2(P00_AARG_, P99_NARG(__VA_ARGS__))(__VA_ARGS__))

#endif

/** @}
 **/


/**
 ** @ingroup preprocessor_blocks
 ** @brief Declare a @c for loop for which all iterations can be run
 ** independently and out of order.
 **
 ** This can be used syntactically exactly as the keyword @c for,
 ** except that the programmer asserts with this that the depending
 ** statement or block can be executed independently and out of order for all instances.
 **
 ** @code
 ** P99_PARALLEL_FOR (unsigned i = 0; i < limit; ++i) {
 **    unsigned sum = a[i] + b[i];
 **    a[i] *= sum;
 ** }
 ** @endcode
 **
 ** The resulting code then may be parallelized and (if the platform
 ** supports this) multiple threads may be used to speed up the
 ** execution.
 ** @see P99_PARALLEL_PRAGMA for the conditions under which this will
 ** result in a parallel execution.
 **/
#define P99_PARALLEL_FOR _Pragma(P99_PARALLEL_PRAGMA) for


#define P00_PRAGMA_DO(PRAG, TYPE, VAR, LOW, LEN, INCR)                           \
P00_BLK_START                                                                    \
P00_BLK_BEFORE(register TYPE                                                     \
               P99_PASTE2(p00_start_, VAR) = (LOW),                              \
               P99_PASTE2(p00_stop_, VAR) = P99_PASTE2(p00_start_, VAR) + (LEN), \
               P99_PASTE2(p00_incr_, VAR) = (INCR))                              \
P99_PRAGMA(PRAG)                                                                 \
     for (register TYPE P99_PASTE2(p00_i_, VAR) = P99_PASTE2(p00_start_, VAR);   \
          P99_LIKELY(P99_PASTE2(p00_i_, VAR) < P99_PASTE2(p00_stop_, VAR));      \
          P99_PASTE2(p00_i_, VAR) += P99_PASTE2(p00_incr_, VAR))                 \
       P00_BLK_START                                                             \
         P00_BLK_BEFORE(TYPE const VAR = P99_PASTE2(p00_i_, VAR))

#ifdef P00_DOXYGEN
/**
 ** @ingroup preprocessor_blocks
 ** @brief A fortran like do-loop with bounds that are fixed at the
 ** beginning
 **
 ** @param TYPE is the type of the control variable. It must be an
 ** arithmetic type.
 **
 ** @param VAR is the name of the control variable. It is not mutable
 ** inside the loop as if it where declared <code>TYPE const
 ** VAR</code>.
 **
 ** @param LOW is the start value of VAR for the first iteration. Only
 ** evaluated once before all iterations. Must be assignment
 ** compatible to type @a TYPE.
 **
 ** @param LEN is the length of the iteration and is
 ** non-inclusive. Only evaluated once before all iterations. Must be
 ** assignment compatible to type @a TYPE.
 **
 ** @param INCR is the increment of VAR after each iteration. Only
 ** evaluated once before all iterations. @a INCR defaults to @c 1 if
 ** omitted. Must be assignment compatible to type @a TYPE.
 **
 ** @code
 ** P99_DO(size_t, i, a, n, inc) {
 **   A[i] *= B[i-1]
 ** }
 ** @endcode
 ** would expand to something similar to
 ** @code
 ** for (size_t i = a; i < (a + n); i += inc) {
 **   A[i] *= B[i-1]
 ** }
 ** @endcode
 **
 ** only that
 ** -  the bounds of the loop (involving @c a and @c n) and the
 ** increment @c inc are fixed once when entering this construct
 ** - the loop variable @c i is not modifiable within the block
 **
 ** @warning Placing a @c #pragma directive directly in front of
 ** ::P99_DO will not work because of syntactic restrictions. Use
 ** ::P99_PRAGMA_DO instead.
 ** @see P99_PARALLEL_DO for a parallel variant of this
 ** @see P99_PRAGMA_DO for a variant of this that can be controlled
 ** with an arbitrary @c #pragma directive.
 **/
#define P99_DO(TYPE, VAR, LOW, LEN, INCR) for(;;)
/**
 ** @ingroup preprocessor_blocks
 ** @brief as ::P99_DO but performs the iterations out of order
 ** @see P99_DO for an explanation of the arguments
 ** @see P99_FOR for a more general parallel iteration construct
 **/
#define P99_PARALLEL_DO(TYPE, VAR, LOW, LEN, INCR) for(;;)
/**
 ** @brief as ::P99_DO but allows you to additionally place a pragma
 ** directive in front of the generated @c for loop
 **
 ** @warning Just placing a @c #pragma directive directly in front of
 ** ::P99_DO will not work because of syntactic restrictions
 **/
#define P99_PRAGMA_DO(PRAG, TYPE, VAR, LOW, LEN, INCR) for(;;)
#else
P00_DOCUMENT_TYPE_ARGUMENT(P99_DO, 0)
#define P99_DO(TYPE, VAR, ...) P99_PRAGMA_DO(, TYPE, VAR, __VA_ARGS__)
P00_DOCUMENT_TYPE_ARGUMENT(P99_PARALLEL_DO, 0)
#define P99_PARALLEL_DO(TYPE, VAR, ...) P99_PRAGMA_DO(P99_PARALLEL_PRAGMA, TYPE, VAR, __VA_ARGS__)
P00_DOCUMENT_TYPE_ARGUMENT(P99_PRAGMA_DO, 1)
#define P99_PRAGMA_DO(PRAG, TYPE, VAR, ...)                    \
P99_IF_EQ(P99_NARG(__VA_ARGS__), 2)                            \
(P00_PRAGMA_DO(PRAG, TYPE, VAR, __VA_ARGS__, 1))               \
(P00_PRAGMA_DO(PRAG, TYPE, VAR, __VA_ARGS__))
#endif

#define P00_FORALL_OP(NAME, I, REC, X) REC X

#define P00_FORALL_FUNC(NAME, X, I) P99_DO(size_t, X, 0, (NAME)[I])

#define P00_FORALL(N, NAME, ...) P99_FOR(NAME, N, P00_FORALL_OP, P00_FORALL_FUNC, __VA_ARGS__)

/**
 ** @ingroup preprocessor_blocks
 ** @brief A multi-index @c for loop
 **
 ** Given the names of @c N identifiers after @a NAME, @a NAME must
 ** correspond to an array of lengths with dimension at least @c N.
 ** @code
 ** size_t NAME[N];
 ** @endcode
 ** For @c N = 3 this could e.g be
 ** @code
 ** size_t const D[] = { 3, 4, 7 };
 ** @endcode
 ** Then, the macro in the following
 ** @code
 ** P99_FORALL(D, i0, i1, i2) {
 **   A[i0][i1][i2] *= B[i0][i1][i2]
 ** }
 ** @endcode
 ** would expand to something similar to
 ** @code
 ** for (size_t i0 = 0; i0 < D[0]; ++i0)
 **   for (size_t i1 = 0; i1 < D[1]; ++i1)
 **     for (size_t i2 = 0; i2 < D[2]; ++i2) {
 **        A[i0][i1][i2] *= B[i0][i1][i2]
 **     }
 ** @endcode
 **
 ** only that
 ** - the bounds of the loops (involving @c D[0], @c D[1] and @c
 **    D[2]) are fixed once when entering this construct
 ** - the loop variables @c i0, @c i1 and @c i2 are not modifiable
 **    within the block
 **
 ** @see P99_PARALLEL_FORALL for a variant that uses OpenMp to parallelize the loop.
 ** @see P99_DO for a simple fortran like iteration
 ** @see P99_CDIM for a macro that computes the absolute position of a
 **   index N-tuple in a multi-dimensional array.
 **/
P00_DOCUMENT_MULTIPLE_ARGUMENT(P99_FORALL, 0)
#define P99_FORALL(NAME, ...) P00_FORALL(P99_NARG(__VA_ARGS__), NAME, __VA_ARGS__)

#define P00_PARALLEL_FORALL_FUNC(NAME, X, I)  P99_PARALLEL_DO(size_t, X, 0, (NAME)[I])
#define P00_PARALLEL_FORALL(N, NAME, ...) P99_FOR(NAME, N, P00_FORALL_OP, P00_PARALLEL_FORALL_FUNC, __VA_ARGS__)

/**
 ** @ingroup preprocessor_blocks
 ** @brief A multi-index @c for loop who's dependent statement or
 ** block may be executed out of order.
 **
 ** This has the same semantics as ::P99_FORALL only that it assumes
 ** the independence of the of each execution of the statement from
 ** the others.
 **
 ** @see P99_FORALL for a variant that doesn't need that assumption,
 ** i.e where the statements should be executed sequentially in order.
 ** @see P99_PARALLEL_DO for a simple fortran like parallel iteration
 ** @see P99_PARALLEL_FOR for a parallel replacement of @c for
 ** @see P99_CDIM for a macro that computes the absolute position of a
 **   index N-tuple in a multi-dimensional array.
 **/
P00_DOCUMENT_MULTIPLE_ARGUMENT(P99_PARALLEL_FORALL, 0)
#define P99_PARALLEL_FORALL(NAME, ...) P00_PARALLEL_FORALL(P99_NARG(__VA_ARGS__), NAME, __VA_ARGS__)


#define P00_CASERANGE0(NAME, X, I) case ((NAME)+I):
#define P00_CASERANGE(START, LEN, ...)                               \
if (0) {                                                             \
  /* Let this be empty if LEN evaluates to 0. In that case we'd      \
     probably encounter a warning about an unused label. */          \
  P99_IF_EQ_0(LEN)()                                                 \
    /* Execution will only go here if one of the cases is chosen. */ \
    (P99_FOR(START, LEN, P00_SEP, P00_CASERANGE0, P99_REP(LEN,))     \
     /* Then it just continues with the else part */                 \
     goto P99_LINEID(__VA_ARGS__);)                                  \
    } else                                                           \
  /* execution will just fall through, here, if a previous case      \
     matched */                                                      \
  P99_LINEID(__VA_ARGS__)

#ifdef P00_DOXYGEN
/**
 ** @ingroup preprocessor_blocks
 ** @brief implement a range syntax for case labels.
 **
 ** gcc has an analogous extension to the C syntax. Something like
 ** @code
 ** case '0'..'9': return 23;
 ** @endcode
 ** This here implements such a thing with macros and should as such
 ** be more portable.
 ** @code
 ** switch (argv[0][0]) {
 **   P99_CASERANGE('\0', 0): return -1;
 **   P99_CASERANGE('0', 10): return 0;
 **   P99_CASERANGE('A', 25): return 1;
 **   P99_CASERANGE('a', 25, oioi): return 2;
 **   default: return 3;
 ** }
 ** @endcode
 **
 ** @param START must evaluate to an expression that can be used as a case label
 **
 ** @param LEN must evaluate to a decimal number. If this is 0 the
 ** entire depending statement is skipped and a warning about an
 ** unused label might result. (For me this is e.g the case for the
 ** first range in the above example.)
 **
 ** The additional variable argument list is optional and is used to
 ** "name" the range. This is only necessary if you have more than one
 ** P99_CASERANGE on the same logical line of code.
 **
 ** This is intended to have the same flow control rules as if there
 ** were just @em one case label in front of @em one statement. In
 ** particular, the dependent statement may just be a @c break to
 ** break out of the enclosing @c switch.
 **/
#define P99_CASERANGE(START, LEN, ...)
#else
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_CASERANGE, 0)
P00_DOCUMENT_NUMBER_ARGUMENT(P99_CASERANGE, 1)
#define P99_CASERANGE(START, ...) P00_CASERANGE(START, __VA_ARGS__, _caselabel)
#endif


#endif 	    /* !P99_FOR_H_ */
