#include <check.h>
#include <metaresc.h>
#include <regression.h>

/*
  The type of the data in mr_res_t is defined by the text field `type`, which
  is the discriminator of the union `data`, and the size of the data by
  `mr_size`. The document below is inconsistent in both ways. The size of
  the dynamic array is 1, but the initializer has 8 bytes. And the branch of
  the discriminated union is `uint8_t`, while the data is initialized as an
  array of `uint64_t`. A document like this must be rejected by the loader.
*/

#define MR_RES_CINIT { .type = "uint8_t", .mr_size = 1, .data = { (uint64_t[]){ 0x0123456789ABCDEF } }, }

START_TEST (cinit_load_oversized_resource) {
  char text[] = MR_STRINGIFY_READONLY (MR_RES_CINIT);
  mr_res_t res;
  memset (&res, 0, sizeof (res));

  mr_status_t status = MR_LOAD_CINIT (mr_res_t, text, &res);
  ck_assert_msg (MR_FAILURE == status, "load of a resource with data larger than its size succeeded");

  MR_FREE_RECURSIVELY (mr_res_t, &res);
} END_TEST

/*
  A fixed size array may have a size field in the same way as a pointer.
  The size field limits the number of serialized elements. A serializable
  size field must match the number of loaded elements, and a size field of
  type void is set by the loader.
*/

TYPEDEF_STRUCT (counted_array_t,
                (int32_t, x, [4], "array limited by a count field", { "count" }, "count_field_name"),
                (int, count),
                );

TYPEDEF_STRUCT (sized_array_t,
                (int32_t, x, [4], "array limited by a size field", { "size" }, "size_field_name"),
                (size_t, size),
                );

TYPEDEF_STRUCT (void_counted_array_t,
                (int32_t, x, [4], "array limited by a count field that is not serialized", { "count" }, "count_field_name"),
                VOID (int, count),
                );

START_TEST (sized_array_round_trip) {
  counted_array_t counted = { .x = { 1, 2, 3, 4 }, .count = 2, };
  ALL_METHODS (ASSERT_SAVE_LOAD, counted_array_t, &counted);
  counted.count = 4;
  ALL_METHODS (ASSERT_SAVE_LOAD, counted_array_t, &counted);

  sized_array_t sized = { .x = { 1, 2, 3, 4 }, .size = 3 * sizeof (sized.x[0]), };
  ALL_METHODS (ASSERT_SAVE_LOAD, sized_array_t, &sized);

  void_counted_array_t void_counted = { .x = { 1, 2, 3, 4 }, .count = 3, };
  ALL_METHODS (ASSERT_SAVE_LOAD, void_counted_array_t, &void_counted);
} END_TEST

#define COUNTED_ARRAY_CINIT(COUNT) { .x = { 1, 2, 3 }, .count = COUNT, }

START_TEST (cinit_load_sized_array) {
  char matching[] = MR_STRINGIFY_READONLY (COUNTED_ARRAY_CINIT (3));
  char fewer[] = MR_STRINGIFY_READONLY (COUNTED_ARRAY_CINIT (2));
  char more[] = MR_STRINGIFY_READONLY (COUNTED_ARRAY_CINIT (4));
  counted_array_t counted;

  memset (&counted, 0, sizeof (counted));
  ck_assert_msg (MR_SUCCESS == MR_LOAD_CINIT (counted_array_t, matching, &counted),
                 "load of an array with a matching count field failed");
  memset (&counted, 0, sizeof (counted));
  ck_assert_msg (MR_FAILURE == MR_LOAD_CINIT (counted_array_t, fewer, &counted),
                 "load of an array with more elements than its count field succeeded");
  memset (&counted, 0, sizeof (counted));
  ck_assert_msg (MR_FAILURE == MR_LOAD_CINIT (counted_array_t, more, &counted),
                 "load of an array with fewer elements than its count field succeeded");

  char void_counted_text[] = "{ .x = { 1, 2, 3 } }";
  void_counted_array_t void_counted;
  memset (&void_counted, 0, sizeof (void_counted));
  ck_assert_msg (MR_SUCCESS == MR_LOAD_CINIT (void_counted_array_t, void_counted_text, &void_counted),
                 "load of an array with a void count field failed");
  ck_assert_msg (3 == void_counted.count, "void count field is %d, expected 3", void_counted.count);
} END_TEST

/*
  Elements of arrays and pointers are unions that are discriminated by
  the field of the array or the pointer. The loaded branch must be the one
  that the discriminator selects, in the same way as for a union field.
*/

TYPEDEF_UNION (float_int_t, (float, real), (int32_t, whole));
TYPEDEF_ENUM (float_int_disc_t, (FI_FLOAT, , "real"), (FI_INT, , "whole"));
TYPEDEF_STRUCT (union_field_t, (float_int_t, u, , "disc"), (float_int_disc_t, disc));
TYPEDEF_STRUCT (union_array_t, (float_int_t, arr, [2], "disc"), (float_int_disc_t, disc));
TYPEDEF_STRUCT (union_pointer_t, (float_int_t *, p, , "disc"), (float_int_disc_t, disc));

