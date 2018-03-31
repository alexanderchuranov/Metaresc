#include <check.h>
#include <metaresc.h>
#include <regression.h>

#include <mr_ic.h>

static mr_hash_value_t
long_int_t_get_hash (mr_ptr_t node, const void * context)
{
  return (node.long_int_t);
}

static int
long_int_t_cmp (mr_ptr_t x, mr_ptr_t y, const void * context)
{
  return (x.long_int_t - y.long_int_t);
}

static mr_status_t
long_int_t_sum_visitor (mr_ptr_t node, const void * context)
{
  long * sum = (long*) context;
  *sum += node.long_int_t;
  return (MR_SUCCESS);
}

static void
ic_types_foreach (int (*callback) (mr_ic_t * ic, char * mr_ic_type))
{
  int i;
#define MR_IC_INIT(TYPE) [MR_IC_##TYPE] = #TYPE

  static char *
    mr_ic_types[] = {
    MR_IC_INIT (UNSORTED_ARRAY),
    MR_IC_INIT (SORTED_ARRAY),
    MR_IC_INIT (RBTREE),
    MR_IC_INIT (HASH_TREE),
    MR_IC_INIT (HASH_NEXT),
  };

  for (i = 0; i < sizeof (mr_ic_types) / sizeof (mr_ic_types[0]); ++i)
    if (mr_ic_types[i] != NULL)
      {
	mr_ic_t ic;
	ck_assert_msg (MR_SUCCESS == mr_ic_new (&ic, long_int_t_get_hash, long_int_t_cmp, "long_int_t", i, NULL),
		       "mr_ic_new failed for mr_ic_type_t %s", mr_ic_types[i]);
	ck_assert_msg (callback (&ic, mr_ic_types[i]), "test failed for mr_ic_type_t %s", mr_ic_types[i]);
	mr_ic_free (&ic);
      }
}

static mr_ic_rarray_t mr_ic_rarray = {
  .ra = (mr_ptr_t[]) {
    { .long_int_t = 1, },
    { .long_int_t = 2, },
    { .long_int_t = 3, },
  },
  .size = 3 * sizeof (mr_ic_rarray.ra[0]),
  .alloc_size = -1,
};

static int ic_empty_cb (mr_ic_t * ic, char * mr_ic_type)
{
  long sum = 0;
  ck_assert_msg (MR_SUCCESS == mr_ic_foreach (ic, long_int_t_sum_visitor, &sum), "sum failed for mr_ic_type %s", mr_ic_type);
  return (0 == sum);
}

MR_START_TEST (ic_empty, "Check empty ic") { ic_types_foreach (ic_empty_cb); } END_TEST

