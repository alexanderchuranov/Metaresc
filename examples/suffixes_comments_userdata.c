
#include <stdio.h>
#include <stdlib.h>

#include "suffixes_comments_userdata.h"

int main()
{
  char * str = RL_SAVE_CINIT (rl_conf_t, &rl_conf);
  if (str)
    {
      puts (str);
      RL_FREE (str);
    }
  
  return (EXIT_SUCCESS);
}
