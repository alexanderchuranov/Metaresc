/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <sys/types.h> /* ssize_t */

#include <metaresc.h>
#include <mr_rbtree.h>

#define NONE_IDX ((1U << (sizeof (((mr_rbtree_t*)NULL)->pool[0].next[0]) * __CHAR_BIT__ - 1)) - 1)

static unsigned
mr_rbtree_node_new (mr_rbtree_t * rbtree)
{
  unsigned idx = rbtree->size / sizeof (rbtree->pool[0]);
  ssize_t size = rbtree->size;
  ssize_t alloc_size = rbtree->alloc_size;
  mr_rbtree_node_t * node = mr_rarray_allocate_element ((void**)&rbtree->pool, &size, &alloc_size, sizeof (rbtree->pool[0]));
  rbtree->alloc_size = alloc_size;
  rbtree->size = size;

  if (NULL == node)
    return (NONE_IDX);

  node->next[0].idx = node->next[1].idx = NONE_IDX;
  node->next[0].bit = node->next[1].bit = true;
  return (idx);
}

static inline void
mr_rbtree_update_parent (mr_rbtree_t * rbtree, unsigned dst, unsigned src, unsigned parent)
{
  if (parent == NONE_IDX)
    rbtree->root = dst;
  else 
    rbtree->pool[parent].next[(rbtree->pool[parent].next[1].idx == src) ? 1 : 0].idx = dst;
}

static inline void
mr_rbtree_rotate (mr_rbtree_t * rbtree, unsigned node, unsigned parent, bool left)
{
  unsigned child = rbtree->pool[node].next[!left].idx;
  
  rbtree->pool[node].next[!left].idx = rbtree->pool[child].next[left].idx;
  rbtree->pool[child].next[left].idx = node;

  mr_rbtree_update_parent (rbtree, child, node, parent);
}

static void 
mr_rbtree_rebalance_add (mr_rbtree_t * rbtree, unsigned node, unsigned * parents, unsigned parents_cnt)
{
  while ((parents_cnt > 1) && rbtree->pool[parents[parents_cnt - 1]].next[0].bit)
    {
      unsigned int parent = parents[--parents_cnt];
      unsigned int grand_parent = parents[--parents_cnt];
      bool left = (parent == rbtree->pool[grand_parent].next[0].idx);
      unsigned uncle = rbtree->pool[grand_parent].next[left].idx;
      
      if ((uncle != NONE_IDX) && rbtree->pool[uncle].next[0].bit)
	{
	  rbtree->pool[parent].next[0].bit = false;
	  rbtree->pool[uncle].next[0].bit = false;
	  rbtree->pool[grand_parent].next[0].bit = true;
	  node = grand_parent;
	}
      else
	{
	  unsigned brother = rbtree->pool[parent].next[left].idx;

	  if (node != brother)
	    rbtree->pool[parent].next[0].bit = false;
	  else
	    {
	      mr_rbtree_rotate (rbtree, parent, grand_parent, !left);
	      rbtree->pool[node].next[0].bit = false;
	    }

	  unsigned grand_grand_parent = (parents_cnt > 0) ? parents[parents_cnt - 1] : NONE_IDX;
	  mr_rbtree_rotate (rbtree, grand_parent, grand_grand_parent, left);
	  rbtree->pool[grand_parent].next[0].bit = true;
	  break;
	}
    }

  rbtree->pool[rbtree->root].next[0].bit = false;
}

static void
mr_rbtree_rebalance_del (mr_rbtree_t * rbtree, bool left, unsigned * parents, unsigned parents_cnt)
{
  for (;;)
    {
      unsigned parent = (parents_cnt > 0) ? parents[parents_cnt - 1] : NONE_IDX;
      unsigned brother = rbtree->pool[parent].next[left].idx;
      unsigned child = rbtree->pool[parent].next[!left].idx;

      if ((child != NONE_IDX) && rbtree->pool[child].next[0].bit)
	{
	  rbtree->pool[child].next[0].bit = false;
	  break;
	}

      if (brother != NONE_IDX)
	{
	  if (rbtree->pool[brother].next[0].bit)
	    {
	      rbtree->pool[brother].next[0].bit = false;
	      rbtree->pool[parent].next[0].bit = true;
	      unsigned grand_parent = (parents_cnt > 1) ? parents[parents_cnt - 2] : NONE_IDX;
	      mr_rbtree_rotate (rbtree, parent, grand_parent, !left);
	      
	      parents[parents_cnt - 1] = brother;
	      parents[parents_cnt++] = parent;
	      brother = rbtree->pool[parent].next[left].idx;
	    }
      
	  if (brother != NONE_IDX)
	    {
	      if (((rbtree->pool[brother].next[0].idx == NONE_IDX) || !rbtree->pool[rbtree->pool[brother].next[0].idx].next[0].bit) &&
		  ((rbtree->pool[brother].next[1].idx == NONE_IDX) || !rbtree->pool[rbtree->pool[brother].next[1].idx].next[0].bit))
		rbtree->pool[brother].next[0].bit = true;
	      else
		{
		  unsigned nephew = rbtree->pool[brother].next[!left].idx;

		  if ((nephew != NONE_IDX) && rbtree->pool[nephew].next[0].bit)
		    {
		      rbtree->pool[nephew].next[0].bit = false;
		      rbtree->pool[brother].next[0].bit = true;
		      mr_rbtree_rotate (rbtree, brother, parent, left);

		      brother = nephew;
		    }

		  rbtree->pool[brother].next[0].bit = rbtree->pool[parent].next[0].bit;
		  rbtree->pool[parent].next[0].bit = false;
		  rbtree->pool[rbtree->pool[brother].next[left].idx].next[0].bit = false;

		  unsigned grand_parent = (parents_cnt > 1) ? parents[parents_cnt - 2] : NONE_IDX;
		  mr_rbtree_rotate (rbtree, parent, grand_parent, !left);
		  break;
		}
	    }
	}

      if (0 == --parents_cnt)
	break;
      left = (rbtree->pool[parents[parents_cnt - 1]].next[0].idx == parent);
    }
}

