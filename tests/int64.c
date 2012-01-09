#include <check.h>
#include <reslib.h>
#include <regression.h>

#include <int.h>

TYPEDEF_STRUCT (struct_uint64_t, uint64_t x);
TYPEDEF_STRUCT (struct_int64_t, int64_t x);

RL_START_TEST (zero_u_int64_t, "zero [u]int64_t") { ALL_METHODS (ASSERT_SAVE_LOAD_INT, 64, 0); } END_TEST
RL_START_TEST (all_bits_u_int64_t, "all bits set [u]int64_t") { ALL_METHODS (ASSERT_SAVE_LOAD_INT, 64, -1); } END_TEST
RL_START_TEST (max_signed_u_int64_t, "max signed [u]int64_t") { ALL_METHODS (ASSERT_SAVE_LOAD_INT, 64, (1ULL << (64 - 1)) - 1); } END_TEST
RL_START_TEST (min_signed_u_int64_t, "min signed [u]int64_t") { ALL_METHODS (ASSERT_SAVE_LOAD_INT, 64, (1ULL << (64 - 1))); } END_TEST
RL_START_TEST (random_u_int64_t, "random [u]int64_t") { ALL_METHODS (ASSERT_SAVE_LOAD_INT, 64, 0x123456789abcdef0LL); } END_TEST

MAIN ();
