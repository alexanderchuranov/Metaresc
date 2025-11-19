#include <check.h>
#include <metaresc.h>
#include <regression.h>
#include <mr_config.h>

TYPEDEF_STRUCT (ld_ptr_t, (const volatile long double *, x))

START_TEST (ld_ptr) {
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, ld_ptr_t, NULL);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, ld_ptr_t, (long double[]){ 0 });
} END_TEST

TYPEDEF_ENUM (_enum_t, ZERO, ONE);
TYPEDEF_ENUM (packed_enum_t, ATTRIBUTES (__attribute__ ((packed))), PE_ZERO, PE_ONE);
TYPEDEF_STRUCT (enum_ptr_t, (_enum_t *, x));
TYPEDEF_STRUCT (packed_enum_ptr_t, (packed_enum_t *, x));

START_TEST (enum_ptr) {
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, enum_ptr_t, NULL);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, enum_ptr_t, (_enum_t[]){ ZERO });
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, enum_ptr_t, (_enum_t[]){ ONE });
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, packed_enum_ptr_t, NULL);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, packed_enum_ptr_t, (packed_enum_t[]){ PE_ZERO });
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, packed_enum_ptr_t, (packed_enum_t[]){ PE_ONE });
} END_TEST

static int warnings = 0;
static void
msg_handler (const char * file_name, const char * func_name, int line, mr_log_level_t log_level, mr_message_id_t message_id, va_list args)
{
  if (MR_MESSAGE_SAVE_ENUM == message_id)
    ++warnings;
}

#define SKIP_METHOD_XDR 0

START_TEST (invalid_enum_ptr) {
  int checked = 0;
  mr_msg_handler_t save_msg_handler = mr_conf.msg_handler;

#define CMP_ENUMS(TYPE, X, Y, ...) ({ ++checked; (*((X)->x) != *((Y)->x));})

  mr_conf.msg_handler = msg_handler;
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, enum_ptr_t, (_enum_t[]){ -1 }, CMP_ENUMS);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, packed_enum_ptr_t, (packed_enum_t[]){ -1 }, CMP_ENUMS);
  mr_conf.msg_handler = save_msg_handler;

  ck_assert_msg ((checked == warnings), "Save/load of ivnalid enum value didn't produced mathced number of warnings (%d != %d)", checked, warnings);
} END_TEST

#undef SKIP_METHOD_XDR

TYPEDEF_STRUCT (char_ptr_t, (char *, x, , , .stype.mr_type = MR_TYPE_POINTER));

START_TEST (char_ptr) {
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, char_ptr_t, NULL);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, char_ptr_t, "x");
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, char_ptr_t, (char[]){ 0 });
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, char_ptr_t, (char[]){ 127 });
} END_TEST

TYPEDEF_STRUCT (charp_ptr_t, (char**, x));
TYPEDEF_STRUCT (string_ptr_t, (string_t*, x));

START_TEST (string_ptr) {
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, charp_ptr_t, NULL);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, charp_ptr_t, (char*[]){ "x" });
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, string_ptr_t, NULL);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, string_ptr_t, (string_t[]){ "x" });
} END_TEST

TYPEDEF_STRUCT (struct_ptr_t, (enum_ptr_t*, x));

START_TEST (struct_ptr) {
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_ptr_t, NULL);
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_ptr_t, (enum_ptr_t[]){ { NULL } });
  ALL_METHODS (ASSERT_SAVE_LOAD_TYPE, struct_ptr_t, (enum_ptr_t[]){ { (_enum_t[]){ ZERO } } });
} END_TEST

TYPEDEF_STRUCT (self_ref_ptr_t, (_enum_t *, x), (_enum_t, y));

START_TEST (self_ref_ptr) {
  self_ref_ptr_t x = { (_enum_t[]){ ZERO }, ONE };
  ALL_METHODS (ASSERT_SAVE_LOAD, self_ref_ptr_t, &x);
  x.x = &x.y;
  ALL_METHODS (ASSERT_SAVE_LOAD, self_ref_ptr_t, &x);
} END_TEST

