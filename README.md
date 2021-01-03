# METARESC: META data and RESource library for C language

[![YourActionName Actions Status](https://github.com/alexanderchuranov/Metaresc/workflows/ci/badge.svg)](https://github.com/alexanderchuranov/Metaresc/actions)

METARESC is a software library written in the C programming language. It
allows declaring types using a special METARESC grammar. The variables
of types declared that way may be subsequently serialized into various
formats and deserialized later. To achieve this the library adds
metadata annotations to types. These annotations may be used for may
purposes far beyond just achieving the persistence.

## How to build:

Install external dependencies `autoconf`, `automake`, `libtool`, `pkg-config`, `flex`, `bison`, `libxml2-dev`, `check`, `libdwarf-dev`.
Clone Metaresc from the Github, configure and build according to the standard autoconf/automake process.

### Ubuntu

On Ubuntu, run the following command:

```console
$ apt-get install autoconf automake libtool pkg-config flex bison libxml2-dev check libdwarf-dev
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
### MacOs

On Big Sur install developer tools first:

```console
$ xcode-select --install
```

Install external dependencies:
```console
$ brew install autoconf automake libtool pkg-config flex bison libxml2 check
```

For DWARF (debug info format) support install libdwarf:

```console
$ git clone git://git.code.sf.net/p/libdwarf/code libdwarf-code
$ cd libdwarf-code
$ sh scripts/FIX-CONFIGURE-TIMES
$ ./configure
$ make
$ sudo make install
```

Checkout and build Metaresc:

```console
$ git clone https://github.com/alexanderchuranov/Metaresc.git
$ cd Metaresc
$ ./autogen.sh
$ ./configure HAVE_BISON=yes YACC=/usr/local/opt/bison/bin/bison LIBS="-lz -ll"
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
$ ./configure CFLAGS=-D__USE_MINGW_ANSI_STDIO HAVE_BISON=yes HAVE_FLEX=yes YACC=~/win_flex_bison3-latest/win_bison.exe LEX=~/win_flex_bison3-latest/win_flex.exe
```

Build and check library

```console
$ make check
```

## How to build sample app

Instruction below will cover scenario when Metaresc is build locally
and is not installed into the system.

Create a folder at the same level as Metaresc. Start with hello world
stub that will use `metaresc.h`.

```c
#include <stdio.h>
#include <stdlib.h>

#include <metaresc.h>
  
int main (int argc, char * argv[])
{
  printf ("Hello world!\n");
  return (EXIT_SUCCESS);
}
```

Compiler should be instructed where to find Metaresc header file, so
we should add `-I../Metaresc/src` as a compilation flag. Minimal
`Makefile` for this app should be as follows: 

```make
all: sample

CFLAGS += -I../Metaresc/src
```

As a next step we will add some custom structure type definition,
variable of that type and serialization of this variable into format
consumable by C compiler as a static initialization of that type. For
demonstration purposes sample app will define type for the binary tree
node with `char * value`. Metaresc provides two basic layers that
enables reflection in C. First is a macro language that replaces
standard type definition semantics. Second is an API to introspect
meta-data generated by macro language and serialize structures
according to this metadata. Macro language is pretty similar to
standard semantics of types definition in C, but still require some
learning curve. In the following example it should be
self-explanatory.

```c
#include <stdio.h>
#include <stdlib.h>

#include <metaresc.h>

TYPEDEF_STRUCT (tree_node_t,
		(char *, value),
		(tree_node_t *, left),
		(tree_node_t *, right),
		);
  
int main (int argc, char * argv[])
{
  tree_node_t root = {
    "root",
    (tree_node_t[]){ { "left" } },
    (tree_node_t[]){ { "right" } },
  };
  
  char * dump = MR_SAVE_CINIT (tree_node_t, &root);
  if (dump)
    {
      printf ("%s", dump);
      MR_FREE (dump);
    }
  return (EXIT_SUCCESS);
}
```

At this point we need to link our application against statically build
Metaresc library files. `Makefile` should be extended as follows:
```make
all: sample

CFLAGS += -I../Metaresc/src
LDLIBS += ../Metaresc/src/.libs/libmetaresc.a
```

Compilation with Clang will produce a warning about duplicated
'const' specifier: `sample.c:6:1: warning: duplicate 'const'
declaration specifier [-Wduplicate-decl-specifier]` You may suppress
it with the `-Wno-duplicate-decl-specifier` flag. I.e. final `Makefile` is
as follows:

```make
all: sample

CFLAGS += -I../Metaresc/src -Wno-duplicate-decl-specifier
LDLIBS += ../Metaresc/src/.libs/libmetaresc.a
```

Output of this sample application is as follows:
```c
{
  .value = "root",
  .left = (tree_node_t[]){
    {
      .value = "left",
      .left = NULL,
      .right = NULL
    }
  },
  .right = (tree_node_t[]){
    {
      .value = "right",
      .left = NULL,
      .right = NULL
    }
  }
}
```

You may find custom macro language for types definition as an
overkill. In this case you could use Metaresc macro language only for
metadata generation and keep definitions of your types in a plain
C. It basically means that you need duplicate type definition:
once in standard C and one more time with a Metaresc macro
language. In this case global preprocessor variable `MR_MODE` should
be defined into `DESC`. Example above will look as follows:

```c
#include <stdio.h>
#include <stdlib.h>

#include <metaresc.h>

typedef struct tree_node_t {
  char * value;
  struct tree_node_t * left;
  struct tree_node_t * right;
} tree_node_t;

#define MR_MODE DESC
TYPEDEF_STRUCT (tree_node_t,
		(char *, value),
		(tree_node_t *, left),
		(tree_node_t *, right),
		);
  
int main (int argc, char * argv[])
{
  tree_node_t root = {
    "root",
    (tree_node_t[]){ { "left" } },
    (tree_node_t[]){ { "right" } },
  };
  
  char * dump = MR_SAVE_CINIT (tree_node_t, &root);
  if (dump)
    {
      printf ("%s", dump);
      MR_FREE (dump);
    }
  return (EXIT_SUCCESS);
}
```

The good news is that you could omit types specification in macro
language for all fields of basic types (boolean, integer, float,
complex, single characters, strings, pointers on all types listed
above and even pointers on structures of base type). So example above
could be reduced to:
```c
#include <stdio.h>
#include <stdlib.h>

#include <metaresc.h>

typedef struct tree_node_t {
  char * value;
  struct tree_node_t * left;
  struct tree_node_t * right;
} tree_node_t;

#define MR_MODE DESC
TYPEDEF_STRUCT (tree_node_t, value, left, right);
  
int main (int argc, char * argv[])
{
  tree_node_t root = {
    "root",
    (tree_node_t[]){ { "left" } },
    (tree_node_t[]){ { "right" } },
  };
  
  char * dump = MR_SAVE_CINIT (tree_node_t, &root);
  if (dump)
    {
      printf ("%s", dump);
      MR_FREE (dump);
    }
  return (EXIT_SUCCESS);
}
```

I would expect that some of the users still will find this a bit
challenging and overcomplicated. For those users library provides a
way to generate metadata from compiler debug symbols. It adds some
complexity to a build process, but doesn't require any interaction
with a macro language. In this case sample above will look as follows:

```c
#include <stdio.h>
#include <stdlib.h>

#include <metaresc.h>

typedef struct tree_node_t {
  char * value;
  struct tree_node_t * left;
  struct tree_node_t * right;
} tree_node_t;

int main (int argc, char * argv[])
{
  tree_node_t root = {
    "root",
    (tree_node_t[]){ { "left" } },
    (tree_node_t[]){ { "right" } },
  };
  
  char * dump = MR_SAVE_CINIT (tree_node_t, &root);
  if (dump)
    {
      printf ("%s", dump);
      MR_FREE (dump);
    }
  return (EXIT_SUCCESS);
}
```
You need to add one more file to the project `sample_types.c`:

```c
#include <metaresc.h>

mr_td_t mr_td[] = {
#include "sample_types.h"
};

static void __attribute__ ((constructor)) mr_types_init ()
{
  int i;
  for (i = 0; i < sizeof (mr_td) / sizeof (mr_td[0]); ++i)
    mr_add_type (&mr_td[i]);
}
```

`Makefile` should be modified as follows:

```make
all: sample

sample: sample.o sample_types.o
sample_no_types: sample.o
	$(LINK.c) $^ $(LOADLIBES) $(LDLIBS) -o $@
sample_types.o: sample_types.c sample_types.h
sample_types.h: sample_no_types
ifeq ($(shell uname), Darwin)
	dsymutil $?
endif
	../Metaresc/src/mr_dwarf $? > $@

clean:
	$(RM) *.o sample_types.h

CFLAGS += -I../Metaresc/src -Wno-duplicate-decl-specifier -O2 -g -Wall
LDLIBS += ../Metaresc/src/.libs/libmetaresc.a
```

By default library provides serialization/deserialization to/from
following formats:
* CINIT - format consumable by C compiler for static initialization of
corresponding types
* JSON - [JavaScript Object Notation](https://www.json.org/json-en.html)
* XML1 - builtin parser/serializer of XML
* XML2 - interface to libxml2
* SCM - Lisp-like notation
* XDR - binary format for [External Data Representation
Standard](https://tools.ietf.org/html/rfc4506)
