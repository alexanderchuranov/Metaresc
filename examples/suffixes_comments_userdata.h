
#include <metaresc.h>

TYPEDEF_STRUCT (example_t,
		/* optional argument with meta data for type itself */
		ATTRIBUTES (__attribute__((packed)), "Comment on type", { "auxiliary void pointer" }, "char" /* type of aux ptr */),
		/* all 4 arguments of ATTRIBUTES are optional */
                (int, x1), /* (type, name) */
                (int, x2, [3]), /* (type, name, suffix) */
                (double, x3, /* empty */, "Text comment for the field"),  /* (type, name, suffix, comment) */
                (float, x4, /* empty */, /* empty */, { "auxiliary void pointer" }, "char"), /* (type, name, suffix, comment, { void_ptr }) */
		 /* (type, name, suffix, comment, { void_ptr }, type_of_void_ptr_as_string) */
                (long double, x5, /* empty */, /* empty */, { (example_t[]){ { .x1 = 1, .x3 = 1.0 } } }, "example_t"),
		int x6,
		volatile const unsigned long long int x7,
		string_t x8
                )
