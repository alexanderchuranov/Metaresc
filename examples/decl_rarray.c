
#include <assert.h>
#include <stdlib.h>

#include "decl_rarray.h"

static organization_t firm = {
  .name = "Gen-sys",
  .employees = {
    .size = 3 * sizeof (employee_t),
    .data = (employee_t[]){
      { "Will", "Rodman", 200000 },
      { "Static", "Jacobs", 250000 },
      { "Robert", "Franklin", 170000 },
    },
  },
};

int main ()
{
  char * xml = MR_SAVE_XML (organization_t, &firm);

  if (NULL == xml)
    printf ("Serialization failed\n");
  else
    {
      printf ("%s", xml);
      MR_FREE (xml);
    }

  return (EXIT_SUCCESS);
}
