
#include <reslib.h>

TYPEDEF_STRUCT (back_end_server,
                (int, id),
                (unsigned long int, address),
                (unsigned short int, port)
                )

typedef struct {
  int data;
} filter;

TYPEDEF_STRUCT (policy,
                (back_end_server*, server_a),
                (back_end_server*, server_b),
                (filter*, f),
                (back_end_server*, fallback)
                )
