
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <metaresc.h>

TYPEDEF_ENUM (union_discriminator_t,
	      (UD_VOID_PTR, , "void_ptr"),
	      (UD_UC_ARRAY, , "uc_array"),
	      (UD_FLOAT_VAL, , "float_val")
	      )

TYPEDEF_UNION (union_t,
	       (void *, void_ptr),
               (unsigned char, uc_array, [sizeof (float)]),
               (float, float_val)
               )

TYPEDEF_STRUCT (discriminated_union_t,
		(union_t, union_val, , "union_discriminator"),
		(union_discriminator_t, union_discriminator)
		)

int main ()
{
  discriminated_union_t u = { { .float_val = M_PI }, UD_UC_ARRAY };
  char * xml = MR_SAVE_XML (discriminated_union_t, &u);

  if (NULL == xml)
    printf ("Serialization failed\n");
  else
    {
      printf ("%s", xml);
      MR_FREE (xml);
    }
  
  return (EXIT_SUCCESS);
}
