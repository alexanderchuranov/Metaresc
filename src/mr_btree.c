/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

#include <metaresc.h>
#include <mr_btree.h>

#define NONE_IDX 0
#define FLIP 1
#define MR_PATH_SIZE (sizeof (((mr_tree_traverse_t*)0)->path) / sizeof (((mr_tree_traverse_t*)0)->path[0]))

void
mr_tree_init (mr_tree_t * tree)
{
  memset (tree, 0, sizeof (*tree));
}

void
mr_tree_free (mr_tree_t * tree)
{
  if (tree->pool)
    MR_FREE (tree->pool);
  mr_tree_init (tree);
}

mr_status_t
mr_tree_reserve (mr_tree_t * tree, unsigned items_count, bool reset)
{
  if (reset)
    tree->size = 0;

  if (0 == items_count)
    return (MR_SUCCESS);

  size_t alloc_size = (items_count + 1) * sizeof (tree->pool[0]); /* add extra slot for root node */

  if (tree->alloc_size < alloc_size)
    {
      void * pool = MR_REALLOC (tree->pool, alloc_size);
      if (NULL == pool)
	{
	  MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	  return (MR_FAILURE);
	}
      tree->pool = pool;
      tree->alloc_size = alloc_size;
    }
  return (MR_SUCCESS);
}

static unsigned
mr_tree_node_new (mr_tree_t * tree)
{
  typeof (tree->pool) node = mr_rarray_allocate_element
    ((void**)&tree->pool, &tree->size, &tree->alloc_size, sizeof (tree->pool[0]));

  if (NULL == node)
    return (NONE_IDX);

  memset (node, 0, sizeof (*node));
  return (tree->size / sizeof (tree->pool[0]) - 1);
}

mr_status_t
mr_tree_walk (mr_tree_t * tree, mr_visit_fn_t visit_fn, void * context)
{
  unsigned idx, size = 0;
  unsigned path[MR_PATH_SIZE];
  
  if (tree->size < 2 * sizeof (tree->pool[0]))
    return (MR_SUCCESS);
  
  for (idx = tree->pool->root.idx; ; idx = tree->pool[idx].next[MR_RIGHT].idx)
    {
      for ( ; idx != NONE_IDX; idx = tree->pool[idx].next[MR_LEFT].idx)
	path[size++] = idx;
      if (0 == size)
	break;
      idx = path[--size];
      if (MR_SUCCESS != visit_fn (tree->pool[idx].key, context))
	return (MR_FAILURE);
    }
  return (MR_SUCCESS);
}

void
mr_tree_next (mr_tree_t * tree, mr_tree_traverse_t * traverse, mr_child_idx_t next)
{
  unsigned traverse_size = traverse->size / sizeof (traverse->path[0]);
  if (traverse_size <= 1)
    return;
  
  if (traverse->equal || (traverse->path[traverse_size - 1].child_idx == next))
    {
      unsigned idx = traverse->path[traverse_size - 1].idx;
      traverse->path[traverse_size - 1].child_idx = next;
      if (tree->pool[idx].next[next].idx != NONE_IDX)
	for (idx = tree->pool[idx].next[next].idx;
	     idx != NONE_IDX;
	     idx = tree->pool[idx].next[next ^ FLIP].idx)
	  traverse->path[traverse_size++] = (typeof (traverse->path[0])){
	    .idx = idx,
	    .child_idx = next ^ FLIP,
	  };
      else
	while (--traverse_size > 1)
	  if (traverse->path[traverse_size - 1].child_idx != next)
	    break;
    }

  traverse->equal = true;
  traverse->size = traverse_size * sizeof (traverse->path[0]);
}

void
mr_tree_find (mr_ptr_t key, mr_tree_t * tree, mr_compar_fn_t compar_fn, void * context, mr_tree_traverse_t * traverse)
{
  unsigned traverse_size = 0;
  bool equal = false;
  unsigned idx;

  traverse->path[traverse_size++] = (typeof (traverse->path[0])){
    .idx = NONE_IDX,
    .child_idx = MR_LEFT,
  };
  
  if (tree->size >= 2 * sizeof (tree->pool[0]))
    for (idx = tree->pool->root.idx; idx != NONE_IDX; )
      {
	int cmp = compar_fn (key, tree->pool[idx].key, context);
	mr_child_idx_t child_idx = (cmp > 0) ? MR_RIGHT : MR_LEFT;

	equal = (0 == cmp);
	traverse->path[traverse_size++] = (typeof (traverse->path[0])){
	  .idx = idx,
	  .child_idx = child_idx,
	};
	
	if (equal)
	  break;
	idx = tree->pool[idx].next[child_idx].idx;
      }

  traverse->equal = equal;
  traverse->size = traverse_size * sizeof (traverse->path[0]);
}

