#include <check.h>
#include <metaresc.h>
#include <regression.h>

TYPEDEF_ENUM (_enum_t, (MINUS_ONE, = -1), ZERO, ONE, (TWO, = 2), (THREE, = 3));

TYPEDEF_STRUCT (struct_bitfield_int8_t, BITFIELD (int8_t, x, : sizeof (int8_t) * __CHAR_BIT__ - 1));
TYPEDEF_STRUCT (struct_bitfield_uint8_t, BITFIELD (uint8_t, x, : sizeof (uint8_t) * __CHAR_BIT__ - 1));
TYPEDEF_STRUCT (struct_bitfield_int16_t, BITFIELD (int16_t, x, : sizeof (int16_t) * __CHAR_BIT__ - 1));
TYPEDEF_STRUCT (struct_bitfield_uint16_t, BITFIELD (uint16_t, x, : sizeof (uint16_t) * __CHAR_BIT__ - 1));
TYPEDEF_STRUCT (struct_bitfield_int32_t, BITFIELD (int32_t, x, : sizeof (int32_t) * __CHAR_BIT__ - 1));
TYPEDEF_STRUCT (struct_bitfield_uint32_t, BITFIELD (uint32_t, x, : sizeof (uint32_t) * __CHAR_BIT__ - 1));
TYPEDEF_STRUCT (struct_bitfield_int64_t, BITFIELD (int64_t, x, : sizeof (int64_t) * __CHAR_BIT__ - 1));
TYPEDEF_STRUCT (struct_bitfield_uint64_t, BITFIELD (uint64_t, x, : sizeof (uint64_t) * __CHAR_BIT__ - 1));
TYPEDEF_STRUCT (struct_bitfield_intmax_t, BITFIELD (mr_intmax_t, x, : sizeof (mr_intmax_t) * __CHAR_BIT__ - 1));
TYPEDEF_STRUCT (struct_bitfield_uintmax_t, BITFIELD (mr_uintmax_t, x, : sizeof (mr_uintmax_t) * __CHAR_BIT__ - 1));

TYPEDEF_STRUCT (struct_bitfield_enum_t, BITFIELD (_enum_t, x, : sizeof (_enum_t) * __CHAR_BIT__ - 1));

#define CMP_SCALAR(TYPE, X, Y, ...) ((X)->x != (Y)->x)

#define ASSERT_SAVE_LOAD_BITFIELD(METHOD, VALUE) ({			\
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_bitfield_int8_t, VALUE, CMP_SCALAR); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_bitfield_int16_t, VALUE, CMP_SCALAR); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_bitfield_int32_t, VALUE, CMP_SCALAR); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_bitfield_int64_t, VALUE, CMP_SCALAR); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_bitfield_intmax_t, VALUE, CMP_SCALAR); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_bitfield_uint8_t, VALUE, CMP_SCALAR); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_bitfield_uint16_t, VALUE, CMP_SCALAR); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_bitfield_uint32_t, VALUE, CMP_SCALAR); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_bitfield_uint64_t, VALUE, CMP_SCALAR); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_bitfield_uintmax_t, VALUE, CMP_SCALAR); \
    });

START_TEST (bitfield_enum_zero) { ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_bitfield_enum_t, ZERO); } END_TEST
START_TEST (bitfield_enum_three) { ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_bitfield_enum_t, THREE); } END_TEST

static int warnings;

static void
msg_handler (const char * file_name, const char * func_name, int line, mr_log_level_t log_level, mr_message_id_t message_id, va_list args)
{
  if (MR_MESSAGE_SAVE_ENUM == message_id)
    ++warnings;
}

#define SKIP_METHOD_XDR 0

START_TEST (invalid_bitfield_enum_t) {
  int checked = 0;
  mr_msg_handler_t save_msg_handler = mr_conf.msg_handler;

#define CMP_SCALAR_CNT(...) ({ ++checked; CMP_SCALAR (__VA_ARGS__);})

  warnings = 0;
  mr_conf.msg_handler = msg_handler;
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_bitfield_enum_t, -2, CMP_SCALAR_CNT);
  mr_conf.msg_handler = save_msg_handler;

  ck_assert_msg ((checked == warnings), "Save/load of ivnalid enum value didn't produced mathced number of warnings (%d != %d)", checked, warnings);
} END_TEST

#undef SKIP_METHOD_XDR

START_TEST (bitfield_int_0) {
  ALL_METHODS (ASSERT_SAVE_LOAD_BITFIELD, 0);
} END_TEST

START_TEST (bitfield_int_3) {
  ALL_METHODS (ASSERT_SAVE_LOAD_BITFIELD, 3);
} END_TEST

START_TEST (bitfield_int_m1) {
  ALL_METHODS (ASSERT_SAVE_LOAD_BITFIELD, -1);
} END_TEST

START_TEST (bitfield_negative_enum) {
  mr_msg_handler_t save_msg_handler = mr_conf.msg_handler;

  warnings = 0;
  mr_conf.msg_handler = msg_handler;
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_bitfield_enum_t, MINUS_ONE, CMP_SCALAR);
  mr_conf.msg_handler = save_msg_handler;

  ck_assert_msg ((0 == warnings), "Save/load of negative enum value produced %d warnings", warnings);
} END_TEST

MAIN_TEST_SUITE ((bitfield_enum_zero, "bitfield as enum"),
		 (bitfield_enum_three, "bitfield as enum"),
		 (invalid_bitfield_enum_t, "invalid enum"),
		 (bitfield_int_0, "bitfield as integer for value 0"),
		 (bitfield_int_3, "bitfield as integer for value 3"),
		 (bitfield_int_m1, "bitfield as integer for value -1"),
		 (bitfield_negative_enum, "-1 should serialize as enum value, but not integer")
		 );
