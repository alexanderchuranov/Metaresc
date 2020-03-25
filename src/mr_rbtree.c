/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <sys/types.h> /* ssize_t */

#include <metaresc.h>
#include <mr_rbtree.h>

static void
print_trace (mr_rbtree_t * rbtree, unsigned idx)
{
  int b_height = 0, height = 0;
  unsigned node = idx;
  for (node = idx; node != -1; node = rbtree->pool[node].parent)
    {
      b_height += !rbtree->pool[node].red;
      ++height;
    }
  printf ("path = %d:%d : ", b_height, height);

  unsigned path[height];
  int i = 0;
  for (node = idx; node != -1; node = rbtree->pool[node].parent)
    path[i++] = node;
  for (--i; i >= 0; --i)
    printf ("[%02u:%d] -> ", path[i], rbtree->pool[path[i]].red);
  printf ("[-1]\n");
}

static void
check_tree_rec (mr_rbtree_t * rbtree, unsigned idx)
{
  if (-1 == idx)
    return;

  if (rbtree->pool[idx].red && 
      (rbtree->pool[idx].parent != -1) &&
      (rbtree->pool[rbtree->pool[idx].parent].red))
    printf ("Two red nodes %u -> %u\n", rbtree->pool[idx].parent, idx);

  int i;
  for (i = 0; i < sizeof (rbtree->pool[0].next) / sizeof (rbtree->pool[0].next[0]); ++i)
    {
      if ((-1 != rbtree->pool[idx].next[i]) && (rbtree->pool[rbtree->pool[idx].next[i]].parent != idx))
	printf ("child<->parent mismatch %u -> %u (%u)\n", idx, rbtree->pool[idx].next[i], rbtree->pool[rbtree->pool[idx].next[i]].parent);
      check_tree_rec (rbtree, rbtree->pool[idx].next[i]);
      if (-1 == rbtree->pool[idx].next[i])
	print_trace (rbtree, idx);
    }
}

static void __attribute__ ((unused))
check_tree (mr_rbtree_t * rbtree)
{
  printf ("Tree check start\n");
  check_tree_rec (rbtree, rbtree->root);
  printf ("Tree check finish\n");
}

TYPEDEF_STRUCT (typed_rbtree_t,
		(mr_rbtree_t *, rbtree),
		string_t key_type
		)

static void __attribute__ ((unused))
dump_rbtree (mr_rbtree_t * rbtree)
{
  typed_rbtree_t typed_rbtree = {
    .rbtree = rbtree,
    .key_type = "intptr_t",
  };
  char * dump = MR_SAVE_CINIT (typed_rbtree_t, &typed_rbtree);
  if (dump)
    {
      printf ("%s", dump);
      MR_FREE (dump);
    }
}

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
    return (-1);

  node->parent = node->next[0] = node->next[1] = -1;
  node->red = true;
  return (idx);
}

static void
mr_rbtree_rotate (mr_rbtree_t * rbtree, unsigned node, bool left)
{
  unsigned parent = rbtree->pool[node].parent;
  unsigned child = rbtree->pool[node].next[!left];
  unsigned grand_child = rbtree->pool[child].next[left];
  
  rbtree->pool[node].next[!left] = grand_child;
  rbtree->pool[child].next[left] = node;
  if (-1 != grand_child)
    rbtree->pool[grand_child].parent = node;

  rbtree->pool[child].parent = parent;
  if (-1 == parent)
    rbtree->root = child;
  else
    rbtree->pool[parent].next[(rbtree->pool[parent].next[1] == node)] = child;
  rbtree->pool[node].parent = child;
}

static void 
mr_rbtree_rebalance_add (mr_rbtree_t * rbtree, unsigned node)
{
  while ((rbtree->pool[node].parent != -1) && rbtree->pool[rbtree->pool[node].parent].red 
	 && (rbtree->pool[node].parent != rbtree->root))
    {
      unsigned parent = rbtree->pool[node].parent;
      unsigned grand_parent = rbtree->pool[parent].parent;  
      bool left = (parent == rbtree->pool[grand_parent].next[0]);
      unsigned uncle = rbtree->pool[grand_parent].next[left];

      if ((uncle != -1) && rbtree->pool[uncle].red)
	{
	  rbtree->pool[parent].red = false;
	  rbtree->pool[uncle].red = false;
	  rbtree->pool[grand_parent].red = true;
	  node = grand_parent;
	}
      else
	{
	  unsigned brother = rbtree->pool[parent].next[left];

	  if (node != brother)
	    rbtree->pool[parent].red = false;
	  else
	    {
	      mr_rbtree_rotate (rbtree, parent, !left);
	      rbtree->pool[node].red = false;
	    }

	  mr_rbtree_rotate (rbtree, grand_parent, left);
	  rbtree->pool[grand_parent].red = true;
	  break;
	}
    }

  rbtree->pool[rbtree->root].red = false;
}

