/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include <metaresc.h>
#include <mr_value.h>
#include <mr_ic.h>

mr_status_t
mr_value_to_mr_ptrdes (mr_ptrdes_t * ptrdes, mr_value_t * mr_value)
{
  ptrdes->value_type = mr_value->value_type;
  switch (mr_value->value_type)
    {
    case MR_VT_CHAR: ptrdes->load_params.vt_char = mr_value->vt_char; break;
    case MR_VT_STRING: ptrdes->load_params.vt_string = mr_value->vt_string; break;
    case MR_VT_SUBSTR: ptrdes->load_params.vt_substr = mr_value->vt_substr; break;
    case MR_VT_ID: ptrdes->load_params.vt_substr = mr_value->vt_substr; break;
    case MR_VT_INT: memcpy (&ptrdes->load_params.vt_int, &mr_value->vt_int, sizeof (mr_value->vt_int)); break;
    case MR_VT_FLOAT: memcpy (&ptrdes->load_params.vt_float, &mr_value->vt_float, sizeof (mr_value->vt_float)); break;
    case MR_VT_COMPLEX:
      ptrdes->load_params.vt_complex = MR_CALLOC (1, sizeof (*ptrdes->load_params.vt_complex));
      if (NULL == ptrdes->load_params.vt_complex)
	return (MR_FAILURE);
      *ptrdes->load_params.vt_complex = mr_value->vt_complex;
      break;
    default:
      return (MR_FAILURE);
    }
  return (MR_SUCCESS);
}

mr_status_t
mr_ptrdes_to_mr_value (mr_value_t * mr_value, mr_ptrdes_t * ptrdes)
{
  memset (mr_value, 0, sizeof (*mr_value));
  mr_value->value_type = ptrdes->value_type;
  switch (mr_value->value_type)
    {
    case MR_VT_CHAR: mr_value->vt_char = ptrdes->load_params.vt_char; break;
    case MR_VT_STRING: mr_value->vt_string = ptrdes->load_params.vt_string; break;
    case MR_VT_SUBSTR: mr_value->vt_substr = ptrdes->load_params.vt_substr; break;
    case MR_VT_ID: mr_value->vt_substr = ptrdes->load_params.vt_substr; break;
    case MR_VT_INT: memcpy (&mr_value->vt_int, &ptrdes->load_params.vt_int, sizeof (mr_value->vt_int)); break;
    case MR_VT_FLOAT: memcpy (&mr_value->vt_float, &ptrdes->load_params.vt_float, sizeof (mr_value->vt_float)); break;
    case MR_VT_COMPLEX: mr_value->vt_complex = *ptrdes->load_params.vt_complex; break;
    default:
      return (MR_FAILURE);
    }
  return (MR_SUCCESS);
}

static mr_status_t
enum_names_visitor (mr_ptr_t key, const void * context)
{
  mr_ed_t * edp = key.ptr;
  int enum_name_length = strlen (edp->name.str);
  int * enum_max_length = (int*)context;
  if (enum_name_length > *enum_max_length)
    *enum_max_length = enum_name_length;
  return (MR_SUCCESS);
}

/**
 * Read enum value from string
 * @param data pointer on place to save value
 * @param str string with enum
 * @return A pointer on the rest of parsed string
 */
static char *
mr_get_enum (char * str, mr_uintmax_t * data)
{
  static int enum_max_length = 0;
  if (0 == enum_max_length)
    mr_ic_foreach (&mr_conf.enum_by_name, enum_names_visitor, &enum_max_length);
  
  char * name = str;
  int size = 0;

  while (isalnum (*str) || ('_' == *str))
    {
      ++str;
      if (++size > enum_max_length)
	{
	  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNKNOWN_ENUM, name);
	  return (NULL);
	}
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
      mr_ed_t * edp = mr_get_enum_by_name (name_);

      if (NULL == edp)
	{
	  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNKNOWN_ENUM, name_);
	  return (NULL);
	}

      switch (edp->mr_type)
	{
	case MR_TYPE_INT8:
	case MR_TYPE_INT16:
	case MR_TYPE_INT32:
	case MR_TYPE_INT64:
	  *data = edp->value._signed;
	  break;

	case MR_TYPE_UINT8:
	case MR_TYPE_UINT16:
	case MR_TYPE_UINT32:
	case MR_TYPE_UINT64:
	  *data = edp->value._unsigned;
	  break;

	default:
	  return (NULL);
	}
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
mr_get_int (char * str, mr_uintmax_t * data)
{
  while (isspace (*str))
    ++str;
  if (isalpha (*str) || ('_' == *str))
    str = mr_get_enum (str, data);
  else
    *data = mr_strtouintmax (str, &str, 0);
  return (str);
}

