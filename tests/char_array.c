#include <check.h>
#include <metaresc.h>
#include <regression.h>

/*
  As in C, a char array may be filled up to its size without a terminating zero.
  Everything that reads a char array as a string must stop at its size.
*/

typedef char name4_t[4];

TYPEDEF_UNION (abcd_wxyz_t, (int32_t, abcd), (int32_t, wxyz));

/* the discriminator is followed by a non-zero byte, so a string that runs past it names no branch */
TYPEDEF_STRUCT (named_union_t,
                (abcd_wxyz_t, u, , "name"),
                (name4_t, name),
                (char, tail),
                );

START_TEST (full_char_array_discriminator) {
  named_union_t x;
  memset (&x, 0, sizeof (x));
  memcpy (x.name, "wxyz", sizeof (x.name));
  x.u.wxyz = 3;
  x.tail = 'X';

  char * saved = MR_SAVE_CINIT (named_union_t, &x);
  ck_assert_msg (saved != NULL, "save failed");
  ck_assert_msg (strstr (saved, ".wxyz = 3") != NULL, "wrong branch is saved:\n%s", saved);
  MR_FREE (saved);

  ALL_METHODS (ASSERT_SAVE_LOAD, named_union_t, &x);
} END_TEST

/* the char array is the last field, so a read past it leaves the allocated block */
TYPEDEF_STRUCT (last_name_t,
                (int32_t, i),
                (name4_t, name),
                );

START_TEST (full_char_array_hash_cmp) {
  last_name_t * x = MR_CALLOC (1, sizeof (*x));
  last_name_t * y = MR_CALLOC (1, sizeof (*y));
  ck_assert_msg ((x != NULL) && (y != NULL), "out of memory");
  memcpy (x->name, "abcd", sizeof (x->name));
  memcpy (y->name, "abcd", sizeof (y->name));

  ck_assert_msg (0 == MR_CMP_STRUCTS (last_name_t, x, y), "equal char arrays compare as different");
  ck_assert_msg (MR_HASH_STRUCT (last_name_t, x) == MR_HASH_STRUCT (last_name_t, y), "equal char arrays have different hashes");

  memcpy (y->name, "abce", sizeof (y->name));
  ck_assert_msg (MR_CMP_STRUCTS (last_name_t, x, y) < 0, "'abcd' compares as not less than 'abce'");
  memcpy (y->name, "abc", sizeof ("abc"));
  ck_assert_msg (MR_CMP_STRUCTS (last_name_t, x, y) > 0, "'abcd' compares as not greater than 'abc'");

  MR_FREE (x);
  MR_FREE (y);
} END_TEST

/* savers must not copy a char array on the stack */
typedef char huge_name_t[32 << 20];

TYPEDEF_STRUCT (huge_name_struct_t,
                (huge_name_t, name),
                );

/* ASSERT_SAVE_LOAD restores into a variable on the stack, so only save is checked here */
#define ASSERT_SAVE(METHOD, TYPE, X, ...) ({                            \
      mr_rarray_t serialized = MR_SAVE_ ## METHOD ## _RA (TYPE, X);     \
      ck_assert_msg (serialized.data.ptr != NULL, "save for method " #METHOD " on type " #TYPE " failed"); \
      MR_FREE (serialized.data.ptr);                                    \
    })

START_TEST (huge_char_array_save) {
  huge_name_struct_t * x = MR_CALLOC (1, sizeof (*x));
  ck_assert_msg (x != NULL, "out of memory");
  strcpy (x->name, "abc");
  ALL_METHODS (ASSERT_SAVE, huge_name_struct_t, x);
  MR_FREE (x);
} END_TEST

MAIN_TEST_SUITE ((full_char_array_discriminator, "a char array without a terminating zero selects a union branch"),
                 (full_char_array_hash_cmp, "char arrays without a terminating zero are hashed and compared"),
                 (huge_char_array_save, "a large char array is saved without a copy on the stack")
                 );
