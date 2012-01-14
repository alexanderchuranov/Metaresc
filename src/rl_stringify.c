/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

#include <reslib.h>
#include <rlconfig.h>
#include <math.h>
#ifdef HAVE_ENDIAN_H
#include <endian.h>
#endif /* HAVE_ENDIAN_H */
#ifdef HAVE_MACHINE_ENDIAN_H
#include <machine/endian.h>
#endif /* HAVE_MACHINE_ENDIAN_H */

#define RL_OUTPUT_FORMAT_TYPE(TYPE, FORMAT)				\
  static char * rl_output_format_ ## TYPE (rl_ptrdes_t * ptrdes) {	\
    char str[RL_MAX(RL_INT_TO_STRING_BUF_SIZE, RL_FLOAT_TO_STRING_BUF_SIZE)]; \
    sprintf (str, FORMAT, *(TYPE*)ptrdes->data);			\
    return (RL_STRDUP (str));						\
  }

RL_OUTPUT_FORMAT_TYPE (int8_t, "%" SCNi8);
RL_OUTPUT_FORMAT_TYPE (uint8_t, "%" SCNu8);
RL_OUTPUT_FORMAT_TYPE (int16_t, "%" SCNi16);
RL_OUTPUT_FORMAT_TYPE (uint16_t, "%" SCNu16);
RL_OUTPUT_FORMAT_TYPE (int32_t, "%" SCNi32);
RL_OUTPUT_FORMAT_TYPE (uint32_t, "%" SCNu32);
RL_OUTPUT_FORMAT_TYPE (int64_t, "%" SCNi64);
RL_OUTPUT_FORMAT_TYPE (uint64_t, "%" SCNu64);
RL_OUTPUT_FORMAT_TYPE (float, "%.8g");
RL_OUTPUT_FORMAT_TYPE (double, "%.17g");
RL_OUTPUT_FORMAT_TYPE (long_double_t, "%.20Lg");

/**
 * Init IO handlers Table
 */
void __attribute__((constructor)) rl_init_output_format (void)
{
  rl_conf.output_format[RL_TYPE_INT8] = rl_output_format_int8_t;
  rl_conf.output_format[RL_TYPE_UINT8] = rl_output_format_uint8_t;
  rl_conf.output_format[RL_TYPE_INT16] = rl_output_format_int16_t;
  rl_conf.output_format[RL_TYPE_UINT16] = rl_output_format_uint16_t;
  rl_conf.output_format[RL_TYPE_INT32] = rl_output_format_int32_t;
  rl_conf.output_format[RL_TYPE_UINT32] = rl_output_format_uint32_t;
  rl_conf.output_format[RL_TYPE_INT64] = rl_output_format_int64_t;
  rl_conf.output_format[RL_TYPE_UINT64] = rl_output_format_uint64_t;
  rl_conf.output_format[RL_TYPE_FLOAT] = rl_output_format_float;
  rl_conf.output_format[RL_TYPE_DOUBLE] = rl_output_format_double;
  rl_conf.output_format[RL_TYPE_LONG_DOUBLE] = rl_output_format_long_double_t;
}

/**
 * RL_UINTxx type saving handler. Make a string from *(uintXX_t*)data.
 * @param ptrdes pointer descriptor
 * @return stringified int value
 */
#define RL_STRINGIFY_TYPE(TYPE, RL_TYPE)				\
  char * rl_stringify_ ## TYPE (rl_ptrdes_t * ptrdes) {			\
    if (rl_conf.output_format[RL_TYPE])					\
      return (rl_conf.output_format[RL_TYPE](ptrdes));			\
    return (RL_STRDUP ("0"));						\
  }

RL_STRINGIFY_TYPE (int8, RL_TYPE_INT8);
RL_STRINGIFY_TYPE (uint8, RL_TYPE_UINT8);
RL_STRINGIFY_TYPE (int16, RL_TYPE_INT16);
RL_STRINGIFY_TYPE (uint16, RL_TYPE_UINT16);
RL_STRINGIFY_TYPE (int32, RL_TYPE_INT32);
RL_STRINGIFY_TYPE (uint32, RL_TYPE_UINT32);
RL_STRINGIFY_TYPE (int64, RL_TYPE_INT64);
RL_STRINGIFY_TYPE (uint64, RL_TYPE_UINT64);

