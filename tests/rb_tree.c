#include <stdio.h>
#include <search.h>

int cmp (const void * a, const void * b) { return 1; }

int main ()
{
  void * root = NULL;
  int cnt;

  int cmp (const void * a, const void * b) { return 1; }
  void count (const void * nodep, VISIT visit_type, int level) { cnt += (visit_type == leaf); }

  for (cnt = 0; cnt < 3; ++cnt)
    tsearch (NULL, &root, cmp);
  cnt = 0;
  twalk (root, count);
  return (1 != cnt);
}
