/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <metaresc.h>
#include <mr_btree.h>

#define NONE_IDX 0
#define FLIP 1

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

static unsigned
mr_tree_node_new (mr_tree_t * tree)
{
  typeof (tree->pool) node = mr_rarray_allocate_element
    ((void**)&tree->pool, &tree->size, &tree->alloc_size, sizeof (tree->pool[0]));

  if (NULL == node)
    return (-1);

  memset (node, 0, sizeof (*node));
  return (tree->size / sizeof (tree->pool[0]) - 1);
}

unsigned int
mr_tree_find (mr_ptr_t key, mr_tree_t * tree, mr_compar_fn_t compar_fn, void * context, mr_tree_path_t * parents)
{
  unsigned idx;
  unsigned cnt = 0;

  parents[cnt].idx = NONE_IDX;
  parents[cnt].equal = false;
  parents[cnt++].child_idx = MR_LEFT;
  if (tree->size >= 2 * sizeof (tree->pool[0]))
    for (idx = tree->pool->root.idx; idx != NONE_IDX; )
      {
	parents[cnt].idx = idx;
	int cmp = compar_fn (key, tree->pool[idx].key, context);
	bool equal = (0 == cmp);
	mr_child_idx_t child_idx = (cmp > 0) ? MR_RIGHT : MR_LEFT;
	
	parents[cnt].child_idx = child_idx;
	parents[cnt++].equal = equal;
	if (equal)
	  break;
	idx = tree->pool[idx].next[child_idx].idx;
      }
  return (cnt);
}

static mr_ptr_t *
mr_tree_add (mr_ptr_t key, mr_tree_t * tree, mr_compar_fn_t compar_fn, void * context, void (rebalance) (mr_tree_t *, mr_tree_path_t *, unsigned))
{
  if (tree->size <= 0)
    if (mr_tree_node_new (tree) == -1)
      return (NULL);

  mr_tree_path_t path[MR_PATH_SIZE];
  unsigned path_size = mr_tree_find (key, tree, compar_fn, context, path);
  
  if (path[path_size - 1].equal)
    return (&tree->pool[path[path_size - 1].idx].key);

  unsigned idx = mr_tree_node_new (tree);
  if (-1 == idx)
    return (NULL);

  path[path_size].idx = idx;
  tree->pool[idx].key = key;
  tree->pool[path[path_size - 1].idx].next[path[path_size - 1].child_idx].idx = idx;

  rebalance (tree, path, path_size);
  
  return (&tree->pool[idx].key);
}

static mr_status_t
mr_tree_del (mr_ptr_t key, mr_tree_t * rbtree, mr_compar_fn_t compar_fn, void * context, void (rebalance) (mr_tree_t *, mr_tree_path_t *, unsigned))
{
  mr_tree_path_t path[MR_PATH_SIZE];
  unsigned path_size = mr_tree_find (key, rbtree, compar_fn, context, path);

  if (!path[path_size - 1].equal)
    return (MR_FAILURE);

  unsigned node = path[path_size - 1].idx;
  unsigned del = node;
  if ((NONE_IDX == rbtree->pool[node].next[MR_LEFT].idx) ||
      (NONE_IDX == rbtree->pool[node].next[MR_RIGHT].idx))
    --path_size;
  else
    {
      path[path_size - 1].child_idx = MR_RIGHT;
      for (del = rbtree->pool[node].next[MR_RIGHT].idx;
	   rbtree->pool[del].next[MR_LEFT].idx != NONE_IDX;
	   del = rbtree->pool[del].next[MR_LEFT].idx)
	{
	  path[path_size].child_idx = MR_LEFT;
	  path[path_size++].idx = del;
	}
      path[path_size].idx = del;
    }

  rbtree->pool[path[path_size - 1].idx].next[path[path_size - 1].child_idx].idx =
    rbtree->pool[del].next[(rbtree->pool[del].next[MR_RIGHT].idx != NONE_IDX) ? MR_RIGHT : MR_LEFT].idx;

  rbtree->pool[node].key = rbtree->pool[del].key;

  rebalance (rbtree, path, path_size);

  rbtree->size -= sizeof (rbtree->pool[0]);
  unsigned last = rbtree->size / sizeof (rbtree->pool[0]);
  if (del != last)
    {
      path_size = mr_tree_find (rbtree->pool[last].key, rbtree, compar_fn, context, path);
      if (!path[path_size - 1].equal)
	return (MR_FAILURE);
      
      rbtree->pool[del] = rbtree->pool[last];
      rbtree->pool[path[path_size - 2].idx].next[path[path_size - 2].child_idx].idx = del;
    }

  return (MR_SUCCESS);
}