TYPEDEF_STRUCT (self_ref_string_t,
		(char, x, [sizeof ("x")], , .stype.mr_type = MR_TYPE_CHAR_ARRAY, .stype.is_array = false),
		string_t y,
		(string_t *, z),
		mr_ptr_t v);

START_TEST (self_ref_string) {
  self_ref_string_t x = { "x", "y", (string_t[]){ "z" }};
  x.v.ptr = &x.z; /* resolved not-NULL void pointer */
  ALL_METHODS (ASSERT_SAVE_LOAD, self_ref_string_t, &x);
  x.v.ptr = x.z;
  ALL_METHODS (ASSERT_SAVE_LOAD, self_ref_string_t, &x);
  x.v.ptr = *x.z;
  ALL_METHODS (ASSERT_SAVE_LOAD, self_ref_string_t, &x);
  x.v.ptr = "xxx"; /* unresolved not-NULL void pointer */
  ALL_METHODS (ASSERT_SAVE_LOAD, self_ref_string_t, &x);
  x.z = &x.y;
  ALL_METHODS (ASSERT_SAVE_LOAD, self_ref_string_t, &x);
  x.y = x.x;
  ALL_METHODS (ASSERT_SAVE_LOAD, self_ref_string_t, &x);
} END_TEST

TYPEDEF_STRUCT (rtfr_struct_t,
		int x,
		int y);

TYPEDEF_STRUCT (resolve_typed_forward_ref_t,
		(int *, y),
		(rtfr_struct_t *, x),
		);

START_TEST (resolve_typed_forward_ref) {
  resolve_typed_forward_ref_t x = { .x = (rtfr_struct_t[]){ {0, 1} } };
  x.y = &x.x->y;
  ALL_METHODS (ASSERT_SAVE_LOAD, resolve_typed_forward_ref_t, &x);
} END_TEST

TYPEDEF_UNION (int_float_t,
	       int _int,
	       float _float,
	       )

TYPEDEF_STRUCT (typed_union_t,
		string_t type,
		(int_float_t, uif, , "type"),
		)

TYPEDEF_STRUCT (union_resolution_t,
		(typed_union_t *, typed_union),
		string_t type,
		(int_float_t *, uifp, , "type"),
		)

START_TEST (union_resolution_correctness) {
  union_resolution_t union_resolution = {
    .type = "_float",
    .typed_union = (typed_union_t[]){
      {
	.type = "_int",
	.uif = { ._int = 1234567890, },
      },
    },
  };
  union_resolution.uifp = &union_resolution.typed_union->uif;
  
  mr_ptrdes_t expected[] =
    {
      {},
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "union_resolution_t", .name.str = "union_resolution_t" }},
	.mr_type = MR_TYPE_STRUCT,
	.flags = MR_IS_UNNAMED,
	.next = 0,
	.first_child = 2,
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "typed_union_t *", .name.str = "typed_union" }},
	.mr_type = MR_TYPE_POINTER,
	.flags = MR_NO_FLAGS,
	.next = 3,
	.first_child = 6,
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "string_t", .name.str = "type" }},
	.mr_type = MR_TYPE_STRING,
	.flags = MR_NO_FLAGS,
	.next = 5,
	.first_child = 0,
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "string_t", .name.str = "type" }},
	.mr_type = MR_TYPE_CHAR_ARRAY,
	.flags = MR_NO_FLAGS,
	.next = 0,
	.first_child = 0,
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int_float_t *", .name.str = "uifp" }},
	.mr_type = MR_TYPE_POINTER,
	.flags = MR_NO_FLAGS,
	.next = 0,
	.first_child = 11,
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "typed_union_t *", .name.str = "typed_union" }},
	.mr_type = MR_TYPE_STRUCT,
	.flags = MR_IS_UNNAMED,
	.next = 0,
	.first_child = 7,
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "string_t", .name.str = "type" }},
	.mr_type = MR_TYPE_STRING,
	.flags = MR_NO_FLAGS,
	.next = 9,
	.first_child = 0,
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "string_t", .name.str = "type" }},
	.mr_type = MR_TYPE_CHAR_ARRAY,
	.flags = MR_NO_FLAGS,
	.next = 0,
	.first_child = 0,
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int_float_t", .name.str = "uif" }},
	.mr_type = MR_TYPE_UNION,
	.flags = MR_NO_FLAGS,
	.next = 0,
	.first_child = 10,
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int", .name.str = "_int" }},
	.mr_type = MR_TYPE_DETECT (int),
	.flags = MR_NO_FLAGS,
	.next = 0,
	.first_child = 0,
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int_float_t *", .name.str = "uifp" }},
	.mr_type = MR_TYPE_UNION,
	.flags = MR_IS_UNNAMED,
	.next = 0,
	.first_child = 12,
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "float", .name.str = "_float" }},
	.mr_type = MR_TYPE_FLOAT,
	.flags = MR_NO_FLAGS,
	.next = 0,
	.first_child = 0,
      },
    };

  ASSERT_MR_SAVE (union_resolution_t, &union_resolution, expected);
} END_TEST

