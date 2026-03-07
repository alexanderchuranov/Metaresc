#include <assert.h>
#include <metaresc.h>
#include <check.h>

#include <regression.h>

START_TEST (json_cross_ref) {
  char * mr_conf_json = MR_SAVE_JSON (mr_conf_t, &mr_conf);
  ck_assert_msg (mr_conf_json, "Memory allocation failed");

  mr_ra_ptrdes_t ptrs_json = {};
  mr_add_ptr_to_list (&ptrs_json);
  ck_assert_msg (ptrs_json.ra, "Memory allocation failed");
  mr_json_load (mr_conf_json, &ptrs_json);
  mr_idx_t ptrs_json_count = ptrs_json.size / sizeof (ptrs_json.ra[0]);

  mr_ptrdes_t * ptrs_orig = MR_SAVE (mr_conf_t, &mr_conf);
  ck_assert_msg (ptrs_orig, "Serialization to graph failed");
  mr_ptrdes_t * map[ptrs_orig->next] = {};
  mr_idx_t i, j;
  bool mismatched_nodes = false;

  for (i = 1; i < ptrs_orig->next; ++i)
    ptrs_orig[i].idx = 0;
  mr_remove_empty_nodes (ptrs_orig);
  for (i = 1; i < ptrs_orig->next; ++i)
    map[ptrs_orig[i].idx] = &ptrs_orig[i];

  ck_assert_msg (ptrs_json_count <= ptrs_orig->next, "Number of nodes of deserialized JSON is more then in the original structure");
  
  for (i = 1; i < ptrs_json_count; ++i)
    if (ptrs_json.ra[i].fdp)
      {
	ck_assert_msg (map[i], "Deserialized JSON has a node that is not presented in original structure");
	ck_assert_msg (map[i]->fdp, "Unnamed node in original structure has a name in deserialized JSON");

	if (strcmp (ptrs_json.ra[i].fdp->name.str, map[i]->fdp->name.str) != 0)
	  {
	    fprintf (stderr, "[%d] original struct field name '%s' mismatched JSON deserialized '%s'\n",
		     i,
		     map[i]->fdp ? map[i]->fdp->name.str : "unnamed",
		     ptrs_json.ra[i].fdp ? ptrs_json.ra[i].fdp->name.str : "unnamed");

	    fprintf (stderr, "Path in the original struct: ");
	    for (j = map[i]->parent; j != MR_NULL_IDX; j = ptrs_orig[j].parent)
	      fprintf (stderr, "(%s : %s) ",
		       ptrs_orig[j].fdp ? ptrs_orig[j].fdp->stype.type : "untyped",
		       ptrs_orig[j].fdp ? ptrs_orig[j].fdp->name.str : "unnamed");
	    fprintf (stderr, "\n");

	    fprintf (stderr, "Path in deserialized JSON: ");
	    for (j = ptrs_json.ra[i].parent; j != MR_NULL_IDX; j = ptrs_json.ra[j].parent)
	      fprintf (stderr, "(%s : %s) ",
		       ptrs_json.ra[j].fdp ? ptrs_json.ra[j].fdp->stype.type : "untyped",
		       ptrs_json.ra[j].fdp ? ptrs_json.ra[j].fdp->name.str : "unnamed");
	    fprintf (stderr, "\n");
	    mismatched_nodes = true;
	  }
      }

  mr_free_load_values (&ptrs_json);
  MR_FREE (mr_conf_json);

  ck_assert_msg (!mismatched_nodes, "Deserialized JSON has mismatched nodes with original structure");
} END_TEST

MAIN_TEST_SUITE ((json_cross_ref, "validate that JSON deserialization node indexing matches original node indexing"));
