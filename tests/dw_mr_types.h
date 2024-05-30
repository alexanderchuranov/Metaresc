#include <metaresc.h>

TYPEDEF_STRUCT (mr_pointer_t,
		(signed int **, int_dptr),
		(signed int * *, int_dptr_),
		(mr_type_t **, enum_dptr),
		(mr_type_t * *, enum_dptr_),
		);

#define MAKE_FIELD(TYPE) (TYPE, MR_PASTE2 (_, __COUNTER__)), (TYPE *, MR_PASTE2 (_, __COUNTER__)),

TYPEDEF_STRUCT (mr_builtin_types_t,
		MR_FOREACH (MAKE_FIELD, MR_BUILTIN_TYPES)
		);

typedef char * my_string_t;

TYPEDEF_STRUCT (mr_array_t,
		(signed int *, int_ptr_array, [2]),
		(mr_type_t *, enum_ptr_array, [2]),
		(my_string_t, my_string_array, [2]),
		(my_string_t *, my_string_ptrs_array, [2]),
		(char *, string_array, [2]),
		(char * *, string_ptrs_array, [2]),
		(int, array1d, [2]),
		(int, array2d, [2][3]),
		(int, array3d, [2][3][4]),
		(int, array4d, [2][3][4][1]),
		(int, array5d, [2][3][4][5][6]),
		);

TYPEDEF_STRUCT (mr_void_fields_t,
		VOID (mr_ptr_t, void_union),
		VOID (mr_array_t, void_struct),
		);

typedef struct mr_inline_enum_t {
  enum {_0} inline_enum;
} mr_inline_enum_t;

TYPEDEF_STRUCT (mr_anon_union_t,
		/* three embeded anonymous unions */
		ANON_UNION (),
		int _0,
		ANON_UNION (),
		int _1,
		ANON_UNION (),
		int _2,
		END_ANON_UNION (),
		END_ANON_UNION (),
		END_ANON_UNION (),
		int _3,
		/* one more anonymous union - should be named identically my macro and DWARF */
		ANON_UNION (),
		int _4,
		END_ANON_UNION (),
		/* named anonymous union. adds one more field and indexing of fields in macro and DWARF after this field mismatching */ 
		ANON_UNION (name),
		int _5,
		END_ANON_UNION (),
		int _6,
		/* one more anonymous union to check that test skip anonymous unions after named anonymous unions */
		ANON_UNION (),
		int _7,
		END_ANON_UNION (),
		);

TYPEDEF_STRUCT (mr_bitfields_t,
		(int, _8bits, : __CHAR_BIT__),
		(int, _7bits, : __CHAR_BIT__ - 1),
		);
