/* -*- C -*- */
/* This file is part of Metaresc project */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <emptystruct.h>

int
main (int argc, char * argv[])
{
  empty_t empty;
  
  memset (&empty, 0, sizeof (empty));
  char * str = MR_SAVE_CINIT (empty_t, &empty);
  MR_LOAD_CINIT (empty_t, str, &empty);
  if (str)
    MR_FREE (str);
  
  return (EXIT_SUCCESS);
}
