#include <check.h>
#include <reslib.h>
#include <regression.h>

TYPEDEF_STRUCT (struct_rarray_t, RARRAY (string_t, x))

RL_START_TEST(rarray_ext_eq_ptr_type, "rarray_t .ext = .ptr_type = \"string_t\"") {
  struct_rarray_t orig = {
    .x =
    {
      .data = (string_t[]){ "0", "1", },
      .size = 2 * sizeof (string_t),
      .alloc_size = 2 * sizeof (string_t),
      .ext = { "string_t" },
      .ptr_type = "string_t",
    },
  };
  ALL_METHODS (ASSERT_SAVE_LOAD, struct_rarray_t, &orig);
} END_TEST
