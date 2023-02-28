#define __USE_GNU
#include <check.h>
#include <metaresc.h>
#include <regression.h>

#define PTR_CMP(TYPE, X, Y, ...) (*(void**)(X) != *(void**)(Y))

#define ASSERT_SAVE_LOAD_FUNC(METHOD, VALUE) ({				\
      ASSERT_SAVE_LOAD_TYPE (METHOD, int_int_func_t, VALUE, PTR_CMP);	\
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_func_inline_t, VALUE, PTR_CMP); \
      ASSERT_SAVE_LOAD_TYPE (METHOD, struct_func_type_t, VALUE, PTR_CMP); \
    })

TYPEDEF_FUNC (int, int_int_func_t, (int))
TYPEDEF_STRUCT (struct_func_inline_t, (int, func, (int)))
TYPEDEF_STRUCT (struct_func_type_t, (int_int_func_t, func))

int public_int_int_func (int x) { return (x); }
static int private_int_int_func (int x) { return (x); }

START_TEST (public_func) { ALL_METHODS (ASSERT_SAVE_LOAD_FUNC, public_int_int_func); } END_TEST
START_TEST (private_func) { ALL_METHODS (ASSERT_SAVE_LOAD_FUNC, private_int_int_func); } END_TEST

MAIN_TEST_SUITE ((public_func, "public function"),
		 (private_func, "private function")
		 );

