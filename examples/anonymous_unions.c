
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
  struct_t s;
  
  s.y = .375;
  return (EXIT_SUCCESS);
}
