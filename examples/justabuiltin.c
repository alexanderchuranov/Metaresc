
#include <stdio.h>
#include <stdlib.h>

#include <reslib.h>

int
main (int argc, char * argv[])
{
  int x = 5;
  
  char * str = RL_SAVE_XML (int, &x);
  if (str)
    {
      puts (str);
      RL_FREE (str);
    }
  
  return (EXIT_SUCCESS);
}
