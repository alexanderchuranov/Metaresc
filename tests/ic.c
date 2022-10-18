#include <stdlib.h>

#include <check.h>
#include <metaresc.h>
#include <mr_btree.h>
#include <regression.h>

#include <mr_ic.h>

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
uintptr_t_sum_visitor (mr_ptr_t node, const void * context)
{
  long * sum = (long*) context;
  *sum += node.uintptr;
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
    MR_IC_INIT (HASH),
    MR_IC_INIT (STATIC_ARRAY),
    MR_IC_INIT (RBTREE),
    MR_IC_INIT (AVLTREE),
  };

  for (i = 0; i < sizeof (mr_ic_types) / sizeof (mr_ic_types[0]); ++i)
    if (mr_ic_types[i] != NULL)
      {
	mr_ic_t ic;
	ck_assert_msg (MR_SUCCESS == mr_ic_new (&ic, uintptr_t_get_hash, uintptr_t_cmp, "uintptr", i, NULL),
		       "mr_ic_new failed for mr_ic_type_t %s", mr_ic_types[i]);
	ck_assert_msg (callback (&ic, mr_ic_types[i]), "test failed for mr_ic_type_t %s", mr_ic_types[i]);
	mr_ic_free (&ic);
      }
}

static mr_ic_rarray_t mr_ic_rarray = {
  .ra = (mr_ptr_t[]) {
    { .uintptr = 1, },
    { .uintptr = 2, },
    { .uintptr = 3, },
  },
  .size = 3 * sizeof (mr_ic_rarray.ra[0]),
  .alloc_size = -1,
};

static int ic_empty_cb (mr_ic_t * ic, char * mr_ic_type)
{
  long sum = 0;
  ck_assert_msg (MR_SUCCESS == mr_ic_foreach (ic, uintptr_t_sum_visitor, &sum), "sum failed for mr_ic_type %s", mr_ic_type);
  return (0 == sum);
}

START_TEST (ic_empty) { ic_types_foreach (ic_empty_cb); } END_TEST

