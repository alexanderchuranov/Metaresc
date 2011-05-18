# -*- makefile -*-

AM_CFLAGS = --fast-math -g -O2 -Wall -std=gnu99

noinst_PROGRAMS = gjobs basic separate emptystruct justabuiltin builtins_auto

gjobs_SOURCES = gjobs.c
gjobs_LDADD = ../src/libreslib.la

basic_SOURCES = basic.c
basic_LDADD = ../src/libreslib.la

separate_SOURCES = separate.c separate_employee.c
separate_LDADD = ../src/libreslib.la

emptystruct_SOURCES = emptystruct.c
emptystruct_LDADD = ../src/libreslib.la

justabuiltin_SOURCES = justabuiltin.c
justabuiltin_LDADD = ../src/libreslib.la

builtins_auto_SOURCES = builtins_auto.c
builtins_auto_LDADD = ../src/libreslib.la
