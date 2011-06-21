
#include <reslib.h>

typedef struct {
  int x, *p;
} s1_t;

TYPEDEF_STRUCT (s2_t,
                (int, x),
                (int*, p)
                )
