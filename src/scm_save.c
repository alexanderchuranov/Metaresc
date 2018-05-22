/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <metaresc.h>
#include <mr_stringify.h>

TYPEDEF_FUNC (char *, scm_save_handler_t, (int, mr_ra_ptrdes_t *))

#define MR_SCM_EMPTY "()"
#define MR_SCM_FALSE "#f"
#define MR_SCM_TRUE "#t"

#define MR_SCM_INDENT_SPACES (2)
#define MR_SCM_INDENT_TEMPLATE "\n%*s"

#define MR_SCM_NAMED_FIELD_START "("
#define MR_SCM_NAMED_FIELD_END " . %s)"
#define MR_SCM_COMPAUND_START "("
#define MR_SCM_COMPAUND_END ")"
#define MR_SCM_ARRAY_PREFIX "#"
#define MR_SCM_ATTR_INT ";(%s %d)"

#define MR_SCM_UNNAMED_FIELDS (0)
#define MR_SCM_NAMED_FIELDS (!MR_SCM_UNNAMED_FIELDS)

static int scm_named_fields[MR_TYPE_LAST] = {
  [0 ... MR_TYPE_LAST - 1] = MR_SCM_UNNAMED_FIELDS,
  [MR_TYPE_STRUCT] = MR_SCM_NAMED_FIELDS,
  [MR_TYPE_UNION] = MR_SCM_NAMED_FIELDS,
  [MR_TYPE_ANON_UNION] = MR_SCM_NAMED_FIELDS,
  [MR_TYPE_NAMED_ANON_UNION] = MR_SCM_NAMED_FIELDS,
};

/**
 * MR_NONE type saving handler.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors
 * @return stringified int value
 */
static char *
scm_save_none (int idx, mr_ra_ptrdes_t * ptrs)
{
  return (NULL);
}

#define MR_SCM_BITMASK_OR_DELIMITER " "
#define MR_SCM_BITMASK_TEMPLATE "(logior %s)"
#define MR_SCM_COMPLEX_FLOAT_TEMPLATE "(+ %s %si)"

static char * mr_stringify_float_scm (mr_ptrdes_t * ptrdes) { return (mr_output_format_float (ptrdes)); }
static char * mr_stringify_double_scm (mr_ptrdes_t * ptrdes) { return (mr_output_format_double (ptrdes)); }
static char * mr_stringify_long_double_t_scm (mr_ptrdes_t * ptrdes) { return (mr_output_format_long_double_t (ptrdes)); }

static MR_OUTPUT_FORMAT_COMPLEX (float_scm, float, MR_TYPE_FLOAT, MR_SCM_COMPLEX_FLOAT_TEMPLATE);
static MR_OUTPUT_FORMAT_COMPLEX (double_scm, double, MR_TYPE_DOUBLE, MR_SCM_COMPLEX_FLOAT_TEMPLATE);
static MR_OUTPUT_FORMAT_COMPLEX (long_double_t_scm, long double, MR_TYPE_LONG_DOUBLE, MR_SCM_COMPLEX_FLOAT_TEMPLATE);

#define SCM_SAVE_COMPLEX_FLOAT_TYPE(TYPE) static char * scm_save_ ## TYPE (int idx, mr_ra_ptrdes_t * ptrs) { return (mr_output_format_ ## TYPE (&ptrs->ra[idx])); }

SCM_SAVE_COMPLEX_FLOAT_TYPE (complex_float_scm);
SCM_SAVE_COMPLEX_FLOAT_TYPE (complex_double_scm);
SCM_SAVE_COMPLEX_FLOAT_TYPE (complex_long_double_t_scm);

/**
 * MR_XXX type saving handler. Make a string from *(XXX_t*)data.
 * \@param idx an index of node in ptrs
 * \@param ptrs resizeable array with pointers descriptors
 * \@return stringified int value
 */
#define SCM_SAVE_TYPE(TYPE) static char * scm_save_ ## TYPE (int idx, mr_ra_ptrdes_t * ptrs) { return (mr_stringify_ ## TYPE (&ptrs->ra[idx])); }

