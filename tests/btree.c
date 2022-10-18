#include <stdlib.h>

#include <check.h>
#include <metaresc.h>
#include <mr_btree.h>
#include <regression.h>

static int
uintptr_t_cmp (mr_ptr_t x, mr_ptr_t y, const void * context)
{
  return (x.uintptr > y.uintptr) - (x.uintptr < y.uintptr);
}

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

MAIN_TEST_SUITE (
		 (ic_rbtree, "Check red/black tree implementation"),
		 (ic_avltree, "Check AVL tree implementation")
		 );