/**
 * RL_FLOAT type saving handler. Stringify float value.
 * @param ptrs resizeable array with pointers descriptors 
 * @return stringified float value
 */
RL_STRINGIFY_TYPE (float, RL_TYPE_FLOAT);
RL_STRINGIFY_TYPE (double, RL_TYPE_DOUBLE);
RL_STRINGIFY_TYPE (long_double_t, RL_TYPE_LONG_DOUBLE);

/**
 * Stringify integer value.
 * @param ptrs resizeable array with pointers descriptors 
 * @return stringified enum value
 */
static char *
rl_stringify_uint (rl_ptrdes_t * ptrdes)
{
  switch (ptrdes->fd.size)
    {
    case sizeof (uint8_t): return (rl_stringify_uint8 (ptrdes));
    case sizeof (uint16_t): return (rl_stringify_uint16 (ptrdes));
    case sizeof (uint32_t): return (rl_stringify_uint32 (ptrdes));
    case sizeof (uint64_t): return (rl_stringify_uint64 (ptrdes));
    default:
      RL_MESSAGE (RL_LL_WARN, RL_MESSAGE_INT_OF_UNKNOWN_SIZE, ptrdes->fd.size);
      return (RL_STRDUP ("0"));
    }
}

static uint64_t
rl_get_int_value (rl_ptrdes_t * ptrdes)
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
      memcpy (&value, ptrdes->data, RL_MIN (ptrdes->fd.size, sizeof (value))); /* NB: only for little endian */
#else
#error Support for non little endian architectures to be implemented
#endif /*__BYTE_ORDER == __LITTLE_ENDIAN */
      break;
    }
  value &= (2LL << (8 * ptrdes->fd.size - 1)) - 1; /* Somehow (1 << 32) == 1, that's why we need this trick */
  return (value);
}

/**
 * RL_ENUM type saving handler. Look up enum descriptor and save as
 * stringified enum value or as integer otherwise.
 * @param ptrs resizeable array with pointers descriptors 
 * @return stringified enum value
 */
char *
rl_stringify_enum (rl_ptrdes_t * ptrdes)
{
  rl_td_t * tdp = rl_get_td_by_name (ptrdes->fd.type); /* look up for type descriptor */

  /* check whether type descriptor was found */
  if (NULL == tdp)
    RL_MESSAGE (RL_LL_WARN, RL_MESSAGE_NO_TYPE_DESCRIPTOR, ptrdes->fd.type);
  else if (RL_TYPE_ENUM != tdp->rl_type)
    RL_MESSAGE (RL_LL_WARN, RL_MESSAGE_TYPE_NOT_ENUM, ptrdes->fd.type);
  else
    {
      uint64_t value = rl_get_int_value (ptrdes);
      rl_fd_t * fdp = rl_get_enum_by_value (tdp, value);
      if (fdp && fdp->name)
	return (RL_STRDUP (fdp->name));
      RL_MESSAGE (RL_LL_WARN, RL_MESSAGE_SAVE_ENUM, value, tdp->type);
    }
  /* save as integer otherwise */
  return (rl_stringify_uint (ptrdes));
}

/**
 * RL_BITFIELD type saving handler. Stringify as integer.
 * @param ptrs resizeable array with pointers descriptors 
 * @return stringified enum value
 */
char *
rl_stringify_bitfield (rl_ptrdes_t * ptrdes)
{
  rl_ptrdes_t ptrdes_;
  uint64_t value;

  rl_save_bitfield_value (ptrdes, &value);
  ptrdes_ = *ptrdes;
  ptrdes_.data = &value;

  switch (ptrdes->fd.rl_type_aux)
    {
    case RL_TYPE_INT8: return (rl_stringify_int8 (&ptrdes_));
    case RL_TYPE_UINT8: return (rl_stringify_uint8 (&ptrdes_));
    case RL_TYPE_INT16: return (rl_stringify_int16 (&ptrdes_));
    case RL_TYPE_UINT16: return (rl_stringify_uint16 (&ptrdes_));
    case RL_TYPE_INT32: return (rl_stringify_int32 (&ptrdes_));
    case RL_TYPE_UINT32: return (rl_stringify_uint32 (&ptrdes_));
    case RL_TYPE_INT64: return (rl_stringify_int64 (&ptrdes_));
    case RL_TYPE_UINT64: return (rl_stringify_uint64 (&ptrdes_));
    case RL_TYPE_ENUM: return (rl_stringify_enum (&ptrdes_));
    default: return (rl_stringify_uint64 (&ptrdes_));
    }
}

