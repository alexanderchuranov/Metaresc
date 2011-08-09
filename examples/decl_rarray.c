
#include <assert.h>
#include <stdlib.h>
#include "decl_rarray.h"

int main()
{
  organization_t firm;
  
  memset(&firm, 0, sizeof(firm));
  
  firm.name = "Gen-sys";
  firm.employees.alloc_size = sizeof(employee_t) * 3;
  firm.employees.data = RL_MALLOC(firm.employees.alloc_size);
  firm.employees.size = 0;
  
  employee_t* e = firm.employees.data;
  
  e[0].firstname = "Will";
  e[0].lastname  = "Rodman";
  e[0].salary    = 200000;
  
  e[1].firstname = "Steven";
  e[1].lastname  = "Jacobs";
  e[1].salary    = 630000;
  
  e[2].firstname = "Robert";
  e[2].lastname  = "Franklin";
  e[2].salary    = 170000;
  
  firm.employees.size = 3 * sizeof(employee_t);
  
  char* xml = RL_SAVE_XML(organization_t, &firm);
  
  if (xml)
  {
    puts(xml);
    return (EXIT_SUCCESS);
  }
  else {
    fprintf(stderr, "Serialization failure.");
    return (EXIT_FAILURE);
  }
}
