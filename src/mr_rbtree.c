/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <metaresc.h>
#include <mr_rbtree.h>

#define NONE_IDX 0
#define FLIP 1

TYPEDEF_ENUM (mr_child_idx_t,
	      (MR_LEFT, = 0),
	      (MR_RIGHT, = 1),
	      )

static unsigned
mr_rbtree_node_new (mr_rbtree_t * rbtree)
{
  unsigned idx = rbtree->size / sizeof (rbtree->pool[0]);
  mr_rbtree_node_t * node = mr_rarray_allocate_element ((void**)&rbtree->pool, &rbtree->size, &rbtree->alloc_size, sizeof (rbtree->pool[0]));

  if (NULL == node)
    return (-1);

  memset (node, 0, sizeof (*node));
  return (idx);
}

static inline void
mr_rbtree_update_parent (mr_rbtree_t * rbtree, unsigned dst, unsigned src, unsigned parent)
{
  mr_child_idx_t src_idx = (src == rbtree->pool[parent].next[MR_RIGHT].idx) ? MR_RIGHT : MR_LEFT;
  rbtree->pool[parent].next[src_idx].idx = dst;
}

static inline void
mr_rbtree_rotate (mr_rbtree_t * rbtree, unsigned node, unsigned parent, mr_child_idx_t child_idx)
{
  unsigned child = rbtree->pool[node].next[child_idx].idx;
  
  rbtree->pool[node].next[child_idx].idx = rbtree->pool[child].next[child_idx ^ FLIP].idx;
  rbtree->pool[child].next[child_idx ^ FLIP].idx = node;

  mr_rbtree_update_parent (rbtree, child, node, parent);
}

static void 
mr_rbtree_rebalance_add (mr_rbtree_t * rbtree, unsigned node, unsigned * parents, unsigned parents_cnt)
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

static void
mr_rbtree_rebalance_del (mr_rbtree_t * rbtree, mr_child_idx_t child_idx, unsigned * parents, unsigned parents_cnt)
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

void
mr_rbtree_init (mr_rbtree_t * rbtree)
{
  memset (rbtree, 0, sizeof (*rbtree));
}

void
mr_rbtree_free (mr_rbtree_t * rbtree)
{
  if (rbtree->pool)
    MR_FREE (rbtree->pool);
  mr_rbtree_init (rbtree);
}

int
mr_rbtree_find (mr_ptr_t key, mr_rbtree_t * rbtree, mr_compar_fn_t compar_fn, void * context, unsigned * parents, unsigned * parents_cnt)
{
  unsigned idx;
  unsigned cnt = 0;
  int cmp = -1;

  parents[cnt++] = NONE_IDX;
  if (rbtree->size >= 2 * sizeof (rbtree->pool[0]))
    for (idx = rbtree->pool->root.idx; idx != NONE_IDX; idx = rbtree->pool[idx].next[(cmp > 0) ? MR_RIGHT : MR_LEFT].idx)
      {
	parents[cnt++] = idx;
	cmp = compar_fn (key, rbtree->pool[idx].key, context);
	if (0 == cmp)
	  break;
      }
  *parents_cnt = cnt;
  return (cmp);
}

mr_status_t
mr_rbtree_del (mr_ptr_t key, mr_rbtree_t * rbtree, mr_compar_fn_t compar_fn, void * context)
{
  unsigned parents[sizeof (rbtree->pool[0].next[0]) * __CHAR_BIT__ << 1];
  unsigned parents_cnt = 0;  
  int cmp = mr_rbtree_find (key, rbtree, compar_fn, context, parents, &parents_cnt);

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
      cmp = mr_rbtree_find (rbtree->pool[last].key, rbtree, compar_fn, context, parents, &parents_cnt);
      if (cmp != 0)
	return (MR_FAILURE);
      
      mr_rbtree_update_parent (rbtree, del, last, parents[parents_cnt - 2]);
      rbtree->pool[del] = rbtree->pool[last];
    }

  return (MR_SUCCESS);
}

mr_ptr_t *
mr_rbtree_add (mr_ptr_t key, mr_rbtree_t * rbtree, mr_compar_fn_t compar_fn, void * context)
{
  if (rbtree->size <= 0)
    if (mr_rbtree_node_new (rbtree) == -1)
      return (NULL);

  unsigned parents[sizeof (rbtree->pool[0].next[0]) * __CHAR_BIT__ << 1];
  unsigned parents_cnt = 0;  
  int cmp = mr_rbtree_find (key, rbtree, compar_fn, context, parents, &parents_cnt);
  unsigned parent = parents[parents_cnt - 1];
  
  if (0 == cmp)
    return (&rbtree->pool[parent].key);

  unsigned idx = mr_rbtree_node_new (rbtree);
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

static bool
mr_rbtree_is_valid_recurse (mr_rbtree_t * rbtree, unsigned idx, int b_height_accum, int b_height_expected, bool * visited, mr_compar_fn_t cmp, void * context)
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
mr_rbtree_is_valid (mr_rbtree_t * rbtree, mr_compar_fn_t cmp, void * context)
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
