// compiler guards are deliberately removed,
// no #ifndef #define #endif here

#include <reslib.h>

#undef RL_TYPE_NAME
#define RL_TYPE_NAME employee_t
RL_TYPEDEF_STRUCT ()
  RL_STRING (firstname)
  RL_STRING (lastname)
  RL_UINT32 (salary)
RL_END_STRUCT ()

#undef RL_MODE
