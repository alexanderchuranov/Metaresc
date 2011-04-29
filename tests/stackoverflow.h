/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

#include <reslib.h>

#define RUNTIME_LIMIT (3 * 1024 * 1024 + (512 + 256 + 128 + 126) * 1024 + 64 + 32 + 8 + 4 + 2) /* runtime limit */
#define VALGRIND_LIMIT ((1024 - 256 - 128 + 8 + 2) * 1024) /* valgrind default limit */
#define ARRAY_SIZE VALGRIND_LIMIT

TYPEDEF_STRUCT (large_struct_t,
		NONE (char, array, [ARRAY_SIZE])
		)