TYPEDEF_STRUCT (linked_list_ptr_t,
		(struct linked_list_t *, ptr));

TYPEDEF_STRUCT (linked_list_t,
		(linked_list_ptr_t, next));

TYPEDEF_STRUCT (root_struct_t,
		(linked_list_ptr_t *, ll_ptr));

START_TEST (backward_ref_is_a_field) {
  linked_list_t ll;
  root_struct_t root_struct;
  ll.next.ptr = &ll;
  root_struct.ll_ptr = &ll.next;

  mr_ptrdes_t expected[] =
    {
      {},
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "root_struct_t", .name.str = "root_struct_t" }},
	.mr_type = MR_TYPE_STRUCT,
	.flags = MR_IS_UNNAMED,
	.next = 0,
	.first_child = 2
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "linked_list_ptr_t *", .name.str = "ll_ptr" }},
	.mr_type = MR_TYPE_POINTER,
	.next = 0,
	.first_child = 3
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "linked_list_ptr_t *", .name.str = "ll_ptr" }},
	.mr_type = MR_TYPE_STRUCT,
	.flags = MR_IS_UNNAMED,
	.next = 0,
	.first_child = 4
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "struct linked_list_t *", .name.str = "ptr" }},
	.mr_type = MR_TYPE_POINTER,
	.next = 0,
	.first_child = 5
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "struct linked_list_t *", .name.str = "ptr" }},
	.mr_type = MR_TYPE_STRUCT,
	.flags = MR_IS_REFERENCED | MR_IS_UNNAMED,
	.next = 0,
	.first_child = 6
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "linked_list_ptr_t", .name.str = "next" }},
	.mr_type = MR_TYPE_STRUCT,
	.next = 0,
	.first_child = 7
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "struct linked_list_t *", .name.str = "ptr" }},
	.mr_type = MR_TYPE_POINTER,
	.flags = MR_IS_REFERENCE,
	.next = 0,
	.first_child = 5
      }
    };

  ASSERT_MR_SAVE (root_struct_t, &root_struct, expected);
} END_TEST

TYPEDEF_STRUCT (two_dynamic_arrays_t,
		(int *, da1, , "dynamic array one", { .size_field_offset = offsetof (two_dynamic_arrays_t, size1), }, "size_field_offset"),
		(ssize_t, size1),
		(int *, da2, , "dynamic array two", { .size_field_offset = offsetof (two_dynamic_arrays_t, size2), }, "size_field_offset"),
		(ssize_t, size2),
		);

