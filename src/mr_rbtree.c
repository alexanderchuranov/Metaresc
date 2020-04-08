/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <metaresc.h>
#include <mr_rbtree.h>

#define NONE_IDX 0
#define FLIP 1

unsigned int
mr_avltree_find (mr_ptr_t key, mr_tree_t * tree, mr_compar_fn_t compar_fn, void * context, mr_tree_path_t * parents)
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

static inline void
mr_rbtree_update_parent (mr_tree_t * rbtree, unsigned dst, unsigned src, unsigned parent)
{
  mr_child_idx_t src_idx = (src == rbtree->pool[parent].next[MR_RIGHT].idx) ? MR_RIGHT : MR_LEFT;
  rbtree->pool[parent].next[src_idx].idx = dst;
}

static inline void
mr_rbtree_rotate (mr_tree_t * rbtree, unsigned node, unsigned parent, mr_child_idx_t child_idx)
{
  unsigned child = rbtree->pool[node].next[child_idx].idx;
  
  rbtree->pool[node].next[child_idx].idx = rbtree->pool[child].next[child_idx ^ FLIP].idx;
  rbtree->pool[child].next[child_idx ^ FLIP].idx = node;

  mr_rbtree_update_parent (rbtree, child, node, parent);
}

static void 
mr_rbtree_rebalance_add (mr_tree_t * rbtree, unsigned node, unsigned * parents, unsigned parents_cnt)
{
  while ((parents_cnt > 2) && rbtree->pool[parents[parents_cnt - 1]].red.bit)
    {
      unsigned parent = parents[--parents_cnt];
      unsigned grand_parent = parents[--parents_cnt];
      mr_child_idx_t parent_idx = (parent == rbtree->pool[grand_parent].next[MR_RIGHT].idx) ? MR_RIGHT : MR_LEFT;
      unsigned uncle = rbtree->pool[grand_parent].next[parent_idx ^ FLIP].idx;
      
      if (!rbtree->pool[uncle].red.bit)
	{
	  unsigned brother = rbtree->pool[parent].next[parent_idx ^ FLIP].idx;

	  if (node != brother)
	    rbtree->pool[parent].red.bit = false;
	  else
	    {
	      mr_rbtree_rotate (rbtree, parent, grand_parent, parent_idx ^ FLIP);
	      rbtree->pool[node].red.bit = false;
	    }

	  mr_rbtree_rotate (rbtree, grand_parent, parents[parents_cnt - 1], parent_idx);
	  rbtree->pool[grand_parent].red.bit = true;
	  break;
	}

      rbtree->pool[parent].red.bit = false;
      rbtree->pool[uncle].red.bit = false;
      rbtree->pool[grand_parent].red.bit = true;
      node = grand_parent;
    }
}

mr_ptr_t *
mr_rbtree_add (mr_ptr_t key, mr_tree_t * rbtree, mr_compar_fn_t compar_fn, void * context)
{
  if (rbtree->size <= 0)
    if (mr_tree_node_new (rbtree) == -1)
      return (NULL);

  unsigned parents[sizeof (rbtree->pool[0].next[0]) * __CHAR_BIT__ << 1];
  unsigned parents_cnt = 0;  
  int cmp = mr_tree_find (key, rbtree, compar_fn, context, parents, &parents_cnt);
  unsigned parent = parents[parents_cnt - 1];
  
  if (0 == cmp)
    return (&rbtree->pool[parent].key);

  unsigned idx = mr_tree_node_new (rbtree);
  if (-1 == idx)
    return (NULL);

  rbtree->pool[idx].key = key;
  rbtree->pool[idx].red.bit = true;
  rbtree->pool[parent].next[(cmp > 0) ? MR_RIGHT : MR_LEFT].idx = idx;
  if (rbtree->pool[parent].red.bit)
    mr_rbtree_rebalance_add (rbtree, idx, parents, parents_cnt);
  
  rbtree->pool[rbtree->pool->root.idx].red.bit = false;
  
  return (&rbtree->pool[idx].key);
}

