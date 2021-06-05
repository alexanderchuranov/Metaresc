
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <builtins.h>

int
main (int argc, char * argv[])
{
  builtins_t b;
  memset (&b, -1, sizeof (b));
  MR_PRINT ((builtins_t, &b, XML));
  return (EXIT_SUCCESS);
}
