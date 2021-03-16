
#include <stdio.h>
#include <stdlib.h>

#include <metaresc.h>

TYPEDEF_FUNC (ATTRIBUTES (__attribute__ ((aligned (sizeof (int64_t)))) , "function type sample"), int, func_t, (char *, int, int *, double))

int main ()
{
  MR_PRINT ("func_t = ", (mr_td_t, mr_get_td_by_name ("func_t")));
  return (EXIT_SUCCESS);
}
