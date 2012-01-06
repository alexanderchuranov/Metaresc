
#include <reslib.h>

TYPEDEF_STRUCT (example_t,
                (int, x1),
                (int, x2, [3]),
                (double, x3, /* empty */, "The readings are taken each hour."),
                (float, x4, /* empty */, /* empty */, { "%03.3f" }, "string_t"),
		int x5,
		volatile const unsigned long long int x6,
		string_t x7
                )
