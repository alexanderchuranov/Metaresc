/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef HAVE_CONFIG_H
#include <mr_config.h>
#endif /* HAVE_CONFIG_H */
#include <metaresc.h>
#include <mr_value.h>
#include <mr_ic.h>

/**
 * Stores a parsed value into a node descriptor. A substring is stored as an
 * offset into the loaded string, an integer in the narrowest fitting
 * representation.
 * @param ptrdes node descriptor
 * @param mr_value parsed value
 * @param str loaded string that substrings point into
 * @return status
 */
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
      {
        typeof (ptrdes->vt_double) casted = mr_value->vt_long_double;
        (void)mr_hash_block (&casted, sizeof (casted));
        if (mr_value->vt_long_double == casted)
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
      }

    case MR_VT_COMPLEX_LONG_DOUBLE:
      {
        typeof (ptrdes->vt_complex_float) casted = mr_value->vt_complex_long_double;
        (void)mr_hash_block (&casted, sizeof (casted));
        if (mr_value->vt_complex_long_double == casted)
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
      }

    default:
      return (MR_FAILURE);
    }
  return (MR_SUCCESS);
}

/**
 * Restores a parsed value from a node descriptor. This is the inverse of
 * mr_value_to_mr_ptrdes.
 * @param mr_value receives the value
 * @param ptrdes node descriptor
 * @param str loaded string that substrings point into, may be NULL
 * @return status
 */
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

/**
 * Visitor for the enum values of a type that finds the length of the
 * longest name.
 * @param key enum value descriptor
 * @param context pointer on the maximum length, updated in place
 * @return MR_SUCCESS
 */
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
 * @param str string with enum
 * @param data pointer on place to save value
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
  if (isalpha ((unsigned char)*str) || ('_' == *str))
    str = mr_get_enum (str, data);
  else
    {
      char * start = str;
      int saved_errno = errno;
      errno = 0;
      *data = mr_strtouintmax (str, &str, 0);
      bool overflow = (ERANGE == errno);
      errno = saved_errno;
      if (overflow)
        {
          MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_INT_LITERAL_RANGE, start);
          return (NULL);
        }
    }
  return (str);
}

/**
 * Converts the text of an integer literal to long double digit by digit. It is
 * used for literals that do not fit an integer.
 * @param s text of the literal
 * @param base base of the literal, 0 to detect it from the prefix
 * @return value of the literal
 */
static long double
mr_int_literal_to_long_double (char * s, int base)
{
  while (isspace ((unsigned char)*s))
    ++s;
  char * sign = s;
  bool neg = ('-' == *s);
  if (('-' == *s) || ('+' == *s))
    ++s;
  /* without a leading zero base 0 is decimal */
  if ((10 == base) || ((0 == base) && ('0' != *s)))
    return (strtold (sign, NULL)); /* rounds correctly */

  if (((0 == base) || (16 == base)) && ('0' == s[0]) && (('x' == s[1]) || ('X' == s[1])))
    {
      s += 2;
      base = 16;
    }
  else if (0 == base)
    base = 8;

  long double value = 0;
  for ( ; ; ++s)
    {
      unsigned int c = (unsigned char)*s;
      if (isdigit (c))
        c -= '0';
      else if (isalpha (c))
        c -= isupper (c) ? 'A' - 10 : 'a' - 10;
      else
        break;
      if (c >= base)
        break;
      value = value * base + c;
    }
  return (neg ? -value : value);
}

/**
 * Sets a value from the text of an integer literal in a document. A literal that
 * does not fit the integer type is not wrapped around: it becomes a long double,
 * so a floating point field gets its value, and an integer field rejects it.
 * @param value receives the value
 * @param text text of the literal, with an optional sign
 * @param base base of the literal, 0 to detect it from the prefix
 */
void
mr_value_set_int_literal (mr_value_t * value, char * text, int base)
{
  int saved_errno = errno;
  errno = 0;
  mr_uintmax_t uvalue = mr_strtouintmax (text, NULL, base);
  bool overflow = (ERANGE == errno);
  errno = saved_errno;

  if (!overflow)
    {
      char * s = text;
      while (isspace ((unsigned char)*s))
        ++s;
      /* a negative literal must fit the signed type */
      overflow = ('-' == *s) && ((mr_intmax_t)uvalue > 0);
    }

  if (overflow)
    {
      value->value_type = MR_VT_LONG_DOUBLE;
      value->vt_long_double = mr_int_literal_to_long_double (text, base);
    }
  else
    {
      value->value_type = MR_VT_INTMAX;
      value->vt_intmax = uvalue;
    }
}

/**
 * Parses an integer or a bitmask of integers or enum names joined with '|'.
 * @param str string to parse
 * @param mr_value receives the integer value
 * @return pointer after the parsed text or NULL on error
 */
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

/**
 * Parses a floating point number.
 * @param str string to parse
 * @param mr_value receives the value
 * @return pointer after the parsed text or NULL on error
 */
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

/**
 * Parses a complex number: a real part, an imaginary part with an 'I' suffix,
 * or both joined with '+' or '-'.
 * @param str string to parse
 * @param mr_value receives the value
 * @return pointer after the parsed text or NULL on error
 */
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

/**
 * Parses a value of the type that is set in mr_value and skips the
 * whitespaces after it.
 * @param mr_value value with the expected value type, receives the value
 * @param str string to parse
 * @return pointer after the parsed text or NULL on error
 */
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

/**
 * Converts a floating point value to an integer. A conversion of a value out
 * of the range of the integer type is undefined, so it is rejected.
 * @param value floating point value
 * @param result receives the integer value
 * @return status, failure for NaN and values out of range
 */
