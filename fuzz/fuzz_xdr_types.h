/* -*- C -*- */
/* This file is a part of Metaresc project */

/*
  Types that are loaded by the XDR fuzzer. They are shared by the harness
  (fuzz_xdr_load.c) and by the seed writer (fuzz_xdr_seed.c).

  The fixture covers the field kinds that the XDR loader decodes with a
  length or a count taken from the stream: strings, pointers with a count
  field, a character array and a fixed size array. The two arrays are the
  last fields of the structure, so an overflow of any of them runs out of
  the object and is reported by AddressSanitizer.
*/

#ifndef _FUZZ_XDR_TYPES_H_
#define _FUZZ_XDR_TYPES_H_

#include <metaresc.h>

#ifdef HAVE_RPC_TYPES_H

/* A typedef of a char array is MR_TYPE_CHAR_ARRAY, a plain char[] field is a generic array */
typedef char fuzz_str_t[8];

TYPEDEF_ENUM (fuzz_color_t, FUZZ_RED, FUZZ_GREEN, FUZZ_BLUE);

TYPEDEF_UNION (fuzz_union_t,
               (int32_t, i),
               (double, d),
               (char *, s),
               );

TYPEDEF_ENUM (fuzz_discriminator_t,
              (FUZZ_DISC_I, , "i"),
              (FUZZ_DISC_D, , "d"),
              (FUZZ_DISC_S, , "s"),
              );

TYPEDEF_STRUCT (fuzz_node_t,
                (char *, name),
                (fuzz_node_t *, next),
                );

TYPEDEF_STRUCT (fuzz_t,
                (fuzz_color_t, color),
                (fuzz_union_t, value),
                (fuzz_discriminator_t, value_discriminator),
                (char *, text),
                (fuzz_node_t *, nodes),
                (ssize_t, nodes_count),
                (int, bits, : 5),
                (bool, flag, : 1),
                (double, matrix, [2][2]),
                (fuzz_str_t, str),
                (int32_t, arr, [4]),
                );

#endif /* HAVE_RPC_TYPES_H */

#endif /* _FUZZ_XDR_TYPES_H_ */
