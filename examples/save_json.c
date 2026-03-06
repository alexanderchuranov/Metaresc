
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
  char * mr_conf_serialized = MR_SAVE_JSON (mr_conf_t, &mr_conf);
  MR_PRINT ("#1 Serialized ", strlen (mr_conf_serialized), "\n");

  mr_conf_t mr_conf_loaded = {};
  mr_status_t status = MR_LOAD_JSON (mr_conf_t, mr_conf_serialized, &mr_conf_loaded);
  MR_PRINT ("Load status ", (mr_status_t, &status));

  //mr_conf = mr_conf_loaded;
  mr_ptrdes_t * mr_conf_serialized_ = MR_SAVE (mr_ic_t, &mr_conf_loaded.type_by_name);
  MR_PRINT ("Serialized ", mr_conf_serialized_->next, "\n");

  mr_conf = mr_conf_saved;
  //MR_FREE_RECURSIVELY (mr_conf_t, &mr_conf_loaded);
  MR_PRINT ("Free done\n");

  MR_FREE (mr_conf_serialized_);
  MR_FREE (mr_conf_serialized);
  return (EXIT_SUCCESS);

  employee_t employee = { "John", "Doe", 123456 };
  MR_PRINT ((employee_t, &employee, JSON));
  return (EXIT_SUCCESS);
}
