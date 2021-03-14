#include <stdarg.h>
#include <check.h>
#include <metaresc.h>
#include <regression.h>

TYPEDEF_ENUM (mr_enum_t,
	      ZERO,
	      (ONE),
	      (TWO, = 2 /* set value explicitly */),
	      (THREE, = 3, "comment on enum value"),
	      (FOUR, /* value */, /* comment */, { "metadata as a void pointer" }),
	      (FIVE, /* value */, /* comment */, { "metadata as a void pointer" }, "string" /* type of metadata void pointer as string */),
	      (SIX, /* value */, /* comment */, { (mr_enum_t[]){ 2 } }, "mr_enum_t" /* even enum itself */),
	      SEVEN /* trailing comma is optional */
	      )

TYPEDEF_ENUM (mr_bitmask_t,
	      (NONE, = 0),
	      (FIRST, = 1 << 0),
	      (SECOND, = 1 << 1),
	      (THIRD, = 1 << 2),
	      (FORTH, = 1 << 3),
	      )

TYPEDEF_ENUM (mr_enum_uint8_t, ATTRIBUTES (__attribute__ ((packed))),
	      UINT8_ZERO, UINT8_ONE, (UINT8_TWO, = 2), (UINT8_THREE, = 3))
TYPEDEF_ENUM (mr_enum_uint16_t, ATTRIBUTES (__attribute__ ((packed))),
	      UINT16_ZERO, UINT16_ONE, (UINT16_TWO, = 2), (UINT16_THREE, = 3), (UINT16_LAST, = 1UL << __CHAR_BIT__ * (sizeof (uint16_t) >> 1)))
TYPEDEF_ENUM (mr_enum_uint32_t, ATTRIBUTES (__attribute__ ((packed))),
	      UINT32_ZERO, UINT32_ONE, (UINT32_TWO, = 2), (UINT32_THREE, = 3), (UINT32_LAST, = 1UL << __CHAR_BIT__ * (sizeof (uint32_t) >> 1)))
TYPEDEF_ENUM (mr_enum_uint64_t, ATTRIBUTES (__attribute__ ((packed))),
	      UINT64_ZERO, UINT64_ONE, (UINT64_TWO, = 2), (UINT64_THREE, = 3), (UINT64_LAST, = 1UL << __CHAR_BIT__ * (sizeof (uint64_t) >> 1)))

TYPEDEF_STRUCT (struct_mr_enum_t, (mr_enum_t, x))
TYPEDEF_STRUCT (struct_mr_enum_uint8_t, (mr_enum_uint8_t, x))
TYPEDEF_STRUCT (struct_mr_enum_uint16_t, (mr_enum_uint16_t, x))
TYPEDEF_STRUCT (struct_mr_enum_uint32_t, (mr_enum_uint32_t, x))
TYPEDEF_STRUCT (struct_mr_enum_uint64_t, (mr_enum_uint64_t, x))
TYPEDEF_STRUCT (struct_mr_bitmask_t, (mr_bitmask_t, x))

#define ASSERT_SAVE_LOAD_ENUM(METHOD, VALUE, ...) ({			\
      ASSERT_SAVE_LOAD_TYPE (METHOD, mr_enum_t, VALUE, __VA_ARGS__);	\
      ASSERT_SAVE_LOAD_TYPE (METHOD, mr_enum_uint8_t, VALUE, __VA_ARGS__); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, mr_enum_uint16_t, VALUE, __VA_ARGS__); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, mr_enum_uint32_t, VALUE, __VA_ARGS__); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, mr_enum_uint64_t, VALUE, __VA_ARGS__); \
    })

#define ASSERT_SAVE_LOAD_STRUCT_ENUM(METHOD, VALUE, ...) ({		\
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_mr_enum_t, VALUE, __VA_ARGS__);	\
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_mr_enum_uint8_t, VALUE, __VA_ARGS__); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_mr_enum_uint16_t, VALUE, __VA_ARGS__); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_mr_enum_uint32_t, VALUE, __VA_ARGS__); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_mr_enum_uint64_t, VALUE, __VA_ARGS__); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_mr_bitmask_t, VALUE, __VA_ARGS__); \
    })

START_TEST (zero_mr_enum_t) { ALL_METHODS (ASSERT_SAVE_LOAD_ENUM, 0); } END_TEST
START_TEST (zero_mr_struct_enum_t) { ALL_METHODS (ASSERT_SAVE_LOAD_STRUCT_ENUM, 0); } END_TEST

START_TEST (three_mr_enum_t) { ALL_METHODS (ASSERT_SAVE_LOAD_ENUM, (int)THREE); } END_TEST
START_TEST (three_mr_struct_enum_t) { ALL_METHODS (ASSERT_SAVE_LOAD_STRUCT_ENUM, (int)THREE); } END_TEST

static int warnings = 0;

static void
msg_handler (const char * file_name, const char * func_name, int line, mr_log_level_t log_level, mr_message_id_t message_id, va_list args)
{
  if (MR_MESSAGE_SAVE_ENUM == message_id)
    ++warnings;
}

#define SKIP_METHOD_XDR 0

START_TEST (invalid_mr_enum_t) {
  int checked = 0;
  mr_msg_handler_t save_msg_handler = mr_conf.msg_handler;
  
#define CMP_ENUMS(...) ({ ++checked; CMP_SERIALIAZED (__VA_ARGS__);})
#define CMP_STRUCT_ENUMS(...) ({ ++checked; CMP_SERIALIAZED (__VA_ARGS__);})

  mr_conf.msg_handler = msg_handler;
  ALL_METHODS (ASSERT_SAVE_LOAD_ENUM, -1, CMP_ENUMS);
  ALL_METHODS (ASSERT_SAVE_LOAD_STRUCT_ENUM, -1, CMP_STRUCT_ENUMS);
  mr_conf.msg_handler = save_msg_handler;

  ck_assert_msg ((checked == warnings), "Save/load of ivnalid enum value didn't produced mathced number of warnings (%d != %d)", checked, warnings);
} END_TEST

#undef SKIP_METHOD_XDR

MAIN_TEST_SUITE ((zero_mr_enum_t, "zero as number enum"),
		 (zero_mr_struct_enum_t, "zero as number enum"),
		 (three_mr_enum_t, "three as enum"),
		 (three_mr_struct_enum_t, "three as enum"),
		 (invalid_mr_enum_t, "invalid enum")
		 );
