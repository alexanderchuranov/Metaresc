#include <check.h>
#include <metaresc.h>
#include <regression.h>

TYPEDEF_STRUCT (struct_char_t, char x)

#undef TEST_METHODS
#define TEST_METHODS BISON_FLEX_METHODS XDR_METHODS
MR_START_TEST (all_chars, "run whole charset") {
  int i;
  for (i = 0; i < (1 << __CHAR_BIT__); ++i)
    {
      char c = i;
      ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, char, c, SCALAR_CMP);
      ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_char_t, c, STRUCT_X_CMP);
    }
} END_TEST

#ifdef HAVE_LIBXML2
#undef TEST_METHODS
#define TEST_METHODS LIBXML2_METHODS
MR_START_TEST (print_chars, "run printable charset for libxml2") {
  int i;
  for (i = 0; i < (1 << __CHAR_BIT__); ++i)
    {
      char c = i;
      if (isprint (c))
	{
	  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, char, c, SCALAR_CMP);
	  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_char_t, c, STRUCT_X_CMP);
	}
    }
} END_TEST
#endif

MAIN ();
