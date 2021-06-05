#include <stdio.h>
#include <stdlib.h>

#include "marshalling_pointers_to_functions.h"

int main ()
{
  with_functions_t with_functions = {
    .value = 7,
    .func = puts
  };
  MR_PRINT ((with_functions_t, &with_functions, XML));
  return (EXIT_SUCCESS);
}
