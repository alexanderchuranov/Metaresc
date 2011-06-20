#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

#AC_PREREQ(2.63)
AC_INIT([Metadata for C], [1.0], [hnarkaytis@gmail.com])
AM_INIT_AUTOMAKE([reslib], [1.0])
AC_CONFIG_SRCDIR([src/rlprotos.h])
AC_CONFIG_HEADERS([src/rlconfig.h src/rlexport.h])
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
AC_PROG_YACC
AC_PROG_LEX

YACC=bison

AC_ARG_VAR(HAVE_BISON, [Bison check])
if test x != x${YACC} && test -x ${YACC} ; then
  AC_MSG_CHECKING(${YACC})
  AC_MSG_RESULT(yes)
  HAVE_BISON=yes
else
  AC_CHECK_PROG(HAVE_BISON, $YACC, yes)
fi
AC_ARG_VAR(HAVE_FLEX, [Flex check])
if test x != x${LEX} && test -x ${LEX} ; then
  AC_MSG_CHECKING(${LEX})
  AC_MSG_RESULT(yes)
  HAVE_FLEX=yes
else
  AC_CHECK_PROG(HAVE_FLEX, $LEX, yes)
fi

AM_CONDITIONAL(HAVE_BISON_FLEX, test x${HAVE_BISON} = xyes && test x${HAVE_FLEX} = xyes)
if test x${HAVE_BISON} = xyes && test x${HAVE_FLEX} = xyes; then
  AC_DEFINE(HAVE_BISON_FLEX, [], [Bison & Flex found])
fi

# Checks for libraries.
XML2CONFIG="`which xml2-config 2> /dev/null`"
if test x != x${XML2CONFIG} && test -x ${XML2CONFIG} ; then
  CPPFLAGS="${CPPFLAGS} `${XML2CONFIG} --cflags`"
  LDFLAGS="${LDFLAGS} `${XML2CONFIG} --libs`"
  AC_CHECK_LIB(xml2, xmlKeepBlanksDefault, [HAVE_LIBXML2=yes], [HAVE_LIBXML2=no])
  if test x${HAVE_LIBXML2} = xyes ; then
    AC_DEFINE(HAVE_LIBXML2, [], [Libxml2 found])
  fi
else
  HAVE_LIBXML2=no
fi
AM_CONDITIONAL(HAVE_LIBXML2, test x${HAVE_LIBXML2} = xyes)


