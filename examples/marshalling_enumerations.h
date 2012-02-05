
#include <metaresc.h>

TYPEDEF_ENUM (color_t,
              (BLACK),
              (RED,     = 1),
              (GREEN,      ),
              (YELLOW),
              (BLUE,    = 4),
              (MAGENTA, = 5),
              (CYAN,    = 6),
              (WHITE,   = 7)
              )

TYPEDEF_STRUCT (screen_t,
                (int, width),
                (int, height),
                (color_t, bg),
                (color_t, fg)
                )
