
#include <reslib.h>

TYPEDEF_STRUCT (example_t,
                (int, var1),
                (int, array1, [3]),
                (double, temp2, [24], "The readings are taken each hour."),
                (float, lengths, [7], "In meters, not feets.", { "%03.3f" })
                )
