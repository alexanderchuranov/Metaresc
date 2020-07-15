/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

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

mr_status_t
mr_tree_walk (mr_tree_t * tree, mr_visit_fn_t visit_fn, const void * context)
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
  if (traverse->size <= 1)
    return;
  
  if (traverse->equal || (traverse->path[traverse->size - 1].child_idx == next))
    {
      if (tree->pool[traverse->path[traverse->size - 1].idx].next[next].idx != NONE_IDX)
	{
	  unsigned idx = traverse->path[traverse->size - 1].idx;

	  traverse->path[traverse->size - 1].child_idx = next;
	  for (idx = tree->pool[idx].next[next].idx;
	       idx != NONE_IDX;
	       idx = tree->pool[idx].next[next ^ FLIP].idx)
	    traverse->path[traverse->size++] = (typeof (traverse->path[0])){
	      .idx = idx,
	      .child_idx = next ^ FLIP,
	    };
	}
      else
	while (--traverse->size > 1)
	  if (traverse->path[traverse->size - 1].child_idx != next)
	    break;
    }
  traverse->equal = true;
}

void
mr_tree_find (mr_ptr_t key, mr_tree_t * tree, mr_compar_fn_t compar_fn, void * context, mr_tree_traverse_t * traverse)
{
  unsigned idx;

  traverse->size = 0;
  traverse->equal = true;
  traverse->path[traverse->size++] = (typeof (traverse->path[0])){
    .idx = NONE_IDX,
    .child_idx = MR_LEFT,
  };
  
  if (tree->size >= 2 * sizeof (tree->pool[0]))
    for (idx = tree->pool->root.idx; idx != NONE_IDX; )
      {
	int cmp = compar_fn (key, tree->pool[idx].key, context);
	bool equal = (0 == cmp);
	mr_child_idx_t child_idx = (cmp > 0) ? MR_RIGHT : MR_LEFT;
	
	traverse->path[traverse->size++] = (typeof (traverse->path[0])){
	  .idx = idx,
	  .child_idx = child_idx,
	};
	
	if (equal)
	  return;
	idx = tree->pool[idx].next[child_idx].idx;
      }
  traverse->equal = false;
}

static inline mr_ptr_t *
mr_tree_add (mr_ptr_t key, mr_tree_t * tree, mr_compar_fn_t compar_fn, void * context, void (rebalance) (mr_tree_t *, mr_tree_traverse_t *))
{
  if (tree->size <= 0)
    if (mr_tree_node_new (tree) == -1)
      return (NULL);

  mr_tree_traverse_t traverse;
  mr_tree_find (key, tree, compar_fn, context, &traverse);
  if (traverse.equal)
    return (&tree->pool[traverse.path[traverse.size - 1].idx].key);

  unsigned idx = mr_tree_node_new (tree);
  if (-1 == idx)
    return (NULL);

  traverse.path[traverse.size].idx = idx;
  tree->pool[idx].key = key;
  mr_tree_path_t * parent = &traverse.path[traverse.size - 1];
  tree->pool[parent->idx].next[parent->child_idx].idx = idx;

  rebalance (tree, &traverse);
  
  return (&tree->pool[idx].key);
}

