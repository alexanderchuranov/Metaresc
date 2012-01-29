
#include <stdlib.h>

#include <reslib.h>

TYPEDEF_STRUCT (struct1_t,
                (char *, name),
                ANON_UNION (),
                  (int, x),
                  (float, y),
                END_ANON_UNION (),
                (char *, description)
                )

int main ()
{
  struct1_t s1;
  
  s1.y = .375;
  return (EXIT_SUCCESS);
}
