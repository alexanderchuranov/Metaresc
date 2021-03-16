
#include <stdio.h>
#include <stdlib.h>

#include <metaresc.h>

TYPEDEF_STRUCT (with_f_pointers_t,
                (char *, pfunc, (int))
                )

int main ()
{
  MR_PRINT ("with_f_pointers_t = ", (mr_td_t, mr_get_td_by_name ("with_f_pointers_t")));
  return (EXIT_SUCCESS);
}
