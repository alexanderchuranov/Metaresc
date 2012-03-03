#include <check.h>
#include <metaresc.h>
#include <regression.h>

TYPEDEF_STRUCT (ld_ptr_t, (const volatile long double *, x));

MR_START_TEST (ld_ptr, "pointer on a long double (type name with spaces)") {
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, ld_ptr_t, NULL);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, ld_ptr_t, (long double[]){ 0 });
} END_TEST

TYPEDEF_ENUM (_enum_t, ZERO, ONE);
TYPEDEF_ENUM (packed_enum_t, ATTRIBUTES (__attribute__ ((packed, aligned (sizeof (uint16_t))))), PE_ZERO, PE_ONE);
TYPEDEF_STRUCT (enum_ptr_t, (_enum_t *, x));
TYPEDEF_STRUCT (packed_enum_ptr_t, (packed_enum_t *, x));

MR_START_TEST (enum_ptr, "pointer on enum") {
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, enum_ptr_t, NULL);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, enum_ptr_t, (_enum_t[]){ ZERO });
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, enum_ptr_t, (_enum_t[]){ ONE });
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, packed_enum_ptr_t, NULL);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, packed_enum_ptr_t, (packed_enum_t[]){ PE_ZERO });
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, packed_enum_ptr_t, (packed_enum_t[]){ PE_ONE });
} END_TEST

MR_START_TEST (invalid_enum_ptr, "pointer on invalid enum") {
  int checked = 0;
  int warnings = 0;
  void (*save_msg_handler) (const char*, const char*, int, mr_log_level_t, mr_message_id_t, va_list) = mr_conf.msg_handler;

  void msg_handler (const char * file_name, const char * func_name, int line, mr_log_level_t log_level, mr_message_id_t message_id, va_list args)
  {
    if ((MR_MESSAGE_SAVE_ENUM == message_id) || (MR_MESSAGE_SAVE_BITMASK == message_id))
      ++warnings;
  }

#define CMP_ENUMS(TYPE, X, Y, ...) ({ ++checked; (*((X)->x) != *((Y)->x));})

  mr_conf.msg_handler = msg_handler;
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, enum_ptr_t, (_enum_t[]){ -1 }, CMP_ENUMS);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, packed_enum_ptr_t, (packed_enum_t[]){ -1 }, CMP_ENUMS);
  mr_conf.msg_handler = save_msg_handler;
  
  ck_assert_msg ((checked == warnings), "Save/load of ivnalid enum value didn't produced mathced number of warnings (%d != %d)", checked, warnings);
} END_TEST

TYPEDEF_STRUCT (char_ptr_t, POINTER (char, x));

MR_START_TEST (char_ptr, "pointer on char") {
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, char_ptr_t, NULL);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, char_ptr_t, "x");
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, char_ptr_t, (char[]){ 0 });
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, char_ptr_t, (char[]){ 255 });
} END_TEST

TYPEDEF_STRUCT (charp_ptr_t, (char**, x));
TYPEDEF_STRUCT (string_ptr_t, (string_t*, x));

MR_START_TEST (string_ptr, "pointer on string") {
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, charp_ptr_t, NULL);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, charp_ptr_t, (char*[]){ "x" });
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, string_ptr_t, NULL);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, string_ptr_t, (string_t[]){ "x" });
} END_TEST

TYPEDEF_STRUCT (struct_ptr_t, (enum_ptr_t*, x));

MR_START_TEST (struct_ptr, "pointer on struct") {
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_ptr_t, NULL);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_ptr_t, (enum_ptr_t[]){ { NULL } });
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_ptr_t, (enum_ptr_t[]){ { (_enum_t[]){ ZERO } } });
} END_TEST

TYPEDEF_STRUCT (self_ref_ptr_t, (_enum_t *, x), (_enum_t, y));

MR_START_TEST (self_ref_ptr, "self referenced pointer") {
  self_ref_ptr_t x = { (_enum_t[]){ ZERO }, ONE };
  ALL_METHODS (ASSERT_SAVE_LOAD, self_ref_ptr_t, &x);
  x.x = &x.y;
  ALL_METHODS (ASSERT_SAVE_LOAD, self_ref_ptr_t, &x);
  x.x = (void*)&x;
  ALL_METHODS (ASSERT_SAVE_LOAD, self_ref_ptr_t, &x);
} END_TEST

TYPEDEF_STRUCT (self_ref_string_t,
		CHAR_ARRAY (char, x, [sizeof ("x")]),
		string_t y,
		(string_t *, z),
		(void *, v));

MR_START_TEST (self_ref_string, "self referenced strings") {
  self_ref_string_t x = { "x", "y", (string_t[]){ "z" }};
  x.v = &x.z; /* resolved not-NULL void pointer */
  ALL_METHODS (ASSERT_SAVE_LOAD, self_ref_string_t, &x);
  x.z = &x.y;
  ALL_METHODS (ASSERT_SAVE_LOAD, self_ref_string_t, &x);
  x.y = x.x;
  ALL_METHODS (ASSERT_SAVE_LOAD, self_ref_string_t, &x);
  x.v = "z"; /* unresolved not-NULL void pointer */
  ALL_METHODS (ASSERT_SAVE_LOAD, self_ref_string_t, &x);
} END_TEST

TYPEDEF_STRUCT (rtfr_struct_t,
		int x,
		int y);
  
TYPEDEF_STRUCT (resolve_typed_forward_ref_t,
		(rtfr_struct_t *, x),
		(int *, y) /* pointers are loaded via stack, so one level ponters will be loaded in a reverse order */
		);

MR_START_TEST (resolve_typed_forward_ref, "test of forvard reference resolution") {
  resolve_typed_forward_ref_t x = { (rtfr_struct_t[]){ {0, 1} } };
  x.y = &x.x->y;
  ALL_METHODS (ASSERT_SAVE_LOAD, resolve_typed_forward_ref_t, &x);
} END_TEST

MAIN ();
