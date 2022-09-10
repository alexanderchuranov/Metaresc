#include <time.h>
#include <assert.h>
#include <metaresc.h>
#include <mr_ic.h>
#include <check.h>

#include <regression.h>

TYPEDEF_STRUCT (c_t,
		ANON_UNION (),
		ANON_UNION (),
		(uint8_t, x),
		END_ANON_UNION (),
		END_ANON_UNION (),
		);

TYPEDEF_UNION (b_t, (c_t, c));
TYPEDEF_STRUCT (a_t, (b_t, b));

static clock_t
measure_time_for_serialization (mr_rarray_t * rarray)
{
  clock_t _time = clock ();
  mr_ra_ptrdes_t ptrs = MR_SAVE (mr_rarray_t, rarray);
  _time = clock () - _time;
  
  if (ptrs.ra)
    MR_FREE (ptrs.ra);
  
  return (_time);
}

static clock_t
measure_time_for_n_elements (int n)
{
  a_t * array = MR_CALLOC (n, sizeof (*array));
  ck_assert_msg (array != NULL, "Memory allocation failed");
  
  mr_rarray_t rarray;
  rarray.data.ptr = array;
  rarray.mr_size = n * sizeof (*array);
  rarray.type = "a_t";
  
  clock_t avg_time = 0;
  int i;
  for (i = 0; i < 1 << 2; ++i)
    avg_time += measure_time_for_serialization (&rarray);

  MR_FREE (array);
  return (avg_time);
}

START_TEST (mr_save_discriminated_union_complexity) {
  int size = 1 << 8;
  clock_t base_time;  
  do {
    size += size >> 1;
    base_time = measure_time_for_n_elements (size);
  } while (base_time < CLOCKS_PER_SEC / 50);

  clock_t scale_time = measure_time_for_n_elements (size * 4);

  ck_assert_msg (scale_time < base_time * 5, "Union resolution is not in constant time (%d vs %d)", (int)scale_time, (int)base_time);
} END_TEST

MAIN_TEST_SUITE ((mr_save_discriminated_union_complexity, "validate that complexity of saving discriminated unions is not dependent on number of union cases"));
