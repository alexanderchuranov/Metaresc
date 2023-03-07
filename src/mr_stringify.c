/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

#include <math.h>
#include <stdbool.h>

#ifndef __USE_XOPEN2K8
#define __USE_XOPEN2K8
#endif /* __USE_XOPEN2K8 */

#include <string.h>

#ifdef HAVE_CONFIG_H
#include <mr_config.h>
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_DLFCN_H
#define __USE_GNU
#include <dlfcn.h>
#endif /* HAVE_DLFCN_H */

#include <metaresc.h>
#include <flt_values.h>
#include <mr_stringify.h>
#include <mr_ic.h>

int mr_ra_printf_void (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (0);
}

int mr_ra_printf_bool_default (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  return (*(bool*)ptrdes->data.ptr ?
	  mr_ra_append_string (mr_ra_str, "true") :
	  mr_ra_append_string (mr_ra_str, "false"));
}

#define MR_RA_PRINTF_TMPLT(TYPE, TMPLT)					\
  int mr_ra_printf_ ## TYPE ## _default (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)	\
  { return (mr_ra_printf (mr_ra_str, TMPLT, *(TYPE *)ptrdes->data.ptr)); }

MR_RA_PRINTF_TMPLT (float, "%." MR_STRINGIFY_READONLY (MR_FLT_DIG) "g");
MR_RA_PRINTF_TMPLT (double, "%." MR_STRINGIFY_READONLY (MR_DBL_DIG) "g");
MR_RA_PRINTF_TMPLT (long_double_t, "%." MR_STRINGIFY_READONLY (MR_LDBL_DIG) "Lg");

#ifndef __SCNi8
#define __SCNi8 SCNi8
#endif /* __SCNi8 */

#ifndef __SCNu8
#define __SCNu8 SCNu8
#endif /* __SCNu8 */
  
MR_RA_PRINTF_TMPLT (int8_t, "%" __SCNi8);
MR_RA_PRINTF_TMPLT (uint8_t, "%" __SCNu8);
MR_RA_PRINTF_TMPLT (int16_t, "%" SCNi16);
MR_RA_PRINTF_TMPLT (uint16_t, "%" SCNu16);
MR_RA_PRINTF_TMPLT (int32_t, "%" SCNi32);
MR_RA_PRINTF_TMPLT (uint32_t, "%" SCNu32);
MR_RA_PRINTF_TMPLT (int64_t, "%" SCNi64);
MR_RA_PRINTF_TMPLT (uint64_t, "%" SCNu64);

char *
mr_uintmaxtostr (char * out, mr_uintmax_t value)
{
  *--out = 0;
  do {
    *--out = '0' + value % 10;
    value /= 10;
  } while (value != 0);
  return (out);
}

int mr_ra_printf_int128_t_default (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  mr_intmax_t value = *(mr_intmax_t*)ptrdes->data.ptr;
  char buffer[(sizeof (value) * 12 + 4) / 5 + sizeof ("-")]; /* log10 (256) = 12/5 */
  bool negative = false;

  if (value < 0)
    {
      negative = true;
      value = ~value + 1; /* clang has a bug so that -(1 << 127) == 0 */
    }
  char * out = mr_uintmaxtostr (&buffer[sizeof (buffer)], value);
  if (negative)
    *--out = '-';
  return (mr_ra_append_string (mr_ra_str, out));
}

int mr_ra_printf_uint128_t_default (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  mr_uintmax_t value = *(mr_uintmax_t*)ptrdes->data.ptr;
  char buffer[(sizeof (value) * 12 + 4) / 5 + sizeof ("-")]; /* log10 (256) = 12/5 */
  char * out = mr_uintmaxtostr (&buffer[sizeof (buffer)], value);
  return (mr_ra_append_string (mr_ra_str, out));
}

