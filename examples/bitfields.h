
#include <metaresc.h>

TYPEDEF_ENUM (month_t,
	      JAN, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC)

TYPEDEF_STRUCT (date_t,
                BITFIELD (unsigned int, year, :12),
                BITFIELD (month_t, month, :4),
                BITFIELD (unsigned int, day, :5),
                BITFIELD (unsigned int, hour, :5),
                BITFIELD (unsigned int, minute, :6),
                )