static void
mr_rbtree_rebalance_del (mr_tree_t * rbtree, mr_child_idx_t child_idx, unsigned * parents, unsigned parents_cnt)
{
  for (;;)
    {
      unsigned parent = parents[parents_cnt - 1];
      unsigned child = rbtree->pool[parent].next[child_idx].idx;
      unsigned brother = rbtree->pool[parent].next[child_idx ^ FLIP].idx;

      if (rbtree->pool[child].red.bit)
	{
	  rbtree->pool[child].red.bit = false;
	  break;
	}

      if (rbtree->pool[brother].red.bit)
	{
	  rbtree->pool[brother].red.bit = false;
	  rbtree->pool[parent].red.bit = true;
	  
	  mr_rbtree_rotate (rbtree, parent, parents[parents_cnt - 2], child_idx ^ FLIP);
	      
	  parents[parents_cnt - 1] = brother;
	  parents[parents_cnt++] = parent;
	  brother = rbtree->pool[parent].next[child_idx ^ FLIP].idx;
	}
      
      if (rbtree->pool[rbtree->pool[brother].next[MR_LEFT].idx].red.bit ||
	  rbtree->pool[rbtree->pool[brother].next[MR_RIGHT].idx].red.bit)
	{
	  unsigned nephew = rbtree->pool[brother].next[child_idx].idx;

	  if (rbtree->pool[nephew].red.bit)
	    {
	      rbtree->pool[nephew].red.bit = false;
	      rbtree->pool[brother].red.bit = true;
	      
	      mr_rbtree_rotate (rbtree, brother, parent, child_idx);

	      brother = nephew;
	    }

	  rbtree->pool[brother].red.bit = rbtree->pool[parent].red.bit;
	  rbtree->pool[parent].red.bit = false;
	  rbtree->pool[rbtree->pool[brother].next[child_idx ^ FLIP].idx].red.bit = false;

	  mr_rbtree_rotate (rbtree, parent, parents[parents_cnt - 2], child_idx ^ FLIP);
	  break;
	}

      rbtree->pool[brother].red.bit = true;

      if (0 == --parents_cnt)
	break;
      child_idx = (parent == rbtree->pool[parents[parents_cnt - 1]].next[MR_RIGHT].idx) ? MR_RIGHT : MR_LEFT;
    }

  rbtree->pool->red.bit = false;
}

int
mr_tree_find (mr_ptr_t key, mr_tree_t * tree, mr_compar_fn_t compar_fn, void * context, unsigned * parents, unsigned * parents_cnt)
{
  unsigned idx;
  unsigned cnt = 0;
  int cmp = -1;

  parents[cnt++] = NONE_IDX;
  if (tree->size >= 2 * sizeof (tree->pool[0]))
    for (idx = tree->pool->root.idx; idx != NONE_IDX; idx = tree->pool[idx].next[(cmp > 0) ? MR_RIGHT : MR_LEFT].idx)
      {
	parents[cnt++] = idx;
	cmp = compar_fn (key, tree->pool[idx].key, context);
	if (0 == cmp)
	  break;
      }
  *parents_cnt = cnt;
  return (cmp);
}

