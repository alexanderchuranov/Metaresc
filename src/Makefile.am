# -*- makefile -*-

AM_CFLAGS = @AM_CFLAGS@
AM_CPPFLAGS = -I$(top_srcdir)/src

DISTCLEANFILES = mr_pp.h
MOSTLYCLEANFILES = *.tab.[ch] *.lex.[ch]

SUFFIXES = .y .l .lex.h .tab.h .lex.c .tab.c

.y.tab.h:
	$(YACC) -d $<

.y.tab.c:
	$(YACC) $<

.l.lex.h:
	$(LEX) --header-file=$@ -o $*.lex.c $<

.l.lex.c:
	$(LEX) -o $@ $<

pkgincludedir = $(includedir)

lib_LTLIBRARIES = libmetaresc.la 

libmetaresc_la_SOURCES = metaresc.c mr_stringify.c mr_save.c mr_load.c xml_save.c cinit_save.c scm_save.c mr_ic.c mr_tsearch.c mr_hsort.c mr_value.c
if HAVE_BISON_FLEX
  libmetaresc_la_SOURCES += xml1_load.tab.c xml1_load.lex.c cinit_load.tab.c cinit_load.lex.c scm_load.tab.c scm_load.lex.c
endif
if HAVE_LIBXML2
  libmetaresc_la_SOURCES += xml2_load.c
endif
if HAVE_XDR
  libmetaresc_la_SOURCES += xdr.c
endif

pkginclude_HEADERS = metaresc.h mr_protos.h mr_export.h mr_pp.h mr_tsearch.h

xml1_load.tab.c: xml1_load.y xml1_load.lex.h
xml1_load.lex.c: xml1_load.l xml1_load.tab.h
cinit_load.tab.c: cinit_load.y cinit_load.lex.h
cinit_load.lex.c: cinit_load.l cinit_load.tab.h
scm_load.tab.c: scm_load.y scm_load.lex.h
scm_load.lex.c: scm_load.l scm_load.tab.h
$(libmetaresc_la_OBJECTS): mr_pp.h
mr_pp.h: mr_pp.sh
	$(srcdir)/mr_pp.sh `grep MR_PP_DEPTH mr_export.h | cut -d " " -f 3` > mr_pp.h

valgrind:
	 $(MAKE) $(AM_MAKEFLAGS) check TESTS_ENVIRONMENT="$(LIBTOOL) --mode=execute $(VALGRIND)"
