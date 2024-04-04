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

TYPEDEF_STRUCT (mr_bitfields_t,
		BITFIELD (int, _8bits, : __CHAR_BIT__),
		BITFIELD (int, _7bits, : __CHAR_BIT__ - 1),
		);
