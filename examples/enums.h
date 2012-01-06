
#include <reslib.h>

TYPEDEF_ENUM (color_t,
              BLACK,          // auto-enumerated and becomes 0
              (RED),          // auto-enumerated and becomes 1
              
              (GREEN,   = 2), // explicitly set to 2
              (YELLOW,  = 3,  "set to 3 - this is an enumeration constant comment"),
              (BLUE,    = 4,  "set to 4", { "metadata is a void pointer" }),
              
              (MAGENTA, /* value argument may be empty */,
                        "becomes 5",
                        { "next argument is a type of this poiner" },
                        "string_t"
              ),
              
              (CYAN,    /* auto-enumerated */,
                        /* no comment      */,
                        { (color_t[]){ 6 } },
                        "color_t"
              ),
              
              WHITE // trailing comma is optional
              )
