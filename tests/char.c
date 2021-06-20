#include <check.h>
#include <metaresc.h>
#include <regression.h>

TYPEDEF_STRUCT (struct_char_t, char x)

#define SKIP_METHOD_XML2 0

START_TEST (all_chars) {
  int i;
  for (i = 0; i < (1 << __CHAR_BIT__); ++i)
    {
      char c = i;
      ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, char, c);
      ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_char_t, c);
    }
} END_TEST

#undef SKIP_METHOD_XML2

START_TEST (print_chars) {
#ifdef HAVE_LIBXML2
#undef TEST_METHODS
#define TEST_METHODS XML2
  int i;
  for (i = 0; i < (1 << __CHAR_BIT__); ++i)
    {
      char c = i;
      if (isprint (c))
	{
	  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, char, c);
	  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_char_t, c);
	}
    }
#endif
} END_TEST

MAIN_TEST_SUITE ((all_chars, "run whole charset"),
		 (print_chars, "run printable charset for libxml2")
		 );
