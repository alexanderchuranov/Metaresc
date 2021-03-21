/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#ifdef HAVE_CONFIG_H
#include <mr_config.h>
#endif /* HAVE_CONFIG_H */

#include <string.h>
#include <stdbool.h>

#include <metaresc.h>
#include <mr_value.h>

/**
 * Read enum value from string
 * @param data pointer on place to save value
 * @param str string with enum
 * @return A pointer on the rest of parsed string
 */
static char *
mr_get_enum (char * str, uint64_t * data)
{
  char * name = str;

  while (isalnum (*str) || (*str == '_'))
    ++str;
  
  size_t size = str - name;

  if (size > mr_conf.enum_max_length)
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNKNOWN_ENUM, name);
      return (NULL);
    }
  
  char name_[size + 1];
  memcpy (name_, name, size);
  name_[size] = 0;

  if (0 == strcmp ("false", name_))
    *data = false;
  else if (0 == strcmp ("true", name_))
    *data = true;
  else
    {
      mr_fd_t * fdp = mr_get_enum_by_name (name_);
      if (NULL == fdp)
	{
	  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNKNOWN_ENUM, name_);
	  return (NULL);
	}
      *data = fdp->param.enum_param._unsigned;
    }
  return (str);
}

/**
 * Read int value from string (may be as ENUM)
 * @param str string with int
 * @param data pointer on place to save int
 * @return A pointer on the rest of parsed string
 */
static char *
mr_get_int (char * str, uint64_t * data)
{
  int offset;
  while (isspace (*str))
    ++str;
  if (isalpha (*str))
    str = mr_get_enum (str, data);
  else if ('0' == *str)
    {
      if ('x' == str[1])
	{
	  if (1 != sscanf (str, "%" SCNx64 "%n", data, &offset))
	    {
	      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_INT, str);
	      return (NULL);
	    }
	}
      else
	{
	  if (1 != sscanf (str, "%" SCNo64 "%n", data, &offset))
	    {
	      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_INT, str);
	      return (NULL);
	    }
	}
      str += offset;
    }
  else
    {
      if ((1 == sscanf (str, "%" SCNu64 "%n", data, &offset)) || (1 == sscanf (str, "%" SCNd64 "%n", data, &offset)))
	str += offset;
      else
	{
	  MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_INT, str);
	  return (NULL);
	}
    }
  return (str);
}

static char * 
mr_load_bitmask (char * str, mr_value_t * mr_value)
{
  uint64_t accum = 0;
  for (;;)
    {
      uint64_t value;
      str = mr_get_int (str, &value);
      if (NULL == str)
	return (NULL);
      accum |= value;
      while (isspace (*str))
	++str;
      if (*str != '|')
	break;
      ++str;
    }
  mr_value->vt_int = accum;
  return (str);
}

static char *
mr_load_long_double (char * str, mr_value_t * mr_value)
{
  int offset;
  if (1 != sscanf (str, "%Lg%n", &mr_value->vt_float, &offset))
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_LONG_DOUBLE, str);
      return (NULL);
    }
  return (&str[offset]);
}

static char *
mr_load_complex_long_double (char * str, mr_value_t * mr_value)
{
  int offset;
  ieee_long_double_t real, imag;

  if (str[0] == 'I')
    {
      real.long_double = 0;
      imag.long_double = 1;
      mr_value->vt_complex[MR_REAL] = real.ieee_854_long_double;
      mr_value->vt_complex[MR_IMAG] = imag.ieee_854_long_double;
      return (&str[1]);
    }

  if (1 != sscanf (str, "%Lg%n", &real.long_double, &offset))
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_COMPLEX_LONG_DOUBLE, str);
      return (NULL);
    }

  char * tail = &str[offset];
  if (tail[0] == 'I')
    {
      imag.long_double = 0;
      mr_value->vt_complex[MR_REAL] = imag.ieee_854_long_double;
      mr_value->vt_complex[MR_IMAG] = real.ieee_854_long_double;
      return (&tail[1]);
    }

  if ((tail[0] != ' ') || (tail[1] != '+') || (tail[2] != ' '))
    {
      imag.long_double = 0;
      mr_value->vt_complex[MR_REAL] = real.ieee_854_long_double;
      mr_value->vt_complex[MR_IMAG] = imag.ieee_854_long_double;
      return (tail);
    }
  tail += 3;
  if (tail[0] == 'I')
    {
      imag.long_double = 1;
      mr_value->vt_complex[MR_REAL] = real.ieee_854_long_double;
      mr_value->vt_complex[MR_IMAG] = imag.ieee_854_long_double;
      return (tail);
    }

  if (1 != sscanf (tail, "%LgI%n", &imag.long_double, &offset))
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_COMPLEX_LONG_DOUBLE, str);
      return (NULL);
    }

  mr_value->vt_complex[MR_REAL] = real.ieee_854_long_double;
  mr_value->vt_complex[MR_IMAG] = imag.ieee_854_long_double;
  return (&tail[offset]);
}