static inline mr_ptr_t *
mr_tree_add (mr_ptr_t key, mr_tree_t * tree, mr_compar_fn_t compar_fn, void * context, void (rebalance) (mr_tree_t *, mr_tree_traverse_t *))
{
  if (tree->size <= 0)
    {
      mr_tree_node_new (tree);
      if (NULL == tree->pool)
	return (NULL);
    }

  mr_tree_traverse_t traverse;
  mr_tree_find (key, tree, compar_fn, context, &traverse);
  unsigned traverse_size = traverse.size / sizeof (traverse.path[0]);
  if (traverse.equal)
    return (&tree->pool[traverse.path[traverse_size - 1].idx].key);

  unsigned idx = mr_tree_node_new (tree);
  if (NONE_IDX == idx)
    return (NULL);

  traverse.path[traverse_size].idx = idx;
  tree->pool[idx].key = key;

  mr_tree_path_t * parent = &traverse.path[traverse_size - 1];
  tree->pool[parent->idx].next[parent->child_idx].idx = idx;

  rebalance (tree, &traverse);
  
  return (&tree->pool[idx].key);
}

static inline mr_status_t
mr_tree_del (mr_ptr_t key, mr_tree_t * tree, mr_compar_fn_t compar_fn, void * context, void (rebalance) (mr_tree_t *, mr_tree_traverse_t *))
{
  mr_tree_traverse_t traverse;
  mr_tree_find (key, tree, compar_fn, context, &traverse);

  if (!traverse.equal)
    return (MR_FAILURE);

  unsigned traverse_size = traverse.size / sizeof (traverse.path[0]);
  unsigned node = traverse.path[traverse_size - 1].idx;
  unsigned del = node;

  if ((NONE_IDX == tree->pool[node].next[MR_LEFT].idx) || (NONE_IDX == tree->pool[node].next[MR_RIGHT].idx))
    --traverse_size;
  else
    {
      traverse.path[traverse_size - 1].child_idx = MR_RIGHT;
      for (del = tree->pool[node].next[MR_RIGHT].idx;
	   tree->pool[del].next[MR_LEFT].idx != NONE_IDX;
	   del = tree->pool[del].next[MR_LEFT].idx)
	traverse.path[traverse_size++] = (typeof (traverse.path[0])){ .child_idx = MR_LEFT, .idx = del, };
      traverse.path[traverse_size].idx = del;
    }
  traverse.size = traverse_size * sizeof (traverse.path[0]);

  mr_tree_path_t * parent = &traverse.path[traverse_size - 1];
  tree->pool[parent->idx].next[parent->child_idx].idx =
    tree->pool[del].next[(tree->pool[del].next[MR_RIGHT].idx != NONE_IDX) ? MR_RIGHT : MR_LEFT].idx;

  tree->pool[node].key = tree->pool[del].key;

  rebalance (tree, &traverse);

  tree->size -= sizeof (tree->pool[0]);
  unsigned last = tree->size / sizeof (tree->pool[0]);
  if (del != last)
    {
      mr_tree_find (tree->pool[last].key, tree, compar_fn, context, &traverse);
      if (!traverse.equal)
	return (MR_FAILURE);
      
      tree->pool[del] = tree->pool[last];
      parent = &traverse.path[traverse.size / sizeof (traverse.path[0]) - 2];
      tree->pool[parent->idx].next[parent->child_idx].idx = del;
    }

  return (MR_SUCCESS);
}

TYPEDEF_FUNC (bool, node_validator_t, (mr_tree_t * /* tree */, unsigned /* idx */, uint8_t * /* height */));

