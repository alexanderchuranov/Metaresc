#include <metaresc.h>
#include <mr_save.h> /* MR_ONE_SHIFT */
#include <mr_stringify.h>

static int
mr_print_pointer (FILE * fd, mr_type_t mr_type_aux, mr_type_class_t mr_type_class, ssize_t size, char * type, char * method, void * value)
{
#define MR_TYPE_NAME(TYPE) [MR_TYPE_DETECT (TYPE)] = MR_STRINGIFY_READONLY (TYPE),
  static char * type_name[] = {
    MR_FOREACH (MR_TYPE_NAME,
		string_t, char, bool,
		int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, mr_int128_t, mr_uint128_t,
		float, complex_float_t, double, complex_double_t, long_double_t, complex_long_double_t)
  };

  if (NULL == value)
    return (fprintf (fd, "(null)"));

  if ((mr_type_aux > 0) && (mr_type_aux < sizeof (type_name) / sizeof (type_name[0])))
    if (type_name[mr_type_aux] && (NULL == type))
      type = type_name[mr_type_aux];

  if (NULL == type)
    return (fprintf (fd, "%p", value));

#undef MR_SAVE
#define MR_SAVE MR_SAVE_VOID_PTR
#define MR_SAVE_VOID_PTR(MR_TYPE_NAME_STR, S_PTR) ({			\
      void * __ptr__ = S_PTR;						\
      mr_fd_t __fd__;							\
      memset (&__fd__, 0, sizeof (__fd__));				\
      __fd__.name.str = MR_TYPE_NAME_STR;				\
      __fd__.stype.type = MR_TYPE_NAME_STR;				\
      __fd__.non_persistent = true;					\
      mr_detect_type (&__fd__);						\
      if (__fd__.stype.tdp && (size >= 0))				\
	{								\
	  __fd__.stype.size = size;					\
	  __fd__.stype.mr_type_aux = __fd__.stype.mr_type;		\
	  __fd__.stype.mr_type = MR_TYPE_ARRAY;				\
	  __fd__.stype.dim.dim[0] = size / __fd__.stype.tdp->size;	\
	  __fd__.stype.dim.size = sizeof (__fd__.stype.dim.dim[0]);	\
	}								\
      mr_save (__ptr__, &__fd__);					\
    })

#define MR_POINTER_TMPLT "%p "
#define MR_TYPE_TMPLT "(%s%s)"
#define MR_TYPED_POINTER_TMPLT MR_POINTER_TMPLT MR_TYPE_TMPLT
  char * serialized = NULL;
  char typed_ptr_str[sizeof (MR_TYPED_POINTER_TMPLT) + sizeof (void *) * 2 + strlen (type)];
  typed_ptr_str[0] = 0;

  if (MR_POINTER_TYPE_CLASS == mr_type_class)
    snprintf (typed_ptr_str, sizeof (typed_ptr_str), MR_POINTER_TMPLT, value);

  if (0 == strcmp (method, "CINIT"))
    {
      serialized = MR_SAVE_CINIT (type, value);
      if (MR_POINTER_TYPE_CLASS == mr_type_class)
	snprintf (typed_ptr_str, sizeof (typed_ptr_str), MR_TYPED_POINTER_TMPLT, value, type, (size >= 0) ? "[]" : "");
    }
  else if (0 == strcmp (method, "JSON"))
    serialized = MR_SAVE_JSON (type, value);
  else if (0 == strcmp (method, "SCM"))
    serialized = MR_SAVE_SCM (type, value);
  else if (0 == strcmp (method, "XML"))
    serialized = MR_SAVE_XML (type, value);
  else if (0 == strcmp (method, "XML1"))
    serialized = MR_SAVE_XML1 (type, value);
#ifdef HAVE_LIBXML2
  else if (0 == strcmp (method, "XML2"))
    serialized = MR_SAVE_XML2 (type, value);
#endif /* HAVE_LIBXML2 */
#ifdef HAVE_LIBYAML
  else if (0 == strcmp (method, "YAML"))
    serialized = MR_SAVE_YAML (type, value);
#endif /* HAVE_LIBYAML */

  if (NULL == serialized)
    return (fprintf (fd, "%p", value));

  serialized[strlen (serialized) - 1] = 0;
  int rv = fprintf (fd, "%s%s", typed_ptr_str, serialized);
  MR_FREE (serialized);
  return (rv);
}

