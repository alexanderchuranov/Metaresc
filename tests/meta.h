/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

#include <reslib.h>

#undef RL_TYPE_NAME
#define RL_TYPE_NAME meta_type_t
RL_TYPEDEF_ENUM ()
  RL_ENUM_DEF (SIMPLE)
RL_END_ENUM ()

#undef RL_TYPE_NAME
#define RL_TYPE_NAME meta_info_t
RL_TYPEDEF_STRUCT ()
  RL_ENUM (meta_type_t, meta_type)
  RL_STRING (info)
  RL_STRING (format)
RL_END_STRUCT ()
