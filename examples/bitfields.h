
#include <metaresc.h>

TYPEDEF_ENUM (month_t,
	      JAN, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC)

TYPEDEF_STRUCT (date_t,
                (unsigned int, year, :12),
                (month_t, month, :4),
                (unsigned int, day, :5),
                (unsigned int, hour, :5),
                (unsigned int, minute, :6),
                )