#define ASSERT_LOAD_STATUS(TYPE, STATUS, ...) ({                        \
      char text[] = MR_STRINGIFY_READONLY (__VA_ARGS__);                \
      TYPE x;                                                           \
      memset (&x, 0, sizeof (x));                                       \
      mr_status_t status = MR_LOAD_CINIT (TYPE, text, &x);              \
      ck_assert_msg (STATUS == status, "load of %s returned %d", text, status); \
      if (MR_SUCCESS == status)                                         \
        MR_FREE_RECURSIVELY (TYPE, &x);                                 \
    })

START_TEST (cinit_load_union_elements_branch) {
  ASSERT_LOAD_STATUS (union_field_t, MR_SUCCESS, { .u = { .real = 1 }, .disc = FI_FLOAT });
  ASSERT_LOAD_STATUS (union_field_t, MR_FAILURE, { .u = { .real = 1 }, .disc = FI_INT });
  ASSERT_LOAD_STATUS (union_array_t, MR_SUCCESS, { .arr = { { .real = 1 }, { .real = 2 } }, .disc = FI_FLOAT });
  ASSERT_LOAD_STATUS (union_array_t, MR_FAILURE, { .arr = { { .real = 1 }, { .real = 2 } }, .disc = FI_INT });
  ASSERT_LOAD_STATUS (union_pointer_t, MR_SUCCESS, { .p = (float_int_t[]){ { .whole = 1 } }, .disc = FI_INT });
  ASSERT_LOAD_STATUS (union_pointer_t, MR_FAILURE, { .p = (float_int_t[]){ { .real = 1 } }, .disc = FI_INT });
} END_TEST

START_TEST (union_elements_round_trip) {
  union_array_t array = { .arr = { { .whole = 1 }, { .whole = 2 } }, .disc = FI_INT, };
  ALL_METHODS (ASSERT_SAVE_LOAD, union_array_t, &array);
  float_int_t value = { .real = 1.5 };
  union_pointer_t pointer = { .p = &value, .disc = FI_FLOAT, };
  ALL_METHODS (ASSERT_SAVE_LOAD, union_pointer_t, &pointer);
} END_TEST

/*
  Rows of a multi dimensional array are parts of a single block, so
  a pointer into a row may refer on data up to the end of the whole array,
  but not further.
*/

TYPEDEF_STRUCT (matrix_ref_t,
                (int32_t, x, [2][3]),
                (int32_t *, p, , "pointer into the matrix", { "n" }, "count_field_name"),
                (int, n),
                );

START_TEST (matrix_cross_row_reference) {
  matrix_ref_t orig = { .x = { { 1, 2, 3 }, { 4, 5, 6 } }, .n = 4, };
  orig.p = &orig.x[0][1];
  ALL_METHODS (ASSERT_SAVE_LOAD, matrix_ref_t, &orig);

  /* x[0][1] is node 5, there are 5 elements up to the end of the matrix */
#define MATRIX_REF_CINIT(N) "{ .x = { { 1, /* ref_idx = 5 */ 2, 3 }, { 4, 5, 6 } }, .p = /* ref = 5 */ NULL, .n = " #N " }"
  char to_the_end[] = MATRIX_REF_CINIT (5);
  char past_the_end[] = MATRIX_REF_CINIT (6);
  matrix_ref_t x;

  memset (&x, 0, sizeof (x));
  ck_assert_msg (MR_SUCCESS == MR_LOAD_CINIT (matrix_ref_t, to_the_end, &x), "reference up to the end of the matrix failed");
  ck_assert_msg (x.p == &x.x[0][1], "reference was not set to x[0][1]");
  memset (&x, 0, sizeof (x));
  ck_assert_msg (MR_FAILURE == MR_LOAD_CINIT (matrix_ref_t, past_the_end, &x), "reference past the end of the matrix succeeded");
} END_TEST

/*
  A reference index of a document is not a node index. A document may give
  any node any reference index, so the size of a reference must be checked
  against the node that the reference resolves to.
*/

TYPEDEF_STRUCT (ref_idx_t,
                (int32_t, big, [16]),
                (int32_t, small),
                (int32_t *, p, , "pointer on small", { "n" }, "count_field_name"),
                (int, n),
                );