START_TEST (tda_same_ptr_and_size) {
  two_dynamic_arrays_t tda;
  int array[2] = { 1, 2, };

  tda.da1 = array;
  tda.size1 = sizeof (array);
  tda.da2 = array;
  tda.size2 = sizeof (array);

  mr_ptrdes_t expected[] =
  {
    {},
    {
      .fdp = (mr_fd_t[]){{ .stype.type = "two_dynamic_arrays_t", .name.str = "two_dynamic_arrays_t" }},
      .mr_type = MR_TYPE_STRUCT,
      .flags = MR_IS_UNNAMED,
      .next = 0,
      .first_child = 2
    },
    {
      .fdp = (mr_fd_t[]){{ .stype.type = "int *", .name.str = "da1" }},
      .mr_type = MR_TYPE_POINTER,
      .next = 3,
      .first_child = 6
    },
    {
      .fdp = (mr_fd_t[]){{ .stype.type = "ssize_t", .name.str = "size1" }},
      .mr_type = MR_TYPE_DETECT (ssize_t),
      .next = 4,
      .first_child = 0
    },
    {
      .fdp = (mr_fd_t[]){{ .stype.type = "int *", .name.str = "da2" }},
      .mr_type = MR_TYPE_POINTER,
      .flags = MR_IS_REFERENCE,
      .next = 5,
      .first_child = 6
    },
    {
      .fdp = (mr_fd_t[]){{ .stype.type = "ssize_t", .name.str = "size2" }},
      .mr_type = MR_TYPE_DETECT (ssize_t),
      .next = 0,
      .first_child = 0
    },
    {
      .fdp = (mr_fd_t[]){{ .stype.type = "int *", .name.str = "da1" }},
      .mr_type = MR_TYPE_DETECT (int),
      .flags = MR_IS_REFERENCED | MR_IS_UNNAMED,
      .next = 7,
      .first_child = 0
    },
    {
      .fdp = (mr_fd_t[]){{ .stype.type = "int *", .name.str = "da1" }},
      .mr_type = MR_TYPE_DETECT (int),
      .flags = MR_IS_UNNAMED,
      .next = 0,
      .first_child = 0
    }
  };

  ASSERT_MR_SAVE (two_dynamic_arrays_t, &tda, expected);
} END_TEST

START_TEST (tda_same_ptr_and_bigger) {
  two_dynamic_arrays_t tda;
  int array[2] = { 1, 2, };

  tda.da1 = array;
  tda.size1 = sizeof (array);
  tda.da2 = array;
  tda.size2 = sizeof (array[0]);
  
  mr_ptrdes_t expected[] =
    {
      {},
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "two_dynamic_arrays_t", .name.str = "two_dynamic_arrays_t" }},
	.mr_type = MR_TYPE_STRUCT,
	.flags = MR_IS_UNNAMED,
	.next = 0,
	.first_child = 2
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int *", .name.str = "da1" }},
	.mr_type = MR_TYPE_POINTER,
	.next = 3,
	.first_child = 6
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "ssize_t", .name.str = "size1" }},
	.mr_type = MR_TYPE_DETECT (ssize_t),
	.next = 4,
	.first_child = 0
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int *", .name.str = "da2" }},
	.mr_type = MR_TYPE_POINTER,
	.flags = MR_IS_REFERENCE,
	.next = 5,
	.first_child = 6
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "ssize_t", .name.str = "size2" }},
	.mr_type = MR_TYPE_INT64,
	.next = 0,
	.first_child = 0
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int *", .name.str = "da1" }},
	.mr_type = MR_TYPE_DETECT (int),
	.flags = MR_IS_REFERENCED | MR_IS_UNNAMED,
	.next = 7,
	.first_child = 0
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int *", .name.str = "da1" }},
	.mr_type = MR_TYPE_DETECT (int),
	.flags = MR_IS_UNNAMED,
	.next = 0,
	.first_child = 0
      }
    };

  ASSERT_MR_SAVE (two_dynamic_arrays_t, &tda, expected);
} END_TEST

