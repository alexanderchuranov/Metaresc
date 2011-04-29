
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <builtins_auto.h>

int
main (int argc, char * argv[])
{
  builtins_auto_t b;
  memset (&b, -1, sizeof (b));
  char * str = RL_SAVE_XML (builtins_auto_t, &b);
  if (str)
    {
      puts (str);
      RL_FREE (str);
    }
  
  return (EXIT_SUCCESS);
}
