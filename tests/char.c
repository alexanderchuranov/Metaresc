#include <check.h>
#include <reslib.h>
#include <regression.h>

TYPEDEF_STRUCT (struct_char_t, char x)

#define SKIP_METHOD_XML2 0

RL_START_TEST(all_chars, "run whole charset") {
  int i;
  for (i = 0; i < (1 << 8); ++i)
    {
      char c = i;
      ALL_METHODS (ASSERT_SAVE_LOAD, char, &c, CMP_SCALAR);
      ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_char_t, c, CMP_STRUCT_X);
    }
} END_TEST
