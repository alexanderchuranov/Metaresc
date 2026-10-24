/* -*- C -*- */
/* This file is a part of Metaresc project */

/*
  Writes seed inputs for fuzz_xdr_load into a directory:
    seed_populated.xdr - an object with every kind of field set
    seed_zeroed.xdr    - an all zero object
  Build and run as described in README.md. The corpus has to be written
  again after the types in fuzz_xdr_types.h change.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <metaresc.h>
#include <fuzz_xdr_types.h>

#ifdef HAVE_RPC_TYPES_H

static int
write_seed (const char * dir, const char * name, fuzz_t * object)
{
  mr_rarray_t ra = MR_SAVE_XDR_RA (fuzz_t, object);
  char path[1024];
  snprintf (path, sizeof (path), "%s/%s", dir, name);

  FILE * fp = fopen (path, "wb");
  if ((NULL == ra.data.ptr) || (NULL == fp) || (fwrite (ra.data.ptr, 1, ra.MR_SIZE, fp) != ra.MR_SIZE))
    {
      fprintf (stderr, "failed to write %s\n", path);
      return (1);
    }
  fclose (fp);
  printf ("%s: %u bytes\n", path, (unsigned)ra.MR_SIZE);
  MR_FREE (ra.data.ptr);
  return (0);
}

int
main (int argc, char * argv[])
{
  if (argc != 2)
    {
      fprintf (stderr, "usage: %s output-directory\n", argv[0]);
      return (EXIT_FAILURE);
    }

  fuzz_node_t nodes[2] = { { .name = "first", .next = &nodes[1], }, { .name = "second", .next = NULL, }, };

  fuzz_t populated;
  memset (&populated, 0, sizeof (populated));
  populated.color = FUZZ_BLUE;
  populated.value.s = "union string";
  populated.value_discriminator = FUZZ_DISC_S;
  populated.text = "text";
  populated.nodes = nodes;
  populated.nodes_count = 2;
  populated.bits = 7;
  populated.flag = true;
  populated.matrix[0][0] = 1.5;
  populated.matrix[1][1] = -2.5;
  strcpy (populated.str, "abc");
  populated.arr[0] = 1;
  populated.arr[3] = 4;

  fuzz_t zeroed;
  memset (&zeroed, 0, sizeof (zeroed));

  return (write_seed (argv[1], "seed_populated.xdr", &populated) | write_seed (argv[1], "seed_zeroed.xdr", &zeroed));
}

#else /* HAVE_RPC_TYPES_H */

int
main (int argc, char * argv[])
{
  fprintf (stderr, "XDR is not available\n");
  return (EXIT_FAILURE);
}

#endif /* HAVE_RPC_TYPES_H */
