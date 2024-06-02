#include <time.h>
#include <assert.h>
#include <metaresc.h>
#include <mr_ic.h>
#include <check.h>

#include <regression.h>

TYPEDEF_STRUCT (a_t,
		ANON_UNION (),
		ANON_UNION (),
		ANON_UNION (),
		ANON_UNION (),
		ANON_UNION (),
		ANON_UNION (),
		ANON_UNION (),
		ANON_UNION (),
		ANON_UNION (),
		ANON_UNION (),
		ANON_UNION (),
		ANON_UNION (),
		ANON_UNION (),
		ANON_UNION (),
		(uint8_t, x),
		END_ANON_UNION (),
		END_ANON_UNION (),
		END_ANON_UNION (),
		END_ANON_UNION (),
		END_ANON_UNION (),
		END_ANON_UNION (),
		END_ANON_UNION (),
		END_ANON_UNION (),
		END_ANON_UNION (),
		END_ANON_UNION (),
		END_ANON_UNION (),
		END_ANON_UNION (),
		END_ANON_UNION (),
		END_ANON_UNION (),
		);

static int
measure_time_for_n_elements (int n)
{
  a_t * array = MR_CALLOC (n, sizeof (*array));
  ck_assert_msg (array != NULL, "Memory allocation failed");
  
  mr_rarray_t rarray;
  rarray.data.ptr = array;
  rarray.mr_size = n * sizeof (*array);
  rarray.type = "a_t";
  
  clock_t _time = clock ();
  mr_ra_ptrdes_t ptrs = MR_SAVE (mr_rarray_t, &rarray);
  _time = clock () - _time;

  if (ptrs.ra)
    MR_FREE (ptrs.ra);
  
  MR_FREE (array);
  return (_time);
}

START_TEST (mr_save_discriminated_union_complexity) {
  int i, size = 1 << 8;
  int base_time, double_time = measure_time_for_n_elements (size);
  do {
    size <<= 1;
    base_time = double_time;
    double_time = measure_time_for_n_elements (size);
  } while (double_time < CLOCKS_PER_SEC / 8);

  for (i = 0; i < 3; ++i)
    {
      int another_try = measure_time_for_n_elements (size);
      double_time = MR_MIN (double_time, another_try);
    }

  ck_assert_msg (double_time < (base_time * 5) / 2, "Union resolution is not in constant time (%d vs %d)", double_time, base_time);
} END_TEST

MAIN_TEST_SUITE ((mr_save_discriminated_union_complexity, "validate that complexity of saving discriminated unions is not dependent on collisions in hash table"));
