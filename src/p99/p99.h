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
#ifndef   	P99_H_
# define   	P99_H_

/**
 ** @file
 ** @brief Use this to include all of P99.
 **/

/**
 ** @mainpage P99 - Preprocessor macros and functions for c99
 **
 ** P99 is a suite of macro and function definitions that ease the
 ** programming in modern C, aka C99. By using new tools from C99 we
 ** implement default arguments for functions, scope bound resource
 ** management, transparent allocation and initialization, ...
 **
 ** @section introduction Macros and inline functions working together
 **
 ** In C, functions (whether @c inline or not) and macros fulfill
 ** different purposes. Their difference should not be seen as
 ** ideological as some seem to take it, and what is even more
 ** important, they may work nicely together.
 **
 ** Macros are text replacement that is done at compile time and they
 ** can do things like ::P99_SIGNED(EXPR) which is defined by P99. That
 ** macro takes an expression as an argument, and tells at compile time
 ** of whether or not the integral type of @c EXPR is signed or
 ** not. Additionally it guarantees that @c EXPR itself is not
 ** evaluated at run time (so there are no side effects), but that
 ** only its type is taken into consideration.
 **
 ** Such an example shows that macros are ideally used when the type
 ** of an expression is to be determined and you want to act
 ** accordingly to that type. On the other hand, the pitfall with
 ** macros is that their arguments may be evaluated several times,
 ** which is bad because of side effects.
 **
 ** Functions on the other hand are typed, which makes them more
 ** strict or, phrased negatively, less flexible. Consider the
 ** function
 **
 ** @code
 ** inline
 ** uintmax_t p00_abs_signed(intmax_t a) {
 **  return (a < 0) ? -(uintmax_t)a : (uintmax_t)a;
 ** }
 ** @endcode
 **
 ** It takes signed integer value @c a and computes its absolute
 ** value. Observe that the return type of this function is
 ** unsigned. This has to be so, since otherwise not all valid values
 ** could be necessarily be realized.
 **
 ** ::p00_abs_signed would not be a good candidate for a macro, since
 ** @c a is evaluated twice in the expression; once in the controlling
 ** expression and once for returning its value or its negation.
 **
 ** We may use this function with any integral type, but then the
 ** result would probably not what a naive programmer would expect if
 ** the argument is a large unsigned value. The argument will be
 ** promoted to @c intmax_t. In case that the value @c A that is
 ** passed to the call is positive and greater than <code> INTMAX_MAX
 ** = 2<sup>N</sup> -1</code>, the result is probably not what we'd
 ** expect.@fntm 1@efntm If the conversion to
 ** @c intmax_t doesn't result in a range error thrown by the run time
 ** system (it would be permitted to do so), the argument @a a of the
 ** function would receive the negative value @c -C where @c C is @c
 ** 2<sup>N</sup> - A.  The result of the function call would then be
 ** @c C and not @c A.
 **
 ** With the following macro we get rid of these restrictions by
 ** combining the macro and the function:
 **
 ** @code
 ** #define P99_ABS(EXPR) (P99_SIGNED(EXPR) ? p00_abs_signed(EXPR) : (EXPR))
 ** @endcode
 **
 ** This has the following properties
 **
 ** <ul>
 **     <li>For any integral type it returns the correct result.
 **     @fntm 2
 **     @efntm
 **     </li>
 **     <li>The argument @c EXPR is evaluated exactly once.</li>
 **     <li>Any recent and decent compiler will create
 **         @ref inline "optimal code" for that combined macro.
 **     </li>
 ** </ul>
 **
 ** In that spirit, P99 aims to provide utilities that often combine
 ** macros and @c inline funcions and that are only possible with the
 ** @ref c99 "features"
 ** that come with C99 (read ANSI C as normalized in 1999)
 ** and that where absent in C89. The features include among others
 **  - @ref variadic
 **  - @ref inline
 **  - @ref initializers
 **  - @ref compound
 **  - @ref hide
 **  - @ref pragma
 **
 ** With these it implements @ref utilities "utilities" that
 ** previously had not been possible to program in C (or C macros) or
 ** that were very difficult to implement
 **  - @ref defaults
 **  - @ref blocks
 **  - @ref condi
 **  - @ref alloc
 **
 ** P99 also provides numerous facilities for
 ** @ref programming "macro programming."
 **
 ** P99 is not a C library in the classical sense but merely a
 ** collection of include files:
 **  - There is no binary library that is to be linked to your
 **    executable. The few functions that are provided are small
 **    wrappers that are compiled directly into your code.
 **  - There is nothing to configure, P99 include files should work
 **    out of the box with any conforming C99 compiler.
 **
 ** @fnt 1 @efnt
 ** Here <code>N+1</code> is the width of @c uintmax_t,
 ** assuming most common representations of signed integers.
 ** @fnt 2 @efnt
 ** Well, there is exactly one exception to that: on systems where @c
 ** -INTMAX_MIN is not representable in @c uintmax_t, this same value
 ** may cause problems.
 **
 ** @section credits Credits and Rights
 ** @subsection author Author and Maintainer
 ** @author <a href="http://www.loria.fr/~gustedt/">Jens Gustedt</a>
 ** @date 2010
 **
 ** @subsection version Version
 **
 ** The version that this documentation describes can be identified
 ** via the macros ::P99_VERSION_DATE, namely Sun Feb 13 23:03:02 2011 +0100. It also is
 ** tagged with an hexadecimal ID tag that is given in
 ** ::P99_VERSION_ID, namely f94fc70286dc7f201e9cddedd9974c37018cc7b4.
 **
 ** @subsection copyright Copyright
 ** Copyright &copy; 2010 Jens Gustedt, INRIA, France, http://www.inria.fr/
 **
 ** @htmlinclude SHORTLICENCE-open.txt
 **
 ** @subsection license License
 **
 ** <pre>
 **@verbinclude LICENSE-QPL.txt
 ** </pre>
 **/

