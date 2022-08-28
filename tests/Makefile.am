# -*- makefile -*-

AUTOMAKE_OPTIONS = parallel-tests
AM_CFLAGS = @AM_CFLAGS@ $(CHECK_CFLAGS)
AM_LDFLAGS = @AM_LDFLAGS@
AM_CPPFLAGS = -I$(top_srcdir)/src
LIBS += ../src/libmetaresc.la $(CHECK_LIBS) $(LIBXML2_LIBS)
VALGRIND = valgrind --leak-check=full --error-exitcode=1 --trace-children=yes --quiet
CALLGRIND = valgrind --tool=callgrind --collect-jumps=yes --quiet

if HAVE_CHECK

  check_PROGRAMS = long_double double float string int8 int16 int32 int64 enum char array union mr_ptr bitfield pointer_int pointer_other resizable_pointer bool complex_float complex_double complex_long_double function ic mr_types mem_allocations mr_copy mr_hash_cmp dw_types

if HAVE_EXECINFO
  check_PROGRAMS += mf_mr_save mf_mr_copy
endif

if HAVE_XDR
  check_PROGRAMS += smoke_xdr
if HAVE_EXECINFO
  check_PROGRAMS += mf_xdr_save mf_xdr_load
endif
endif

if HAVE_BISON_FLEX
  check_PROGRAMS += smoke_cinit smoke_json smoke_scm smoke_xml1 expr
if HAVE_EXECINFO
  check_PROGRAMS += mf_cinit_save mf_cinit_load mf_json_save mf_json_load mf_scm_save mf_scm_load mf_xml1_save mf_xml1_load
endif
endif

if HAVE_LIBXML2
  check_PROGRAMS += smoke_xml2
if HAVE_EXECINFO
  check_PROGRAMS += mf_xml2_save mf_xml2_load
endif
endif

if HAVE_LIBDWARF
  check_PROGRAMS += dw_export
endif

endif

dw_export.$(OBJEXT): dw_export.h dw_types.h
dw_export.h: ../src/libmetaresc.la
if OSX
	dsymutil ../src/.libs/`$(SED) -ne "/^dlname='/{s/^dlname='\(.*\)'/\1/;p;}" $?`
endif
	../src/mr_dwarf ../src/.libs/`$(SED) -ne "/^dlname='/{s/^dlname='\(.*\)'/\1/;p;}" $?` > $@

dw_types.h: dw_types$(EXEEXT)
if OSX
	dsymutil .libs/$?
endif
	../src/mr_dwarf .libs/$? > $@

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

valgrind: ../src/libmetaresc.la
if HAVE_LIBDWARF
	LD_LIBRARY_PATH="../src/.libs; $(LD_LIBRARY_PATH)" $(VALGRIND) ../src/.libs/mr_dwarf ../src/.libs/`$(SED) -ne "/^dlname='/{s/^dlname='\(.*\)'/\1/;p;}" $?` > /dev/null
endif
	$(MAKE) $(AM_MAKEFLAGS) check LOG_COMPILER="$(LOG_COMPILER) $(LIBTOOL) $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=execute $(VALGRIND)"

callgrind:
	$(MAKE) $(AM_MAKEFLAGS) check LOG_COMPILER="$(LOG_COMPILER) $(LIBTOOL) $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=execute $(CALLGRIND)" CK_FORK=no
