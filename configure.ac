#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([META data and RESource library for C language],[1.0],[hnarkaytis@gmail.com])
AC_CONFIG_AUX_DIR([.])
AM_INIT_AUTOMAKE([1.0])
AC_CONFIG_SRCDIR([src/mr_protos.h])
AC_CONFIG_HEADERS([src/mr_config.h src/mr_export.h])
AC_CONFIG_MACRO_DIR([m4])
AC_LANG(C)
AC_CANONICAL_HOST

case $host_os in
mingw*)
  lt_cv_deplibs_check_method=pass_all
  CFLAGS="${CFLAGS} -Wno-unused-command-line-argument -Wno-format -Wno-unused-but-set-variable -mno-ms-bitfields -D__USE_MINGW_ANSI_STDIO"
  ;;
esac

# Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AM_PROG_AR
LT_INIT([win32-dll])
test -n "${YACC}" || YACC="bison"
AC_PROG_YACC
AC_PROG_LEX

AM_CFLAGS=""
for aux_CFLAGS in -Wall -Werror -O2 -gdwarf-4 -Wno-initializer-overrides -Wno-bitfield-constant-conversion
do
  save_CFLAGS=${CFLAGS}
  CFLAGS="${CFLAGS} ${aux_CFLAGS}"
  AC_MSG_CHECKING([${CC} accepts flag ${aux_CFLAGS}])
  AC_COMPILE_IFELSE( [AC_LANG_PROGRAM( [[]], [[]])], [ AM_CFLAGS="${AM_CFLAGS} ${aux_CFLAGS}"; AC_MSG_RESULT([yes])], [AC_MSG_RESULT([no])])
  CFLAGS=${save_CFLAGS}
done

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
AC_ARG_WITH([libxml2], AS_HELP_STRING([--without-libxml2], [Build without libxml2]))

if test "x${with_libxml2}" != "xno"; then
  PKG_CHECK_MODULES([LIBXML2], [libxml-2.0 >= 2.0], [ HAVE_LIBXML2=yes ], [] )
fi

AM_CONDITIONAL(HAVE_LIBXML2, test x${HAVE_LIBXML2} = xyes)
if [[ x${HAVE_LIBXML2} = xyes ]]; then
  AC_DEFINE(HAVE_LIBXML2, [], [define if libxml2 is present])
  AM_CFLAGS="${AM_CFLAGS} ${LIBXML2_CFLAGS}"
  LIBS="${LIBS} ${LIBXML2_LIBS}"
fi

AC_ARG_WITH([libyaml], AS_HELP_STRING([--without-libyaml], [Build without libyaml]))

if test "x${with_libyaml}" != "xno"; then
  PKG_CHECK_MODULES([LIBYAML], [yaml-0.1 >= 0.2.0], [ HAVE_LIBYAML=yes ], [] )
fi

AM_CONDITIONAL(HAVE_LIBYAML, test x${HAVE_LIBYAML} = xyes)
if [[ x${HAVE_LIBYAML} = xyes ]]; then
  AC_DEFINE(HAVE_LIBYAML, [], [define if libyaml is present])
  AM_CFLAGS="${AM_CFLAGS} ${LIBYAML_CFLAGS}"
  LIBS="${LIBS} ${LIBYAML_LIBS}"
fi

AC_SUBST(AM_CFLAGS, ${AM_CFLAGS})
AC_SUBST(AM_LDFLAGS, ${AM_LDFLAGS})

AC_CHECK_LIB(execinfo, backtrace, [ LIBS="${LIBS} -lexecinfo" ])

AC_ARG_WITH([libdwarf], AS_HELP_STRING([--without-libdwarf], [Build without libdwarf]))
if [[ x${with_libdwarf} != xno ]]; then
  AC_CONFIG_SUBDIRS([libdwarf-code])
fi
AM_CONDITIONAL(HAVE_LIBDWARF, test x${with_libdwarf} != xno)

AC_ARG_WITH([perftest], AS_HELP_STRING([--without-perftest], [Build without performance tests]))
AM_CONDITIONAL(HAVE_PERFTEST, test x${with_perftest} != xno)