static bool
mr_btree_is_valid_recurse (mr_tree_t * tree, unsigned idx, mr_compar_fn_t cmp, void * context, node_validator_t node_validator, uint8_t * height, int level, unsigned left, unsigned right)
{
  if (NONE_IDX == idx)
    return (true);
  
  if (++level > MR_PATH_SIZE)
    {
      fprintf (stderr, "Node [%u] exceed maximum tree level %d\n", idx, level);
      return (false);
    }
  
  if (height[idx] != 0)
    {
      fprintf (stderr, "Node [%u] double linked\n", idx);
      return (false);
    }

  if ((left != NONE_IDX) && (cmp (tree->pool[idx].key, tree->pool[left].key, context) <= 0))
    {
      fprintf (stderr, "Tree unordered. node (%u) <= left (%u)\n", idx, left);
      return (false);
    }

  if ((right != NONE_IDX) && (cmp (tree->pool[right].key, tree->pool[idx].key, context) <= 0))
    {
      fprintf (stderr, "Tree unordered. node (%u) => right (%u)\n", idx, right);
      return (false);
    }

  if (!mr_btree_is_valid_recurse (tree, tree->pool[idx].next[MR_LEFT].idx, cmp, context, node_validator, height, level, left, idx))
    return (false);
  if (!mr_btree_is_valid_recurse (tree, tree->pool[idx].next[MR_RIGHT].idx, cmp, context, node_validator, height, level, idx, right))
    return (false);
  
  return (node_validator (tree, idx, height));
}

static bool
mr_btree_is_valid (mr_tree_t * tree, mr_compar_fn_t cmp, void * context, node_validator_t node_validator)
{
  if (tree->size <= 0)
    return (true);

  unsigned idx, count = tree->size / sizeof (tree->pool[0]);
  mr_child_idx_t child_idx;
  
  if (0 == count)
    {
      fprintf (stderr, "Tree is not empty, but it has no root\n");
      return (false);
    }

  for (idx = 0; idx < count; ++idx)
    for (child_idx = MR_LEFT; child_idx <= MR_RIGHT; ++child_idx)
      if (tree->pool[idx].next[child_idx].idx >= count)
	{
	  fprintf (stderr, "Node [%u] has a child reference out of range %u > %u\n", idx, tree->pool[idx].next[child_idx].idx, count);
	  return (false);
	}

  uint8_t * height = MR_CALLOC (count, sizeof (*height));
  
  if (NULL == height)
    return (false);

  height[NONE_IDX] = 1;
  bool valid = mr_btree_is_valid_recurse (tree, tree->pool->root.idx, cmp, context, node_validator, height, 0, NONE_IDX, NONE_IDX);

  if (valid)
    for (idx = 0; idx < count; ++idx)
      if (0 == height[idx])
	{
	  valid = false;
	  fprintf (stderr, "Node [%u] is not linked\n", idx);
	  break;
	}

  MR_FREE (height);
  return (valid);
}

static inline void
mr_tree_rotate (mr_tree_t * tree, mr_tree_path_t * path, mr_child_idx_t child_idx)
{
  unsigned node = path[0].idx;
  unsigned child = tree->pool[node].next[child_idx].idx;
  
  tree->pool[node].next[child_idx].idx = tree->pool[child].next[child_idx ^ FLIP].idx;
  tree->pool[child].next[child_idx ^ FLIP].idx = node;

  tree->pool[path[-1].idx].next[path[-1].child_idx].idx = child;
}

static inline void 
mr_tree_grand_rotate (mr_tree_t * tree, mr_tree_path_t * path, mr_child_idx_t child_idx)
{
  unsigned node = path[0].idx;
  unsigned child = tree->pool[node].next[child_idx].idx;
  unsigned grand_child = tree->pool[child].next[child_idx ^ FLIP].idx;
  
  tree->pool[node].next[child_idx].idx = tree->pool[grand_child].next[child_idx ^ FLIP].idx;
  tree->pool[child].next[child_idx ^ FLIP].idx = tree->pool[grand_child].next[child_idx].idx;
  
  tree->pool[grand_child].next[child_idx ^ FLIP].idx = node;
  tree->pool[grand_child].next[child_idx].idx = child;

  tree->pool[path[-1].idx].next[path[-1].child_idx].idx = grand_child;
}

