#ifndef _MR_VALUE_H_
#define _MR_VALUE_H_

#include <stdbool.h>

#include <metaresc.h>

extern mr_status_t mr_value_to_mr_ptrdes (mr_ptrdes_t * ptrdes, mr_value_t * mr_value);
extern mr_status_t mr_ptrdes_to_mr_value (mr_value_t * mr_value, mr_ptrdes_t * ptrdes);

extern mr_status_t mr_value_cast (mr_value_type_t value_type, mr_value_t * value);
extern mr_status_t mr_value_neg (mr_value_t * value);
extern bool mr_value_is_zero (mr_value_t * value);
extern mr_status_t mr_value_add (mr_value_t * result, mr_value_t * left, mr_value_t * right);
extern mr_status_t mr_value_sub (mr_value_t * result, mr_value_t * left, mr_value_t * right);
extern mr_status_t mr_value_div (mr_value_t * result, mr_value_t * left, mr_value_t * right);
extern mr_status_t mr_value_mul (mr_value_t * result, mr_value_t * left, mr_value_t * right);
extern mr_status_t mr_value_mod (mr_value_t * result, mr_value_t * left, mr_value_t * right);
extern mr_status_t mr_value_bit_or (mr_value_t * result, mr_value_t * left, mr_value_t * right);
extern mr_status_t mr_value_bit_and (mr_value_t * result, mr_value_t * left, mr_value_t * right);
extern mr_status_t mr_value_bit_xor (mr_value_t * result, mr_value_t * left, mr_value_t * right);

#endif /* _MR_VALUE_H_ */
