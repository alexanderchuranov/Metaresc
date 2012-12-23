/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <math.h>
#ifdef HAVE_CONFIG_H
# include <mr_config.h>
#endif /* HAVE_CONFIG_H */
#ifdef HAVE_ENDIAN_H
#include <endian.h>
#endif /* HAVE_ENDIAN_H */
#ifdef HAVE_MACHINE_ENDIAN_H
#include <machine/endian.h>
#endif /* HAVE_MACHINE_ENDIAN_H */

#include <metaresc.h>
#include <mr_ic.h>

#define MR_OUTPUT_FORMAT_TYPE(TYPE, FORMAT)				\
  static char * mr_output_format_ ## TYPE (mr_ptrdes_t * ptrdes) {	\
    char str[MR_MAX(MR_INT_TO_STRING_BUF_SIZE, MR_FLOAT_TO_STRING_BUF_SIZE)]; \
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

/**
 * Init IO handlers Table
 */
void __attribute__((constructor)) mr_init_output_format (void)
{
  mr_conf.output_format[MR_TYPE_INT8] = mr_output_format_int8_t;
  mr_conf.output_format[MR_TYPE_UINT8] = mr_output_format_uint8_t;
  mr_conf.output_format[MR_TYPE_INT16] = mr_output_format_int16_t;
  mr_conf.output_format[MR_TYPE_UINT16] = mr_output_format_uint16_t;
  mr_conf.output_format[MR_TYPE_INT32] = mr_output_format_int32_t;
  mr_conf.output_format[MR_TYPE_UINT32] = mr_output_format_uint32_t;
  mr_conf.output_format[MR_TYPE_INT64] = mr_output_format_int64_t;
  mr_conf.output_format[MR_TYPE_UINT64] = mr_output_format_uint64_t;
  mr_conf.output_format[MR_TYPE_FLOAT] = mr_output_format_float;
  mr_conf.output_format[MR_TYPE_DOUBLE] = mr_output_format_double;
  mr_conf.output_format[MR_TYPE_LONG_DOUBLE] = mr_output_format_long_double_t;
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
    return (MR_STRDUP ("0"));						\
  }

MR_STRINGIFY_TYPE (int8, MR_TYPE_INT8);
MR_STRINGIFY_TYPE (uint8, MR_TYPE_UINT8);
MR_STRINGIFY_TYPE (int16, MR_TYPE_INT16);
MR_STRINGIFY_TYPE (uint16, MR_TYPE_UINT16);
MR_STRINGIFY_TYPE (int32, MR_TYPE_INT32);
MR_STRINGIFY_TYPE (uint32, MR_TYPE_UINT32);
MR_STRINGIFY_TYPE (int64, MR_TYPE_INT64);
MR_STRINGIFY_TYPE (uint64, MR_TYPE_UINT64);

/**
 * MR_FLOAT type saving handler. Stringify float value.
 * \@param mr_save_data save routines data and lookup structures
 * \@return stringified float value
 */
MR_STRINGIFY_TYPE (float, MR_TYPE_FLOAT);
MR_STRINGIFY_TYPE (double, MR_TYPE_DOUBLE);
MR_STRINGIFY_TYPE (long_double_t, MR_TYPE_LONG_DOUBLE);

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
    case sizeof (uint8_t): return (mr_stringify_uint8 (ptrdes));
    case sizeof (uint16_t): return (mr_stringify_uint16 (ptrdes));
    case sizeof (uint32_t): return (mr_stringify_uint32 (ptrdes));
    case sizeof (uint64_t): return (mr_stringify_uint64 (ptrdes));
    default:
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_INT_OF_UNKNOWN_SIZE, ptrdes->fd.size);
      return (MR_STRDUP ("0"));
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
	if (fdp && fdp->hashed_name.name)
	  return (MR_STRDUP (fdp->hashed_name.name));
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_SAVE_ENUM, value, tdp->hashed_name.name);
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
    case MR_TYPE_INT8: return (mr_stringify_int8 (&ptrdes_));
    case MR_TYPE_UINT8: return (mr_stringify_uint8 (&ptrdes_));
    case MR_TYPE_INT16: return (mr_stringify_int16 (&ptrdes_));
    case MR_TYPE_UINT16: return (mr_stringify_uint16 (&ptrdes_));
    case MR_TYPE_INT32: return (mr_stringify_int32 (&ptrdes_));
    case MR_TYPE_UINT32: return (mr_stringify_uint32 (&ptrdes_));
    case MR_TYPE_INT64: return (mr_stringify_int64 (&ptrdes_));
    case MR_TYPE_UINT64: return (mr_stringify_uint64 (&ptrdes_));
    case MR_TYPE_ENUM: return (mr_stringify_enum (&ptrdes_));
    default: return (mr_stringify_uint64 (&ptrdes_));
    }
}

