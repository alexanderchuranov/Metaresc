
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <metaresc.h>

TYPEDEF_STRUCT (int_dbl_pointer_t,
		(struct int_pointer_t *, ip_ptr),
		(int *, ptr)
		)

TYPEDEF_STRUCT (int_pointer_t,
		(int *, ptr)
		)

int main ()
{
  int x = 1234;
  int y = 4321;
  int_pointer_t x_ptr = { &x };
  int_dbl_pointer_t idp = { &x_ptr, &y };
  MR_PRINT ((int_dbl_pointer_t, &idp, XML));
  return (EXIT_SUCCESS);
}
