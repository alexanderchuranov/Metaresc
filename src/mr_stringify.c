/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <math.h>
#include <stdbool.h>
#ifdef HAVE_CONFIG_H
#include <mr_config.h>
#endif /* HAVE_CONFIG_H */
#ifdef HAVE_ENDIAN_H
#include <endian.h>
#endif /* HAVE_ENDIAN_H */
#ifdef HAVE_MACHINE_ENDIAN_H
#include <machine/endian.h>
#endif /* HAVE_MACHINE_ENDIAN_H */
#ifdef HAVE_DLFCN_H
#define __USE_GNU
#include <dlfcn.h>
#endif /* HAVE_DLFCN_H */

#include <metaresc.h>
#include <mr_stringify.h>
#include <mr_ic.h>

char *
mr_output_format_bool (mr_ptrdes_t * ptrdes)
{
  return (*(bool*)ptrdes->data ? MR_STRDUP ("TRUE") : MR_STRDUP ("FALSE"));
}

#define MR_OUTPUT_FORMAT_TYPE(TYPE, FORMAT)				\
  char * mr_output_format_ ## TYPE (mr_ptrdes_t * ptrdes) {		\
    char str[MR_FLOAT_TO_STRING_BUF_SIZE] = "";				\
    sprintf (str, FORMAT, *(TYPE*)ptrdes->data);			\
    return (MR_STRDUP (str));						\
  }

MR_OUTPUT_FORMAT_TYPE (int8_t, "%" SCNi8);
MR_OUTPUT_FORMAT_TYPE (uint8_t, "%" SCNu8);
MR_OUTPUT_FORMAT_TYPE (int16_t, "%" SCNi16);
MR_OUTPUT_FORMAT_TYPE (uint16_t, "%" SCNu16);
MR_OUTPUT_FORMAT_TYPE (int32_t, "%" SCNi32);
MR_OUTPUT_FORMAT_TYPE (uint32_t, "%" SCNu32);
MR_OUTPUT_FORMAT_TYPE (int64_t, "%" SCNi64);
MR_OUTPUT_FORMAT_TYPE (uint64_t, "%" SCNu64);
MR_OUTPUT_FORMAT_TYPE (float, "%.8g");
MR_OUTPUT_FORMAT_TYPE (double, "%.17g");
MR_OUTPUT_FORMAT_TYPE (long_double_t, "%.20Lg");

#define MR_OUTPUT_FORMAT_COMPLEX(NAME_SUFFIX, TYPE_NAME, MR_TYPE)	\
  char * mr_output_format_complex_ ## NAME_SUFFIX (mr_ptrdes_t * ptrdes) { \
    char str[2 * MR_FLOAT_TO_STRING_BUF_SIZE] = "";			\
    mr_ptrdes_t real_ptrdes = *ptrdes;					\
    mr_ptrdes_t imag_ptrdes = *ptrdes;					\
    TYPE_NAME real = __real__ *(complex TYPE_NAME *)ptrdes->data;	\
    TYPE_NAME imag = __imag__ *(complex TYPE_NAME *)ptrdes->data;	\
    real_ptrdes.data = &real;						\
    real_ptrdes.fd.mr_type = MR_TYPE;					\
    imag_ptrdes.data = &imag;						\
    imag_ptrdes.fd.mr_type = MR_TYPE;					\
    char * real_str = mr_stringify_ ## NAME_SUFFIX (&real_ptrdes);	\
    char * imag_str = mr_stringify_ ## NAME_SUFFIX (&imag_ptrdes);	\
    if (real_str && imag_str)						\
      sprintf (str, "%s + %si", real_str, imag_str);			\
    if (real_str)							\
      MR_FREE (real_str);						\
    if (imag_str)							\
      MR_FREE (imag_str);						\
    return (MR_STRDUP (str));						\
  }

MR_OUTPUT_FORMAT_COMPLEX (float, float, MR_TYPE_FLOAT);
MR_OUTPUT_FORMAT_COMPLEX (double, double, MR_TYPE_DOUBLE);
MR_OUTPUT_FORMAT_COMPLEX (long_double_t, long double, MR_TYPE_LONG_DOUBLE);