static mr_status_t
mr_load_var (char * str, void * arg)
{
  mr_value_t * mr_value = arg;
  
  if ((NULL == str) || (NULL == mr_value))
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
      return (MR_FAILURE);
    }
  
  switch (mr_value->value_type)
    {
    case MR_VT_INT:
      str = mr_load_bitmask (str, mr_value);
      break;
    case MR_VT_FLOAT:
      str = mr_load_long_double (str, mr_value);
      break;
    case MR_VT_COMPLEX:
      str = mr_load_complex_long_double (str, mr_value);
      break;
    default:
      break;
    }
  
  if (NULL == str)
    return (MR_FAILURE);
  while (isspace (*str))
    ++str;
  if (*str != 0)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_DATA_AT_THE_END, str);
      return (MR_FAILURE);
    }
  return (MR_SUCCESS);
}

static mr_status_t
mr_value_id_to_int (char * str, void * arg)
{
  mr_value_t * mr_value = arg;
  mr_fd_t * fdp = mr_get_enum_by_name (str);
    
  if (NULL == fdp)
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNKNOWN_ENUM, str);
      return (MR_FAILURE);
    }
  
  mr_value->value_type = MR_VT_INT;
  mr_value->vt_int = fdp->param.enum_param._signed;
  return (MR_SUCCESS);
}

static mr_status_t
mr_value_id (mr_value_t * mr_value)
{
  if (MR_VT_ID != mr_value->value_type)
    return (MR_SUCCESS);

  return (mr_process_quoted_str (&mr_value->vt_quoted_substr, mr_value_id_to_int, mr_value));
}

mr_status_t
mr_value_cast (mr_value_type_t value_type, mr_value_t * mr_value)
{
  mr_status_t status = MR_SUCCESS;

  switch (mr_value->value_type)
    {
    case MR_VT_ID:
      status = mr_value_id (mr_value);
      if (MR_SUCCESS != status)
	return (status);
      /* proceeding as int */
      
    case MR_VT_INT:
      switch (value_type)
	{
	case MR_VT_INT: break;
	case MR_VT_FLOAT: mr_value->vt_float = mr_value->vt_int; break;
	case MR_VT_COMPLEX:
	  {
	    complex_long_double_t vt_complex = mr_value->vt_int;
	    mr_value->vt_complex[MR_REAL] = (ieee_long_double_t) { .long_double = __real__ vt_complex }.ieee_854_long_double;
	    mr_value->vt_complex[MR_IMAG] = (ieee_long_double_t) { .long_double = __imag__ vt_complex }.ieee_854_long_double;
	    break;
	  }
	  
	default: status = MR_FAILURE; break;
	}
      break;
      
    case MR_VT_FLOAT:
      switch (value_type)
	{
	case MR_VT_INT: mr_value->vt_int = mr_value->vt_float; break;
	case MR_VT_FLOAT: break;
	case MR_VT_COMPLEX:
	  {
	    complex_long_double_t vt_complex = mr_value->vt_float;
	    mr_value->vt_complex[MR_REAL] = (ieee_long_double_t) { .long_double = __real__ vt_complex }.ieee_854_long_double;
	    mr_value->vt_complex[MR_IMAG] = (ieee_long_double_t) { .long_double = __imag__ vt_complex }.ieee_854_long_double;
	    break;
	  }
	  
	default: status = MR_FAILURE; break;
	}
      break;
      
    case MR_VT_COMPLEX:
      switch (value_type)
	{
	case MR_VT_INT: mr_value->vt_int = (ieee_long_double_t) { .ieee_854_long_double = mr_value->vt_complex[MR_REAL] }.long_double; break;
	case MR_VT_FLOAT: mr_value->vt_float = (ieee_long_double_t) { .ieee_854_long_double = mr_value->vt_complex[MR_REAL] }.long_double; break;
	case MR_VT_COMPLEX: break;
	default: status = MR_FAILURE; break;
	}
      break;
      
    case MR_VT_QUOTED_SUBSTR:
      {
	mr_value_t result = { .value_type = value_type };
	status = mr_process_quoted_str (&mr_value->vt_quoted_substr, mr_load_var, &result);
	if (MR_SUCCESS == status)
	  *mr_value = result;
	break;
      }
      
    default:
      status = MR_FAILURE;
      break;
    }
  
  if (MR_SUCCESS == status)
    mr_value->value_type = value_type;
  else
    MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_WRONG_RESULT_TYPE);

  return (status);
}