mr_status_t
mr_rbtree_del (mr_ptr_t key, mr_tree_t * rbtree, mr_compar_fn_t compar_fn, void * context)
{
  unsigned parents[sizeof (rbtree->pool[0].next[0]) * __CHAR_BIT__ << 1];
  unsigned parents_cnt = 0;  
  int cmp = mr_tree_find (key, rbtree, compar_fn, context, parents, &parents_cnt);

  if (cmp != 0)
    return (MR_FAILURE);

  unsigned node = parents[parents_cnt - 1];
  unsigned del = node;
  if ((NONE_IDX == rbtree->pool[node].next[MR_LEFT].idx) ||
      (NONE_IDX == rbtree->pool[node].next[MR_RIGHT].idx))
    --parents_cnt;
  else
    for (del = rbtree->pool[node].next[MR_RIGHT].idx; rbtree->pool[del].next[MR_LEFT].idx != NONE_IDX; del = rbtree->pool[del].next[MR_LEFT].idx)
      parents[parents_cnt++] = del;

  unsigned child = rbtree->pool[del].next[(rbtree->pool[del].next[MR_RIGHT].idx != NONE_IDX) ? MR_RIGHT : MR_LEFT].idx;
  unsigned parent = parents[parents_cnt - 1];
  mr_child_idx_t del_idx = (del == rbtree->pool[parent].next[MR_RIGHT].idx) ? MR_RIGHT : MR_LEFT;

  mr_rbtree_update_parent (rbtree, child, del, parent);

  if (node != del)
    rbtree->pool[node].key = rbtree->pool[del].key;

  if ((parent != NONE_IDX) && !rbtree->pool[del].red.bit)
    mr_rbtree_rebalance_del (rbtree, del_idx, parents, parents_cnt);

  rbtree->pool[rbtree->pool->root.idx].red.bit = false;

  rbtree->size -= sizeof (rbtree->pool[0]);
  unsigned last = rbtree->size / sizeof (rbtree->pool[0]);
  if (del != last)
    {
      cmp = mr_tree_find (rbtree->pool[last].key, rbtree, compar_fn, context, parents, &parents_cnt);
      if (cmp != 0)
	return (MR_FAILURE);
      
      rbtree->pool[del] = rbtree->pool[last];
      mr_rbtree_update_parent (rbtree, del, last, parents[parents_cnt - 2]);
    }

  return (MR_SUCCESS);
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

  b_height_accum += !rbtree->pool[idx].red.bit;

  int child_idx;
  for (child_idx = MR_LEFT; child_idx <= MR_RIGHT; ++child_idx)
    {
      unsigned child = rbtree->pool[idx].next[child_idx].idx;
      if (rbtree->pool[idx].red.bit && (rbtree->pool[child].red.bit))
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

  if (rbtree->pool->root.idx >= count)
    {
      fprintf (stderr, "Root reference is out of range\n");
      return (false);
    }

  if (rbtree->pool[NONE_IDX].red.bit)
    {
      fprintf (stderr, "NONE node is not black\n");
      return (false);
    }

  if (rbtree->pool[rbtree->pool->root.idx].red.bit)
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
    height += !rbtree->pool[idx].red.bit;

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
mr_avltree_rotate_2 (mr_tree_t * avltree, mr_tree_path_t * path, unsigned idx)
{
  avltree->pool[path[idx].idx].next[path[idx].child_idx].idx =
    avltree->pool[path[idx + 1].idx].next[path[idx].child_idx ^ FLIP].idx;
  avltree->pool[path[idx + 1].idx].next[path[idx].child_idx ^ FLIP].idx = path[idx].idx;

  avltree->pool[path[idx - 1].idx].next[path[idx - 1].child_idx].idx = path[idx + 1].idx;
}

static void 
mr_avltree_rotate_3 (mr_tree_t * avltree, mr_tree_path_t * path, unsigned idx)
{
  avltree->pool[path[idx].idx].next[path[idx].child_idx].idx =
    avltree->pool[path[idx + 2].idx].next[path[idx].child_idx ^ FLIP].idx;
  avltree->pool[path[idx + 1].idx].next[path[idx].child_idx ^ FLIP].idx =
    avltree->pool[path[idx + 2].idx].next[path[idx].child_idx].idx;
  
  avltree->pool[path[idx + 2].idx].next[path[idx].child_idx ^ FLIP].idx = path[idx].idx;
  avltree->pool[path[idx + 2].idx].next[path[idx].child_idx].idx = path[idx + 1].idx;

  avltree->pool[path[idx - 1].idx].next[path[idx - 1].child_idx].idx = path[idx + 2].idx;
}

static void 
mr_avltree_rebalance_add (mr_tree_t * avltree, unsigned node, mr_tree_path_t * path, unsigned path_size)
{
  int idx;
  for (idx = path_size - 1; (idx > 0) && avltree->pool[path[idx].idx].balanced.bit; --idx)
    {
      avltree->pool[path[idx].idx].balanced.bit = false;
      avltree->pool[path[idx].idx].longer.child_idx = path[idx].child_idx;
    }

  if (NONE_IDX == idx)
    return;

  if (avltree->pool[path[idx].idx].longer.child_idx != path[idx].child_idx)
    {
      avltree->pool[path[idx].idx].balanced.bit = true; /* took the shorter path */
    }
  else if (avltree->pool[path[idx + 1].idx].balanced.bit)
    {
      avltree->pool[path[idx + 1].idx].balanced.bit = false;
      avltree->pool[path[idx + 1].idx].longer.child_idx = path[idx].child_idx ^ FLIP;
      mr_avltree_rotate_2 (avltree, path, idx);
    }
  else if (path[idx].child_idx == path[idx + 1].child_idx)
    {
      avltree->pool[path[idx].idx].balanced.bit = true;
      avltree->pool[path[idx + 1].idx].balanced.bit = true;
      mr_avltree_rotate_2 (avltree, path, idx);
    }
  else
    {
      if (avltree->pool[path[idx + 2].idx].balanced.bit)
	{
	  avltree->pool[path[idx].idx].balanced.bit = true;
	  avltree->pool[path[idx + 1].idx].balanced.bit = true;
	}
      else if (path[idx + 2].child_idx == path[idx + 1].child_idx)
	{
	  avltree->pool[path[idx].idx].balanced.bit = true;
	  avltree->pool[path[idx + 1].idx].longer.child_idx ^= FLIP;
	}
      else
	{
	  avltree->pool[path[idx].idx].longer.child_idx ^= FLIP;
	  avltree->pool[path[idx + 1].idx].balanced.bit = true;
	}
      avltree->pool[path[idx + 2].idx].balanced.bit = true;
	
      mr_avltree_rotate_3 (avltree, path, idx);
    }
}

mr_ptr_t *
mr_avltree_add (mr_ptr_t key, mr_tree_t * avltree, mr_compar_fn_t compar_fn, void * context)
{
  if (avltree->size <= 0)
    if (mr_tree_node_new (avltree) == -1)
      return (NULL);

  mr_tree_path_t path[sizeof (avltree->pool[0].next[0]) * __CHAR_BIT__ << 1];
  unsigned path_size = mr_avltree_find (key, avltree, compar_fn, context, path);
  
  if (path[path_size - 1].equal)
    return (&avltree->pool[path[path_size - 1].idx].key);

  unsigned idx = mr_tree_node_new (avltree);
  if (-1 == idx)
    return (NULL);
  
  path[path_size].idx = idx;
  avltree->pool[idx].key = key;
  avltree->pool[idx].balanced.bit = true;
  avltree->pool[path[path_size - 1].idx].next[path[path_size - 1].child_idx].idx = idx;

  mr_avltree_rebalance_add (avltree, idx, path, path_size);
  
  return (&avltree->pool[idx].key);
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
      if (avltree->pool[node].balanced.bit)
	return (true);
      
      fprintf (stderr, "Node [%u] height delta is zero, but node marked as unbalanced\n", node);
      return (false);
    }

  if (longer != avltree->pool[node].longer.child_idx)
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

  if (avltree->pool->root.idx >= count)
    {
      fprintf (stderr, "Root reference is out of range\n");
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
