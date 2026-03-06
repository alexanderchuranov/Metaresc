
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

  mr_ra_ptrdes_t _ptrs_ = {};
  mr_add_ptr_to_list (&_ptrs_);
  if (_ptrs_.ra)
    {
      mr_json_load (mr_conf_serialized, &_ptrs_);
      MR_PRINT (_ptrs_.ra[21102], "\n");
      mr_free_load_values (&_ptrs_);
    }

  mr_conf_t mr_conf_loaded = {};
  mr_status_t status = MR_LOAD_JSON (mr_conf_t, mr_conf_serialized, &mr_conf_loaded);
  MR_PRINT ("Load status ", (mr_status_t, &status));

  //mr_conf = mr_conf_loaded;
  //mr_ptrdes_t * mr_conf_serialized_ = MR_SAVE (mr_ic_t, &mr_conf_loaded.type_by_name);
  //MR_PRINT ("Serialized ", mr_conf_serialized_->next, "\n");
  int i;
  mr_ic_hash_t * hash = &mr_conf_loaded.type_by_name.hash;
  for (i = 0; i < hash->count; ++i)
    if (hash->hash_table[i].ptr)
      MR_PRINT ("[", i, "] type ", ((mr_td_t*)hash->hash_table[i].ptr)->type.str, "\n");
  mr_conf = mr_conf_saved;
  //MR_FREE_RECURSIVELY (mr_conf_t, &mr_conf_loaded);
  MR_PRINT ("Free done\n");

  //MR_FREE (mr_conf_serialized_);
  MR_FREE (mr_conf_serialized);
  return (EXIT_SUCCESS);

  employee_t employee = { "John", "Doe", 123456 };
  MR_PRINT ((employee_t, &employee, JSON));
  return (EXIT_SUCCESS);
}