/**
 * RL_BITMASK type saving handler. Look up type descriptor and save as
 * stringified bitmask value or as integer otherwise.
 * @param ptrs resizeable array with pointers descriptors 
 * @return stringified enum value
 */
char *
rl_stringify_bitmask (rl_ptrdes_t * ptrdes, char * bitmask_or_delimiter)
{
  int i;
  uint64_t value = 0;
  char * str = NULL;
  void * str_;
  rl_td_t * tdp;
  int count;

  /* look up for type descriptor */
  tdp = rl_get_td_by_name (ptrdes->fd.type);
  /* check whether type descriptor was found */
  if (NULL == tdp)
    {
      RL_MESSAGE (RL_LL_WARN, RL_MESSAGE_NO_TYPE_DESCRIPTOR, ptrdes->fd.type);
      return (rl_stringify_uint (ptrdes));
    }
  if (RL_TYPE_ENUM != tdp->rl_type)
    {
      RL_MESSAGE (RL_LL_WARN, RL_MESSAGE_TYPE_NOT_ENUM, ptrdes->fd.type);
      return (rl_stringify_uint (ptrdes));
    }

  value = rl_get_int_value (ptrdes);
  
  count = tdp->fields.size / sizeof (tdp->fields.data[0]);
  if (0 == value)
    {
      rl_fd_t * fdp = rl_get_enum_by_value (tdp, value);
      str = RL_STRDUP (fdp && fdp->name ? fdp->name : "0");
    }
  else
    {
      /* decompose value on bitmask */
      for (i = 0; i < count; ++i)
	if ((value & tdp->fields.data[i].param.enum_value)
	    && !(~value & tdp->fields.data[i].param.enum_value))
	  {
	    if (NULL == str)
	      str = RL_STRDUP (tdp->fields.data[i].name);
	    else
	      {
		char * name = tdp->fields.data[i].name;
		str_ = RL_REALLOC (str, strlen (str) + strlen (bitmask_or_delimiter) + strlen (name) + 1);
		if (NULL == str_)
		  {
		    RL_MESSAGE (RL_LL_FATAL, RL_MESSAGE_OUT_OF_MEMORY);
		    RL_FREE (str);
		    return (NULL);
		  }
		str = (char*) str_;
		strcat (str, bitmask_or_delimiter);
		strcat (str, name);
	      }
	    value &= ~tdp->fields.data[i].param.enum_value;
	  }

      if (value != 0)
	{
	  /* save non-matched part as integer */
	  char number[RL_INT_TO_STRING_BUF_SIZE];
	  sprintf (number, "%" SCNu64, value);
	  if (NULL == str)
	    return (RL_STRDUP (number));
	  str_ = RL_REALLOC (str, strlen (str) + strlen (bitmask_or_delimiter) + strlen (number) + 1);
	  if (NULL == str_)
	    {
	      RL_MESSAGE (RL_LL_FATAL, RL_MESSAGE_OUT_OF_MEMORY);
	      RL_FREE (str);
	      return (NULL);
	    }
	  str = (char*) str_;
	  strcat (str, bitmask_or_delimiter);
	  strcat (str, number);
	  RL_MESSAGE (RL_LL_WARN, RL_MESSAGE_SAVE_BITMASK, value);
	}
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

  str_ = RL_MALLOC (length + 1);
  if (NULL == str)
    {
      RL_MESSAGE (RL_LL_FATAL, RL_MESSAGE_OUT_OF_MEMORY);
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

char *
xml_unquote_string (char * str, int length)
{
  char * str_ = RL_MALLOC (length + 1);
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
      RL_MESSAGE (RL_LL_FATAL, RL_MESSAGE_OUT_OF_MEMORY);
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
	      RL_MESSAGE (RL_LL_WARN, RL_MESSAGE_WRONG_XML_ESC, esc);
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
		RL_MESSAGE (RL_LL_WARN, RL_MESSAGE_UNKNOWN_XML_ESC, esc);
		str_[length_++] = str[j];
	      }
	  }
      }
  str_[length_] = 0;
  return (str_);
}  