START_TEST (cinit_load_reference_index_is_not_node_index) {
  /* node 2 is big, but the reference index 2 is given to small */
#define REF_IDX_CINIT(N) "{ .big = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }, " \
    ".small = /* ref_idx = 2 */ 7, .p = /* ref = 2 */ NULL, .n = " #N " }"
  char one[] = REF_IDX_CINIT (1);
  char sixteen[] = REF_IDX_CINIT (16);
  ref_idx_t x;

  memset (&x, 0, sizeof (x));
  ck_assert_msg (MR_SUCCESS == MR_LOAD_CINIT (ref_idx_t, one, &x), "reference on a single element failed");
  ck_assert_msg (x.p == &x.small, "reference was not set to small");
  memset (&x, 0, sizeof (x));
  ck_assert_msg (MR_FAILURE == MR_LOAD_CINIT (ref_idx_t, sixteen, &x),
                 "reference with the size of big succeeded for small");
} END_TEST

/*
  Size fields may be of any integer type, but the sizes of loaded data are
  of type mr_size_t. A size field must not wrap around or be truncated
  to match the loaded data.
*/

TYPEDEF_STRUCT (wide_count_t,
                (int32_t, arr, [4], "array limited by a 64 bit count field", { "arr_count" }, "count_field_name"),
                (int64_t, arr_count),
                (int32_t *, p, , "pointer with a 64 bit count field", { "p_count" }, "count_field_name"),
                (int64_t, p_count),
                );

START_TEST (cinit_load_wide_count_field) {
  /* 2^32 + 2 is truncated to 2, 2^62 * 4 bytes wraps around to 0 */
  char valid[] = "{ .arr = { 1, 2 }, .arr_count = 2, .p = (int32_t[]){ 1, 2 }, .p_count = 2 }";
  char array_truncated[] = "{ .arr = { 1, 2 }, .arr_count = 4294967298 }";
  char pointer_truncated[] = "{ .p = (int32_t[]){ 1, 2 }, .p_count = 4294967298 }";
  char reference_wrapped[] = "{ .arr = { /* ref_idx = 3 */ 1, 2 }, .arr_count = 2, .p = /* ref = 3 */ NULL, .p_count = 4611686018427387904 }";
  char negative[] = "{ .p = (int32_t[]){ 1, 2 }, .p_count = -2 }";
  wide_count_t x;

  memset (&x, 0, sizeof (x));
  ck_assert_msg (MR_SUCCESS == MR_LOAD_CINIT (wide_count_t, valid, &x), "load of 64 bit count fields failed");
  MR_FREE_RECURSIVELY (wide_count_t, &x);
#define ASSERT_WIDE_COUNT_FAILS(TEXT) ({                                \
      memset (&x, 0, sizeof (x));                                       \
      ck_assert_msg (MR_FAILURE == MR_LOAD_CINIT (wide_count_t, TEXT, &x), "load of %s succeeded", TEXT); \
    })
  ASSERT_WIDE_COUNT_FAILS (array_truncated);
  ASSERT_WIDE_COUNT_FAILS (pointer_truncated);
  ASSERT_WIDE_COUNT_FAILS (reference_wrapped);
  ASSERT_WIDE_COUNT_FAILS (negative);
} END_TEST

/*
  Elements of a pointer on a char array type are fixed size char arrays in
  the block of the pointer. The type needs a descriptor (MR_TYPEDEF_DESC_BI),
  otherwise the size of the elements is not known. A string that is longer
  than an element is truncated as for a char array field. It must not grow
  the element in place: an element after the first one is in the middle of
  the block of the pointer.
*/

typedef char ca4_t[4];
MR_TYPEDEF_DESC_BI (ca4_t);

TYPEDEF_STRUCT (ca_ptr_t,
                (ca4_t *, p, , "pointer on char arrays", { "n" }, "count_field_name"),
                (int, n),
                );

TYPEDEF_STRUCT (ca_ref_t,
                string_t s,
                (ca4_t, a),
                (ca4_t *, p, , "a single char array"),
                );

static mr_status_t
load_ca_ptr (char * text, ca_ptr_t * x)
{
  memset (x, 0, sizeof (*x));
  mr_status_t status = MR_LOAD_CINIT (ca_ptr_t, text, x);
  if (MR_SUCCESS != status)
    memset (x, 0, sizeof (*x));
  return (status);
}

START_TEST (char_array_pointer_round_trip) {
  ca_ptr_t ptr = { .p = (ca4_t[]){ "ab", "cde", "" }, .n = 3, };
  ALL_METHODS (ASSERT_SAVE_LOAD, ca_ptr_t, &ptr);

  ca_ref_t ref = { .s = "abc", .a = "xyz", };
  ref.p = &ref.a;
  ALL_METHODS (ASSERT_SAVE_LOAD, ca_ref_t, &ref);
  ref.p = (ca4_t*)ref.s;
  ALL_METHODS (ASSERT_SAVE_LOAD, ca_ref_t, &ref);
} END_TEST

