
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <builtins.h>

int
main (int argc, char * argv[])
{
  builtins_t b;
  memset (&b, -1, sizeof (b));
  char * xml = MR_SAVE_XML (builtins_t, &b);

  if (NULL == xml)
    printf ("Serialization failed\n");
  else
    {
      printf ("%s", xml);
      MR_FREE (xml);
    }

  return (EXIT_SUCCESS);
}
