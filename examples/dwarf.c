#include <stdio.h>
#include <stdlib.h>

#include <metaresc.h>

typedef struct tree_node_t {
  char * value;
  struct tree_node_t * left, * right;
} tree_node_t;

static tree_node_t root = {
  "root",
  (tree_node_t[]) { { "left" } },
  (tree_node_t[]) { { "right" } },
};

int main (int argc, char * argv[])
{
  char * dump = MR_SAVE_CINIT (tree_node_t, &root);
  if (dump)
    {
      printf ("%s", dump);
      MR_FREE (dump);
    }
  return (EXIT_SUCCESS);
}
