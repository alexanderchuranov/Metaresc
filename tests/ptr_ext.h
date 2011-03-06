/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

#include <reslib.h>

#undef RL_TYPE_NAME
#define RL_TYPE_NAME x_t
RL_TYPEDEF_STRUCT ()
  RL_POINTER_STRUCT (y_t, y)
RL_END_STRUCT ()

#undef RL_TYPE_NAME
#define RL_TYPE_NAME z_t
RL_TYPEDEF_STRUCT ()
  RL_POINTER (x_t, x)
RL_END_STRUCT ()

#undef RL_TYPE_NAME
#define RL_TYPE_NAME y_t
RL_TYPEDEF_STRUCT ()
  RL_STRUCT (z_t, z)
RL_END_STRUCT ()

#undef RL_TYPE_NAME
#define RL_TYPE_NAME main_data_t
RL_TYPEDEF_STRUCT ()
  RL_POINTER_STRUCT (shifted_data_t, shifted_data1)
  RL_POINTER_STRUCT (shifted_data_t, shifted_data2)
  RL_POINTER_STRUCT (static_data_t, static_data)
  RL_POINTER_STRUCT (shifted_data_t, shifted_data3)
  RL_POINTER_STRUCT (shifted_data_t, shifted_data4)
RL_END_STRUCT ()

#undef RL_TYPE_NAME
#define RL_TYPE_NAME static_data_t
RL_TYPEDEF_STRUCT ()
  RL_ARRAY (uint16_t, data, [4])
RL_END_STRUCT ()

#undef RL_TYPE_NAME
#define RL_TYPE_NAME shifted_data_t
RL_TYPEDEF_STRUCT ()
  RL_ARRAY (uint8_t, data, [4])
RL_END_STRUCT ()

#undef RL_MODE
