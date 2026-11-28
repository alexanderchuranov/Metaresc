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

MAIN_TEST_SUITE ((utf8_round_trip, "UTF-8 strings, char arrays and chars beyond ASCII are saved and loaded"),
                 (utf8_saved_as_is, "JSON and XML save UTF-8 as is"),
                 (json_load_unicode_escapes, "JSON escapes of code points load as UTF-8"),
                 (xml1_load_character_references, "XML character references load as UTF-8")
                 );
