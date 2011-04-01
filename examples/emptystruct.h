
#include <reslib.h>
                              // equivalent to the following:
#undef RL_TYPE_NAME           //
#define RL_TYPE_NAME empty_t  // typedef struct
RL_TYPEDEF_STRUCT ()          // { } empty_t;
RL_END_STRUCT ()              //
                              // and some METARESC meta-data magic
#undef RL_MODE