/**
 * Init IO handlers Table
 */
void __attribute__((constructor)) mr_init_output_format (void)
{
  mr_conf.output_format[MR_TYPE_BOOL] = mr_output_format_bool;
  mr_conf.output_format[MR_TYPE_INT8] = mr_output_format_int8_t;
  mr_conf.output_format[MR_TYPE_UINT8] = mr_output_format_uint8_t;
  mr_conf.output_format[MR_TYPE_INT16] = mr_output_format_int16_t;
  mr_conf.output_format[MR_TYPE_UINT16] = mr_output_format_uint16_t;
  mr_conf.output_format[MR_TYPE_INT32] = mr_output_format_int32_t;
  mr_conf.output_format[MR_TYPE_UINT32] = mr_output_format_uint32_t;
  mr_conf.output_format[MR_TYPE_INT64] = mr_output_format_int64_t;
  mr_conf.output_format[MR_TYPE_UINT64] = mr_output_format_uint64_t;
  mr_conf.output_format[MR_TYPE_FLOAT] = mr_output_format_float;
  mr_conf.output_format[MR_TYPE_COMPLEX_FLOAT] = mr_output_format_complex_float;
  mr_conf.output_format[MR_TYPE_DOUBLE] = mr_output_format_double;
  mr_conf.output_format[MR_TYPE_COMPLEX_DOUBLE] = mr_output_format_complex_double;
  mr_conf.output_format[MR_TYPE_LONG_DOUBLE] = mr_output_format_long_double_t;
  mr_conf.output_format[MR_TYPE_COMPLEX_LONG_DOUBLE] = mr_output_format_complex_long_double_t;
}

/**
 * MR_UINTxx type saving handler. Make a string from *(uintXX_t*)data.
 * \@param mr_save_data save routines data and lookup structures
 * \@return stringified int value
 */
#define MR_STRINGIFY_TYPE(TYPE, MR_TYPE)				\
  char * mr_stringify_ ## TYPE (mr_ptrdes_t * ptrdes) {			\
    if (mr_conf.output_format[MR_TYPE])					\
      return (mr_conf.output_format[MR_TYPE](ptrdes));			\
    return (mr_output_format_ ## TYPE (ptrdes));			\
  }

MR_STRINGIFY_TYPE (bool, MR_TYPE_BOOL);
MR_STRINGIFY_TYPE (int8_t, MR_TYPE_INT8);
MR_STRINGIFY_TYPE (uint8_t, MR_TYPE_UINT8);
MR_STRINGIFY_TYPE (int16_t, MR_TYPE_INT16);
MR_STRINGIFY_TYPE (uint16_t, MR_TYPE_UINT16);
MR_STRINGIFY_TYPE (int32_t, MR_TYPE_INT32);
MR_STRINGIFY_TYPE (uint32_t, MR_TYPE_UINT32);
MR_STRINGIFY_TYPE (int64_t, MR_TYPE_INT64);
MR_STRINGIFY_TYPE (uint64_t, MR_TYPE_UINT64);

/**
 * MR_FLOAT type saving handler. Stringify float value.
 * \@param mr_save_data save routines data and lookup structures
 * \@return stringified float value
 */
MR_STRINGIFY_TYPE (float, MR_TYPE_FLOAT);
MR_STRINGIFY_TYPE (complex_float, MR_TYPE_COMPLEX_FLOAT);
MR_STRINGIFY_TYPE (double, MR_TYPE_DOUBLE);
MR_STRINGIFY_TYPE (complex_double, MR_TYPE_COMPLEX_DOUBLE);
MR_STRINGIFY_TYPE (long_double_t, MR_TYPE_LONG_DOUBLE);
MR_STRINGIFY_TYPE (complex_long_double_t, MR_TYPE_COMPLEX_LONG_DOUBLE);