START_TEST (cinit_load_char_array_pointer) {
  char valid[] = "{ .p = (ca4_t[]){ \"ab\", \"cde\" }, .n = 2 }";
  char second_long[] = "{ .p = (ca4_t[]){ \"ab\", \"abcdefghijklmnop\" }, .n = 2 }";
  char first_long[] = "{ .p = (ca4_t[]){ \"abcdefghijklmnop\", \"ab\" }, .n = 2 }";
  char more[] = "{ .p = (ca4_t[]){ \"ab\", \"cd\" }, .n = 3 }";
  char fewer[] = "{ .p = (ca4_t[]){ \"ab\", \"cd\" }, .n = 1 }";
  ca_ptr_t x;

  ck_assert_msg (MR_SUCCESS == load_ca_ptr (valid, &x), "load of %s failed", valid);
  ck_assert_msg ((0 == strcmp (x.p[0], "ab")) && (0 == strcmp (x.p[1], "cde")), "wrong elements loaded from %s", valid);
  MR_FREE_RECURSIVELY (ca_ptr_t, &x);

  ck_assert_msg (MR_SUCCESS == load_ca_ptr (second_long, &x), "load of %s failed", second_long);
  ck_assert_msg ((0 == strcmp (x.p[0], "ab")) && (0 == memcmp (x.p[1], "abcd", sizeof (x.p[1]))),
                 "second element of %s was not truncated", second_long);
  MR_FREE_RECURSIVELY (ca_ptr_t, &x);

  ck_assert_msg (MR_SUCCESS == load_ca_ptr (first_long, &x), "load of %s failed", first_long);
  ck_assert_msg ((0 == memcmp (x.p[0], "abcd", sizeof (x.p[0]))) && (0 == strcmp (x.p[1], "ab")),
                 "first element of %s was not truncated", first_long);
  MR_FREE_RECURSIVELY (ca_ptr_t, &x);

  ck_assert_msg (MR_FAILURE == load_ca_ptr (more, &x), "load of %s succeeded", more);
  ck_assert_msg (MR_FAILURE == load_ca_ptr (fewer, &x), "load of %s succeeded", fewer);
} END_TEST

START_TEST (cinit_load_char_array_reference) {
  /* the content of a string must have at least the size of an element */
  char content[] = "{ .s = /* ref_idx = 1 */ \"abc\", .p = /* ref_content = 1 */ NULL }";
  char content_short[] = "{ .s = /* ref_idx = 1 */ \"ab\", .p = /* ref_content = 1 */ NULL }";
  /* a pointer on a char array may refer on a char array, but not on a string node */
  char char_array[] = "{ .s = \"\", .a = /* ref_idx = 1 */ \"abc\", .p = /* ref = 1 */ NULL }";
  char string[] = "{ .s = /* ref_idx = 1 */ \"abc\", .p = /* ref = 1 */ NULL }";
  ca_ref_t x;

  memset (&x, 0, sizeof (x));
  ck_assert_msg (MR_SUCCESS == MR_LOAD_CINIT (ca_ref_t, content, &x), "load of %s failed", content);
  ck_assert_msg ((void*)x.p == (void*)x.s, "pointer was not set to the content of the string");
  MR_FREE_RECURSIVELY (ca_ref_t, &x);

  memset (&x, 0, sizeof (x));
  ck_assert_msg (MR_SUCCESS == MR_LOAD_CINIT (ca_ref_t, char_array, &x), "load of %s failed", char_array);
  ck_assert_msg (x.p == &x.a, "pointer was not set to the char array field");
  MR_FREE_RECURSIVELY (ca_ref_t, &x);

  memset (&x, 0, sizeof (x));
  ck_assert_msg (MR_FAILURE == MR_LOAD_CINIT (ca_ref_t, content_short, &x), "load of %s succeeded", content_short);
  memset (&x, 0, sizeof (x));
  ck_assert_msg (MR_FAILURE == MR_LOAD_CINIT (ca_ref_t, string, &x), "load of %s succeeded", string);
} END_TEST

/*
  A reference keeps the reference index of the document in first_child until
  the references are resolved, so only pointers and strings may be references,
  and a reference can not have child nodes. Otherwise the index is taken for
  the index of the first child node, which may be out of the nodes.
*/
#define ASSERT_TEXT_LOAD_STATUS(METHOD, TYPE, STATUS, TEXT) ({          \
      char text[] = TEXT;                                               \
      TYPE x;                                                           \
      memset (&x, 0, sizeof (x));                                       \
      mr_status_t status = MR_LOAD_ ## METHOD (TYPE, text, &x);         \
      ck_assert_msg (STATUS == status, "load of %s returned %d", TEXT, status); \
      if (MR_SUCCESS == status)                                         \
        MR_FREE_RECURSIVELY (TYPE, &x);                                 \
    })

