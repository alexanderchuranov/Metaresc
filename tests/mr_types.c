#include <check.h>
#include <metaresc.h>
#include <regression.h>

typedef char char_array_t[1];
typedef char * char_ptr_t;
typedef int32_t int32_alias_t;
typedef int32_t * int32_ptr_t;

TYPEDEF_STRUCT (mr_empty_t);

TYPEDEF_STRUCT (mr_incomplete_t, (int, x, [0]), VOID (int, y, []));

TYPEDEF_ENUM (_enum_t, ATTRIBUTES (__attribute__ ((packed))),
	      (ZERO, = 0),
	      (ONE, = 1 << 0),
	      (TWO, = 1 << 1),
	      (HIGHEST_BIT, = -(1 << (__CHAR_BIT__ - 1))),
	      );

TYPEDEF_STRUCT (embeded_struct_t,
		int x
		);

TYPEDEF_UNION (embeded_union_t,
	       int x,
	       float y,
	       );

TYPEDEF_STRUCT (struct_t,
		bool bool_,
		int8_t int8_,
		uint8_t uint8_,
		int16_t int16_,
		uint16_t uint16_,
		int32_t int32_,
		uint32_t uint32_,
		int64_t int64_,
		uint64_t uint64_,
		(mr_intmax_t, mr_intmax),
		(mr_uintmax_t, mr_uintmax),
		float float_,
		complex float complex_float_,
		double double_,
		complex double complex_double_,
		long double long_double_,
		complex long double complex_long_double_,
		const volatile unsigned long long int const_volatile_unsigned_long_long_int_,
		size_t size,
		ssize_t ssize,
		(char_array_t, char_array_),
		char char_,
		string_t string,
		const string_t const_string,
		volatile string_t volatile_string,
		const volatile string_t const_volatile_string,
		volatile const string_t volatile_const_string,
		(char_ptr_t, char_ptr_),
		(int32_alias_t, int32_alias0),
		(int32_alias_t *, int32_alias1),
		(int32_alias_t, int32_alias2, [1]),
		(int32_alias_t *, int32_alias3, [1]),
		BITFIELD (int32_alias_t, int32_alias4, : 1),
		(int32_ptr_t, int32_ptr),
		(char *, string_),
		(const char *, const_string_),
		(volatile char *, volatile_string_),
		(const volatile char *, const_volatile_string_),
		(volatile const char *, volatile_const_string_),

		(void *, void_ptr_),
		(void const *, void_const_ptr_),
		(void * const, void_ptr_const),
		(void const * const, void_const_ptr_const),
		(const void *, const_void_ptr_),
		(volatile void *, volatile_void_ptr_),
		(const volatile void *, const_volatile_void_ptr_),
		(volatile const void *, volatile_const_void_ptr_),
		
		(float *, float_ptr_),
		(const float * const, const_float_ptr_),
		(volatile float * volatile, volatile_float_ptr_),
		(const volatile float * const volatile, const_volatile_float_ptr_),
		(volatile const float * volatile const, volatile_const_float_ptr_),
		
		(char_array_t *, char_array_ptr_),
		(const char_array_t *, const_char_array_ptr_),
		(volatile char_array_t *, volatile_char_array_ptr_),
		(const volatile char_array_t *, const_volatile_char_array_ptr_),
		(volatile const char_array_t *, volatile_const_char_array_ptr_),

		(char **, string_ptr_),
		(const char **, const_string_ptr_),
		(volatile char **, volatile_string_ptr_),
		(const volatile char **, const_volatile_string_ptr_),
		(volatile const char **, volatile_const_string_ptr_),
		
		(char * const *, string_const_ptr_),
		(char * volatile *, string_volatile_ptr_),
		(char * const volatile *, string_const_volatile_ptr_),
		(char * volatile const *, string_volatile_const_ptr_),
		
		(string_t const *, string_t_const_ptr_),
		(string_t volatile *, string_t_volatile_ptr_),
		(string_t const volatile *, string_t_const_volatile_ptr_),
		(string_t volatile const *, string_t_volatile_const_ptr_),

		(embeded_struct_t, embeded_struct),
		(const volatile struct embeded_struct_t, const_volatile_struct_embeded_struct),
		(embeded_struct_t *, embeded_struct_ptr),
		(const volatile struct embeded_struct_t *, const_volatile_struct_embeded_struct_ptr),

		(embeded_union_t, embeded_union),
		(const volatile union embeded_union_t, const_volatile_union_embeded_union),
		(embeded_union_t *, embeded_union_ptr),
		(const volatile union embeded_union_t *, const_volatile_union_embeded_union_ptr),

		(_enum_t, _enum),
		(const volatile enum _enum_t, const_volatile_enum_enum),
		(_enum_t *, _enum_ptr),
		(const volatile enum _enum_t *, const_volatile_enum_enum_ptr),

		BITFIELD (bool, bf_bool, :1),
		BITFIELD (uint8_t, bf_uint8_t, :__CHAR_BIT__ * sizeof (uint8_t) - 1),
		BITFIELD (int8_t, bf_int8_t, :__CHAR_BIT__ * sizeof (int8_t) - 1),
		BITFIELD (uint16_t, bf_uint16_t, :__CHAR_BIT__ * sizeof (uint16_t) - 1),
		BITFIELD (int16_t, bf_int16_t, :__CHAR_BIT__ * sizeof (int16_t) - 1),
		BITFIELD (uint32_t, bf_uint32_t, :__CHAR_BIT__ * sizeof (uint32_t) - 1),
		BITFIELD (int32_t, bf_int32_t, :__CHAR_BIT__ * sizeof (int32_t) - 1),
		BITFIELD (uint64_t, bf_uint64_t, :__CHAR_BIT__ * sizeof (uint64_t) - 1),
		BITFIELD (int64_t, bf_int64_t, :__CHAR_BIT__ * sizeof (int64_t) - 1),
#ifdef HAVE_INT128
		BITFIELD (mr_uint128_t, bf_uint128_t, :__CHAR_BIT__ * sizeof (mr_uint128_t) - 1),
		BITFIELD (mr_int128_t, bf_int128_t, :__CHAR_BIT__ * sizeof (mr_int128_t) - 1),
#endif /* HAVE_INT128 */
		BITFIELD (mr_type_t, bf_enum, : 6),
		BITFIELD (const volatile enum mr_type_t, bf_const_volatile_enum_enum, : 6),
		(int32_alias_t, f, (volatile int32_alias_t const *, int32_t [2] [3], int32_t * [2] [3], char [2], char [2] [3])),
		);