/**
 * Stringify integer value.
 * @param ptrdes descriptor of the saved field
 * @return stringified enum value
 */
static char *
mr_stringify_uint (mr_ptrdes_t * ptrdes)
{
  switch (ptrdes->fd.size)
    {
    case sizeof (uint8_t): return (mr_stringify_uint8_t (ptrdes));
    case sizeof (uint16_t): return (mr_stringify_uint16_t (ptrdes));
    case sizeof (uint32_t): return (mr_stringify_uint32_t (ptrdes));
    case sizeof (uint64_t): return (mr_stringify_uint64_t (ptrdes));
    default:
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_INT_OF_UNKNOWN_SIZE, ptrdes->fd.size);
      return (MR_STRDUP ("0"));
    }
}

/**
 * Save as string pointer descriptor
 * @param ptrdes descriptor of the saved field
 * @return stringified pointer value
 */
char *
mr_stringify_func (mr_ptrdes_t * ptrdes)
{
  if (MR_TRUE == ptrdes->flags.is_null)
    return (MR_STRDUP (""));
  else
    {
      void * func = *(void**)ptrdes->data;
      char str[MR_INT_TO_STRING_BUF_SIZE];
#ifdef HAVE_LIBDL
      Dl_info info;
      memset (&info, 0, sizeof (info));
      if (0 != dladdr (func, &info))
	{
	  if (info.dli_sname && (func == info.dli_saddr)) /* found some non-null name and address matches */
	    return (MR_STRDUP (info.dli_sname));
	}
#endif /* HAVE_LIBDL */
      sprintf (str, "%p", func);
      return (MR_STRDUP (str));
    }
}

/**
 * Gets enum value as integer
 * @param ptrdes descriptor of the saved field
 * @return enum value
 */
static uint64_t
mr_get_enum_value (mr_ptrdes_t * ptrdes)
{
  uint64_t value = 0;
  switch (ptrdes->fd.size)
    {
    case sizeof (uint8_t): value = * (uint8_t*) ptrdes->data; break;
    case sizeof (uint16_t): value = * (uint16_t*) ptrdes->data; break;
    case sizeof (uint32_t): value = * (uint32_t*) ptrdes->data; break;
    case sizeof (uint64_t): value = * (uint64_t*) ptrdes->data; break;
    default:
#if __BYTE_ORDER == __LITTLE_ENDIAN
      memcpy (&value, ptrdes->data, MR_MIN (ptrdes->fd.size, sizeof (value))); /* NB: only for little endian */
#else
#error Support for non little endian architectures to be implemented
#endif /*__BYTE_ORDER == __LITTLE_ENDIAN */
      break;
    }
  return (value);
}

/**
 * MR_ENUM type saving handler. Look up enum descriptor and save as
 * stringified enum value or as integer otherwise.
 * @param ptrdes descriptor of the saved field
 * @return stringified enum value
 */
char *
mr_stringify_enum (mr_ptrdes_t * ptrdes)
{
  mr_td_t * tdp = mr_get_td_by_name (ptrdes->fd.type); /* look up for type descriptor */
  /* check whether type descriptor was found */
  if (NULL == tdp)
    MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_NO_TYPE_DESCRIPTOR, ptrdes->fd.type);
  else if (MR_TYPE_ENUM != tdp->mr_type)
    MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_TYPE_NOT_ENUM, ptrdes->fd.type);
  else
    {
      ptrdes->fd.size = tdp->size_effective;
      {
	uint64_t value = mr_get_enum_value (ptrdes);
	mr_fd_t * fdp = mr_get_enum_by_value (tdp, value);
	if (fdp && fdp->name.str)
	  return (MR_STRDUP (fdp->name.str));
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_SAVE_ENUM, value, tdp->type.str, ptrdes->fd.name.str);
      }
    }
  /* save as integer otherwise */
  return (mr_stringify_uint (ptrdes));
}

/**
 * MR_BITFIELD type saving handler. Stringify as integer.
 * @param ptrdes descriptor of the saved field
 * @return stringified enum value
 */
