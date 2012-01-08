#include <stdarg.h>
#include <check.h>
#include <reslib.h>
#include <regression.h>

TYPEDEF_ENUM (rl_enum_t,
	      ZERO,
	      (ONE),
	      (TWO, = 2 /* set value explicitly */),
	      (THREE, /* value */, "comment on enum value"),
	      (FOUR, /* value */, /* comment */, { "metadata as a void pointer" }),
	      (FIVE, /* value */, /* comment */, { "metadata as a void pointer" }, "string_t" /* type of metadata void pointer as string */),
	      (SIX, /* value */, /* comment */, { (rl_enum_t[]){ 2 } }, "rl_enum_t" /* even enum itself */),
	      SEVEN /* trailing comma is optional */
	      );

TYPEDEF_ENUM (rl_enum_uint8_t, ATTRIBUTES (__attribute__ ((packed, aligned (sizeof (uint8_t))))),
	      UINT8_ZERO, UINT8_ONE, (UINT8_TWO, = 2))
TYPEDEF_ENUM (rl_enum_uint16_t, ATTRIBUTES (__attribute__ ((packed, aligned (sizeof (uint16_t))))),
	      UINT16_ZERO, UINT16_ONE, (UINT16_TWO, = 2))
TYPEDEF_ENUM (rl_enum_uint32_t, ATTRIBUTES (__attribute__ ((packed, aligned (sizeof (uint32_t))))),
	      UINT32_ZERO, UINT32_ONE, (UINT32_TWO, = 2))
TYPEDEF_ENUM (rl_enum_uint64_t, ATTRIBUTES (__attribute__ ((packed, aligned (sizeof (uint64_t))))),
	      UINT64_ZERO, UINT64_ONE, (UINT64_TWO, = 2))

TYPEDEF_STRUCT (struct_rl_enum_t, (rl_enum_t, x));
TYPEDEF_STRUCT (struct_rl_enum_uint8_t, (rl_enum_uint8_t, x));
TYPEDEF_STRUCT (struct_rl_enum_uint16_t, (rl_enum_uint16_t, x));
TYPEDEF_STRUCT (struct_rl_enum_uint32_t, (rl_enum_uint32_t, x));
TYPEDEF_STRUCT (struct_rl_enum_uint64_t, (rl_enum_uint64_t, x));

#define ASSERT_SAVE_LOAD_ENUM(METHOD, VALUE, ...) ({			\
      ASSERT_SAVE_LOAD_TYPE (METHOD, rl_enum_t, VALUE, __VA_ARGS__);	\
      ASSERT_SAVE_LOAD_TYPE (METHOD, rl_enum_uint8_t, VALUE, __VA_ARGS__); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, rl_enum_uint16_t, VALUE, __VA_ARGS__); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, rl_enum_uint32_t, VALUE, __VA_ARGS__); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, rl_enum_uint64_t, VALUE, __VA_ARGS__); \
    })

#define ASSERT_SAVE_LOAD_STRUCT_ENUM(METHOD, VALUE, ...) ({		\
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_rl_enum_t, { VALUE }, __VA_ARGS__);	\
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_rl_enum_uint8_t, { VALUE }, __VA_ARGS__); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_rl_enum_uint16_t, { VALUE }, __VA_ARGS__); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_rl_enum_uint32_t, { VALUE }, __VA_ARGS__); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_rl_enum_uint64_t, { VALUE }, __VA_ARGS__); \
    })

/*
  sizeof (rl_enum_uintXX_t) is determined by alignment attribute, but real size if sizeof (uint8_t) due to attribute packed.
  That's why we can't compare results by MEMCMP macro, which uses object size.
 */
#define CMP_ENUMS(X, ...) (*(X) != *(__VA_ARGS__))
#define CMP_STRUCT_ENUMS(X, ...) ((X)->x != (__VA_ARGS__)->x)
  
RL_START_TEST(zero_rl_enum_t, "zero as number enum") {
  ALL_METHODS (ASSERT_SAVE_LOAD_ENUM, 0, CMP_ENUMS);
  ALL_METHODS (ASSERT_SAVE_LOAD_STRUCT_ENUM, 0, CMP_STRUCT_ENUMS);
} END_TEST

RL_START_TEST(invalid_rl_enum_t, "invalid enum") {
  int checked = 0;
  int warnings = 0;
  void (*save_msg_handler) (const char*, const char*, int, rl_log_level_t, rl_message_id_t, va_list) = rl_conf.msg_handler;

  void msg_handler (const char * file_name, const char * func_name, int line, rl_log_level_t log_level, rl_message_id_t message_id, va_list args)
  {
    if (RL_MESSAGE_SAVE_ENUM == message_id)
      ++warnings;
  }

#define CMP_ENUMS_(X, ...) ({ ++checked; CMP_ENUMS (X, __VA_ARGS__);})
#define CMP_STRUCT_ENUMS_(X, ...) ({ ++checked; CMP_STRUCT_ENUMS (X, __VA_ARGS__);})
  
  rl_conf.msg_handler = msg_handler;
  ALL_METHODS (ASSERT_SAVE_LOAD_ENUM, -1, CMP_ENUMS_);
  ALL_METHODS (ASSERT_SAVE_LOAD_STRUCT_ENUM, -1, CMP_STRUCT_ENUMS_);
  rl_conf.msg_handler = save_msg_handler;
  
  ck_assert_msg ((checked == warnings), "Save/load of ivnalid enum value didn't produced mathced number of warnings (%d != %d)", checked, warnings);
} END_TEST