START_TEST (tda_overlapping_1) {
  two_dynamic_arrays_t tda;
  int array[3] = { 1, 2, 3, };

  tda.da1 = &array[1];
  tda.size1 = 2 * sizeof (array[0]);
  tda.da2 = &array[0];
  tda.size2 = 2 * sizeof (array[0]);

  mr_ptrdes_t expected[] =
    {
      {},
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "two_dynamic_arrays_t", .name.str = "two_dynamic_arrays_t" }},
	.mr_type = MR_TYPE_STRUCT,
	.flags = MR_IS_UNNAMED,
	.next = 0,
	.first_child = 2
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int *", .name.str = "da1" }},
	.mr_type = MR_TYPE_POINTER,
	.flags = MR_IS_REFERENCE,
	.next = 3,
	.first_child = 6
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "ssize_t", .name.str = "size1" }},
	.mr_type = MR_TYPE_DETECT (ssize_t),
	.next = 4,
	.first_child = 0
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int *", .name.str = "da2" }},
	.mr_type = MR_TYPE_POINTER,
	.flags = MR_NO_FLAGS,
	.next = 5,
	.first_child = 8
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "ssize_t", .name.str = "size2" }},
	.mr_type = MR_TYPE_DETECT (ssize_t),
	.next = 0,
	.first_child = 0
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int *", .name.str = "da2" }},
	.mr_type = MR_TYPE_DETECT (int),
	.flags = MR_IS_REFERENCED | MR_IS_UNNAMED,
	.next = 7,
	.first_child = 0
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int *", .name.str = "da2" }},
	.mr_type = MR_TYPE_DETECT (int),
	.flags = MR_IS_UNNAMED,
	.next = 0,
	.first_child = 0
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int *", .name.str = "da2" }},
	.mr_type = MR_TYPE_DETECT (int),
	.flags = MR_IS_UNNAMED,
	.next = 6,
	.first_child = 0
      }
    };

  ASSERT_MR_SAVE (two_dynamic_arrays_t, &tda, expected);
} END_TEST

START_TEST (tda_overlapping_2) {
  two_dynamic_arrays_t tda;
  int array[3] = { 1, 2, 3, };

  tda.da1 = &array[0];
  tda.size1 = 2 * sizeof (array[0]);
  tda.da2 = &array[1];
  tda.size2 = 2 * sizeof (array[0]);

  mr_ptrdes_t expected[] =
    {
      {},
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "two_dynamic_arrays_t", .name.str = "two_dynamic_arrays_t" }},
	.mr_type = MR_TYPE_STRUCT,
	.flags = MR_IS_UNNAMED,
	.next = 0,
	.first_child = 2
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int *", .name.str = "da1" }},
	.mr_type = MR_TYPE_POINTER,
	.next = 3,
	.first_child = 6
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "ssize_t", .name.str = "size1" }},
	.mr_type = MR_TYPE_DETECT (ssize_t),
	.next = 4,
	.first_child = 0
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int *", .name.str = "da2" }},
	.mr_type = MR_TYPE_POINTER,
	.flags = MR_IS_REFERENCE,
	.next = 5,
	.first_child = 7
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "ssize_t", .name.str = "size2" }},
	.mr_type = MR_TYPE_DETECT (ssize_t),
	.next = 0,
	.first_child = 0
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int *", .name.str = "da1" }},
	.mr_type = MR_TYPE_DETECT (int),
	.flags = MR_IS_UNNAMED,
	.next = 7,
	.first_child = 0
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int *", .name.str = "da1" }},
	.mr_type = MR_TYPE_DETECT (int),
	.flags = MR_IS_REFERENCED | MR_IS_UNNAMED,
	.next = 8,
	.first_child = 0
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int *", .name.str = "da1" }},
	.mr_type = MR_TYPE_DETECT (int),
	.flags = MR_IS_UNNAMED,
	.next = 0,
	.first_child = 0
      }
    };

  ASSERT_MR_SAVE (two_dynamic_arrays_t, &tda, expected);
} END_TEST

