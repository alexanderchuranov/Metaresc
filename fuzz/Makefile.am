# -*- makefile -*-

AUTOMAKE_OPTIONS = parallel-tests
AM_CFLAGS = @AM_CFLAGS@
AM_LDFLAGS = @AM_LDFLAGS@
AM_CPPFLAGS = -I$(top_srcdir)/src -I$(srcdir)
LIBS += ../src/libmetaresc.la

EXTRA_DIST = README.md fuzz_xdr_types.h fuzz_xdr_seed.c corpus

# The libFuzzer build is not a part of the autotools build, see README.md.
# Here the same harness replays the corpus as a regular test.
if HAVE_XDR
  check_PROGRAMS = fuzz_xdr_load_replay
  fuzz_xdr_load_replay_SOURCES = fuzz_xdr_load.c fuzz_replay_main.c
  fuzz_xdr_load_replay_CPPFLAGS = $(AM_CPPFLAGS) -DFUZZ_CORPUS_DIR=\"$(srcdir)/corpus/xdr_load\"
  TESTS = $(check_PROGRAMS)
endif