void
mr_rbtree_init (mr_rbtree_t * rbtree)
{
  memset (rbtree, 0, sizeof (*rbtree));
  rbtree->root = NONE_IDX;
}

void
mr_rbtree_free (mr_rbtree_t * rbtree)
{
  if (rbtree->pool)
    MR_FREE (rbtree->pool);
  memset (rbtree, 0, sizeof (*rbtree));
  rbtree->root = NONE_IDX;
}

int
mr_rbtree_find (mr_ptr_t key, mr_rbtree_t * rbtree, mr_compar_fn_t compar_fn, void * context, unsigned * parents, unsigned * parents_cnt)
{
  unsigned idx;
  unsigned cnt = 0;
  int cmp = 0;
  for (idx = rbtree->root; idx != NONE_IDX; )
    {
      parents[cnt++] = idx;
      cmp = compar_fn (key, rbtree->pool[idx].key, context);
      if (0 == cmp)
	break;

      if (cmp < 0)
	idx = rbtree->pool[idx].next[0].idx;
      else
	idx = rbtree->pool[idx].next[1].idx;
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

  if ((0 == parents_cnt) || (cmp != 0))
    return (MR_FAILURE);

  unsigned node = parents[parents_cnt - 1];
  unsigned del = node;
  if ((rbtree->pool[node].next[0].idx != NONE_IDX) && (rbtree->pool[node].next[1].idx != NONE_IDX))
    for (del = rbtree->pool[node].next[1].idx; rbtree->pool[del].next[0].idx != NONE_IDX; del = rbtree->pool[del].next[0].idx)
      parents[parents_cnt++] = del;
  else
    --parents_cnt;

  unsigned child = rbtree->pool[del].next[(rbtree->pool[del].next[1].idx != NONE_IDX) ? 1 : 0].idx;
  unsigned parent = (parents_cnt > 0) ? parents[parents_cnt - 1] : NONE_IDX;
  bool left = (parent != NONE_IDX) && (rbtree->pool[parent].next[0].idx == del);

  mr_rbtree_update_parent (rbtree, child, del, parent);

  if (node != del)
    rbtree->pool[node].key = rbtree->pool[del].key;

  if ((parent != NONE_IDX) && !rbtree->pool[del].next[0].bit)
    mr_rbtree_rebalance_del (rbtree, left, parents, parents_cnt);

  if (rbtree->root != NONE_IDX)
    rbtree->pool[rbtree->root].next[0].bit = false;

  rbtree->size -= sizeof (rbtree->pool[0]);
  unsigned last = rbtree->size / sizeof (rbtree->pool[0]);
  if (del != last)
    {
      cmp = mr_rbtree_find (rbtree->pool[last].key, rbtree, compar_fn, context, parents, &parents_cnt);
      if ((0 == parents_cnt) || (cmp != 0))
	return (MR_FAILURE);
      
      unsigned last_parent = (parents_cnt > 1) ? parents[parents_cnt - 2] : NONE_IDX;
      rbtree->pool[del] = rbtree->pool[last];
      mr_rbtree_update_parent (rbtree, del, last, last_parent);
    }

  return (MR_SUCCESS);
}

mr_ptr_t *
mr_rbtree_add (mr_ptr_t key, mr_rbtree_t * rbtree, mr_compar_fn_t compar_fn, void * context)
{
  if (rbtree->size <= 0)
    {
      rbtree->size = 0;
      rbtree->root = mr_rbtree_node_new (rbtree);
      if (NONE_IDX == rbtree->root)
	return (NULL);
      rbtree->pool[rbtree->root].key = key;
      rbtree->pool[rbtree->root].next[0].bit = false;
      return (&rbtree->pool[rbtree->root].key);
    }

  unsigned parents[sizeof (rbtree->pool[0].next[0]) * __CHAR_BIT__ << 1];
  unsigned parents_cnt = 0;  
  int cmp = mr_rbtree_find (key, rbtree, compar_fn, context, parents, &parents_cnt);

  if (0 == parents_cnt)
    return (NULL);

  unsigned parent = parents[parents_cnt - 1];
  if (0 == cmp)
    return (&rbtree->pool[parent].key);

  unsigned idx = mr_rbtree_node_new (rbtree);
  if (NONE_IDX == idx)
    return (NULL);
  
  rbtree->pool[idx].key = key;
  rbtree->pool[parent].next[(cmp > 0) ? 1 : 0].idx = idx;

  if (rbtree->pool[parent].next[0].idx)
    mr_rbtree_rebalance_add (rbtree, idx, parents, parents_cnt);

  return (&rbtree->pool[idx].key);
}

static bool
mr_rbtree_is_valid_recurse (mr_rbtree_t * rbtree, unsigned idx, int b_height_accum, int b_height_expected, bool * visited, mr_compar_fn_t cmp, void * context)
{
  if (NONE_IDX == idx)
    {
      if (b_height_accum != b_height_expected)
	fprintf (stderr, "Mismatched black height %d != %d\n", b_height_accum, b_height_expected);
      return (b_height_accum == b_height_expected);
    }

  visited[idx] = true;

  b_height_accum += !rbtree->pool[idx].next[0].bit;

  if (NONE_IDX != rbtree->pool[idx].next[0].idx)
    {
      unsigned child = rbtree->pool[idx].next[0].idx;
      if (rbtree->pool[idx].next[0].bit && (rbtree->pool[child].next[0].bit))
	{
	  fprintf (stderr, "Two red nodes %u -> %u\n", idx, child);
	  return (false);
	}
      if (cmp (rbtree->pool[child].key, rbtree->pool[idx].key, context) > 0)
	{
	  fprintf (stderr, "Left child (%u) > parent(%u)\n", child, idx);
	  return (false);
	}
    }

  if (NONE_IDX != rbtree->pool[idx].next[1].idx)
    {
      unsigned child = rbtree->pool[idx].next[1].idx;
      if (rbtree->pool[idx].next[0].bit && (rbtree->pool[child].next[0].bit))
	{
	  fprintf (stderr, "Two red nodes %u -> %u\n", idx, child);
	  return (false);
	}
      if (cmp (rbtree->pool[child].key, rbtree->pool[idx].key, context) < 0)
	{
	  fprintf (stderr, "Right child (%u) < parent(%u)\n", child, idx);
	  return (false);
	}
    }

  if (!mr_rbtree_is_valid_recurse (rbtree, rbtree->pool[idx].next[0].idx, b_height_accum, b_height_expected, visited, cmp, context))
    return (false);
  if (!mr_rbtree_is_valid_recurse (rbtree, rbtree->pool[idx].next[1].idx, b_height_accum, b_height_expected, visited, cmp, context))
    return (false);

  return (true);
}

bool
mr_rbtree_is_valid (mr_rbtree_t * rbtree, mr_compar_fn_t cmp, void * context)
{
  if (rbtree->size <= 0)
    return (true);

  if (NONE_IDX == rbtree->root)
    {
      fprintf (stderr, "Tree is not empty, but it has no root\n");
      return (false);
    }

  unsigned count = rbtree->size / sizeof (rbtree->pool[0]);

  if (rbtree->root >= count)
    {
      fprintf (stderr, "Root reference is out of range\n");
      return (false);
    }

  if (rbtree->pool[rbtree->root].next[0].bit)
    {
      fprintf (stderr, "Root is not black\n");
      return (false);
    }

  unsigned idx;
  for (idx = 0; idx < count; ++idx)
    {
      if ((rbtree->pool[idx].next[0].idx != NONE_IDX) && (rbtree->pool[idx].next[0].idx >= count))
	{
	  fprintf (stderr, "Node [%u] has a left child reference out of range %u > %u\n", idx, rbtree->pool[idx].next[0].idx, count);
	  return (false);
	}
      if ((rbtree->pool[idx].next[1].idx != NONE_IDX) && (rbtree->pool[idx].next[1].idx >= count))
	{
	  fprintf (stderr, "Node [%u] has a right child reference out of range %u > %u\n", idx, rbtree->pool[idx].next[1].idx, count);
	  return (false);
	}
    }

  int height = 0;
  for (idx = rbtree->root; NONE_IDX != idx; idx = rbtree->pool[idx].next[0].idx)
    height += !rbtree->pool[idx].next[0].bit;

  bool visited[count];
  memset (visited, 0, sizeof (visited));
  if (!mr_rbtree_is_valid_recurse (rbtree, rbtree->root, 0, height, visited, cmp, context))
    return (false);
  
  for (idx = 0; idx < count; ++idx)
    if (!visited[idx])
      {
	fprintf (stderr, "Node [%u] is not reachable\n", idx);
	return (false);
      }

  return (true);
}
