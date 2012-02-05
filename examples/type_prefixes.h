
#include <metaresc.h>

typedef struct {
  int x;
  int * p;
} s1_t;

TYPEDEF_STRUCT (s2_t,
                (int, x),
                (int*, p)
                )