static inline void
mr_tree_rotate (mr_tree_t * tree, mr_tree_path_t * path, unsigned idx, mr_child_idx_t child_idx)
{
  unsigned node = path[idx].idx;
  unsigned child = tree->pool[node].next[child_idx].idx;
  
  tree->pool[node].next[child_idx].idx = tree->pool[child].next[child_idx ^ FLIP].idx;
  tree->pool[child].next[child_idx ^ FLIP].idx = node;

  tree->pool[path[idx - 1].idx].next[path[idx - 1].child_idx].idx = child;
}

static inline void 
mr_tree_grand_rotate (mr_tree_t * tree, mr_tree_path_t * path, unsigned idx, mr_child_idx_t child_idx)
{
  unsigned node = path[idx].idx;
  unsigned child = tree->pool[node].next[child_idx].idx;
  unsigned grand_child = tree->pool[child].next[child_idx ^ FLIP].idx;
  
  tree->pool[node].next[child_idx].idx = tree->pool[grand_child].next[child_idx ^ FLIP].idx;
  tree->pool[child].next[child_idx ^ FLIP].idx = tree->pool[grand_child].next[child_idx].idx;
  
  tree->pool[grand_child].next[child_idx ^ FLIP].idx = node;
  tree->pool[grand_child].next[child_idx].idx = child;

  tree->pool[path[idx - 1].idx].next[path[idx - 1].child_idx].idx = grand_child;
}