static inline mr_status_t
mr_tree_del (mr_ptr_t key, mr_tree_t * rbtree, mr_compar_fn_t compar_fn, void * context, void (rebalance) (mr_tree_t *, mr_tree_traverse_t *))
{
  mr_tree_traverse_t traverse;
  mr_tree_find (key, rbtree, compar_fn, context, &traverse);

  if (!traverse.equal)
    return (MR_FAILURE);

  unsigned node = traverse.path[traverse.size - 1].idx;
  unsigned del = node;
  if ((NONE_IDX == rbtree->pool[node].next[MR_LEFT].idx) || (NONE_IDX == rbtree->pool[node].next[MR_RIGHT].idx))
    --traverse.size;
  else
    {
      traverse.path[traverse.size - 1].child_idx = MR_RIGHT;
      for (del = rbtree->pool[node].next[MR_RIGHT].idx;
	   rbtree->pool[del].next[MR_LEFT].idx != NONE_IDX;
	   del = rbtree->pool[del].next[MR_LEFT].idx)
	traverse.path[traverse.size++] = (typeof (traverse.path[0])){ .child_idx = MR_LEFT, .idx = del, };
      traverse.path[traverse.size].idx = del;
    }

  mr_tree_path_t * parent = &traverse.path[traverse.size - 1];
  rbtree->pool[parent->idx].next[parent->child_idx].idx =
    rbtree->pool[del].next[(rbtree->pool[del].next[MR_RIGHT].idx != NONE_IDX) ? MR_RIGHT : MR_LEFT].idx;

  rbtree->pool[node].key = rbtree->pool[del].key;

  rebalance (rbtree, &traverse);

  rbtree->size -= sizeof (rbtree->pool[0]);
  unsigned last = rbtree->size / sizeof (rbtree->pool[0]);
  if (del != last)
    {
      mr_tree_find (rbtree->pool[last].key, rbtree, compar_fn, context, &traverse);
      if (!traverse.equal)
	return (MR_FAILURE);
      
      rbtree->pool[del] = rbtree->pool[last];
      parent = &traverse.path[traverse.size - 2];
      rbtree->pool[parent->idx].next[parent->child_idx].idx = del;
    }

  return (MR_SUCCESS);
}

TYPEDEF_FUNC (bool, node_validator_t, (mr_tree_t * /* tree */, unsigned /* idx */, uint8_t * /* height */))

static bool
mr_btree_is_valid_recurse (mr_tree_t * tree, unsigned idx, mr_compar_fn_t cmp, void * context, node_validator_t node_validator, uint8_t * height, int level)
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
      
  mr_child_idx_t child_idx;
  for (child_idx = MR_LEFT; child_idx <= MR_RIGHT; ++child_idx)
    {
      unsigned child = tree->pool[idx].next[child_idx].idx;
      if (NONE_IDX != child)
	{
	  int diff = cmp (tree->pool[child].key, tree->pool[idx].key, context);
	  if (0 == diff)
	    {
	      fprintf (stderr, "Tree unordered. Child (%u) == parent (%u)\n", child, idx);
	      return (false);
	    }	    
	  if ((diff > 0) ^ child_idx)
	    {
	      fprintf (stderr, "Tree unordered. Child (%u) <> parent (%u)\n", child, idx);
	      return (false);
	    }
	  if (!mr_btree_is_valid_recurse (tree, child, cmp, context, node_validator, height, level))
	    return (false);
	}
    }
  
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
  bool valid = mr_btree_is_valid_recurse (tree, tree->pool->root.idx, cmp, context, node_validator, height, 0);

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
  unsigned path_size = traverse->size;
  mr_tree_path_t * path = traverse->path;
  
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
		mr_tree_rotate (rbtree, &path[path_size + 1], parent_idx ^ FLIP);
		rbtree->pool[node].rb.red = false;
	      }

	    mr_tree_rotate (rbtree, &path[path_size], parent_idx);
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
  unsigned path_size = traverse->size;
  mr_tree_path_t * path = traverse->path;
  
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
	  
	    mr_tree_rotate (rbtree, &path[path_size - 1], child_idx ^ FLIP);
	      
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
		mr_tree_rotate (rbtree, &path[path_size], child_idx);

		brother = nephew;
	      }

	    rbtree->pool[brother].rb.red = rbtree->pool[parent].rb.red;
	    rbtree->pool[parent].rb.red = false;
	    rbtree->pool[rbtree->pool[brother].next[child_idx ^ FLIP].idx].rb.red = false;

	    mr_tree_rotate (rbtree, &path[path_size - 1], child_idx ^ FLIP);
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
  int idx;
  
  avltree->pool[traverse->path[traverse->size].idx].avl.balanced = true;
  
  for (idx = traverse->size - 1; idx > 0; --idx)
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
  int idx;
  
  for (idx = traverse->size - 1; idx > 0; --idx)
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