SCM_SAVE_TYPE (int8_t);
SCM_SAVE_TYPE (uint8_t);
SCM_SAVE_TYPE (int16_t);
SCM_SAVE_TYPE (uint16_t);
SCM_SAVE_TYPE (int32_t);
SCM_SAVE_TYPE (uint32_t);
SCM_SAVE_TYPE (int64_t);
SCM_SAVE_TYPE (uint64_t);
SCM_SAVE_TYPE (enum);
SCM_SAVE_TYPE (float);
SCM_SAVE_TYPE (double);
SCM_SAVE_TYPE (long_double_t);
SCM_SAVE_TYPE (bitfield);

static char *
scm_save_bool (int idx, mr_ra_ptrdes_t * ptrs)
{
  return (*(bool*)ptrs->ra[idx].data.ptr ? mr_strdup (MR_SCM_TRUE) : mr_strdup (MR_SCM_FALSE));
}

/**
 * MR_BITMASK type saving handler. Look up type descriptor and save as
 * stringified bitmask value or as integer otherwise.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors
 * @return stringified enum value
 */
static char *
scm_save_bitmask (int idx, mr_ra_ptrdes_t * ptrs)
{
  char * str = mr_stringify_bitmask (&ptrs->ra[idx], MR_SCM_BITMASK_OR_DELIMITER);
  if (str)
    {
      char * str_ = MR_MALLOC ((sizeof (MR_SCM_BITMASK_TEMPLATE) - 1) + (strlen (str) - 2) + 1);
      if (NULL == str_)
	{
	  MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	  return (NULL);
	}
      sprintf (str_, MR_SCM_BITMASK_TEMPLATE, str);
      MR_FREE (str);
      str = str_;
    }
  return (str);
}

/**
 * MR_TYPE_FUNC & MR_TYPE_FUNC_TYPE type saving handler.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors
 * @return stringified function name
 */
static char *
scm_save_func (int idx, mr_ra_ptrdes_t * ptrs)
{
  if (true == ptrs->ra[idx].flags.is_null)
    return (mr_strdup (MR_SCM_FALSE));
  else
    return (mr_stringify_func (&ptrs->ra[idx]));
}

/**
 * MR_CHAR type saving handler. Stringify char.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors
 * @return stringified char
 */
static char *
scm_save_char (int idx, mr_ra_ptrdes_t * ptrs)
{
  char c = *(char*)ptrs->ra[idx].data.ptr;
  char str[] = "#\\x00";
  if (isprint (c))
    {
      str[2] = c;
      str[3] = 0;
    }
  else
    sprintf (str, "#\\x%02x", (int)(unsigned char)c);
  return (mr_strdup (str));
}

/**
 * Quote string.
 * @param str string pointer
 * @param quote quote character
 * @return quoted string
 */
static char *
scm_quote_string (char * str, char quote)
{
  int length = 0;
  char * str_;
  char * ptr;

  for (ptr = str; *ptr; ++ptr)
    {
      if ((quote == *ptr) || ('\\' == *ptr))
	++length;
      if (!isprint (*ptr))
	length += 4;
      ++length;
    }
  str_ = MR_MALLOC (length + 3);
  if (NULL == str_)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (NULL);
    }
  length = 0;
  str_[length++] = quote;
  for (ptr = str; *ptr; ++ptr)
    {
      if ((quote == *ptr) || ('\\' == *ptr))
	str_[length++] = '\\';
      if (isprint (*ptr))
	str_[length++] = *ptr;
      else
	{
	  str_[length++] = '\\';
	  sprintf (&str_[length], "x%02x;", (int)(unsigned char)*ptr);
	  length += 4;
	}
    }
  str_[length++] = quote;
  str_[length++] = 0;
  return (str_);
}

/**
 * MR_CHAR_ARRAY type saving handler. Save char array.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors
 * @return char array value
 */
static char *
scm_save_char_array (int idx, mr_ra_ptrdes_t * ptrs)
{
  return (scm_quote_string (ptrs->ra[idx].data.ptr, '"'));
}

/**
 * MR_STRING type saving handler. Save string.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors
 * @return string value
 */