START_TEST (load_reference_on_non_pointer) {
  /* positive control: a pointer may be a reference */
  ASSERT_TEXT_LOAD_STATUS (CINIT, ref_idx_t, MR_SUCCESS, "{ .small = /* ref_idx = 1 */ 7, .p = /* ref = 1 */ NULL, .n = 1 }");

  ASSERT_TEXT_LOAD_STATUS (CINIT, ref_idx_t, MR_FAILURE, "/* ref = 5 */ 4");
  ASSERT_TEXT_LOAD_STATUS (CINIT, ref_idx_t, MR_FAILURE, "/* ref = 9 */ { .small = 1 }");
  ASSERT_TEXT_LOAD_STATUS (CINIT, ref_idx_t, MR_FAILURE, "{ .small = /* ref = 1 */ 7 }");
  ASSERT_TEXT_LOAD_STATUS (CINIT, ca_ptr_t, MR_FAILURE, "{ .p = /* ref = 9 */ (ca4_t[]){ \"ab\" }, .n = 1 }");
  ASSERT_TEXT_LOAD_STATUS (SCM, ref_idx_t, MR_FAILURE, ";(ref . 11)\n()");
  ASSERT_TEXT_LOAD_STATUS (XML1, ref_idx_t, MR_FAILURE, "<?xml version=\"1.0\"?><ref_idx_t ref=\"11\"><small>1</small></ref_idx_t>");
} END_TEST

/*
  CINIT and SCM read the annotation of a reference after the value. A value with
  elements, or a second annotation, would lose its child nodes to the reference index.
*/
START_TEST (load_reference_after_value) {
  ASSERT_TEXT_LOAD_STATUS (SCM, ref_idx_t, MR_SUCCESS, "(\n;(ref_idx . 5)\n(small . 7)\n;(ref . 5)\n(p . nil)\n(n . 1))");
  ASSERT_TEXT_LOAD_STATUS (CINIT, ref_idx_t, MR_FAILURE, "{ .small = /* ref_idx = 1 */ 7, .p = /* ref = 1 */ (int32_t[]){ 5 }, .n = 1 }");
  ASSERT_TEXT_LOAD_STATUS (CINIT, ref_idx_t, MR_FAILURE, "{ .small = /* ref_idx = 1 */ 7, .p = /* ref_content = 1 */ (int32_t[]){ 5 }, .n = 1 }");
  ASSERT_TEXT_LOAD_STATUS (CINIT, ref_idx_t, MR_FAILURE, "{ .small = /* ref_idx = 1 */ 7, .p = /* ref = 1 */ /* ref = 1 */ NULL, .n = 1 }");
  ASSERT_TEXT_LOAD_STATUS (SCM, ref_idx_t, MR_FAILURE, "(\n;(ref_idx . 5)\n(small . 7)\n;(ref . 5)\n(p . (5))\n(n . 1))");
} END_TEST

/* SCM saves a character as \x%02x; a malformed escape keeps the characters that follow it */
START_TEST (scm_load_hex_escape) {
  struct {
    char * text;
    char * expected;
  } cases[] = {
    { "((s . \"\\x41;z\"))", "Az" },
    { "((s . \"\\x41z\"))", "Az" },
    { "((s . \"\\x414;z\"))", "A4;z" },
  };
  int i;
  for (i = 0; i < sizeof (cases) / sizeof (cases[0]); ++i)
    {
      char text[64];
      ca_ref_t x;
      strcpy (text, cases[i].text);
      memset (&x, 0, sizeof (x));
      ck_assert_msg (MR_SUCCESS == MR_LOAD_SCM (ca_ref_t, text, &x), "load of %s failed", cases[i].text);
      ck_assert_msg ((x.s != NULL) && (0 == strcmp (x.s, cases[i].expected)), "load of %s gave '%s'", cases[i].text, x.s ? x.s : "(null)");
      MR_FREE_RECURSIVELY (ca_ref_t, &x);
    }
} END_TEST

/*
  As in C, a field may be initialized more than once and the last value wins.
  But a field that owns memory (a string or a pointer with elements) can not be
  overwritten: the memory would be lost, or freed twice on a failed load.
  Branches of a union share the memory, and a union has only one branch.
*/
TYPEDEF_UNION (int_str_t, (int32_t, whole), (string_t, text));
TYPEDEF_ENUM (int_str_disc_t, (IS_INT, , "whole"), (IS_STR, , "text"));
TYPEDEF_STRUCT (int_str_field_t, (int_str_t, u, , "disc"), (int_str_disc_t, disc));

