# Fuzzing

Fuzzers feed mutated inputs to the deserializers and rely on sanitizers to
report memory errors. The tests only load what the library has saved, so
they never meet a stream with a wrong length or count.

Currently there is one target, the XDR loader (`fuzz_xdr_load.c`).

## Files

* `fuzz_xdr_types.h` - types that are loaded by the fuzzer. The arrays are
  the last fields, so an overflow runs out of the object and
  AddressSanitizer reports it
* `fuzz_xdr_load.c` - the harness (`LLVMFuzzerTestOneInput`)
* `fuzz_replay_main.c` - `main()` that feeds files to the harness
* `fuzz_xdr_seed.c` - writes the seed inputs
* `corpus/xdr_load/` - inputs that are replayed by `make check`

## Replay in `make check`

`make check` builds the harness with `fuzz_replay_main.c` and runs it on
every file in `corpus/xdr_load/`. This works with any compiler and on any
platform where XDR is available. Run it under sanitizers (the CI job with
`-fsanitize=address,undefined`) to make memory errors visible.

A replay can also be run by hand on any files or directories:

```console
$ fuzz/fuzz_xdr_load_replay crash-0123abcd
```

## Fuzzing with libFuzzer

libFuzzer needs Clang with the compiler runtimes (`libclang-rt-dev` on
Ubuntu) and runs on Linux. Build the library statically with coverage
instrumentation, then link the harness with `-fsanitize=fuzzer`:

```console
$ SAN="-g -O1 -fno-omit-frame-pointer -fsanitize=address,undefined -fno-sanitize=signed-integer-overflow,function"
$ ./configure CFLAGS="-I/usr/include/tirpc $SAN -fsanitize=fuzzer-no-link" \
    LIBS=-ltirpc CC=clang --without-libdwarf --disable-shared
$ make -C src
$ clang $SAN -fsanitize=fuzzer -DHAVE_CONFIG_H -Isrc -Ifuzz \
    -I/usr/include/libxml2 -I/usr/include/tirpc fuzz/fuzz_xdr_load.c \
    src/.libs/libmetaresc.a `xml2-config --libs` -lyaml -ltirpc -ldl -rdynamic \
    -o fuzz_xdr_load
```

Two UBSan checks are excluded. `signed-integer-overflow`, because for
`__int128` multiplication (`mr_value_mul`) Clang emits calls to `__muloti4`
from compiler-rt, and the link fails without it. `function`, because the
library calls handlers through pointers of a different function type
(reported in `mr_save.c`) and every run is flooded with those reports.

Run it on a copy of the corpus, because libFuzzer adds new inputs to the
directory it is given:

```console
$ cp -r fuzz/corpus/xdr_load /tmp/corpus
$ ./fuzz_xdr_load /tmp/corpus -max_len=512 -max_total_time=120 -detect_leaks=0
```

After a successful load the harness frees the object recursively, which
fails if the loaded object is not consistent (see Known issues). Build the
harness with `-DFUZZ_LOADER_ONLY` to skip that step and fuzz the loader
alone.

The harness refuses allocations above 1 MiB. Without that a few mutated
bytes in a length field make the loader request gigabytes, and the fuzzer
stops on out-of-memory before it reaches anything else. Library messages
are silenced to keep iterations fast.

## Adding a regression input

A crash is written to a file (`crash-<hash>`). Minimize it, check that the
replay fails without the fix and passes with it, and then copy it into
`corpus/xdr_load/`:

```console
$ ./fuzz_xdr_load -minimize_crash=1 crash-0123abcd
$ cp minimized-from-0123abcd fuzz/corpus/xdr_load/
```

## Seeds

The seeds are written by `fuzz_xdr_seed.c`. It has to be run again after
the types in `fuzz_xdr_types.h` change:

```console
$ gcc -DHAVE_CONFIG_H -Isrc -Ifuzz -I/usr/include/libxml2 -I/usr/include/tirpc \
    fuzz/fuzz_xdr_seed.c src/.libs/libmetaresc.a `xml2-config --libs` \
    -lyaml -ltirpc -ldl -rdynamic -o fuzz_xdr_seed
$ ./fuzz_xdr_seed fuzz/corpus/xdr_load
```

## Known issues

Inputs in `known_issues/` fail now. Move them into `corpus/xdr_load/` when
the issue is fixed, so that they become regression tests.

* `xdr_count_mismatch.xdr` - a pointer and its count field are two separate
  values in the stream. `xdr_load_pointer` sets the count from the size of
  the allocation, but the count field is loaded after the pointer and
  overwrites it with the value from the stream. The load succeeds with
  `nodes_count` far above the number of elements that were allocated, and
  `MR_FREE_RECURSIVELY` (like any other traversal of the object) reads past
  the end of the block. The text loaders set the sizes of the pointers in a
  pass after all fields are loaded (`mr_load_pointer_postponed`). It is not
  related to the bounds checks in the XDR loader: the input fails the same
  way without them.
* `xdr.c:403` (`xdr_int64_t`) - UBSan reports a left shift of a negative
  value. It is reported for any input with a negative high word and does not
  stop the run.

`corpus/xdr_load/regress_array_count_overflow.xdr` is the input that the
fuzzer found against the loader without the bounds checks (heap overflow in
`xdr_load_array`, within the first 5 executions).