/**
 ** @page conventions Programming conventions
 **
 ** P99 uses some programming conventions that might be interesting
 ** for projects that include its header files.
 **
 **  -# @ref standardconformance
 **  -# @ref OSindependence
 **  -# @ref prefixes
 **  -# @ref variableInit
 **  -# @ref temporaries
 **
 ** @section standardconformance Standard conformance
 **
 ** Where we can, we try to be conforming to the C99 standard and to
 ** clearly mark extensions, if we use them.
 **
 ** @subsection UB Undefined behavior
 **
 ** The C specification has many places where it explicitly says that
 ** under certain circumstances the behavior of the resulting code is
 ** undefined. Generally this means that a conforming C implementation
 ** is not obliged to capture such circumstances and for code that
 ** uses such undefined behavior might do anything, from
 ** do-the-right-thing, crashing to eating your hard drive.
 **
 ** P99 should not produce any such undefined behavior.
 **
 ** @subsection IB Implementation specific behavior
 **
 ** In other places the standard leaves room for C implementations to
 ** specify certain behavior.
 **
 ** P99 tries not use any special feature that might be the result of
 ** such implementation specific behavior. This concerns in particular
 ** arithmetic on integer types. Here the standard allows certain
 ** variations:
 **
 **  - padding bits: integer types may have padding bits that do not
 **    count into their width (# of significant bits) but into their
 **    size (storage requirement). So generally we have to be careful
 **    to not use expressions that use @c sizeof expressions for
 **    shifts.
 **
 **  - encoding of signed types: C99 allows three different encodings
 **    for signed integers. We do not assume any of these encodings
 **    but build macros that are valid for all of them.
 **
 **  - signed under- and overflow: arithmetic on signed integer types
 **    may under- or overflow and C99 leaves it to the implementation
 **    of whether or not this silently wraps around or triggers a
 **    signal. All expressions that involve signed types should be
 **    such that they avoid this implementation specific behavior. E.g
 **    to compute the absolute value of a negative @c int @c a we
 **    would use @c -(unsigned)a. This expression guarantees
 **    that the result is well defined even for corner cases (here @c
 **    a being @c INT_MIN in two's complement representation) and will
 **    never trigger a range error.
 **
 **  - We do not suppose the presence of the @c typedefs @c uintptr_t
 **    or @c intptr_t since they are optional in C. In particular we
 **    may not assume that there is any sensible conversion between
 **    pointers and integer types.
 **
 ** @section prefixes Defining identifiers
 **
 ** Macro names that implement a functionality of P99 are generally
 ** uppercase. Exceptions from that rule are @ref hide.
 ** All other identifiers are lowercase.
 **
 ** P99 uses the common prefixes @c P99_ and @c p99_ for macros and
 ** other identifiers, respectively. If you include any of the P99
 ** files future version could use any new identifier with one of
 ** those prefixes, so don't define some of them your own.
 **
 ** The same rule holds for the prefixes @c P00_and @c p00_ are used
 ** for identifiers, only that they are auxiliary and that need not be
 ** documented. Such identifiers are ignored in the doxygen
 ** documentation.
 **
 ** @section OSindependence Operating system independence
 **
 ** The P99 macros and functions as such should be independent of the
 ** execution system and compiler. Nevertheless, for the time being
 ** they are only tested on POSIX systems, namely linux. So if there
 ** is a pitfall for other systems that we didn't see, yet, please let
 ** us know.
 **
 ** In contrast to that general policy, there is @em one file that is
 ** dependent on the system, p99_posix_default.h. As the name
 ** indicates it is designed for POSIX systems and provides default
 ** arguments for some POSIX functions.
 **
 ** Also, some of the examples throughout this documentation are taken
 ** from programs that would typically run on POSIX systems. We hope
 ** that they easily may stand as such and don't need much
 ** explanations for programmers of other systems.
 **
 ** @section variableInit Variable initialization
 **
 ** Where possible, P99 uses initializers to initialize variables. For
 ** each type @c T where such an initialization is possible, there
 ** should be a macro @c T_INITIALIZER that does a standard
 ** initialization. Such a macro should use the @ref initializers
 ** scheme.
 **
 ** @code
 ** typedef struct toto toto;
 ** struct toto { double a; unsigned b; };
 ** #define TOTO_INITIALIZER { .a = 0.0; .b = 0u }
 ** @endcode
 **
 ** In case that you want just the default behavior of C, namely that
 ** all fields are recursively initialized with @c 0 then you could
 ** just use
 ** @code
 ** #define TOTO_INITIALIZER P99_INIT
 ** @endcode
 ** to make this choice explicit.
 **
 ** Such initializers can be easily assembled together
 ** @code
 ** typedef struct tutu tutu;
 ** struct tutu { toto A; bool c; };
 ** #define TUTU_INITIALIZER(VAL) { .A = TOTO_INITIALIZER, .c = (VAL) }
 ** @endcode
 **
 ** As you can see in this example, @c INITIALIZER can be a `normal'
 ** macro or a function like macro.
 **
 ** For dynamic initialization we assume that an `init' function
 ** exists that
 ** - takes a pointer as a first argument
 ** - tests for the validity of that pointer, and
 ** - returns exactly the same pointer
 ** @code
 ** toto* toto_init(toto* t) {
 **          // assign from a compound literal
 **   if (t) *t = (toto)TOTO_INITIALIZER;
 **   return t;
 ** }
 ** tutu* tutu_init(tutu* t, bool val) {
 **   if (t) {
 **     toto_init(&(t->A));
 **     t->c = val;
 **   }
 **   return t;
 ** }
 ** @endcode
 **
 ** @section temporaries Use of temporary lvalues
 **
 ** Often when programming utilities for C that are supposed to return
 ** a pointer to an array or structure the question arises who is
 ** allocating the space, the caller or the callee.
 **
 ** P99 goes a different way, in between, in that tries to take away
 ** that burden mostly from the programmer of both, caller and callee.
 ** Let us look at the hypothetical function
 ** @code
 ** char const* hostname(char buffer[], size_t len);
 ** @endcode
 **
 ** which could be defined as being similar to the POSIX @c
 ** gethostname function, only that it doesn't return an error
 ** indicator but a pointer to the name or a null pointer if it fails. An old
 ** time (and dangerous!) calling convention for such a function would
 ** perhaps have been to return a statically allocated buffer in case
 ** that the @c buffer argument is a null pointer.
 **
 ** P99 lets you define more convenient and less dangerous calling
 ** conventions:
 ** @ref defaults
 ** allows us to define a
 ** @ref hide "macro of the same name"
 ** that uses a
 ** @ref compound "compound litteral"
 ** if no argument is given to the same function.
 **
 ** @code
 ** #define hostname(...) P99_CALL_DEFARG(hostname, 2, __VA_ARGS__)
 ** #define hostname_defarg_0() P99_LVAL(char[HOSTNAME_MAX])
 ** #define hostname_defarg_1() HOST_NAME_MAX
 ** @endcode
 **
 ** This defines three different macros. One that is used in the place
 ** the programmer places a call to @c hostname. The other two, @c
 ** hostname_defarg_0 and @c hostname_defarg_1, are used by the macro
 ** @c hostname when the respective arguments are left out.
 **
 ** Now @c hostname can be used in three different ways.
 ** <ol>
 ** <li>Such that
 ** the caller is responsible and obtains space on the heap:
 ** @code
 ** char const*const host = hostname(malloc(mylen), mylen);
 ** .
 ** free(host);
 ** @endcode
 ** </li>
 ** <li>Such that the caller initializes its own variable that has a
 ** storage class that fits best to the needs:
 ** @code
 ** char host[mylen];
 ** .
 ** hostname(host, mylen);
 ** @endcode
 ** </li>
 ** <li>
 ** Or such that the space is allocated on the stack of the current
 ** scope of the call:
 ** @code
 ** char const*const host = hostname();
 ** @endcode
 ** </li>
 ** </ol>
 **
 ** The later is then equivalent to
 ** @code
 ** char tmp[HOSTNAME_MAX] = { 0 };
 ** char const*const host = hostname(tmp, HOSTNAME_MAX);
 ** @endcode
 ** but without leaving a non-const access to the contents of @c tmp.
 **
 **
 ** It uses a temporary value that is only valid inside the block in
 ** which the @c get_hostname macro is expanded. The handling of this
 ** temporary is implicit, neither the caller nor the callee have to
 ** worry of allocating or deallocating it.  On the calling side this
 ** convention is simple to use without having the callee expose a
 ** static buffer.
 **
 ** In P99, is currently applied in a few places, in particular in the
 ** header file "p99_posix_default.h". The usage of it will probably
 ** grow in future releases.
 **/