START_TEST (load_field_twice) {
  ref_idx_t x;
  char scalars[] = "{ .small = 1, .small = 2 }";
  memset (&x, 0, sizeof (x));
  ck_assert_msg (MR_SUCCESS == MR_LOAD_CINIT (ref_idx_t, scalars, &x), "load of %s failed", scalars);
  ck_assert_msg (2 == x.small, "the last value of a field must win");

  ASSERT_TEXT_LOAD_STATUS (CINIT, int_str_field_t, MR_SUCCESS, "{ .u = { .whole = 3 }, .u = { .whole = 4 }, .disc = IS_INT }");
  ASSERT_TEXT_LOAD_STATUS (CINIT, int_str_field_t, MR_FAILURE, "{ .u = { .whole = 3, .text = \"a\" }, .disc = IS_STR }");
  ASSERT_TEXT_LOAD_STATUS (CINIT, ca_ref_t, MR_FAILURE, "{ .s = \"a\", .s = \"b\" }");
  ASSERT_TEXT_LOAD_STATUS (CINIT, ca_ptr_t, MR_FAILURE, "{ .p = (ca4_t[]){ \"a\" }, .p = (ca4_t[]){ \"b\" }, .n = 1 }");
  ASSERT_TEXT_LOAD_STATUS (CINIT, int_str_field_t, MR_FAILURE, "{ .u = { .text = \"a\" }, .u = { .whole = 3 }, .disc = IS_INT }");
  ASSERT_TEXT_LOAD_STATUS (CINIT, int_str_field_t, MR_FAILURE, "{ .u = { .text = \"a\", .whole = 3 }, .disc = IS_INT }");
  ASSERT_TEXT_LOAD_STATUS (SCM, ca_ref_t, MR_FAILURE, "((s . \"a\") (s . \"b\"))");

  /* a reference is set after the document is parsed, and would be freed through the other node */
  ASSERT_TEXT_LOAD_STATUS (CINIT, ca_ref_t, MR_SUCCESS, "{ .s = NULL, .s = /* ref_idx = 1 */ \"abc\", .p = /* ref_content = 1 */ NULL }");
  ASSERT_TEXT_LOAD_STATUS (CINIT, ca_ref_t, MR_FAILURE, "{ .s = /* ref_idx = 1 */ \"abc\", .p = /* ref_content = 1 */ NULL, .p = NULL }");
  ASSERT_TEXT_LOAD_STATUS (CINIT, ref_idx_t, MR_FAILURE, "{ .small = /* ref_idx = 1 */ 7, .p = /* ref = 1 */ NULL, .p = NULL, .n = 1 }");
  ASSERT_TEXT_LOAD_STATUS (JSON, ref_idx_t, MR_FAILURE, "{ \"small\" : 7, \"p\" : 2, \"p\" : null, \"n\" : 1 }");
} END_TEST

/* a numeric escape of XML without the closing ';' is not an escape */
START_TEST (xml1_load_incomplete_escape) {
  char text[] = "<?xml version=\"1.0\"?><ca_ref_t><s>&#x01abcdefghijklmnopqrstuvwxyz</s></ca_ref_t>";
  ca_ref_t x;
  memset (&x, 0, sizeof (x));
  ck_assert_msg (MR_SUCCESS == MR_LOAD_XML1 (ca_ref_t, text, &x), "load of %s failed", text);
  ck_assert_msg (x.s && (0 == strcmp (x.s, "&#x01abcdefghijklmnopqrstuvwxyz")), "wrong string loaded from %s", text);
  MR_FREE_RECURSIVELY (ca_ref_t, &x);
} END_TEST

/*
  CINIT saves a control character as an octal escape of 3 digits, which may be followed by a digit.
  XML can not hold control characters, so the other formats are not checked here.
*/
START_TEST (control_char_before_digit_round_trip) {
  ca_ref_t x = { .s = "\0012\0377", .a = "\0029" };
  ASSERT_SAVE_LOAD (CINIT, ca_ref_t, &x);
} END_TEST

/* a conversion of a floating point value out of the range of an integer is undefined */
START_TEST (cinit_load_float_out_of_int_range) {
  ref_idx_t x;
  char in_range[] = "{ .small = 2.0 }";
  memset (&x, 0, sizeof (x));
  ck_assert_msg (MR_SUCCESS == MR_LOAD_CINIT (ref_idx_t, in_range, &x), "load of %s failed", in_range);
  ck_assert_msg (2 == x.small, "wrong value loaded from %s", in_range);

  ASSERT_TEXT_LOAD_STATUS (CINIT, ref_idx_t, MR_FAILURE, "{ .small = 1e999 }");
  ASSERT_TEXT_LOAD_STATUS (CINIT, ref_idx_t, MR_FAILURE, "{ .small = -1e999 }");
} END_TEST

TYPEDEF_STRUCT (int_range_t,
                (double, d),
                (int64_t, i64),
                (uint8_t, u8),
                (int8_t, i8),
                );

#define TEN_TO_40 "10000000000000000000000000000000000000000"

