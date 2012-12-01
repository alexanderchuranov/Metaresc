
#include <stdlib.h>

#include <metaresc.h>

TYPEDEF_STRUCT (struct_t,
                (char *, name),
                ANON_UNION (),
                  (int, x),
                  (float, y),
                END_ANON_UNION (),
                (char *, description)
                )

int main ()
{
  struct_t my_struct;
  my_struct.name = my_struct.description = "string";
  my_struct.y = 1;
  printf ("my_struct.x = 0x%08x my_struct.y = %f\n", my_struct.x, my_struct.y);
  return (EXIT_SUCCESS);
}
