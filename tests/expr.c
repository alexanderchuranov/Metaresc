#include <complex.h>

#include <check.h>
#include <metaresc.h>
#include <regression.h>

#define ASSERT_LOAD_EXPR(TYPE, VALUE)					\
  ck_assert_msg (((TYPE)(VALUE) == MR_LOAD_CINIT (TYPE, #VALUE)),	\
		 "restored value mismatched original for method CINIT on expression %s", #VALUE); \

START_TEST (parse_bool_true) { ASSERT_LOAD_EXPR (bool, true); } END_TEST
START_TEST (parse_int_zero) { ASSERT_LOAD_EXPR (int, 1); } END_TEST
START_TEST (parse_int_paren) { ASSERT_LOAD_EXPR (int, (1)); } END_TEST
START_TEST (parse_unary_minus) { ASSERT_LOAD_EXPR (int, (1 + -1)); } END_TEST
START_TEST (parse_int_sum) { ASSERT_LOAD_EXPR (int, 1 + 1); } END_TEST
START_TEST (parse_complex_sum) { ASSERT_LOAD_EXPR (complex float, 1 + 1i); } END_TEST
START_TEST (parse_float_op1) { ASSERT_LOAD_EXPR (float, (1. + 1i) / 2); } END_TEST
START_TEST (parse_float_op2) { ASSERT_LOAD_EXPR (float, 3.14159265358979323846 + 2 * 2.7182818284590452354); } END_TEST
START_TEST (parse_bits_op1) { ASSERT_LOAD_EXPR (int, 7 ^ 5 * 8); } END_TEST
START_TEST (parse_bits_op2) { ASSERT_LOAD_EXPR (int, 2 | 5 * 8); } END_TEST

MAIN_TEST_SUITE ((parse_bool_true, "parse bool true = (0)"),
		 (parse_int_zero, "parse int 1"),
		 (parse_int_paren, "parse int x = (1)"),
		 (parse_unary_minus, "parse int x = (1 + -1)"),
		 (parse_int_sum, "parse int x = 1 + 1"),
		 (parse_complex_sum, "parse float complex x = 1 + 1i"),
		 (parse_float_op1, "parse float x = (1. + 1i) / 2"),
		 (parse_float_op2, "parse float x = MR_PI + 2 * MR_E"),
		 (parse_bits_op1, "parse int x = 7 ^ 5 * 8"),
		 (parse_bits_op2, "parse int x = 2 | 5 * 8")
		 );