static int ic_index_cb (mr_ic_t * ic, char * mr_ic_type)
{
  long sum = 0;
  long sum_ = 0;
  int i;

  ck_assert_msg (MR_SUCCESS == mr_ic_index (ic, &mr_ic_rarray), "index failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (MR_SUCCESS == mr_ic_foreach (ic, long_int_t_sum_visitor, &sum), "sum failed for mr_ic_type %s", mr_ic_type);
  
  for (i = 0; i < mr_ic_rarray.size / sizeof (mr_ic_rarray.ra[0]); ++i)
    sum_ += mr_ic_rarray.ra[i].long_int_t;
  return (sum_ == sum);
}

MR_START_TEST (ic_index, "Check indexing") { ic_types_foreach (ic_index_cb); } END_TEST

static int ic_add_empty_cb (mr_ic_t * ic, char * mr_ic_type)
{
  long sum = 0;
  mr_ptr_t x = { .long_int_t = 1, };
  ck_assert_msg (NULL != mr_ic_add (ic, x), "add failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (MR_SUCCESS == mr_ic_foreach (ic, long_int_t_sum_visitor, &sum), "sum failed for mr_ic_type %s", mr_ic_type);
  return (x.long_int_t == sum);
}

MR_START_TEST (ic_add_empty, "Check add to empty collection") { ic_types_foreach (ic_add_empty_cb); } END_TEST

static int ic_add_existing_indexed_cb (mr_ic_t * ic, char * mr_ic_type)
{
  long sum = 0;
  mr_ptr_t x = { .long_int_t = 1, };
  long sum_ = 0;
  int i;

  ck_assert_msg (MR_SUCCESS == mr_ic_index (ic, &mr_ic_rarray), "index failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (NULL != mr_ic_add (ic, x), "add failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (MR_SUCCESS == mr_ic_foreach (ic, long_int_t_sum_visitor, &sum), "sum failed for mr_ic_type %s", mr_ic_type);

  for (i = 0; i < mr_ic_rarray.size / sizeof (mr_ic_rarray.ra[0]); ++i)
    sum_ += mr_ic_rarray.ra[i].long_int_t;
  return (sum_ == sum);
}

MR_START_TEST (ic_add_indexed, "Add existing element to indexed collection") { ic_types_foreach (ic_add_existing_indexed_cb); } END_TEST

static int ic_add_non_indexed_cb (mr_ic_t * ic, char * mr_ic_type)
{
  long sum = 0;
  mr_ptr_t x = { .long_int_t = -1, };
  long sum_ = x.long_int_t;
  int i;

  ck_assert_msg (MR_SUCCESS == mr_ic_index (ic, &mr_ic_rarray), "index failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (NULL != mr_ic_add (ic, x), "add failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (MR_SUCCESS == mr_ic_foreach (ic, long_int_t_sum_visitor, &sum), "sum failed for mr_ic_type %s", mr_ic_type);

  for (i = 0; i < mr_ic_rarray.size / sizeof (mr_ic_rarray.ra[0]); ++i)
    sum_ += mr_ic_rarray.ra[i].long_int_t;
  return (sum_ == sum);
}

MR_START_TEST (ic_add_non_indexed, "Add new element to indexed collection") { ic_types_foreach (ic_add_non_indexed_cb); } END_TEST

static int ic_del_add_cb (mr_ic_t * ic, char * mr_ic_type)
{
  long sum = 0;
  mr_ptr_t x = { .long_int_t = 1, };
  ck_assert_msg (NULL != mr_ic_add (ic, x), "add failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (MR_SUCCESS == mr_ic_del (ic, x), "del failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (MR_SUCCESS == mr_ic_foreach (ic, long_int_t_sum_visitor, &sum), "sum failed for mr_ic_type %s", mr_ic_type);
  return (0 == sum);
}

MR_START_TEST (ic_del_added, "Check deletion of added element") { ic_types_foreach (ic_del_add_cb); } END_TEST

static int ic_del_indexed_cb (mr_ic_t * ic, char * mr_ic_type)
{
  long sum = 0;
  long sum_ = 0;
  int i;

  ck_assert_msg (MR_SUCCESS == mr_ic_index (ic, &mr_ic_rarray), "index failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (MR_SUCCESS == mr_ic_del (ic, mr_ic_rarray.ra[0]), "del failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (MR_SUCCESS == mr_ic_foreach (ic, long_int_t_sum_visitor, &sum), "sum failed for mr_ic_type %s", mr_ic_type);
  
  for (i = 1; i < mr_ic_rarray.size / sizeof (mr_ic_rarray.ra[0]); ++i)
    sum_ += mr_ic_rarray.ra[i].long_int_t;
  return (sum_ == sum);
}

MR_START_TEST (ic_del_indexed, "Check deletion of indexed element") { ic_types_foreach (ic_del_indexed_cb); } END_TEST

static int ic_find_indexed_cb (mr_ic_t * ic, char * mr_ic_type)
{
  int i;
  mr_ptr_t * find;
  mr_ptr_t x = { .long_int_t = -1, };
  
  ck_assert_msg (MR_SUCCESS == mr_ic_index (ic, &mr_ic_rarray), "index failed for mr_ic_type %s", mr_ic_type);
  
  find = mr_ic_find (ic, x);
  ck_assert_msg (NULL == find, "Non-indexed element was found for mr_ic_type_t %s", mr_ic_type);
  
  for (i = 0; i < mr_ic_rarray.size / sizeof (mr_ic_rarray.ra[0]); ++i)
    {
      find = mr_ic_find (ic, mr_ic_rarray.ra[i]);
      ck_assert_msg (NULL != find, "Failed to find element in indexed data for mr_ic_type_t %s", mr_ic_type);
      ck_assert_msg (mr_ic_rarray.ra[i].long_int_t == find->long_int_t,
		     "Found wrong element in indexed data for mr_ic_type_t %s", mr_ic_type);
    }
  return (!0);
}

MR_START_TEST (ic_find_indexed, "Check find over indexed data") { ic_types_foreach (ic_find_indexed_cb); } END_TEST

static int ic_find_add_cb (mr_ic_t * ic, char * mr_ic_type)
{
  mr_ptr_t x = { .long_int_t = 1, };
  ck_assert_msg (NULL != mr_ic_add (ic, x), "add failed for mr_ic_type %s", mr_ic_type);
  mr_ptr_t * find = mr_ic_find (ic, x);
  ck_assert_msg (NULL != find, "find failed for mr_ic_type %s", mr_ic_type);
  return (x.long_int_t == find->long_int_t);
}

MR_START_TEST (ic_find_added, "Check search of added element") { ic_types_foreach (ic_find_add_cb); } END_TEST

MAIN ();