char *
mr_stringify_bitfield (mr_ptrdes_t * ptrdes)
{
  mr_ptrdes_t ptrdes_;
  uint64_t value;

  mr_save_bitfield_value (ptrdes, &value);
  ptrdes_ = *ptrdes;
  ptrdes_.data = &value;

  switch (ptrdes->fd.mr_type_aux)
    {
    case MR_TYPE_BOOL: return (mr_stringify_bool (&ptrdes_));
    case MR_TYPE_INT8: return (mr_stringify_int8_t (&ptrdes_));
    case MR_TYPE_UINT8: return (mr_stringify_uint8_t (&ptrdes_));
    case MR_TYPE_INT16: return (mr_stringify_int16_t (&ptrdes_));
    case MR_TYPE_UINT16: return (mr_stringify_uint16_t (&ptrdes_));
    case MR_TYPE_INT32: return (mr_stringify_int32_t (&ptrdes_));
    case MR_TYPE_UINT32: return (mr_stringify_uint32_t (&ptrdes_));
    case MR_TYPE_INT64: return (mr_stringify_int64_t (&ptrdes_));
    case MR_TYPE_UINT64: return (mr_stringify_uint64_t (&ptrdes_));
    case MR_TYPE_ENUM: return (mr_stringify_enum (&ptrdes_));
    default: return (mr_stringify_uint64_t (&ptrdes_));
    }
}

static char *
mr_decompose_bitmask_add (char * str, char * bitmask_or_delimiter, char * token)
{
  char * str_ = MR_REALLOC (str, strlen (str) + strlen (bitmask_or_delimiter) + strlen (token) + 1);
  if (NULL == str_)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      MR_FREE (str);
    }
  else
    {
      strcat (str_, bitmask_or_delimiter);
      strcat (str_, token);
    }
  return (str_);
}

/**
 * MR_BITMASK type saving handler. Look up type descriptor and save as
 * stringified bitmask value or as integer otherwise.
 * @param ptrdes descriptor of the saved field
 * @param bitmask_or_delimiter delimiter for OR statement
 * @return stringified enum value
 */
char *
mr_stringify_bitmask (mr_ptrdes_t * ptrdes, char * bitmask_or_delimiter)
{
  int i, count;
  uint64_t value = 0;
  char * str = NULL;
  mr_td_t * tdp = mr_get_td_by_name (ptrdes->fd.type); /* look up for type descriptor */

  /* check whether type descriptor was found */
  if (NULL == tdp)
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_NO_TYPE_DESCRIPTOR, ptrdes->fd.type);
      return (mr_stringify_uint (ptrdes));
    }

  if (MR_TYPE_ENUM != tdp->mr_type)
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_TYPE_NOT_ENUM, ptrdes->fd.type);
      return (mr_stringify_uint (ptrdes));
    }

  value = mr_get_enum_value (ptrdes);

  if (0 == value)
    {
      mr_fd_t * fdp = mr_get_enum_by_value (tdp, value);
      return (MR_STRDUP (fdp && fdp->name.str ? fdp->name.str : "0"));
    }

  /* decompose value on bitmask */
  count = tdp->fields.size / sizeof (tdp->fields.data[0]);
  for (i = 0; i < count; ++i)
    {
      mr_fd_t * fdp = tdp->fields.data[i].fdp;
      if ((value & fdp->param.enum_value) && !(~value & fdp->param.enum_value))
	{
	  if (NULL == str)
	    str = MR_STRDUP (fdp->name.str);
	  else
	    str = mr_decompose_bitmask_add (str, bitmask_or_delimiter, fdp->name.str);
	  if (NULL == str)
	    break;
	  value &= ~fdp->param.enum_value;
	}
      if (0 == value)
	break;
    }

  if (value != 0)
    {
      /* save non-matched part as integer */
      char number[MR_INT_TO_STRING_BUF_SIZE];
      sprintf (number, "%" SCNu64, value);
      str = mr_decompose_bitmask_add (str, bitmask_or_delimiter, number);
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_SAVE_BITMASK, value);
    }
  return (str);
}