AC_CHECK_MEMBERS([struct xdr_ops.x_putint32, struct xdr_ops.x_getint32], , , [[#include <rpc/types.h>
#include <rpc/xdr.h>]])

# Checks for header files.
AC_FUNC_ALLOCA
AC_HEADER_STDC
AC_CHECK_HEADERS([inttypes.h libintl.h malloc.h stddef.h stdlib.h string.h unistd.h])

AC_MSG_CHECKING([for endian.h])
# Linux non-standard location
if test -e /usr/include/endian.h; then
  AC_DEFINE(ENDIAN_H, </usr/include/endian.h>, [ path to endian.h ])
  AC_MSG_RESULT([/usr/include/endian.h])
else
# BSD non-standard location
  if test -e /usr/include/machine/endian.h; then
    AC_DEFINE(ENDIAN_H, </usr/include/machine/endian.h>, [ path to endian.h ])
    AC_MSG_RESULT([/usr/include/machine/endian.h])
  else
# There is no standard location :-)
    AC_MSG_FAILURE([endian.h not found])
  fi
fi

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_REALLOC
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([memset strchr strndup strdup strncasecmp strstr xdr_int8_t xdr_uint8_t])

AC_ARG_ENABLE(tree-lookup,
	[  --enable-tree-lookup		enables red-black tree lookup for type descriptor search instead of non-collision hash],
	AC_DEFINE(RL_TREE_LOOKUP, [], [Red-black tree lookup is enabled]))
AC_ARG_ENABLE(pp-depth,	[  --enable-pp-depth		set depth for preprocessor routines], PP_DEPTH=$enableval, PP_DEPTH=128)
AC_MSG_CHECKING(preprocessor routines depth)
AC_MSG_RESULT(${PP_DEPTH})
AC_DEFINE_UNQUOTED(RL_PP_DEPTH, ${PP_DEPTH}, [Preprocessor routines depth])
./src/rlpp.sh ${PP_DEPTH} > src/rlpp.h

UNCOVERED_TYPE="long long"
SIZEOF_UNCOVERED_TYPE=64

TYPE="char"
AC_MSG_CHECKING([unsigned ${TYPE} compatibility with uint8_t, uint16_t, uint32_t, uint64_t ])
AC_COMPILE_IFELSE( [AC_LANG_PROGRAM( [[#include <stdint.h>]], [[ (int)sizeof (struct { int:-!!(__builtin_types_compatible_p (unsigned ${TYPE}, uint8_t) | __builtin_types_compatible_p (unsigned ${TYPE}, uint16_t) | __builtin_types_compatible_p (unsigned ${TYPE}, uint32_t) | __builtin_types_compatible_p (unsigned ${TYPE}, uint64_t)); }); ]])],
		   [ UNCOVERED_TYPE=${TYPE}; AC_MSG_RESULT([no])], [AC_MSG_RESULT([yes])])
TYPE="short"
AC_MSG_CHECKING([unsigned ${TYPE} compatibility with uint8_t, uint16_t, uint32_t, uint64_t ])
AC_COMPILE_IFELSE( [AC_LANG_PROGRAM( [[#include <stdint.h>]], [[ (int)sizeof (struct { int:-!!(__builtin_types_compatible_p (unsigned ${TYPE}, uint8_t) | __builtin_types_compatible_p (unsigned ${TYPE}, uint16_t) | __builtin_types_compatible_p (unsigned ${TYPE}, uint32_t) | __builtin_types_compatible_p (unsigned ${TYPE}, uint64_t)); }); ]])],
		   [ UNCOVERED_TYPE=${TYPE}; AC_MSG_RESULT([no])], [AC_MSG_RESULT([yes])])
TYPE="int"
AC_MSG_CHECKING([unsigned ${TYPE} compatibility with uint8_t, uint16_t, uint32_t, uint64_t ])
AC_COMPILE_IFELSE( [AC_LANG_PROGRAM( [[#include <stdint.h>]], [[ (int)sizeof (struct { int:-!!(__builtin_types_compatible_p (unsigned ${TYPE}, uint8_t) | __builtin_types_compatible_p (unsigned ${TYPE}, uint16_t) | __builtin_types_compatible_p (unsigned ${TYPE}, uint32_t) | __builtin_types_compatible_p (unsigned ${TYPE}, uint64_t)); }); ]])],
		   [ UNCOVERED_TYPE=${TYPE}; AC_MSG_RESULT([no])], [AC_MSG_RESULT([yes])])
TYPE="long"
AC_MSG_CHECKING([unsigned ${TYPE} compatibility with uint8_t, uint16_t, uint32_t, uint64_t ])
AC_COMPILE_IFELSE( [AC_LANG_PROGRAM( [[#include <stdint.h>]], [[ (int)sizeof (struct { int:-!!(__builtin_types_compatible_p (unsigned ${TYPE}, uint8_t) | __builtin_types_compatible_p (unsigned ${TYPE}, uint16_t) | __builtin_types_compatible_p (unsigned ${TYPE}, uint32_t) | __builtin_types_compatible_p (unsigned ${TYPE}, uint64_t)); }); ]])],
		   [ UNCOVERED_TYPE=${TYPE}; AC_MSG_RESULT([no])], [AC_MSG_RESULT([yes])])
TYPE="long long"
AC_MSG_CHECKING([unsigned ${TYPE} compatibility with uint8_t, uint16_t, uint32_t, uint64_t ])
AC_COMPILE_IFELSE( [AC_LANG_PROGRAM( [[#include <stdint.h>]], [[ (int)sizeof (struct { int:-!!(__builtin_types_compatible_p (unsigned ${TYPE}, uint8_t) | __builtin_types_compatible_p (unsigned ${TYPE}, uint16_t) | __builtin_types_compatible_p (unsigned ${TYPE}, uint32_t) | __builtin_types_compatible_p (unsigned ${TYPE}, uint64_t)); }); ]])],
		   [ UNCOVERED_TYPE=${TYPE}; AC_MSG_RESULT([no])], [AC_MSG_RESULT([yes])])
AC_MSG_CHECKING(uncovered int type)
AC_MSG_RESULT(${UNCOVERED_TYPE})

AC_COMPILE_IFELSE( [AC_LANG_PROGRAM( [[#include <stdint.h>]], [[ (int)sizeof (struct { int:-!!(sizeof (uint8_t) != sizeof (${UNCOVERED_TYPE})); }); ]])], [ SIZEOF_UNCOVERED_TYPE=8 ])
AC_COMPILE_IFELSE( [AC_LANG_PROGRAM( [[#include <stdint.h>]], [[ (int)sizeof (struct { int:-!!(sizeof (uint16_t) != sizeof (${UNCOVERED_TYPE})); }); ]])], [ SIZEOF_UNCOVERED_TYPE=16 ])
AC_COMPILE_IFELSE( [AC_LANG_PROGRAM( [[#include <stdint.h>]], [[ (int)sizeof (struct { int:-!!(sizeof (uint32_t) != sizeof (${UNCOVERED_TYPE})); }); ]])], [ SIZEOF_UNCOVERED_TYPE=32 ])
AC_COMPILE_IFELSE( [AC_LANG_PROGRAM( [[#include <stdint.h>]], [[ (int)sizeof (struct { int:-!!(sizeof (uint64_t) != sizeof (${UNCOVERED_TYPE})); }); ]])], [ SIZEOF_UNCOVERED_TYPE=64 ])
AC_MSG_CHECKING(size of uncovered int type)
AC_MSG_RESULT(${SIZEOF_UNCOVERED_TYPE})

AC_DEFINE_UNQUOTED(RL_UNCOVERED_TYPE, ${UNCOVERED_TYPE}, [int type which is not compatible with uint8_t, uint16_t, uint32_t, uint64_t])
AC_DEFINE_UNQUOTED(RL_SIZEOF_UNCOVERED_TYPE, ${SIZEOF_UNCOVERED_TYPE}, [sizeof int type which is not compatible with uint8_t, uint16_t, uint32_t, uint64_t])

AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile examples/Makefile])
AC_OUTPUT
