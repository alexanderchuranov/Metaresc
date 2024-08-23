#include <dw_mr_types.h>
#include <check.h>
#include <regression.h>

mr_pointer_t mr_pointer;
mr_builtin_types_t mr_builtin_types;
mr_array_t mr_array;
mr_anon_union_t mr_anon_union;
mr_void_fields_t mr_void_fields;
mr_inline_enum_t mr_inline_enum;
mr_bitfields_t mr_bitfields;

#define POSITIVE_TYPE_DETECTION(TYPE, PREFIX, SUFFIX) ({		\
      TYPE PREFIX var SUFFIX;						\
      char * type = MR_OBJ_TYPE_DWARF (var);				\
      ck_assert_msg (type && (0 == strcmp (type, #TYPE)),		\
		     "DWARF type detection mismatched for type " #TYPE #PREFIX #SUFFIX ", but got %s", type); \
    })

#define NEGATIVE_TYPE_DETECTION(TYPE, PREFIX, SUFFIX) ({		\
      TYPE PREFIX var SUFFIX;						\
      char * type = MR_OBJ_TYPE_DWARF (var);				\
      ck_assert_msg (type == NULL,					\
		     "DWARF type detection mismatched for type " #TYPE #PREFIX #SUFFIX ". Expected NULL, but got %s", type); \
    })

START_TEST (dw_var_type_detection)
{
  if (NULL == mr_conf.dwarf_list)
    return;

  POSITIVE_TYPE_DETECTION (mr_type_t, , );
  POSITIVE_TYPE_DETECTION (mr_type_t, *, );
  POSITIVE_TYPE_DETECTION (mr_type_t, , [1]);
  POSITIVE_TYPE_DETECTION (mr_type_t, , [1][2]);
  NEGATIVE_TYPE_DETECTION (mr_type_t, *, [1]);

  NEGATIVE_TYPE_DETECTION (int, , );
  NEGATIVE_TYPE_DETECTION (int, *, );
  NEGATIVE_TYPE_DETECTION (int, , [1]);
  NEGATIVE_TYPE_DETECTION (int, *, [1]);
  NEGATIVE_TYPE_DETECTION (void, *, );
  NEGATIVE_TYPE_DETECTION (void, **, );
} END_TEST

MAIN_TEST_SUITE ((dw_var_type_detection, "Check variables types detection with DWARF"));