static inline void 
mr_rbtree_rebalance_add (mr_tree_t * rbtree, mr_tree_traverse_t * traverse)
{
  unsigned traverse_size = traverse->size / sizeof (traverse->path[0]);
  mr_tree_path_t * path = traverse->path;
  
  rbtree->pool[path[traverse_size].idx].rb.red = true;
  
  if (rbtree->pool[path[traverse_size - 1].idx].rb.red)
    while ((traverse_size > 2) && rbtree->pool[path[traverse_size - 1].idx].rb.red)
      {
	unsigned node = path[traverse_size].idx;
	unsigned parent = path[--traverse_size].idx;
	unsigned grand_parent = path[--traverse_size].idx;
	mr_child_idx_t parent_idx = path[traverse_size].child_idx;
	unsigned uncle = rbtree->pool[grand_parent].next[parent_idx ^ FLIP].idx;
      
	if (!rbtree->pool[uncle].rb.red)
	  {
	    unsigned brother = rbtree->pool[parent].next[parent_idx ^ FLIP].idx;

	    if (node != brother)
	      rbtree->pool[parent].rb.red = false;
	    else
	      {
		mr_tree_rotate (rbtree, &path[traverse_size + 1], parent_idx ^ FLIP);
		rbtree->pool[node].rb.red = false;
	      }

	    mr_tree_rotate (rbtree, &path[traverse_size], parent_idx);
	    rbtree->pool[grand_parent].rb.red = true;
	    break;
	  }

	rbtree->pool[parent].rb.red = false;
	rbtree->pool[uncle].rb.red = false;
	rbtree->pool[grand_parent].rb.red = true;
      }

  rbtree->pool[rbtree->pool->root.idx].rb.red = false;
}

mr_ptr_t *
mr_rbtree_add (mr_ptr_t key, mr_tree_t * tree, mr_compar_fn_t compar_fn, void * context)
{
  return (mr_tree_add (key, tree, compar_fn, context, mr_rbtree_rebalance_add));
}

static void
mr_rbtree_rebalance_del (mr_tree_t * rbtree, mr_tree_traverse_t * traverse)
{
  unsigned traverse_size = traverse->size / sizeof (traverse->path[0]);
  mr_tree_path_t * path = traverse->path;
  
  if (!rbtree->pool[path[traverse_size].idx].rb.red)
    for ( ; traverse_size > 0; --traverse_size)
      {
	unsigned parent = path[traverse_size - 1].idx;
	mr_child_idx_t child_idx = path[traverse_size - 1].child_idx;
	unsigned child = rbtree->pool[parent].next[child_idx].idx;
	unsigned brother = rbtree->pool[parent].next[child_idx ^ FLIP].idx;

	if (rbtree->pool[child].rb.red)
	  {
	    rbtree->pool[child].rb.red = false;
	    break;
	  }

	if (rbtree->pool[brother].rb.red)
	  {
	    rbtree->pool[brother].rb.red = false;
	    rbtree->pool[parent].rb.red = true;
	  
	    mr_tree_rotate (rbtree, &path[traverse_size - 1], child_idx ^ FLIP);
	      
	    path[traverse_size - 1].idx = brother;
	    path[traverse_size].child_idx = child_idx;
	    path[traverse_size++].idx = parent;
	    brother = rbtree->pool[parent].next[child_idx ^ FLIP].idx;
	  }
      
	if (rbtree->pool[rbtree->pool[brother].next[MR_LEFT].idx].rb.red ||
	    rbtree->pool[rbtree->pool[brother].next[MR_RIGHT].idx].rb.red)
	  {
	    unsigned nephew = rbtree->pool[brother].next[child_idx].idx;

	    if (rbtree->pool[nephew].rb.red)
	      {
		rbtree->pool[nephew].rb.red = false;
		rbtree->pool[brother].rb.red = true;

		path[traverse_size].idx = brother;
		path[traverse_size - 1].child_idx ^= FLIP;
		mr_tree_rotate (rbtree, &path[traverse_size], child_idx);

		brother = nephew;
	      }

	    rbtree->pool[brother].rb.red = rbtree->pool[parent].rb.red;
	    rbtree->pool[parent].rb.red = false;
	    rbtree->pool[rbtree->pool[brother].next[child_idx ^ FLIP].idx].rb.red = false;

	    mr_tree_rotate (rbtree, &path[traverse_size - 1], child_idx ^ FLIP);
	    break;
	  }

	rbtree->pool[brother].rb.red = true;
      }

  rbtree->pool->rb.red = false;
  rbtree->pool[rbtree->pool->root.idx].rb.red = false;
}

