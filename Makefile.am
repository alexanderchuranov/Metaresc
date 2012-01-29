## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src tests examples

docs: Doxyfile
	doxygen $<

distclean-local:
	find "(" -name '*.gcda' -o -name '*.gcno' -o -name '*.gcov' ")" -delete