mr_status_t
mr_value_neg (mr_value_t * value)
{
  switch (value->value_type)
    {
    case MR_VT_ID:
      if (MR_SUCCESS != mr_value_id (value))
	return (MR_FAILURE);
      /* proceeding as int */
    case MR_VT_INT:
      value->vt_int = -value->vt_int;
      break;
      
    case MR_VT_FLOAT:
      value->vt_float = -value->vt_float;
      break;
      
    case MR_VT_COMPLEX:
      {
	complex_long_double_t vt_complex;
	__real__ vt_complex = (ieee_long_double_t) { .ieee_854_long_double = value->vt_complex[MR_REAL] }.long_double;
	__imag__ vt_complex = (ieee_long_double_t) { .ieee_854_long_double = value->vt_complex[MR_IMAG] }.long_double;
	vt_complex = -vt_complex;
	value->vt_complex[MR_REAL] = (ieee_long_double_t) { .long_double = __real__ vt_complex }.ieee_854_long_double;
	value->vt_complex[MR_IMAG] = (ieee_long_double_t) { .long_double = __imag__ vt_complex }.ieee_854_long_double;
      }
      break;
      
    default:
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_WRONG_RESULT_TYPE);
      return (MR_FAILURE);
    }
  return (MR_SUCCESS);
}

#define MR_VALUE_OP(OP_NAME, OP, ...)					\
  mr_status_t mr_value_ ## OP_NAME (mr_value_t * result, mr_value_t * left, mr_value_t * right) \
  {									\
    mr_value_type_t result_type = MR_MAX (left->value_type, right->value_type);	\
    if (result_type < MR_VT_INT)					\
      result_type = MR_VT_INT;						\
    memset (result, 0, sizeof (*result));				\
    if ((MR_SUCCESS != mr_value_cast (result_type, left)) ||		\
	(MR_SUCCESS != mr_value_cast (result_type, right)))		\
      {									\
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_WRONG_RESULT_TYPE);		\
	return (MR_FAILURE);						\
      }									\
    result->value_type = result_type;					\
    __VA_ARGS__;							\
    switch (result_type)						\
      {									\
      case MR_VT_INT: result->vt_int = left->vt_int OP right->vt_int; break; \
      case MR_VT_FLOAT: result->vt_float = left->vt_float OP right->vt_float; break; \
      case MR_VT_COMPLEX:						\
	{								\
	  complex_long_double_t _left, _right, _result;			\
	  __real__ _left = (ieee_long_double_t) { .ieee_854_long_double = left->vt_complex[MR_REAL] }.long_double; \
	  __imag__ _left = (ieee_long_double_t) { .ieee_854_long_double = left->vt_complex[MR_IMAG] }.long_double; \
	  __real__ _right = (ieee_long_double_t) { .ieee_854_long_double = right->vt_complex[MR_REAL] }.long_double; \
	  __imag__ _right = (ieee_long_double_t) { .ieee_854_long_double = right->vt_complex[MR_IMAG] }.long_double; \
	  _result = _left OP _right;					\
	  result->vt_complex[MR_REAL] = (ieee_long_double_t) { .long_double = __real__ _result }.ieee_854_long_double; \
	  result->vt_complex[MR_IMAG] = (ieee_long_double_t) { .long_double = __imag__ _result }.ieee_854_long_double; \
	}								\
	break;								\
      default:								\
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_WRONG_RESULT_TYPE);		\
	return (MR_FAILURE);						\
      }									\
    return (MR_SUCCESS);						\
  }

#define MR_INT_VALUE_OP(OP_NAME, OP, ...)				\
  mr_status_t mr_value_ ## OP_NAME (mr_value_t * result, mr_value_t * left, mr_value_t * right) \
  {									\
    memset (result, 0, sizeof (*result));				\
    if (MR_SUCCESS != mr_value_id (left))				\
      return (MR_FAILURE);						\
    if (MR_SUCCESS != mr_value_id (right))				\
      return (MR_FAILURE);						\
    if ((left->value_type != MR_VT_INT) || (right->value_type != MR_VT_INT)) \
      {									\
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_WRONG_RESULT_TYPE);		\
	return (MR_FAILURE);						\
      }									\
    result->value_type = MR_VT_INT;					\
    __VA_ARGS__;							\
    result->vt_int = left->vt_int OP right->vt_int;			\
    return (MR_SUCCESS);						\
  }

bool
mr_value_is_zero (mr_value_t * value)
{
  switch (value->value_type)
    {
    case MR_VT_INT: return (0 == value->vt_int);
    case MR_VT_FLOAT: return (0 == value->vt_float);
    case MR_VT_COMPLEX: return ((0 == (ieee_long_double_t) { .ieee_854_long_double = value->vt_complex[MR_REAL] }.long_double) &&
				(0 == (ieee_long_double_t) { .ieee_854_long_double = value->vt_complex[MR_IMAG] }.long_double));
    default: MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_WRONG_RESULT_TYPE);
    }
  return (false);
}
  
MR_VALUE_OP (add, +);
MR_VALUE_OP (sub, -);
MR_VALUE_OP (mul, *);
MR_VALUE_OP (div, /, if (mr_value_is_zero (right)) { MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_DIVISION_BY_ZERO); return (MR_FAILURE); });
MR_INT_VALUE_OP (mod, %, if (mr_value_is_zero (right)) { MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_DIVISION_BY_ZERO); return (MR_FAILURE); });
MR_INT_VALUE_OP (bit_or, |);
MR_INT_VALUE_OP (bit_and, &);
MR_INT_VALUE_OP (bit_xor, ^);
