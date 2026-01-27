#include <stdarg.h>
#include <check.h>
#include <metaresc.h>
#include <regression.h>

typedef char char_array_t[256];
TYPEDEF_STRUCT (struct_string_t, string_t x);
TYPEDEF_STRUCT (struct_charp_t, (char *, x));
TYPEDEF_STRUCT (struct_char_array_t, (char, x, [256])); /* saved as an array of characters */
TYPEDEF_STRUCT (struct_ca_t, (char, x, [256], , .stype.mr_type = MR_TYPE_CHAR_ARRAY, .stype.is_array = false)); /* saved as a quoted string */
TYPEDEF_STRUCT (struct_ca_type_t, (char_array_t, x));

TYPEDEF_STRUCT (struct_ca_str_t, (char_array_t, x), string_t y);
TYPEDEF_STRUCT (struct_str_ca_t, string_t y, (char_array_t, x));
TYPEDEF_STRUCT (struct_str_str_t, string_t x, string_t y);

TYPEDEF_STRUCT (struct_ca_int_t, ATTRIBUTES (__attribute__ ((packed))),
		(char, x, [1], , .stype.mr_type = MR_TYPE_CHAR_ARRAY, .stype.is_array = false),
		int y);

#define ASSERT_SAVE_LOAD_CHAR_POINTER(METHOD, ...) ({			\
      ASSERT_SAVE_LOAD_TYPE (METHOD, string_t,  __VA_ARGS__);		\
      ASSERT_SAVE_LOAD_TYPE (METHOD, char *, __VA_ARGS__);		\
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_string_t, __VA_ARGS__);	\
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_charp_t, __VA_ARGS__);	\
    })

#define ASSERT_SAVE_LOAD_STRING(METHOD, ...) ({				\
      ASSERT_SAVE_LOAD_CHAR_POINTER (METHOD, __VA_ARGS__);		\
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_char_array_t, __VA_ARGS__); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_ca_t, __VA_ARGS__);		\
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_ca_type_t, __VA_ARGS__);	\
    })

#define ASSERT_SAVE_LOAD_CA_INT(METHOD, VALUE, ...) ({			\
      struct_ca_int_t x = { VALUE, '0' };				\
      ASSERT_SAVE_LOAD (METHOD, struct_ca_int_t, &x, __VA_ARGS__);	\
    })

START_TEST (null_string) { ALL_METHODS (ASSERT_SAVE_LOAD_CHAR_POINTER, NULL); } END_TEST
START_TEST (empty_string) { ALL_METHODS (ASSERT_SAVE_LOAD_STRING, ""); } END_TEST
START_TEST (printable_string) { ALL_METHODS (ASSERT_SAVE_LOAD_STRING, "\040\041\042\043\044\045\046\047\050\051\052\053\054\055\056\057\060\061\062\063\064\065\066\067\070\071\072\073\074\075\076\077\100\101\102\103\104\105\106\107\110\111\112\113\114\115\116\117\120\121\122\123\124\125\126\127\130\131\132\133\134\135\136\137\140\141\142\143\144\145\146\147\150\151\152\153\154\155\156\157\160\161\162\163\164\165\166\167\170\171\172\173\174\175\176"); } END_TEST
START_TEST (xml_special_string) { ALL_METHODS (ASSERT_SAVE_LOAD_STRING, "&<>;'\"\t\r\n"); } END_TEST

static int warnings = 0;
static void
msg_handler (const char * file_name, const char * func_name, int line, mr_log_level_t log_level, mr_message_id_t message_id, va_list args)
{
  ++warnings;
}

START_TEST (char_array_overflow) {
  mr_msg_handler_t save_msg_handler = mr_conf.msg_handler;
  mr_conf.msg_handler = msg_handler;
  ALL_METHODS (ASSERT_SAVE_LOAD_CA_INT, "0");
  mr_conf.msg_handler = save_msg_handler;
  ck_assert_msg ((0 == warnings), "Unexpected warnings #%d", warnings);
} END_TEST

START_TEST (pointer_match_content_known) {
  struct_ca_str_t orig = { .x = "string_t" };
  orig.y = orig.x;
  mr_ptrdes_t * ptrs = MR_SAVE (struct_ca_str_t, &orig);
  if (ptrs != NULL)
    {
      bool string_is_a_reference = false;
      mr_idx_t i;
      for (i = 1; i < ptrs[0].next; ++i)
	if (ptrs[i].fdp &&
	    (ptrs[i].fdp->name.str != NULL) &&
	    (0 == strcmp (ptrs[i].fdp->name.str, "y")) &&
	    (ptrs[i].flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE)))
	  string_is_a_reference = true;

      MR_FREE (ptrs);
      ck_assert_msg (string_is_a_reference, "string was not resolved a reference on char array");
    }
  ALL_METHODS (ASSERT_SAVE_LOAD, struct_ca_str_t, &orig);
} END_TEST

START_TEST (pointer_match_content_unknown) {
  struct_str_ca_t orig = { .x = "string_t" };
  orig.y = orig.x;
  mr_ptrdes_t * ptrs = MR_SAVE (struct_str_ca_t, &orig);
  if (ptrs != NULL)
    {
      bool string_is_a_reference = false;
      mr_idx_t i;
      for (i = 1; i < ptrs[0].next; ++i)
	if (ptrs[i].fdp &&
	    (ptrs[i].fdp->name.str != NULL) &&
	    (0 == strcmp (ptrs[i].fdp->name.str, "y")) &&
	    (ptrs[i].flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE)))
	  string_is_a_reference = true;

      MR_FREE (ptrs);
      ck_assert_msg (string_is_a_reference, "string was not resolved a reference on char array");
    }
  ALL_METHODS (ASSERT_SAVE_LOAD, struct_str_ca_t, &orig);
} END_TEST

START_TEST (pointer_match_another_pointer) {
  struct_str_str_t orig = { .x = "string_t", };
  mr_td_t * string_tdp = mr_get_td_by_name ("string_t");
  orig.y = orig.x;
  mr_ptrdes_t * ptrs = MR_SAVE (struct_str_str_t, &orig);

  if (ptrs != NULL)
    {
      int ptr_cnt = 0;
      mr_idx_t i;
      for (i = 1; i < ptrs[0].next; ++i)
	if ((ptrs[i].fdp->stype.tdp == string_tdp) && (ptrs[i].flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE)))
	  ++ptr_cnt;
      
      MR_FREE (ptrs);
      ck_assert_msg (1 == ptr_cnt, "pointer on existing string was not detected properly");
    }
  ALL_METHODS (ASSERT_SAVE_LOAD, struct_str_str_t, &orig);
} END_TEST

MAIN_TEST_SUITE ((null_string, "NULL string"),
		 (empty_string, "empty string"),
		 (printable_string, "all printable characters string"),
		 (xml_special_string, "xml special characters string"),
		 (char_array_overflow, "inline char array overflow"),
		 (pointer_match_content_known, "strings points on known char array"),
		 (pointer_match_content_unknown, "strings points on unknown char array"),
		 (pointer_match_another_pointer, "two strings points on the same content")
		 );
