
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
  mr_json_load (mr_conf_serialized, &_ptrs_);
  mr_ptrdes_t * ptrs = MR_SAVE (mr_conf_t, &mr_conf);
  mr_ptrdes_t * map[ptrs->next] = {};
  int i, j;

  for (i = 1; i < ptrs->next; ++i)
    ptrs[i].idx = 0;
  mr_remove_empty_nodes (ptrs);

  int max = 0;
  for (i = 1; i < ptrs->next; ++i)
    {
      map[ptrs[i].idx] = &ptrs[i];
      if (max < ptrs[i].idx)
	max = ptrs[i].idx;
    }
  MR_PRINT ("Load count ", _ptrs_.size / sizeof (_ptrs_.ra[0]), " save max idx ", max, "\n");
  for (i = 1; i < _ptrs_.size / sizeof (_ptrs_.ra[0]); ++i)
    if (_ptrs_.ra[i].fdp && map[i] && map[i]->fdp &&
	(strcmp (_ptrs_.ra[i].fdp->name.str, map[i]->fdp->name.str) != 0))
      {
	MR_PRINT ("[", i, "] ",
		  map[i]->fdp ? map[i]->fdp->name.str : "unnamed", " ",
		  _ptrs_.ra[i].fdp ? _ptrs_.ra[i].fdp->name.str : "unnamed", "\n");
	for (j = map[i]->parent; j != MR_NULL_IDX; j = ptrs[j].parent)
	  MR_PRINT ("(",
		    ptrs[j].fdp ? ptrs[j].fdp->stype.type : "untyped", " : ",
		    ptrs[j].fdp ? ptrs[j].fdp->name.str : "unnamed", ") ");
	MR_PRINT ("\n");
	for (j = _ptrs_.ra[i].parent; j != MR_NULL_IDX; j = _ptrs_.ra[j].parent)
	  MR_PRINT ("(",
		    _ptrs_.ra[j].fdp ? _ptrs_.ra[j].fdp->stype.type : "untyped", " : ",
		    _ptrs_.ra[j].fdp ? _ptrs_.ra[j].fdp->name.str : "unnamed", ") ");
	MR_PRINT ("\n");
      }
  return (EXIT_SUCCESS);

  mr_conf_t mr_conf_loaded = {};
  mr_status_t status = MR_LOAD_JSON (mr_conf_t, mr_conf_serialized, &mr_conf_loaded);
  MR_PRINT ("Load status ", (mr_status_t, &status));

  mr_conf = mr_conf_loaded;
  mr_ptrdes_t * mr_conf_serialized_ = MR_SAVE (mr_ic_t, &mr_conf_loaded.type_by_name);
  MR_PRINT ("Serialized ", mr_conf_serialized_->next, "\n");

  mr_conf = mr_conf_saved;
  MR_FREE_RECURSIVELY (mr_conf_t, &mr_conf_loaded);
  MR_PRINT ("Free done\n");

  MR_FREE (mr_conf_serialized_);
  MR_FREE (mr_conf_serialized);
  return (EXIT_SUCCESS);

  employee_t employee = { "John", "Doe", 123456 };
  MR_PRINT ((employee_t, &employee, JSON));
  return (EXIT_SUCCESS);
}