#define MR_RA_PRINTF_COMPLEX(TYPE, SUFFIX)				\
  int mr_ra_printf_complex_ ## SUFFIX (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes, char * delimiter)	\
  {									\
    int count = 0;							\
    mr_ptrdes_t _ptrdes = *ptrdes;					\
    TYPE real = __real__ *(complex TYPE *)ptrdes->data.ptr;		\
    TYPE imag = __imag__ *(complex TYPE *)ptrdes->data.ptr;		\
    if (MR_ISNAN (real) || MR_ISNAN (imag))				\
      return (TRY_CATCH_THROW (mr_ra_append_string (mr_ra_str, "NAN"))); \
    if (real != 0)							\
      {									\
	_ptrdes.data.ptr = &real;					\
	count += TRY_CATCH_THROW (mr_ra_printf_ ## SUFFIX (mr_ra_str, &_ptrdes)); \
	if (imag != 0)							\
	  count += TRY_CATCH_THROW (mr_ra_append_string (mr_ra_str, delimiter)); \
      }									\
    if (imag != 0)							\
      {									\
	_ptrdes.data.ptr = &imag;					\
	if (imag != 1)							\
	  count += TRY_CATCH_THROW (mr_ra_printf_ ## SUFFIX (mr_ra_str, &_ptrdes)); \
	count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, 'I'));	\
      }									\
    if ((real == 0) && (imag == 0))					\
      count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '0'));	\
    return (count);							\
  }

MR_RA_PRINTF_COMPLEX (float, float)
MR_RA_PRINTF_COMPLEX (double, double)
MR_RA_PRINTF_COMPLEX (long double, long_double_t)

#define MR_RA_PRINTF_TYPE(TYPE)	MR_RA_PRINTF_TYPE_ (TYPE, MR_TYPE_DETECT (TYPE))
#define MR_RA_PRINTF_TYPE_(TYPE, MR_TYPE)				\
  int mr_ra_printf_ ## TYPE (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes) { \
    if (mr_conf.output_format[MR_TYPE])					\
      return (mr_conf.output_format[MR_TYPE](mr_ra_str, ptrdes));	\
    return (mr_ra_printf_ ## TYPE ## _default (mr_ra_str, ptrdes));	\
  }

MR_FOREACH (MR_RA_PRINTF_TYPE, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, float, double, long_double_t)
MR_RA_PRINTF_TYPE_ (bool, MR_TYPE_BOOL);
MR_RA_PRINTF_TYPE_ (int128_t, MR_TYPE_INT128);
MR_RA_PRINTF_TYPE_ (uint128_t, MR_TYPE_UINT128);

int mr_ra_printf_enum (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes)
{
  mr_td_t * tdp = ptrdes->tdp;
  mr_type_t mr_type = MR_TYPE_UINT8;

  /* check whether type descriptor was found */
  if (tdp)
    {
      mr_enum_value_type_t value = mr_get_enum_value (tdp, ptrdes->data.ptr);
      mr_fd_t * fdp = mr_get_enum_by_value (tdp, value);
      if (fdp)
	if (fdp->name.str)
	  return (mr_ra_append_string (mr_ra_str, fdp->name.str));
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_SAVE_ENUM, value, tdp->type.str, ptrdes->name);
      mr_type = tdp->param.enum_param.mr_type_effective;
    }
  /* save as integer otherwise */
  switch (mr_type)
    {
#define MR_RA_PRINTF_INT(TYPE) MR_RA_PRINTF_INT_ (TYPE, MR_TYPE_DETECT (TYPE))
#define MR_RA_PRINTF_INT_(TYPE, MR_TYPE) case MR_TYPE: return (mr_ra_printf_ ## TYPE (mr_ra_str, ptrdes));
      MR_FOREACH (MR_RA_PRINTF_INT, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t);
      MR_RA_PRINTF_INT_ (int128_t, MR_TYPE_INT128);
      MR_RA_PRINTF_INT_ (uint128_t, MR_TYPE_UINT128);
    default: return (-1);
    }
}