/* integer literals are not wrapped around, neither in the literal nor in the field */
START_TEST (load_int_literal_out_of_range) {
  int_range_t x;
  char cinit[] = "{ .d = " TEN_TO_40 ", .i64 = 18446744073709551615, .u8 = -1, .i8 = -128 }";
  memset (&x, 0, sizeof (x));
  ck_assert_msg (MR_SUCCESS == MR_LOAD_CINIT (int_range_t, cinit, &x), "load of %s failed", cinit);
  ck_assert_msg ((x.d > 0.999999e40) && (x.d < 1.000001e40), "%s loaded d = %g", cinit, x.d);
  ck_assert_msg ((-1 == x.i64) && (255 == x.u8) && (-128 == x.i8), "wrong values loaded from %s", cinit);

  char json[] = "{ \"d\" : -" TEN_TO_40 " }";
  memset (&x, 0, sizeof (x));
  ck_assert_msg (MR_SUCCESS == MR_LOAD_JSON (int_range_t, json, &x), "load of %s failed", json);
  ck_assert_msg ((x.d < -0.999999e40) && (x.d > -1.000001e40), "%s loaded d = %g", json, x.d);

  char scm[] = "(\n(d . " TEN_TO_40 ")\n)";
  memset (&x, 0, sizeof (x));
  ck_assert_msg (MR_SUCCESS == MR_LOAD_SCM (int_range_t, scm, &x), "load of %s failed", scm);
  ck_assert_msg ((x.d > 0.999999e40) && (x.d < 1.000001e40), "%s loaded d = %g", scm, x.d);

  ASSERT_TEXT_LOAD_STATUS (CINIT, int_range_t, MR_FAILURE, "{ .u8 = 256 }");
  ASSERT_TEXT_LOAD_STATUS (CINIT, int_range_t, MR_FAILURE, "{ .i8 = -129 }");
  ASSERT_TEXT_LOAD_STATUS (CINIT, int_range_t, MR_FAILURE, "{ .i64 = 18446744073709551617 }");
  ASSERT_TEXT_LOAD_STATUS (CINIT, int_range_t, MR_FAILURE, "{ .i64 = " TEN_TO_40 " }");
  ASSERT_TEXT_LOAD_STATUS (JSON, int_range_t, MR_FAILURE, "{ \"i64\" : " TEN_TO_40 " }");
  ASSERT_TEXT_LOAD_STATUS (SCM, int_range_t, MR_FAILURE, "(\n(u8 . 257)\n)");
  ASSERT_TEXT_LOAD_STATUS (XML1, int_range_t, MR_FAILURE, "<?xml version=\"1.0\"?><int_range_t><u8>257</u8></int_range_t>");
  ASSERT_TEXT_LOAD_STATUS (XML1, int_range_t, MR_FAILURE, "<?xml version=\"1.0\"?><int_range_t><i64>" TEN_TO_40 "</i64></int_range_t>");
} END_TEST

/* indexes of references are not cut to 32 bits, and a node is not both kinds of a reference */
START_TEST (load_reference_index_out_of_range) {
  ASSERT_TEXT_LOAD_STATUS (CINIT, ref_idx_t, MR_FAILURE, "{ .small = /* ref_idx = 4294967297 */ 7, .p = /* ref = 1 */ NULL, .n = 1 }");
  ASSERT_TEXT_LOAD_STATUS (CINIT, ref_idx_t, MR_FAILURE, "{ .small = /* ref_idx = 1 */ 7, .p = /* ref = 4294967297 */ NULL, .n = 1 }");
  ASSERT_TEXT_LOAD_STATUS (SCM, ref_idx_t, MR_FAILURE, "(\n;(ref_idx . 4294967301)\n(small . 7)\n;(ref . 5)\n(p . nil)\n(n . 1))");

#define XML1_REF(SMALL_ATTR, P_ATTR) "<?xml version=\"1.0\"?><ref_idx_t><small " SMALL_ATTR ">7</small><p " P_ATTR "/><n>1</n></ref_idx_t>"
  ASSERT_TEXT_LOAD_STATUS (XML1, ref_idx_t, MR_SUCCESS, XML1_REF ("ref_idx=\"2\"", "ref=\"2\""));
  ASSERT_TEXT_LOAD_STATUS (XML1, ref_idx_t, MR_FAILURE, XML1_REF ("ref_idx=\"4294967298\"", "ref=\"2\""));
  ASSERT_TEXT_LOAD_STATUS (XML1, ref_idx_t, MR_FAILURE, XML1_REF ("ref_idx=\"2\"", "ref=\"4294967298\""));
  ASSERT_TEXT_LOAD_STATUS (XML1, ref_idx_t, MR_FAILURE, XML1_REF ("ref_idx=\"2\"", "ref=\"2\" ref_content=\"2\""));
#ifdef HAVE_LIBXML2
  ASSERT_TEXT_LOAD_STATUS (XML2, ref_idx_t, MR_SUCCESS, XML1_REF ("ref_idx=\"2\"", "ref=\"2\""));
  ASSERT_TEXT_LOAD_STATUS (XML2, ref_idx_t, MR_FAILURE, XML1_REF ("ref_idx=\"4294967298\"", "ref=\"2\""));
  ASSERT_TEXT_LOAD_STATUS (XML2, ref_idx_t, MR_FAILURE, XML1_REF ("ref_idx=\"2\"", "ref=\"2\" ref_content=\"2\""));
#endif /* HAVE_LIBXML2 */
#ifdef HAVE_LIBYAML
  ASSERT_TEXT_LOAD_STATUS (YAML, ref_idx_t, MR_SUCCESS, "---\nsmall: &ref_idx_2 7\np: *ref_idx_2\nn: 1\n...\n");
  ASSERT_TEXT_LOAD_STATUS (YAML, ref_idx_t, MR_FAILURE, "---\nsmall: &ref_idx_4294967298 7\np: *ref_idx_2\nn: 1\n...\n");
  ASSERT_TEXT_LOAD_STATUS (YAML, ref_idx_t, MR_FAILURE, "---\nsmall: &ref_idx_2 7\np: *ref_idx_4294967298\nn: 1\n...\n");
#endif /* HAVE_LIBYAML */
} END_TEST

