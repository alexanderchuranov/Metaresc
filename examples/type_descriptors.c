
#include <stdio.h>
#include <stdlib.h>
#include <metaresc.h>

TYPEDEF_STRUCT (employee_t,
		(char *, firstname),
		(char *, lastname),
		(int, salary),
		)

int main ()
{
  char type[] = "employee_t";
  mr_td_t const * td = mr_get_td_by_name (type);
  
  if (td)
    {
      printf ("information for '%s':\n", type);
      char const * type_class = NULL;
    
      switch (td->mr_type)
	{
	case MR_TYPE_STRUCT:
	  type_class = "a struct";
	  break;
	case MR_TYPE_ENUM:
	  type_class = "an enumeration";
	  break;
	case MR_TYPE_UNION:
	  type_class = "a union";
	  break;
	default:
	  type_class = "an unknown type";
	}
    
      printf ("\t%s is %s\n", type, type_class);
      printf ("\tit's declared name is '%s'\n", td->type);
      printf ("\tvariables of this type occupy %i bytes of memory\n", td->size);
    }
  else
    {
      printf ("error: can't obtain type information for type '%s'\n", type);
    }
  
  return (EXIT_SUCCESS);
}