mr_status_t
mr_rbtree_del (mr_ptr_t key, mr_tree_t * tree, mr_compar_fn_t compar_fn, void * context)
{
  return (mr_tree_del (key, tree, compar_fn, context, mr_rbtree_rebalance_del));
}

static bool
mr_rbnode_is_valid (mr_tree_t * rbtree, unsigned idx, uint8_t * height)
{
  mr_child_idx_t child_idx;
  for (child_idx = MR_LEFT; child_idx <= MR_RIGHT; ++child_idx)
    {
      unsigned child = rbtree->pool[idx].next[child_idx].idx;
      if (rbtree->pool[idx].rb.red && rbtree->pool[child].rb.red)
	{
	  fprintf (stderr, "Two red nodes %u -> %u\n", idx, child);
	  return (false);
	}
    }
  
  if (height[rbtree->pool[idx].rb.left] != height[rbtree->pool[idx].rb.right])
    {
      fprintf (stderr, "Node [%u] has childen with a different height %d != %d\n",
	       idx, height[rbtree->pool[idx].rb.left], height[rbtree->pool[idx].rb.right]);
      return (false);
    }
  height[idx] = height[rbtree->pool[idx].rb.left] + !rbtree->pool[idx].rb.red;

  return (true);
}

bool
mr_rbtree_is_valid (mr_tree_t * rbtree, mr_compar_fn_t cmp, void * context)
{
  if (!mr_btree_is_valid (rbtree, cmp, context, mr_rbnode_is_valid))
    return (false);

  if (rbtree->size > 0)
    {
      if (rbtree->pool[NONE_IDX].rb.red)
	{
	  fprintf (stderr, "NONE node is not black\n");
	  return (false);
	}

      if (rbtree->pool[rbtree->pool->root.idx].rb.red)
	{
	  fprintf (stderr, "Root is not black\n");
	  return (false);
	}
    }

  return (true);
}

static inline void 
mr_avltree_rebalance_add (mr_tree_t * avltree, mr_tree_traverse_t * traverse)
{
  unsigned idx, traverse_size = traverse->size / sizeof (traverse->path[0]);
  
  avltree->pool[traverse->path[traverse_size].idx].avl.balanced = true;
  
  for (idx = traverse_size - 1; idx > 0; --idx)
    {
      unsigned node = traverse->path[idx].idx;
      mr_child_idx_t child_idx = traverse->path[idx].child_idx;
      if (avltree->pool[node].avl.balanced)
	{
	  avltree->pool[node].avl.balanced = false;
	  avltree->pool[node].avl.longer = child_idx;
	  continue;
	}

      if (avltree->pool[node].avl.longer != child_idx)
	{
	  avltree->pool[node].avl.balanced = true; /* took the shorter path */
	  break;
	}

      unsigned child = avltree->pool[node].next[child_idx].idx;
      
      /*
      // unreachable case
      if (avltree->pool[child].avl.balanced)
	{
	  avltree->pool[child].avl.balanced = false;
	  avltree->pool[child].avl.longer = child_idx ^ FLIP;
	  mr_tree_rotate (avltree, &traverse->path[idx], child_idx);
	  break;
	}
      */
      
      if (avltree->pool[node].avl.longer == avltree->pool[child].avl.longer)
	{
	  avltree->pool[node].avl.balanced = true;
	  avltree->pool[child].avl.balanced = true;
	  mr_tree_rotate (avltree, &traverse->path[idx], child_idx);
	  break;
	}

      unsigned grand_child = avltree->pool[child].next[child_idx ^ FLIP].idx;
      
      if (avltree->pool[grand_child].avl.balanced)
	{
	  avltree->pool[node].avl.balanced = true;
	  avltree->pool[child].avl.balanced = true;
	}
      else if (avltree->pool[child].avl.longer == avltree->pool[grand_child].avl.longer)
	{
	  avltree->pool[node].avl.balanced = true;
	  avltree->pool[child].avl.longer ^= FLIP;
	  avltree->pool[grand_child].avl.balanced = true;
	}
      else
	{
	  avltree->pool[node].avl.longer ^= FLIP;
	  avltree->pool[child].avl.balanced = true;
	  avltree->pool[grand_child].avl.balanced = true;
	}
	
      mr_tree_grand_rotate (avltree, &traverse->path[idx], child_idx);
      break;
    }
}