int mr_ra_printf_bitmask (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes, char * delimiter)
{
  mr_td_t * tdp = ptrdes->tdp;
  /* check whether type descriptor was found */
  if (NULL == tdp)
    return (mr_ra_printf_enum (mr_ra_str, ptrdes));
  if ((MR_TYPE_ENUM != tdp->mr_type) || (!tdp->param.enum_param.is_bitmask))
    return (mr_ra_printf_enum (mr_ra_str, ptrdes));

  bool first = true;
  int count = 0;
  int i, fields_count = tdp->fields_size / sizeof (tdp->fields[0]);
  mr_enum_value_type_t value = mr_get_enum_value (tdp, ptrdes->data.ptr);
      
  if (0 == value)
    return (mr_ra_printf_enum (mr_ra_str, ptrdes));

  for (i = 0; i < fields_count; ++i)
    if (value & tdp->fields[i].fdp->param.enum_param._unsigned)
      {
	if (first)
	  first = false;
	else
	  count += TRY_CATCH_THROW (mr_ra_append_string (mr_ra_str, delimiter));
	count += TRY_CATCH_THROW (mr_ra_append_string (mr_ra_str, tdp->fields[i].fdp->name.str));
	value ^= tdp->fields[i].fdp->param.enum_param._unsigned;
	if (0 == value)
	  break;
      }

  if (0 != value)
    {
      mr_ptrdes_t _ptrdes = { .data = { &value } };
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_SAVE_ENUM, value, tdp->type.str, ptrdes->name);
      if (!first)
	count += TRY_CATCH_THROW (mr_ra_append_string (mr_ra_str, delimiter));
      
#define CASE_PRINT_BY_TYPE(TYPE) CASE_PRINT_BY_TYPE_ (TYPE, MR_TYPE_DETECT (TYPE))
#define CASE_PRINT_BY_TYPE_(TYPE, MR_TYPE)				\
      case MR_TYPE:							\
	count += TRY_CATCH_THROW (mr_ra_printf_ ## TYPE (mr_ra_str, &_ptrdes)); \
	break;
      
      switch (tdp->param.enum_param.mr_type_effective)
	{
	  MR_FOREACH (CASE_PRINT_BY_TYPE, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t);
	  CASE_PRINT_BY_TYPE_ (int128_t, MR_TYPE_INT128);
	  CASE_PRINT_BY_TYPE_ (uint128_t, MR_TYPE_UINT128);
	default: break;
	}
    }
  return (count);
}

int mr_ra_printf_bitfield (mr_rarray_t * mr_ra_str, mr_ptrdes_t * ptrdes, char * delimiter)
{
  mr_ptrdes_t _ptrdes = *ptrdes;
  int count = 0;
  uint64_t value;

  mr_save_bitfield_value (ptrdes, &value);
  _ptrdes.data.ptr = &value;

  switch (ptrdes->mr_type_aux)
    {
      MR_FOREACH (CASE_PRINT_BY_TYPE, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t);
      CASE_PRINT_BY_TYPE_ (bool, MR_TYPE_BOOL);
      CASE_PRINT_BY_TYPE_ (int128_t, MR_TYPE_INT128);
      CASE_PRINT_BY_TYPE_ (uint128_t, MR_TYPE_UINT128);

    case MR_TYPE_ENUM:
      count += mr_ra_printf_bitmask (mr_ra_str, &_ptrdes, delimiter);
      break;
    default: break;
    }
  return (count);
}  

TYPEDEF_STRUCT (mr_func_name_t,
		ANON_UNION (),
		(uintptr_t, func_),
		(void *, func),
		END_ANON_UNION (),
		(const char *, name))

TYPEDEF_STRUCT (mr_ra_fn_t,
		(mr_func_name_t *, ra, , , { .offset = offsetof (mr_ra_fn_t, size) }, "offset"),
		(ssize_t, size),
		(ssize_t, alloc_size))

mr_hash_value_t
mr_fn_get_hash (const mr_ptr_t x, const void * context)
{
  const mr_ra_fn_t * ra_fn = context;
  return (ra_fn->ra[x.intptr].func_);
}

int
mr_fn_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const mr_ra_fn_t * ra_fn = context;
  return ((ra_fn->ra[x.intptr].func > ra_fn->ra[y.intptr].func) - (ra_fn->ra[x.intptr].func < ra_fn->ra[y.intptr].func));
}

static mr_ic_t cache = { .ic_type = MR_IC_UNINITIALIZED, };
static mr_ra_fn_t ra_fn = { .ra = NULL, .size = 0, .alloc_size = 0, };

static void __attribute__ ((destructor))
mr_fn_cache_cleanup (void)
{
  if (ra_fn.ra)
    MR_FREE (ra_fn.ra);
  ra_fn.ra = NULL;
  ra_fn.size = ra_fn.alloc_size = 0;
  mr_ic_free (&cache);
}

