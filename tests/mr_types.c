#include <check.h>
#include <metaresc.h>
#include <regression.h>

typedef char char_array_t[1];
typedef char * char_ptr_t;

TYPEDEF_STRUCT (mr_empty_t);

TYPEDEF_STRUCT (mr_incomplete_t, (int, x, [0]), VOID (int, y, []));

TYPEDEF_ENUM (_enum_t,
	      ZERO
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
		(const float *, const_float_ptr_),
		(volatile float *, volatile_float_ptr_),
		(const volatile float *, const_volatile_float_ptr_),
		(volatile const float *, volatile_const_float_ptr_),
		
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
		BITFIELD (_enum_t, bf_enum, :1),
		BITFIELD (const volatile enum _enum_t, bf_const_volatile_enum_enum, :1),
		);

TYPEDEF_STRUCT (void_function_field_t,
		VOID (void, (*func), (void)));

START_TEST (check_void_function_field) {
  mr_detect_type (NULL);

  mr_td_t * tdp = mr_get_td_by_name ("void_function_field_t");
  ck_assert_msg (tdp != NULL, "Failed to get type descriptor for type void_function_field_t.");
  mr_fd_t * fdp = mr_get_fd_by_name (tdp, "func");
  ck_assert_msg (fdp != NULL, "Failed to get field descriptor for field 'func'.");
} END_TEST

typedef struct ext_struct_t {
  int16_t x;
  int32_t y;
  uint8_t * z;
  complex float const * z_c;
  complex double volatile * z_v;
  complex long double const volatile * z_cv;
  const float * c_z;
  volatile double * v_z;
  const volatile long double * cv_z;
  struct ext_struct_t * self_ptr;
  const struct ext_struct_t * c_self_ptr;
  volatile struct ext_struct_t * v_self_ptr;
  const volatile struct ext_struct_t * cv_self_ptr;
  struct ext_struct_t const * self_ptr_c;
  struct ext_struct_t volatile * self_ptr_v;
  struct ext_struct_t const volatile * self_ptr_cv;
  const struct ext_struct_t const * c_self_ptr_c;
  volatile struct ext_struct_t volatile * v_self_ptr_v;
} ext_struct_t;

#define MR_MODE DESC
TYPEDEF_STRUCT (ext_struct_t,
		x,
		( , y),
		z, z_c, z_v, z_cv, c_z, v_z, cv_z,
		self_ptr, c_self_ptr, v_self_ptr, cv_self_ptr, self_ptr_c, self_ptr_v, self_ptr_cv, c_self_ptr_c, v_self_ptr_v,
		);