mr_ptr_t *
mr_avltree_add (mr_ptr_t key, mr_tree_t * tree, mr_compar_fn_t compar_fn, void * context)
{
  return (mr_tree_add (key, tree, compar_fn, context, mr_avltree_rebalance_add));
}

static void 
mr_avltree_rebalance_del (mr_tree_t * avltree, mr_tree_traverse_t * traverse)
{
  unsigned idx, traverse_size = traverse->size / sizeof (traverse->path[0]);
  
  for (idx = traverse_size - 1; idx > 0; --idx)
    {
      unsigned node = traverse->path[idx].idx;
      mr_child_idx_t child_idx = traverse->path[idx].child_idx;
      if (avltree->pool[node].avl.balanced)
	{
	  avltree->pool[node].avl.balanced = false;
	  avltree->pool[node].avl.longer = child_idx ^ FLIP;
	  break;
	}
      
      if (avltree->pool[node].avl.longer == child_idx)
	{
	  avltree->pool[node].avl.balanced = true;
	  continue;
	}
      
      unsigned child = avltree->pool[node].next[child_idx ^ FLIP].idx;
      
      if (avltree->pool[child].avl.balanced)
	{
	  avltree->pool[child].avl.balanced = false;
	  avltree->pool[child].avl.longer = child_idx;
	  mr_tree_rotate (avltree, &traverse->path[idx], child_idx ^ FLIP);
	  break;
	}
      
      if (avltree->pool[node].avl.longer == avltree->pool[child].avl.longer)
	{
	  avltree->pool[node].avl.balanced = true;
	  avltree->pool[child].avl.balanced = true;
	  mr_tree_rotate (avltree, &traverse->path[idx], child_idx ^ FLIP);
	  continue;
	}

      unsigned grand_child = avltree->pool[child].next[child_idx].idx;
      
      if (avltree->pool[grand_child].avl.balanced)
	{
	  avltree->pool[node].avl.balanced = true;
	  avltree->pool[child].avl.balanced = true;
	}
      else if (avltree->pool[child].avl.longer == avltree->pool[grand_child].avl.longer)
	{
	  avltree->pool[node].avl.balanced = true;
	  avltree->pool[child].avl.longer ^= FLIP;
	  avltree->pool[grand_child].avl.balanced = true;
	}
      else
	{
	  avltree->pool[node].avl.longer ^= FLIP;
	  avltree->pool[child].avl.balanced = true;
	  avltree->pool[grand_child].avl.balanced = true;
	}
	
      mr_tree_grand_rotate (avltree, &traverse->path[idx], child_idx ^ FLIP);
    }
}

mr_status_t
mr_avltree_del (mr_ptr_t key, mr_tree_t * tree, mr_compar_fn_t compar_fn, void * context)
{
  return (mr_tree_del (key, tree, compar_fn, context, mr_avltree_rebalance_del));
}

static bool
mr_avlnode_is_valid (mr_tree_t * avltree, unsigned idx, uint8_t * height)
{
  int left_height = height[avltree->pool[idx].avl.left];
  int right_height = height[avltree->pool[idx].avl.right];
  int delta;
  mr_child_idx_t longer;
  
  if (left_height > right_height)
    {
      height[idx] = left_height + 1;
      delta = left_height - right_height;
      longer = MR_LEFT;
    }
  else
    {
      height[idx] = right_height + 1;
      delta = right_height - left_height;
      longer = MR_RIGHT;
    }

  if (delta > 1)
    {
      fprintf (stderr, "Node [%u] height delta is mode then 1 (%d)\n", idx, delta);
      return (false);
    }
  
  if (0 == delta)
    {
      if (avltree->pool[idx].avl.balanced)
	return (true);
      
      fprintf (stderr, "Node [%u] height delta is zero, but node marked as unbalanced\n", idx);
      return (false);
    }

  if (avltree->pool[idx].avl.balanced)
    {
      fprintf (stderr, "Node [%u] marked as balanced, but it is not true\n", idx);
      return (false);
    }
  
  if (longer != avltree->pool[idx].avl.longer)
    {
      fprintf (stderr, "Node [%u] wrong longer path marked\n", idx);
      return (false);
    }
  return (true);
}

bool
mr_avltree_is_valid (mr_tree_t * avltree, mr_compar_fn_t cmp, void * context)
{
  return (mr_btree_is_valid (avltree, cmp, context, mr_avlnode_is_valid));
}
