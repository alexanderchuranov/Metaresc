#include <stdlib.h>

#include <check.h>
#include <metaresc.h>
#include <mr_ic.h>
#include <regression.h>

TYPEDEF_CHAR_ARRAY (char_array_t, 2);
TYPEDEF_STRUCT (struct_func_t, (void, func, (void)));
TYPEDEF_STRUCT (struct_bit_field_t, BITFIELD (unsigned int, bf, :2));
TYPEDEF_STRUCT (struct_array_t, (int, array, [1]));
TYPEDEF_ENUM (mr_enum_t, ZERO, ONE, TWO);
TYPEDEF_FUNC (void, void_func_t, (void));
TYPEDEF_STRUCT (struct_2fields_t, int x, int y);

TYPEDEF_STRUCT (type_samples_t,
		string_t type,
		size_t samples_size,
		(mr_ptr_t *, samples, , "type", { .offset = offsetof (type_samples_t, samples_size) }, "offset"));

void func_zero (void) {}
void func_one (void) {}
void func_two (void) {}

#define MR_TYPE_SAMPLE(TYPE, X, I) { (TYPE[]){MR_REMOVE_PAREN (X)} }
#define MR_APPEND_COMMA(NAME, I, REC, X) REC, X
#define MR_TYPE_SAMPLES_VALUES_INIT(TYPE, ...) {			\
    .type = #TYPE,							\
      .samples_size = MR_NARG (__VA_ARGS__) * sizeof (mr_ptr_t),	\
      .samples = (mr_ptr_t[]){						\
	MR_FOR (TYPE, MR_NARG (__VA_ARGS__), MR_APPEND_COMMA, MR_TYPE_SAMPLE, __VA_ARGS__) \
      },								\
      },
#define MR_TYPE_SAMPLES_VALUES(TYPE, ...) [MR_TYPE_DETECT (TYPE)] = MR_TYPE_SAMPLES_VALUES_INIT (TYPE, __VA_ARGS__)
#define MR_TYPE_SAMPLES_NUMBERS(TYPE) MR_TYPE_SAMPLES_VALUES (TYPE, 0, 1, 2)

static type_samples_t type_samples[] = {
  MR_FOREACH (MR_TYPE_SAMPLES_NUMBERS, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, float, complex float, double, complex double, long double, complex long double)
  MR_TYPE_SAMPLES_VALUES (bool, false, true)
  MR_TYPE_SAMPLES_VALUES (char, '0', '1', '2')
  MR_TYPE_SAMPLES_VALUES (string_t, "0", "1", "2")
  MR_TYPE_SAMPLES_VALUES (char_array_t, "0", "1", "2")
  [MR_TYPE_STRUCT] = MR_TYPE_SAMPLES_VALUES_INIT (struct_func_t, {func_zero}, {func_one}, {func_two})
  [MR_TYPE_ENUM] = MR_TYPE_SAMPLES_VALUES_INIT (mr_enum_t, ZERO, ONE, TWO)
  [MR_TYPE_FUNC_TYPE] = MR_TYPE_SAMPLES_VALUES_INIT (void_func_t, func_zero, func_one, func_two)
  [MR_TYPE_BITFIELD] = MR_TYPE_SAMPLES_VALUES_INIT (struct_bit_field_t, {0}, {1}, {2})
  [MR_TYPE_ARRAY] = MR_TYPE_SAMPLES_VALUES_INIT (struct_array_t, {0}, {1}, {2})
  [MR_TYPE_LAST] = {},
  MR_TYPE_SAMPLES_VALUES_INIT (struct_2fields_t, ({0, 0}), ({0, 1}), ({1, 0}), ({1, 1}))
};

