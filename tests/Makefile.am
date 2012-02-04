# -*- makefile -*-

AUTOMAKE_OPTIONS = parallel-tests
AM_CFLAGS = --fast-math -Wall -Werror
INCLUDES = -I$(top_srcdir)/src
LIBS += ../src/libreslib.la $(CHECK_LIBS)
VALGRIND = valgrind --leak-check=full --error-exitcode=1 --trace-children=yes --quiet
CALLGRIND = valgrind --tool=callgrind --collect-jumps=yes --quiet

check_PROGRAMS = smoke_xdr smoke_cinit smoke_json smoke_scm smoke_xml1 smoke_xml2 long_double double float rarray string int8 int16 int32 int64 enum char array union rl_ptr bitfield pointer_int pointer_other

TESTS=$(check_PROGRAMS)

valgrind:
	 $(MAKE) $(AM_MAKEFLAGS) check TESTS_ENVIRONMENT="$(LIBTOOL) --mode=execute $(VALGRIND)"

callgrind:
	 $(MAKE) $(AM_MAKEFLAGS) check TESTS_ENVIRONMENT="$(LIBTOOL) --mode=execute $(CALLGRIND)" CK_FORK=no
