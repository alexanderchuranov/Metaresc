# -*- makefile -*-

AM_CFLAGS = --fast-math -g -O2 -Wall -Werror
LIBS += ../src/libreslib.la $(CHECK_LIBS)
VALGRIND = valgrind --leak-check=full --error-exitcode=1 --trace-children=yes --quiet

check_PROGRAMS = xdr
TESTS=$(check_PROGRAMS)

valgrind:
	 $(MAKE) $(AM_MAKEFLAGS) check TESTS_ENVIRONMENT="$(LIBTOOL) --mode=execute $(VALGRIND)"

noinst_PROGRAMS = test stackoverflow highload ptr_ext rb_tree

valgrind_test: test
	$(LIBTOOL) --mode=execute valgrind $<

stackoverflow_test: stackoverflow
	$(LIBTOOL) --mode=execute valgrind $<

highload_test: highload
	$(LIBTOOL) --mode=execute time $<
