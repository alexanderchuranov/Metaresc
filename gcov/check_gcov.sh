#!/bin/bash

make -j check

cd src

gcov -b -o .libs ../../src/*.c *.c