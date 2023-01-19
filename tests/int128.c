#include <check.h>
#include <metaresc.h>
#include <regression.h>

#include <int.h>

#define int128_t mr_int128_t
#define uint128_t mr_uint128_t

TYPEDEF_STRUCT (struct_uint128_t, (uint128_t, x));
TYPEDEF_STRUCT (struct_int128_t, (int128_t, x));

START_TEST (zero_u_int128_t) { ALL_METHODS (ASSERT_SAVE_LOAD_INT, 128, 0); } END_TEST
START_TEST (all_bits_u_int128_t) { ALL_METHODS (ASSERT_SAVE_LOAD_INT, 128, -1); } END_TEST
START_TEST (max_signed_u_int128_t) { ALL_METHODS (ASSERT_SAVE_LOAD_INT, 128, (((uint128_t)1) << (128 - 1)) - 1); } END_TEST
START_TEST (min_signed_u_int128_t) { ALL_METHODS (ASSERT_SAVE_LOAD_INT, 128, (((uint128_t)1) << (128 - 1))); } END_TEST
START_TEST (random_u_int128_t) { ALL_METHODS (ASSERT_SAVE_LOAD_INT, 128, 0x123456789abcdef0LL); } END_TEST

MAIN_TEST_SUITE ((zero_u_int128_t, "zero [u]int128_t"),
		 (all_bits_u_int128_t, "all bits set [u]int128_t"),
		 (max_signed_u_int128_t, "max signed [u]int128_t"),
		 (min_signed_u_int128_t, "min signed [u]int128_t"),
		 (random_u_int128_t, "random [u]int128_t")
		 );