typedef struct {
  uint64_t value;
  mr_td_t * tdp;
  char * str;
  char * bitmask_or_delimiter;
} mr_decompose_bitmask_t;

static void
mr_decompose_bitmask_add (mr_decompose_bitmask_t * mr_decompose_bitmask, char * token)
{
  char * str = MR_REALLOC (mr_decompose_bitmask->str,
			    strlen (mr_decompose_bitmask->str) +
			    strlen (mr_decompose_bitmask->bitmask_or_delimiter) +
			    strlen (token) + 1);
  if (NULL == str)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      MR_FREE (mr_decompose_bitmask->str);
      mr_decompose_bitmask->str = NULL;
    }
  else
    {
      mr_decompose_bitmask->str = str;
      strcat (mr_decompose_bitmask->str, mr_decompose_bitmask->bitmask_or_delimiter);
      strcat (mr_decompose_bitmask->str, token);
    }
}  

static int
mr_decompose_bitmask_value (mr_ptr_t key, const void * context)
{
  mr_fd_t * fdp = key.ptr;
  mr_decompose_bitmask_t * mr_decompose_bitmask = (void*)context;
  if ((mr_decompose_bitmask->value & fdp->param.enum_value)
      && !(~mr_decompose_bitmask->value & fdp->param.enum_value))
    {
      if (NULL == mr_decompose_bitmask->str)
	mr_decompose_bitmask->str = MR_STRDUP (fdp->hashed_name.name);
      else
	mr_decompose_bitmask_add (mr_decompose_bitmask, fdp->hashed_name.name);
      if (NULL == mr_decompose_bitmask->str)
	return (!0);
      mr_decompose_bitmask->value &= ~fdp->param.enum_value;
    }
  return (0 == mr_decompose_bitmask->value);
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
  mr_decompose_bitmask_t mr_decompose_bitmask = {
    .value = 0,
    .str = NULL,
    .bitmask_or_delimiter = bitmask_or_delimiter,
    .tdp = mr_get_td_by_name (ptrdes->fd.type), /* look up for type descriptor */
  };
  
  
  /* check whether type descriptor was found */
  if (NULL == mr_decompose_bitmask.tdp)
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_NO_TYPE_DESCRIPTOR, ptrdes->fd.type);
      return (mr_stringify_uint (ptrdes));
    }
  if (MR_TYPE_ENUM != mr_decompose_bitmask.tdp->mr_type)
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_TYPE_NOT_ENUM, ptrdes->fd.type);
      return (mr_stringify_uint (ptrdes));
    }

  mr_decompose_bitmask.value = mr_get_enum_value (ptrdes);
  
  if (0 == mr_decompose_bitmask.value)
    {
      mr_fd_t * fdp = mr_get_enum_by_value (mr_decompose_bitmask.tdp, mr_decompose_bitmask.value);
      return (MR_STRDUP (fdp && fdp->hashed_name.name ? fdp->hashed_name.name : "0"));
    }
  /* decompose value on bitmask */
  mr_ic_foreach (&mr_decompose_bitmask.tdp->fields, mr_decompose_bitmask_value, &mr_decompose_bitmask);

  if (mr_decompose_bitmask.value != 0)
    {
      /* save non-matched part as integer */
      char number[MR_INT_TO_STRING_BUF_SIZE];
      sprintf (number, "%" SCNu64, mr_decompose_bitmask.value);
      mr_decompose_bitmask_add (&mr_decompose_bitmask, number);
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_SAVE_BITMASK, mr_decompose_bitmask.value);
    }
  return (mr_decompose_bitmask.str);
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
xml_unquote_string (char * str, int length)
{
  char * str_ = MR_MALLOC (length + 1);
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
  
  for (j = 0; j < length; ++j)
    if (str[j] != '&')
      str_[length_++] = str[j];
    else
      {
	char esc[ESC_SIZE];
	strncpy (esc, &str[j], sizeof (esc) - 1);
	if ('#' == str[j + 1])
	  {
	    int32_t code = 0;
	    int size = 0;
	    if (1 != sscanf (&str[j], XML_NONPRINT_ESC "%n", &code, &size))
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
	      if (0 == strncasecmp (&str[j], map_cp[i], map_s[i]))
		{
		  str_[length_++] = map_c[i];
		  j += map_s[i] - 1; /* one more increase in the loop */
		  break;
		}
	    if (i >= map_size)
	      {
		MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNKNOWN_XML_ESC, esc);
		str_[length_++] = str[j];
	      }
	  }
      }
  str_[length_] = 0;
  return (str_);
}  
