#include <math.h>
#include <check.h>
#include <reslib.h>
#include <regression.h>

#include <union.h>

#define ASSERT_SAVE_LOAD_RL_PTR(METHOD, TYPE, VALUE, ...) ({		\
      struct_rl_ptr_t x = { { (TYPE[]){ { { M_PI }, VALUE } } }, #TYPE }; \
      ASSERT_SAVE_LOAD (METHOD, struct_rl_ptr_t, &x, __VA_ARGS__);	\
    })

RL_START_TEST (union_rl_ptr_int_1, "rl_ptr_t is a pointer on any ResLib type") {
  ALL_METHODS (ASSERT_SAVE_LOAD_RL_PTR, struct_union_int8_t, 1);
  ALL_METHODS (ASSERT_SAVE_LOAD_RL_PTR, struct_union_int16_t, 1);
  ALL_METHODS (ASSERT_SAVE_LOAD_RL_PTR, struct_union_int32_t, 1);
  ALL_METHODS (ASSERT_SAVE_LOAD_RL_PTR, struct_union_int64_t, 1);
} END_TEST

RL_START_TEST (union_rl_ptr_enum_int32, "rl_ptr_t is a pointer on any ResLib type") {
  ALL_METHODS (ASSERT_SAVE_LOAD_RL_PTR, struct_union_enum8_t, U8D_INT32);
  ALL_METHODS (ASSERT_SAVE_LOAD_RL_PTR, struct_union_enum16_t, U16D_INT32);
  ALL_METHODS (ASSERT_SAVE_LOAD_RL_PTR, struct_union_enum32_t, U32D_INT32);
  ALL_METHODS (ASSERT_SAVE_LOAD_RL_PTR, struct_union_enum64_t, U64D_INT32);
  ALL_METHODS (ASSERT_SAVE_LOAD_RL_PTR, struct_union_enum_t, UD_INT32);
} END_TEST

RL_START_TEST (union_rl_ptr_int_ptr_1, "rl_ptr_t is a pointer on any ResLib type") {
  ALL_METHODS (ASSERT_SAVE_LOAD_RL_PTR, struct_union_int8_ptr_t, (int8_t[]){ 1 });
  ALL_METHODS (ASSERT_SAVE_LOAD_RL_PTR, struct_union_int16_ptr_t, (int16_t[]){ 1 });
  ALL_METHODS (ASSERT_SAVE_LOAD_RL_PTR, struct_union_int32_ptr_t, (int32_t[]){ 1 });
  ALL_METHODS (ASSERT_SAVE_LOAD_RL_PTR, struct_union_int64_ptr_t, (int64_t[]){ 1 });
  ALL_METHODS (ASSERT_SAVE_LOAD_RL_PTR, struct_union_enum_ptr_t, (enum_discriminator_t[]){ UD_INT32 });
} END_TEST

RL_START_TEST (union_rl_ptr_string_y, "rl_ptr_t is a pointer on any ResLib type") {
  ALL_METHODS (ASSERT_SAVE_LOAD_RL_PTR, struct_union_string_t, "y");
  ALL_METHODS (ASSERT_SAVE_LOAD_RL_PTR, struct_union_ca_t, "y");
  ALL_METHODS (ASSERT_SAVE_LOAD_RL_PTR, struct_union_string_ptr_t, (string_t[]){ "y" });
  ALL_METHODS (ASSERT_SAVE_LOAD_RL_PTR, struct_union_ca_ptr_t, (ca2_t[]){ "y" });
} END_TEST

RL_START_TEST (union_rl_ptr_string, "rl_ptr_t is a pointer on any ResLib type") {
  struct_rl_ptr_t x = { { "string_t" }, "string_t" };	
  ALL_METHODS (ASSERT_SAVE_LOAD, struct_rl_ptr_t, &x);
  x.ptr.ptr = "x";
  ALL_METHODS (ASSERT_SAVE_LOAD, struct_rl_ptr_t, &x);	
} END_TEST

MAIN ();
