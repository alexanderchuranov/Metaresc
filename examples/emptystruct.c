/* -*- C -*- */
/* This file is part of ResLib project */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <emptystruct.h>

int
main (int argc, char * argv[])
{
  empty_t empty;
  
  char * str = RL_SAVE_CINIT (empty_t, &empty);
  memset (&empty, 0, sizeof (empty));
  RL_LOAD_CINIT (empty_t, str, &empty);
  if (str)
    RL_FREE (str);
  
  return (EXIT_SUCCESS);
}