int
mr_print_value (FILE * fd, mr_type_t mr_type, mr_type_t mr_type_aux, mr_type_class_t mr_type_class, ssize_t size, char * type, char * method, ...)
{
  static const char * const formats[] =
    {
      [MR_TYPE_INT8]   = "%" SCNi8,
      [MR_TYPE_UINT8]  = "%" SCNu8,
      [MR_TYPE_INT16]  = "%" SCNi16,
      [MR_TYPE_UINT16] = "%" SCNu16,
      [MR_TYPE_INT32]  = "%" SCNi32,
      [MR_TYPE_UINT32] = "%" SCNu32,
      [MR_TYPE_INT64]  = "%" SCNi64,
      [MR_TYPE_UINT64] = "%" SCNu64,
      [MR_TYPE_FLOAT]  = "%g",
      [MR_TYPE_DOUBLE] = "%g",
      [MR_TYPE_LONG_DOUBLE] = "%Lg",
      [MR_TYPE_CHAR]   = "%c",
      [MR_TYPE_CHAR_ARRAY] = "%s",
      [MR_TYPE_STRING] = "%s",
    };
  const char * format = NULL;
  int rv = 0;
  va_list args;
  char * serialized_enum = NULL;

  va_start (args, method);

#define MR_ENUM_INT_TYPES_LIST int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t
#define MR_TYPE_MASK(TYPE) MR_ONE_SHIFT (MR_TYPE_DETECT (TYPE))
#define MR_ENUM_INT_TYPES (0 MR_FOREACH (MR_TYPE_MASK, MR_ENUM_INT_TYPES_LIST))
  if (type && (MR_ENUM_INT_TYPES >> mr_type) & 1)
    {
      mr_td_t * tdp = mr_get_td_by_name (type);
      if (tdp && (MR_TYPE_ENUM == tdp->mr_type))
	{
	  mr_enum_value_type_t value = 0;
	  mr_fd_t fd = { .stype.tdp = tdp, };
	  mr_ptrdes_t ptrdes = { .data.ptr = &value, .fdp = &fd, };
	  mr_rarray_t mr_ra_str = {
	    .data = { mr_strdup ("") },
	    .MR_SIZE = sizeof (""),
	    .type = "string",
	    .alloc_size = sizeof (""),
	  };

	  switch (mr_type)
	    {
#define MR_CASE_TYPE(TYPE)					\
	      case MR_TYPE_DETECT (TYPE):			\
		value = va_arg (args, typeof (0 + (TYPE)0));	\
		break;
	      MR_FOREACH (MR_CASE_TYPE, MR_ENUM_INT_TYPES_LIST);
	    default:
	      break;
	    }

	  mr_ra_printf_bitmask (&mr_ra_str, &ptrdes, " | ");
	  serialized_enum = mr_ra_str.data.string;
	}
    }

  if ((mr_type >= 0) &&
      (mr_type < sizeof (formats) / sizeof (formats[0])))
    format = formats[mr_type];

  if (serialized_enum)
    {
      rv = fprintf (fd, "%s", serialized_enum);
      MR_FREE (serialized_enum);
    }
  else if (format != NULL)
    rv = vfprintf (fd, format, args);
  else
    switch (mr_type)
      {
      case MR_TYPE_BOOL:
	{
	  bool value = va_arg (args, int);
	  if (value)
	    rv = fprintf (fd, "true");
	  else
	    rv = fprintf (fd, "false");
	  break;
	}
      case MR_TYPE_INT128:
	{
	  mr_intmax_t value = va_arg (args, mr_intmax_t);
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
	  rv = fprintf (fd, "%s", out);
	  break;
	}
      case MR_TYPE_UINT128:
	{
	  mr_uintmax_t value = va_arg (args, mr_uintmax_t);
	  char buffer[(sizeof (value) * 12 + 4) / 5 + sizeof ("-")]; /* log10 (256) = 12/5 */
	  char * out = mr_uintmaxtostr (&buffer[sizeof (buffer)], value);
	  rv = fprintf (fd, "%s", out);
	  break;
	}
      case MR_TYPE_COMPLEX_FLOAT:
	{
	  complex_float_t value = va_arg (args, complex_float_t);
	  rv = fprintf (fd, "%g%+gI", __real__ value, __imag__ value);
	  break;
	}
      case MR_TYPE_COMPLEX_DOUBLE:
	{
	  complex_double_t value = va_arg (args, complex_double_t);
	  rv = fprintf (fd, "%g%+gI", __real__ value, __imag__ value);
	  break;
	}
      case MR_TYPE_COMPLEX_LONG_DOUBLE:
	{
	  complex_long_double_t value = va_arg (args, complex_long_double_t);
	  rv = fprintf (fd, "%Lg%+LgI", __real__ value, __imag__ value);
	  break;
	}
      case MR_TYPE_NONE:
	rv = mr_print_pointer (fd, mr_type_aux, mr_type_class, size, type, method, va_arg (args, void *));
	break;
	
      default:
	break;
      }
  va_end (args);
  return (rv);
}
