# -*- makefile -*-

AUTOMAKE_OPTIONS = parallel-tests
AM_CFLAGS = @AM_CFLAGS@ $(CHECK_CFLAGS)
AM_LDFLAGS = @AM_LDFLAGS@
AM_CPPFLAGS = -I$(top_srcdir)/src
LIBS += ../src/libmetaresc.la $(CHECK_LIBS)
VALGRIND = valgrind --leak-check=full --error-exitcode=1 --trace-children=yes
CALLGRIND = valgrind --tool=callgrind --collect-jumps=yes --quiet

if HAVE_CHECK

  check_PROGRAMS = long_double double float string int8 int16 int32 int64 enum char array union mr_ptr bitfield pointer_int pointer_other resizable_pointer bool complex_float complex_double complex_long_double function ic btree generic_ic mr_types mem_allocations mr_copy mr_hash_cmp

if HAVE_INT128
  check_PROGRAMS += int128
endif

if HAVE_PERFTEST
  check_PROGRAMS += union_resolution_complexity perf_save
endif

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

  expr.$(OBJEXT): CFLAGS += -ffast-math

if HAVE_PERFTEST
  check_PROGRAMS += perf_cinit perf_json perf_scm perf_xml1
endif
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

if HAVE_LIBYAML
  check_PROGRAMS += smoke_yaml
if HAVE_EXECINFO
  check_PROGRAMS += mf_yaml_save mf_yaml_load
endif
endif

if HAVE_LIBDWARF
  check_PROGRAMS += dw_export dw_types dw_types_full
  CLEANFILES = dw_export.h dw_types.h
endif

endif

dw_export.$(OBJEXT): dw_export.h dw_types.h
dw_types_init.$(OBJEXT): dw_types.h
dw_export.h: ../src/libmetaresc.la
	type dsymutil && dsymutil ../src/.libs/`$(SED) -ne "/^dlname='/{s/^dlname='\(.*\)'/\1/;p;}" $?` || echo "dsymutil not found"
	../src/mr_dwarf ../src/.libs/`$(SED) -ne "/^dlname='/{s/^dlname='\(.*\)'/\1/;p;}" $?` > $@

dw_types.h: dw_types$(EXEEXT)
	type dsymutil && dsymutil .libs/$? || echo "dsymutil not found"
	../src/mr_dwarf .libs/$? > $@

dw_types_full_SOURCES = dw_types.c
dw_types_full_LDADD = dw_types_init.$(OBJEXT)
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
mf_yaml_save_LDADD = mem_failures.$(OBJEXT)
mf_yaml_load_LDADD = mem_failures.$(OBJEXT)

TESTS=$(check_PROGRAMS)

valgrind: ../src/libmetaresc.la
if HAVE_LIBDWARF
	LD_LIBRARY_PATH="../src/.libs; $(LD_LIBRARY_PATH)" $(VALGRIND) ../src/.libs/mr_dwarf ../src/.libs/`$(SED) -ne "/^dlname='/{s/^dlname='\(.*\)'/\1/;p;}" $?` > /dev/null
endif
	$(MAKE) $(AM_MAKEFLAGS) check LOG_COMPILER="$(LOG_COMPILER) $(LIBTOOL) $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=execute $(VALGRIND)"

callgrind:
	$(MAKE) $(AM_MAKEFLAGS) check LOG_COMPILER="$(LOG_COMPILER) $(LIBTOOL) $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=execute $(CALLGRIND)" CK_FORK=no