TYPEDEF_STRUCT (void_function_field_t,
		VOID (void, (*func), (void)));

TYPEDEF_FUNC (int32_alias_t, func_t, (volatile int32_alias_t const *));

START_TEST (check_void_function_field) {
  mr_conf_init ();

  mr_td_t * tdp = mr_get_td_by_name ("void_function_field_t");
  ck_assert_msg (tdp != NULL, "Failed to get type descriptor for type void_function_field_t.");
  mr_fd_t * fdp = mr_get_fd_by_name (tdp, "func");
  ck_assert_msg (fdp != NULL, "Failed to get field descriptor for field 'func'.");
} END_TEST

typedef struct ext_struct_t {
  int16_t x;
  int32_t y;
  uint8_t * z;
  complex float const * const z_c;
  complex double volatile * volatile z_v;
  complex long double const volatile * const volatile z_cv;
  const float * c_z;
  volatile double * v_z;
  const volatile long double * cv_z;
  struct { int _1; } * struct_ptr;
  struct ext_struct_t * self_ptr;
  const struct ext_struct_t * c_self_ptr;
  volatile struct ext_struct_t * v_self_ptr;
  const volatile struct ext_struct_t * cv_self_ptr;
  struct ext_struct_t * const self_ptr_c;
  struct ext_struct_t * volatile self_ptr_v;
  struct ext_struct_t * const volatile self_ptr_cv;
  struct ext_struct_t const * const c_self_ptr_c;
  struct ext_struct_t volatile * volatile v_self_ptr_v;
} ext_struct_t;

typedef enum ext_enum_t {
  _0,
  _1,
  _2,
} ext_enum_t;

#define MR_MODE DESC
TYPEDEF_STRUCT (struct ext_struct_t,
		( , y),
		x,
		z, z_c, z_v, z_cv, c_z, v_z, cv_z,
		struct_ptr,
		self_ptr, c_self_ptr, v_self_ptr, cv_self_ptr, self_ptr_c, self_ptr_v, self_ptr_cv, c_self_ptr_c, v_self_ptr_v,
		);

TYPEDEF_ENUM (enum ext_enum_t, _0, _1);

