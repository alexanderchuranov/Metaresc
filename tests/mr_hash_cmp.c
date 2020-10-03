#include <stdlib.h>

#include <check.h>
#include <metaresc.h>
#include <regression.h>

TYPEDEF_STRUCT (embeded_struct_t,
		int x,
		)

TYPEDEF_UNION (embeded_union_t,
	       int32_t x,
	       float y,
	       )

TYPEDEF_ENUM (_enum_t,
	      ZERO,
	      ONE,
	      TWO,
	      THREE,
	      )

TYPEDEF_FUNC (void, void_func_t, (void))

TYPEDEF_STRUCT (all_t,
		(char *, str),
		(char, static_str, [2]),
		bool _bool,
		char _char,
		int8_t _int8,
		uint8_t _uint8,
		int16_t _int16,
		uint16_t _uint16,
		int32_t _int32,
		uint32_t _uint32,
		int64_t _int64,
		uint64_t _uint64,
		float _float,
		complex float _complex_float,
		double _double,
		complex double _complex_double,
		long double _long_double,
		complex long double _complex_long_double,
		(int, array, [2]),
		(embeded_struct_t, _embeded_struct),
		(embeded_union_t, _embeded_union),
		(_enum_t, _enum),
		BITFIELD (_enum_t, bf1, :2),
		BITFIELD (_enum_t, bf2, :2),
		(void, f, (void)),
		(void_func_t, void_func),
		VOID (int, x),
		)

MR_START_TEST (mr_hash_cmp_equal, "check two identical arrays of strings") {
  char * x[] = { "x", NULL, "y", };
  char * y[] = { "x", NULL, "y", };
  ck_assert_msg (MR_HASH_STRUCT ( , x) == MR_HASH_STRUCT ( , y), "mismatch of hash for identical arrays");
  ck_assert_msg (MR_CMP_STRUCTS ( , x, y) == 0, "wrong equals check for identical arrays");
} END_TEST

MR_START_TEST (mr_hash_cmp_non_equal, "check two identical arrays of strings") {
  char * x[] = { "x", NULL, "y", };
  char * y[] = { "x", NULL, "z", };
  ck_assert_msg (MR_HASH_STRUCT ( , x) != MR_HASH_STRUCT ( , y), "match of hash for non-identical arrays");
  ck_assert_msg (MR_CMP_STRUCTS ( , x, y) != 0, "wrong equals check for non-identical arrays");
} END_TEST

