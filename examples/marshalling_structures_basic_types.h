
#include <reslib.h>

TYPEDEF_STRUCT (floatings_t,
                (float, f),
                (long double, ld),
                (double _Complex, dc)
                )

TYPEDEF_STRUCT (basics_t,
                (char, c),
                (unsigned long int, uli),
                (floatings_t, fl)
                )
