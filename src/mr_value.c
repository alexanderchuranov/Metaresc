/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#ifdef HAVE_CONFIG_H
#include <mr_config.h>
#endif /* HAVE_CONFIG_H */
#include <string.h>

#include <metaresc.h>

int
mr_value_cast (mr_value_type_t value_type, mr_value_t * value)
{
  int status = EXIT_SUCCESS;
  switch (value_type)
    {
    case MR_VT_INT:
      switch (value->value_type)
	{
	case MR_VT_INT: value->vt_int = value->vt_int; break;
	case MR_VT_FLOAT: value->vt_int = value->vt_float; break;
	case MR_VT_COMPLEX: value->vt_int = value->vt_complex; break;
	default:
	  MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_WRONG_RESULT_TYPE);
	  status = EXIT_FAILURE;
	  break;
	}
      break;
    case MR_VT_FLOAT:
      switch (value->value_type)
	{
	case MR_VT_INT: value->vt_float = value->vt_int; break;
	case MR_VT_FLOAT: value->vt_float = value->vt_float; break;
	case MR_VT_COMPLEX: value->vt_float = value->vt_complex; break;
	default:
	  MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_WRONG_RESULT_TYPE);
	  status = EXIT_FAILURE;
	  break;
	}
      break;
    case MR_VT_COMPLEX:
      switch (value->value_type)
	{
	case MR_VT_INT: value->vt_complex = value->vt_int; break;
	case MR_VT_FLOAT: value->vt_complex = value->vt_float; break;
	case MR_VT_COMPLEX: value->vt_complex = value->vt_complex; break;
	default:
	  MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_WRONG_RESULT_TYPE);
	  status = EXIT_FAILURE;
	  break;
	}
      break;
    default:
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_WRONG_RESULT_TYPE);
      status = EXIT_FAILURE;
      break;
    }
  return (status);
}

void
mr_value_neg (mr_value_t * result, mr_value_t * value)
{
  memset (result, 0, sizeof (*result));
  switch (value->value_type)
    {
    case MR_VT_INT: value->vt_int = -value->vt_int; break;
    case MR_VT_FLOAT: value->vt_float = -value->vt_float; break;
    case MR_VT_COMPLEX: value->vt_complex = -value->vt_complex; break;
    default: MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_WRONG_RESULT_TYPE); break;
    }
}

#define MR_VALUE_OP(OP_NAME, OP, ...)					\
  void mr_value_ ## OP_NAME (mr_value_t * result, mr_value_t * left, mr_value_t * right) \
  {									\
    memset (result, 0, sizeof (*result));				\
    mr_value_type_t result_type = MR_MAX (left->value_type, right->value_type);	\
    if ((result_type != MR_VT_INT) && (result_type != MR_VT_FLOAT) && (result_type != MR_VT_COMPLEX)) \
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_WRONG_RESULT_TYPE);		\
    else								\
      {									\
	mr_value_cast (result_type, left);				\
	mr_value_cast (result_type, right);				\
	result->value_type = result_type;				\
	__VA_ARGS__;							\
	switch (result_type)						\
	  {								\
	  case MR_VT_INT: result->vt_int = left->vt_int OP right->vt_int; break; \
	  case MR_VT_FLOAT: result->vt_float = left->vt_float OP right->vt_float; break; \
	  case MR_VT_COMPLEX: result->vt_complex = left->vt_complex OP right->vt_complex; break; \
	  default: MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_WRONG_RESULT_TYPE); break; \
	  }								\
      }									\
  }

#define MR_INT_VALUE_OP(OP_NAME, OP, ...)				\
  void mr_value_ ## OP_NAME (mr_value_t * result, mr_value_t * left, mr_value_t * right) \
  {									\
    memset (result, 0, sizeof (*result));				\
    if ((left->value_type != MR_VT_INT) || (right->value_type != MR_VT_INT)) \
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_WRONG_RESULT_TYPE);		\
    else								\
      {									\
	result->value_type = MR_VT_INT;					\
	__VA_ARGS__;							\
	result->vt_int = left->vt_int OP right->vt_int;			\
      }									\
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
MR_VALUE_OP (div, /, if (mr_value_is_zero (right)) { MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_DIVISION_BY_ZERO); return; });
MR_INT_VALUE_OP (mod, %, if (mr_value_is_zero (right)) { MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_DIVISION_BY_ZERO); return; });
MR_INT_VALUE_OP (bit_or, |);
MR_INT_VALUE_OP (bit_and, &);
MR_INT_VALUE_OP (bit_xor, ^);