static void
mr_rbtree_update_parent (mr_rbtree_t * rbtree, int dst, int src)
{
  unsigned parent = rbtree->pool[src].parent;

  if (parent == -1)
    rbtree->root = dst;
  else 
    rbtree->pool[parent].next[(rbtree->pool[parent].next[1] == src)] = dst;

  if (dst != -1)
    rbtree->pool[dst].parent = parent;
}

static void
mr_rbtree_update_children (mr_rbtree_t * rbtree, int dst, int src)
{
  int i;
  for (i = 0; i < sizeof (rbtree->pool[0].next) / sizeof (rbtree->pool[0].next[0]); ++i)
    {
      unsigned next = rbtree->pool[src].next[i];
      rbtree->pool[dst].next[i] = next;
      if (next != -1)
	rbtree->pool[next].parent = dst;
    }
}

static void
mr_rbtree_rebalance_del (mr_rbtree_t * rbtree, unsigned parent, bool left)
{
  for (;;)
    {
      unsigned brother = rbtree->pool[parent].next[left];
      unsigned child = rbtree->pool[parent].next[!left];

      if ((child != -1) && rbtree->pool[child].red)
	{
	  rbtree->pool[child].red = false;
	  break;
	}

      if (brother != -1)
	{
	  if (rbtree->pool[brother].red)
	    {
	      rbtree->pool[brother].red = false;
	      rbtree->pool[parent].red = true;
	      mr_rbtree_rotate (rbtree, parent, !left);
	      brother = rbtree->pool[parent].next[left];
	    }
      
	  if (brother != -1)
	    {
	      if (((rbtree->pool[brother].next[0] == -1) || !rbtree->pool[rbtree->pool[brother].next[0]].red) &&
		  ((rbtree->pool[brother].next[1] == -1) || !rbtree->pool[rbtree->pool[brother].next[1]].red))
		rbtree->pool[brother].red = true;
	      else
		{
		  unsigned nephew = rbtree->pool[brother].next[!left];

		  if ((nephew != -1) && rbtree->pool[nephew].red)
		    {
		      rbtree->pool[nephew].red = false;
		      rbtree->pool[brother].red = true;
		      mr_rbtree_rotate (rbtree, brother, left);
		      brother = nephew;
		    }

		  rbtree->pool[brother].red = rbtree->pool[parent].red;
		  rbtree->pool[parent].red = false;
		  rbtree->pool[rbtree->pool[brother].next[left]].red = false;
		  mr_rbtree_rotate (rbtree, parent, !left);
		  break;
		}
	    }
	}
      unsigned next_parent = rbtree->pool[parent].parent;
      if (next_parent == -1)
	break;
      left = (rbtree->pool[next_parent].next[0] == parent);
      parent = next_parent;
    }
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
  memset (rbtree, 0, sizeof (*rbtree));
}

unsigned
mr_rbtree_find (mr_ptr_t key, mr_rbtree_t * rbtree, mr_compar_fn_t compar_fn, void * context, int * parent_cmp)
{
  unsigned idx, parent = -1;
  int cmp = 0;
  for (idx = rbtree->root; idx != -1; idx = rbtree->pool[idx].next[(cmp > 0)])
    {
      parent = idx;
      cmp = compar_fn (key, rbtree->pool[idx].key, context);
      if (0 == cmp)
	break;
    }
  *parent_cmp = cmp;
  return (parent);
}

mr_status_t
mr_rbtree_del (mr_ptr_t key, mr_rbtree_t * rbtree, mr_compar_fn_t compar_fn, void * context)
{
  int cmp;
  unsigned node = mr_rbtree_find (key, rbtree, compar_fn, context, &cmp);

  if ((-1 == node) || (cmp != 0))
    return (MR_FAILURE);

  unsigned del = node;
  if ((rbtree->pool[node].next[0] != -1) && (rbtree->pool[node].next[1] != -1))
    for (del = rbtree->pool[node].next[1]; rbtree->pool[del].next[0] != -1; del = rbtree->pool[del].next[0]);

  unsigned child = rbtree->pool[del].next[(rbtree->pool[del].next[0] == -1)];
  unsigned parent = rbtree->pool[del].parent;
  bool left = (parent != -1) && (rbtree->pool[parent].next[0] == del);

  mr_rbtree_update_parent (rbtree, child, del);

  if (node != del)
    rbtree->pool[node].key = rbtree->pool[del].key;

  if ((parent != -1) && !rbtree->pool[del].red)
    mr_rbtree_rebalance_del (rbtree, parent, left);

  if (rbtree->root != -1)
    rbtree->pool[rbtree->root].red = false;

  unsigned last = rbtree->size / sizeof (rbtree->pool[0]) - 1;
  if (del != last)
    {
      rbtree->pool[del].key = rbtree->pool[last].key;
      rbtree->pool[del].red = rbtree->pool[last].red;
      mr_rbtree_update_parent (rbtree, del, last);
      mr_rbtree_update_children (rbtree, del, last);
    }
  rbtree->size -= sizeof (rbtree->pool[0]);

  return (MR_SUCCESS);
}

