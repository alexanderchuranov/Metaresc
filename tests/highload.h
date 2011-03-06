/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

#include <reslib.h>

#undef RL_TYPE_NAME
#define RL_TYPE_NAME list_t
RL_TYPEDEF_STRUCT ()
  RL_POINTER (list_t, next)
RL_END_STRUCT ()

#undef RL_TYPE_NAME
#define RL_TYPE_NAME array_t
RL_TYPEDEF_STRUCT ()
  RL_RARRAY (list_t, rarray)
RL_END_STRUCT ()

#undef RL_MODE
