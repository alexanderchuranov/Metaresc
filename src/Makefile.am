# -*- makefile -*-

AM_CFLAGS = --fast-math -g -O2 -Wall -std=gnu99

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

lib_LTLIBRARIES = libreslib.la 

libreslib_la_SOURCES = reslib.c rl_stringify.c rl_save.c rl_load.c xdr.c xml1_save.c cinit_save.c scm_save.c tsearch.c
if HAVE_BISON_FLEX
  libreslib_la_SOURCES += xml1_load.tab.c xml1_load.lex.c cinit_load.tab.c cinit_load.lex.c scm_load.tab.c scm_load.lex.c
endif
if HAVE_LIBXML2
  libreslib_la_SOURCES += xml2_save.c xml2_load.c
endif

pkginclude_HEADERS = reslib.h rlprotos.h rlexport.h p99/*

clean-local:
	rm -f *.tab.[ch] *.lex.[ch]

xml1_load.tab.c: xml1_load.y xml1_load.lex.h
xml1_load.lex.c: xml1_load.l xml1_load.tab.h
cinit_load.tab.c: cinit_load.y cinit_load.lex.h
cinit_load.lex.c: cinit_load.l cinit_load.tab.h
scm_load.tab.c: scm_load.y scm_load.lex.h
scm_load.lex.c: scm_load.l scm_load.tab.h
