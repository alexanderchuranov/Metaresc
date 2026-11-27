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
$ SAN="-g -O1 -fno-omit-frame-pointer -fsanitize=address,undefined -fno-sanitize=function"
$ ./configure CFLAGS="-I/usr/include/tirpc $SAN -fsanitize=fuzzer-no-link" \
    LIBS=-ltirpc CC=clang --without-libdwarf --disable-shared
$ make -C src
$ clang $SAN -fsanitize=fuzzer -DHAVE_CONFIG_H -Isrc -Ifuzz \
    -I/usr/include/libxml2 -I/usr/include/tirpc fuzz/fuzz_xdr_load.c \
    src/.libs/libmetaresc.a `xml2-config --libs` -lyaml -ltirpc -ldl -rdynamic \
    -o fuzz_xdr_load
```

One UBSan check is excluded: `function`, because the library calls handlers
through pointers of a different function type (reported in `mr_save.c`) and
every run is flooded with those reports. The library has no signed `__int128`
multiplication: Clang checks it for overflow with `__muloti4` from
compiler-rt, and the link of the fuzzer fails without it.

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

## Corpus

`corpus/xdr_load/` holds three kinds of inputs:

* `seed_*.xdr` - the seeds (see above)
* `regress_*.xdr` - inputs that crashed or were wrongly accepted by the
  loader before a fix
* files named by a SHA-1 hash - inputs that the fuzzer found and that add
  coverage. They were selected with `-merge=1` after a fuzzing run.

To refresh the coverage inputs, fuzz a copy of the corpus and merge the
result back. `-merge=1` only adds the inputs that reach new code:

```console
$ cp -r fuzz/corpus/xdr_load /tmp/grown
$ ./fuzz_xdr_load /tmp/grown -max_len=512 -max_total_time=300 -fork=4 -detect_leaks=0
$ ./fuzz_xdr_load -merge=1 -max_len=512 -detect_leaks=0 fuzz/corpus/xdr_load /tmp/grown
```

## Known issues

There are none at the moment. An input for an issue that is not fixed yet
goes into a `known_issues/` directory (add it to `EXTRA_DIST`), and moves
into `corpus/xdr_load/` with the fix, so that it becomes a regression test.

Regression inputs:

* `regress_array_count_overflow.xdr` - the input that the fuzzer found
  against the loader without the bounds checks (heap overflow in
  `xdr_load_array`, within the first 5 executions).
* `regress_count_mismatch.xdr` - a pointer and its count field are two
  separate values in the stream, and the count field from the stream did not
  match the number of loaded elements. The loader now requires the size
  field to match the loaded data exactly.
* `regress_opaque_flag.xdr` - the opaque flag on a field other than
  `MR_OPAQUE_DATA`. The flag was removed, and opaque data is selected by
  the name of the field only.
* `regress_content_ref_type.xdr`, `regress_content_ref_overflow.xdr` - a
  typed pointer that aliases the characters of a string.
* `regress_unsized_pointer_elements.xdr` - a pointer without a size field
  that owns more than one element.
