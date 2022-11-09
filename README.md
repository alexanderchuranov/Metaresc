# METARESC: META data and RESource library for C language

[![YourActionName Actions Status](https://github.com/alexanderchuranov/Metaresc/workflows/ci/badge.svg)](https://github.com/alexanderchuranov/Metaresc/actions)

METARESC is a software library written in the C programming language. It
allows declaring types using a special METARESC grammar. The variables
of types declared that way may be subsequently serialized into various
formats and deserialized later. To achieve this the library adds
metadata annotations to types. These annotations may be used for many
purposes far beyond just achieving the persistence.

<!-- markdown-toc start - Don't edit this section. Run M-x markdown-toc-generate-toc again -->
**Table of Contents**

- [METARESC: META data and RESource library for C language](#metaresc-meta-data-and-resource-library-for-c-language)
    - [How to build library:](#how-to-build-library)
        - [Ubuntu](#ubuntu)
        - [MacOs](#macos)
        - [FreeBSD](#freebsd)
        - [Windows](#windows)
    - [How to build a sample app](#how-to-build-a-sample-app)
    - [Serialization/deserialization interface](#serializationdeserialization-interface)
        - [Serialization](#serialization)
        - [Deserialization](#deserialization)
        - [MR_PRINT helper macro](#mr_print-helper-macro)
    - [Types declaration macro language](#types-declaration-macro-language)
        - [Structure type declaration](#structure-type-declaration)
            - [Fields of a basic types](#fields-of-a-basic-types)
            - [Extended semantics for fields declaration](#extended-semantics-for-fields-declaration)
            - [Non-serializable fields](#non-serializable-fields)
            - [Field declaration](#field-declaration)
            - [Pointer declaration](#pointer-declaration)
            - [Array declaration](#array-declaration)
            - [Function pointer declaration](#function-pointer-declaration)
            - [Bitfields declaration](#bitfields-declaration)
            - [Union declaration](#union-declaration)
            - [Text metadata and resource information](#text-metadata-and-resource-information)
            - [NULL-terminated strings vs character arrays](#null-terminated-strings-vs-character-arrays)
        - [Union type declaration](#union-type-declaration)
        - [Enumeration type declaration](#enumeration-type-declaration)
        - [Function pointer type declaration](#function-pointer-type-declaration)
        - [Metadata and resources definition for types](#metadata-and-resources-definition-for-types)
    - [Extra features](#extra-features)
        - [Deep copy](#deep-copy)
        - [Recursive memory deallocation](#recursive-memory-deallocation)
        - [Objects hashing](#objects-hashing)
        - [Comparation of structures](#comparation-of-structures)
        - [Structure of serialization graph](#structure-of-serialization-graph)
        - [Access and structure of type descriptor](#access-and-structure-of-type-descriptor)
        - [Access and structure of field descriptor](#access-and-structure-of-field-descriptor)
        - [Enumerations metadata](#enumerations-metadata)
    - [Internals](#internals)
        - [Error handling](#error-handling)
        - [Memory allocation](#memory-allocation)
        - [How to make certain field in type non-serializable](#how-to-make-certain-field-in-type-non-serializable)
        - [Formatted output to a resizable array](#formatted-output-to-a-resizable-array)
        - [Indexing framework](#indexing-framework)

<!-- markdown-toc end -->

## How to build library:

Install external dependencies `autoconf`, `automake`, `libtool`, `pkg-config`, `flex`, `bison`, `libxml2-dev`, `check`, `libdwarf-dev`.
Clone Metaresc from the Github, configure and build according to the standard autoconf/automake process.

### Ubuntu

On Ubuntu, run the following command:

```console
# apt-get install git autoconf automake libtool pkg-config flex bison libxml2-dev check
```

Checkout Metaresc from github

```console
$ git clone https://github.com/alexanderchuranov/Metaresc.git
$ cd Metaresc
$ git submodule update --init --recursive --remote
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
$ make -j 4 check
```
### MacOs

On Big Sur install developer tools first:

```console
$ xcode-select --install
```

Install external dependencies:
```console
# brew install autoconf automake libtool pkg-config flex bison libxml2 check
```

Checkout and build Metaresc:

```console
$ git clone https://github.com/alexanderchuranov/Metaresc.git
$ cd Metaresc
$ git submodule update --init --recursive --remote
$ ./autogen.sh
$ ./configure HAVE_BISON=yes YACC=/usr/local/opt/bison/bin/bison
$ make -j 4 check
```

### FreeBSD

On FreeBSD 12.1 install external dependencies:
```console
# pkg install git autoconf automake libtool pkgconf flex bison libxml2 check
```

Checkout and build Metaresc:

```console
$ git clone https://github.com/alexanderchuranov/Metaresc.git
$ cd Metaresc
$ git submodule update --init --recursive --remote
$ ./autogen.sh
$ ./configure
$ make -j 4 check
```

### Windows

Download and install msys2 from https://www.msys2.org/

Install external dependencies:

```console
# pacman --noconfirm -S --needed --overwrite "*" base-devel git autoconf automake libtool pkg-config flex bison mingw-w64-x86_64-check mingw-w64-x86_64-toolchain
```

Checkout and build Metaresc:

```console
$ git clone https://github.com/alexanderchuranov/Metaresc.git
$ cd Metaresc
$ git submodule update --init --recursive --remote
$ ./autogen.sh
$ ./configure --without-libxml2 --disable-static --enable-shared
$ sed -i.bak -e "s/\(allow_undefined=\)yes/\1no/" libtool
$ make -j 4 check
```

## How to build a sample app

Instruction below will cover scenario when Metaresc is build locally
and is not installed into the system.

Create a folder at the same level as Metaresc. Start with hello world
stub that will use `metaresc.h`.

```c
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

CFLAGS += -I../Metaresc/src `xml2-config --cflags`
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
  
  MR_PRINT ("tree = ", (tree_node_t, &root));
  return (EXIT_SUCCESS);
}
```

At this point we need to link our application against statically build
Metaresc library files. `Makefile` should be extended as follows:
```make
all: sample

CFLAGS += -I../Metaresc/src `xml2-config --cflags`
LDLIBS += ../Metaresc/src/.libs/libmetaresc.a `xml2-config --libs`
```

Output of this sample application is as follows:
```c
tree = {
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
  
  MR_PRINT ("tree = ", (tree_node_t, &root));
  return (EXIT_SUCCESS);
}
```

The good news is that you could omit types specification in macro
language for all fields of basic types (boolean, integer, float,
complex, single characters, strings, pointers on all types listed
above and even pointers on structures of base type). So example above
could be reduced to:
```c
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
  
  MR_PRINT ("tree = ", (tree_node_t, &root));
  return (EXIT_SUCCESS);
}
```

I would expect that some of the users still will find this a bit
challenging and overcomplicated. For those users library provides a
way to generate metadata from compiler debug symbols. It adds some
complexity to a build process, but doesn't require any interaction
with a macro language. In this case sample above will look as follows:

```c
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
  
  MR_PRINT ("tree = ", (tree_node_t, &root));
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
	type dsymutil && dsymutil $? || echo "dsymutil no found"
	../Metaresc/src/mr_dwarf $? > $@

clean:
	$(RM) *.o sample_types.h

CFLAGS += -I../Metaresc/src `xml2-config --cflags` -O2 -g -Wall
LDLIBS += ../Metaresc/src/.libs/libmetaresc.a `xml2-config --libs`
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

## Serialization/deserialization interface

### Serialization

`MR_SAVE_*` macroses do a serialization. 
`MR_SAVE_CINIT`/`MR_SAVE_JSON`/`MR_SAVE_XML1`/`MR_SAVE_XML2`/`MR_SAVE_SCM`
are string based serialization and require only two arguments: type of
the pointer and pointer on data. Type of the pointer by default should
be specified as C language token. Save macro do verification that
data pointer matches specified type and after that convert type to a
string. The rest of serialization process require type only as a
string. It might happen that user serialization function gets data
pointer as `void*` and type as a string. In this case you need to skip
type verification step and access searilization with stringified
type. Unfortunatelly library can't automatically detect in which form
type is provided, so you need to make some macro tricks for switching
between those two modes. All serialization macroses internally uses
macro `MR_SAVE (type, pointer)`. This macro serialize data into
graph-like internal representation, which later on gets converted to
specific formats. By default `MR_SAVE` is defined to `MR_SAVE_TYPED`
which expects `type` to be a C token. There is another underlaying
macro `MR_SAVE_STR_TYPED` which takes `type` as a string. User could
redefine `MR_SAVE` to `MR_SAVE_STR_TYPED` as needed.
There is also an option to skip `type` for `MR_SAVE_TYPED` macro and
provide only second parameter (data pointer). With Clang library will
detect type of the data pointer and will convert it to a string. On
GCC this feature is not supported and empty type will work only for
pointers on basic types (int, float, complex) which doesn't really
make much sense in real life use cases.

Data pointer might be a pointer on a local array. In this cases
library will serialize whole array. Example below demostrates
serialization of an array with auto detection of pointer type (works
both with GCC and Clang).
```c
  int array[] = {1, 2, 3};
  char * dump = MR_SAVE_CINIT ( , array);
  if (dump)
    {
      printf ("array = %s", dump);
      MR_FREE (dump);
    }
```
This will output:
```c
array = {
  1,
  2,
  3
}
```

In case of failure serialization macro will return `NULL`. More
details on error handling could be found in section [Error
handling](#error_handling).
Memory allocated by serialization macroses must be deallocated by
`MR_FREE` function. More details on memory allocation could be found
in section [Memory allocation](#memory_allocation)

`MR_SAVE_XDR (type, pointer, xdrs)` is a little bit different from the
rest. It requires 3 parameters. First two parameters are passed
directly to `MR_SAVE` and described above. The third parameted is an
XDR stream descriptor. This macro returns status of the operation as
an enum of type `mr_status_t`. Possible values of this enum are
`MR_SUCCESS` and `MR_FAILURE`.
Stream descriptor is a structure of type `XDR` that must be
initialized with designated constructor. LibC provides constructor
(`xdr_create`) for stream that will redirect serialized data to file
descriptor. Metaresc also provide constructor (`xdrra_create`) that
will store serialized data into memory. 
For convenience purposes library also provides wrapper macro
(`MR_SAVE_XDR_RA (type, pointer)`) that have all boiler plate 
code for serialization into memory and returns result in a form of
*resizable array* (`mr_rarray_t`). Declaration of type could be found
in mr_proto.h Other serialization methods also have corresponding
`MR_SAVE_*_RA` equivivalents for serialization to *resizable array*.

For serialization to XML user could either use specific method
(XML2 for libxml2 and XML1 for custom serialization) or just use
`MR_SAVE_XML`/`MR_SAVE_XML_RA` that will redirect to a first available
option (XML2 by default). XML1 custom serialization implemented to
fully match output of `xmlDocDumpFormatMemory ()` from libxml2. The
benefit of XML1 vs XML2 is a 40% reduction of the memory footprint and
comparable improvement in performance.

### Deserialization
`MR_LOAD_*` macro family do deserialization. There are two options how
those macro could be used:
* with 3 arguments `(type, src, dst)` those macro will return status
  of the operation. In this case `type` is a C language token that
  define type of loading structure, `src` is either a string in
  designated format or an XDR stream handler, `dst` is a pointer of
  `type*` where data should be loaded. With Clang first parameter is
  optional because could be derived from the pointer type of `dst`.

* the same macro with 2 arguments `(type, src)` will return a
  structure of designated type. Error handling in this case could be
  intercepted via logging callback described in [Error
  handling](#error_handling) section.

Example below demonstrates the concept:
```c
#include <metaresc.h>

TYPEDEF_STRUCT (mr_div_t,
                int quot,
                int rem,
                );

int main ()
{
  mr_div_t div3, div2 = MR_LOAD_CINIT (mr_div_t, "{1, 2}");
  mr_status_t status = MR_LOAD_CINIT (mr_div_t, "{3, 4}", &div3);
  MR_PRINT ("div2 = ", (mr_div_t, &div2, JSON),
	    "status = ", (mr_status_t, &status),
	    "div3 = ", (mr_div_t, &div3, JSON)
	    );
  return (EXIT_SUCCESS);
}
```
The output of this program is:
```c
div2 = {
  "quot" : 1,
  "rem" : 2
}
status = MR_SUCCESS
div3 = {
  "quot" : 3,
  "rem" : 4
}
```

For unification purposes all formats also support loading from
*resizable arrays*. Macroses
`MR_LOAD_*_RA (type, resizable_array, dst)` takes a pointer on a
`resizable_array_t` as a second arguments. Those macroses also could
be used with 2 arguments assuming the same semantics as primary versions
of load macroses.

Note that Metaresc serialization/deserialization supports cross
referencing of pointers on the same object. Object will be serialized
only once and other references on this object will be annotated
accordingly, so deserialization process will be able to restore
pointers correctly. This also impose a limitation on deserialization
with simplified semantics (2 arguments). It should not be used in case
if serialization tree may contain a pointer on a root object. A
simpliest example of this case is a linked list element referring to
its root element.

```c
#include <metaresc.h>

TYPEDEF_STRUCT (linked_list_t,
		(linked_list_t *, next),
		);

int main ()
{
  linked_list_t linked_list;
  linked_list.next = &linked_list;
  char * dump = MR_SAVE_CINIT (linked_list_t, &linked_list);
  if (dump)
    {
      linked_list_t ll3, ll2 = MR_LOAD_CINIT (linked_list_t, dump);
      mr_status_t status = MR_LOAD_CINIT (linked_list_t, dump, &ll3);
      bool ll2_self_referenced = &ll2 == ll2.next;
      bool ll3_self_referenced = &ll3 == ll3.next;
      MR_PRINT ("dump = ", dump, "status = ", (mr_status_t, &status),
		"ll2_self_referenced = ", ll2_self_referenced, "\n",
		"ll3_self_referenced = ", ll3_self_referenced, "\n"
		);
      MR_FREE (dump);
    }
  return (EXIT_SUCCESS);
}
```
The output is:
```c
dump = /* ref_idx = 0 */ {
  .next = /* ref = 0 */ NULL
}
status = MR_SUCCESS
ll2_self_referenced = false
ll3_self_referenced = true
```

### MR_PRINT helper macro
For logging and debugging purposes its useful to have a wrapper layer
that will incorporate standard routine for output of serialized objects:
serialization to string, `NULL` checking, output to file and memory
deallocation. Metaresc provides `MR_PRINT` and `MR_FPRINT` macroses
that hides all the boiler plate code and simulate Python-like
semantics. Those variadic macroses automatically detects type of
arguments and output them with proper formatting. 
Supported types are: strings, charecters, booleans,
integers, float, complex, pointers. Pointers on basic types also
output content of referenced memory. With Clang pointers on structures
and unions are also augmented with serialized content of referenced
memory (CINIT format). Arguments in parentheses are passed to
serialization engine. There should be 2 or 3 arguments in parentheses
to trigger serialization routine. 2 arguments case is `(type,
pointer)` that are passed to `MR_SAVE_CINIT` macro. 3 arguments case
is `(type, pointer, format)` that allows to serialize into any other
supported format.

`MR_PRINT` output to `stdout` and `MR_FPRINT` use the first argument as
file descriptor. Both macroses returns number of outputed bytes.

## Types declaration macro language

Metaresc provides 4 top level macro definitions.
* `TYPEDEF_STRUCT` - equivalent for `typedef struct type_t {} type_t;`
* `TYPEDEF_UNION` - equivalent for `typedef union type_t {} type_t;`
* `TYPEDEF_ENUM` - equivalent for `typedef enum type_t {} type_t;`
* `TYPEDEF_FUNC` - works for definition of function pointer types

### Structure type declaration
`TYPEDEF_STRUCT` must have at least one argument - struct type name.
All consequent arguments are delimited by comma and considered as a
structure fields declarations. Empty declarations are ignored. Fields
declarations could be presented in a multiple forms:
1. most of the basic types could be declared in a standard C notation
2. pointers, fields of a custom types, arrays, function pointers
should be declared as a structured set in parentheses
3. non-serializable fields, bitfields, anonymous unions looks like a
macro calls, i.e. a `keyword` followed by a set of arguments in parentheses.

#### Fields of a basic types
This is a simplified notation for basic types. It allows declaration
of a **single** field with a type name that consists of the following
keywords:
* `unsigned`
* `signed`
* `char`
* `short`
* `int`
* `long`
* `bool`
* `float`
* `double`
* `complex`
* `int8_t`
* `uint8_t`
* `int16_t`
* `uint16_t`
* `int32_t`
* `uint32_t`
* `int64_t`
* `uint64_t`
* `size_t`
* `ssize_t`
* `long_double_t`
* `string_t`
* `mr_ptr_t`
* `volatile`
* `const`

Here is a valid example:
```c
TYPEDEF_STRUCT (sample_t,
		int x,
		long int y,
		long long int z,
		volatile long long int v,
		);
```

#### Extended semantics for fields declaration
Extended semantics allows not only declaration itself, but also a
metadata definition. User could augment fields with text metadata and
arbitrary structured resources. User could access metadata and
resources at runtime via reflection API provided by Metaresc. Field
declaration is presented as positional set of parameters in
parentheses:

(type, name, _suffix_, _text\_metadata_, _{ pointer\_on\_resources\_array }_, _resource\_type_, _resources\_array\_size_)

Only the first two parameters are mandatory, the rest are optional.
1. **type** is a field type
2. **name** is a field name
3. **_suffix_** is used for declaration of arrays and function pointers
4. **_text\_metadata_** is a user defined string
5. **_pointer\_on\_resources\_array_** is a `void*` pointer that user can \
initialize with a pointer on array of structured resources
6. **_resource\_type_** is a string that defines type of resource pointer
7. **_resource\_array\_size_** is an integer value that denotes size of \
resource array

**_text\_metadata_** and **_resource_** information are available at
  run-time through reflection API.

Example below demonstrates extended semantics:
```c
TYPEDEF_STRUCT (sample_t,
		(int, field),
		(int *, pointer),
		(int, array, [2]),
		(int, function, (int)),
		(int, metadata, /* suffix */, "text metadata"),
		(int, void_resource, /* suffix */, /* text metadata */, { "string as a void pointer" }),
		(int, structured_resource, /* suffix */, /* text metadata */, { (sample_t[]){{0}} }, "sample_t"),
		(int, structured_resources_array, /* suffix */, /* text metadata */, { (sample_t[]){{0}, {1}} }, "sample_t", 2 * sizeof (sample_t)),
		);
```

#### Non-serializable fields
For the fields that should not be serialized use keyword `VOID` as a
prefix for declaration. Metaresc still detect type of those fields, but
skip them at the serialization/deserialization process.
Sample declaration as follows:

```c
TYPEDEF_STRUCT (non_serializable_t,
		VOID (int, field),
		VOID (int *, pointer),
		VOID (int, array, [2]),
		VOID (int, _array[2]),
		VOID (int, (*function), (int)),
		VOID (int, bitfield, :4),
		);
```

#### Field declaration
Type of the field may consist of multiple tokens and could include
keywords: `const`, `volatile`, `restrict`, `struct`, `union`,
`enum`. Name of the field must be strictly one token.

#### Pointer declaration
Metaresc is capable to resolve pointers on basic types and on custom
user's types. `char *` is treated as pointer on a NULL-terminated
string, but not a pointer on a single character.

Double pointers are not supported and user should use intermediate
wrapper structure to represent each level of indirect
access. I.e. `int ** double_pointer` should be declared as follows:

```c
TYPEDEF_STRUCT (int_ptr_t,
		(int *, ptr));

TYPEDEF_STRUCT (sample_t,
		(int_ptr_t *, double_pointer));
```

Default serialization of a pointer is a single instance of designated
type, but Metaresc also supports representation of pointers as arrays
of variable size. Size of the array in this case should provided as
another field of the same structure. User may specify name of
this field via structured resource of the pointer field. There are two
options how to do this.
1. User may specify name of the `size` field as a string and denote 
that type of the resource is a `"string"`. Sample declaration as 
follows:

```c
TYPEDEF_STRUCT (resizable_array_t,
		(sample_t *, array, /* suffix */, /* text metadata */, { "array_size" }, "string"),
		ssize_t array_size,
		);
```
Existence of the `size` field could be validated only at the run-time,
so this method makes a loosely-coupled definition.

2. Another way to specify `size` field is to provide `offset` of this
field as a structured resource. Sample declaration as follows:

```c
TYPEDEF_STRUCT (resizable_array_t,
		(sample_t *, array, /* suffix */, /* text metadata */, { .offset = offsetof (resizable_array_t, array_size) }, "offset"),
		ssize_t array_size,
		);
```
This method ensures that `size` field is presented in
designated structure, but does not verify that `pointer` and `size`
fields are within the same structure and `size` field has appropriate
type. This validation happens at run-time. `size` field might be of
any integer type including `bool` and `char`. It could also be an
`enum` or `bitfield` which are integer types by language design. `size`
field could also be a pointer on any type listed above except
`bitfield`. `bitfields` could be specified as `size` field only with
the first declaration method, because compiler can't calculate
`offsetof` for the `bitfields`.

Descriptors for pointer fields that are generated from DWARF debug
info have structured resource configured according to the first
method. I.e. `size` field configured as a string identifier and formed
from the name of the field with `_size` suffix.

#### Array declaration
Third argument `suffix` in the field's declaration denotes dimensions
of the array. Metaresc is capable to distinguish one- and two-
dimensional arrays. Higher orders of dimensions are treated as
two-dimensional arrays with aggregated lower dimension. User should
use intermediate wrapper types for propper serialization of 3+
dimensional arrays.

```c
TYPEDEF_STRUCT (array_1d_t,
		(int, array, [2]));
TYPEDEF_STRUCT (array_2d_t,
		(int, array, [2][2]));
TYPEDEF_STRUCT (array_3d_t,
		(array_1d_t, array, [2][2]));
TYPEDEF_STRUCT (_array_3d_t,
		(array_2d_t, array, [2]));
TYPEDEF_STRUCT (array_4d_t,
		(array_2d_t, array, [2][2]));
```

Zero-size arrays are also supported. Type descriptor will have all
meta information for those fields, but serialization will omit them as
empty fields. C standard allows empty-size declaration at the end of
the structure. In Metaresc those fields could be declared as
non-serializable.

```c
TYPEDEF_STRUCT (array_t,
		(int, zero_size_array, [0]),
		VOID (int, empty_size_array, []));
```

Base type of array's declaration might be:
* C basic type (character, string, boolean, integer, float, complex)
* any other type declared within Metaresc (struct, enum, union,
function, char array)
* pointer on types listed above

`meta` and `res` fields will be derived for serialization of
individual array's elements. This allows extended semantics for
serialization of unions and pointers:
* define discriminators for unions
* define overrides for union discriminators
* define size specification for pointers

#### Function pointer declaration
If `suffix` is an expression in parentheses, then this field is
treated as a function pointer declaration. I.e. declaration is
equivalent of `type (*name) suffix;` as a standard type
declaration. List of function arguments is processed and Metaresc type
descriptor contains this list in a structured way. This information
could be retrieved at run-time through reflection API. User must not
use arguments names in this declaration. Variadic functions should be
declared as non-serializable fields. Metaresc serialize function
pointers as function names retrieved via `dladdr ()`. You need to
compile with `-rdynamic` flag to enable resolution of pointers into
function names at run time. If function name is not available then the
pointer is serialized as hex value.

```c
TYPEDEF_STRUCT (functions_t,
		(int, my_fork, ()),
		(int, my_fork_implicit_void, (void)),
		(int, my_vprintf, (const char * restrict /* format */, va_list /* ap */)),
		VOID (int, (*my_printf), (const char * restrict /* format */, ...)),
		);
```

#### Bitfields declaration
For bitfields use the keyword `BITFIELD` as a prefix for declaration.

BITFIELD (type, name, _suffix_, _text\_metadata_, _{ pointer\_on\_resources\_array }_, _resource\_type_, _resources\_array\_size_)

**type** must be one of integer types including `bool`. `enums` are
  also represented as integer types by language design.

Sample declaration as follows:

```c
TYPEDEF_STRUCT (bitfields_t,
		BITFIELD (int, size, : sizeof (int) * __CHAR_BIT__ - 1),
		BITFIELD (bool, used, : 1));
```

#### Union declaration
Union field declaration works as declaration of any other type. The
tricky part is how to differentiate which branch of the union to
serialize at run-time. **_text\_metadata_** of the union field works
for identification of discriminator. Discriminator is a field in
serialization hierarchy that in run-time identifies active branch of
the union. The reason why this field might not be in parent structure
is because for structures like linked lists or trees it make sense to
have this discriminator only at the top level, but not in individual
nodes. If **_text\_metadata_** is not specified union is serialized as
a first branch in union declaration. The same default serialization
works in case if discriminor was not found at run-time. Identification
of the active branch is based on the type of the discriminator field:

* Integer types and booleans are interpreted as an index of active
  branch. 0-indexing is used.

* Dynamic (`char *`) and static (`char[]`) strings are interpreted as
  a name of the active branch.

* `enums` resolves to a name of active branch based on
  **_text\_metadata_** of `enum` value.

* Bitfields resolves based on their base type.

* Structures and unions resolves as a first field recursively down to
  basic type field listed above.

* Array resolves as a first element, even though array might be of a
  zero size.

* Pointers resolves based on non-NULL pointer value according rules
  listed above.

Example below demonstrates the concept:

```c
TYPEDEF_UNION (union_t,
	       VOID (intptr_t, non_serializable),
	       (intptr_t, int_value),
	       (double, dbl_value),
	       (char *, str_value),
	       (void *, ptr_value),
	       );

TYPEDEF_ENUM (union_discriminator_t,
	      (UD_INT, , "int_value"),
	      (UD_DBL, , "dbl_value"),
	      (UD_STR, , "str_value"),
	      (UD_PTR, , "ptr_value"),
	      );

TYPEDEF_STRUCT (tree_node_t,
		(union_t, value, , "discriminator"),
		(struct tree_node_t *, left),
		(struct tree_node_t *, right),
		);

TYPEDEF_STRUCT (tree_t,
		(union_discriminator_t, discriminator),
		(tree_node_t *, root),
		);
```

For declaration of anonymous union use keywords:

* `ANON_UNION (name, attributes)`
* `END_ANON_UNION (text_metadata, { pointer_on_resources_array }, resource_type, resource_array_size)`

All arguments for them are optional. Even though union is anonymous it
still require some **name** for serialization into self-descriptive
formats like XML or JSON. If name is not specified Metaresc will
auto-generate the name for it. Compiler specific `__attributes__ (())`
modifiers could be passed over **attributes** argument. Previous
example with anonymous union will look as follows:

```c
TYPEDEF_STRUCT (tree_node_t,
		ANON_UNION (value, __attribute__ ((packed))),
		(intptr_t, int_value),
		(double, dbl_value),
		(char *, str_value),
		(void *, ptr_value),
		END_ANON_UNION ("discriminator"),
		(struct tree_node_t *, left),
		(struct tree_node_t *, right),
		);
```

Metaresc also allows to make discriminator's resolution overrides for
individual declarations of union fields. For above example you may
want to have alternative type in which values discriminated with
`UD_STR` must not be serialized. This could be achieved in the
following way:

```c
TYPEDEF_STRUCT (single_linked_list_node_t,
		(union_t, value, , "discriminator",
		{ (mr_ud_override_t[]){ { UD_STR, "non_serializable"} } }, "mr_ud_override_t"),
		(struct tree_node_t *, next),
		);
```
Union field declaration should be augmented with an array of type
`mr_ud_override_t`. This type represents a tuple of integer value and
a string. First is a value of discriminator that require override and
second is name of the branch in union. In case of multiple overrides
you should also provide a size of this array. Example below
demonstrates a recommended approach:

```c
#define OVERRIDES				\
  (mr_ud_override_t[]){				\
    { UD_STR, "non_serializable"},		\
    { UD_PTR, "non_serializable"},		\
  }

TYPEDEF_STRUCT (double_linked_list_node_t,
		(union_t, value, , "discriminator",
		{ OVERRIDES }, "mr_ud_override_t", sizeof (OVERRIDES)),
		(struct tree_node_t *, prev),
		(struct tree_node_t *, next),
		);
```

Those overrides affects discriminators of integer types, booleans and
enums. Overrides also could be specified for pointers on unions and
arrays of unions. Unfortunatelly you can't declare overrides for
pointers on dynamic arrays, because both declarations uses structured
resources of the field.

Descriptors for union fields that are generated from DWARF debug
info have **text\_metadata** set to the name of the field with
`_discriminator` suffix. Names for anonymous unions are auto-generated
based on DWARF internal indexing schema.

#### Text metadata and resource information
Text metadata is a user defined string that could be retrieved at
run-time through reflection API. This property is also used for union
fields discrimination (a way to identify at run-time which union branch
to use).

Resource information (_text\_metadata_, _{
pointer\_on\_resources\_array }_, _resource\_type_,
_resources\_array\_size_) is also available at run-time through
reflection API and is used for dynamic arrays size specification and
union discriminator overrides.

#### NULL-terminated strings vs character arrays
There are multiple notions for `char *` fields.
* The most common case is a NULL-terminated string.
* In some cases this pointer is considered as a pointer on a single
character.
* It is also possible that pointer should be considered as a pointer
on an array or characters of a certain length.

By default `char *` is classified by Metaresc as a NULL-terminated
string. For declaration of a pointer on a single character or an array
the user should use the keyword `POINTER`.

POINTER (type, name, _text\_metadata_, _{ pointer\_on\_resources\_array }_, _resource\_type_, _resources\_array\_size_)

It is similar to standard declaration, but doesn't have **_suffix_**
argument. Pointer on an array could be declared in the same fashion
as for any other pointer. Here is a sample declaration of substring
type.

```c
TYPEDEF_STRUCT (substr_t,
		POINTER (char, str, , { .offset = offsetof (substr_t, length) }, "offset"),
		VOID (size_t, length));
```

The same problem is applicable for the declaration of characters
array. In most cases this declaration implies limited length
NULL-terminated static string, but in some cases user might want to
serialize this field as an array of characters. Standard declaration
of the array will be considered as a second case. For limited length
NULL-terminated static strings user should use either keyword
CHAR_ARRAY or declare custom type. Example as follows:

```c
typedef char static_string_t[sizeof ("Metaresc")];

TYPEDEF_STRUCT (char_array_t,
		(char, array, [sizeof ("Metaresc")], "array of characters"),
		CHAR_ARRAY (char, inline_static_string, [sizeof ("Metaresc")], "inline static NULL-terminated string"),
		(static_string_t, static_string, , "static NULL-terminated string"));
```

Serialization of this structure into C-init format produces the
following output.

```c
{
  .array = {
    'M',
    'e',
    't',
    'a',
    'r',
    'e',
    's',
    'c',
    '\000'
  },
  .inline_static_string = "Metaresc",
  .static_string = "Metaresc"
}
```

### Union type declaration
Union declaration is matching structure type declaration on 100%. Use
macro `TYPEDEF_UNION` for union type declaration. Everything that
works for structures works for unions, including transparent unions.

### Enumeration type declaration
Macro `TYPEDEF_ENUM` works for enumeration types declaration. First
argument of the macro is a type name and the rest are enumeration
values. For enumeration values you can use either simplified semantics
or extended variant.

(name, _value\_assignment_, _text\_metadata_,
{ _pointer\_on\_resources\_array_ }, _resource\_type_,
_resource\_array\_size_)

All arguments except the first one are optional. Example below
demonstrates the use of the macro:

```c
TYPEDEF_ENUM (color_t, ATTRIBUTES (__attribute__ ((packed))),
              RED,           /* auto-enumerated and becomes 0 */
              (ORANGE),      /* auto-enumerated and becomes 1 */

              (YELLOW, = 2), /* explicitly set to 2 */
              (GREEN,  = 3,  "set to 3 - this is a textual meta info"),
              (BLUE, ,  "auto-enumerated", { "a void pointer for arbitrary resource" }),

              (PURPLE,
	       /* value argument may be empty */,
	       "becomes 5",
	       { "next argument is a type of this poiner" },
	       "string"
	       ),

              (PINK,
	       /* auto-enumerated */,
	       /* no meta         */,
	       { (color_t[]){ PINK } },
	       "color_t" /* type itself might be used for initialization of resource */
	       ),

              (BROWN,
	       /* auto-enumerated */,
	       /* no meta         */,
	       { (color_t[]){ RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE, PINK, BROWN } },
	       "color_t",
	       8 * sizeof (color_t) /* size of resource array */
	       )           /* trailing comma is optional */
              );
```

### Function pointer type declaration
For declaration of function types use macro `TYPEDEF_FUNC`. Here is an
example of the usage:

```c
TYPEDEF_FUNC (int, compar_fn_t, (const void * /* x */, const void * /* y */));
```

Declaration of function type in C language optionally accepts names of
the argument. Unfortunatelly with Metaresc types only must be
used. Types of return value and all arguments are stored in type
descriptor and are available for introspection at run time.

### Metadata and resources definition for types
All type declaration macroses accepts keyword `ATTRIBUTES` as an
argument at any position. This keyword must be followed with a list of
parameters in parentheses.

```c
ATTRIBUTES (attributes, text_metadata, { pointer_on_resources_array }, resource_type, resource_array_size)
```

`attributes` is a token that will be used at type declaration. As an
example this might be `__attribute__ ((packed, aligned (64)))`. The
rest is a metadata that is stored in type desriptor and is accessible
for introspection at run time. The structure of this metadata is
identical to metadata of struct/union fields.

All arguments for `ATTRIBUTES` are positional and optional.

## Extra features

Introspection of data structures allows to reconstruct a graph of
individual objects and references between them. Graph
representation enables a set of extra features, such as: 
- [Deep copy](#deep-copy)
- [Recursive memory deallocation](#recursive-memory-deallocation)
- [Objects hashing](#objects-hashing)
- [Comparation of structures](#comparation-of-structures)

### Deep copy
`MR_COPY_RECURSIVELY` is similar to `MR_LOAD_*` macroses. It may take
2 or 3 arguemnts and semantics of those arguments is the same as for
`MR_LOAD_*` macroses. The source (`src`) in this case is a pointer on
original structure. All non-void pointers and strings will be
allocated with `MR_CALLOC` and copied from the original source. Cross
references will be restored identically to origin.

Example below demostrates a deep copy of a binary tree.

```c
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
  
  tree_node_t copy3, copy2 = MR_COPY_RECURSIVELY (tree_node_t, &root);
  mr_status_t status = MR_COPY_RECURSIVELY (tree_node_t, &root, &copy3);
  MR_PRINT ("copy2 = ", (tree_node_t, &copy2),
  	    "status = ", (mr_status_t, &status),
	    "copy3 = ", (tree_node_t, &copy3));
  MR_FREE_RECURSIVELY (tree_node_t, &copy2);
  MR_FREE_RECURSIVELY (tree_node_t, &copy3);
  return (EXIT_SUCCESS);
}
```

### Recursive memory deallocation
Macro `MR_FREE_RECURSIVELY` requires two arguments `(type,
pointer)`. Underlying function call traverse through the graph and
frees all non-NULL pointers and strings. Deallocation of the top level
object is a duty of a calling function. Example above demostrates the
usage of this macro. As a result of execution of this code all memory
blocks allocated by `MR_COPY_RECURSIVELY` are freed by
`MR_FREE_RECURSIVELY`. Valgrind memory check as follows:

```console
==9190== Memcheck, a memory error detector
==9190== Copyright (C) 2002-2012, and GNU GPL'd, by Julian Seward et al.
==9190== Using Valgrind-3.8.1 and LibVEX; rerun with -h for copyright info
==9190== Command: ./examples/.libs/type_prefixes
==9190== 
copy2 = {
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
status = MR_SUCCESS
copy3 = {
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
==9190== 
==9190== HEAP SUMMARY:
==9190==     in use at exit: 0 bytes in 0 blocks
==9190==   total heap usage: 167 allocs, 167 frees, 67,699 bytes allocated
==9190== 
==9190== All heap blocks were freed -- no leaks are possible
==9190== 
==9190== For counts of detected and suppressed errors, rerun with: -v
==9190== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 2 from 2)
```

### Objects hashing
Macro `MR_HASH_STRUCT` takes two arguments `(type, pointer)` and
returns unsigned integer value of type
`mr_hash_value_t`. Non-serializable fields (declared as `VOID ()` or
of types not known to Metadata) are not used for hashing. Typed
pointers and strings are hashed based on content value, but not a
pointer value. Copies derived by `MR_COPY_RECURSIVELY` have identical
hash values as an original object.

### Comparation of structures
`MR_CMP_STRUCTS` is a macro that has 3 arguments: `(type, pointer1,
pointer2)`. It takes two pointers of the same type and returns
integrer value that represents result of structures
comparation. Non-serializable fields are not used for comparation.

### Structure of serialization graph
`MR_SAVE` mr_ptrs_dfs

### Access and structure of type descriptor
`mr_get_td_by_name`

### Access and structure of field descriptor
`mr_get_fd_by_name`

### Enumerations metadata
`mr_get_enum_by_value` `mr_get_enum_by_name`

## Internals

### Error handling

### Memory allocation

### How to make certain field in type non-serializable
`mr_type_void_fields`

### Formatted output to a resizable array
`mr_ra_printf`

### Indexing framework
