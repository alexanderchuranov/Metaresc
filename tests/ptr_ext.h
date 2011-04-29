/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

#include <reslib.h>

TYPEDEF_STRUCT (x_t,
		POINTER_STRUCT (y_t, y)
		)

TYPEDEF_STRUCT (z_t, 
		(x_t *, x)
		)

TYPEDEF_STRUCT (y_t,
		(z_t, z)
		)

TYPEDEF_STRUCT (static_data_t,
		(uint16_t, data, [4])
		)

TYPEDEF_STRUCT (shifted_data_t,
		(uint8_t, data, [4])
		)

TYPEDEF_STRUCT (main_data_t,
		(shifted_data_t *, shifted_data1),
		(shifted_data_t *, shifted_data2),
		(static_data_t *, static_data),
		(shifted_data_t *, shifted_data3),
		(shifted_data_t *, shifted_data4),
		)
