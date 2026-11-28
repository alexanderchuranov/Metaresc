#include <check.h>
#include <metaresc.h>
#include <regression.h>

/*
  JSON and XML documents are UTF-8. Save writes characters beyond ASCII as they
  are, and load decodes escapes of code points (\uXXXX, &#xXXXX;) into UTF-8.
*/

typedef char utf8_name_t[16];

TYPEDEF_STRUCT (utf8_struct_t,
                string_t text,
                (utf8_name_t, name),
                char c,
                );

#define CAFE "caf\xc3\xa9"
#define EURO "\xe2\x82\xac"
#define SMILE "\xf0\x9f\x98\x80"
#define REPLACEMENT "\xef\xbf\xbd"

START_TEST (utf8_round_trip) {
  utf8_struct_t x = { .text = CAFE " " EURO " " SMILE, .name = CAFE EURO, .c = (char)0xe9 };
  ALL_METHODS (ASSERT_SAVE_LOAD, utf8_struct_t, &x);
} END_TEST

START_TEST (utf8_saved_as_is) {
  utf8_struct_t x = { .text = CAFE " " EURO " " SMILE, .name = CAFE EURO, .c = 'a' };
  char * saved = MR_SAVE_JSON (utf8_struct_t, &x);
  ck_assert_msg ((saved != NULL) && (strstr (saved, x.text) != NULL) && (strstr (saved, x.name) != NULL), "JSON escapes UTF-8:\n%s", saved);
  MR_FREE (saved);
  saved = MR_SAVE_XML1 (utf8_struct_t, &x);
  ck_assert_msg ((saved != NULL) && (strstr (saved, x.text) != NULL) && (strstr (saved, x.name) != NULL), "XML escapes UTF-8:\n%s", saved);
  MR_FREE (saved);
} END_TEST

#define ASSERT_LOAD_TEXT(METHOD, TEXT, EXPECTED) ({                     \
      char text[] = TEXT;                                               \
      utf8_struct_t x;                                                  \
      memset (&x, 0, sizeof (x));                                       \
      ck_assert_msg (MR_SUCCESS == MR_LOAD_ ## METHOD (utf8_struct_t, text, &x), "load of %s failed", TEXT); \
      ck_assert_msg ((x.text != NULL) && (0 == strcmp (x.text, EXPECTED)), "load of %s gave '%s'", TEXT, x.text ? x.text : "(null)"); \
      MR_FREE_RECURSIVELY (utf8_struct_t, &x);                          \
    })

START_TEST (json_load_unicode_escapes) {
  ASSERT_LOAD_TEXT (JSON, "{ \"text\" : \"caf\\u00e9\" }", CAFE);
  ASSERT_LOAD_TEXT (JSON, "{ \"text\" : \"\\u20ac\" }", EURO);
  ASSERT_LOAD_TEXT (JSON, "{ \"text\" : \"\\ud83d\\ude00!\" }", SMILE "!");
  /* a surrogate without its pair is not a character */
  ASSERT_LOAD_TEXT (JSON, "{ \"text\" : \"\\ud83d!\" }", REPLACEMENT "!");
  ASSERT_LOAD_TEXT (JSON, "{ \"text\" : \"\\ude00\\u0041\" }", REPLACEMENT "A");
} END_TEST

START_TEST (xml1_load_character_references) {
  ASSERT_LOAD_TEXT (XML1, "<?xml version=\"1.0\"?><utf8_struct_t><text>caf&#xE9;</text></utf8_struct_t>", CAFE);
  ASSERT_LOAD_TEXT (XML1, "<?xml version=\"1.0\"?><utf8_struct_t><text>&#x20AC;&#x1F600;</text></utf8_struct_t>", EURO SMILE);
  ASSERT_LOAD_TEXT (XML1, "<?xml version=\"1.0\"?><utf8_struct_t><text>caf&#233;&#8364;</text></utf8_struct_t>", CAFE EURO);
  ASSERT_LOAD_TEXT (XML1, "<?xml version=\"1.0\"?><utf8_struct_t><text>&#xD800;&#x110000;</text></utf8_struct_t>", REPLACEMENT REPLACEMENT);
} END_TEST

/*
  A byte that is not a part of a valid UTF-8 sequence is saved as the code
  point of the same value, and loads as the UTF-8 sequence of that code point.
*/
#define ASSERT_INVALID_UTF8_SAVE_LOAD(METHOD) ({                        \
      utf8_struct_t x = { .text = "a\xff" CAFE "\xc3", .name = "b\xfe", .c = 'c' }; \
      utf8_struct_t y;                                                  \
      memset (&y, 0, sizeof (y));                                       \
      char * saved = MR_SAVE_ ## METHOD (utf8_struct_t, &x);            \
      ck_assert_msg (saved != NULL, #METHOD " save of invalid UTF-8 failed"); \
      ck_assert_msg (MR_SUCCESS == MR_LOAD_ ## METHOD (utf8_struct_t, saved, &y), #METHOD " load failed:\n%s", saved); \
      ck_assert_msg ((y.text != NULL) && (0 == strcmp (y.text, "a\xc3\xbf" CAFE "\xc3\x83")), #METHOD " loaded '%s'", y.text); \
      ck_assert_msg (0 == strcmp (y.name, "b\xc3\xbe"), #METHOD " loaded '%s'", y.name); \
      MR_FREE (saved);                                                  \
      MR_FREE_RECURSIVELY (utf8_struct_t, &y);                          \
    })

START_TEST (invalid_utf8_saved_as_code_points) {
  ASSERT_INVALID_UTF8_SAVE_LOAD (JSON);
  ASSERT_INVALID_UTF8_SAVE_LOAD (XML1);
#ifdef HAVE_LIBYAML
  ASSERT_INVALID_UTF8_SAVE_LOAD (YAML);
#endif /* HAVE_LIBYAML */
} END_TEST

MAIN_TEST_SUITE ((utf8_round_trip, "UTF-8 strings, char arrays and chars beyond ASCII are saved and loaded"),
                 (invalid_utf8_saved_as_code_points, "bytes that are not valid UTF-8 are saved as code points"),
                 (utf8_saved_as_is, "JSON and XML save UTF-8 as is"),
                 (json_load_unicode_escapes, "JSON escapes of code points load as UTF-8"),
                 (xml1_load_character_references, "XML character references load as UTF-8")
                 );