START_TEST (tda_overlapping_3) {
  two_dynamic_arrays_t tda;
  int array[] = { 1, 2, 3, 4, };

  tda.da1 = &array[0];
  tda.size1 = 4 * sizeof (array[0]);
  tda.da2 = &array[1];
  tda.size2 = 2 * sizeof (array[0]);
  
  mr_ptrdes_t expected[] =
    {
      {},
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "two_dynamic_arrays_t", .name.str = "two_dynamic_arrays_t" }},
	.mr_type = MR_TYPE_STRUCT,
	.flags = MR_IS_UNNAMED,
	.next = 0,
	.first_child = 2
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int *", .name.str = "da1" }},
	.mr_type = MR_TYPE_POINTER,
	.next = 3,
	.first_child = 6
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "ssize_t", .name.str = "size1" }},
	.mr_type = MR_TYPE_INT64,
	.next = 4,
	.first_child = 0
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int *", .name.str = "da2" }},
	.mr_type = MR_TYPE_POINTER,
	.flags = MR_IS_REFERENCE,
	.next = 5,
	.first_child = 7
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "ssize_t", .name.str = "size2" }},
	.mr_type = MR_TYPE_DETECT (ssize_t),
	.next = 0,
	.first_child = 0
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int *", .name.str = "da1" }},
	.mr_type = MR_TYPE_DETECT (int),
	.flags = MR_IS_UNNAMED,
	.next = 7,
	.first_child = 0
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int *", .name.str = "da1" }},
	.mr_type = MR_TYPE_DETECT (int),
	.flags = MR_IS_REFERENCED | MR_IS_UNNAMED,
	.next = 8,
	.first_child = 0
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int *", .name.str = "da1" }},
	.mr_type = MR_TYPE_DETECT (int),
	.flags = MR_IS_UNNAMED,
	.next = 9,
	.first_child = 0
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int *", .name.str = "da1" }},
	.mr_type = MR_TYPE_DETECT (int),
	.flags = MR_IS_UNNAMED,
	.next = 0,
	.first_child = 0
      }
    };

  ASSERT_MR_SAVE (two_dynamic_arrays_t, &tda, expected);
} END_TEST

TYPEDEF_STRUCT (int_array_t,
		(int, array, [3]),
		);

TYPEDEF_STRUCT (pointer_to_array_t,
		(int_array_t *, array_ptr),
		(int *, da, , "dynamic array", { .size_field_offset = offsetof (pointer_to_array_t, size), }, "size_field_offset"),
		(ssize_t, size),
		);

START_TEST (pointer_to_array) {
  pointer_to_array_t pointer_to_array;
  int_array_t array = { { 1, 2, 3 }, };
  pointer_to_array.array_ptr = &array;
  pointer_to_array.da = &array.array[1];
  pointer_to_array.size = 2 * sizeof (pointer_to_array.da[0]);

  mr_ptrdes_t expected[] =
    {
      {},
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "pointer_to_array_t", .name.str = "pointer_to_array_t" }},
	.mr_type = MR_TYPE_STRUCT,
	.flags = MR_IS_UNNAMED,
	.next = 0,
	.first_child = 2,
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int_array_t *", .name.str = "array_ptr" }},
	.mr_type = MR_TYPE_POINTER,
	.flags = MR_NO_FLAGS,
	.next = 3,
	.first_child = 5,
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int *", .name.str = "da" }},
	.mr_type = MR_TYPE_POINTER,
	.flags = MR_IS_REFERENCE,
	.next = 4,
	.first_child = 8,
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "ssize_t", .name.str = "size" }},
	.mr_type = MR_TYPE_DETECT (ssize_t),
	.flags = MR_NO_FLAGS,
	.next = 0,
	.first_child = 0,
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int_array_t *", .name.str = "array_ptr" }},
	.mr_type = MR_TYPE_STRUCT,
	.flags = MR_IS_UNNAMED,
	.next = 0,
	.first_child = 6,
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int", .name.str = "array" }},
	.mr_type = MR_TYPE_ARRAY,
	.flags = MR_NO_FLAGS,
	.next = 0,
	.first_child = 7,
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int", .name.str = "array" }},
	.mr_type = MR_TYPE_DETECT (int),
	.flags = MR_IS_UNNAMED,
	.next = 8,
	.first_child = 0,
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int", .name.str = "array" }},
	.mr_type = MR_TYPE_DETECT (int),
	.flags = MR_IS_REFERENCED | MR_IS_UNNAMED,
	.next = 9,
	.first_child = 0,
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int", .name.str = "array" }},
	.mr_type = MR_TYPE_DETECT (int),
	.flags = MR_IS_UNNAMED,
	.next = 0,
	.first_child = 0,
      },
    };
  ASSERT_MR_SAVE (pointer_to_array_t, &pointer_to_array, expected);
} END_TEST