static size_t largest_calloc = 0;
static void * (*saved_calloc) (const char *, const char *, int, size_t, size_t) = NULL;

static void *
recording_calloc (const char * filename, const char * function, int line, size_t count, size_t size)
{
  if (count * size > largest_calloc)
    largest_calloc = count * size;
  return (saved_calloc (filename, function, line, count, size));
}

TYPEDEF_STRUCT (big_ref_node_t, (int32_t, v), (big_ref_node_t *, next));
TYPEDEF_STRUCT (big_ref_t, (big_ref_node_t, a), (big_ref_node_t, b), (big_ref_node_t *, p));

#define ASSERT_BIG_REF_IDX_LOAD(METHOD, TEXT) ({                        \
      char text[] = TEXT;                                               \
      big_ref_t x;                                                      \
      memset (&x, 0, sizeof (x));                                       \
      largest_calloc = 0;                                               \
      saved_calloc = mr_conf.mr_mem.calloc;                             \
      mr_conf.mr_mem.calloc = recording_calloc;                         \
      mr_status_t status = MR_LOAD_ ## METHOD (big_ref_t, text, &x);    \
      mr_conf.mr_mem.calloc = saved_calloc;                             \
      ck_assert_msg (MR_SUCCESS == status, "load of %s failed", TEXT);  \
      ck_assert_msg (&x.a == x.p, "reference of %s is not resolved", TEXT); \
      ck_assert_msg (largest_calloc < (1 << 20), "load of %s allocated %zu bytes", TEXT, largest_calloc); \
    })

/* memory for resolving references depends on the number of nodes, not on the values of reference indexes */
START_TEST (load_big_reference_index) {
  ASSERT_BIG_REF_IDX_LOAD (CINIT, "{ .a = /* ref_idx = 4000000000 */ { .v = 1 }, .p = /* ref = 4000000000 */ NULL }");
#ifdef HAVE_LIBYAML
  ASSERT_BIG_REF_IDX_LOAD (YAML, "---\na: &ref_idx_4000000000\n  v: 1\np: *ref_idx_4000000000\n...\n");
#endif /* HAVE_LIBYAML */
} END_TEST

MAIN_TEST_SUITE ((cinit_load_reference_index_is_not_node_index, "size of a reference is checked against the node it resolves to"),
                 (load_big_reference_index, "a large reference index does not allocate memory in proportion to it"),
                 (load_int_literal_out_of_range, "integers out of the range of the literal or of the field are rejected"),
                 (load_reference_index_out_of_range, "reference indexes out of 32 bits and double references are rejected"),
                 (load_reference_on_non_pointer, "only pointers and strings without child nodes may be references"),
                 (load_reference_after_value, "a reference annotation after a value with child nodes is rejected"),
                 (scm_load_hex_escape, "a malformed hex escape of SCM keeps the following characters"),
                 (load_field_twice,"a field that owns memory can not be loaded twice"),
                 (xml1_load_incomplete_escape, "an incomplete numeric escape of XML is loaded as text"),
                 (control_char_before_digit_round_trip, "a control character followed by a digit is saved and loaded"),
                 (cinit_load_float_out_of_int_range, "a floating point value out of the range of an integer is rejected"),
                 (char_array_pointer_round_trip, "pointers on char array types are saved and loaded"),
                 (cinit_load_char_array_pointer, "elements of pointers on char arrays are fixed size"),
                 (cinit_load_char_array_reference, "references of pointers on char arrays are checked"),
                 (cinit_load_wide_count_field, "size fields are not truncated or wrapped around"),
                 (matrix_cross_row_reference, "pointer into a row of a matrix may refer on the following rows"),
                 (cinit_load_union_elements_branch, "union elements of arrays and pointers must be loaded as the discriminated branch"),
                 (union_elements_round_trip, "union elements of arrays and pointers are saved and loaded"),
                 (cinit_load_oversized_resource, "load of a resource with data larger than its size fails"),
                 (sized_array_round_trip, "arrays limited by a size field are saved and loaded"),
                 (cinit_load_sized_array, "size field of a loaded array must match the number of elements"));
