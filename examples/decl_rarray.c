#include <stdlib.h>

#include "decl_rarray.h"

static organization_t firm = {
  .name = "Gen-sys",
  .size = 3 * sizeof (employee_t),
  .employees = (employee_t[]){
    { "Will", "Rodman", 200000 },
    { "Static", "Jacobs", 250000 },
    { "Robert", "Franklin", 170000 },
  },
};

int main ()
{
  MR_PRINT ((organization_t, &firm, JSON));
  return (EXIT_SUCCESS);
}
