
#include <reslib.h>

TYPEDEF_UNION (floating_t,
               (unsigned char, ar, [sizeof(float)]),
               (float, f)
               )
