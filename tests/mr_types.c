#include <check.h>
#include <metaresc.h>
#include <regression.h>

typedef char char_array_t[1];
typedef char * char_ptr_t;

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
		)

#define ASSERT_FIELD_TYPE_(FIELD, MR_TYPE, MR_TYPE_AUX) ({		\
      mr_td_t * tdp = mr_get_td_by_name ("struct_t");			\
      ck_assert_msg (tdp != NULL, "Failed to get type descriptor for type struct_t."); \
      mr_fd_t * fdp = mr_get_fd_by_name (tdp, #FIELD);			\
      ck_assert_msg (fdp != NULL, "Failed to get field descriptor for field " #FIELD "."); \
      ck_assert_msg (fdp->mr_type == MR_TYPE, "Mismatched mr_type for field " #FIELD " (%d != %d).", \
		     fdp->mr_type, MR_TYPE);				\
      ck_assert_msg (fdp->mr_type_aux == MR_TYPE_AUX, "Mismatched mr_type_aux for field " #FIELD " (%d != %d).", \
		     fdp->mr_type_aux, MR_TYPE_AUX);			\
    })
  
#define ASSERT_FIELD_TYPE(FIELD, MR_TYPE, ...)				\
  ASSERT_FIELD_TYPE_ (FIELD, MR_TYPE, MR_IF_ELSE (MR_IS_EMPTY (__VA_ARGS__)) (MR_TYPE_NONE) (__VA_ARGS__))

