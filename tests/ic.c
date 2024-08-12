#include <check.h>
#include <metaresc.h>
#include <regression.h>

#include <mr_ic.h>

#define MR_IC_INIT(TYPE) [MR_IC_##TYPE] = #TYPE,
static char * mr_ic_types[] = { MR_FOREACH (MR_IC_INIT, UNSORTED_ARRAY, SORTED_ARRAY, STATIC_ARRAY, HASH, RBTREE, AVLTREE) };

static mr_ptr_t mr_ic_rarray[] = {
  { .uintptr = 1, },
  { .uintptr = 2, },
  { .uintptr = 3, },
};

static mr_hash_value_t
uintptr_t_get_hash (mr_ptr_t node, const void * context)
{
  return (node.uintptr);
}

static int
uintptr_t_cmp (mr_ptr_t x, mr_ptr_t y, const void * context)
{
  return (x.uintptr > y.uintptr) - (x.uintptr < y.uintptr);
}

static mr_status_t
uintptr_t_visitor (mr_ptr_t node, void * context)
{
  uint32_t * mask = context;
  uintptr_t value = MR_MIN (node.uintptr, sizeof (*mask) * __CHAR_BIT__ - 1);
  *mask |= 1 << value;
  return (MR_SUCCESS);
}

static void
ic_types_foreach (int (*callback) (mr_ic_t * ic, char * mr_ic_type))
{
  int ic_type;
  for (ic_type = 0; ic_type < sizeof (mr_ic_types) / sizeof (mr_ic_types[0]); ++ic_type)
    if (mr_ic_types[ic_type] != NULL)
      {
	mr_ic_t ic;
	ck_assert_msg (MR_SUCCESS == mr_ic_new (&ic, uintptr_t_get_hash, uintptr_t_cmp, "uintptr", ic_type, NULL),
		       "mr_ic_new failed for mr_ic_type_t %s", mr_ic_types[ic_type]);
	ck_assert_msg (callback (&ic, mr_ic_types[ic_type]), "test failed for mr_ic_type_t %s", mr_ic_types[ic_type]);
	mr_ic_free (&ic);
      }
}

static int ic_empty_cb (mr_ic_t * ic, char * mr_ic_type)
{
  uint32_t mask = 0;
  ck_assert_msg (MR_SUCCESS == mr_ic_foreach (ic, uintptr_t_visitor, &mask), "visitor failed for mr_ic_type %s", mr_ic_type);
  return (0 == mask);
}

START_TEST (ic_empty) { ic_types_foreach (ic_empty_cb); } END_TEST

