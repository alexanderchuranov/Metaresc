/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include <metaresc.h>
#include <mr_value.h>
#include <mr_ic.h>

/*
  GCC 12.x on platforms with long double support do an aggressive optimization and
  incorrectly assume that (x == (double) x) is always true. Here is a function that
  forces GCC to properly cast value and compare it with original value.
  This function is not static or inline intentionally - to avoid incorrect optimization.
*/
bool
mr_long_double_equals_double (long double x, double y)
{
  return (x == y);
}

bool
mr_complex_long_double_equals_complex_float (complex long double x, complex float y)
{
  return (x == y);
}

mr_status_t
mr_value_to_mr_ptrdes (mr_ptrdes_t * ptrdes, mr_value_t * mr_value, char * str)
{
  ptrdes->value_type = mr_value->value_type;
  switch (mr_value->value_type)
    {
    case MR_VT_CHAR:
      ptrdes->vt_char = mr_value->vt_char;
      break;

    case MR_VT_STRING:
    case MR_VT_UNALLOCATED_STRING:
      ptrdes->vt_string = mr_value->vt_string;
      break;

    case MR_VT_SUBSTR:
      ptrdes->vt_substr_pos.offset = mr_value->vt_substr.str - str;
      ptrdes->vt_substr_pos.length = mr_value->vt_substr.length;
      ptrdes->value_type = MR_VT_SUBSTR_POS;
      break;

    case MR_VT_SUBSTR_POS:
      ptrdes->vt_substr_pos = mr_value->vt_substr_pos;
      break;

    case MR_VT_INTMAX:
      if (mr_value->vt_intmax == (typeof (ptrdes->vt_intptr)) mr_value->vt_intmax)
	{
	  ptrdes->vt_intptr = mr_value->vt_intmax;
	  ptrdes->value_type = MR_VT_INTPTR;
	}
      else if (mr_value->vt_intmax == (typeof (ptrdes->vt_uintptr)) mr_value->vt_intmax)
	{
	  ptrdes->vt_uintptr = mr_value->vt_intmax;
	  ptrdes->value_type = MR_VT_UINTPTR;
	}
      else
	{
	  ptrdes->vt_intmax = MR_CALLOC (1, sizeof (*ptrdes->vt_intmax));
	  if (NULL == ptrdes->vt_intmax)
	    return (MR_FAILURE);
	  *ptrdes->vt_intmax = mr_value->vt_intmax;
	}
      break;

    case MR_VT_LONG_DOUBLE:
      if (mr_long_double_equals_double (mr_value->vt_long_double, mr_value->vt_long_double))
	{
	  ptrdes->vt_double = mr_value->vt_long_double;
	  ptrdes->value_type = MR_VT_DOUBLE;
	}
      else
	{
	  ptrdes->vt_long_double = MR_CALLOC (1, sizeof (*ptrdes->vt_long_double));
	  if (NULL == ptrdes->vt_long_double)
	    return (MR_FAILURE);
	  *ptrdes->vt_long_double = mr_value->vt_long_double;
	}
      break;

    case MR_VT_COMPLEX_LONG_DOUBLE:
      if (mr_complex_long_double_equals_complex_float (mr_value->vt_complex_long_double, mr_value->vt_complex_long_double))
	{
	  ptrdes->vt_complex_float = mr_value->vt_complex_long_double;
	  ptrdes->value_type = MR_VT_COMPLEX_FLOAT;
	}
      else
	{
	  ptrdes->vt_complex_long_double = MR_CALLOC (1, sizeof (*ptrdes->vt_complex_long_double));
	  if (NULL == ptrdes->vt_complex_long_double)
	    return (MR_FAILURE);
	  *ptrdes->vt_complex_long_double = mr_value->vt_complex_long_double;
	}
      break;

    default:
      return (MR_FAILURE);
    }
  return (MR_SUCCESS);
}