static char * 
mr_load_bitmask (char * str, mr_value_t * mr_value)
{
  mr_uintmax_t accum = 0;
  for (;;)
    {
      mr_uintmax_t value;
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
  char * next = NULL;
  mr_value->vt_float = strtold (str, &next);
  if (NULL == next)
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_READ_LONG_DOUBLE, str);
      return (NULL);
    }
  return (next);
}

static char *
mr_load_complex_long_double (char * str, mr_value_t * mr_value)
{
  complex_long_double_t cld = 0;
  long_double_t real, imag;
  char * next = NULL;

  while (isspace (*str))
    ++str;
  
  if (*str == 'I')
    {
      __imag__ cld = 1;
      mr_value->vt_complex = cld;
      return (str + 1);
    }

  real = strtold (str, &next);
  if (NULL == next)
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_READ_COMPLEX_LONG_DOUBLE, str);
      return (NULL);
    }
  str = next;

  if (*str == 'I')
    {
      __imag__ cld = real;
      mr_value->vt_complex = cld;
      return (str + 1);
    }

  __real__ cld = real;

  while (isspace (*str))
    ++str;
  
  if (*str != '+')
    {
      mr_value->vt_complex = cld;
      return (str);
    }

  ++str;
  while (isspace (*str))
    ++str;
  
  if (*str == 'I')
    {
      __imag__ cld = 1;
      mr_value->vt_complex = cld;
      return (str + 1);
    }

  imag = strtold (str, &next);
  if (NULL == next)
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_READ_COMPLEX_LONG_DOUBLE, str);
      return (NULL);
    }
  str = next;
  __imag__ cld = imag;
  mr_value->vt_complex = cld;
  
  while (isspace (*str))
    ++str;
  
  if (*str == 'I')
    return (str + 1);

  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_READ_COMPLEX_LONG_DOUBLE, str);
  return (NULL);
}

static char *
mr_load_var (mr_value_t * mr_value, char * str)
{
  if ((NULL == str) || (NULL == mr_value))
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
      return (NULL);
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
    return (NULL);
  while (isspace (*str))
    ++str;
  return (str);
}

