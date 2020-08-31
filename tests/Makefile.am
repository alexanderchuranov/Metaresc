# -*- makefile -*-

AUTOMAKE_OPTIONS = parallel-tests
AM_CFLAGS = @AM_CFLAGS@
AM_LDFLAGS = @AM_LDFLAGS@
AM_CPPFLAGS = -I$(top_srcdir)/src
LIBS += $(top_srcdir)/src/libmetaresc.la $(CHECK_LIBS) $(LIBXML2_LIBS)
VALGRIND = valgrind --leak-check=full --error-exitcode=1 --trace-children=yes --quiet
CALLGRIND = valgrind --tool=callgrind --collect-jumps=yes --quiet

if HAVE_CHECK
  check_PROGRAMS = mf_mr_save mf_mr_copy

if HAVE_XDR
  check_PROGRAMS += smoke_xdr mf_xdr_save mf_xdr_load
endif

if HAVE_BISON_FLEX
  check_PROGRAMS += smoke_cinit mf_cinit_save mf_cinit_load smoke_json mf_json_save mf_json_load smoke_scm mf_scm_save mf_scm_load smoke_xml1 mf_xml1_save mf_xml1_load expr
endif

if HAVE_LIBXML2
  check_PROGRAMS += smoke_xml2 mf_xml2_save mf_xml2_load
endif

  check_PROGRAMS += long_double double float string int8 int16 int32 int64 enum char array union mr_ptr bitfield pointer_int pointer_other resizable_pointer bool complex_float complex_double complex_long_double function

if HAVE_LIBDWARF
  check_PROGRAMS += dw_export
endif

check_PROGRAMS += ic mr_types mem_allocations mr_copy

endif

dw_export.$(OBJEXT): dw_export.h
dw_export.h: $(top_srcdir)/src/libmetaresc.la
	$(top_srcdir)/src/mr_dwarf $(top_srcdir)/src/.libs/`$(SED) -ne "/^dlname='/{s/^dlname='\(.*\)'/\1/;p;}" $<` > $@

mf_mr_save_LDADD = mem_failures.$(OBJEXT)
mf_mr_copy_LDADD = mem_failures.$(OBJEXT)
mf_xdr_save_LDADD = mem_failures.$(OBJEXT)
mf_xdr_load_LDADD = mem_failures.$(OBJEXT)
mf_cinit_save_LDADD = mem_failures.$(OBJEXT)
mf_cinit_load_LDADD = mem_failures.$(OBJEXT)
mf_json_save_LDADD = mem_failures.$(OBJEXT)
mf_json_load_LDADD = mem_failures.$(OBJEXT)
mf_scm_save_LDADD = mem_failures.$(OBJEXT)
mf_scm_load_LDADD = mem_failures.$(OBJEXT)
mf_xml1_save_LDADD = mem_failures.$(OBJEXT)
mf_xml1_load_LDADD = mem_failures.$(OBJEXT)
mf_xml2_save_LDADD = mem_failures.$(OBJEXT)
mf_xml2_load_LDADD = mem_failures.$(OBJEXT)

TESTS=$(check_PROGRAMS)

valgrind:
	 $(MAKE) $(AM_MAKEFLAGS) check TESTS_ENVIRONMENT="$(LIBTOOL) --mode=execute $(VALGRIND)"

callgrind:
	 $(MAKE) $(AM_MAKEFLAGS) check TESTS_ENVIRONMENT="$(LIBTOOL) --mode=execute $(CALLGRIND)" CK_FORK=no
