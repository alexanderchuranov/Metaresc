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

MAIN_TEST_SUITE ((cinit_load_reference_index_is_not_node_index, "size of a reference is checked against the node it resolves to"),
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