MR_START_TEST (mr_hash_cmp_all_types, "check hash and compare for struct with all elementary types") {
  all_t x, y;
  memset (&x, 0, sizeof (x));
  memset (&y, 0, sizeof (y));
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) == MR_HASH_STRUCT (all_t, &y), "mismatch of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) == 0, "wrong equals check for identical structs");
  
  x.str = "x";
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) != MR_HASH_STRUCT (all_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) != 0, "wrong equals check for non-identical structs");
  y.str = "x";
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) == MR_HASH_STRUCT (all_t, &y), "mismatch of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) == 0, "wrong equals check for identical structs");
  
  x.str = "y";
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) != MR_HASH_STRUCT (all_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) != 0, "wrong equals check for non-identical structs");
  y.str = "y";
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) == MR_HASH_STRUCT (all_t, &y), "mismatch of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) == 0, "wrong equals check for identical structs");
  
  x.static_str[0] = 'x';
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) != MR_HASH_STRUCT (all_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) != 0, "wrong equals check for non-identical structs");
  y.static_str[0] = 'x';
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) == MR_HASH_STRUCT (all_t, &y), "mismatch of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) == 0, "wrong equals check for identical structs");

  x._bool = true;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) != MR_HASH_STRUCT (all_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) != 0, "wrong equals check for non-identical structs");
  y._bool = true;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) == MR_HASH_STRUCT (all_t, &y), "mismatch of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) == 0, "wrong equals check for identical structs");

  x._char = 'x';
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) != MR_HASH_STRUCT (all_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) != 0, "wrong equals check for non-identical structs");
  y._char = 'x';
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) == MR_HASH_STRUCT (all_t, &y), "mismatch of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) == 0, "wrong equals check for identical structs");

  x._int8 = 1;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) != MR_HASH_STRUCT (all_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) != 0, "wrong equals check for non-identical structs");
  y._int8 = 1;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) == MR_HASH_STRUCT (all_t, &y), "mismatch of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) == 0, "wrong equals check for identical structs");

  x._uint8 = 1;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) != MR_HASH_STRUCT (all_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) != 0, "wrong equals check for non-identical structs");
  y._uint8 = 1;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) == MR_HASH_STRUCT (all_t, &y), "mismatch of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) == 0, "wrong equals check for identical structs");

  x._int16 = 1;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) != MR_HASH_STRUCT (all_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) != 0, "wrong equals check for non-identical structs");
  y._int16 = 1;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) == MR_HASH_STRUCT (all_t, &y), "mismatch of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) == 0, "wrong equals check for identical structs");

  x._uint16 = 1;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) != MR_HASH_STRUCT (all_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) != 0, "wrong equals check for non-identical structs");
  y._uint16 = 1;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) == MR_HASH_STRUCT (all_t, &y), "mismatch of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) == 0, "wrong equals check for identical structs");

  x._int32 = 1;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) != MR_HASH_STRUCT (all_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) != 0, "wrong equals check for non-identical structs");
  y._int32 = 1;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) == MR_HASH_STRUCT (all_t, &y), "mismatch of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) == 0, "wrong equals check for identical structs");

  x._uint32 = 1;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) != MR_HASH_STRUCT (all_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) != 0, "wrong equals check for non-identical structs");
  y._uint32 = 1;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) == MR_HASH_STRUCT (all_t, &y), "mismatch of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) == 0, "wrong equals check for identical structs");

  x._int64 = 1;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) != MR_HASH_STRUCT (all_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) != 0, "wrong equals check for non-identical structs");
  y._int64 = 1;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) == MR_HASH_STRUCT (all_t, &y), "mismatch of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) == 0, "wrong equals check for identical structs");

  x._uint64 = 1;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) != MR_HASH_STRUCT (all_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) != 0, "wrong equals check for non-identical structs");
  y._uint64 = 1;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) == MR_HASH_STRUCT (all_t, &y), "mismatch of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) == 0, "wrong equals check for identical structs");

  x._float = 1;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) != MR_HASH_STRUCT (all_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) != 0, "wrong equals check for non-identical structs");
  y._float = 1;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) == MR_HASH_STRUCT (all_t, &y), "mismatch of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) == 0, "wrong equals check for identical structs");

  x._complex_float = 1 + I;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) != MR_HASH_STRUCT (all_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) != 0, "wrong equals check for non-identical structs");
  y._complex_float = 1 + I;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) == MR_HASH_STRUCT (all_t, &y), "mismatch of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) == 0, "wrong equals check for identical structs");

  x._double = 1;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) != MR_HASH_STRUCT (all_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) != 0, "wrong equals check for non-identical structs");
  y._double = 1;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) == MR_HASH_STRUCT (all_t, &y), "mismatch of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) == 0, "wrong equals check for identical structs");

  x._complex_double = 1 + I;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) != MR_HASH_STRUCT (all_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) != 0, "wrong equals check for non-identical structs");
  y._complex_double = 1 + I;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) == MR_HASH_STRUCT (all_t, &y), "mismatch of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) == 0, "wrong equals check for identical structs");

  x._long_double = 1;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) != MR_HASH_STRUCT (all_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) != 0, "wrong equals check for non-identical structs");
  y._long_double = 1;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) == MR_HASH_STRUCT (all_t, &y), "mismatch of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) == 0, "wrong equals check for identical structs");

  x._complex_long_double = 1 + I;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) != MR_HASH_STRUCT (all_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) != 0, "wrong equals check for non-identical structs");
  y._complex_long_double = 1 + I;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) == MR_HASH_STRUCT (all_t, &y), "mismatch of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) == 0, "wrong equals check for identical structs");

  x.array[1] = 1;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) != MR_HASH_STRUCT (all_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) != 0, "wrong equals check for non-identical structs");
  y.array[1] = 1;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) == MR_HASH_STRUCT (all_t, &y), "mismatch of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) == 0, "wrong equals check for identical structs");

  x._embeded_struct.x = 1;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) != MR_HASH_STRUCT (all_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) != 0, "wrong equals check for non-identical structs");
  y._embeded_struct.x = 1;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) == MR_HASH_STRUCT (all_t, &y), "mismatch of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) == 0, "wrong equals check for identical structs");

  x._embeded_union.y = 1;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) != MR_HASH_STRUCT (all_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) != 0, "wrong equals check for non-identical structs");
  y._embeded_union.y = 1;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) == MR_HASH_STRUCT (all_t, &y), "mismatch of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) == 0, "wrong equals check for identical structs");

  x._enum = ONE;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) != MR_HASH_STRUCT (all_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) != 0, "wrong equals check for non-identical structs");
  y._enum = ONE;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) == MR_HASH_STRUCT (all_t, &y), "mismatch of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) == 0, "wrong equals check for identical structs");

  x.bf1 = THREE;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) != MR_HASH_STRUCT (all_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) != 0, "wrong equals check for non-identical structs");
  y.bf1 = THREE;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) == MR_HASH_STRUCT (all_t, &y), "mismatch of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) == 0, "wrong equals check for identical structs");

  x.bf2 = TWO;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) != MR_HASH_STRUCT (all_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) != 0, "wrong equals check for non-identical structs");
  y.bf2 = TWO;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) == MR_HASH_STRUCT (all_t, &y), "mismatch of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) == 0, "wrong equals check for identical structs");

  x.f = (void*)"";
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) != MR_HASH_STRUCT (all_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) != 0, "wrong equals check for non-identical structs");
  y.f = x.f;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) == MR_HASH_STRUCT (all_t, &y), "mismatch of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) == 0, "wrong equals check for identical structs");

  x.void_func = (void*)"";
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) != MR_HASH_STRUCT (all_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) != 0, "wrong equals check for non-identical structs");
  y.void_func = x.void_func;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) == MR_HASH_STRUCT (all_t, &y), "mismatch of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) == 0, "wrong equals check for identical structs");

  x.x = 1;
  ck_assert_msg (MR_HASH_STRUCT (all_t, &x) == MR_HASH_STRUCT (all_t, &y), "mismatch of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (all_t, &x, &y) == 0, "wrong equals check for identical structs");
} END_TEST

