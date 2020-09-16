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

#define MR_LOAD_VAR(VAR, STR) mr_load_var (MR_TYPE_DETECT (__typeof__ (*VAR)), STR, VAR)

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
  int size;

  while (isalnum (*str) || (*str == '_'))
    ++str;
  size = str - name;

  {
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
	*data = fdp->param.enum_value._unsigned;
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
mr_load_bitmask (char * str, uint64_t * data)
{
  *data = 0;
  for (;;)
    {
      uint64_t value;
      str = mr_get_int (str, &value);
      if (NULL == str)
	return (NULL);
      *data |= value;
      while (isspace (*str))
	++str;
      if (*str != '|')
	break;
      ++str;
    }
  return (str);
}

static char *
mr_load_long_double (char * str, long double * data)
{
  int offset;
  if (1 != sscanf (str, "%Lg%n", data, &offset))
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_LONG_DOUBLE, str);
      return (NULL);
    }
  return (&str[offset]);
}

static char *
mr_load_complex_long_double (char * str, complex long double * x)
{
  int offset;
  long double real, imag;
  if (2 != sscanf (str, "%Lg + %Lgi%n", &real, &imag, &offset))
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_COMPLEX_LONG_DOUBLE, str);
      return (NULL);
    }
  __real__ * x = real;
  __imag__ * x = imag;
  return (&str[offset]);
}

static mr_status_t
mr_load_var (mr_type_t mr_type, char * str, void * var)
{
  if ((NULL == str) || (NULL == var))
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
      return (MR_FAILURE);
    }
  
  switch (mr_type)
    {
    case MR_TYPE_INT64:
    case MR_TYPE_UINT64:
      str = mr_load_bitmask (str, var);
      break;
    case MR_TYPE_LONG_DOUBLE:
      str = mr_load_long_double (str, var);
      break;
    case MR_TYPE_COMPLEX_LONG_DOUBLE:
      str = mr_load_complex_long_double (str, var);
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

#define MR_VALUE_CAST(VT_VALUE)						\
  switch (value->value_type)						\
    {									\
    case MR_VT_INT: value->VT_VALUE = value->vt_int; break;		\
    case MR_VT_FLOAT: value->VT_VALUE = value->vt_float; break;		\
    case MR_VT_COMPLEX: value->VT_VALUE = value->vt_complex; break;	\
    case MR_VT_QUOTED_SUBSTR:						\
      if (NULL == value->vt_quoted_substr.substr.str)			\
	{								\
	  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_NULL_POINTER);	\
	  status = MR_FAILURE;						\
	}								\
      else								\
	{								\
	  __typeof__ (value->VT_VALUE) vt_value = 0;			\
	  mr_quoted_substr_t * quoted_substr = &value->vt_quoted_substr; \
	  if ((NULL == quoted_substr->unquote) &&			\
	      (0 == quoted_substr->substr.str[quoted_substr->substr.length])) \
	    status = MR_LOAD_VAR (&vt_value, quoted_substr->substr.str); \
	  else								\
	    {								\
	      char dst[quoted_substr->substr.length + 1];		\
	      if (NULL == quoted_substr->unquote)			\
		{							\
		  memcpy (dst, quoted_substr->substr.str, quoted_substr->substr.length); \
		  dst[quoted_substr->substr.length] = 0;		\
		}							\
	      else							\
		quoted_substr->unquote (&quoted_substr->substr, dst);	\
	      status = MR_LOAD_VAR (&vt_value, dst);			\
	    }								\
	  if (MR_SUCCESS == status)					\
	    {								\
	      value->VT_VALUE = vt_value;				\
	      value->value_type = value_type;				\
	    }								\
	}								\
      break;								\
    default:								\
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_WRONG_RESULT_TYPE);		\
      status = MR_FAILURE;						\
      break;								\
    }

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
      mr_fd_t * fdp;
      mr_quoted_substr_t * quoted_substr = &value->vt_quoted_substr;
      if ((NULL == quoted_substr->unquote) &&
	  (0 == quoted_substr->substr.str[quoted_substr->substr.length]))
	fdp = mr_get_enum_by_name (quoted_substr->substr.str);
      else
	{
	  char dst[quoted_substr->substr.length + 1];
  
	  if (NULL == quoted_substr->unquote)
	    {
	      memcpy (dst, quoted_substr->substr.str, quoted_substr->substr.length);
	      dst[quoted_substr->substr.length] = 0;
	    }
	  else
	    quoted_substr->unquote (&quoted_substr->substr, dst);
  
	  fdp = mr_get_enum_by_name (dst);
	}
      
      __typeof__ (value->vt_int) vt_int = 0;
      if (NULL == fdp)
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNKNOWN_ENUM, value->vt_string);
      else
	vt_int = fdp->param.enum_value._signed;
      value->vt_int = vt_int;
    }
}

mr_status_t
mr_value_cast (mr_value_type_t value_type, mr_value_t * value)
{
  mr_status_t status = MR_SUCCESS;
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
      status = MR_FAILURE;
      break;
    }
  return (status);
}

mr_status_t
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
      return (MR_FAILURE);
    }
  return (MR_SUCCESS);
}

#define MR_VALUE_OP(OP_NAME, OP, ...)					\
  mr_status_t mr_value_ ## OP_NAME (mr_value_t * result, mr_value_t * left, mr_value_t * right) \
  {									\
    mr_value_type_t result_type = MR_MAX (left->value_type, right->value_type);	\
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
      case MR_VT_COMPLEX: result->vt_complex = left->vt_complex OP right->vt_complex; break; \
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
    mr_value_id (left);							\
    mr_value_id (right);						\
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
    case MR_VT_COMPLEX: return (0 == value->vt_complex);
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
