
#include <stdio.h>
#include <stdlib.h>

#include <reslib.h>

int
main (int argc, char * argv[])
{
  int x = 5;
  char * xml = MR_SAVE_XML (int, &x);

  if (NULL == xml)
    printf ("Serialization failed\n");
  else
    {
      printf ("%s", xml);
      MR_FREE (xml);
    }
  
  return (EXIT_SUCCESS);
}
