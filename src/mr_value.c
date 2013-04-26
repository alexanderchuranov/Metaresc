/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#ifdef HAVE_CONFIG_H
#include <mr_config.h>
#endif /* HAVE_CONFIG_H */
#include <string.h>
#ifdef HAVE_DLFCN_H
#define __USE_GNU
#include <dlfcn.h>
#endif /* HAVE_DLFCN_H */

#include <metaresc.h>
#include <mr_value.h>

static void
mr_value_id (mr_value_t * value)
{
  if (MR_VT_ID != value->value_type)
    return;

  value->value_type = MR_VT_INT;
  if (NULL == value->vt_string)
    value->vt_int = 0;
  else
    {
      mr_fd_t * fdp = mr_get_enum_by_name (value->vt_string);
      typeof (value->vt_int) vt_int = 0;
      if (NULL == fdp)
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNKNOWN_ENUM, value->vt_string);
      else
	vt_int = fdp->param.enum_value;
      MR_FREE (value->vt_string);
      value->vt_int = vt_int;
    }
}

#define MR_VALUE_CAST(VT_VALUE)						\
   switch (value->value_type)						\
     {									\
     case MR_VT_INT: value->VT_VALUE = value->vt_int; break;		\
     case MR_VT_FLOAT: value->VT_VALUE = value->vt_float; break;	\
     case MR_VT_COMPLEX: value->VT_VALUE = value->vt_complex; break;	\
     case MR_VT_UNKNOWN:						\
       if (NULL == value->vt_string)					\
	 {								\
	   MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_NULL_POINTER);	\
	   status = EXIT_FAILURE;					\
	 }								\
       else								\
	 {								\
	   char * unknown = value->vt_string;				\
	   typeof (value->VT_VALUE) vt_value = 0;			\
	   if (0 == MR_LOAD_CINIT (typeof (value->VT_VALUE), unknown, &vt_value)) \
	     {								\
	       MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_WRONG_EXPRESSION, unknown); \
	       status = EXIT_FAILURE;					\
	     }								\
	   else								\
	     {								\
	       value->VT_VALUE = vt_value;				\
	       MR_FREE (unknown);					\
	     }								\
	 }								\
       break;								\
     default:								\
       MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_WRONG_RESULT_TYPE);		\
       status = EXIT_FAILURE;						\
       break;								\
     }


int
mr_value_cast (mr_value_type_t value_type, mr_value_t * value)
{
  int status = EXIT_SUCCESS;
  mr_value_id (value);
  switch (value_type)
    {
    case MR_VT_INT:
      MR_VALUE_CAST (vt_int);
      break;
      
    case MR_VT_FLOAT:
      MR_VALUE_CAST (vt_float);
      break;
      
    case MR_VT_COMPLEX:
      MR_VALUE_CAST (vt_complex);
      break;
      
    default:
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_WRONG_RESULT_TYPE);
      status = EXIT_FAILURE;
      break;
    }
  if (EXIT_SUCCESS == status)
    value->value_type = value_type;
  return (status);
}

int
mr_value_neg (mr_value_t * value)
{
  mr_value_id (value);
  switch (value->value_type)
    {
    case MR_VT_INT: value->vt_int = -value->vt_int; break;
    case MR_VT_FLOAT: value->vt_float = -value->vt_float; break;
    case MR_VT_COMPLEX: value->vt_complex = -value->vt_complex; break;
    default:
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_WRONG_RESULT_TYPE);
      return (EXIT_FAILURE);
    }
  return (EXIT_SUCCESS);
}

#define MR_VALUE_OP(OP_NAME, OP, ...)					\
  int mr_value_ ## OP_NAME (mr_value_t * result, mr_value_t * left, mr_value_t * right) \
  {									\
    mr_value_type_t result_type = MR_MAX (left->value_type, right->value_type);	\
    memset (result, 0, sizeof (*result));				\
    if ((EXIT_SUCCESS != mr_value_cast (result_type, left)) ||		\
	(EXIT_SUCCESS != mr_value_cast (result_type, right)))		\
      {									\
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_WRONG_RESULT_TYPE);		\
	return (EXIT_FAILURE);						\
      }									\
    result->value_type = result_type;					\
    __VA_ARGS__;							\
    switch (result_type)						\
      {									\
      case MR_VT_INT: result->vt_int = left->vt_int OP right->vt_int; break; \
      case MR_VT_FLOAT: result->vt_float = left->vt_float OP right->vt_float; break; \
      case MR_VT_COMPLEX: result->vt_complex = left->vt_complex OP right->vt_complex; break; \
      default:								\
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_WRONG_RESULT_TYPE);		\
	return (EXIT_FAILURE);						\
      }									\
    return (EXIT_SUCCESS);						\
  }

#define MR_INT_VALUE_OP(OP_NAME, OP, ...)				\
  int mr_value_ ## OP_NAME (mr_value_t * result, mr_value_t * left, mr_value_t * right) \
  {									\
    memset (result, 0, sizeof (*result));				\
    mr_value_id (left);							\
    mr_value_id (right);						\
    if ((left->value_type != MR_VT_INT) || (right->value_type != MR_VT_INT)) \
      {									\
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_WRONG_RESULT_TYPE);		\
	return (EXIT_FAILURE);						\
      }									\
    result->value_type = MR_VT_INT;					\
    __VA_ARGS__;							\
    result->vt_int = left->vt_int OP right->vt_int;			\
    return (EXIT_SUCCESS);						\
  }

int
mr_value_is_zero (mr_value_t * value)
{
  switch (value->value_type)
    {
    case MR_VT_INT: return (0 == value->vt_int);
    case MR_VT_FLOAT: return (0 == value->vt_float);
    case MR_VT_COMPLEX: return (0 == value->vt_complex);
    default: MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_WRONG_RESULT_TYPE);
    }
  return (FALSE);
}
  
MR_VALUE_OP (add, +);
MR_VALUE_OP (sub, -);
MR_VALUE_OP (mul, *);
MR_VALUE_OP (div, /, if (mr_value_is_zero (right)) { MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_DIVISION_BY_ZERO); return (EXIT_FAILURE); });
MR_INT_VALUE_OP (mod, %, if (mr_value_is_zero (right)) { MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_DIVISION_BY_ZERO); return (EXIT_FAILURE); });
MR_INT_VALUE_OP (bit_or, |);
MR_INT_VALUE_OP (bit_and, &);
MR_INT_VALUE_OP (bit_xor, ^);