#define ASSERT_FIELD_TYPE_(TYPE, FIELD, MR_TYPE, MR_TYPE_AUX) ({	\
      mr_td_t * tdp = mr_get_td_by_name (#TYPE);			\
      ck_assert_msg (tdp != NULL, "Failed to get type descriptor for type " #TYPE "."); \
      mr_fd_t * fdp = mr_get_fd_by_name (tdp, #FIELD);			\
      ck_assert_msg (fdp != NULL, "Failed to get field descriptor for field " #FIELD "."); \
      ck_assert_msg (fdp->stype.mr_type == MR_TYPE, "Mismatched mr_type for field " #FIELD " (%d != %d).", \
		     fdp->stype.mr_type, MR_TYPE);				\
      ck_assert_msg (fdp->stype.mr_type_aux == MR_TYPE_AUX, "Mismatched mr_type_aux for field " #FIELD " (%d != %d).", \
		     fdp->stype.mr_type_aux, MR_TYPE_AUX);			\
    })
  
#define ASSERT_FIELD_TYPE(TYPE, FIELD, MR_TYPE, ...)			\
  ASSERT_FIELD_TYPE_ (TYPE, FIELD, MR_TYPE, MR_IF_ELSE (MR_IS_EMPTY (__VA_ARGS__)) (MR_TYPE_NONE) (__VA_ARGS__))

#define ASSERT_STRUCT_FIELD_TYPE(FIELD, MR_TYPE, ...) ASSERT_FIELD_TYPE (struct_t, FIELD, MR_TYPE, __VA_ARGS__)
#define ASSERT_EXT_STRUCT_FIELD_TYPE(FIELD) ASSERT_FIELD_TYPE (ext_struct_t, FIELD, MR_TYPE_DETECT (__typeof__ (((ext_struct_t*)0)->FIELD)))
#define ASSERT_EXT_STRUCT_FIELD_TYPE_PTR(FIELD) ASSERT_FIELD_TYPE (ext_struct_t, FIELD, MR_TYPE_POINTER, MR_TYPE_DETECT_PTR (__typeof__ (((ext_struct_t*)0)->FIELD)))

START_TEST (check_ext_struct) {
  mr_conf_init ();
  
  mr_td_t * tdp = mr_get_td_by_name ("ext_struct_t");
  ck_assert_msg (tdp != NULL, "Failed to get type descriptor for type ext_struct_t.");
  int i;
  for (i = tdp->param.struct_param.fields_size / sizeof (tdp->param.struct_param.fields[0]) - 1; i > 0; --i)
    ck_assert_msg (tdp->param.struct_param.fields[i]->offset > tdp->param.struct_param.fields[i - 1]->offset,
		   "Fields of ext_struct_t are not sorted according to their offsets. '%s' %d < '%s' %d",
		   tdp->param.struct_param.fields[i - 1]->name.str, (int)tdp->param.struct_param.fields[i - 1]->offset,
		   tdp->param.struct_param.fields[i]->name.str, (int)tdp->param.struct_param.fields[i]->offset);
  
  ASSERT_EXT_STRUCT_FIELD_TYPE (x);
  ASSERT_EXT_STRUCT_FIELD_TYPE (y);
  ASSERT_EXT_STRUCT_FIELD_TYPE_PTR (z);
  ASSERT_EXT_STRUCT_FIELD_TYPE_PTR (z_c);
  ASSERT_EXT_STRUCT_FIELD_TYPE_PTR (z_v);
  ASSERT_EXT_STRUCT_FIELD_TYPE_PTR (z_cv);
  ASSERT_EXT_STRUCT_FIELD_TYPE_PTR (c_z);
  ASSERT_EXT_STRUCT_FIELD_TYPE_PTR (v_z);
  ASSERT_EXT_STRUCT_FIELD_TYPE_PTR (cv_z);
  ASSERT_FIELD_TYPE (ext_struct_t, struct_ptr, MR_TYPE_POINTER, MR_TYPE_VOID);
  ASSERT_FIELD_TYPE (ext_struct_t, self_ptr, MR_TYPE_POINTER, MR_TYPE_STRUCT);
  ASSERT_FIELD_TYPE (ext_struct_t, c_self_ptr, MR_TYPE_POINTER, MR_TYPE_STRUCT);
  ASSERT_FIELD_TYPE (ext_struct_t, v_self_ptr, MR_TYPE_POINTER, MR_TYPE_STRUCT);
  ASSERT_FIELD_TYPE (ext_struct_t, cv_self_ptr, MR_TYPE_POINTER, MR_TYPE_STRUCT);
  ASSERT_FIELD_TYPE (ext_struct_t, self_ptr_c, MR_TYPE_POINTER, MR_TYPE_STRUCT);
  ASSERT_FIELD_TYPE (ext_struct_t, self_ptr_v, MR_TYPE_POINTER, MR_TYPE_STRUCT);
  ASSERT_FIELD_TYPE (ext_struct_t, self_ptr_cv, MR_TYPE_POINTER, MR_TYPE_STRUCT);
  ASSERT_FIELD_TYPE (ext_struct_t, c_self_ptr_c, MR_TYPE_POINTER, MR_TYPE_STRUCT);
  ASSERT_FIELD_TYPE (ext_struct_t, v_self_ptr_v, MR_TYPE_POINTER, MR_TYPE_STRUCT);
} END_TEST

START_TEST (check_ext_enum) {
  mr_conf_init ();

  mr_td_t * tdp = mr_get_td_by_name ("ext_enum_t");
  ck_assert_msg (tdp != NULL, "Failed to get type descriptor for type ext_enum_t.");
  ck_assert_msg (tdp->mr_type == MR_TYPE_ENUM, "Wrong mr_type in type descriptor for type ext_enum_t.");

  mr_ed_t * edp = mr_get_enum_by_name ("_0");
  ck_assert_msg (edp != NULL, "Failed to get enum descriptor for enum '_0'.");
  ck_assert_msg (edp->value._unsigned == _0, "Wrong value in enum descriptor for enum '_0'.");

  edp = mr_get_enum_by_name ("_1");
  ck_assert_msg (edp != NULL, "Failed to get enum descriptor for enum '_1'.");
  ck_assert_msg (edp->value._unsigned == _1, "Wrong value in enum descriptor for enum '_1'.");

  edp = mr_get_enum_by_name ("_2");
  ck_assert_msg (edp == NULL, "Unexpected enum descriptor for enum '_2'.");
} END_TEST

START_TEST (check_type_autodetection) {
  embeded_struct_t x = {0};
#ifdef HAVE_BUILTIN_DUMP_STRUCT
  mr_ra_ptrdes_t ptrs = MR_SAVE ( , &x);
#else /* HAVE_BUILTIN_DUMP_STRUCT */
  mr_ra_ptrdes_t ptrs = MR_SAVE (embeded_struct_t , &x);
#endif /* HAVE_BUILTIN_DUMP_STRUCT */
  ck_assert_msg (ptrs.size / sizeof (ptrs.ra[0]) == 2, "Incorrect introspection of the empty struct");
  ck_assert_msg (ptrs.ra != NULL, "Failed to serialize array sructure");
  ck_assert_msg (ptrs.ra[0].mr_type == MR_TYPE_STRUCT, "Incorrect type of root node");
  ck_assert_msg (ptrs.ra[1].mr_type == MR_TYPE_DETECT (__typeof__ (((embeded_struct_t*)0)->x)), "Incorrect type of data node");
  if (ptrs.ra)
    MR_FREE (ptrs.ra);
} END_TEST

START_TEST (check_struct_array_autodetect) {
  embeded_struct_t array[] = { {1}, {2}, {3} };
#ifdef HAVE_BUILTIN_DUMP_STRUCT
  mr_ra_ptrdes_t ptrs = MR_SAVE ( , array);
#else /* HAVE_BUILTIN_DUMP_STRUCT */
  mr_ra_ptrdes_t ptrs = MR_SAVE (embeded_struct_t , array);
#endif /* HAVE_BUILTIN_DUMP_STRUCT */
  ck_assert_msg (ptrs.size / sizeof (ptrs.ra[0]) == 2 * sizeof (array) / sizeof (array[0]) + 1, "Incorrect introspection of the array");
  ck_assert_msg (ptrs.ra != NULL, "Failed to serialize array sructure");
  ck_assert_msg (ptrs.ra[0].mr_type == MR_TYPE_ARRAY, "Incorrect type of root node");
  ck_assert_msg (ptrs.ra[0].mr_type_aux == MR_TYPE_STRUCT, "Incorrect auxiliary type of root node");
  ck_assert_msg (ptrs.ra[0].MR_SIZE == sizeof (array), "Incorrect array size");
  if (ptrs.ra)
    MR_FREE (ptrs.ra);
} END_TEST

START_TEST (check_basic_type_array_autodetect) {
  complex long double array[] = {1, 2, 3};
  mr_ra_ptrdes_t ptrs = MR_SAVE ( , array);
  ck_assert_msg (ptrs.size / sizeof (ptrs.ra[0]) == sizeof (array) / sizeof (array[0]) + 1, "Incorrect introspection of the array");
  ck_assert_msg (ptrs.ra != NULL, "Failed to serialize array sructure");
  ck_assert_msg (ptrs.ra[0].mr_type == MR_TYPE_ARRAY, "Incorrect type of root node");
  ck_assert_msg (ptrs.ra[0].mr_type_aux == MR_TYPE_DETECT (__typeof__ (*array)), "Incorrect auxiliary type of root node");
  ck_assert_msg (ptrs.ra[0].MR_SIZE == sizeof (array), "Incorrect array size");
  if (ptrs.ra)
    MR_FREE (ptrs.ra);
} END_TEST

START_TEST (check_types_detection) {
  mr_conf_init ();

  ASSERT_STRUCT_FIELD_TYPE (bool_, MR_TYPE_BOOL);
  ASSERT_STRUCT_FIELD_TYPE (int8_, MR_TYPE_INT8);
  ASSERT_STRUCT_FIELD_TYPE (uint8_, MR_TYPE_UINT8);
  ASSERT_STRUCT_FIELD_TYPE (int16_, MR_TYPE_INT16);
  ASSERT_STRUCT_FIELD_TYPE (uint16_, MR_TYPE_UINT16);
  ASSERT_STRUCT_FIELD_TYPE (int32_, MR_TYPE_INT32);
  ASSERT_STRUCT_FIELD_TYPE (uint32_, MR_TYPE_UINT32);
  ASSERT_STRUCT_FIELD_TYPE (int64_, MR_TYPE_INT64);
  ASSERT_STRUCT_FIELD_TYPE (uint64_, MR_TYPE_UINT64);
#ifdef HAVE_INT128
  ASSERT_STRUCT_FIELD_TYPE (mr_intmax, MR_TYPE_INT128);
  ASSERT_STRUCT_FIELD_TYPE (mr_uintmax, MR_TYPE_UINT128);
#endif /* HAVE_INT128 */

  ASSERT_STRUCT_FIELD_TYPE (float_, MR_TYPE_FLOAT);
  ASSERT_STRUCT_FIELD_TYPE (complex_float_, MR_TYPE_COMPLEX_FLOAT);
  ASSERT_STRUCT_FIELD_TYPE (double_, MR_TYPE_DOUBLE);
  ASSERT_STRUCT_FIELD_TYPE (complex_double_, MR_TYPE_COMPLEX_DOUBLE);
  ASSERT_STRUCT_FIELD_TYPE (long_double_, MR_TYPE_LONG_DOUBLE);
  ASSERT_STRUCT_FIELD_TYPE (complex_long_double_, MR_TYPE_COMPLEX_LONG_DOUBLE);

  ASSERT_STRUCT_FIELD_TYPE (size, MR_TYPE_DETECT (size_t));
  ASSERT_STRUCT_FIELD_TYPE (ssize, MR_TYPE_DETECT (ssize_t));

  ASSERT_STRUCT_FIELD_TYPE (char_array_, MR_TYPE_CHAR_ARRAY);
  ASSERT_STRUCT_FIELD_TYPE (char_, MR_TYPE_CHAR);

  ASSERT_STRUCT_FIELD_TYPE (string, MR_TYPE_STRING, MR_TYPE_CHAR);
  ASSERT_STRUCT_FIELD_TYPE (const_string, MR_TYPE_STRING, MR_TYPE_CHAR);
  ASSERT_STRUCT_FIELD_TYPE (volatile_string, MR_TYPE_STRING, MR_TYPE_CHAR);
  ASSERT_STRUCT_FIELD_TYPE (const_volatile_string, MR_TYPE_STRING, MR_TYPE_CHAR);
  ASSERT_STRUCT_FIELD_TYPE (volatile_const_string, MR_TYPE_STRING, MR_TYPE_CHAR);

  ASSERT_STRUCT_FIELD_TYPE (char_ptr_, MR_TYPE_STRING, MR_TYPE_CHAR);
  ASSERT_STRUCT_FIELD_TYPE (int32_alias0, MR_TYPE_INT32, MR_TYPE_NONE);
  ASSERT_STRUCT_FIELD_TYPE (int32_alias1, MR_TYPE_POINTER, MR_TYPE_INT32);
  ASSERT_STRUCT_FIELD_TYPE (int32_alias2, MR_TYPE_ARRAY, MR_TYPE_INT32);
  ASSERT_STRUCT_FIELD_TYPE (int32_alias3, MR_TYPE_ARRAY, MR_TYPE_POINTER);
  ASSERT_STRUCT_FIELD_TYPE (int32_ptr, MR_TYPE_POINTER, MR_TYPE_INT32);

  mr_td_t * tdp = mr_get_td_by_name ("_enum_t");
  ck_assert_msg (tdp != NULL, "Type descriptor for _enum_t is not found");
  ck_assert_msg (tdp->param.enum_param.is_bitmask, "Type descriptor for _enum_t is not bitmask");

  ASSERT_STRUCT_FIELD_TYPE (string_, MR_TYPE_STRING, MR_TYPE_CHAR);
  ASSERT_STRUCT_FIELD_TYPE (const_string_, MR_TYPE_STRING, MR_TYPE_CHAR);
  ASSERT_STRUCT_FIELD_TYPE (volatile_string_, MR_TYPE_STRING, MR_TYPE_CHAR);
  ASSERT_STRUCT_FIELD_TYPE (const_volatile_string_, MR_TYPE_STRING, MR_TYPE_CHAR);
  ASSERT_STRUCT_FIELD_TYPE (volatile_const_string_, MR_TYPE_STRING, MR_TYPE_CHAR);

  ASSERT_STRUCT_FIELD_TYPE (void_ptr_, MR_TYPE_POINTER, MR_TYPE_VOID);
  ASSERT_STRUCT_FIELD_TYPE (const_void_ptr_, MR_TYPE_POINTER, MR_TYPE_VOID);
  ASSERT_STRUCT_FIELD_TYPE (void_const_ptr_, MR_TYPE_POINTER, MR_TYPE_VOID);
  ASSERT_STRUCT_FIELD_TYPE (void_ptr_const, MR_TYPE_POINTER, MR_TYPE_VOID);
  ASSERT_STRUCT_FIELD_TYPE (void_const_ptr_const, MR_TYPE_POINTER, MR_TYPE_VOID);
  ASSERT_STRUCT_FIELD_TYPE (volatile_void_ptr_, MR_TYPE_POINTER, MR_TYPE_VOID);
  ASSERT_STRUCT_FIELD_TYPE (const_volatile_void_ptr_, MR_TYPE_POINTER, MR_TYPE_VOID);
  ASSERT_STRUCT_FIELD_TYPE (volatile_const_void_ptr_, MR_TYPE_POINTER, MR_TYPE_VOID);

  ASSERT_STRUCT_FIELD_TYPE (float_ptr_, MR_TYPE_POINTER, MR_TYPE_FLOAT);
  ASSERT_STRUCT_FIELD_TYPE (const_float_ptr_, MR_TYPE_POINTER, MR_TYPE_FLOAT);
  ASSERT_STRUCT_FIELD_TYPE (volatile_float_ptr_, MR_TYPE_POINTER, MR_TYPE_FLOAT);
  ASSERT_STRUCT_FIELD_TYPE (const_volatile_float_ptr_, MR_TYPE_POINTER, MR_TYPE_FLOAT);
  ASSERT_STRUCT_FIELD_TYPE (volatile_const_float_ptr_, MR_TYPE_POINTER, MR_TYPE_FLOAT);

  ASSERT_STRUCT_FIELD_TYPE (char_array_ptr_, MR_TYPE_POINTER, MR_TYPE_CHAR_ARRAY);
  ASSERT_STRUCT_FIELD_TYPE (const_char_array_ptr_, MR_TYPE_POINTER, MR_TYPE_CHAR_ARRAY);
  ASSERT_STRUCT_FIELD_TYPE (volatile_char_array_ptr_, MR_TYPE_POINTER, MR_TYPE_CHAR_ARRAY);
  ASSERT_STRUCT_FIELD_TYPE (const_volatile_char_array_ptr_, MR_TYPE_POINTER, MR_TYPE_CHAR_ARRAY);
  ASSERT_STRUCT_FIELD_TYPE (volatile_const_char_array_ptr_, MR_TYPE_POINTER, MR_TYPE_CHAR_ARRAY);

  ASSERT_STRUCT_FIELD_TYPE (string_ptr_, MR_TYPE_POINTER, MR_TYPE_STRING);
  ASSERT_STRUCT_FIELD_TYPE (const_string_ptr_, MR_TYPE_POINTER, MR_TYPE_STRING);
  ASSERT_STRUCT_FIELD_TYPE (volatile_string_ptr_, MR_TYPE_POINTER, MR_TYPE_STRING);
  ASSERT_STRUCT_FIELD_TYPE (const_volatile_string_ptr_, MR_TYPE_POINTER, MR_TYPE_STRING);
  ASSERT_STRUCT_FIELD_TYPE (volatile_const_string_ptr_, MR_TYPE_POINTER, MR_TYPE_STRING);

  ASSERT_STRUCT_FIELD_TYPE (string_const_ptr_, MR_TYPE_POINTER, MR_TYPE_STRING);
  ASSERT_STRUCT_FIELD_TYPE (string_volatile_ptr_, MR_TYPE_POINTER, MR_TYPE_STRING);
  ASSERT_STRUCT_FIELD_TYPE (string_const_volatile_ptr_, MR_TYPE_POINTER, MR_TYPE_STRING);
  ASSERT_STRUCT_FIELD_TYPE (string_volatile_const_ptr_, MR_TYPE_POINTER, MR_TYPE_STRING);
  
  ASSERT_STRUCT_FIELD_TYPE (string_t_const_ptr_, MR_TYPE_POINTER, MR_TYPE_STRING);
  ASSERT_STRUCT_FIELD_TYPE (string_t_volatile_ptr_, MR_TYPE_POINTER, MR_TYPE_STRING);
  ASSERT_STRUCT_FIELD_TYPE (string_t_const_volatile_ptr_, MR_TYPE_POINTER, MR_TYPE_STRING);
  ASSERT_STRUCT_FIELD_TYPE (string_t_volatile_const_ptr_, MR_TYPE_POINTER, MR_TYPE_STRING);

  ASSERT_STRUCT_FIELD_TYPE (embeded_struct, MR_TYPE_STRUCT, MR_TYPE_NONE);
  ASSERT_STRUCT_FIELD_TYPE (const_volatile_struct_embeded_struct, MR_TYPE_STRUCT, MR_TYPE_NONE);
  ASSERT_STRUCT_FIELD_TYPE (embeded_struct_ptr, MR_TYPE_POINTER, MR_TYPE_STRUCT);
  ASSERT_STRUCT_FIELD_TYPE (const_volatile_struct_embeded_struct_ptr, MR_TYPE_POINTER, MR_TYPE_STRUCT);
  
  ASSERT_STRUCT_FIELD_TYPE (embeded_union, MR_TYPE_UNION, MR_TYPE_NONE);
  ASSERT_STRUCT_FIELD_TYPE (const_volatile_union_embeded_union, MR_TYPE_UNION, MR_TYPE_NONE);
  ASSERT_STRUCT_FIELD_TYPE (embeded_union_ptr, MR_TYPE_POINTER, MR_TYPE_UNION);
  ASSERT_STRUCT_FIELD_TYPE (const_volatile_union_embeded_union_ptr, MR_TYPE_POINTER, MR_TYPE_UNION);
  
  ASSERT_STRUCT_FIELD_TYPE (_enum, MR_TYPE_ENUM, MR_TYPE_NONE);
  ASSERT_STRUCT_FIELD_TYPE (const_volatile_enum_enum, MR_TYPE_ENUM, MR_TYPE_NONE);
  ASSERT_STRUCT_FIELD_TYPE (_enum_ptr, MR_TYPE_POINTER, MR_TYPE_ENUM);
  ASSERT_STRUCT_FIELD_TYPE (const_volatile_enum_enum_ptr, MR_TYPE_POINTER, MR_TYPE_ENUM);
  
  ASSERT_STRUCT_FIELD_TYPE (bf_bool, MR_TYPE_BITFIELD, MR_TYPE_BOOL);
  ASSERT_STRUCT_FIELD_TYPE (bf_uint8_t, MR_TYPE_BITFIELD, MR_TYPE_UINT8);
  ASSERT_STRUCT_FIELD_TYPE (bf_int8_t, MR_TYPE_BITFIELD, MR_TYPE_INT8);
  ASSERT_STRUCT_FIELD_TYPE (bf_uint16_t, MR_TYPE_BITFIELD, MR_TYPE_UINT16);
  ASSERT_STRUCT_FIELD_TYPE (bf_int16_t, MR_TYPE_BITFIELD, MR_TYPE_INT16);
  ASSERT_STRUCT_FIELD_TYPE (bf_uint32_t, MR_TYPE_BITFIELD, MR_TYPE_UINT32);
  ASSERT_STRUCT_FIELD_TYPE (bf_int32_t, MR_TYPE_BITFIELD, MR_TYPE_INT32);
  ASSERT_STRUCT_FIELD_TYPE (bf_uint64_t, MR_TYPE_BITFIELD, MR_TYPE_UINT64);
  ASSERT_STRUCT_FIELD_TYPE (bf_int64_t, MR_TYPE_BITFIELD, MR_TYPE_INT64);
#ifdef HAVE_INT128
  ASSERT_STRUCT_FIELD_TYPE (bf_uint128_t, MR_TYPE_BITFIELD, MR_TYPE_UINT128);
  ASSERT_STRUCT_FIELD_TYPE (bf_int128_t, MR_TYPE_BITFIELD, MR_TYPE_INT128);
#endif /* HAVE_INT128 */
  ASSERT_STRUCT_FIELD_TYPE (bf_enum, MR_TYPE_BITFIELD, MR_TYPE_ENUM);
  ASSERT_STRUCT_FIELD_TYPE (bf_const_volatile_enum_enum, MR_TYPE_BITFIELD, MR_TYPE_ENUM);

  ASSERT_STRUCT_FIELD_TYPE (f, MR_TYPE_FUNC);
  tdp = mr_get_td_by_name ("struct_t");
  mr_fd_t * fdp = mr_get_fd_by_name (tdp, "f");
  ck_assert_msg (fdp->param.func_param.size == 6 * sizeof (fdp->param.func_param.args[0]), "Wrong number of arguments for 'f' of type struct_t.");
  mr_structured_type_t ** args = fdp->param.func_param.args;
  ck_assert_msg (args[0]->mr_type == MR_TYPE_INT32, "Wrong type of return value for 'f' of type struct_t.");
  ck_assert_msg (args[1]->mr_type == MR_TYPE_POINTER, "Wrong type of first argument for 'f' of type struct_t.");
  ck_assert_msg (args[1]->mr_type_aux == MR_TYPE_INT32, "Wrong auxiliary type of first argument for 'f' of type struct_t.");
  ck_assert_msg (args[2]->mr_type == MR_TYPE_ARRAY, "Wrong type of second argument for 'f' of type struct_t.");
  ck_assert_msg (args[2]->mr_type_aux == MR_TYPE_INT32, "Wrong auxiliary type of second argument for 'f' of type struct_t.");
  ck_assert_msg (args[3]->mr_type == MR_TYPE_ARRAY, "Wrong type of third argument for 'f' of type struct_t.");
  ck_assert_msg (args[3]->mr_type_aux == MR_TYPE_POINTER, "Wrong auxiliary type of third argument for 'f' of type struct_t.");
  ck_assert_msg (args[3]->tdp != NULL, "Undetected type of third argument for 'f' of type struct_t.");
  ck_assert_msg (args[3]->tdp->mr_type == MR_TYPE_INT32, "Wrong type of third argument for 'f' of type struct_t.");
  ck_assert_msg (args[4]->mr_type == MR_TYPE_CHAR_ARRAY, "Wrong type of fourth argument for 'f' of type struct_t.");
  ck_assert_msg (args[5]->mr_type == MR_TYPE_ARRAY, "Wrong type of fifth argument for 'f' of type struct_t.");
  ck_assert_msg (args[5]->mr_type_aux == MR_TYPE_CHAR, "Wrong auxiliary type of fifth argument for 'f' of type struct_t.");
} END_TEST

#ifdef HAVE_BUILTIN_DUMP_STRUCT_EXTRA_ARGS

#undef MR_MODE

TYPEDEF_STRUCT (wrap_embeded_struct_t, (embeded_struct_t, es));
TYPEDEF_UNION (wrap_embeded_union_t, (embeded_union_t, eu));

#define TYPEDEF_STRUCT_HACK(TYPE_NAME, ...)				\
  TYPEDEF_STRUCT (TYPE_NAME, __VA_ARGS__);				\
  P00_TYPEDEF_MODE (PROTO, __COUNTER__, STRUCT, _ ## TYPE_NAME, __VA_ARGS__)

TYPEDEF_STRUCT_HACK (dump_struct_types_t,
		     (string_t, string),
		     (char, _char),
		     (bool, _bool),
		     (int8_t, _int8),
		     (uint8_t, _uint8),
		     (int16_t, _int16),
		     (uint16_t, _uint16),
		     (int32_t, _int32),
		     (uint32_t, _uint32),
		     (int64_t, _int64),
		     (uint64_t, _uint64),
		     (mr_int128_t, _int128),
		     (mr_uint128_t, _uint128),
		     (long_double_t, _long_double),
		     (mr_empty_t *, _struct_ptr),
		     (char, _char_array, [2]),
		     (char *, _str_array, [2]),
		     (int, _array, [2]),
		     (int, _2d_array, [3][2]),
		     (int, _3d_array, [4][3][2]),
		     (int, _4d_array, [5][4][3][2]),
		     (int, _5d_array, [5][4][3][2][1]),
		     (void *, void_ptr),
		     (int *, int_ptr),
		     (int *, int_ptr_array, [5][4][3][2]),
		     (int **, int_dptr),
		     (int32_alias_t, int32_alias5),
		     (const int32_alias_t, int32_alias6),
		     (int32_alias_t volatile, int32_alias7),
		     (double, _double),
		     (float, _float),
		     (embeded_struct_t, embeded_struct),
		     (embeded_union_t, embeded_union),
		     (wrap_embeded_struct_t, wrap_embeded_struct),
		     (wrap_embeded_union_t, wrap_embeded_union),
		     );

typedef char * alias_string_t;

struct aliases_t {
  int32_alias_t int32_alias8;
  const int32_alias_t int32_alias9;
  int32_alias_t volatile int32_alias10;
  const alias_string_t volatile alias_string1;
  const volatile alias_string_t alias_string0;
  const char * volatile const string;
  const volatile char * volatile const cv_string;
};

MR_FOREACH (MR_ADD_TYPE, _dump_struct_types_t, struct aliases_t);

START_TEST (dump_struct_types_detection) {
  mr_conf_init ();

  mr_td_t * mr_tdp = mr_get_td_by_name ("dump_struct_types_t");
  ck_assert_msg (mr_tdp != NULL, "Failed to get type descriptor for type dump_struct_types_t.");
  mr_td_t * dst_tdp = mr_get_td_by_name ("_dump_struct_types_t");
  ck_assert_msg (dst_tdp != NULL, "Failed to get type descriptor for type _dump_struct_types_t.");

  int i, count = mr_tdp->param.struct_param.fields_size / sizeof (mr_tdp->param.struct_param.fields[0]);
  for (i = 0; i < count; ++i)
    {
      mr_fd_t * mr_fdp = mr_tdp->param.struct_param.fields[i];
      mr_fd_t * dst_fdp = mr_get_fd_by_name (dst_tdp, mr_fdp->name.str);
      ck_assert_msg (dst_fdp != NULL, "dump_struct have not detected field '%s'", mr_fdp->name.str);

      ck_assert_msg (mr_fdp->stype.mr_type == dst_fdp->stype.mr_type, "dump_struct mismatched mr_type (%d != %d) for field '%s'", mr_fdp->stype.mr_type, dst_fdp->stype.mr_type, mr_fdp->name.str);
      if (!(mr_fdp->stype.mr_type == MR_TYPE_STRING) &&
	  !((mr_fdp->stype.mr_type == MR_TYPE_POINTER) && (mr_fdp->stype.mr_type_aux == MR_TYPE_VOID)))
	ck_assert_msg (mr_fdp->stype.mr_type_aux == dst_fdp->stype.mr_type_aux, "dump_struct mismatched mr_type_aux (%d != %d) for field '%s'", mr_fdp->stype.mr_type_aux, dst_fdp->stype.mr_type_aux, mr_fdp->name.str);

      int j;
      if (mr_fdp->stype.mr_type == MR_TYPE_ARRAY)
	for (j = 0; j < sizeof (mr_fdp->param.array_param.dim.dim) / sizeof (mr_fdp->param.array_param.dim.dim[0]); ++j)
	  {
	    ck_assert_msg (mr_fdp->param.array_param.dim.dim[j].count == dst_fdp->param.array_param.dim.dim[j].count,
			   "dump_struct mismatched array_param.count[%d] (%d != %d) for field '%s'",
			   j, (int)mr_fdp->param.array_param.dim.dim[j].count, (int)dst_fdp->param.array_param.dim.dim[j].count, mr_fdp->name.str);
	    ck_assert_msg (mr_fdp->param.array_param.dim.dim[j].is_last == dst_fdp->param.array_param.dim.dim[j].is_last,
			   "dump_struct mismatched array_param.dim.dim[%d].is_last for field '%s'",
			   j, mr_fdp->name.str);
	    if (mr_fdp->param.array_param.dim.dim[j].is_last)
	      break;
	  }
      ck_assert_msg (mr_fdp->offset == dst_fdp->offset, "dump_struct mismatched offset (%zd != %zd) for field '%s'", mr_fdp->offset, dst_fdp->offset, mr_fdp->name.str);
    }

  mr_td_t * tdp = mr_get_td_by_name ("aliases_t");
  ck_assert_msg (tdp != NULL, "Type descriptor for aliases_t was not found");

  char * string_fields[] = {"alias_string1", "alias_string0", "string", "cv_string"};
  for (i = 0; i < sizeof (string_fields) / sizeof (string_fields[0]); ++i)
    {
      mr_fd_t * fdp = mr_get_fd_by_name (tdp, string_fields[i]);
      ck_assert_msg (fdp != NULL, "Fields descriptor for field '%s' was not found", string_fields[i]);
      ck_assert_msg (fdp->stype.mr_type == MR_TYPE_STRING, "Field '%s' was registered with a wrong mr_type (%d)", string_fields[i], fdp->stype.mr_type);
      ck_assert_msg (fdp->stype.tdp != NULL, "Type desicriptor for field '%s' was not registered", string_fields[i]);
    }
} END_TEST

#else

START_TEST (dump_struct_types_detection) { } END_TEST

#endif /* HAVE_BUILTIN_DUMP_STRUCT_EXTRA_ARGS */

MAIN_TEST_SUITE ((check_void_function_field, "check that non-serializable function pointer is declared correctly"),
		 (check_ext_struct, "check that descriptor for external type is correct"),
		 (check_ext_enum, "check that descriptor for external enum is correct"),
		 (check_type_autodetection, "check struct type autodetection"),
		 (check_struct_array_autodetect, "check strcuts array autodetection"),
		 (check_basic_type_array_autodetect, "check that MR_SAVE handles array correctly"),
		 (check_types_detection, "check that types detected correctly"),
		 (dump_struct_types_detection, "check types detected by __builtin_dump_struct")
		 );