mr_ptr_t *
mr_rbtree_add (mr_ptr_t key, mr_rbtree_t * rbtree, mr_compar_fn_t compar_fn, void * context)
{
  if (rbtree->size <= 0)
    {
      rbtree->size = 0;
      rbtree->root = mr_rbtree_node_new (rbtree);
      if (-1 == rbtree->root)
	return (NULL);
      rbtree->pool[rbtree->root].key = key;
      rbtree->pool[rbtree->root].red = false;
      return (&rbtree->pool[rbtree->root].key);
    }

  int cmp = 0;
  unsigned parent = mr_rbtree_find (key, rbtree, compar_fn, context, &cmp);

  if (-1 == parent)
    return (NULL);

  if (0 == cmp)
    return (&rbtree->pool[parent].key);

  unsigned idx = mr_rbtree_node_new (rbtree);
  if (-1 == idx)
    return (NULL);
  
  rbtree->pool[idx].parent = parent;
  rbtree->pool[idx].key = key;

  rbtree->pool[parent].next[(cmp > 0)] = idx;

  if (rbtree->pool[parent].red)
    mr_rbtree_rebalance_add (rbtree, idx);

  return (&rbtree->pool[idx].key);
}

static bool
mr_rbtree_is_valid_recurse (mr_rbtree_t * rbtree, unsigned idx, int b_height_accum, int b_height_expected, bool * visited, mr_compar_fn_t cmp, void * context)
{
  if (-1 == idx)
    {
      if (b_height_accum != b_height_expected)
	fprintf (stderr, "Mismatched black height %d != %d\n", b_height_accum, b_height_expected);
      return (b_height_accum == b_height_expected);
    }

  visited[idx] = true;

  if (rbtree->pool[idx].red && 
      (rbtree->pool[idx].parent != -1) &&
      (rbtree->pool[rbtree->pool[idx].parent].red))
    {
      fprintf (stderr, "Two red nodes %u -> %u\n", rbtree->pool[idx].parent, idx);
      return (false);
    }

  b_height_accum += !rbtree->pool[idx].red;

  int i;
  for (i = 0; i < sizeof (rbtree->pool[0].next) / sizeof (rbtree->pool[0].next[0]); ++i)
    {
      if ((-1 != rbtree->pool[idx].next[i]) && (rbtree->pool[rbtree->pool[idx].next[i]].parent != idx))
	{
	  fprintf (stderr, "child<->parent mismatch %u -> %u (%u)\n", idx, rbtree->pool[idx].next[i], rbtree->pool[rbtree->pool[idx].next[i]].parent);
	  return (false);
	}

      if ((-1 != rbtree->pool[idx].next[i]) && ((cmp (rbtree->pool[rbtree->pool[idx].next[i]].key, rbtree->pool[idx].key, context) > 0) ^ i))
	{
	  fprintf (stderr, "child (%u) > parent(%u)\n", rbtree->pool[idx].next[i], idx);
	  return (false);
	}
      
      if (!mr_rbtree_is_valid_recurse (rbtree, rbtree->pool[idx].next[i], b_height_accum, b_height_expected, visited, cmp, context))
	return (false);
    }

  return (true);
}

bool
mr_rbtree_is_valid (mr_rbtree_t * rbtree, mr_compar_fn_t cmp, void * context)
{
  if (rbtree->size <= 0)
    return (true);

  if (-1 == rbtree->root)
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

  if (-1 != rbtree->pool[rbtree->root].parent)
    {
      fprintf (stderr, "Root element has an incorrect parent reference\n");
      return (false);
    }

  if (rbtree->pool[rbtree->root].red)
    {
      fprintf (stderr, "Root is not black\n");
      return (false);
    }

  unsigned idx;
  for (idx = 0; idx < count; ++idx)
    {
      if ((rbtree->pool[idx].parent != -1) && (rbtree->pool[idx].parent >= count))
	{
	  fprintf (stderr, "Node [%u] has a parent reference out of range %u > %u\n", idx, rbtree->pool[idx].parent, count);
	  return (false);
	}
      if ((rbtree->pool[idx].next[0] != -1) && (rbtree->pool[idx].next[0] >= count))
	{
	  fprintf (stderr, "Node [%u] has a left child reference out of range %u > %u\n", idx, rbtree->pool[idx].next[0], count);
	  return (false);
	}
      if ((rbtree->pool[idx].next[1] != -1) && (rbtree->pool[idx].next[1] >= count))
	{
	  fprintf (stderr, "Node [%u] has a right child reference out of range %u > %u\n", idx, rbtree->pool[idx].next[1], count);
	  return (false);
	}
    }

  int height = 0;
  for (idx = rbtree->root; -1 != idx; idx = rbtree->pool[idx].next[0])
    height += !rbtree->pool[idx].red;

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
