# -*- makefile -*-

AM_CFLAGS = --fast-math -g -O2 -Wall -std=gnu99

noinst_PROGRAMS = test stackoverflow gjobs highload ptr_ext rb_tree
test_SOURCES = test.c
test_LDADD = ../src/libreslib.la
stackoverflow_SOURCES = stackoverflow.c
stackoverflow_LDADD = ../src/libreslib.la
gjobs_SOURCES = gjobs.c
gjobs_LDADD = ../src/libreslib.la
highload_SOURCES = highload.c
highload_LDADD = ../src/libreslib.la
ptr_ext_SOURCES = ptr_ext.c
ptr_ext_LDADD = ../src/libreslib.la
rb_tree_SOURCES = rb_tree.c

valgrind_test: ./test
	$(CC) -o test test.o ../src/.libs/libreslib.a $(LDFLAGS) $(LIBS)
	valgrind ./test

stackoverflow_test: ./stackoverflow
	$(CC) -o stackoverflow stackoverflow.o ../src/.libs/libreslib.a $(LDFLAGS) $(LIBS)
	valgrind ./stackoverflow

highload_test: ./highload
	$(CC) -o highload highload.o ../src/.libs/libreslib.a $(LDFLAGS) $(LIBS)
	time ./highload

gjobs_test: ./gjobs
	./gjobs < gjobs.xml
