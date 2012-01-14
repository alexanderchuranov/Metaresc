#include <stdarg.h>
#include <check.h>
#include <reslib.h>
#include <regression.h>

TYPEDEF_STRUCT (struct_string_t, string_t x)
TYPEDEF_STRUCT (struct_charp_t, (char *, x))

#define ASSERT_SAVE_LOAD_STRING(METHOD, ...) ({			    \
      ASSERT_SAVE_LOAD_TYPE (METHOD, string_t,  __VA_ARGS__);	    \
      ASSERT_SAVE_LOAD_TYPE (METHOD, char *, __VA_ARGS__);	    \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_string_t, __VA_ARGS__); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_charp_t, __VA_ARGS__);  \
    })

RL_START_TEST (null_string_t, "NULL string") { ALL_METHODS (ASSERT_SAVE_LOAD_STRING, NULL); } END_TEST
RL_START_TEST (empty_string_t, "empty string") { ALL_METHODS (ASSERT_SAVE_LOAD_STRING, ""); } END_TEST
RL_START_TEST (random_string_t, "random string") { ALL_METHODS (ASSERT_SAVE_LOAD_STRING, "string_t"); } END_TEST
RL_START_TEST (printable_string_t, "all printable characters string") { ALL_METHODS (ASSERT_SAVE_LOAD_STRING, "\040\041\042\043\044\045\046\047\050\051\052\053\054\055\056\057\060\061\062\063\064\065\066\067\070\071\072\073\074\075\076\077\100\101\102\103\104\105\106\107\110\111\112\113\114\115\116\117\120\121\122\123\124\125\126\127\130\131\132\133\134\135\136\137\140\141\142\143\144\145\146\147\150\151\152\153\154\155\156\157\160\161\162\163\164\165\166\167\170\171\172\173\174\175\176"); } END_TEST
RL_START_TEST (xml_special_string_t, "xml special characters string") { ALL_METHODS (ASSERT_SAVE_LOAD_STRING, "&<>;'\"\t\r\n"); } END_TEST

MAIN ();
