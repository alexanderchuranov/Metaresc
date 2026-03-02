
#include <stdio.h>
#include <stdlib.h>

#include <metaresc.h>

TYPEDEF_STRUCT (employee_t,
		(char *, firstname),
		(char *, lastname),
		(int, salary),
		)

int
main (int argc, char * argv[])
{
  mr_conf_t mr_conf_saved = mr_conf;
  mr_conf_t * src = &mr_conf;
  mr_rarray_t mr_conf_serialized = MR_SAVE_JSON_RA (mr_conf_t, src);
  MR_PRINT ("#1 Serialized size ", mr_conf_serialized.mr_size, "\n");

  mr_conf_t mr_conf_loaded;
  mr_conf_t * dst = &mr_conf_loaded;
  memset (&mr_conf_loaded, 0, sizeof (mr_conf_loaded));
  mr_status_t status = MR_LOAD_JSON_RA (mr_conf_t, &mr_conf_serialized, dst);
  MR_PRINT ("Load status ", (mr_status_t, &status));

  mr_conf = mr_conf_loaded;
  mr_rarray_t mr_conf_serialized_ = MR_SAVE_JSON_RA (mr_conf_t, &mr_conf);
  MR_PRINT ("#2 Serialized size ", mr_conf_serialized_.mr_size, "\n");

  mr_conf = mr_conf_saved;
  MR_FREE_RECURSIVELY (mr_conf_t, &mr_conf_loaded);
  MR_PRINT ("Free done\n");

  MR_FREE (mr_conf_serialized_.data.ptr);
  MR_FREE (mr_conf_serialized.data.ptr);
  return (EXIT_SUCCESS);

  employee_t employee = { "John", "Doe", 123456 };
  MR_PRINT ((employee_t, &employee, JSON));
  return (EXIT_SUCCESS);
}