static mr_status_t
mr_long_double_to_intmax (long double value, mr_intmax_t * result)
{
  /* the minimal value is a power of two, so the bounds are exact in long double */
  const long double min = (mr_intmax_t)(((mr_uintmax_t)1) << (sizeof (mr_intmax_t) * __CHAR_BIT__ - 1));
  if (!((value >= min) && (value < -min))) /* also false for NaN */
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_INT_RANGE, value);
      return (MR_FAILURE);
    }
  *result = value;
  return (MR_SUCCESS);
}

/**
 * Converts a value to another value type in place. Strings and substrings are
 * parsed and must not have text after the value.
 * @param value_type target value type
 * @param mr_value value to convert
 * @return status
 */
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
        case MR_VT_INTMAX: status = mr_long_double_to_intmax (mr_value->vt_long_double, &mr_value->vt_intmax); break;
        case MR_VT_LONG_DOUBLE: break;
        case MR_VT_COMPLEX_LONG_DOUBLE: mr_value->vt_complex_long_double = mr_value->vt_long_double; break;
        default: status = MR_FAILURE; break;
        }
      break;
      
    case MR_VT_COMPLEX_LONG_DOUBLE:
      switch (value_type)
        {
        case MR_VT_INTMAX: status = mr_long_double_to_intmax (__real__ mr_value->vt_complex_long_double, &mr_value->vt_intmax); break;
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

        int length = MR_MIN (mr_value->vt_substr.length, sizeof ("-1.1e1111 + -1.1e1111 I") + 2 * MR_LDBL_DIG);
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

/**
 * Negates a numeric value in place.
 * @param value value to negate
 * @return status, failure for non-numeric values
 */
mr_status_t
mr_value_neg (mr_value_t * value)
{
  switch (value->value_type)
    {
    case MR_VT_INTMAX:
      /* negation of the minimal value overflows the signed type, so negate it as unsigned */
      value->vt_intmax = -(mr_uintmax_t)value->vt_intmax;
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

/*
  Integer operations on values of documents. Signed overflow is undefined, so the operations
  wrap around in the unsigned type, and the division of the minimal value by -1 is defined
  as in two's complement: the quotient is the minimal value, and the remainder is zero.
*/
static inline mr_intmax_t mr_intmax_add (mr_intmax_t x, mr_intmax_t y) { return ((mr_uintmax_t)x + (mr_uintmax_t)y); }
static inline mr_intmax_t mr_intmax_sub (mr_intmax_t x, mr_intmax_t y) { return ((mr_uintmax_t)x - (mr_uintmax_t)y); }
static inline mr_intmax_t mr_intmax_mul (mr_intmax_t x, mr_intmax_t y) { return ((mr_uintmax_t)x * (mr_uintmax_t)y); }
static inline mr_intmax_t mr_intmax_div (mr_intmax_t x, mr_intmax_t y) { return ((-1 == y) ? -(mr_uintmax_t)x : x / y); }
static inline mr_intmax_t mr_intmax_mod (mr_intmax_t x, mr_intmax_t y) { return ((-1 == y) ? 0 : x % y); }
static inline mr_intmax_t mr_intmax_bit_or (mr_intmax_t x, mr_intmax_t y) { return (x | y); }
static inline mr_intmax_t mr_intmax_bit_and (mr_intmax_t x, mr_intmax_t y) { return (x & y); }
static inline mr_intmax_t mr_intmax_bit_xor (mr_intmax_t x, mr_intmax_t y) { return (x ^ y); }

/*
  Defines mr_status_t mr_value_<OP_NAME> (mr_value_t * result, mr_value_t * left, mr_value_t * right)
  that computes left OP right. Operands are cast to the wider of their types, at least an integer.
  Integers are computed with mr_intmax_<OP_NAME>.
  __VA_ARGS__ is a statement run before the operation (e.g. a check of a divisor).
  It returns MR_SUCCESS or MR_FAILURE for non-numeric operands.
*/
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
    __VA_ARGS__;                                                        \
    switch (result_type)                                                \
      {									\
      case MR_VT_INTMAX: result->vt_intmax = mr_intmax_ ## OP_NAME (left->vt_intmax, right->vt_intmax); break; \
      case MR_VT_LONG_DOUBLE: result->vt_long_double = left->vt_long_double OP right->vt_long_double; break; \
      case MR_VT_COMPLEX_LONG_DOUBLE: result->vt_complex_long_double = left->vt_complex_long_double OP right->vt_complex_long_double; break; \
      default:								\
        MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_WRONG_RESULT_TYPE);		\
        return (MR_FAILURE);						\
      }									\
    return (MR_SUCCESS);                                                \
  }

/*
  Defines mr_status_t mr_value_<OP_NAME> (mr_value_t * result, mr_value_t * left, mr_value_t * right)
  that computes mr_intmax_<OP_NAME> (left, right) for integer operands.
  __VA_ARGS__ is a statement run before the operation (e.g. a check of a divisor).
  It returns MR_SUCCESS or MR_FAILURE for non-integer operands.
*/
#define MR_INT_VALUE_OP(OP_NAME, ...)					\
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
    __VA_ARGS__;                                                        \
    result->vt_intmax = mr_intmax_ ## OP_NAME (left->vt_intmax, right->vt_intmax); \
    return (MR_SUCCESS);                                                \
  }

/**
 * Checks whether a numeric value is zero.
 * @param value value to check
 * @return true if the value is zero, false otherwise and for non-numeric values
 */
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
MR_INT_VALUE_OP (mod, if (mr_value_is_zero (right)) { MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_DIVISION_BY_ZERO); return (MR_FAILURE); });
MR_INT_VALUE_OP (bit_or);
MR_INT_VALUE_OP (bit_and);
MR_INT_VALUE_OP (bit_xor);
