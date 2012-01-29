#!/bin/bash

make -j check

cd src

gcov -a -o .libs ../../src/*.c *.c