START_TEST (generic_ic) {
#define MR_IC_INIT(TYPE) [MR_IC_##TYPE] = #TYPE
  static char *
    mr_ic_types[] = {
    MR_IC_INIT (UNSORTED_ARRAY),
    MR_IC_INIT (SORTED_ARRAY),
    MR_IC_INIT (STATIC_ARRAY),
    MR_IC_INIT (HASH),
    MR_IC_INIT (RBTREE),
    MR_IC_INIT (AVLTREE),
    MR_IC_INIT (STATIC_ARRAY),
  };
  int ic_type, mr_type;

  for (ic_type = 0; ic_type < sizeof (mr_ic_types) / sizeof (mr_ic_types[0]); ++ic_type)
    if (mr_ic_types[ic_type] != NULL)
      for (mr_type = 0; mr_type < sizeof (type_samples) / sizeof (type_samples[0]); ++mr_type)
	if (type_samples[mr_type].type != NULL)
	  {
	    int i, j, samples = type_samples[mr_type].samples_size / sizeof (type_samples[mr_type].samples[0]);
	    mr_ic_t ic;
	    
	    ck_assert_msg (MR_SUCCESS == mr_ic_new (&ic, NULL, NULL, type_samples[mr_type].type, ic_type, NULL),
			   "mr_ic_new failed for mr_ic_type_t %s type %s", mr_ic_types[ic_type], type_samples[mr_type].type);			   
	    ck_assert_msg (MR_SUCCESS == mr_ic_index (&ic, type_samples[mr_type].samples, type_samples[mr_type].samples_size),
			   "mr_ic_index failed for mr_ic_type_t %s type %s", mr_ic_types[ic_type], type_samples[mr_type].type);
	    
	    for (j = 0; j < samples; ++j)
	      ck_assert_msg (NULL != mr_ic_find (&ic, type_samples[mr_type].samples[j]),
			     "mr_ic_find failed for mr_ic_type_t %s type %s", mr_ic_types[ic_type], type_samples[mr_type].type);			   

	    for (i = 0; i < samples; ++i)
	      {
		ck_assert_msg (MR_SUCCESS == mr_ic_del (&ic, type_samples[mr_type].samples[i]),
			       "mr_ic_del failed for mr_ic_type_t %s type %s", mr_ic_types[ic_type], type_samples[mr_type].type);			   
		ck_assert_msg (MR_SUCCESS != mr_ic_del (&ic, type_samples[mr_type].samples[i]),
			       "mr_ic_del failed for mr_ic_type_t %s type %s", mr_ic_types[ic_type], type_samples[mr_type].type);			   
		for (j = 0; j <= i; ++j)
		  ck_assert_msg (NULL == mr_ic_find (&ic, type_samples[mr_type].samples[j]),
				 "mr_ic_find failed for mr_ic_type_t %s type %s", mr_ic_types[ic_type], type_samples[mr_type].type);
		for (j = i + 1; j < samples; ++j)
		  ck_assert_msg (NULL != mr_ic_find (&ic, type_samples[mr_type].samples[j]),
				 "mr_ic_find failed for mr_ic_type_t %s type %s", mr_ic_types[ic_type], type_samples[mr_type].type);
	      }
	    
	    for (i = samples - 1; i >= 0; --i)
	      {
		ck_assert_msg (NULL != mr_ic_add (&ic, type_samples[mr_type].samples[i]),
			       "mr_ic_add failed for mr_ic_type_t %s type %s", mr_ic_types[ic_type], type_samples[mr_type].type);			   
		for (j = 0; j < i; ++j)
		  ck_assert_msg (NULL == mr_ic_find (&ic, type_samples[mr_type].samples[j]),
				 "mr_ic_find failed for mr_ic_type_t %s type %s", mr_ic_types[ic_type], type_samples[mr_type].type);			   
		for (j = i; j < samples; ++j)
		  ck_assert_msg (NULL != mr_ic_find (&ic, type_samples[mr_type].samples[j]),
				 "mr_ic_find failed for mr_ic_type_t %s type %s", mr_ic_types[ic_type], type_samples[mr_type].type);			   
	      }
	    
	    mr_ic_free (&ic);
	  }
} END_TEST

static int
struct_2fields_cmp (struct_2fields_t * x, struct_2fields_t * y)
{
  int cmp = (x->x > y->x) - (x->x < y->x);
  if (cmp)
    return (cmp);
  return ((x->y > y->y) - (x->y < y->y));
}

START_TEST (generic_sort) {
  int i;
  struct_2fields_t array[] = { {1, 1}, {0, 1}, {1, 0}, {0, 0} };
  ck_assert_msg (MR_SUCCESS == mr_generic_sort (array, sizeof (array) / sizeof (array[0]), "struct_2fields_t"),
		  "mr_generic_sort failed");
  for (i = 1; i < sizeof (array) / sizeof (array[0]); ++i)
    ck_assert_msg (struct_2fields_cmp (&array[i - 1], &array[i]) <= 0, "Elements are not sorted");
} END_TEST

MAIN_TEST_SUITE (
		 (generic_ic, "Check generic IC implementation"),
		 (generic_sort, "Check generic sort implementation")
		 );
