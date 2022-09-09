#include <metaresc.h>

int
mr_print_value (FILE * fd, mr_type_t mr_type, mr_type_t mr_type_aux, char * type, mr_size_t size, ...)
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
  va_start (args, size);

  if ((mr_type >= 0) &&
      (mr_type < sizeof (formats) / sizeof (formats[0])))
    format = formats[mr_type];
  
  if (format != NULL)
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
	{
	  void * value = va_arg (args, void *);
	  if (NULL == value)
	    rv = fprintf (fd, "(null)");
	  else if (mr_type_aux != MR_TYPE_NONE)
	    {
	      rv = fprintf (fd, "%p (", value);
	      
#define MR_REDIRECT_TYPE(TYPE)						\
	      case MR_TYPE_DETECT (TYPE):				\
		rv += fprintf (fd, "(" #TYPE ")") +			\
		  mr_print_value (fd, mr_type_aux, MR_TYPE_NONE, NULL, 0, *(TYPE*)value); \
		break;
	      
	      switch (mr_type_aux)
		{
		  MR_FOREACH (MR_REDIRECT_TYPE,
			      string_t, mr_string_t, char, bool,
			      int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t,
			      float, complex_float_t, double, complex_double_t, long_double_t, complex_long_double_t);
		default:
		  rv += fprintf (fd, "unknown");
		  break;
		}
	      rv += fprintf (fd, ")");
	    }
	  else if (NULL == type)
	    rv = fprintf (fd, "%p", value);
	  else
	    {
#undef MR_SAVE
#define MR_SAVE MR_SAVE_VOID_PTR
#define MR_SAVE_VOID_PTR(MR_TYPE_NAME_STR, S_PTR) ({			\
      mr_save_data_t __mr_save_data__;					\
      void * __ptr__ = S_PTR;						\
      mr_fd_t __fd__;							\
      memset (&__fd__, 0, sizeof (__fd__));				\
      __fd__.name.str = mr_normalize_name (MR_STRINGIFY (S_PTR));	\
      __fd__.type = MR_TYPE_NAME_STR;					\
      __fd__.non_persistent = true;					\
      mr_detect_type (&__fd__);						\
      if (size > 0)							\
	{								\
	  __fd__.mr_type_aux = __fd__.mr_type;				\
	  __fd__.mr_type = MR_TYPE_ARRAY;				\
	  __fd__.param.array_param.count = size / __fd__.size;		\
	  __fd__.param.array_param.row_count = 1;			\
	  __fd__.size = size;						\
	}								\
      memset (&__mr_save_data__, 0, sizeof (__mr_save_data__));		\
      if (__ptr__ != NULL)						\
	mr_save (__ptr__, &__fd__, &__mr_save_data__);			\
      __mr_save_data__.ptrs;						\
    })

	      char * serialized = MR_SAVE_CINIT (type, value);
	      if (NULL == serialized)
		rv = fprintf (fd, "%p", value);
	      else
		{
		  serialized[strlen (serialized) - 1] = 0;
		  rv = fprintf (fd, "%p ((%s)%s)", value, type, serialized);
		  MR_FREE (serialized);
		}
	    }
	  break;
	}
      default:
	break;
      }
  va_end (args);
  return (rv);
}
