
#include <stdio.h>
#include <stdlib.h>

#include "marshalling_arrays.h"

int main ()
{
  array_t a = { .a = { 11, 17, 23 } };
  array2d_t a2d = { { {1, 2}, {3, 4} } };
  MR_PRINT ((array_t, &a, XML), (array2d_t, &a2d, XML));
  return (EXIT_SUCCESS);
}
