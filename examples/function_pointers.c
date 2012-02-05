
#include <stdio.h>
#include <stdlib.h>

#include <metaresc.h>

TYPEDEF_STRUCT (with_f_pointers_t,
                (char *, pfunc, (int))
                )

char * even_odd (int x)
{
  return (x & 1 ? "odd" : "even");
}

int main ()
{
  with_f_pointers_t s = { .pfunc = even_odd };
  int numbers[] = { 1, 4, 7 };
  int i;
  
  for (i = 0; i < sizeof (numbers) / sizeof (numbers[0]); ++i)
    printf("%i is %s\n", numbers[i], s.pfunc (numbers[i]));
  
  return (EXIT_SUCCESS);
}
