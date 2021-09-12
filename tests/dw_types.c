#include <metaresc.h>

TYPEDEF_STRUCT (mr_array_t,
		(signed int *, int_ptr_array, [2]),
		(mr_type_t *, enum_ptr_array, [2]),
		(int, array1d, [2]),
		(int, array2d, [2][3]),
		(int, array3d, [2][3][4]),
		)

mr_array_t mr_array;

TYPEDEF_STRUCT (mr_void_fields_t,
		VOID (mr_ptr_t, void_union),
		VOID (mr_array_t, void_struct),
		)

mr_void_fields_t mr_void_fields;

TYPEDEF_STRUCT (mr_bitfields_t,
		BITFIELD (int, _8bits, : __CHAR_BIT__),
		BITFIELD (int, _7bits, : __CHAR_BIT__ - 1),
		)

mr_bitfields_t mr_bitfields;

int __attribute__ ((weak)) main () { return (0); }