static char *
scm_save_string (int idx, mr_ra_ptrdes_t * ptrs)
{
  char * str = *(char**)ptrs->ra[idx].data.ptr;
  if ((NULL == str) || (ptrs->ra[idx].ref_idx >= 0))
    return (mr_strdup (MR_SCM_FALSE));
  else
    return (scm_quote_string (str, '"'));
}

/**
 * MR_POINTER type saving handler. Save pointer as a string.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors
 * @return stringified pointer
 */
static char *
scm_save_pointer (int idx, mr_ra_ptrdes_t * ptrs)
{
  return (mr_strdup (MR_SCM_FALSE));
}

/**
 * Dummy stub for compaund types.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors
 * @return empty string
 */
static char *
scm_save_empty (int idx, mr_ra_ptrdes_t * ptrs)
{
  return (mr_strdup (MR_SCM_EMPTY));
}

/**
 * Init IO handlers Table
 */
static scm_save_handler_t scm_save_handler[] =
  {
    [MR_TYPE_NONE] = scm_save_none,
    [MR_TYPE_VOID] = scm_save_none,
    [MR_TYPE_ENUM] = scm_save_enum,
    [MR_TYPE_BITFIELD] = scm_save_bitfield,
    [MR_TYPE_BITMASK] = scm_save_bitmask,
    [MR_TYPE_BOOL] = scm_save_bool,
    [MR_TYPE_INT8] = scm_save_int8_t,
    [MR_TYPE_UINT8] = scm_save_uint8_t,
    [MR_TYPE_INT16] = scm_save_int16_t,
    [MR_TYPE_UINT16] = scm_save_uint16_t,
    [MR_TYPE_INT32] = scm_save_int32_t,
    [MR_TYPE_UINT32] = scm_save_uint32_t,
    [MR_TYPE_INT64] = scm_save_int64_t,
    [MR_TYPE_UINT64] = scm_save_uint64_t,
    [MR_TYPE_FLOAT] = scm_save_float,
    [MR_TYPE_COMPLEX_FLOAT] = scm_save_complex_float_scm,
    [MR_TYPE_DOUBLE] = scm_save_double,
    [MR_TYPE_COMPLEX_DOUBLE] = scm_save_complex_double_scm,
    [MR_TYPE_LONG_DOUBLE] = scm_save_long_double_t,
    [MR_TYPE_COMPLEX_LONG_DOUBLE] = scm_save_complex_long_double_t_scm,
    [MR_TYPE_CHAR] = scm_save_char,
    [MR_TYPE_CHAR_ARRAY] = scm_save_char_array,
    [MR_TYPE_STRING] = scm_save_string,
    [MR_TYPE_STRUCT] = scm_save_empty,
    [MR_TYPE_FUNC] = scm_save_func,
    [MR_TYPE_FUNC_TYPE] = scm_save_func,
    [MR_TYPE_ARRAY] = scm_save_empty,
    [MR_TYPE_POINTER] = scm_save_pointer,
    [MR_TYPE_UNION] = scm_save_empty,
    [MR_TYPE_ANON_UNION] = scm_save_empty,
    [MR_TYPE_NAMED_ANON_UNION] = scm_save_empty,
  };

static void *
free_and_return_null (char * content)
{
  if (content)
    MR_FREE (content);
  return (NULL);
}

/**
 * Public function. Save scheduler. Save any object as a string.
 * @param ptrs resizeable array with pointers descriptors
 * @return stringified representation of object
 */