static int ic_index_cb (mr_ic_t * ic, char * mr_ic_type)
{
  uint32_t mask = 0;
  uint32_t mask_expected = 0;
  int i;

  ck_assert_msg (MR_SUCCESS == mr_ic_index (ic, mr_ic_rarray, sizeof (mr_ic_rarray)), "index failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (MR_SUCCESS == mr_ic_foreach (ic, uintptr_t_visitor, &mask), "visitor failed for mr_ic_type %s", mr_ic_type);
  
  for (i = 0; i < sizeof (mr_ic_rarray) / sizeof (mr_ic_rarray[0]); ++i)
    uintptr_t_visitor (mr_ic_rarray[i], &mask_expected);
  return (mask == mask_expected);
}

START_TEST (ic_index) { ic_types_foreach (ic_index_cb); } END_TEST

static int ic_index_non_empty_cb (mr_ic_t * ic, char * mr_ic_type)
{
  mr_ptr_t x = { .uintptr = 4, }; /* value which is not presented in mr_ic_rarray */
  ck_assert_msg (NULL != mr_ic_add (ic, x), "add failed for mr_ic_type %s", mr_ic_type);
  return (ic_index_cb (ic, mr_ic_type));
}

START_TEST (ic_index_non_empty) { ic_types_foreach (ic_index_non_empty_cb); } END_TEST

TYPEDEF_STRUCT (key_value_t,
		string_t key,
		ANON_UNION (),
		VOID (char, unserializible),
		char value,
		END_ANON_UNION (),
		);

static mr_ptr_t kv[] = {
  { (key_value_t[]){{"key_a", { .value = 'a' }}}},
  { (key_value_t[]){{"key_x", { .value = 'x' }}}},
  { (key_value_t[]){{"key_x", { .value = 'X' }}}},
};
  
START_TEST (ic_index_order)
{
  int ic_type;
  mr_conf_init ();
  for (ic_type = 0; ic_type < sizeof (mr_ic_types) / sizeof (mr_ic_types[0]); ++ic_type)
    if (mr_ic_types[ic_type] != NULL)
      {
	mr_ic_t ic;
	ck_assert_msg (MR_SUCCESS == mr_ic_new (&ic, NULL, NULL, "key_value_t", ic_type, NULL),
		       "mr_ic_new failed for mr_ic_type_t %s", mr_ic_types[ic_type]);
	ck_assert_msg (MR_SUCCESS == mr_ic_index (&ic, kv, sizeof (kv)), "index failed for mr_ic_type %s", mr_ic_types[ic_type]);
	mr_ptr_t * find = mr_ic_find (&ic, (key_value_t[]){{"key_x"}});
	ck_assert_msg (find != NULL, "find failed for mr_ic_type %s", mr_ic_types[ic_type]);
	ck_assert_msg ('x' == ((key_value_t*)(find->ptr))->value, "found value mismatched expected for mr_ic_type %s", mr_ic_types[ic_type]);
	mr_ic_free (&ic);
      }
} END_TEST

static int ic_add_empty_cb (mr_ic_t * ic, char * mr_ic_type)
{
  uint32_t mask = 0;
  mr_ptr_t x = { .uintptr = 1, };
  ck_assert_msg (NULL != mr_ic_add (ic, x), "add failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (MR_SUCCESS == mr_ic_foreach (ic, uintptr_t_visitor, &mask), "visitor failed for mr_ic_type %s", mr_ic_type);
  return (mask == (1 << x.uintptr));
}

START_TEST (ic_add_empty) { ic_types_foreach (ic_add_empty_cb); } END_TEST

static int ic_add_existing_indexed_cb (mr_ic_t * ic, char * mr_ic_type)
{
  uint32_t mask = 0;
  uint32_t mask_expected = 0;
  mr_ptr_t x = { .uintptr = 1, };
  int i;

  ck_assert_msg (MR_SUCCESS == mr_ic_index (ic, mr_ic_rarray, sizeof (mr_ic_rarray)), "index failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (NULL != mr_ic_add (ic, x), "add failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (MR_SUCCESS == mr_ic_foreach (ic, uintptr_t_visitor, &mask), "sum failed for mr_ic_type %s", mr_ic_type);

  for (i = 0; i < sizeof (mr_ic_rarray) / sizeof (mr_ic_rarray[0]); ++i)
    uintptr_t_visitor (mr_ic_rarray[i], &mask_expected);
  return (mask == mask_expected);
}

START_TEST (ic_add_indexed) { ic_types_foreach (ic_add_existing_indexed_cb); } END_TEST

static int ic_add_non_indexed_cb (mr_ic_t * ic, char * mr_ic_type)
{
  uint32_t mask = 0;
  mr_ptr_t x = { .uintptr = 0, };
  uint32_t mask_expected = 1 << x.uintptr;
  int i;

  ck_assert_msg (MR_SUCCESS == mr_ic_index (ic, mr_ic_rarray, sizeof (mr_ic_rarray)), "index failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (NULL != mr_ic_add (ic, x), "add failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (MR_SUCCESS == mr_ic_foreach (ic, uintptr_t_visitor, &mask), "sum failed for mr_ic_type %s", mr_ic_type);

  for (i = 0; i < sizeof (mr_ic_rarray) / sizeof (mr_ic_rarray[0]); ++i)
    uintptr_t_visitor (mr_ic_rarray[i], &mask_expected);
  return (mask == mask_expected);
}

START_TEST (ic_add_non_indexed) { ic_types_foreach (ic_add_non_indexed_cb); } END_TEST

static int ic_del_add_cb (mr_ic_t * ic, char * mr_ic_type)
{
  uint32_t mask = 0;
  mr_ptr_t x = { .uintptr = 1, };
  ck_assert_msg (NULL != mr_ic_add (ic, x), "add failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (MR_SUCCESS == mr_ic_del (ic, x), "del failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (MR_SUCCESS == mr_ic_foreach (ic, uintptr_t_visitor, &mask), "sum failed for mr_ic_type %s", mr_ic_type);
  return (0 == mask);
}

START_TEST (ic_del_added) { ic_types_foreach (ic_del_add_cb); } END_TEST

static int ic_del_non_indexed_cb (mr_ic_t * ic, char * mr_ic_type)
{
  uint32_t mask = 0;
  mr_ptr_t x = { .uintptr = 1, };
  ck_assert_msg (MR_FAILURE == mr_ic_del (ic, x), "del failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (MR_SUCCESS == mr_ic_foreach (ic, uintptr_t_visitor, &mask), "sum failed for mr_ic_type %s", mr_ic_type);
  return (0 == mask);
}

START_TEST (ic_del_non_indexed) { ic_types_foreach (ic_del_non_indexed_cb); } END_TEST

static int ic_del_indexed_cb (mr_ic_t * ic, char * mr_ic_type)
{
  uint32_t mask = 0;
  uint32_t mask_expected = 0;
  int i;

  ck_assert_msg (MR_SUCCESS == mr_ic_index (ic, mr_ic_rarray, sizeof (mr_ic_rarray)), "index failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (MR_SUCCESS == mr_ic_del (ic, mr_ic_rarray[0]), "del failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (MR_SUCCESS == mr_ic_foreach (ic, uintptr_t_visitor, &mask), "sum failed for mr_ic_type %s", mr_ic_type);
  
  for (i = 1; i < sizeof (mr_ic_rarray) / sizeof (mr_ic_rarray[0]); ++i)
    uintptr_t_visitor (mr_ic_rarray[i], &mask_expected);
  return (mask == mask_expected);
}

START_TEST (ic_del_indexed) { ic_types_foreach (ic_del_indexed_cb); } END_TEST

static int ic_find_indexed_cb (mr_ic_t * ic, char * mr_ic_type)
{
  int i;
  mr_ptr_t * find;
  mr_ptr_t x = { .uintptr = 0, };
  
  ck_assert_msg (MR_SUCCESS == mr_ic_index (ic, mr_ic_rarray, sizeof (mr_ic_rarray)), "index failed for mr_ic_type %s", mr_ic_type);
  
  find = mr_ic_find (ic, x);
  ck_assert_msg (NULL == find, "Non-indexed element was found for mr_ic_type_t %s", mr_ic_type);
  
  for (i = 0; i < sizeof (mr_ic_rarray) / sizeof (mr_ic_rarray[0]); ++i)
    {
      find = mr_ic_find (ic, mr_ic_rarray[i]);
      ck_assert_msg (NULL != find, "Failed to find element in indexed data for mr_ic_type_t %s", mr_ic_type);
      ck_assert_msg (mr_ic_rarray[i].uintptr == find->uintptr,
		     "Found wrong element in indexed data for mr_ic_type_t %s", mr_ic_type);
    }
  return (!0);
}

START_TEST (ic_find_indexed) { ic_types_foreach (ic_find_indexed_cb); } END_TEST

static int ic_find_add_cb (mr_ic_t * ic, char * mr_ic_type)
{
  mr_ptr_t x = { .uintptr = 1, };
  ck_assert_msg (NULL != mr_ic_add (ic, x), "add failed for mr_ic_type %s", mr_ic_type);
  mr_ptr_t * find = mr_ic_find (ic, x);
  ck_assert_msg (NULL != find, "find failed for mr_ic_type %s", mr_ic_type);
  return (x.uintptr == find->uintptr);
}

START_TEST (ic_find_added) { ic_types_foreach (ic_find_add_cb); } END_TEST

MAIN_TEST_SUITE ((ic_empty, "Check empty ic"),
		 (ic_index, "Check indexing of empty collection"),
		 (ic_index_non_empty, "Check indexing of non empty collection"),
		 (ic_index_order, "Check indexing order"),
		 (ic_add_empty, "Check add to empty collection"),
		 (ic_add_indexed, "Add existing element to indexed collection"),
		 (ic_add_non_indexed, "Add new element to indexed collection"),
		 (ic_del_added, "Check deletion of added element"),
		 (ic_del_non_indexed, "Check deletion of non indexed element"),
		 (ic_del_indexed, "Check deletion of indexed element"),
		 (ic_find_indexed, "Check find over indexed data"),
		 (ic_find_added, "Check search of added element")
		 );
