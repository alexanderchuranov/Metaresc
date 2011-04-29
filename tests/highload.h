/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

#include <reslib.h>

TYPEDEF_STRUCT (list_t,
		(list_t *, next)
		)

TYPEDEF_STRUCT (array_t,
		RARRAY (list_t, rarray)
		)
