
#include <stdio.h>
#include <stdlib.h>

#include <function_type.h>

char * _strdup_ (char * str) { return (strdup (str)); }

int main ()
{
  string_porcessor_t x = _strdup_;
  printf ("x = %p\n", x);
  return (EXIT_SUCCESS);
}