static void 
mr_rbtree_rebalance_add (mr_tree_t * rbtree, mr_tree_path_t * path, unsigned path_size)
{
  rbtree->pool[path[path_size].idx].rb.red = true;
  
  if (rbtree->pool[path[path_size - 1].idx].rb.red)
    while ((path_size > 2) && rbtree->pool[path[path_size - 1].idx].rb.red)
      {
	unsigned node = path[path_size].idx;
	unsigned parent = path[--path_size].idx;
	unsigned grand_parent = path[--path_size].idx;
	mr_child_idx_t parent_idx = path[path_size].child_idx;
	unsigned uncle = rbtree->pool[grand_parent].next[parent_idx ^ FLIP].idx;
      
	if (!rbtree->pool[uncle].rb.red)
	  {
	    unsigned brother = rbtree->pool[parent].next[parent_idx ^ FLIP].idx;

	    if (node != brother)
	      rbtree->pool[parent].rb.red = false;
	    else
	      {
		mr_tree_rotate (rbtree, path, path_size + 1, parent_idx ^ FLIP);
		rbtree->pool[node].rb.red = false;
	      }

	    mr_tree_rotate (rbtree, path, path_size, parent_idx);
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
mr_rbtree_rebalance_del (mr_tree_t * rbtree, mr_tree_path_t * path, unsigned path_size)
{
  if (!rbtree->pool[path[path_size].idx].rb.red)
    for ( ; path_size > 0; --path_size)
      {
	unsigned parent = path[path_size - 1].idx;
	mr_child_idx_t child_idx = path[path_size - 1].child_idx;
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
	  
	    mr_tree_rotate (rbtree, path, path_size - 1, child_idx ^ FLIP);
	      
	    path[path_size - 1].idx = brother;
	    path[path_size].child_idx = child_idx;
	    path[path_size++].idx = parent;
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

		path[path_size].idx = brother;
		path[path_size - 1].child_idx ^= FLIP;
		mr_tree_rotate (rbtree, path, path_size, child_idx);

		brother = nephew;
	      }

	    rbtree->pool[brother].rb.red = rbtree->pool[parent].rb.red;
	    rbtree->pool[parent].rb.red = false;
	    rbtree->pool[rbtree->pool[brother].next[child_idx ^ FLIP].idx].rb.red = false;

	    mr_tree_rotate (rbtree, path, path_size - 1, child_idx ^ FLIP);
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
mr_rbtree_is_valid_recurse (mr_tree_t * rbtree, unsigned idx, int b_height_accum, int b_height_expected, bool * visited, mr_compar_fn_t cmp, void * context)
{
  visited[idx] = true;

  if (NONE_IDX == idx)
    {
      if (b_height_accum != b_height_expected)
	fprintf (stderr, "Mismatched black height %d != %d\n", b_height_accum, b_height_expected);
      return (b_height_accum == b_height_expected);
    }

  b_height_accum += !rbtree->pool[idx].rb.red;

  int child_idx;
  for (child_idx = MR_LEFT; child_idx <= MR_RIGHT; ++child_idx)
    {
      unsigned child = rbtree->pool[idx].next[child_idx].idx;
      if (rbtree->pool[idx].rb.red && rbtree->pool[child].rb.red)
	{
	  fprintf (stderr, "Two red nodes %u -> %u\n", idx, child);
	  return (false);
	}
      if ((NONE_IDX != child) &&
	  ((cmp (rbtree->pool[child].key, rbtree->pool[idx].key, context) > 0) ^ child_idx))
	{
	  fprintf (stderr, "Left child (%u) > parent(%u)\n", child, idx);
	  return (false);
	}
      if (!mr_rbtree_is_valid_recurse (rbtree, child, b_height_accum, b_height_expected, visited, cmp, context))
	return (false);
    }

  return (true);
}

bool
mr_rbtree_is_valid (mr_tree_t * rbtree, mr_compar_fn_t cmp, void * context)
{
  if (rbtree->size <= 0)
    return (true);

  unsigned count = rbtree->size / sizeof (rbtree->pool[0]);
  if (0 == count)
    {
      fprintf (stderr, "Tree is not empty, but it has no root\n");
      return (false);
    }

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

  unsigned idx, child_idx;
  for (idx = 0; idx < count; ++idx)
    for (child_idx = MR_LEFT; child_idx <= MR_RIGHT; ++child_idx)
      if (rbtree->pool[idx].next[child_idx].idx >= count)
	{
	  fprintf (stderr, "Node [%u] has a child reference out of range %u > %u\n", idx, rbtree->pool[idx].next[child_idx].idx, count);
	  return (false);
	}

  int height = 0;
  for (idx = rbtree->pool->root.idx; NONE_IDX != idx; idx = rbtree->pool[idx].next[MR_LEFT].idx)
    height += !rbtree->pool[idx].rb.red;

  bool visited[count];
  memset (visited, 0, sizeof (visited));
  if (!mr_rbtree_is_valid_recurse (rbtree, rbtree->pool->root.idx, 0, height, visited, cmp, context))
    return (false);
  
  for (idx = 0; idx < count; ++idx)
    if (!visited[idx])
      {
	fprintf (stderr, "Node [%u] is not reachable\n", idx);
	return (false);
      }

  return (true);
}

static void 
mr_avltree_rebalance_add (mr_tree_t * avltree, mr_tree_path_t * path, unsigned path_size)
{
  int idx;
  
  avltree->pool[path[path_size].idx].avl.balanced = true;
  
  for (idx = path_size - 1; idx > 0; --idx)
    {
      unsigned node = path[idx].idx;
      mr_child_idx_t child_idx = path[idx].child_idx;
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
      if (NONE_IDX == child)
	break;
      
      if (avltree->pool[child].avl.balanced)
	{
	  /* unreachable */
	  avltree->pool[child].avl.balanced = false;
	  avltree->pool[child].avl.longer = child_idx ^ FLIP;
	  mr_tree_rotate (avltree, path, idx, child_idx);
	  break;
	}
      
      if (avltree->pool[node].avl.longer == avltree->pool[child].avl.longer)
	{
	  avltree->pool[node].avl.balanced = true;
	  avltree->pool[child].avl.balanced = true;
	  mr_tree_rotate (avltree, path, idx, child_idx);
	  break;
	}

      unsigned grand_child = avltree->pool[child].next[child_idx ^ FLIP].idx;
      if (NONE_IDX == grand_child)
	break;
      
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
	
      mr_tree_grand_rotate (avltree, path, idx, child_idx);
      break;
    }
}

mr_ptr_t *
mr_avltree_add (mr_ptr_t key, mr_tree_t * tree, mr_compar_fn_t compar_fn, void * context)
{
  return (mr_tree_add (key, tree, compar_fn, context, mr_avltree_rebalance_add));
}

static void 
mr_avltree_rebalance_del (mr_tree_t * avltree, mr_tree_path_t * path, unsigned path_size)
{
  int idx;
  for (idx = path_size - 1; idx > 0; --idx)
    {
      unsigned node = path[idx].idx;
      mr_child_idx_t child_idx = path[idx].child_idx;
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
      if (NONE_IDX == child)
	break;
      
      if (avltree->pool[child].avl.balanced)
	{
	  avltree->pool[child].avl.balanced = false;
	  avltree->pool[child].avl.longer = child_idx;
	  mr_tree_rotate (avltree, path, idx, child_idx ^ FLIP);
	  break;
	}
      
      if (avltree->pool[node].avl.longer == avltree->pool[child].avl.longer)
	{
	  avltree->pool[node].avl.balanced = true;
	  avltree->pool[child].avl.balanced = true;
	  mr_tree_rotate (avltree, path, idx, child_idx ^ FLIP);
	  continue;
	}

      unsigned grand_child = avltree->pool[child].next[child_idx].idx;
      if (NONE_IDX == grand_child)
	break;
      
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
	
      mr_tree_grand_rotate (avltree, path, idx, child_idx ^ FLIP);
    }
}

mr_status_t
mr_avltree_del (mr_ptr_t key, mr_tree_t * tree, mr_compar_fn_t compar_fn, void * context)
{
  return (mr_tree_del (key, tree, compar_fn, context, mr_avltree_rebalance_del));
}

static bool
mr_avltree_is_valid_recurse (mr_tree_t * avltree, unsigned node, int * height, mr_compar_fn_t cmp, void * context)
{
  if (node == NONE_IDX)
    return (true);
  
  if (!mr_avltree_is_valid_recurse (avltree, avltree->pool[node].next[MR_LEFT].idx, height, cmp, context))
    return (false);
  if (!mr_avltree_is_valid_recurse (avltree, avltree->pool[node].next[MR_RIGHT].idx, height, cmp, context))
    return (false);

  int left_height = height[avltree->pool[node].next[MR_LEFT].idx];
  int right_height = height[avltree->pool[node].next[MR_RIGHT].idx];
  int delta;
  mr_child_idx_t longer;
  
  if (left_height > right_height)
    {
      height[node] = left_height + 1;
      delta = left_height - right_height;
      longer = MR_LEFT;
    }
  else
    {
      height[node] = right_height + 1;
      delta = right_height - left_height;
      longer = MR_RIGHT;
    }

  if (delta > 1)
    {
      fprintf (stderr, "Node [%u] height delta is mode then 1 (%d)\n", node, delta);
      return (false);
    }
  
  if (0 == delta)
    {
      if (avltree->pool[node].avl.balanced)
	return (true);
      
      fprintf (stderr, "Node [%u] height delta is zero, but node marked as unbalanced\n", node);
      return (false);
    }

  if (longer != avltree->pool[node].avl.longer)
    {
      fprintf (stderr, "Node [%u] wrong longer path marked\n", node);
      return (false);
    }
  return (true);
}

bool
mr_avltree_is_valid (mr_tree_t * avltree, mr_compar_fn_t cmp, void * context)
{
  if (avltree->size <= 0)
    return (true);

  unsigned count = avltree->size / sizeof (avltree->pool[0]);
  if (0 == count)
    {
      fprintf (stderr, "Tree is not empty, but it has no root\n");
      return (false);
    }

  unsigned idx, child_idx;
  for (idx = 0; idx < count; ++idx)
    for (child_idx = MR_LEFT; child_idx <= MR_RIGHT; ++child_idx)
      if (avltree->pool[idx].next[child_idx].idx >= count)
	{
	  fprintf (stderr, "Node [%u] has a child reference out of range %u > %u\n", idx, avltree->pool[idx].next[child_idx].idx, count);
	  return (false);
	}

  int height[count];
  memset (height, 0, sizeof (height));
  if (!mr_avltree_is_valid_recurse (avltree, avltree->pool->root.idx, height, cmp, context))
    return (false);
  
  for (idx = 1; idx < count; ++idx)
    if (height[idx] == 0)
      {
	fprintf (stderr, "Node [%u] is not reachable\n", idx);
	return (false);
      }

  return (true);
}
