
#include <metaresc.h>

typedef struct {
  int x, * p, a[2], a2d[2][2], (*f)(int); /* legal C declaration */
} s1_t;

/* Metaresc equivalent */
TYPEDEF_STRUCT (s2_t,
                (int, x),
                (int *, p),
                (int, a, [2]),
                (int, a2d, [2][2]),
                (int, f, (int)),
                )
