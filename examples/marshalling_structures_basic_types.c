#include <stdlib.h>

#include "marshalling_structures_basic_types.h"

int main ()
{
  basics_t b = {
    .c = 'Z',
    .uli = 123,
    .fl = {
      .f = 0.3f,
      .ld = 3.14,
      .dc = 7.42
    }
  };

  MR_PRINT ((basics_t, &b, XML));
  return (EXIT_SUCCESS);
}