/**
 ** @page utilities Implemented utilities
 **
 ** P99 implements a lot of different features through macros and
 ** functions, too much to mention explicitly in such an overview. You
 ** will find a structured hierarchy of descriptions below the
 ** "Modules" tag and the documentation of the individual items under
 ** "Files" -> "Globals". Here we will introduce some main features:
 **
 **  -# @ref defaults
 **  -# @ref blocks
 **  -# @ref for
 **  -# @ref condi
 **  -# @ref alloc
 **
 ** @section defaults Default arguments to functions
 **
 ** In section @ref temporaries we have seen a way to provide default
 ** arguments to functions by overloading them with macros. The
 ** general declaration pattern here is as follows
 **
 ** @code
 ** #define NAME(...) P99_CALL_DEFARG(NAME, N, __VA_ARGS__)
 ** @endcode
 **
 ** Where @c NAME becomes the name of a macro and where we also
 ** suppose that there is already a function of the same name @c NAME.
 **
 ** Other than this would be in C++, the default value for the
 ** ::hostname macro that we had given was itself produced by a macro,
 ** namely @c hostname_defarg_0. The convention here is as simple as that:
 **  - when called, ::P99_CALL_DEFARG replaces each argument M
 **    (counting starts at 0) that is not
 **    provided by the tokens
 **    @code
 **    NAME ## _defarg_ ## M ()
 **    @endcode
 **    that is a concatenation of @c NAME with the token @c _defarg_
 **    and the decimal number @c M
 **  - "not provided" here meaning either
 **     - leaving an empty place in an argument list
 **     - giving less arguments than @c N
 **  - to be valid C code this name must then either
 **      -# itself be a macro that is then expanded
 **      -# be a valid function call that can be interpreted by the
 **        compiler
 **
 ** As we have seen in the example (a) is computed in the context of
 ** the caller. This let us simply use a temporary (here a local
 ** @ref compound "compound literal") that was thus valid in that context.
 **
 ** To obtain the same behavior as for C++, namely to provide a
 ** default argument that is evaluated at the place of declaration and
 ** not at the place of the call we have to use (b), a function call.
 ** This will be as efficient as macro call if we use @ref inline for
 ** that purpose.
 **
 ** To ease the programming of this functional approach P99 provides
 ** some machinery. We need three things as in the following example:
 ** @code
 ** P99_PROTOTYPE(rand48_t *, rand48_t_init, rand48_t*, unsigned short, unsigned short, unsigned short);
 ** #define rand48_t_init(...) P99_CALL_DEFARG(rand48_t_init, 4, __VA_ARGS__)
 ** P99_DECLARE_DEFARG(rand48_t_init,
 **                    ,
 **                    useconds(),
 **                    getpid(),
 **                    (atomic_counter_inc(&rand48_counter), atomic_counter_getvalue(&rand48_counter))
 **                    );
 ** @endcode
 **
 ** That are
 **  - a "prototype" of the underlying function, such that P99
 **    knows of the name of the function, the return type and the types
 **    of the arguments.
 **  - the macro definition as we have already seen
 **  - a declaration of the default arguments.
 **
 ** Here in the example there is no default argument for position 0
 ** but one for positions 1 to 3. All three have the type @c
 ** unsigned. The above leads to the automatic generation of three @c
 ** inline function that looks something like:
 **
 ** @code
 ** inline
 ** pthread_mutexattr_t const*
 ** pthread_mutex_init_defarg_1(void) {
 **   return useconds();
 ** }
 ** inline
 ** pthread_mutexattr_t const*
 ** pthread_mutex_init_defarg_2(void) {
 **   return getpid();
 ** }
 ** inline
 ** pthread_mutexattr_t const*
 ** pthread_mutex_init_defarg_3(void) {
 **   return (atomic_counter_inc(&rand48_counter), atomic_counter_getvalue(&rand48_counter));
 ** }
 ** @endcode
 **
 ** This declaration and definition is placed in the context of the
 ** above declaration and not in the context of the caller. Thus
 ** the expression is evaluated in that context, and not in the
 ** context of the caller. In particular for the third function, this
 ** fixes the variable @c rand48_counter to the one that is visible at
 ** the point of declaration.
 **
 ** @section blocks Scope bound resource management with for scopes
 **
 ** Resource management can be tedious in C. <em>E.g</em> to protect a
 ** critical block from simultaneous execution in a threaded
 ** environment you'd have to place a lock / unlock pair before and
 ** after that block:
 ** @code
 ** pthread_mutex_t guard = PTHREAD_MUTEX_INTIALIZER;
 **
 ** pthread_mutex_lock(&amp;guard);
 ** // critical block comes here
 ** pthread_mutex_unlock(&amp;guard);
 ** @endcode
 ** This is very much error prone since you have to provide such calls
 ** every time you have such a block. If the block is longer than some
 ** lines it is difficult to keep track of that, since the lock /
 ** unlock calls are spread on the same level as the other code.
 **
 ** Within C99 (and equally in C++, BTW) it is possible to extend the
 ** language of some sorts such that you may make this easier visible
 ** and guarantee that your lock / unlock calls are matching. Below,
 ** we will give an example of a macro that will help us to write
 ** something like
 ** @code
 ** P99_PROTECTED_BLOCK(pthread_mutex_lock(&guard),
 **                     pthread_mutex_unlock(&guard)) {
 **        // critical block comes here
 ** }
 ** @endcode
 ** If we want to make this even a bit more comfortable for cases that
 ** we still need to know the mutex variable we may have something
 ** like:
 ** @code
 ** PROTECT_IT(guard) {
 **        // critical block comes here
 ** }
 ** @endcode
 ** For cases where the mutex variable is specific to the block:
 **  @code
 ** CRITICAL {
 **        // critical block comes here
 ** }
 ** @endcode
 **
 ** Please look into the documentation of
 ** ::P99_PROTECTED_BLOCK(BEFORE, AFTER) to see how it is defined.  As
 ** you may see there, it uses two <code>for</code> statements. The
 ** first defines a auxiliary variable @c _one1_ that is used to
 ** control that the dependent code is only executed exactly once. The
 ** arguments @c BEFORE and @c AFTER are then placed such that they
 ** will be executed before and after the dependent code,
 ** respectively.
 **
 **
 ** The second @c for is just there to ensure that @c AFTER is even
 ** executed when the dependent code executes a @c break or @c
 ** continue statement. For other preliminary exits such as @c return
 ** or @c exit() there is unfortunately no such cure. When programming
 ** the dependent statement we have to be careful about these, but
 ** this problem is just the same as it had been in the ``plain'' C
 ** version.
 **
 ** Generally there is no run time performance cost for using such a
 ** macro. Any decent compiler will detect that the dependent code is
 ** executed exactly once, and thus optimize out all the control that
 ** has to do with our variable @c _one1_.
 **
 ** The PROTECT_IT macro could now be realized as:
 ** @code
 ** #define PROTECT_IT(NAME)                                   \
 ** P99_PROTECTED_BLOCK(pthread_mutex_lock(&(NAME)),           \
 **                     pthread_mutex_unlock(&(NAME)))
 ** @endcode
 **
 ** To have more specific control about the mutex variable we may use
 ** another P99 macro,
 ** P99_GUARDED_BLOCK(T, NAME, INITIAL, BEFORE, AFTER)
 **
 ** This is a bit more complex than the previous one since in addition
 ** it declares a local variable @c NAME of type
 ** @c T and initializes it. The @c CRITICAL macro
 ** can then be defined as follows
 ** @code
 ** #define CRITICAL                                           \
 ** P99_GUARDED_BLOCK(                                         \
 **     static pthread_mutex_t,                                \
 **     _critical_guard_,                                      \
 **     PTHREAD_MUTEX_INITIALIZER,                             \
 **     pthread_mutex_lock(&_critical_guard_),                 \
 **     pthread_mutex_unlock(&_critical_guard_))
 ** @endcode
 ** Observe the use of @c static for the declaration of
 ** @c _critical_guard_. This guarantees that any thread
 ** that runs through this code uses the same variable and that this
 ** variable is properly initialized at compile time.
 **
 **
 ** Other such block macros that can be implemented with such a technique:
 **   - pre- and postconditions
 **   - ensure that some dynamic initialization of a static variable is performed exactly once
 **   - code instrumentation
 **
 ** An even more sophisticated tool for scope bound resources
 ** management is provided by the macro ::P99_UNWIND_PROTECT
 ** @code
 ** double toto(double x) {
 **  P99_UNWIND_PROTECT {
 **    // do something
 **    while (cond0) {
 **      for (;cond1;) {
 **         if (cond2) P99_UNWIND(-1);
 **         // preliminary return
 **         if (cond3) P99_UNWIND_RETURN 5.7777E-30;
 **      }
 **    }
 **   P99_PROTECT :
 **    // do some cleanup here
 **    // if everything went well ::p99_unwind_code has value 0 otherwise it
 **    // receives a value from P99_UNWIND
 **  }
 **  // regular return
 **  return x * x;
 ** }
 ** @endcode
 **
 ** In this code fragment the statement ::P99_UNWIND will ensure that
 ** the two levels of loops are broken and that the execution
 ** continues at the special label ::P99_PROTECT.
 **
 ** ::P99_UNWIND_RETURN goes one step further. As for ::P99_UNWIND, it
 ** executes the clause after ::P99_PROTECT, but when it reaches the
 ** end of the ::P99_UNWIND_PROTECT scope it will return to the caller
 ** with a return value as specified after ::P99_UNWIND_RETURN, here
 ** the value @c 5.7777E-30.
 **
 ** @section for Multidimensional arrays and parallel loops
 **
 ** We provide some utilities to ease the programming of loop
 ** iterations in one or multiple dimensions. The simplest to use
 ** is ::P99_DO, that resembles much to a @c do loop in fortran. It
 ** fixes the bounds of the iteration once, before entering the
 ** iteration itself.
 **
 ** @code
 ** P99_DO(size_t, i, a, n, inc) {
 **   A[i] *= B[i-1]
 ** }
 ** @endcode
 **
 **
 ** ::P99_FORALL allows to generate nested for loops over
 ** arbitrary many dimensions:
 ** @code
 ** size_t const D[3] = { 20, 17, 31 };
 ** P99_FORALL(D, i, j, k) {
 **      A[i][j][k] *= B[i][j][k];
 ** }
 ** @endcode
 **
 ** will iterate over all combinations of @c i, @c j, @c k in the
 ** bounds specified by @c D.
 **
 ** ::P99_PARALLEL_FOR, where available, will provide a parallelized
 ** version of a simple @c for loop, and ::P99_PARALLEL_DO and
 ** ::P99_PARALLEL_FORALL implements nested parallel loops with
 ** otherwise the same semantics as for ::P99_DO or ::P99_FORALL,
 ** respectively.
 **
 ** @section condi Preprocessor conditionals and loops
 **
 ** P99 provides you with macro features that can become handy if you
 ** have to generate code repetition that might be subject to changes, later.
 ** As examples suppose that you'd have to code something like
 **
 ** @code
 ** tata = A[0]; tete = A[1]; titi = A[2]; toto = A[3];
 ** typedef int hui_0; typedef unsigned hui_1; typedef double hui_2;
 ** @endcode
 **
 ** If over time there are many additions and removals to these lists,
 ** maintaining such a code will not really be a pleasure. In P99 you
 ** may write equivalent statements and declarations just as
 **
 ** @code
 ** P99_VASSIGNS(A, tata, tete, titi, toto);
 ** P99_TYPEDEFS(hui, int, unsigned, double);
 ** @endcode
 **
 ** There are a handful of such predefined macros that you may look up
 ** under @ref statement_lists. Under the hood they all use a more
 ** general macro that you may yourself use to define your own macros:
 ** ::P99_FOR. The use of this will be described more in detail under
 ** @ref programming.
 **
 ** The predefined macros from above usually are also able a nasty
 ** special case if the variadic part of the argument list is
 ** empty. Something like
 **
 ** @code
 ** P99_VASSIGNS(A);
 ** P99_TYPEDEFS(hui);
 ** @endcode
 **
 ** would at least cause a warning with conforming preprocessors if
 ** the macros were implemented directly with something like
 **
 ** @code
 ** #define P99_VASSIGNS(NAME, ...) do_something_here
 ** #define P99_TYPEDEFS(NAME, ...) do_something_else_here
 ** @endcode
 **
 ** since the variable length part should not be empty, according to
 ** the standard. With P99 you don't have these sort of problems, the
 ** above should just result in empty statements or declarations, that
 ** are even capable to swallow the then superfluent semicolon at the
 ** end.
 **
 ** P99 avoids this by testing for the length of the argument list as
 ** a whole with ::P99_NARG and by using a macro conditional
 ** controlled by that length. Such conditionals like ::P99_IF_EMPTY
 ** ensure that the preprocessor decides which code of two different
 ** variants the compiler will see. The fragment
 **
 ** @code
 ** P99_IF_EMPTY(BLA)(special_version)(general_version)
 ** @endcode
 **
 ** will expand to either @c special_version or @c general_version
 ** according to @c BLA. If it expands to an empty token, the first
 ** variant is produced, if it is at least one non-empty token the
 ** second one results.
 **
 ** P99 also implements logical and arithmetic operations in the
 ** preprocessor. Logical operations just evaluate to the tokens @c 0
 ** or @c 1. Arithmetic is restricted to small decimal numbers, less
 ** than ::P99_MAX_NUMBER. Just some examples
 **
 ** @code
 ** P99_IS_EQ(int, double)    ==> 0
 ** P99_IS_EQ(static, static) ==> 1
 ** P99_ADD(4, 5)         ==> 9
 ** @endcode
 **
 ** See @ref preprocessor_operators for more about that.
 **
 ** @section alloc Allocation and initialization facilities
 **
 ** Consistent initialization of variables is an important issue in
 ** C. P99 provides some tools to help you on that, most importantly a
 ** macro ::P99_NEW. Therefore we have to relay on some assumptions
 ** that are specified in @ref variableInit, in particular that there
 ** is an `init' function for each type that we want to use with ::P99_NEW.
 **
 ** For the example type of a circular list element
 **
 ** @code
 ** // Forward declare struct elem and elem
 ** typedef struct elem elem;
 ** .
 ** .
 ** .
 ** struct elem { elem* pred; elem* succ; };
 ** @endcode
 **
 ** we might want to ensure that the fields @c pred and @c succ are
 ** always properly initialized. An `init' function could look as follows:
 ** @code
 ** #define ELEM_INITIALIZER(HERE, PRED, SUCC) {
 **  .pred = (PRED) ? (PRED) : (HERE),
 **  .succ = (SUCC) ? (SUCC) ; (HERE),
 ** }
 ** @endcode
 **
 ** A static initialization of a 4 element list in file scope can then be done as
 ** @code
 ** extern elem * head;
 ** .
 ** .
 ** static elem L0;
 ** static elem L1;
 ** static elem L2;
 ** static elem L3;
 ** static elem L0 = ELEM_INITIALIZER(&L0, &L1, &L3);
 ** static elem L1 = ELEM_INITIALIZER(&L1, &L0, &L2);
 ** static elem L2 = ELEM_INITIALIZER(&L2, &L1, &L3);
 ** static elem L3 = ELEM_INITIALIZER(&L3, &L2, &L0);
 ** head = &L0;
 ** @endcode
 **
 ** Dynamic initialization of a 4 element list on the stack in function scope
 ** @code
 ** elem L[4] = {
 **   [0] = ELEM_INITIALIZER(&L[0], &L[1], &L[3]),
 **   [1] = ELEM_INITIALIZER(&L[1], &L[0], &L[2]),
 **   [2] = ELEM_INITIALIZER(&L[2], &L[1], &L[3]),
 **   [3] = ELEM_INITIALIZER(&L[3], &L[2], &L[0]),
 ** };
 ** @endcode
 **
 ** To do dynamic initialization we would then define something like this:
 ** @code
 ** elem * elem_init(elem* here, elem* there) {
 **   if (here) {
 **     if (there) {
 **        here->pred = there;
 **        here->succ = there->succ;
 **        there->succ = here;
 **        here->succ->pred = here;
 **     } else {
 **        here->pred = here;
 **        here->succ = here;
 **     }
 **   }
 **   return here;
 ** }
 ** @endcode
 **
 ** Initializations of this type of heap variables in function scope
 ** can now simply look like this
 ** @code
 ** elem * a = P99_NEW(elem, P99_0(elem*));
 ** elem * b = P99_NEW(elem, a);
 ** elem * c = P99_NEW(elem, b);
 ** @endcode
 ** or
 ** @code
 ** elem * head = P99_NEW(elem, P99_NEW(elem, P99_NEW(elem, P99_0(elem*))));
 ** @endcode
 **
 ** These define cyclic lists of 3 elements, well initialized and
 ** ready to go.
 **
 ** In fact, the ::P99_NEW macro takes a list of arguments that may be
 ** arbitrarily@fntm 3@efntm
 ** long. It just needs the first, which is supposed to be
 ** the type of the object that is to be created. The other are then
 ** passed as supplementary arguments to the `init' function, here the
 ** parameter @c there.
 **
 ** If the `init' function accepts default arguments to some
 ** parameters, so will ::P99_NEW. With @ref default_arguments, calls
 ** to ::P99_NEW may then omit the second argument:
 **
 ** @code
 ** #define elem_init(...) P99_CALL_DEFARG(elem_init, 2, __VA_ARGS__)
 ** #define elem_init_defarg_1() P99_0(elem*)
 ** .
 ** .
 ** .
 ** elem * a = P99_NEW(elem);
 ** elem * head = P99_NEW(elem, P99_NEW(elem, P99_NEW(elem)));
 ** @endcode
 **
 ** @fnt 3@efnt
 ** The number of arguments might be restricted by your compiler
 ** implementation. Also most of the P99 macros are limited to
 ** ::P99_MAX_NUMBER.
 **
 **/

