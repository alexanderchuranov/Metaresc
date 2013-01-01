#include "type_prefixes.h"

int main ()
{
  char * str = MR_SAVE_CINIT (mr_conf_t, &mr_conf);
  if (str)
    {
      printf ("mr_conf = %s\n", str);
      MR_FREE (str);
    }
  return 0;
}
