# -*- makefile -*-

AUTOMAKE_OPTIONS = parallel-tests
AM_CFLAGS = @AM_CFLAGS@ @LIBXML2_CFLAGS@
AM_LDFLAGS = -rdynamic
INCLUDES = -I$(top_srcdir)/src
LIBS += ../src/libmetaresc.la $(CHECK_LIBS) $(LIBXML2_LIBS)
VALGRIND = valgrind --leak-check=full --error-exitcode=1 --trace-children=yes --quiet
CALLGRIND = valgrind --tool=callgrind --collect-jumps=yes --quiet

if HAVE_CHECK
  check_PROGRAMS = smoke_xdr

if HAVE_BISON_FLEX
  check_PROGRAMS += smoke_cinit smoke_json smoke_scm smoke_xml1 
endif

if HAVE_LIBXML2
  check_PROGRAMS += smoke_xml2
endif

  check_PROGRAMS += long_double double float rarray string int8 int16 int32 int64 enum char array union mr_ptr bitfield pointer_int pointer_other mr_copy bool complex_float complex_double complex_long_double expr
endif

TESTS=$(check_PROGRAMS)

valgrind:
	 $(MAKE) $(AM_MAKEFLAGS) check TESTS_ENVIRONMENT="$(LIBTOOL) --mode=execute $(VALGRIND)"

callgrind:
	 $(MAKE) $(AM_MAKEFLAGS) check TESTS_ENVIRONMENT="$(LIBTOOL) --mode=execute $(CALLGRIND)" CK_FORK=no