#define XML_NONPRINT_ESC "&#x%X;"
#define ESC_SIZE (sizeof (XML_NONPRINT_ESC))
#define ESC_CHAR_MAP_SIZE (256)
static char * map[ESC_CHAR_MAP_SIZE] = {
  [0 ... ESC_CHAR_MAP_SIZE - 1] = NULL,
  [(unsigned char)'\t'] = "\t",
  [(unsigned char)'\n'] = "\n",
  [(unsigned char)'&'] = "&amp;",
  [(unsigned char)'<'] = "&lt;",
  [(unsigned char)'>'] = "&gt;",
};

/**
 * XML quote function. Escapes XML special characters.
 * @param str input string
 * @return XML quoted string
 */
char *
xml_quote_string (char * str)
{
  int length = 0;
  char * str_;

  if (NULL == str)
    return (NULL);

  for (str_ = str; *str_; ++str_)
    if (map[(unsigned char)*str_])
      length += strlen (map[(unsigned char)*str_]);
    else if (isprint (*str_))
      ++length;
    else
      length += sizeof (XML_NONPRINT_ESC) - 1;

  str_ = MR_MALLOC (length + 1);
  if (NULL == str)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (NULL);
    }

  length = 0;
  for (; *str; ++str)
    if (map[(unsigned char)*str])
      {
	strcpy (&str_[length], map[(unsigned char)*str]);
	length += strlen (&str_[length]);
      }
    else if (isprint (*str))
      str_[length++] = *str;
    else
      {
	sprintf (&str_[length], XML_NONPRINT_ESC, (int)(unsigned char)*str);
	length += strlen (&str_[length]);
      }
  str_[length] = 0;
  return (str_);
}

/**
 * XML unquote function. Replace XML special characters aliases on a source characters.
 * @param str input string
 * @param length length of the input string
 * @return XML unquoted string
 */
char *
xml_unquote_string (mr_substr_t * substr)
{
  char * str_ = MR_MALLOC (substr->length + 1);
  int length_ = 0;
  int i, j;

  static int inited = 0;
  static char map_c[ESC_CHAR_MAP_SIZE];
  static char * map_cp[ESC_CHAR_MAP_SIZE];
  static int map_s[ESC_CHAR_MAP_SIZE];
  static int map_size = 0;

  if (0 == inited)
    {
      for (i = 0; i < ESC_CHAR_MAP_SIZE; ++i)
	if (map[i])
	  {
	    int size = strlen (map[i]);
	    if (size > 1)
	      {
		map_c[map_size] = i;
		map_cp[map_size] = map[i];
		map_s[map_size] = size;
		++map_size;
	      }
	  }
      inited = !0;
    }

  if (NULL == str_)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (NULL);
    }

  for (j = 0; j < substr->length; ++j)
    if (substr->str[j] != '&')
      str_[length_++] = substr->str[j];
    else
      {
	char esc[ESC_SIZE];
	strncpy (esc, &substr->str[j], sizeof (esc) - 1);
	if ('#' == substr->str[j + 1])
	  {
	    int32_t code = 0;
	    int size = 0;
	    if (1 != sscanf (&substr->str[j], XML_NONPRINT_ESC "%n", &code, &size))
	      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_WRONG_XML_ESC, esc);
	    else
	      {
		j += size - 1; /* one more +1 in the loop */
		str_[length_++] = code;
	      }
	  }
	else
	  {
	    for (i = 0; i < map_size; ++i)
	      if (0 == strncasecmp (&substr->str[j], map_cp[i], map_s[i]))
		{
		  str_[length_++] = map_c[i];
		  j += map_s[i] - 1; /* one more increase in the loop */
		  break;
		}
	    if (i >= map_size)
	      {
		MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNKNOWN_XML_ESC, esc);
		str_[length_++] = substr->str[j];
	      }
	  }
      }
  str_[length_] = 0;
  return (str_);
}
