
#include <stdio.h>
#include "marshalling_unions.h"

int main()
{
  floating_t fl = { .f = 3.14 };
  
  char const* xml = RL_SAVE_XML( floating_t, &fl );
  puts(xml);
  RL_FREE( xml );
  
  return 0;
}