#define ASSERT_FIELD_TYPE_(TYPE, FIELD, MR_TYPE, MR_TYPE_AUX) ({	\
      mr_td_t * tdp = mr_get_td_by_name (#TYPE);			\
      ck_assert_msg (tdp != NULL, "Failed to get type descriptor for type struct_t."); \
      mr_fd_t * fdp = mr_get_fd_by_name (tdp, #FIELD);			\
      ck_assert_msg (fdp != NULL, "Failed to get field descriptor for field " #FIELD "."); \
      ck_assert_msg (fdp->mr_type == MR_TYPE, "Mismatched mr_type for field " #FIELD " (%d != %d).", \
		     fdp->mr_type, MR_TYPE);				\
      ck_assert_msg (fdp->mr_type_aux == MR_TYPE_AUX, "Mismatched mr_type_aux for field " #FIELD " (%d != %d).", \
		     fdp->mr_type_aux, MR_TYPE_AUX);			\
    })
  
#define ASSERT_FIELD_TYPE(TYPE, FIELD, MR_TYPE, ...)			\
  ASSERT_FIELD_TYPE_ (TYPE, FIELD, MR_TYPE, MR_IF_ELSE (MR_IS_EMPTY (__VA_ARGS__)) (MR_TYPE_NONE) (__VA_ARGS__))

#define ASSERT_STRUCT_FIELD_TYPE(FIELD, MR_TYPE, ...) ASSERT_FIELD_TYPE (struct_t, FIELD, MR_TYPE, __VA_ARGS__)
#define ASSERT_EXT_STRUCT_FIELD_TYPE(FIELD) ASSERT_FIELD_TYPE (ext_struct_t, FIELD, MR_TYPE_DETECT (__typeof__ (((ext_struct_t*)0)->FIELD)))
#define ASSERT_EXT_STRUCT_FIELD_TYPE_PTR(FIELD) ASSERT_FIELD_TYPE (ext_struct_t, FIELD, MR_TYPE_POINTER, MR_TYPE_DETECT_PTR (__typeof__ (((ext_struct_t*)0)->FIELD)))

START_TEST (check_ext_struct) {
  mr_detect_type (NULL);
  
  ASSERT_EXT_STRUCT_FIELD_TYPE (x);
  ASSERT_EXT_STRUCT_FIELD_TYPE (y);
  ASSERT_EXT_STRUCT_FIELD_TYPE_PTR (z);
  ASSERT_EXT_STRUCT_FIELD_TYPE_PTR (z_c);
  ASSERT_EXT_STRUCT_FIELD_TYPE_PTR (z_v);
  ASSERT_EXT_STRUCT_FIELD_TYPE_PTR (z_cv);
  ASSERT_EXT_STRUCT_FIELD_TYPE_PTR (c_z);
  ASSERT_EXT_STRUCT_FIELD_TYPE_PTR (v_z);
  ASSERT_EXT_STRUCT_FIELD_TYPE_PTR (cv_z);
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

START_TEST (check_array_save) {
  complex long double array[] = {1, 2, 3};
  mr_ra_ptrdes_t ptrs = MR_SAVE ( ,  array);
  ck_assert_msg (ptrs.size / sizeof (ptrs.ra[0]) == sizeof (array) / sizeof (array[0]) + 1, "Incorrect introspection of the array");
  ck_assert_msg (ptrs.ra != NULL, "Failed to serialize array sructure");
  ck_assert_msg (ptrs.ra[0].mr_type == MR_TYPE_ARRAY, "Incorrect type of root node");
  ck_assert_msg (ptrs.ra[0].mr_type_aux == MR_TYPE_COMPLEX_LONG_DOUBLE, "Incorrect auxiliary type of root node");
  ck_assert_msg (ptrs.ra[0].size == sizeof (array), "Incorrect array size");
  if (ptrs.ra)
    MR_FREE (ptrs.ra);
} END_TEST

START_TEST (check_types_detection) {
  mr_detect_type (NULL);

  ASSERT_STRUCT_FIELD_TYPE (bool_, MR_TYPE_BOOL);
  ASSERT_STRUCT_FIELD_TYPE (int8_, MR_TYPE_INT8);
  ASSERT_STRUCT_FIELD_TYPE (uint8_, MR_TYPE_UINT8);
  ASSERT_STRUCT_FIELD_TYPE (int16_, MR_TYPE_INT16);
  ASSERT_STRUCT_FIELD_TYPE (uint16_, MR_TYPE_UINT16);
  ASSERT_STRUCT_FIELD_TYPE (int32_, MR_TYPE_INT32);
  ASSERT_STRUCT_FIELD_TYPE (uint32_, MR_TYPE_UINT32);
  ASSERT_STRUCT_FIELD_TYPE (int64_, MR_TYPE_INT64);
  ASSERT_STRUCT_FIELD_TYPE (uint64_, MR_TYPE_UINT64);

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
  ASSERT_STRUCT_FIELD_TYPE (bf_enum, MR_TYPE_BITFIELD, MR_TYPE_ENUM);
  ASSERT_STRUCT_FIELD_TYPE (bf_const_volatile_enum_enum, MR_TYPE_BITFIELD, MR_TYPE_ENUM);
  
} END_TEST

MAIN_TEST_SUITE ((check_void_function_field, "check that non-serializable function pointer is declared correctly"),
		 (check_ext_struct, "check that descriptor for external type is correct"),
		 (check_array_save, "check that MR_SAVE handles array correctly"),
		 (check_types_detection, "check that types detected correctly")
		 );
