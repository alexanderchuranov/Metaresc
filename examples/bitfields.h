
#include <reslib.h>

TYPEDEF_STRUCT (event_coordinates_t,
                (float, longitude),
                (float, latitude),
                BITFIELD (unsigned int, year, 12),
                BITFIELD (unsigned int, month, 4),
                BITFIELD (unsigned int, day, 5),
                BITFIELD (unsigned int, hour, 5),
                BITFIELD (unsigned int, minute, 6),
                BITFIELD (unsigned char, second, 6)
                )