mr_status_t
mr_ptrdes_to_mr_value (mr_value_t * mr_value, mr_ptrdes_t * ptrdes, char * str)
{
  memset (mr_value, 0, sizeof (*mr_value));
  mr_value->value_type = ptrdes->value_type;
  switch (ptrdes->value_type)
    {
    case MR_VT_CHAR:
      mr_value->vt_char = ptrdes->vt_char;
      break;

    case MR_VT_STRING:
    case MR_VT_UNALLOCATED_STRING:
      mr_value->vt_string = ptrdes->vt_string;
      break;

    case MR_VT_SUBSTR_POS:
      if (str != NULL)
	{
	  mr_value->vt_substr.str = &str[ptrdes->vt_substr_pos.offset];
	  mr_value->vt_substr.length = ptrdes->vt_substr_pos.length;
	  mr_value->value_type = MR_VT_SUBSTR;
	}
      else
	{
	  mr_value->vt_intmax = 0;
	  mr_value->value_type = MR_VT_INTMAX;
	}
      break;

    case MR_VT_UINTPTR:
      mr_value->vt_intmax = ptrdes->vt_uintptr;
      mr_value->value_type = MR_VT_INTMAX;
      break;

    case MR_VT_INTPTR:
      mr_value->vt_intmax = ptrdes->vt_intptr;
      mr_value->value_type = MR_VT_INTMAX;
      break;

    case MR_VT_INTMAX:
      mr_value->vt_intmax = *ptrdes->vt_intmax;
      break;

    case MR_VT_DOUBLE:
      mr_value->vt_long_double = ptrdes->vt_double;
      mr_value->value_type = MR_VT_LONG_DOUBLE;
      break;

    case MR_VT_LONG_DOUBLE:
      mr_value->vt_long_double = *ptrdes->vt_long_double;
      break;

    case MR_VT_COMPLEX_FLOAT:
      mr_value->vt_complex_long_double = ptrdes->vt_complex_float;
      mr_value->value_type = MR_VT_COMPLEX_LONG_DOUBLE;
      break;

    case MR_VT_COMPLEX_LONG_DOUBLE:
      mr_value->vt_complex_long_double = *ptrdes->vt_complex_long_double;
      break;

    default:
      return (MR_FAILURE);
    }
  return (MR_SUCCESS);
}

static mr_status_t
enum_names_visitor (mr_ptr_t key, void * context)
{
  mr_ed_t * edp = key.ptr;
  int enum_name_length = strlen (edp->name.str);
  int * enum_max_length = context;
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
mr_get_enum (char * str, mr_intmax_t * data)
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

  *data = mr_get_enum_value_by_name (name_);
  return (str);
}

/**
 * Read int value from string (may be as ENUM)
 * @param str string with int
 * @param data pointer on place to save int
 * @return A pointer on the rest of parsed string
 */
static char *
mr_get_int (char * str, mr_intmax_t * data)
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
  mr_intmax_t accum = 0;
  for (;;)
    {
      mr_intmax_t value;
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
  mr_value->vt_intmax = accum;
  return (str);
}

