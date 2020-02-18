#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([META data and RESource library for C language], [1.0], [hnarkaytis@gmail.com])
AM_INIT_AUTOMAKE([1.0])
AC_CONFIG_SRCDIR([src/mr_protos.h])
AC_CONFIG_HEADERS([src/mr_config.h src/mr_export.h])
AC_CONFIG_MACRO_DIR([m4])
AC_LANG(C)

# Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AM_PROG_LIBTOOL
test -n "${YACC}" || YACC="bison"
AC_PROG_YACC
AC_PROG_LEX

AM_CFLAGS=""
aux_CFLAGS=--fast-math
save_CFLAGS=${CFLAGS}
CFLAGS="${CFLAGS} ${aux_CFLAGS}"
AC_MSG_CHECKING([${CC} accepts flag ${aux_CFLAGS}])
AC_COMPILE_IFELSE( [AC_LANG_PROGRAM( [[]], [[]])], [ AM_CFLAGS="${AM_CFLAGS} ${aux_CFLAGS}"; AC_MSG_RESULT([yes])], [AC_MSG_RESULT([no])])
CFLAGS=${save_CFLAGS}

aux_CFLAGS=-Wall
save_CFLAGS=${CFLAGS}
CFLAGS="${CFLAGS} ${aux_CFLAGS}"
AC_MSG_CHECKING([${CC} accepts flag ${aux_CFLAGS}])
AC_COMPILE_IFELSE( [AC_LANG_PROGRAM( [[]], [[]])], [ AM_CFLAGS="${AM_CFLAGS} ${aux_CFLAGS}"; AC_MSG_RESULT([yes])], [AC_MSG_RESULT([no])])
CFLAGS=${save_CFLAGS}

aux_CFLAGS=-Werror
save_CFLAGS=${CFLAGS}
CFLAGS="${CFLAGS} ${aux_CFLAGS}"
AC_MSG_CHECKING([${CC} accepts flag ${aux_CFLAGS}])
AC_COMPILE_IFELSE( [AC_LANG_PROGRAM( [[]], [[]])], [ AM_CFLAGS="${AM_CFLAGS} ${aux_CFLAGS}"; AC_MSG_RESULT([yes])], [AC_MSG_RESULT([no])])
CFLAGS=${save_CFLAGS}

aux_CFLAGS=-Wno-initializer-overrides
save_CFLAGS=${CFLAGS}
CFLAGS="${CFLAGS} ${aux_CFLAGS}"
AC_MSG_CHECKING([${CC} accepts flag ${aux_CFLAGS}])
AC_COMPILE_IFELSE( [AC_LANG_PROGRAM( [[]], [[]])], [ AM_CFLAGS="${AM_CFLAGS} ${aux_CFLAGS}"; AC_MSG_RESULT([yes])], [AC_MSG_RESULT([no])])
CFLAGS=${save_CFLAGS}

aux_CFLAGS=-Wno-constant-conversion
save_CFLAGS=${CFLAGS}
CFLAGS="${CFLAGS} ${aux_CFLAGS}"
AC_MSG_CHECKING([${CC} accepts flag ${aux_CFLAGS}])
AC_COMPILE_IFELSE( [AC_LANG_PROGRAM( [[]], [[]])], [ AM_CFLAGS="${AM_CFLAGS} ${aux_CFLAGS}"; AC_MSG_RESULT([yes])], [AC_MSG_RESULT([no])])
CFLAGS=${save_CFLAGS}

aux_CFLAGS=-Wno-duplicate-decl-specifier
save_CFLAGS=${CFLAGS}
CFLAGS="${CFLAGS} ${aux_CFLAGS}"
AC_MSG_CHECKING([${CC} accepts flag ${aux_CFLAGS}])
AC_COMPILE_IFELSE( [AC_LANG_PROGRAM( [[]], [[]])], [ AM_CFLAGS="${AM_CFLAGS} ${aux_CFLAGS}"; AC_MSG_RESULT([yes])], [AC_MSG_RESULT([no])])
CFLAGS=${save_CFLAGS}

AM_LDFLAGS=""
aux_CFLAGS=-rdynamic
save_CFLAGS=${CFLAGS}
CFLAGS="${CFLAGS} ${aux_CFLAGS}"
AC_MSG_CHECKING([${CC} accepts flag ${aux_CFLAGS}])
AC_COMPILE_IFELSE( [AC_LANG_PROGRAM( [[]], [[]])], [ AM_LDFLAGS="${AM_LDFLAGS} ${aux_CFLAGS}"; AC_MSG_RESULT([yes])], [AC_MSG_RESULT([no])])
CFLAGS=${save_CFLAGS}

dnl The dlopen() function is in the C library for *BSD and in libdl on GLIBC-based systems
AC_SEARCH_LIBS([dladdr], [dl dld], [AC_DEFINE(HAVE_LIBDL, [], [define if dl is present])], [])
  
AC_CHECK_PROG(HAVE_BISON, ${YACC}, yes)
AC_CHECK_PROG(HAVE_FLEX, ${LEX}, yes)