static mr_status_t
mr_value_id (mr_value_t * mr_value)
{
  if (NULL == mr_value->vt_substr.str)
    return (MR_FAILURE);
  mr_uintmax_t value;
  char * tail = mr_get_enum (mr_value->vt_substr.str, &value);
  if ((NULL == tail) || (tail - mr_value->vt_substr.str != mr_value->vt_substr.length))
    {
      int length = tail ? (tail - mr_value->vt_substr.str) : mr_value->vt_substr.length;
      length = MR_MIN (length, (1 << 6));
      char buf[length + sizeof (char)];
      memcpy (buf, mr_value->vt_substr.str, length);
      buf[length] = 0;
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_DATA_AT_THE_END, buf);
      return (MR_FAILURE);
    }
  mr_value->value_type = MR_VT_INT;
  mr_value->vt_int = value;
  return (MR_SUCCESS);
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
	case MR_VT_COMPLEX: mr_value->vt_complex = mr_value->vt_int; break;
	default: status = MR_FAILURE; break;
	}
      break;
      
    case MR_VT_FLOAT:
      switch (value_type)
	{
	case MR_VT_INT: mr_value->vt_int = mr_value->vt_float; break;
	case MR_VT_FLOAT: break;
	case MR_VT_COMPLEX: mr_value->vt_complex = mr_value->vt_float; break;
	default: status = MR_FAILURE; break;
	}
      break;
      
    case MR_VT_COMPLEX:
      switch (value_type)
	{
	case MR_VT_INT: mr_value->vt_int = mr_value->vt_complex; break;
	case MR_VT_FLOAT: mr_value->vt_float = mr_value->vt_complex; break;
	case MR_VT_COMPLEX: break;
	default: status = MR_FAILURE; break;
	}
      break;
      
    case MR_VT_SUBSTR:
      {
	mr_value_t result = { .value_type = value_type };
	if (NULL == mr_value->vt_substr.str)
	  return (MR_FAILURE);
	char * tail = mr_load_var (&result, mr_value->vt_substr.str);
	if ((NULL == tail) || (tail - mr_value->vt_substr.str != mr_value->vt_substr.length))
	  {
	    int length = tail ? (tail - mr_value->vt_substr.str) : mr_value->vt_substr.length;
	    length = MR_MIN (length, (1 << 6));
	    char buf[length + sizeof (char)];
	    memcpy (buf, mr_value->vt_substr.str, length);
	    buf[length] = 0;
	    MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_DATA_AT_THE_END, buf);
	    return (MR_FAILURE);
	  }
	*mr_value = result;
	break;
      }

    case MR_VT_STRING:
      {
	mr_value_t result = { .value_type = value_type };
	if (NULL == mr_value->vt_string)
	  return (MR_FAILURE);
	char * tail = mr_load_var (&result, mr_value->vt_string);
	if ((NULL == tail) || (*tail))
	  {
	    MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_DATA_AT_THE_END, mr_value->vt_string);
	    return (MR_FAILURE);
	  }
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
    MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_WRONG_RESULT_TYPE);

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
      value->vt_complex = -value->vt_complex;
      break;
      
    default:
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_WRONG_RESULT_TYPE);
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
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_WRONG_RESULT_TYPE);		\
	return (MR_FAILURE);						\
      }									\
    result->value_type = result_type;					\
    __VA_ARGS__;							\
    switch (result_type)						\
      {									\
      case MR_VT_INT: result->vt_int = left->vt_int OP right->vt_int; break; \
      case MR_VT_FLOAT: result->vt_float = left->vt_float OP right->vt_float; break; \
      case MR_VT_COMPLEX: result->vt_complex = left->vt_complex OP right->vt_complex; break; \
      default:								\
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_WRONG_RESULT_TYPE);		\
	return (MR_FAILURE);						\
      }									\
    return (MR_SUCCESS);						\
  }

#define MR_INT_VALUE_OP(OP_NAME, OP, ...)				\
  mr_status_t mr_value_ ## OP_NAME (mr_value_t * result, mr_value_t * left, mr_value_t * right) \
  {									\
    memset (result, 0, sizeof (*result));				\
    if (MR_VT_ID == left->value_type)					\
      if (MR_SUCCESS != mr_value_id (left))				\
	return (MR_FAILURE);						\
    if (MR_VT_ID == right->value_type)					\
      if (MR_SUCCESS != mr_value_id (right))				\
	return (MR_FAILURE);						\
    if ((left->value_type != MR_VT_INT) || (right->value_type != MR_VT_INT)) \
      {									\
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_WRONG_RESULT_TYPE);		\
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
    case MR_VT_COMPLEX: return (0 == value->vt_complex);
    default: MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_WRONG_RESULT_TYPE);
    }
  return (false);
}
  
MR_VALUE_OP (add, +);
MR_VALUE_OP (sub, -);
MR_VALUE_OP (mul, *);
MR_VALUE_OP (div, /, if (mr_value_is_zero (right)) { MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_DIVISION_BY_ZERO); return (MR_FAILURE); });
MR_INT_VALUE_OP (mod, %, if (mr_value_is_zero (right)) { MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_DIVISION_BY_ZERO); return (MR_FAILURE); });
MR_INT_VALUE_OP (bit_or, |);
MR_INT_VALUE_OP (bit_and, &);
MR_INT_VALUE_OP (bit_xor, ^);
