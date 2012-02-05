
#ifndef _METARESC_EXAMPLES_SEPARATE_EMPLOYEE_H_INCLUDED_
#define _METARESC_EXAMPLES_SEPARATE_EMPLOYEE_H_INCLUDED_

#define MR_MODE PROTO
#include "separate_employee_decl.h"

employee_t create_employee (
  char const * firstname,
  char const * lastname,
  int salary);

#endif /* _METARESC_EXAMPLES_SEPARATE_EMPLOYEE_H_INCLUDED_ */
