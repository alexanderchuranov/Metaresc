#include <complex.h>

#include <check.h>
#include <metaresc.h>
#include <regression.h>

#define ASSERT_LOAD_EXPR(TYPE, VALUE)					\
  ck_assert_msg (((TYPE)(VALUE) == MR_LOAD_CINIT (TYPE, #VALUE)),	\
                 "restored value mismatched original for method CINIT on expression %s", #VALUE); \

START_TEST (parse_bool_true) { ASSERT_LOAD_EXPR (bool, true); } END_TEST
START_TEST (parse_bool_false) { ASSERT_LOAD_EXPR (bool, (false)); } END_TEST
START_TEST (parse_bool_char_plus_num) { ASSERT_LOAD_EXPR (int, ('a' + 1)); } END_TEST
START_TEST (parse_int_zero) { ASSERT_LOAD_EXPR (int, 1); } END_TEST
START_TEST (parse_int_paren) { ASSERT_LOAD_EXPR (int, (1)); } END_TEST
START_TEST (parse_unary_minus) { ASSERT_LOAD_EXPR (int, (1 + -1)); } END_TEST
START_TEST (parse_int_sum) { ASSERT_LOAD_EXPR (int, 1 + 1); } END_TEST
START_TEST (parse_complex_sum) { ASSERT_LOAD_EXPR (complex float, 1 + 1i); } END_TEST
START_TEST (parse_float_op1) { ASSERT_LOAD_EXPR (float, (1. + 1i) / 2); } END_TEST
START_TEST (parse_float_op2) { ASSERT_LOAD_EXPR (float, 3.14159265358979323846 + 2 * 2.7182818284590452354); } END_TEST
START_TEST (parse_bits_op1) { ASSERT_LOAD_EXPR (int, 7 ^ 5 * 8); } END_TEST
START_TEST (parse_bits_op2) { ASSERT_LOAD_EXPR (int, 2 | 5 * 8); } END_TEST

/*
  names of fields are not keywords or numbers, but values are (I in CINIT).
  expr.c is built with -ffinite-math-only, so the values are finite.
*/
TYPEDEF_STRUCT (keyword_fields_t,
                (intptr_t, t),
                (uintptr_t, nil),
                (intptr_t, i),
                (double, inf),
                (complex double, nan),
                );

#define KEYWORD_FIELDS_VAL { . t = 1, . nil = 2, . i = 4, . inf = 5.6, . nan = 7.8I + 9.0, . t = true, . nil = true }

START_TEST (parse_keyword_field_names) {
  char text[] = MR_STRINGIFY_READONLY (KEYWORD_FIELDS_VAL);
  keyword_fields_t x, orig = KEYWORD_FIELDS_VAL;
  memset (&x, 0, sizeof (x));
  ck_assert_msg (MR_SUCCESS == MR_LOAD_CINIT (keyword_fields_t, text, &x), "load of %s failed", text);
  ck_assert_msg ((0 == memcmp (&x, &orig, sizeof (x))), "load of %s gave wrong values", text);
  ALL_METHODS (ASSERT_SAVE_LOAD, keyword_fields_t, &orig);
} END_TEST

/*
  Integer expressions are computed in mr_intmax_t. They wrap around as in two's
  complement instead of signed overflow, which is undefined behavior.
*/
#define MR_INTMAX_BITS (sizeof (mr_intmax_t) * __CHAR_BIT__)
#define MR_INTMAX_MIN ((mr_intmax_t)(((mr_uintmax_t)1) << (MR_INTMAX_BITS - 1)))
#define MR_INTMAX_MAX ((mr_intmax_t)((((mr_uintmax_t)1) << (MR_INTMAX_BITS - 1)) - 1))

static void
assert_intmax_expr (char * max, char * fmt, mr_intmax_t expected)
{
  char text[256];
  mr_intmax_t x = 0;
  snprintf (text, sizeof (text), fmt, max, max);
  ck_assert_msg (MR_SUCCESS == MR_LOAD_CINIT (mr_intmax_t, text, &x), "load of %s failed", text);
  ck_assert_msg (x == expected, "wrong value of %s", text);
}

START_TEST (parse_intmax_wrap_around) {
  mr_intmax_t max_value = MR_INTMAX_MAX;
  char * max = MR_SAVE_CINIT (mr_intmax_t, &max_value);
  ck_assert_ptr_nonnull (max);
  max[strcspn (max, " \n")] = 0;

  assert_intmax_expr (max, "%s", MR_INTMAX_MAX);
  assert_intmax_expr (max, "-%s - 1", MR_INTMAX_MIN);
  assert_intmax_expr (max, "%s + 1", MR_INTMAX_MIN);
  assert_intmax_expr (max, "(-%s - 1) - 1", MR_INTMAX_MAX);
  assert_intmax_expr (max, "%s * 2", -2);
  assert_intmax_expr (max, "-(-%s - 1)", MR_INTMAX_MIN);
  assert_intmax_expr (max, "(-%s - 1) / -1", MR_INTMAX_MIN);
  assert_intmax_expr (max, "(-%s - 1) %% -1", 0);
  assert_intmax_expr (max, "7 / -1", -7);
  assert_intmax_expr (max, "7 %% -3", 1);
  MR_FREE (max);
} END_TEST

MAIN_TEST_SUITE ((parse_bool_true, "parse bool true"),
                 (parse_bool_false, "parse bool (false)"),
                 (parse_bool_char_plus_num, "parse int ('a' + 1)"),
                 (parse_int_zero, "parse int 1"),
                 (parse_int_paren, "parse int x = (1)"),
                 (parse_unary_minus, "parse int x = (1 + -1)"),
                 (parse_int_sum, "parse int x = 1 + 1"),
                 (parse_complex_sum, "parse float complex x = 1 + 1i"),
                 (parse_float_op1, "parse float x = (1. + 1i) / 2"),
                 (parse_float_op2, "parse float x = MR_PI + 2 * MR_E"),
                 (parse_bits_op1, "parse int x = 7 ^ 5 * 8"),
                 (parse_bits_op2, "parse int x = 2 | 5 * 8"),
                 (parse_keyword_field_names, "fields named i, inf, nan, t and nil"),
                 (parse_intmax_wrap_around, "integer expressions wrap around without signed overflow")
                 );
