
#include <stdio.h>
#include <stdlib.h>
#include <reslib.h>

TYPEDEF_STRUCT (employee_t,
		(char *, firstname),
		(char *, lastname),
		(int, salary),
		)

int main()
{
  char const name[] = "employee_t";
  rl_td_t const* td = rl_get_td_by_name(name);
  
  if (td)
  {
    printf("information for '%s':\n", name);
    char const* type = 0;
    
    switch (td->rl_type)
    {
      case RL_TYPE_STRUCT: type = "a struct"; break;
      case RL_TYPE_ENUM:   type = "an enumeration"; break;
      case RL_TYPE_UNION:  type = "a union"; break;
      default:
        type = "an unknown type";
    }
    
    printf("\t%s is %s\n", name, type);
    printf("\tit's declared name is '%s'\n", td->type);
    printf("\tvariables of this type occupy %i bytes of memory\n", td->size);
  }
  else {
    printf("error: can't obtain type information for '%s'\n", name);
  }
  
  return EXIT_SUCCESS;
}