static int ic_index_cb (mr_ic_t * ic, char * mr_ic_type)
{
  long sum = 0;
  long sum_ = 0;
  int i;

  ck_assert_msg (MR_SUCCESS == mr_ic_index (ic, mr_ic_rarray.ra, mr_ic_rarray.size), "index failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (MR_SUCCESS == mr_ic_foreach (ic, uintptr_t_sum_visitor, &sum), "sum failed for mr_ic_type %s", mr_ic_type);
  
  for (i = 0; i < mr_ic_rarray.size / sizeof (mr_ic_rarray.ra[0]); ++i)
    sum_ += mr_ic_rarray.ra[i].uintptr;
  return (sum_ == sum);
}

START_TEST (ic_index) { ic_types_foreach (ic_index_cb); } END_TEST

static int ic_add_empty_cb (mr_ic_t * ic, char * mr_ic_type)
{
  long sum = 0;
  mr_ptr_t x = { .uintptr = 1, };
  ck_assert_msg (NULL != mr_ic_add (ic, x), "add failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (MR_SUCCESS == mr_ic_foreach (ic, uintptr_t_sum_visitor, &sum), "sum failed for mr_ic_type %s", mr_ic_type);
  return (x.uintptr == sum);
}

START_TEST (ic_add_empty) { ic_types_foreach (ic_add_empty_cb); } END_TEST

static int ic_add_existing_indexed_cb (mr_ic_t * ic, char * mr_ic_type)
{
  long sum = 0;
  mr_ptr_t x = { .uintptr = 1, };
  long sum_ = 0;
  int i;

  ck_assert_msg (MR_SUCCESS == mr_ic_index (ic, mr_ic_rarray.ra, mr_ic_rarray.size), "index failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (NULL != mr_ic_add (ic, x), "add failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (MR_SUCCESS == mr_ic_foreach (ic, uintptr_t_sum_visitor, &sum), "sum failed for mr_ic_type %s", mr_ic_type);

  for (i = 0; i < mr_ic_rarray.size / sizeof (mr_ic_rarray.ra[0]); ++i)
    sum_ += mr_ic_rarray.ra[i].uintptr;
  return (sum_ == sum);
}

START_TEST (ic_add_indexed) { ic_types_foreach (ic_add_existing_indexed_cb); } END_TEST

static int ic_add_non_indexed_cb (mr_ic_t * ic, char * mr_ic_type)
{
  long sum = 0;
  mr_ptr_t x = { .uintptr = -1, };
  long sum_ = x.uintptr;
  int i;

  ck_assert_msg (MR_SUCCESS == mr_ic_index (ic, mr_ic_rarray.ra, mr_ic_rarray.size), "index failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (NULL != mr_ic_add (ic, x), "add failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (MR_SUCCESS == mr_ic_foreach (ic, uintptr_t_sum_visitor, &sum), "sum failed for mr_ic_type %s", mr_ic_type);

  for (i = 0; i < mr_ic_rarray.size / sizeof (mr_ic_rarray.ra[0]); ++i)
    sum_ += mr_ic_rarray.ra[i].uintptr;
  return (sum_ == sum);
}

START_TEST (ic_add_non_indexed) { ic_types_foreach (ic_add_non_indexed_cb); } END_TEST

static int ic_del_add_cb (mr_ic_t * ic, char * mr_ic_type)
{
  long sum = 0;
  mr_ptr_t x = { .uintptr = 1, };
  ck_assert_msg (NULL != mr_ic_add (ic, x), "add failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (MR_SUCCESS == mr_ic_del (ic, x), "del failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (MR_SUCCESS == mr_ic_foreach (ic, uintptr_t_sum_visitor, &sum), "sum failed for mr_ic_type %s", mr_ic_type);
  return (0 == sum);
}

START_TEST (ic_del_added) { ic_types_foreach (ic_del_add_cb); } END_TEST

static int ic_del_indexed_cb (mr_ic_t * ic, char * mr_ic_type)
{
  long sum = 0;
  long sum_ = 0;
  int i;

  ck_assert_msg (MR_SUCCESS == mr_ic_index (ic, mr_ic_rarray.ra, mr_ic_rarray.size), "index failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (MR_SUCCESS == mr_ic_del (ic, mr_ic_rarray.ra[0]), "del failed for mr_ic_type %s", mr_ic_type);
  ck_assert_msg (MR_SUCCESS == mr_ic_foreach (ic, uintptr_t_sum_visitor, &sum), "sum failed for mr_ic_type %s", mr_ic_type);
  
  for (i = 1; i < mr_ic_rarray.size / sizeof (mr_ic_rarray.ra[0]); ++i)
    sum_ += mr_ic_rarray.ra[i].uintptr;
  return (sum_ == sum);
}

START_TEST (ic_del_indexed) { ic_types_foreach (ic_del_indexed_cb); } END_TEST

static int ic_find_indexed_cb (mr_ic_t * ic, char * mr_ic_type)
{
  int i;
  mr_ptr_t * find;
  mr_ptr_t x = { .uintptr = -1, };
  
  ck_assert_msg (MR_SUCCESS == mr_ic_index (ic, mr_ic_rarray.ra, mr_ic_rarray.size), "index failed for mr_ic_type %s", mr_ic_type);
  
  find = mr_ic_find (ic, x);
  ck_assert_msg (NULL == find, "Non-indexed element was found for mr_ic_type_t %s", mr_ic_type);
  
  for (i = 0; i < mr_ic_rarray.size / sizeof (mr_ic_rarray.ra[0]); ++i)
    {
      find = mr_ic_find (ic, mr_ic_rarray.ra[i]);
      ck_assert_msg (NULL != find, "Failed to find element in indexed data for mr_ic_type_t %s", mr_ic_type);
      ck_assert_msg (mr_ic_rarray.ra[i].uintptr == find->uintptr,
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

START_TEST (ic_rbtree) { 
  mr_tree_t rbtree;
  mr_tree_init (&rbtree);

  mr_ptr_t x = { .uintptr = 0 };
  mr_ptr_t * rv0 = mr_rbtree_add (x, &rbtree, uintptr_t_cmp, NULL);
  ck_assert_msg (NULL != rv0, "Failed to add value to binary tree");
  ck_assert_msg (rv0->uintptr == x.uintptr, "Mismatched key");
  ck_assert_msg (mr_rbtree_is_valid (&rbtree, uintptr_t_cmp, NULL), "Invalid tree");

  mr_ptr_t * rv = mr_rbtree_add (x, &rbtree, uintptr_t_cmp, NULL);
  ck_assert_msg (NULL != rv, "Failed to add value to binary tree");
  ck_assert_msg (rv->uintptr == x.uintptr, "Mismatched key");
  ck_assert_msg (rv == rv0, "Wrong key found");
  ck_assert_msg (mr_rbtree_is_valid (&rbtree, uintptr_t_cmp, NULL), "Invalid tree");

  int i;
  for (i = 1; i < (1 << 10); ++i)
    {
      mr_ptr_t value = { .uintptr = i };
      mr_ptr_t * rv1 = mr_rbtree_add (value, &rbtree, uintptr_t_cmp, NULL);
      ck_assert_msg (NULL != rv1, "Failed to add value to binary tree");
      ck_assert_msg (rv1->uintptr == value.uintptr, "Mismatched key");
      ck_assert_msg (mr_rbtree_is_valid (&rbtree, uintptr_t_cmp, NULL), "Invalid tree");
    }

  for (i = rbtree.size / sizeof (rbtree.pool[0]) - 1; i > 0; --i)
    {
      mr_ptr_t value = rbtree.pool[1].key;
      mr_status_t status = mr_rbtree_del (value, &rbtree, uintptr_t_cmp, NULL);
      ck_assert_msg (MR_SUCCESS == status, "Deletion rerurned failed status");
      ck_assert_msg (i == rbtree.size / sizeof (rbtree.pool[0]), "Failed to del value from binary tree");
      ck_assert_msg (mr_rbtree_is_valid (&rbtree, uintptr_t_cmp, NULL), "Invalid tree");
    }

  mr_tree_free (&rbtree);
} END_TEST

START_TEST (ic_avltree) { 
  mr_tree_t tree;
  mr_tree_init (&tree);

  mr_ptr_t x = { .uintptr = 0 };
  mr_ptr_t * rv0 = mr_avltree_add (x, &tree, uintptr_t_cmp, NULL);
  ck_assert_msg (NULL != rv0, "Failed to add value to binary tree");
  ck_assert_msg (rv0->uintptr == x.uintptr, "Mismatched key");
  ck_assert_msg (mr_avltree_is_valid (&tree, uintptr_t_cmp, NULL), "Invalid tree");

  mr_ptr_t * rv = mr_avltree_add (x, &tree, uintptr_t_cmp, NULL);
  ck_assert_msg (NULL != rv, "Failed to add value to binary tree");
  ck_assert_msg (rv->uintptr == x.uintptr, "Mismatched key");
  ck_assert_msg (rv == rv0, "Wrong key found");
  ck_assert_msg (mr_avltree_is_valid (&tree, uintptr_t_cmp, NULL), "Invalid tree");

  int i;
  for (i = 1; i < 1 << 10; ++i)
    {
      mr_ptr_t value = { .uintptr = i };
      mr_ptr_t * rv1 = mr_avltree_add (value, &tree, uintptr_t_cmp, NULL);
      ck_assert_msg (NULL != rv1, "Failed to add value to binary tree");
      ck_assert_msg (rv1->uintptr == value.uintptr, "Mismatched key");
      ck_assert_msg (mr_avltree_is_valid (&tree, uintptr_t_cmp, NULL), "Invalid tree");
    }

  for (i = tree.size / sizeof (tree.pool[0]) - 1; i > 0; --i)
    {
      mr_ptr_t value = tree.pool[1].key;
      mr_status_t status = mr_avltree_del (value, &tree, uintptr_t_cmp, NULL);
      ck_assert_msg (MR_SUCCESS == status, "Deletion rerurned failed status");
      ck_assert_msg (i == tree.size / sizeof (tree.pool[0]), "Failed to del value from binary tree");
      ck_assert_msg (mr_avltree_is_valid (&tree, uintptr_t_cmp, NULL), "Invalid tree");
    }
  
  mr_tree_free (&tree);
} END_TEST

MAIN_TEST_SUITE ((ic_empty, "Check empty ic"),
		 (ic_index, "Check indexing"),
		 (ic_add_empty, "Check add to empty collection"),
		 (ic_add_indexed, "Add existing element to indexed collection"),
		 (ic_add_non_indexed, "Add new element to indexed collection"),
		 (ic_del_added, "Check deletion of added element"),
		 (ic_del_indexed, "Check deletion of indexed element"),
		 (ic_find_indexed, "Check find over indexed data"),
		 (ic_find_added, "Check search of added element"),
		 (ic_rbtree, "Check red/black tree implementation"),
		 (ic_avltree, "Check AVL tree implementation")
		 );
