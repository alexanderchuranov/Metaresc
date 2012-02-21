/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <metaresc.h>

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

static int scm_named_fields[MR_MAX_TYPES] = {
  [0 ... MR_MAX_TYPES - 1] = MR_SCM_UNNAMED_FIELDS,
  [MR_TYPE_STRUCT] = MR_SCM_NAMED_FIELDS,
  [MR_TYPE_UNION] = MR_SCM_NAMED_FIELDS,
  [MR_TYPE_ANON_UNION] = MR_SCM_NAMED_FIELDS,
};

#define MR_SCM_IS_NOT_ARRAY (0)
#define MR_SCM_IS_ARRAY (!MR_SCM_IS_NOT_ARRAY)

static int scm_is_array[MR_MAX_TYPES] = {
  [0 ... MR_MAX_TYPES - 1] = MR_SCM_IS_NOT_ARRAY,
  [MR_TYPE_EXT_ARRAY] = MR_SCM_IS_ARRAY,
  [MR_TYPE_EXT_RARRAY] = MR_SCM_IS_ARRAY,
};

/**
 * Public function. Save scheduler. Save any object as a string.
 * @param ptrs resizeable array with pointers descriptors
 * @return stringified representation of object
 */
char *
scm_save (mr_ra_mr_ptrdes_t * ptrs)
{
  mr_rarray_t mr_ra_str = { .data = MR_STRDUP (""), .size = sizeof (""), .alloc_size = sizeof (""), .ext = { NULL }, };
  int idx = 0;

  if (NULL == mr_ra_str.data)
    return (NULL);
  
  while (idx >= 0)
    {
      mr_fd_t * fdp = &ptrs->ra.data[idx].fd;
      int level = MR_LIMIT_LEVEL (ptrs->ra.data[idx].level);
      int named_node = MR_SCM_UNNAMED_FIELDS;
      int parent = ptrs->ra.data[idx].parent;
      char * content = NULL;
      int in_comment = 0;

      if (parent >= 0)
	named_node = scm_named_fields[ptrs->ra.data[parent].fd.mr_type];

      if (ptrs->ra.data[idx].ref_idx >= 0)
	{
	  if (mr_ra_printf (&mr_ra_str, MR_SCM_INDENT_TEMPLATE MR_SCM_ATTR_INT,
			    level * MR_SCM_INDENT_SPACES, "",
			    (ptrs->ra.data[idx].flags & MR_PDF_CONTENT_REFERENCE) ? MR_REF_CONTENT : MR_REF,
			    ptrs->ra.data[ptrs->ra.data[idx].ref_idx].idx) < 0)
	    return (NULL);
	  else
	    in_comment = !0;
	}
      if (ptrs->ra.data[idx].flags & MR_PDF_IS_REFERENCED)
	{
	  if (mr_ra_printf (&mr_ra_str, MR_SCM_INDENT_TEMPLATE MR_SCM_ATTR_INT,
			    level * MR_SCM_INDENT_SPACES, "", MR_REF_IDX, ptrs->ra.data[idx].idx) < 0)
	    return (NULL);
	  else
	    in_comment = !0;
	}
	    
      if (ptrs->ra.data[idx].first_child < 0)
	{
	  /* route saving handler */
	  if ((fdp->mr_type_ext >= 0) && (fdp->mr_type_ext < MR_MAX_TYPES)
	      && mr_conf.io_ext_handlers[fdp->mr_type_ext].save.scm)
	    content = mr_conf.io_ext_handlers[fdp->mr_type_ext].save.scm (idx, ptrs);
	  else if ((fdp->mr_type >= 0) && (fdp->mr_type < MR_MAX_TYPES)
		   && mr_conf.io_handlers[fdp->mr_type].save.scm)
	    content = mr_conf.io_handlers[fdp->mr_type].save.scm (idx, ptrs);
	  else
	    MR_MESSAGE_UNSUPPORTED_NODE_TYPE_ (fdp);    

	  if (content)
	    {
	      if (MR_SCM_NAMED_FIELDS == named_node)
		{
		  if (mr_ra_printf (&mr_ra_str, MR_SCM_INDENT_TEMPLATE MR_SCM_NAMED_FIELD_START "%s" MR_SCM_NAMED_FIELD_END,
				    level * MR_SCM_INDENT_SPACES, "", content, ptrs->ra.data[idx].fd.name) < 0)
		    return (NULL);
		}
	      else
		{
		  if (in_comment)
		    if (mr_ra_printf (&mr_ra_str, MR_SCM_INDENT_TEMPLATE, level * MR_SCM_INDENT_SPACES, "") < 0)
		      return (NULL);
		  if (mr_ra_printf (&mr_ra_str, " %s", content) < 0)
		    return (NULL);
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
	  if (MR_SCM_IS_ARRAY == scm_is_array[fdp->mr_type_ext])
	    if (mr_ra_printf (&mr_ra_str, MR_SCM_ARRAY_PREFIX) < 0)
	      return (NULL);
	  if (mr_ra_printf (&mr_ra_str, MR_SCM_COMPAUND_START) < 0)
	    return (NULL);
	}
      
      if (ptrs->ra.data[idx].first_child >= 0)
	idx = ptrs->ra.data[idx].first_child;
      else
	{
	  while ((ptrs->ra.data[idx].next < 0) && (ptrs->ra.data[idx].parent >= 0))
	    {
	      idx = ptrs->ra.data[idx].parent;
	      level = MR_LIMIT_LEVEL (ptrs->ra.data[idx].level);
	      named_node = MR_SCM_UNNAMED_FIELDS;
	      parent = ptrs->ra.data[idx].parent;

	      if (parent >= 0)
		named_node = scm_named_fields[ptrs->ra.data[parent].fd.mr_type];
	      
	      if (mr_ra_printf (&mr_ra_str, MR_SCM_COMPAUND_END) < 0)
		return (NULL);
	      if (MR_SCM_NAMED_FIELDS == named_node)
		if (mr_ra_printf (&mr_ra_str, MR_SCM_NAMED_FIELD_END, ptrs->ra.data[idx].fd.name) < 0)
		  return (NULL);
	    }
	  idx = ptrs->ra.data[idx].next;
	}
    }

  return (mr_ra_str.data);
}

/**
 * MR_NONE type saving handler.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors
 * @return stringified int value
 */
static char *
scm_save_none (int idx, mr_ra_mr_ptrdes_t * ptrs)
{
  return (NULL);
}

/**
 * MR_XXX type saving handler. Make a string from *(XXX_t*)data.
 * \@param idx an index of node in ptrs
 * \@param ptrs resizeable array with pointers descriptors
 * \@return stringified int value
 */
#define SCM_SAVE_TYPE(TYPE) static char * scm_save_ ## TYPE (int idx, mr_ra_mr_ptrdes_t * ptrs) { return (mr_stringify_ ## TYPE (&ptrs->ra.data[idx])); }

SCM_SAVE_TYPE (int8);
SCM_SAVE_TYPE (uint8);
SCM_SAVE_TYPE (int16);
SCM_SAVE_TYPE (uint16);
SCM_SAVE_TYPE (int32);
SCM_SAVE_TYPE (uint32);
SCM_SAVE_TYPE (int64);
SCM_SAVE_TYPE (uint64);
SCM_SAVE_TYPE (enum);
SCM_SAVE_TYPE (float);
SCM_SAVE_TYPE (double);
SCM_SAVE_TYPE (long_double_t);
SCM_SAVE_TYPE (bitfield);

#define MR_SCM_BITMASK_OR_DELIMITER " "
#define MR_SCM_BITMASK_TEMPLATE "(bit-or %s)"

/**
 * MR_BITMASK type saving handler. Look up type descriptor and save as
 * stringified bitmask value or as integer otherwise.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors 
 * @return stringified enum value
 */
static char *
scm_save_bitmask (int idx, mr_ra_mr_ptrdes_t * ptrs)
{
  char * str = mr_stringify_bitmask (&ptrs->ra.data[idx], MR_SCM_BITMASK_OR_DELIMITER);
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
 * MR_CHAR type saving handler. Stringify char.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors 
 * @return stringified float value
 */
static char *
scm_save_char (int idx, mr_ra_mr_ptrdes_t * ptrs)
{
  char c = *(char*)ptrs->ra.data[idx].data;
  char str[] = "#\\x00";
  if (isprint (c))
    {
      str[2] = c;
      str[3] = 0;
    }
  else
    sprintf (&str[3], "%02x", (int)(unsigned char)c);
  return (MR_STRDUP (str));
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
scm_save_char_array (int idx, mr_ra_mr_ptrdes_t * ptrs)
{
  return (scm_quote_string (ptrs->ra.data[idx].data, '"'));
}

/**
 * MR_STRING type saving handler. Save string.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors 
 * @return string value
 */
static char *
scm_save_string (int idx, mr_ra_mr_ptrdes_t * ptrs)
{
  char * str = *(char**)ptrs->ra.data[idx].data;
  if ((NULL == str) || (ptrs->ra.data[idx].ref_idx >= 0))
    return (MR_STRDUP (MR_SCM_FALSE));
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
scm_save_pointer (int idx, mr_ra_mr_ptrdes_t * ptrs)
{ 
  return (MR_STRDUP (MR_SCM_FALSE));
}

/**
 * Dummy stub for compaund types.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors
 * @return empty string
 */
static char *
scm_save_empty (int idx, mr_ra_mr_ptrdes_t * ptrs)
{ 
  return (MR_STRDUP (MR_SCM_EMPTY));
}

/**
 * Init IO handlers Table
 */
static void __attribute__((constructor)) mr_init_save_scm (void)
{
  mr_conf.io_handlers[MR_TYPE_NONE].save.scm = scm_save_none;
  mr_conf.io_handlers[MR_TYPE_VOID].save.scm = scm_save_none;
  mr_conf.io_handlers[MR_TYPE_ENUM].save.scm = scm_save_enum;
  mr_conf.io_handlers[MR_TYPE_BITFIELD].save.scm = scm_save_bitfield;
  mr_conf.io_handlers[MR_TYPE_BITMASK].save.scm = scm_save_bitmask;
  mr_conf.io_handlers[MR_TYPE_INT8].save.scm = scm_save_int8;
  mr_conf.io_handlers[MR_TYPE_UINT8].save.scm = scm_save_uint8;
  mr_conf.io_handlers[MR_TYPE_INT16].save.scm = scm_save_int16;
  mr_conf.io_handlers[MR_TYPE_UINT16].save.scm = scm_save_uint16;
  mr_conf.io_handlers[MR_TYPE_INT32].save.scm = scm_save_int32;
  mr_conf.io_handlers[MR_TYPE_UINT32].save.scm = scm_save_uint32;
  mr_conf.io_handlers[MR_TYPE_INT64].save.scm = scm_save_int64;
  mr_conf.io_handlers[MR_TYPE_UINT64].save.scm = scm_save_uint64;
  mr_conf.io_handlers[MR_TYPE_FLOAT].save.scm = scm_save_float;
  mr_conf.io_handlers[MR_TYPE_DOUBLE].save.scm = scm_save_double;
  mr_conf.io_handlers[MR_TYPE_LONG_DOUBLE].save.scm = scm_save_long_double_t;
  mr_conf.io_handlers[MR_TYPE_CHAR].save.scm = scm_save_char;
  mr_conf.io_handlers[MR_TYPE_CHAR_ARRAY].save.scm = scm_save_char_array;
  mr_conf.io_handlers[MR_TYPE_STRING].save.scm = scm_save_string;
  mr_conf.io_handlers[MR_TYPE_STRUCT].save.scm = scm_save_empty;
  mr_conf.io_handlers[MR_TYPE_FUNC].save.scm = scm_save_none;
  mr_conf.io_handlers[MR_TYPE_FUNC_TYPE].save.scm = scm_save_none;
  mr_conf.io_handlers[MR_TYPE_UNION].save.scm = scm_save_empty;
  mr_conf.io_handlers[MR_TYPE_ANON_UNION].save.scm = scm_save_empty;

  mr_conf.io_ext_handlers[MR_TYPE_EXT_ARRAY].save.scm = scm_save_empty;
  mr_conf.io_ext_handlers[MR_TYPE_EXT_RARRAY_DATA].save.scm = scm_save_pointer;
  mr_conf.io_ext_handlers[MR_TYPE_EXT_POINTER].save.scm = scm_save_pointer;
}
