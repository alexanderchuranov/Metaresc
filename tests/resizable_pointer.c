#include <check.h>
#include <metaresc.h>
#include <regression.h>

TYPEDEF_STRUCT (resizable_array_t,
		(int *, data, , , { "size" }, "char"),
		int size,
		);

MR_START_TEST (resizable_array, "test pointer as a resizable array") {
  resizable_array_t orig = 
    {
      .data = (typeof (orig.data[0])[]){ -1, 1, },
      .size = 2 * sizeof (orig.data[0]),
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_t, &orig);
} END_TEST

TYPEDEF_STRUCT (resizable_array_without_size_t,
		(int8_t *, data, , , { "size" }, "char"),
		VOID (int, size),
		);

MR_START_TEST (resizable_array_without_size, "test pointer as a resizable array with non-serializable size") {
  resizable_array_without_size_t orig = 
    {
      .data = (typeof (orig.data[0])[]){ -1, 1, },
      .size = 2 * sizeof (orig.data[0]),
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_array_without_size_t, &orig);
} END_TEST

TYPEDEF_STRUCT (resizable_mr_ptr_t,
		(mr_ptr_t, data, , "dynamic_type"),
		int MR_SIZE,
		(char *, dynamic_type),
		);

MR_START_TEST (resizable_mr_ptr, "test mr_ptr_t as a resizable array") {
  resizable_mr_ptr_t orig = 
    {
      .data = { (resizable_mr_ptr_t[]){ { { NULL }, 0, "mr_ptr_t", }, { { NULL }, 0, "string_t", }, } },
      .MR_SIZE = 2 * sizeof (resizable_mr_ptr_t),
      .dynamic_type = "resizable_mr_ptr_t",
    };
  ALL_METHODS (ASSERT_SAVE_LOAD, resizable_mr_ptr_t, &orig);
} END_TEST

MAIN ();
