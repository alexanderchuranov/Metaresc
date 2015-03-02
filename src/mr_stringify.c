/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <math.h>
#include <stdbool.h>

#ifndef __USE_XOPEN2K8
#define __USE_XOPEN2K8
#endif /* __USE_XOPEN2K8 */

#include <string.h>

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

MR_OUTPUT_FORMAT_COMPLEX (float, float, MR_TYPE_FLOAT, CINIT_COMPLEX_FLOAT_QUOTE);
MR_OUTPUT_FORMAT_COMPLEX (double, double, MR_TYPE_DOUBLE, CINIT_COMPLEX_FLOAT_QUOTE);
MR_OUTPUT_FORMAT_COMPLEX (long_double_t, long double, MR_TYPE_LONG_DOUBLE, CINIT_COMPLEX_FLOAT_QUOTE);

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
  if (TRUE == ptrdes->flags.is_null)
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
      int64_t value = mr_get_enum_value (tdp, ptrdes->data);
      mr_fd_t * fdp = mr_get_enum_by_value (tdp, value);
      if (fdp && fdp->name.str)
	return (MR_STRDUP (fdp->name.str));
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_SAVE_ENUM, value, tdp->type.str, ptrdes->fd.name.str);
      ptrdes->fd.size = tdp->size_effective;
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

  value = mr_get_enum_value (tdp, ptrdes->data);

  if (0 == value)
    {
      mr_fd_t * fdp = mr_get_enum_by_value (tdp, value);
      return (MR_STRDUP (fdp && fdp->name.str ? fdp->name.str : "0"));
    }

  /* decompose value on bitmask */
  count = tdp->fields_size / sizeof (tdp->fields[0]);
  for (i = 0; i < count; ++i)
    {
      mr_fd_t * fdp = tdp->fields[i].fdp;
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
      mr_ptrdes_t ptrdes = { .data = &value, };
      char * number = mr_stringify_uint64_t (&ptrdes);
      if (number != NULL)
	{
	  str = mr_decompose_bitmask_add (str, bitmask_or_delimiter, number);
	  MR_FREE (number);
	}
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_SAVE_BITMASK, value);
    }
  return (str);
}
