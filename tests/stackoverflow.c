/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

#include <stdio.h>
#include <stdlib.h>

//#define RL_CHECK_TYPES(RL_TYPE_NAME, S_PTR...)
#include <stackoverflow.h>
#include <stackoverflow.h>

static large_struct_t large_struct = {
  .array = { [0 ... ARRAY_SIZE - 1] = 0, },
};

int
main (void)
{
  rl_rarray_t serialized = RL_SAVE_XDR_RA (large_struct_t, &large_struct);

  printf ("sizeof (large_struct_t) = %zd serialized = %d\n", sizeof (large_struct_t), serialized.size);
#if 1
  large_struct = RL_LOAD_XDR_RA (large_struct_t, &serialized);
#endif
  RL_LOAD_XDR_RA (large_struct_t, &serialized, &large_struct);
  return (EXIT_SUCCESS);
}