char *
scm_save (mr_ra_ptrdes_t * ptrs)
{
  mr_rarray_t mr_ra_str = { .data = { mr_strdup ("") }, .MR_SIZE = sizeof (""), .type = "char", .alloc_size = sizeof (""), };
  int idx = 0;

  if (NULL == mr_ra_str.data.ptr)
    return (NULL);

  while (idx >= 0)
    {
      mr_fd_t * fdp = &ptrs->ra[idx].fd;
      int level = MR_LIMIT_LEVEL (ptrs->ra[idx].save_params.level);
      int named_node = MR_SCM_UNNAMED_FIELDS;
      int parent = ptrs->ra[idx].parent;
      char * content = NULL;
      int in_comment = false;

      if (parent >= 0)
	named_node = scm_named_fields[ptrs->ra[parent].fd.mr_type];

      if (ptrs->ra[idx].ref_idx >= 0)
	{
	  if (mr_ra_printf (&mr_ra_str, MR_SCM_INDENT_TEMPLATE MR_SCM_ATTR_INT,
			    level * MR_SCM_INDENT_SPACES, "",
			    (ptrs->ra[idx].flags.is_content_reference) ? MR_REF_CONTENT : MR_REF,
			    ptrs->ra[ptrs->ra[idx].ref_idx].idx) < 0)
	    return (NULL);
	  else
	    in_comment = true;
	}
      if (ptrs->ra[idx].flags.is_referenced)
	{
	  if (mr_ra_printf (&mr_ra_str, MR_SCM_INDENT_TEMPLATE MR_SCM_ATTR_INT,
			    level * MR_SCM_INDENT_SPACES, "", MR_REF_IDX, ptrs->ra[idx].idx) < 0)
	    return (NULL);
	  else
	    in_comment = true;
	}

      if (ptrs->ra[idx].first_child < 0)
	{
	  /* route saving handler */
	  if ((fdp->mr_type < MR_TYPE_LAST) && scm_save_handler[fdp->mr_type])
	    content = scm_save_handler[fdp->mr_type] (idx, ptrs);
	  else
	    MR_MESSAGE_UNSUPPORTED_NODE_TYPE_ (fdp);

	  if (content)
	    {
	      if (MR_SCM_NAMED_FIELDS == named_node)
		{
		  if (mr_ra_printf (&mr_ra_str, MR_SCM_INDENT_TEMPLATE MR_SCM_NAMED_FIELD_START "%s" MR_SCM_NAMED_FIELD_END,
				    level * MR_SCM_INDENT_SPACES, "", content, ptrs->ra[idx].fd.name.str) < 0)
		    return (free_and_return_null (content));
		}
	      else
		{
		  if (in_comment)
		    if (mr_ra_printf (&mr_ra_str, MR_SCM_INDENT_TEMPLATE, level * MR_SCM_INDENT_SPACES, "") < 0)
		      return (free_and_return_null (content));
		  if (mr_ra_printf (&mr_ra_str, " %s", content) < 0)
		    return (free_and_return_null (content));
		}
	      MR_FREE (content);
	    }
	}
      else
	{
	  if ((idx != 0) || in_comment)
	    if (mr_ra_printf (&mr_ra_str, MR_SCM_INDENT_TEMPLATE, level * MR_SCM_INDENT_SPACES, "") < 0)
	      return (NULL);
	  if (MR_SCM_NAMED_FIELDS == named_node)
	    if (mr_ra_printf (&mr_ra_str, MR_SCM_NAMED_FIELD_START) < 0)
	      return (NULL);
	  if (MR_TYPE_ARRAY == fdp->mr_type)
	    if (mr_ra_printf (&mr_ra_str, MR_SCM_ARRAY_PREFIX) < 0)
	      return (NULL);
	  if (mr_ra_printf (&mr_ra_str, MR_SCM_COMPAUND_START) < 0)
	    return (NULL);
	}

      if (ptrs->ra[idx].first_child >= 0)
	idx = ptrs->ra[idx].first_child;
      else
	{
	  while ((ptrs->ra[idx].next < 0) && (ptrs->ra[idx].parent >= 0))
	    {
	      idx = ptrs->ra[idx].parent;
	      named_node = MR_SCM_UNNAMED_FIELDS;
	      parent = ptrs->ra[idx].parent;

	      if (parent >= 0)
		named_node = scm_named_fields[ptrs->ra[parent].fd.mr_type];

	      if (mr_ra_printf (&mr_ra_str, MR_SCM_COMPAUND_END) < 0)
		return (NULL);
	      if (MR_SCM_NAMED_FIELDS == named_node)
		if (mr_ra_printf (&mr_ra_str, MR_SCM_NAMED_FIELD_END, ptrs->ra[idx].fd.name.str) < 0)
		  return (NULL);
	    }
	  idx = ptrs->ra[idx].next;
	}
    }

  return (mr_ra_str.data.ptr);
}
