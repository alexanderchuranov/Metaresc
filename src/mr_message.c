#ifdef HAVE_CONFIG_H
# include <mr_config.h>
#endif /* HAVE_CONFIG_H */
#ifdef HAVE_EXECINFO_H
# include <execinfo.h>
#endif /* HAVE_EXECINFO_H */

#include <stdlib.h> /* free () */
#include <metaresc.h>

static int
mr_vscprintf (const char * format, va_list args) 
{
  va_list _args;
  va_copy (_args, args);
  int retval = vsnprintf (NULL, 0, format, _args);
  va_end (_args);
  return (retval);
}

static int 
mr_vasprintf (char ** strp, const char * fmt, va_list args) 
{
  *strp = NULL;
  int len = mr_vscprintf (fmt, args);
  if (len <= 0) 
    return (len);
  char * str = MR_CALLOC (len + 1, sizeof (*str));
  if (NULL == str) 
    return (-1);
  int _len = vsnprintf (str, len + 1, fmt, args);
  if (_len < 0)
    {
      MR_FREE (str);
      return (len);
    }
  *strp = str;
  return (_len);
}

/**
 * Format message. Allocates memory for message that need to be freed.
 * @param message_id message template string ID
 * @param args variadic agruments
 * @return message string allocated by standard malloc. Need to be freed outside.
 */
char *
mr_message_format (mr_message_id_t message_id, va_list args)
{
  static const char * messages[MR_MESSAGE_LAST + 1] = { [0 ... MR_MESSAGE_LAST] = NULL };
  static bool messages_inited = false;
  const char * format = "Unknown MR_MESSAGE_ID.";
  char * message = NULL;

  if (!messages_inited)
    {
      mr_td_t * tdp = mr_get_td_by_name_internal ("mr_message_id_t");
      if (tdp)
	{
	  int i;
	  for (i = 0; tdp->param.enum_param.enums[i] != NULL; ++i)
	    messages[tdp->param.enum_param.enums[i]->value._unsigned] = tdp->param.enum_param.enums[i]->meta;
	  messages_inited = true;
	}
    }

  if ((message_id <= sizeof (messages) / sizeof (messages[0])) && messages[message_id])
    format = messages[message_id];

  if (format)
    (void)mr_vasprintf (&message, format, args);

  return (message);
}

/**
 * Redirect message to user defined handler or output message to stderr.
 * @param file_name file name
 * @param func_name function name
 * @param line line number
 * @param log_level logging level of message
 * @param message_id message template string ID
 */
void
mr_message (const char * file_name, const char * func_name, int line, mr_log_level_t log_level, mr_message_id_t message_id, ...)
{
  char * message;
  va_list args;

  va_start (args, message_id);
  /* if we have user defined message handler then pass error to it */
  if (mr_conf.msg_handler)
    mr_conf.msg_handler (file_name, func_name, line, log_level, message_id, args);
  else if (log_level >= mr_conf.log_level)
    {
      const char * log_level_str_ = "Unknown";
#define LL_INIT(LEVEL) [MR_LL_##LEVEL] = #LEVEL,
      static const char * log_level_str[] =
	{ MR_FOREACH (LL_INIT, ALL, TRACE, DEBUG, INFO, WARN, ERROR, FATAL, OFF) };

      if (((int)log_level >= 0)
	  && ((int)log_level <= sizeof (log_level_str) / sizeof (log_level_str[0])))
	if (log_level_str[log_level])
	  log_level_str_ = log_level_str[log_level];

#ifdef HAVE_EXECINFO_H
      if (log_level <= MR_LL_DEBUG)
	{
	  void * array[8];
	  size_t size;
	  char ** strings;
	  size_t i;

	  size = backtrace (array, sizeof (array) / sizeof (array[0]));
	  strings = backtrace_symbols (array, size);
	  fprintf (stderr, "Obtained %zd stack frames.\n", size);
	  if (strings)
	    {
	      for (i = 0; i < size; ++i)
		fprintf (stderr, "%s\n", strings[i]);
	      free (strings);
	    }
	}
#endif /* HAVE_EXECINFO_H */
      
      message = mr_message_format (message_id, args);
      if (NULL == message)
	fprintf (stderr, "%s: in %s %s() line %d: Out of memory formating error message #%d\n", log_level_str_, file_name, func_name, line, message_id);
      else
	{
	  fprintf (stderr, "%s: in %s %s() line %d: %s\n", log_level_str_, file_name, func_name, line, message);
	  MR_FREE (message);
	}
    }
  va_end (args);
}

/**
 * printf into resizable array
 * @param mr_ra_str a pointer on resizable array
 * @param format standard printf format string
 * @param ... arguments for printf
 * @return length of added content and -1 in case of memory allocation failure
 */
int
mr_ra_printf (mr_rarray_t * mr_ra_str, const char * format, ...)
{
  va_list args;
  int length, _length;

  va_start (args, format);
  length = mr_vscprintf (format, args);

  if (length < 0)
    goto free_mr_ra;

  if ((0 == mr_ra_str->MR_SIZE) || (NULL == mr_ra_str->data.ptr))
    goto free_mr_ra;

  size_t size = mr_ra_str->MR_SIZE;
  char * tail = mr_rarray_append (mr_ra_str, length);

  if (NULL == tail)
    goto free_mr_ra;
  else
    _length = vsnprintf (tail - 1, length + 1, format, args);

  if (_length < 0)
    goto free_mr_ra;
  
  mr_ra_str->MR_SIZE = _length + size;

  va_end (args);
  return (_length);

 free_mr_ra:
  if (mr_ra_str->data.ptr)
    MR_FREE (mr_ra_str->data.ptr);
  mr_ra_str->data.ptr = NULL;
  mr_ra_str->MR_SIZE = mr_ra_str->alloc_size = 0;
  va_end (args);  
  return (-1);
}
