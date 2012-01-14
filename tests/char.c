#include <check.h>
#include <reslib.h>
#include <regression.h>

TYPEDEF_STRUCT (struct_char_t, char x)

RL_START_TEST (all_chars, "run whole charset") {
  int i;
  for (i = 0; i < (1 << 8); ++i)
    {
      char c = i;
      ALL_METHODS (ASSERT_SAVE_LOAD, char, &c, SCALAR_CMP);
      ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_char_t, c, STRUCT_X_CMP);
    }
} END_TEST

MAIN ();
