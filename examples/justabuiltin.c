
#include <stdio.h>
#include <stdlib.h>

#include <metaresc.h>

int
main (int argc, char * argv[])
{
  int x = 5;
  MR_PRINT (( , &x, XML));
  return (EXIT_SUCCESS);
}
