# METARESC: META data and RESource library for C language

[![YourActionName Actions Status](https://github.com/alexanderchuranov/Metaresc/workflows/ci/badge.svg)](https://github.com/alexanderchuranov/Metaresc/actions)

METARESC is a software library written in C programming language. It
allows declaring types using a special METARESC grammar. The variables
of types declared that way may be subsequently serialized into various
formats and deserialized later. To achieve this the library adds
metadata annotations to types. These annotations may be used for may
purposes far beyond just achieving the persistence.

## How to build:

Install external dependencies `autoconf`, `automake`, `libtool`, `pkg-config`, `flex`, `bison`, `libxml2-dev`, `check`.

### Ubuntu

On Ubuntu, run the following command:

```console
$ apt-get install autoconf automake libtool pkg-config flex bison libxml2-dev check
```

Checkout Metaresc from github

```console
$ git clone https://github.com/alexanderchuranov/Metaresc.git
```

Run autoconf/automake generators:

```console
$ ./autogen.sh
```

Configure project for target system

```console
$ ./configure
```

Build and check library

```console
$ make check
```

### Windows

On Windows one can use MinGW

* Download MinGW installer from here: https://sourceforge.net/projects/mingw/files/latest/download
* Choose Basic Setup > mingw-developer-toolkit && mingw32-base

Also:

* You will need Bison v 3+ and `pkg-config`.
* Download Bison and Flex from here: https://sourceforge.net/projects/winflexbison/files/latest/download
* Extract archive to user home dir in MSYS: `c:\MinGW\msys\1.0\home\UserName\`
* Download `pkg-config` from here: https://sourceforge.net/projects/pkgconfiglite/files/latest/download
* Extract archive to `c:\MinGW`

Optionally you could install `libxml2-dev` and `check`. It should be a similar process with prebuilt binaries.

Having this minimal configuration you could build the library using standard autoconf/automake toolchain. 

Checkout Metaresc from github

```console
$ git clone https://github.com/alexanderchuranov/Metaresc.git
```

Run autoconf/automake generators:

```console
$ ./autogen.sh
```

Configure project for target system

```console
$ CFLAGS=-D__USE_MINGW_ANSI_STDIO HAVE_BISON=yes HAVE_FLEX=yes YACC=~/win_flex_bison3-latest/win_bison.exe LEX=~/win_flex_bison3-latest/win_flex.exe ./configure
```

Build and check library

```console
$ make check
```