START_TEST (mr_ptr_resolution) {
  int res_array[] = {1, 2};
  mr_res_t res = { 
    .data = { res_array },
    .type = "int",
    .mr_size = sizeof (res_array),
  };
  mr_ptrdes_t expected[] =
    {
      {},
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "mr_res_t", .name.str = "mr_res_t" }},
	.mr_type = MR_TYPE_STRUCT,
	.flags = MR_IS_UNNAMED,
	.next = 0,
	.first_child = 2,
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "char *", .name.str = "type" }},
	.mr_type = MR_TYPE_STRING,
	.flags = MR_NO_FLAGS,
	.next = 4,
	.first_child = 0,
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "char *", .name.str = "type" }},
	.mr_type = MR_TYPE_CHAR_ARRAY,
	.flags = MR_NO_FLAGS,
	.next = 0,
	.first_child = 0,
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "ssize_t", .name.str = "mr_size" }},
	.mr_type = MR_TYPE_DETECT (ssize_t),
	.flags = MR_NO_FLAGS,
	.next = 5,
	.first_child = 0,
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "mr_ptr_t", .name.str = "data" }},
	.mr_type = MR_TYPE_UNION,
	.flags = MR_NO_FLAGS,
	.next = 0,
	.first_child = 6,
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int", .name.str = "int" }},
	.mr_type = MR_TYPE_POINTER,
	.flags = MR_IS_UNNAMED,
	.next = 0,
	.first_child = 7,
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int", .name.str = "int" }},
	.mr_type = MR_TYPE_DETECT (int),
	.flags = MR_IS_UNNAMED,
	.next = 8,
	.first_child = 0,
      },
      {
	.fdp = (mr_fd_t[]){{ .stype.type = "int", .name.str = "int" }},
	.mr_type = MR_TYPE_DETECT (int),
	.flags = MR_IS_UNNAMED,
	.next = 0,
	.first_child = 0,
      },
    };
  ASSERT_MR_SAVE (mr_res_t, &res, expected);
} END_TEST

MAIN_TEST_SUITE ((ld_ptr, "pointer on a long double (type name with spaces)"),
		 (enum_ptr, "pointer on enum"),
		 (invalid_enum_ptr, "pointer on invalid enum"),
		 (char_ptr, "pointer on char"),
		 (string_ptr, "pointer on string"),
		 (struct_ptr, "pointer on struct"),
		 (self_ref_ptr, "self referenced pointer"),
		 (self_ref_string, "self referenced strings"),
		 (resolve_typed_forward_ref, "test of forvard reference resolution"),
		 (union_resolution_correctness, "test correctness of union resolution"),
		 (backward_ref_is_a_field, "Saved pointer is a field in struct"),
		 (tda_same_ptr_and_size, "Two dynamic arrays with same pointers and size"),
		 (tda_same_ptr_and_bigger, "Two dynamic arrays with same pointers and one is bigger"),
		 (tda_overlapping_1, "Two overlaping dynamic arrays. Lower pointer saved first"),
		 (tda_overlapping_2, "Two overlaping dynamic arrays. Lower pointer saved second"),
		 (tda_overlapping_3, "Two overlaping dynamic arrays. First saved pointer is embeded into second one"),
		 (pointer_to_array, "Pointer into the middle of static array loaded first"),
		 (mr_ptr_resolution, "test of mr_ptr_t resolution")
		 );