const char * mr_serialize_func (void * func)
{
#ifdef HAVE_LIBDL
  mr_func_name_t * new_fn = NULL;

  if (mr_conf.cache_func_resolve)
    {
      intptr_t idx = ra_fn.size / sizeof (ra_fn.ra[0]);

      new_fn = mr_rarray_allocate_element ((void*)&ra_fn.ra, &ra_fn.size, &ra_fn.alloc_size, sizeof (ra_fn.ra[0]));
      if (new_fn)
	{
	  new_fn->func = func;
	  new_fn->name = NULL;
      
	  if (MR_IC_UNINITIALIZED == cache.ic_type)
	    {
	      mr_res_t context = {
		.data = { &ra_fn, },
		.type = "mr_ra_fn_t",
		.MR_SIZE = sizeof (ra_fn),
	      };
	      mr_ic_new (&cache, mr_fn_get_hash, mr_fn_cmp, "intptr", MR_IC_HASH, &context);
	    }
	  mr_ptr_t * add = mr_ic_add (&cache, idx);
	  if (add != NULL)
	    if (add->intptr != idx)
	      {
		ra_fn.size -= sizeof (ra_fn.ra[0]);
		return (ra_fn.ra[add->intptr].name);
	      }
	}
    }
  
  Dl_info info;
  memset (&info, 0, sizeof (info));
  if (0 != dladdr (func, &info))
    {
      if (info.dli_sname && (func == info.dli_saddr)) /* found some non-null name and address matches */
	{
	  void * func_ = dlsym (RTLD_DEFAULT, info.dli_sname); /* try backward resolve. MAC OS X could resolve static functions, but can't make backward resolution */
	  if (func_ == func)
	    {
	      if (new_fn)
		new_fn->name = info.dli_sname;
	      return (info.dli_sname);
	    }
	}
    }
#endif /* HAVE_LIBDL */
  return (NULL);
}

int mr_ra_append_char (mr_rarray_t * mr_ra_str, char c)
{
  if ((0 == mr_ra_str->MR_SIZE) || (NULL == mr_ra_str->data.ptr))
    goto free_mr_ra;

  char * tail = mr_rarray_append (mr_ra_str, sizeof (c));

  if (NULL == tail)
    goto free_mr_ra;

  tail[-1] = c;
  tail[0] = 0;
  return (sizeof (c));

 free_mr_ra:
  if (mr_ra_str->data.ptr)
    MR_FREE (mr_ra_str->data.ptr);
  mr_ra_str->data.ptr = NULL;
  mr_ra_str->MR_SIZE = mr_ra_str->alloc_size = 0;
  return (-1);
}

int mr_ra_append_string (mr_rarray_t * mr_ra_str, char * str)
{
  if ((0 == mr_ra_str->MR_SIZE) || (NULL == mr_ra_str->data.ptr))
    goto free_mr_ra;

  int length = strlen (str);
  char * tail = mr_rarray_append (mr_ra_str, length);

  if (NULL == tail)
    goto free_mr_ra;

  strcpy (&tail[-1], str);
  return (length);

 free_mr_ra:
  if (mr_ra_str->data.ptr)
    MR_FREE (mr_ra_str->data.ptr);
  mr_ra_str->data.ptr = NULL;
  mr_ra_str->MR_SIZE = mr_ra_str->alloc_size = 0;
  return (-1);
}

char mr_esc_char_map[MR_ESC_CHAR_MAP_SIZE] =
  {
    [(unsigned char)'\a'] = (unsigned char)'a',
    [(unsigned char)'\b'] = (unsigned char)'b',
    [(unsigned char)'\t'] = (unsigned char)'t',
    [(unsigned char)'\n'] = (unsigned char)'n',
    [(unsigned char)'\v'] = (unsigned char)'v',
    [(unsigned char)'\f'] = (unsigned char)'f',
    [(unsigned char)'\r'] = (unsigned char)'r',
    [(unsigned char)'\\'] = (unsigned char)'\\',
  };

/**
 * Quote string.
 * @param mr_ra_str output buffer
 * @param str string pointer
 * @return number of outputed chars
 */
int mr_ra_printf_quote_string (mr_rarray_t * mr_ra_str, char * str, char * char_pattern)
{
  char * ptr;
  int count = 0;
  
  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '"'));
  
  for (ptr = str; *ptr; ++ptr)
    {
      char mapped = mr_esc_char_map[(unsigned char)*ptr];
      if (*ptr == '"')
	mapped = '"';
      if (mapped)
	{
	  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '\\'));
	  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, mapped));
	}
      else if (isprint (*ptr))
	count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, *ptr));
      else
	count += TRY_CATCH_THROW (mr_ra_printf (mr_ra_str, char_pattern, *(unsigned char *)ptr));
    }
  
  count += TRY_CATCH_THROW (mr_ra_append_char (mr_ra_str, '"'));
  return (count);
}

