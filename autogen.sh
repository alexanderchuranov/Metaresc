#! /bin/sh

# Run this to generate all the auto-generated files needed by the GNU
# configure program

aclocal
libtoolize --force
autoheader
automake --add-missing
autoconf
