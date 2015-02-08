#include <check.h>
#include <metaresc.h>
#include <regression.h>

TYPEDEF_ENUM (packed_enum_t, ATTRIBUTES (__attribute__ ((packed, aligned (sizeof (uint16_t))))), ZERO, ONE, TWO)
TYPEDEF_STRUCT (packed_enum_rarray_t, RARRAY (packed_enum_t, x))
TYPEDEF_STRUCT (string_rarray_t, RARRAY (string_t, x))
TYPEDEF_STRUCT (self_referred_rarray_t, RARRAY (self_referred_rarray_t, x))
TYPEDEF_STRUCT (int_rarray_t, RARRAY (int, x))

MR_START_TEST (rarray_empty, "rarray empty") {
  string_rarray_t orig = {
    .x =
    {
      .data = NULL,
      .size = 0,
      .alloc_size = 0,
    },
  };
  ALL_METHODS (ASSERT_SAVE_LOAD, string_rarray_t, &orig);
} END_TEST

MR_START_TEST (rarray_self_referred, "self referred rarray") {
  self_referred_rarray_t orig = {
    .x =
    {
      .data = (typeof (orig.x.data[0])[]){
	{
	  .x =
	  {
	    .data = NULL,
	    .size = 0,
	    .alloc_size = 0,
	  },
	}
      },
      .size = sizeof (orig.x.data[0]),
      .alloc_size = sizeof (orig.x.data[0]),
    },
  };
  ALL_METHODS (ASSERT_SAVE_LOAD, self_referred_rarray_t, &orig);
  orig.x.data->x.data = &orig;
  orig.x.data->x.size = orig.x.data->x.alloc_size = sizeof (self_referred_rarray_t);
  ALL_METHODS (ASSERT_SAVE_LOAD, self_referred_rarray_t, &orig);
} END_TEST

MR_START_TEST (rarray_ext_eq_ptr_type, "rarray_t .ext = .ptr_type = \"char\"") {
  string_rarray_t orig = {
    .x =
    {
      .data = (typeof (orig.x.data[0])[]){ "0", "1", },
      .size = 2 * sizeof (orig.x.data[0]),
      .alloc_size = 2 * sizeof (orig.x.data[0]),
      .res = { "string_t" },
      .res_type = "char",
    },
  };
  ALL_METHODS (ASSERT_SAVE_LOAD, string_rarray_t, &orig);
} END_TEST

MR_START_TEST (rarray_referenced_content, "referenced content") {
  string_rarray_t orig = {
    .x =
    {
      .data = (typeof (orig.x.data[0])[]){ "string_t", "string_t", },
      .size = 2 * sizeof (orig.x.data[0]),
      .alloc_size = 2 * sizeof (orig.x.data[0]),
      .res = { "string_t" },
      .res_type = "char",
    },
  };
  ALL_METHODS (ASSERT_SAVE_LOAD, string_rarray_t, &orig);
} END_TEST

MR_START_TEST (rarray_referenced_elements, "referenced elements") {
  string_rarray_t orig = {
    .x =
    {
      .data = (typeof (orig.x.data[0])[]){ "0", "1", },
      .size = 2 * sizeof (orig.x.data[0]),
      .alloc_size = 2 * sizeof (orig.x.data[0]),
    },
  };
  orig.x.res.ptr = &orig.x.data[1];
  ALL_METHODS (ASSERT_SAVE_LOAD, string_rarray_t, &orig);
  orig.x.res.ptr = &orig.x.data[0];
  ALL_METHODS (ASSERT_SAVE_LOAD, string_rarray_t, &orig);
} END_TEST

MR_START_TEST (rarray_packed_enum, "packed enum - sizeof != used bytes") {
  packed_enum_rarray_t orig = {
    .x =
    {
      .data = (typeof (orig.x.data[0])[]){ ONE, TWO, },
      .size = 2 * sizeof (orig.x.data[0]), /* sizeof returns more then compiler really initialize */
      .alloc_size = 2 * sizeof (orig.x.data[0]),
    },
  };
  ALL_METHODS (ASSERT_SAVE_LOAD, packed_enum_rarray_t, &orig);
} END_TEST

MR_START_TEST (rarray_opaque_data, "rarray with opaque data") {
  int_rarray_t orig = {
    .x =
    {
      .data = (typeof (orig.x.data[0])[]){ 1, 2 },
      .size = 2 * sizeof (orig.x.data[0]),
      .alloc_size = 2 * sizeof (orig.x.data[0]),
      .res = { NULL },
      .res_type = MR_RARRAY_OPAQUE_DATA_T_STR,
    },
  };
  ALL_METHODS (ASSERT_SAVE_LOAD, int_rarray_t, &orig);
} END_TEST

MAIN ();
