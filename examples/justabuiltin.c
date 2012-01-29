
#include <stdio.h>
#include <stdlib.h>

#include <reslib.h>

int
main (int argc, char * argv[])
{
  int x = 5;
  char * xml = RL_SAVE_XML (int, &x);

  if (NULL == xml)
    printf ("Serialization failed\n");
  else
    {
      printf ("%s", xml);
      RL_FREE (xml);
    }
  
  return (EXIT_SUCCESS);
}
