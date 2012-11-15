
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
  return (EXIT_SUCCESS);
}
