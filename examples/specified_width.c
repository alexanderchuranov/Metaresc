
#include <stdio.h>
#include <stdlib.h>

#include "specified_width.h"

int main()
{
  specified_width_t b;
  memset (&b, 0, sizeof (b));
  char * str = RL_SAVE_CINIT (rl_conf_t, &rl_conf);
  if (str)
    {
      puts (str);
      RL_FREE (str);
    }
  
  return (EXIT_SUCCESS);
}