MR_START_TEST (check_types_detection, "check that types detected correctly") {
  mr_detect_type (NULL);

  ASSERT_FIELD_TYPE (bool_, MR_TYPE_BOOL);
  ASSERT_FIELD_TYPE (int8_, MR_TYPE_INT8);
  ASSERT_FIELD_TYPE (uint8_, MR_TYPE_UINT8);
  ASSERT_FIELD_TYPE (int16_, MR_TYPE_INT16);
  ASSERT_FIELD_TYPE (uint16_, MR_TYPE_UINT16);
  ASSERT_FIELD_TYPE (int32_, MR_TYPE_INT32);
  ASSERT_FIELD_TYPE (uint32_, MR_TYPE_UINT32);
  ASSERT_FIELD_TYPE (int64_, MR_TYPE_INT64);
  ASSERT_FIELD_TYPE (uint64_, MR_TYPE_UINT64);

  ASSERT_FIELD_TYPE (float_, MR_TYPE_FLOAT);
  ASSERT_FIELD_TYPE (complex_float_, MR_TYPE_COMPLEX_FLOAT);
  ASSERT_FIELD_TYPE (double_, MR_TYPE_DOUBLE);
  ASSERT_FIELD_TYPE (complex_double_, MR_TYPE_COMPLEX_DOUBLE);
  ASSERT_FIELD_TYPE (long_double_, MR_TYPE_LONG_DOUBLE);
  ASSERT_FIELD_TYPE (complex_long_double_, MR_TYPE_COMPLEX_LONG_DOUBLE);

  ASSERT_FIELD_TYPE (char_array_, MR_TYPE_CHAR_ARRAY);
  ASSERT_FIELD_TYPE (char_, MR_TYPE_CHAR);

  ASSERT_FIELD_TYPE (string, MR_TYPE_STRING);
  ASSERT_FIELD_TYPE (const_string, MR_TYPE_STRING);
  ASSERT_FIELD_TYPE (volatile_string, MR_TYPE_STRING);
  ASSERT_FIELD_TYPE (const_volatile_string, MR_TYPE_STRING);
  ASSERT_FIELD_TYPE (volatile_const_string, MR_TYPE_STRING);

  ASSERT_FIELD_TYPE (char_ptr_, MR_TYPE_STRING);
  
  ASSERT_FIELD_TYPE (string_, MR_TYPE_STRING, MR_TYPE_CHAR);
  ASSERT_FIELD_TYPE (const_string_, MR_TYPE_STRING, MR_TYPE_CHAR);
  ASSERT_FIELD_TYPE (volatile_string_, MR_TYPE_STRING, MR_TYPE_CHAR);
  ASSERT_FIELD_TYPE (const_volatile_string_, MR_TYPE_STRING, MR_TYPE_CHAR);
  ASSERT_FIELD_TYPE (volatile_const_string_, MR_TYPE_STRING, MR_TYPE_CHAR);

  ASSERT_FIELD_TYPE (void_ptr_, MR_TYPE_POINTER, MR_TYPE_VOID);
  ASSERT_FIELD_TYPE (const_void_ptr_, MR_TYPE_POINTER, MR_TYPE_VOID);
  ASSERT_FIELD_TYPE (void_const_ptr_, MR_TYPE_POINTER, MR_TYPE_VOID);
  ASSERT_FIELD_TYPE (void_ptr_const, MR_TYPE_POINTER, MR_TYPE_VOID);
  ASSERT_FIELD_TYPE (void_const_ptr_const, MR_TYPE_POINTER, MR_TYPE_VOID);
  ASSERT_FIELD_TYPE (volatile_void_ptr_, MR_TYPE_POINTER, MR_TYPE_VOID);
  ASSERT_FIELD_TYPE (const_volatile_void_ptr_, MR_TYPE_POINTER, MR_TYPE_VOID);
  ASSERT_FIELD_TYPE (volatile_const_void_ptr_, MR_TYPE_POINTER, MR_TYPE_VOID);

  ASSERT_FIELD_TYPE (float_ptr_, MR_TYPE_POINTER, MR_TYPE_FLOAT);
  ASSERT_FIELD_TYPE (const_float_ptr_, MR_TYPE_POINTER, MR_TYPE_FLOAT);
  ASSERT_FIELD_TYPE (volatile_float_ptr_, MR_TYPE_POINTER, MR_TYPE_FLOAT);
  ASSERT_FIELD_TYPE (const_volatile_float_ptr_, MR_TYPE_POINTER, MR_TYPE_FLOAT);
  ASSERT_FIELD_TYPE (volatile_const_float_ptr_, MR_TYPE_POINTER, MR_TYPE_FLOAT);

  ASSERT_FIELD_TYPE (char_array_ptr_, MR_TYPE_POINTER, MR_TYPE_CHAR_ARRAY);
  ASSERT_FIELD_TYPE (const_char_array_ptr_, MR_TYPE_POINTER, MR_TYPE_CHAR_ARRAY);
  ASSERT_FIELD_TYPE (volatile_char_array_ptr_, MR_TYPE_POINTER, MR_TYPE_CHAR_ARRAY);
  ASSERT_FIELD_TYPE (const_volatile_char_array_ptr_, MR_TYPE_POINTER, MR_TYPE_CHAR_ARRAY);
  ASSERT_FIELD_TYPE (volatile_const_char_array_ptr_, MR_TYPE_POINTER, MR_TYPE_CHAR_ARRAY);

  ASSERT_FIELD_TYPE (string_ptr_, MR_TYPE_POINTER, MR_TYPE_STRING);
  ASSERT_FIELD_TYPE (const_string_ptr_, MR_TYPE_POINTER, MR_TYPE_STRING);
  ASSERT_FIELD_TYPE (volatile_string_ptr_, MR_TYPE_POINTER, MR_TYPE_STRING);
  ASSERT_FIELD_TYPE (const_volatile_string_ptr_, MR_TYPE_POINTER, MR_TYPE_STRING);
  ASSERT_FIELD_TYPE (volatile_const_string_ptr_, MR_TYPE_POINTER, MR_TYPE_STRING);

  ASSERT_FIELD_TYPE (string_const_ptr_, MR_TYPE_POINTER, MR_TYPE_STRING);
  ASSERT_FIELD_TYPE (string_volatile_ptr_, MR_TYPE_POINTER, MR_TYPE_STRING);
  ASSERT_FIELD_TYPE (string_const_volatile_ptr_, MR_TYPE_POINTER, MR_TYPE_STRING);
  ASSERT_FIELD_TYPE (string_volatile_const_ptr_, MR_TYPE_POINTER, MR_TYPE_STRING);
  
  ASSERT_FIELD_TYPE (string_t_const_ptr_, MR_TYPE_POINTER, MR_TYPE_STRING);
  ASSERT_FIELD_TYPE (string_t_volatile_ptr_, MR_TYPE_POINTER, MR_TYPE_STRING);
  ASSERT_FIELD_TYPE (string_t_const_volatile_ptr_, MR_TYPE_POINTER, MR_TYPE_STRING);
  ASSERT_FIELD_TYPE (string_t_volatile_const_ptr_, MR_TYPE_POINTER, MR_TYPE_STRING);

} END_TEST

MAIN ();
