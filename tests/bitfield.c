#include <check.h>
#include <metaresc.h>
#include <regression.h>

TYPEDEF_ENUM (_enum_t, ZERO, ONE, (TWO, = 2), (THREE, = 3))

TYPEDEF_STRUCT (struct_bitfield_int8_t, BITFIELD (int8_t, x, 7))
TYPEDEF_STRUCT (struct_bitfield_uint8_t, BITFIELD (uint8_t, x, 7))
TYPEDEF_STRUCT (struct_bitfield_int16_t, BITFIELD (int16_t, x, 15))
TYPEDEF_STRUCT (struct_bitfield_uint16_t, BITFIELD (uint16_t, x, 15))
TYPEDEF_STRUCT (struct_bitfield_int32_t, BITFIELD (int32_t, x, 31))
TYPEDEF_STRUCT (struct_bitfield_uint32_t, BITFIELD (uint32_t, x, 31))
TYPEDEF_STRUCT (struct_bitfield_int64_t, BITFIELD (int64_t, x, 63))
TYPEDEF_STRUCT (struct_bitfield_uint64_t, BITFIELD (uint64_t, x, 63))

TYPEDEF_STRUCT (struct_bitfield_enum_t, BITFIELD (_enum_t, x, sizeof (enum_t) * 8 - 1))

#define ASSERT_SAVE_LOAD_BITFIELD(METHOD, VALUE, ...) ({		\
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_bitfield_int8_t, VALUE, __VA_ARGS__); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_bitfield_int16_t, VALUE, __VA_ARGS__); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_bitfield_int32_t, VALUE, __VA_ARGS__); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_bitfield_int64_t, VALUE, __VA_ARGS__); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_bitfield_uint8_t, VALUE, __VA_ARGS__); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_bitfield_uint16_t, VALUE, __VA_ARGS__); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_bitfield_uint32_t, VALUE, __VA_ARGS__); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_bitfield_uint64_t, VALUE, __VA_ARGS__); \
    })

MR_START_TEST (bitfield_enum_zero, "bitfield as enum") { ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_bitfield_enum_t, ZERO, STRUCT_X_CMP); } END_TEST
MR_START_TEST (bitfield_enum_three, "bitfield as enum") { ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_bitfield_enum_t, THREE, STRUCT_X_CMP); } END_TEST

MR_START_TEST (invalid_bitfield_enum_t, "invalid enum") {
  int checked = 0;
  int warnings = 0;
  void (*save_msg_handler) (const char*, const char*, int, mr_log_level_t, mr_message_id_t, va_list) = mr_conf.msg_handler;

  void msg_handler (const char * file_name, const char * func_name, int line, mr_log_level_t log_level, mr_message_id_t message_id, va_list args)
  {
    if ((MR_MESSAGE_SAVE_ENUM == message_id) || (MR_MESSAGE_SAVE_BITMASK == message_id))
      ++warnings;
  }

#define CMP_STRUCT_(...) ({ ++checked; STRUCT_X_CMP (__VA_ARGS__);})

  mr_conf.msg_handler = msg_handler;
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_bitfield_enum_t, -1, CMP_STRUCT_);
  mr_conf.msg_handler = save_msg_handler;
  
  ck_assert_msg ((checked == warnings), "Save/load of ivnalid enum value didn't produced mathced number of warnings (%d != %d)", checked, warnings);
} END_TEST

MR_START_TEST (bitfield_int_0, "bitfield as integer for value 0") {
  ALL_METHODS (ASSERT_SAVE_LOAD_BITFIELD, 0, STRUCT_X_CMP);
} END_TEST

MR_START_TEST (bitfield_int_3, "bitfield as integer for value 3") {
  ALL_METHODS (ASSERT_SAVE_LOAD_BITFIELD, 3, STRUCT_X_CMP);
} END_TEST

MR_START_TEST (bitfield_int_m1, "bitfield as integer for value -1") {
  ALL_METHODS (ASSERT_SAVE_LOAD_BITFIELD, -1, STRUCT_X_CMP);
} END_TEST

MAIN ();
