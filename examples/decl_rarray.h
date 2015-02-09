
#include <metaresc.h>

TYPEDEF_STRUCT (employee_t,
                (char *, firstname),
                (char *, lastname),
                int salary
                )

TYPEDEF_STRUCT (organization_t,
                (char *, name),
		(employee_t *, employees, /* suffix */ , /* meta */ , { "size" }, "char"),
		(ssize_t, size),
                )
