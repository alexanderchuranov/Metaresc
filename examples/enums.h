
#include <metaresc.h>

TYPEDEF_ENUM (color_t, ATTRIBUTES (__attribute__ ((packed))),
              RED,           /* auto-enumerated and becomes 0 */
              (ORANGE),      /* auto-enumerated and becomes 1 */

              (YELLOW, = 2), /* explicitly set to 2 */
              (GREEN,  = 3,  "set to 3 - this is a textual meta info"),
              (BLUE, ,  "auto-enumerated", { "a void pointer for arbitrary resource" }),

              (PURPLE,
	       /* value argument may be empty */,
	       "becomes 5",
	       { "next argument is a type of this poiner" },
	       "string"
	       ),

              (PINK,
	       /* auto-enumerated */,
	       /* no meta         */,
	       { (color_t[]){ PINK } },
	       "color_t" /* type itself might be used for initialization of resource */
	       ),

              (BROWN,
	       /* auto-enumerated */,
	       /* no meta         */,
	       { (color_t[]){ RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE, PINK, BROWN } },
	       "color_t",
	       8 * sizeof (color_t) /* size of resource array */
	       )           /* trailing comma is optional */
              );
