#include <check.h>
#include <metaresc.h>
#include <regression.h>

#include <int.h>

TYPEDEF_STRUCT (struct_uint8_t, uint8_t x)
TYPEDEF_STRUCT (struct_int8_t, int8_t x)

MR_START_TEST (zero_u_int8_t, "zero [u]int8_t") { ALL_METHODS (ASSERT_SAVE_LOAD_INT, 8, 0); } END_TEST
MR_START_TEST (all_bits_u_int8_t, "all bits set [u]int8_t") { ALL_METHODS (ASSERT_SAVE_LOAD_INT, 8, -1); } END_TEST
MR_START_TEST (max_signed_u_int8_t, "max signed [u]int8_t") { ALL_METHODS (ASSERT_SAVE_LOAD_INT, 8, (1ULL << (8 - 1)) - 1); } END_TEST
MR_START_TEST (min_signed_u_int8_t, "min signed [u]int8_t") { ALL_METHODS (ASSERT_SAVE_LOAD_INT, 8, (1ULL << (8 - 1))); } END_TEST
MR_START_TEST (random_u_int8_t, "random [u]int8_t") { ALL_METHODS (ASSERT_SAVE_LOAD_INT, 8, 0x12); } END_TEST

MAIN ();
