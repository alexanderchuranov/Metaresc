#ifndef _MR_VALUE_H_
#define _MR_VALUE_H_

#include <metaresc.h>

extern int mr_value_cast (mr_value_type_t value_type, mr_value_t * value);
extern int mr_value_neg (mr_value_t * result, mr_value_t * value);
extern int mr_value_is_zero (mr_value_t * value);
extern int mr_value_add (mr_value_t * result, mr_value_t * left, mr_value_t * right);
extern int mr_value_sub (mr_value_t * result, mr_value_t * left, mr_value_t * right);
extern int mr_value_div (mr_value_t * result, mr_value_t * left, mr_value_t * right);
extern int mr_value_mul (mr_value_t * result, mr_value_t * left, mr_value_t * right);
extern int mr_value_mod (mr_value_t * result, mr_value_t * left, mr_value_t * right);
extern int mr_value_bit_or (mr_value_t * result, mr_value_t * left, mr_value_t * right);
extern int mr_value_bit_and (mr_value_t * result, mr_value_t * left, mr_value_t * right);
extern int mr_value_bit_xor (mr_value_t * result, mr_value_t * left, mr_value_t * right);

#endif /* _MR_VALUE_H_ */
