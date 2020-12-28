#include <check.h>
#include <metaresc.h>
#include <regression.h>

TYPEDEF_STRUCT (int8_ptr_t, (int8_t *, x))
TYPEDEF_STRUCT (uint8_ptr_t, (uint8_t *, x))
TYPEDEF_STRUCT (int16_ptr_t, (int16_t *, x))
TYPEDEF_STRUCT (uint16_ptr_t, (uint16_t *, x))
TYPEDEF_STRUCT (int32_ptr_t, (int32_t *, x))
TYPEDEF_STRUCT (uint32_ptr_t, (uint32_t *, x))
TYPEDEF_STRUCT (int64_ptr_t, (int64_t *, x))
TYPEDEF_STRUCT (uint64_ptr_t, (uint64_t *, x))

START_TEST (int8_ptr) {
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, int8_ptr_t, NULL);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, int8_ptr_t, (int8_t[]){ 0 });
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, int8_ptr_t, (int8_t[]){ -1 });
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, int8_ptr_t, (int8_t[]){ 1 });
} END_TEST

START_TEST (uint8_ptr) {
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, uint8_ptr_t, NULL);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, uint8_ptr_t, (uint8_t[]){ 0 });
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, uint8_ptr_t, (uint8_t[]){ -1 });
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, uint8_ptr_t, (uint8_t[]){ 1 });
} END_TEST

START_TEST (int16_ptr) {
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, int16_ptr_t, NULL);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, int16_ptr_t, (int16_t[]){ 0 });
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, int16_ptr_t, (int16_t[]){ -1 });
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, int16_ptr_t, (int16_t[]){ 1 });
} END_TEST

START_TEST (uint16_ptr) {
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, uint16_ptr_t, NULL);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, uint16_ptr_t, (uint16_t[]){ 0 });
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, uint16_ptr_t, (uint16_t[]){ -1 });
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, uint16_ptr_t, (uint16_t[]){ 1 });
} END_TEST

START_TEST (int32_ptr) {
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, int32_ptr_t, NULL);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, int32_ptr_t, (int32_t[]){ 0 });
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, int32_ptr_t, (int32_t[]){ -1 });
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, int32_ptr_t, (int32_t[]){ 1 });
} END_TEST

START_TEST (uint32_ptr) {
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, uint32_ptr_t, NULL);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, uint32_ptr_t, (uint32_t[]){ 0 });
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, uint32_ptr_t, (uint32_t[]){ -1 });
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, uint32_ptr_t, (uint32_t[]){ 1 });
} END_TEST

START_TEST (int64_ptr) {
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, int64_ptr_t, NULL);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, int64_ptr_t, (int64_t[]){ 0 });
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, int64_ptr_t, (int64_t[]){ -1 });
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, int64_ptr_t, (int64_t[]){ 1 });
} END_TEST

START_TEST (uint64_ptr) {
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, uint64_ptr_t, NULL);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, uint64_ptr_t, (uint64_t[]){ 0 });
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, uint64_ptr_t, (uint64_t[]){ -1 });
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, uint64_ptr_t, (uint64_t[]){ 1 });
} END_TEST

MAIN_TEST_SUITE ((int8_ptr, "pointer on int"),
		 (uint8_ptr, "pointer on uint"),
		 (int16_ptr, "pointer on int"),
		 (uint16_ptr, "pointer on uint"),
		 (int32_ptr, "pointer on int"),
		 (uint32_ptr, "pointer on uint"),
		 (int64_ptr, "pointer on int"),
		 (uint64_ptr, "pointer on uint")
		 );
