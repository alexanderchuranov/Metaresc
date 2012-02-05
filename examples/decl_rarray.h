
#include <metaresc.h>

TYPEDEF_STRUCT (employee_t,
                (char *, firstname),
                (char *, lastname),
                int salary
                )

TYPEDEF_STRUCT (organization_t,
                (char *, name),
                RARRAY (employee_t, employees)
                )
