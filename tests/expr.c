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
                (int, i),
                (double, inf),
                (complex double, nan),
                (bool, t),
                (bool, nil),
                );

START_TEST (parse_keyword_field_names) {
  char text[] = "{ .i = 1, . inf = -1.5, .nan = 2 * I }";
  keyword_fields_t x;
  memset (&x, 0, sizeof (x));
  ck_assert_msg (MR_SUCCESS == MR_LOAD_CINIT (keyword_fields_t, text, &x), "load of %s failed", text);
  ck_assert_msg ((1 == x.i) && (-1.5 == x.inf) && (2 * I == x.nan), "load of %s gave wrong values", text);

  keyword_fields_t orig = { .i = 3, .inf = -1.5, .nan = 1 + 2 * I, .t = true, .nil = true, };
  ALL_METHODS (ASSERT_SAVE_LOAD, keyword_fields_t, &orig);
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
                 (parse_keyword_field_names, "fields named i, inf, nan, t and nil")
                 );