AC_CHECK_MEMBERS([struct xdr_ops.x_putint32, struct xdr_ops.x_getint32], , , [[#include <rpc/types.h>
#include <rpc/xdr.h>]])

# Checks for header files.
AC_CHECK_HEADERS([execinfo.h rpc/types.h values.h])

AC_CHECK_HEADER([execinfo.h], [ HAVE_EXECINFO=yes ])
AM_CONDITIONAL(HAVE_EXECINFO, test x${HAVE_EXECINFO} = xyes)

AC_CHECK_HEADER([rpc/types.h], [ HAVE_XDR=yes ])
AM_CONDITIONAL(HAVE_XDR, test x${HAVE_XDR} = xyes)

# Checks for library functions.
AC_CHECK_FUNCS([ backtrace xdr_int8_t xdr_uint8_t xdr_uint16_t xdr_u_int16_t xdr_uint32_t xdr_u_int32_t ])

AC_MSG_CHECKING([${CC} supports __builtin_dump_struct ])
AC_LINK_IFELSE( [AC_LANG_PROGRAM( [[#include <stdio.h>]], [[ struct { int x; } x; (void)__builtin_dump_struct (&x, &printf); ]])], [ HAVE_BUILTIN_DUMP_STRUCT=yes ], [ HAVE_BUILTIN_DUMP_STRUCT=no ])
AC_MSG_RESULT([${HAVE_BUILTIN_DUMP_STRUCT}])
if [[ x${HAVE_BUILTIN_DUMP_STRUCT} = xyes ]]; then
  AC_DEFINE(HAVE_BUILTIN_DUMP_STRUCT, [], [define if __builtin_dump_struct is present])
fi

AC_MSG_CHECKING([${CC} supports __builtin_dump_struct extra args ])
AC_LINK_IFELSE( [AC_LANG_PROGRAM( [[#include <stdio.h>]], [[ struct { int x; } x; (void)__builtin_dump_struct (&x, &printf, 0); ]])], [ HAVE_BUILTIN_DUMP_STRUCT_EXTRA_ARGS=yes ], [ HAVE_BUILTIN_DUMP_STRUCT_EXTRA_ARGS=no ])
AC_MSG_RESULT([${HAVE_BUILTIN_DUMP_STRUCT_EXTRA_ARGS}])
if [[ x${HAVE_BUILTIN_DUMP_STRUCT_EXTRA_ARGS} = xyes ]]; then
  AC_DEFINE(HAVE_BUILTIN_DUMP_STRUCT_EXTRA_ARGS, [], [define if __builtin_dump_struct supports extra args])
fi

AC_ARG_ENABLE(pp-depth,	[  --enable-pp-depth		set depth for preprocessor routines], PP_DEPTH=$enableval, PP_DEPTH=256)
AC_MSG_CHECKING(preprocessor routines depth)
AC_MSG_RESULT(${PP_DEPTH})
AC_DEFINE_UNQUOTED(MR_PP_DEPTH, ${PP_DEPTH}, [Preprocessor routines depth])
AC_SUBST(PP_DEPTH, ${PP_DEPTH})

AC_ARG_ENABLE(ra_ud_idx,	[  --enable-ra-ud-idx		set type for resizable array of union discriminators indexes], RA_UD_IDX=$enableval, RA_UD_IDX=uint8_t)
AC_MSG_CHECKING(type of union discriminators indexes)
AC_MSG_RESULT(${RA_UD_IDX})
AC_DEFINE_UNQUOTED(MR_RA_UD_IDX_TYPE, ${RA_UD_IDX}, [type of union discriminators indexes])

UNCOVERED_TYPE="long long"
SIZEOF_UNCOVERED_TYPE=64

for TYPE in char short int long "long long"
do
  AC_MSG_CHECKING([unsigned ${TYPE} compatibility with uint8_t, uint16_t, uint32_t, uint64_t ])
  AC_COMPILE_IFELSE( [AC_LANG_PROGRAM( [[#include <stdint.h>]], [[ (int)sizeof (struct { int:-!!(__builtin_types_compatible_p (unsigned ${TYPE}, uint8_t) | __builtin_types_compatible_p (unsigned ${TYPE}, uint16_t) | __builtin_types_compatible_p (unsigned ${TYPE}, uint32_t) | __builtin_types_compatible_p (unsigned ${TYPE}, uint64_t)); }); ]])], [ UNCOVERED_TYPE=${TYPE}; AC_MSG_RESULT([no])], [AC_MSG_RESULT([yes])])
done
AC_MSG_CHECKING(uncovered int type)
AC_MSG_RESULT(${UNCOVERED_TYPE})

AC_MSG_CHECKING(size of uncovered int type)
for TYPE_SIZE in 8 16 32 64
do
  AC_COMPILE_IFELSE( [AC_LANG_PROGRAM( [[#include <stdint.h>]], [[ (int)sizeof (struct { int:-!!(sizeof (uint${TYPE_SIZE}_t) != sizeof (${UNCOVERED_TYPE})); }); ]])], [ SIZEOF_UNCOVERED_TYPE=${TYPE_SIZE} ])
done
AC_MSG_RESULT(${SIZEOF_UNCOVERED_TYPE})

AC_DEFINE_UNQUOTED(MR_UNCOVERED_TYPE, ${UNCOVERED_TYPE}, [int type which is not compatible with uint8_t, uint16_t, uint32_t, uint64_t])
AC_DEFINE_UNQUOTED(MR_SIZEOF_UNCOVERED_TYPE, ${SIZEOF_UNCOVERED_TYPE}, [sizeof int type which is not compatible with uint8_t, uint16_t, uint32_t, uint64_t])

AC_CHECK_SIZEOF([long double])
AC_CHECK_SIZEOF([double])

AC_MSG_CHECKING(long double significant bytes)
MR_SIZEOF_LONG_DOUBLE=$ac_cv_sizeof_long_double
AC_RUN_IFELSE([
  AC_LANG_PROGRAM(
    [#include <stdio.h>],
    [static const long double ld = 0.3L; int i, size = 0; for (i = 0; i < sizeof (ld); ++i) if (*(((unsigned char*)&ld) + i)) ++size; fprintf (stderr, "%d", size);]
    )],
    [MR_SIZEOF_LONG_DOUBLE=$(./conftest$EXEEXT 2>&1)], [true], [true])
AC_DEFINE_UNQUOTED(MR_SIZEOF_LONG_DOUBLE, ${MR_SIZEOF_LONG_DOUBLE}, [long double significant bytes])
AC_MSG_RESULT(${MR_SIZEOF_LONG_DOUBLE})

AC_MSG_CHECKING(digits for float values)
MR_FLT_DIG=8
AC_RUN_IFELSE([
  AC_LANG_PROGRAM(
    [#include <stdio.h>
     #include <float.h>],
    [fprintf (stderr, "%d", FLT_DIG + 2);]
    )],
    [MR_FLT_DIG=$(./conftest$EXEEXT 2>&1)], [true], [true])
AC_DEFINE_UNQUOTED(MR_FLT_DIG, ${MR_FLT_DIG}, [digits for float values])
AC_MSG_RESULT(${MR_FLT_DIG})

AC_MSG_CHECKING(digits for double values)
MR_DBL_DIG=17
AC_RUN_IFELSE([
  AC_LANG_PROGRAM(
    [#include <stdio.h>
     #include <float.h>],
    [fprintf (stderr, "%d", DBL_DIG + 2);]
    )],
    [MR_DBL_DIG=$(./conftest$EXEEXT 2>&1)], [true], [true])
AC_DEFINE_UNQUOTED(MR_DBL_DIG, ${MR_DBL_DIG}, [digits for double values])
AC_MSG_RESULT(${MR_DBL_DIG})

AC_MSG_CHECKING(digits for long double values)
MR_DBL_DIG=35
AC_RUN_IFELSE([
  AC_LANG_PROGRAM(
    [#include <stdio.h>
     #include <float.h>],
    [fprintf (stderr, "%d", LDBL_DIG + 2);]
    )],
    [MR_LDBL_DIG=$(./conftest$EXEEXT 2>&1)], [true], [true])
AC_DEFINE_UNQUOTED(MR_LDBL_DIG, ${MR_LDBL_DIG}, [digits for long double values])
AC_MSG_RESULT(${MR_LDBL_DIG})

AC_MSG_CHECKING(if __int128 is supported)
AC_COMPILE_IFELSE( [AC_LANG_PROGRAM( [[]], [[ (void)(__int128)0; ]])], [ HAVE_INT128=yes], [ HAVE_INT128=no ])
AC_MSG_RESULT(${HAVE_INT128})
AM_CONDITIONAL(HAVE_INT128, test x${HAVE_INT128} = xyes)
if [[ x${HAVE_INT128} = xyes ]]; then
  AC_DEFINE(MR_HAVE_INT128, [], [define if __int128 is supported])
fi

AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile examples/Makefile])
AC_OUTPUT
