
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
  char * xml = MR_SAVE_XML (int_dbl_pointer_t, &idp);

  if (NULL == xml)
    printf ("Serialization failed\n");
  else
    {
      printf ("%s", xml);
      MR_FREE (xml);
    }
  
  return (EXIT_SUCCESS);
}
