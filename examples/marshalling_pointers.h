
#include <metaresc.h>

TYPEDEF_STRUCT (back_end_server_t,
                (int, id),
                (unsigned long int, address),
                (unsigned short int, port)
                )

typedef struct {
  int data;
} filter_t;

TYPEDEF_STRUCT (policy_t,
                (back_end_server_t *, server_a),
                (back_end_server_t *, server_b),
                (filter_t *, f),
                (back_end_server_t *, fallback)
                )