/**
 ** @page programming Macro programming with P99
 **
 ** Most macros and features for macro programming with P99 are
 ** defined in @ref meta_programming. This allows to do things like
 ** <dl>
 **  <dt>@ref arg_counting</dt>
 **  <dt>rudimentary argument list processing</dt>
 **     <dd>to obtain e.g a sublist of the argument list (::P99_NARG) or revert an
 **     argument (::P99_REVS)</dd>
 **  <dt>@ref unrolling</dt>
 **     <dd>not restricted to usual @c for loops but also e.g to produce a
 **     sequence of declarations with initializers (::P99_VASSIGNS)</dd>
 **  <dt>constant generation</dt>
 **     <dd>to compose @c double constants</dd>
 **  <dt>type and keyword classification</dt>
 **  <dt>@ref blocks</dt>
 ** </dl>
 **
 ** @section arg_counting Argument List Counting
 **
 ** To implement macros that evaluate conditionally on the number of
 ** arguments that they receive we will need to determine how many
 ** arguments it receives. This can be achieved with something like
 ** the following:
 **
 ** @code
 ** #define P00_ARG2(_0, _1, _2, ...) _2
 ** #define NARG2(...) P00_ARG2(__VA_ARGS__, 2, 1, 0)
 ** @endcode
 **
 ** If NARG2 is called with two arguments the @c 2 of its expansion is in
 ** third position and we will see this @c 2. If it is called with just
 ** one argument the 1 will be in that place and thus be the result of
 ** the expansion. You probably easily imagine an extension of that
 ** macro to treat more arguments, look into the P99 sources to see a
 ** version for ::P99_MAX_NUMBER.
 **
 ** The toy macro @c NARG2 has an important property, namely that it
 ** swallows @em all its arguments when called correctly (say with 0,
 ** 1 or 2 arguments) and just returns a token that corresponds to the
 ** number. Such a property is important for macro programming since
 ** we don't want to have the compiler itself see the same expressions
 ** multiple times.
 **
 ** The @c NARG2 has a major disadvantage, it will not be able to
 ** detect an empty argument list. This is due to a fundamental
 ** difference between C and its preprocessor. For C a parenthesis @c ()
 ** is empty and contains no argument. For the preprocessor it
 ** contains just one argument, and this argument is the empty token
 ** <code>&nbsp;</code>.
 **
 ** So in fact @c NARG2 is cheating. It doesn't count the number of
 ** arguments that it receives, but returns the number of commas plus
 ** one. In particular, even if it receives an empty argument list it
 ** will return @c 1. The macro ::P99_NARG deals with that and returns
 ** the token @c 0, if the list is empty.
 **
 ** Other macros are then programmed with similar tricks as for @c
 ** NARG2, here: the variable argument list is positioned at the
 ** beginning of a new macro list that is then completed by a list of
 ** values that contain the different tokens that are returned conditionally.
 **
 ** A second trick is then to paste the name of another macro with
 ** that number together. Look e.g at ::P99_DUPL. When called as follows
 **
 ** @code
 ** (P99_DUPL(3, A))  ==> (P00_DUPL3(A))
 ** @endcode
 **
 ** this then is replaced @fntm 4@efntm
 ** as follow
 ** @code
 ** (P00_DUPL3(A)) ==> (A, P00_DUPL2(A)) ==> (A, A, P00_DUPL1(A)) ==> (A, A, A)
 ** @endcode
 **
 ** The example of ::P99_DUPL together with ::P00_DUPL1, ... in file
 ** p99_generated.h shows a general strategy to overcome the lack of
 ** control structures or recursion in the preprocessor. But generally
 ** it would be tedious and error prone to have to copy similar
 ** definitions over and over again. Therefore P99 implements some of
 ** these with a script and collects them in the above mentioned
 ** include file. This is probably not something you should or
 ** want to do yourself. Section @ref unrolling will show how to
 ** avoid this with higher level preprocessor programming constructs.
 **
 **
 ** The next step in macro programming is then to use ::P99_NARG to
 ** obtain the length of a list and to use this number token to
 ** derive a macro name with the number in it
 **
 ** @code
 ** P99_PASTE(t, o, k, e, n)
 ** ==>
 ** P00_PASTE(P00_NARG(t, o, k, e, n), t, o, k, e, n)
 ** ==>
 ** P00_PASTE(5, t, o, k, e, n)
 ** ==>
 ** P00__PASTE(P99_PASTE, 5, t, o, k, e, n)
 ** ==>
 ** P99_PASTE ## 5(t, o, k, e, n)
 ** ==>
 ** P99_PASTE5(t, o, k, e, n)
 ** ==>
 ** .
 ** ==>
 ** token
 ** @endcode
 **
 ** @fnt 4 @efnt
 ** The actual definition is a bit more complicated to capture special
 ** cases.
 **
 ** @section unrolling Code Unrolling
 **
 ** Code unrolling is a generalization of what classicaly would be
 ** left to the compiler (and not the preprocessor): loop
 ** unrolling. Suppose that in some context you have a loop of fixed
 ** length
 **
 ** @code
 ** for (unsigned i = 0; i < 4; ++i)
 **    a[i] = b[i];
 ** @endcode
 **
 ** There are good chances that an optimizing compiler would unroll
 ** this loop
 **
 ** @code
 ** a[0] = b[0];
 ** a[1] = b[1];
 ** a[2] = b[2];
 ** a[3] = b[3];
 ** @endcode
 **
 ** This would have the advantage to spoil a CPU register for the
 ** variable @c i and also that the addressing of the individual
 ** elements now can be done with constant offsets from the
 ** basepointers of @c a and @c b.
 **
 ** We will see below how to achieve such a loop unrolling directly
 ** with the preprocessor, avoiding to rely on the compiler to do so.
 **
 ** But code unrolling can do more than that. It may be helpful where
 ** we have repeated pieces of code for which there is no loop
 ** construct in C, for example in a declaration:
 ** @code
 ** signed a[4] = { b[0], b[1], b[2], b[3] };
 ** @endcode
 **
 ** With P99 you can write this simply as
 ** @code
 ** signed a[4] = { P99_ACCESSORS(b, 4) };
 ** @endcode
 **
 ** If the length of your vectors might perhaps change during the
 ** development of your program, you can have the length of the
 ** array as a compile time parameter
 ** @code
 ** #define N 4
 ** .
 ** signed a[N] = { P99_ACCESSORS(b, N) };
 ** @endcode
 **
 ** You'd later just change @c N and the rest of your code remains consistent.
 **
 ** As another example of flexibility take another assignment example,
 ** namely some "deserialization" of a @c struct. Suppose that we have
 ** a variable of a @c struct type to which we want to assign values
 ** that we receive through an array @c b:
 ** @code
 ** A.x = b[0];
 ** A.y0 = b[1];
 ** A.o7 = b[2];
 ** A.k = b[3];
 ** @endcode
 **
 ** Here the pattern is somewhat regular, you assign the elements of
 ** @c b in order, but the left hand sides are arbitrary. P99 can do
 ** that for you
 ** @code
 ** P99_VASSIGNS(b, A.x, A.y0, A.o7, A.k);
 ** @endcode
 **
 ** BTW, with the two P99 macros that we just introduced we can now
 ** perform the loop unrolling from the beginning:
 ** @code
 ** P99_VASSIGNS(b, P99_ACCESSORS(a, 4));
 ** @endcode
 **
 ** The "control structure" that is behind these two macros is called
 ** ::P99_FOR. This is by itself a macro that at least 4 parameters,
 ** named @c NAME, @c N, @c OP, @c FUNC, but generally will have more,
 ** namely a list <code>A0, A1, ... , A{N-1}</code> of extra
 ** arguments. The idea behind is simple:
 **  - the argument @c FUNC is supposed to be a macro name that is
 **    going to be applied to each of the @c A{i}
 **  - argument @c OP in turn is a macro that is called to control the
 **    "glue" between the different occurrences of the @c FUNC invocations.
 **
 ** An example of @c FUNC would be something like <code>NAME[i]</code>
 ** for an accessor. For @c OP we provide simple choices as ::P00_SEQ
 ** which just puts commas between the occurrences or ::P00_SEP to do
 ** so with semicolons. For more exact examples and the syntax for @c
 ** OP and @c FUNC please refer to the documentation of ::P99_FOR.
 **/