static char *
mr_load_long_double (char * str, mr_value_t * mr_value)
{
  char * next = NULL;
  mr_value->vt_long_double = strtold (str, &next);
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
      mr_value->vt_complex_long_double = cld;
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
      mr_value->vt_complex_long_double = cld;
      return (str + 1);
    }

  __real__ cld = real;

  while (isspace (*str))
    ++str;
  
  if (*str != '+')
    {
      mr_value->vt_complex_long_double = cld;
      return (str);
    }

  ++str;
  while (isspace (*str))
    ++str;
  
  if (*str == 'I')
    {
      __imag__ cld = 1;
      mr_value->vt_complex_long_double = cld;
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
  mr_value->vt_complex_long_double = cld;
  
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
    case MR_VT_INTMAX:
      str = mr_load_bitmask (str, mr_value);
      break;
    case MR_VT_LONG_DOUBLE:
      str = mr_load_long_double (str, mr_value);
      break;
    case MR_VT_COMPLEX_LONG_DOUBLE:
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

mr_status_t
mr_value_cast (mr_value_type_t value_type, mr_value_t * mr_value)
{
  mr_status_t status = MR_SUCCESS;
  switch (mr_value->value_type)
    {
    case MR_VT_CHAR:
      switch (value_type)
	{
	case MR_VT_INTMAX: mr_value->vt_intmax = mr_value->vt_char; break;
	case MR_VT_LONG_DOUBLE: mr_value->vt_long_double = mr_value->vt_char; break;
	case MR_VT_COMPLEX_LONG_DOUBLE: mr_value->vt_complex_long_double = mr_value->vt_char; break;
	default: status = MR_FAILURE; break;
	}
      break;
      
    case MR_VT_INTMAX:
      switch (value_type)
	{
	case MR_VT_INTMAX: break;
	case MR_VT_LONG_DOUBLE: mr_value->vt_long_double = mr_value->vt_intmax; break;
	case MR_VT_COMPLEX_LONG_DOUBLE: mr_value->vt_complex_long_double = mr_value->vt_intmax; break;
	default: status = MR_FAILURE; break;
	}
      break;
      
    case MR_VT_LONG_DOUBLE:
      switch (value_type)
	{
	case MR_VT_INTMAX: mr_value->vt_intmax = mr_value->vt_long_double; break;
	case MR_VT_LONG_DOUBLE: break;
	case MR_VT_COMPLEX_LONG_DOUBLE: mr_value->vt_complex_long_double = mr_value->vt_long_double; break;
	default: status = MR_FAILURE; break;
	}
      break;
      
    case MR_VT_COMPLEX_LONG_DOUBLE:
      switch (value_type)
	{
	case MR_VT_INTMAX: mr_value->vt_intmax = mr_value->vt_complex_long_double; break;
	case MR_VT_LONG_DOUBLE: mr_value->vt_long_double = mr_value->vt_complex_long_double; break;
	case MR_VT_COMPLEX_LONG_DOUBLE: break;
	default: status = MR_FAILURE; break;
	}
      break;
      
    case MR_VT_SUBSTR:
      {
	mr_value_t result = { .value_type = value_type };
	if (NULL == mr_value->vt_substr.str)
	  return (MR_FAILURE);

	int length = MR_MIN (mr_value->vt_substr.length, (1 << 6));
	char buf[length + sizeof (char)];
	memcpy (buf, mr_value->vt_substr.str, length);
	buf[length] = 0;

	char * tail = mr_load_var (&result, buf);
	if ((NULL == tail) || *tail)
	  {
	    MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_DATA_AT_THE_END, buf);
	    return (MR_FAILURE);
	  }
	*mr_value = result;
	break;
      }

    case MR_VT_STRING:
    case MR_VT_UNALLOCATED_STRING:
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
    case MR_VT_INTMAX:
      value->vt_intmax = -value->vt_intmax;
      break;
      
    case MR_VT_LONG_DOUBLE:
      value->vt_long_double = -value->vt_long_double;
      break;
      
    case MR_VT_COMPLEX_LONG_DOUBLE:
      value->vt_complex_long_double = -value->vt_complex_long_double;
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
    if (result_type < MR_VT_INTMAX)					\
      result_type = MR_VT_INTMAX;					\
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
      case MR_VT_INTMAX: result->vt_intmax = left->vt_intmax OP right->vt_intmax; break; \
      case MR_VT_LONG_DOUBLE: result->vt_long_double = left->vt_long_double OP right->vt_long_double; break; \
      case MR_VT_COMPLEX_LONG_DOUBLE: result->vt_complex_long_double = left->vt_complex_long_double OP right->vt_complex_long_double; break; \
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
    if ((left->value_type > MR_VT_INTMAX) || (right->value_type > MR_VT_INTMAX)) \
      {									\
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_WRONG_RESULT_TYPE);		\
	return (MR_FAILURE);						\
      }									\
    if ((MR_SUCCESS != mr_value_cast (MR_VT_INTMAX, left)) ||		\
	(MR_SUCCESS != mr_value_cast (MR_VT_INTMAX, right)))		\
      {									\
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_WRONG_RESULT_TYPE);		\
	return (MR_FAILURE);						\
      }									\
    result->value_type = MR_VT_INTMAX;					\
    __VA_ARGS__;							\
    result->vt_intmax = left->vt_intmax OP right->vt_intmax;		\
    return (MR_SUCCESS);						\
  }

bool
mr_value_is_zero (mr_value_t * value)
{
  switch (value->value_type)
    {
    case MR_VT_INTMAX: return (0 == value->vt_intmax);
    case MR_VT_LONG_DOUBLE: return (0 == value->vt_long_double);
    case MR_VT_COMPLEX_LONG_DOUBLE: return (0 == value->vt_complex_long_double);
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