AM_CONDITIONAL(HAVE_BISON_FLEX, test x${HAVE_BISON}${HAVE_FLEX} = xyesyes)
if [[ x${HAVE_BISON}${HAVE_FLEX} = xyesyes ]]; then
  AC_DEFINE(HAVE_BISON_FLEX, [], [Bison & Flex found])
fi

PKG_CHECK_MODULES([CHECK], [check >= 0.9.5], [ HAVE_CHECK=yes ], [] )
AM_CONDITIONAL(HAVE_CHECK, test x${HAVE_CHECK} = xyes)

# Checks for libraries.
AC_ARG_WITH([libxml2], AS_HELP_STRING([--with-libxml2], [Build with the ibxml2]))

if test "x$with_libxml2" != "xno"; then
  PKG_CHECK_MODULES([LIBXML2], [libxml-2.0 >= 2.0], [ HAVE_LIBXML2=yes ], [] )
fi

AM_CONDITIONAL(HAVE_LIBXML2, test x${HAVE_LIBXML2} = xyes)
if [[ x${HAVE_LIBXML2} = xyes ]]; then
  AC_DEFINE(HAVE_LIBXML2, [], [define if libxml2 is present])
  AM_CFLAGS="${AM_CFLAGS} ${LIBXML2_CFLAGS}"
fi

AC_SUBST(AM_CFLAGS, ${AM_CFLAGS})
AC_SUBST(AM_LDFLAGS, ${AM_LDFLAGS})

AC_CHECK_LIB(execinfo, backtrace, [ LIBS="${LIBS} -lexecinfo" ])

AC_CHECK_MEMBERS([struct xdr_ops.x_putint32, struct xdr_ops.x_getint32], , , [[#include <rpc/types.h>
#include <rpc/xdr.h>]])

# Checks for header files.
AC_CHECK_HEADERS([execinfo.h rpc/types.h values.h])
AC_CHECK_HEADER([rpc/types.h], [ HAVE_XDR=yes ])
AM_CONDITIONAL(HAVE_XDR, test x${HAVE_XDR} = xyes)

# Checks for library functions.
AC_CHECK_FUNCS([ backtrace xdr_int8_t xdr_uint8_t xdr_uint16_t xdr_u_int16_t xdr_uint32_t xdr_u_int32_t xdr_uint64_t xdr_u_int64_t])

AC_ARG_ENABLE(pp-depth,	[  --enable-pp-depth		set depth for preprocessor routines], PP_DEPTH=$enableval, PP_DEPTH=128)
AC_MSG_CHECKING(preprocessor routines depth)
AC_MSG_RESULT(${PP_DEPTH})
AC_DEFINE_UNQUOTED(MR_PP_DEPTH, ${PP_DEPTH}, [Preprocessor routines depth])

UNCOVERED_TYPE="long long"
SIZEOF_UNCOVERED_TYPE=64

for TYPE in char short int long "long long"
do
  AC_MSG_CHECKING([unsigned ${TYPE} compatibility with uint8_t, uint16_t, uint32_t, uint64_t ])
  AC_COMPILE_IFELSE( [AC_LANG_PROGRAM( [[#include <stdint.h>]], [[ (int)sizeof (struct { int:-!!(__builtin_types_compatible_p (unsigned ${TYPE}, uint8_t) | __builtin_types_compatible_p (unsigned ${TYPE}, uint16_t) | __builtin_types_compatible_p (unsigned ${TYPE}, uint32_t) | __builtin_types_compatible_p (unsigned ${TYPE}, uint64_t)); }); ]])], [ UNCOVERED_TYPE=${TYPE}; AC_MSG_RESULT([no])], [AC_MSG_RESULT([yes])])
done
AC_MSG_CHECKING(uncovered int type)
AC_MSG_RESULT(${UNCOVERED_TYPE})

for TYPE_SIZE in 8 16 32 64
do
  AC_COMPILE_IFELSE( [AC_LANG_PROGRAM( [[#include <stdint.h>]], [[ (int)sizeof (struct { int:-!!(sizeof (uint${TYPE_SIZE}_t) != sizeof (${UNCOVERED_TYPE})); }); ]])], [ SIZEOF_UNCOVERED_TYPE=${TYPE_SIZE} ])
done
AC_MSG_CHECKING(size of uncovered int type)
AC_MSG_RESULT(${SIZEOF_UNCOVERED_TYPE})

AC_DEFINE_UNQUOTED(MR_UNCOVERED_TYPE, ${UNCOVERED_TYPE}, [int type which is not compatible with uint8_t, uint16_t, uint32_t, uint64_t])
AC_DEFINE_UNQUOTED(MR_SIZEOF_UNCOVERED_TYPE, ${SIZEOF_UNCOVERED_TYPE}, [sizeof int type which is not compatible with uint8_t, uint16_t, uint32_t, uint64_t])

AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile examples/Makefile])
AC_OUTPUT