/**
 ** @page c99 C99 features
 **
 ** As extensions to C89, C99 offers some features that largely
 ** improve its possibilities of programming efficiently and portable
 ** at the same time. There are four of these new concepts that are
 ** particularly important for P99, without them P99 wouldn't be
 ** possible.
 ** - @ref variadic
 ** - @ref inline
 ** - @ref initializers
 ** - @ref compound
 ** - @ref pragma
 **
 ** @section variadic Variadic macros
 **
 ** The preprocessor now has a feature that previously already C
 ** functions could have: a macro may accept an argument list that may
 ** vary in size. As for functions such a macro is defined with `...'
 ** in the argument list to indicate a list following the initial,
 ** named, arguments:
 **
 ** @code
 ** #define TOTO(NAME, ...)  NAME[__VA_ARGS__]
 ** @endcode
 **
 ** The variable length list then is referred by the reserved
 ** identifier @c __VA_ARGS__.
 **
 ** This functionality of C99 allows us e.g to implement macros for
 ** @ref defaults and to do @ref unrolling.
 **
 ** @section inline Inline functions
 **
 ** The new keyword @c inline is a borrow from C++ with some slightly
 ** changed semantics. The important part for P99 is that functions
 ** can be @em defined in a header file (an not only declared) when
 ** specified as @c inline.
 **
 ** This allows us to define small wrappers in include files through
 ** macros, without generating conflicts in different compilation
 ** units. By that we can avoid one of the major drawbacks of C macro
 ** programming: a macro cannot @em define another macro. In addition
 ** functions, compared to macros, have other advantages
 ** - they are typesafe
 ** - their arguments are evaluated exactly once
 **
 **
 ** @section initializers Designated initializers
 **
 ** In C89, initialization of structures can be tedious and error prone:
 ** @code
 ** typedef struct toto toto;
 ** struct toto {
 **   unsigned a;
 **   double b;
 ** };
 ** .
 ** .
 ** toto A = { 0, 1 };
 ** @endcode
 **
 ** Components are initialized in the order of the type
 ** declaration. Here the @c 0 in the initializer is used to
 ** initialize the component @c A.a and the @c 1 for A.b.
 **
 ** Whenever the structure @c toto changes during a development
 ** process, we would have to revisit @em all initializations to see
 ** whether or not the are still consistent:
 ** - if the order of components @c a and @c b changes, the order of
 **   the expressions must be inversed
 ** - if we insert an element before @c a or @c b, the initialization
 **   of @c b by @c 1 in the example is replaced by the default
 **   initialization, namely @c 0.0.
 **
 ** Keeping track of these may be particularly difficult if the
 ** components are of similar types, such that an initializer for one
 ** is valid for the other.
 **
 ** With designated initializers this situation changes substantially:
 ** @code
 ** toto A = { .a = 0, .b = 1 };
 ** @endcode
 **
 ** By that we avoid all of the problems that are mentioned
 ** above. This scheme is robust against reordering and insertion of
 ** components. In a certain sense it is also robust against the
 ** renaming of components: all initializations will then simply fail
 ** at compile time, so it is easy to identify the problem spots.
 **
 ** For a more detailed discussion of initialization and P99 see @ref
 ** variableInit.
 **
 ** @section compound Compound literals
 **
 ** A compound literal is syntactically given as a compound
 ** initializer and a cast such as
 ** @code
 ** (int[2]){ 4, 5}
 ** (T){ .d = 1, .a = 10 }.
 ** @endcode
 **
 ** It is best seen to define a temporary object of the requested
 ** type, initialized with the same rules as would a named variable of
 ** that type.
 **
 **  - Such unnamed temporary objects can be initialized on the fly, e.g
 **     as the arguments to functions, and they are alive to the end of
 **     the block in which they are defined.
 **  - They define an lvalue from which an address can be taken.
 **  - Unless the type of the cast is defined with `const' the content
 **     of such a variable is modifiable.
 **
 ** Example: The following returns the pointer to a character array
 ** that is initialized with all @c `a', a terminating @c 0 character and
 ** that is a valid objet until the program leaves the current block.
 ** @code
 ** char const*const hui = memset((char[256]){0}, 'a', 255);
 ** @endcode
 **
 ** It would be equivalent to the following
 ** @code
 ** char tmp[256] = { 0 };
 ** char const*const hui = memset(tmp, 'a', 255);
 ** @endcode
 **
 ** Using the compound literal here has the advantage that no other
 ** non-const reference to the temporary is exposed.
 **
 ** The compound literal syntax is not always very easy to read in
 ** fact it might even hurt your eyes. P99 gives you a shortcut for
 ** compound literals that are initialized from the all @c 0
 ** initializer. With that the above could have been written:
 **
 ** @code
 ** char const*const hui = memset(P99_LVAL(char[256]), 'a', 255);
 ** @endcode
 **
 ** @section hide Macros that hide a function
 **
 ** Per se, this is not a new feature of C99 but had been present
 ** before. The preprocessor has two special rules, one that applies
 ** generally to macros and the other that applies on to functional macros:
 **
 **  -# If during expansion of a macro XXX the token XXX is found, it
 **    is not expanded. So there is no recursion in C macros.
 **  -# If a functional macro YYY is found without a following
 **    opening parenthesis it is not expanded.
 **
 ** Theses features can be used to define a macro and another
 ** identifier that have the same name. It is sometimes used to all
 ** for a test if some functionality is present on a platform. E.g on
 ** my computer I have
 **
 ** @code
 ** #define stdin stdin
 ** @endcode
 **
 ** This can be used as follows
 ** @code
 ** #ifdef stdin
 **   // Do something for an hosted environment
 **   // Use stdin as usual
 ** #else
 **   // Do something for a free standing environment
 **   // We don't have stdin at all, write to a log file or so.
 ** #endif
 ** @endcode
 **
 ** But we may equally use this technique for a function symbol. POSIX
 ** explicitly allows this for example for the functions in @em stdio.h
 **
 ** <center>
 ** <em>
 ** The following shall be declared as functions and may also be
 ** defined as macros.<br />
 ** Function prototypes shall be provided.
 ** </em>
 ** </center>
 **
 ** Lets have a look at some random function from there and suppose it
 ** would be given as follows:
 ** @code
 ** int putc(int, FILE *);
 ** #define putc(C, F) (is_it_special(C) ? do_something_clever(C, F) : putc(C, F) )
 ** @endcode
 **
 ** (Yes this evaluates @c C two times.)  With that, these uses of @c
 ** putc are still valid:
 ** @code
 ** // Use the macro and implicitly the function, relies on rule 1
 ** putc('A', stdout);
 **
 ** // Just use the function not the macro, relies on rule 2
 ** (putc)('A', stdout);
 **
 ** // Get the address of putc and store it in my_putc, relies on rule 2
 ** int (*my_putc)(int, FILE*) = &putc;
 ** @endcode
 **
 ** The example above with @c putc has a particular pitfall if we have
 ** the above definitions in a header file and then include this file
 ** at the place where we define the the function:
 **
 ** @code
 ** #include <stdio.h>
 **
 ** int putc(int, FILE *) {
 **   // do the right thing here
 ** }
 ** @endcode
 **
 ** This will simply explode since the preprocessor will expand the
 ** functional reference to @c putc. This can be explicitly avoided
 ** by undefining the macro before the definition, but for this the
 ** implementor of @c putc has to know that it is also a macro.
 **
 ** With P99, we use this technique to @em overload a function to
 ** provide it with @ref defaults. A macro defined in that way will
 ** avoid this pitfall: if it is called with the same number of
 ** arguments (or more) that all are non-empty, it will produce the
 ** same token sequence as if the macro had not been defined.
 **
 ** @subsection pragma Pragmas inside macros
 **
 ** The traditional approach of C had been to specify meta information
 ** for the compiler in so called pragmas:
 ** @code
 ** #pragma omp parallel for
 ** for (size_t i = 0; i < n; ++i) c[i] += a[i] * b[i];
 ** @endcode
 **
 ** The inconvenience of such a construct is that it has always to be
 ** on a line of its own and could not be placed in a macro. For that
 ** reason most compilers provided extensions that let the programmer
 ** place meta information more precisely at some specific point of
 ** the code, e.g gcc has an @c __attribute__ extension for that.
 **
 ** C99 adds a keyword to overcome that difficulty and to normalize
 ** the link between macros and @c #pragma: @c _Pragma.
 ** @code
 ** _Pragma("omp parallel for") for (size_t i = 0; i < n; ++i) c[i] += a[i] * b[i];
 ** @endcode
 ** P99 uses this feature for extensions concerning OpenMP, in
 ** particular the ::P99_PARALLEL_FOR and ::P99_PARALLEL_FORALL
 ** macros.
 **
 **/



#include "p99_type.h"
#include "p99_block.h"
#include "p99_choice.h"
#include "p99_defarg.h"
#include "p99_enum.h"
#include "p99_int.h"
#include "p99_new.h"
#include "p99_double.h"
#include "p99_swap.h"

#endif 	    /* !P99_H_ */
