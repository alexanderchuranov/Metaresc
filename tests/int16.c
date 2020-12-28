#include <check.h>
#include <metaresc.h>
#include <regression.h>

#include <int.h>

TYPEDEF_STRUCT (struct_uint16_t, uint16_t x)
TYPEDEF_STRUCT (struct_int16_t, int16_t x)

START_TEST (zero_u_int16_t) { ALL_METHODS (ASSERT_SAVE_LOAD_INT, 16, 0); } END_TEST
START_TEST (all_bits_u_int16_t) { ALL_METHODS (ASSERT_SAVE_LOAD_INT, 16, -1); } END_TEST
START_TEST (max_signed_u_int16_t) { ALL_METHODS (ASSERT_SAVE_LOAD_INT, 16, (1ULL << (16 - 1)) - 1); } END_TEST
START_TEST (min_signed_u_int16_t) { ALL_METHODS (ASSERT_SAVE_LOAD_INT, 16, (1ULL << (16 - 1))); } END_TEST
START_TEST (random_u_int16_t) { ALL_METHODS (ASSERT_SAVE_LOAD_INT, 16, 0x1234); } END_TEST

MAIN_TEST_SUITE ((zero_u_int16_t, "zero [u]int16_t"),
		 (all_bits_u_int16_t, "all bits set [u]int16_t"),
		 (max_signed_u_int16_t, "max signed [u]int16_t"),
		 (min_signed_u_int16_t, "min signed [u]int16_t"),
		 (random_u_int16_t, "random [u]int16_t")
		 );
