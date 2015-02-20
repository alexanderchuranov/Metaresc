
#include <metaresc.h>

TYPEDEF_ENUM (color_t,
              BLACK,          /* auto-enumerated and becomes 0 */
              (RED),          /* auto-enumerated and becomes 1 */

              (GREEN,   = 2), /* explicitly set to 2 */
              (YELLOW,  = 3,  "set to 3 - this is a textual meta info"),
              (BLUE, ,  "auto-enumerated", { "a void pointer for arbitrary resource" }),

              (MAGENTA,
	       /* value argument may be empty */,
	       "becomes 5",
	      { "next argument is a type of this poiner" },
	       "char"
	       ),

              (CYAN,
	       /* auto-enumerated */,
	       /* no meta         */,
	      { (color_t[]){ CYAN } },
	       "color_t" /* type itself might be used for initialization of resource */
	       ),

              (WHITE,
	       /* auto-enumerated */,
	       /* no meta         */,
	      { (color_t[]){ CYAN, WHITE } },
	       "color_t",
	       2 * sizeof (color_t) /* size of resource array */
	       )           /* trailing comma is optional */
              )