MR_START_TEST (mr_hash_cmp_mr_conf, "Check hash and generic compare function on MR_COPY_RECURSIVELY") {
  mr_conf_t _mr_conf = MR_COPY_RECURSIVELY (mr_conf_t, &mr_conf);
  ck_assert_msg (MR_HASH_STRUCT (mr_conf_t, &mr_conf) == MR_HASH_STRUCT (mr_conf_t, &_mr_conf), "mismatch of hash for mr_conf copy");
  ck_assert_msg (MR_CMP_STRUCTS (mr_conf_t, &mr_conf, &_mr_conf) == 0, "wrong equals check for mr_conf copy");
  MR_FREE_RECURSIVELY (mr_conf_t, &_mr_conf);
} END_TEST

#undef MR_MODE
#define MR_MODE DESC
TYPEDEF_STRUCT (div_t, quot, rem)

MR_START_TEST (mr_hash_cmp_great_or_less, "Check that compare works correctly") {
  div_t x, y;
  memset (&x, 0, sizeof (x));
  memset (&y, 0, sizeof (y));
  ck_assert_msg (MR_HASH_STRUCT (div_t, &x) == MR_HASH_STRUCT (div_t, &y), "match of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (div_t, &x, &y) == 0, "equals check for identical structs");
  x.quot = 1;
  ck_assert_msg (MR_HASH_STRUCT (div_t, &x) != MR_HASH_STRUCT (div_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (div_t, &x, &y) > 0, "compare check for non-identical structs");
  x.quot = -1;
  ck_assert_msg (MR_HASH_STRUCT (div_t, &x) != MR_HASH_STRUCT (div_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (div_t, &x, &y) < 0, "compare check for non-identical structs");
  y.quot = -1;
  ck_assert_msg (MR_HASH_STRUCT (div_t, &x) == MR_HASH_STRUCT (div_t, &y), "match of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (div_t, &x, &y) == 0, "compare check for identical structs");
  x.rem = 1;
  ck_assert_msg (MR_HASH_STRUCT (div_t, &x) != MR_HASH_STRUCT (div_t, &y), "match of hash for non-identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (div_t, &x, &y) > 0, "compare check for non-identical structs");
  x.rem = -1;
  ck_assert_msg (MR_HASH_STRUCT (div_t, &x) != MR_HASH_STRUCT (div_t, &y), "match of hash for identical structs");
  ck_assert_msg (MR_CMP_STRUCTS (div_t, &x, &y) < 0, "compare check for non-identical structs");
} END_TEST

MAIN ();
