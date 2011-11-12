/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

#include <reslib.h>

#define RL_SCM_EMPTY "()"
#define RL_SCM_FALSE "#f"
#define RL_SCM_TRUE "#t"

#define RL_SCM_INDENT_SPACES (2)
#define RL_SCM_INDENT_TEMPLATE "\n%*s"

#define RL_SCM_NAMED_FIELD_START "("
#define RL_SCM_NAMED_FIELD_END " . %s)"
#define RL_SCM_COMPAUND_START "("
#define RL_SCM_COMPAUND_END ")"
#define RL_SCM_ARRAY_PREFIX "#"
#define RL_SCM_ATTR_INT ";(%s %d)"

#define RL_SCM_UNNAMED_FIELDS (0)
#define RL_SCM_NAMED_FIELDS (!RL_SCM_UNNAMED_FIELDS)

static int scm_named_fields[RL_MAX_TYPES] = {
  [0 ... RL_MAX_TYPES - 1] = RL_SCM_UNNAMED_FIELDS,
  [RL_TYPE_STRUCT] = RL_SCM_NAMED_FIELDS,
  [RL_TYPE_UNION] = RL_SCM_NAMED_FIELDS,
  [RL_TYPE_ANON_UNION] = RL_SCM_NAMED_FIELDS,
};

#define RL_SCM_IS_NOT_ARRAY (0)
#define RL_SCM_IS_ARRAY (!RL_SCM_IS_NOT_ARRAY)

static int scm_is_array[RL_MAX_TYPES] = {
  [0 ... RL_MAX_TYPES - 1] = RL_SCM_IS_NOT_ARRAY,
  [RL_TYPE_EXT_ARRAY] = RL_SCM_IS_ARRAY,
  [RL_TYPE_EXT_RARRAY] = RL_SCM_IS_ARRAY,
};

/**
 * Public function. Save scheduler. Save any object as a string.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors
 * @return stringified representation of object
 */
char *
scm_save (rl_ra_rl_ptrdes_t * ptrs)
{
  rl_rarray_t rl_ra_str = { .data = RL_STRDUP (""), .size = sizeof (""), .alloc_size = sizeof (""), .ext = { NULL }, };
  int idx = 0;

  if (NULL == rl_ra_str.data)
    return (NULL);
  
  while (idx >= 0)
    {
      rl_fd_t * fdp = &ptrs->ra.data[idx].fd;
      int level = RL_LIMIT_LEVEL (ptrs->ra.data[idx].level);
      int named_node = RL_SCM_UNNAMED_FIELDS;
      int parent = ptrs->ra.data[idx].parent;
      char * content = NULL;
      int in_comment = 0;

      if (parent >= 0)
	named_node = scm_named_fields[ptrs->ra.data[parent].fd.rl_type];

      if (ptrs->ra.data[idx].ref_idx >= 0)
	{
	  if (rl_ra_printf (&rl_ra_str, RL_SCM_INDENT_TEMPLATE RL_SCM_ATTR_INT,
			    level * RL_SCM_INDENT_SPACES, "",
			    (ptrs->ra.data[idx].flags & RL_PDF_CONTENT_REFERENCE) ? RL_REF_CONTENT : RL_REF,
			    ptrs->ra.data[ptrs->ra.data[idx].ref_idx].idx) < 0)
	    return (NULL);
	  else
	    in_comment = !0;
	}
      if (ptrs->ra.data[idx].flags & RL_PDF_IS_REFERENCED)
	{
	  if (rl_ra_printf (&rl_ra_str, RL_SCM_INDENT_TEMPLATE RL_SCM_ATTR_INT,
			    level * RL_SCM_INDENT_SPACES, "", RL_REF_IDX, ptrs->ra.data[idx].idx) < 0)
	    return (NULL);
	  else
	    in_comment = !0;
	}
      if (ptrs->ra.data[idx].flags & RL_PDF_RARRAY_SIZE)
	{
	  if (rl_ra_printf (&rl_ra_str, RL_SCM_INDENT_TEMPLATE RL_SCM_ATTR_INT,
			    level * RL_SCM_INDENT_SPACES, "", RL_RARRAY_SIZE, ptrs->ra.data[idx].rarray_size) < 0)
	    return (NULL);
	  else
	    in_comment = !0;
	}
	    
      if (ptrs->ra.data[idx].first_child < 0)
	{
	  /* route saving handler */
	  if ((fdp->rl_type_ext >= 0) && (fdp->rl_type_ext < RL_MAX_TYPES)
	      && rl_conf.io_ext_handlers[fdp->rl_type_ext].save.scm)
	    content = rl_conf.io_ext_handlers[fdp->rl_type_ext].save.scm (idx, ptrs);
	  else if ((fdp->rl_type >= 0) && (fdp->rl_type < RL_MAX_TYPES)
		   && rl_conf.io_handlers[fdp->rl_type].save.scm)
	    content = rl_conf.io_handlers[fdp->rl_type].save.scm (idx, ptrs);
	  else
	    RL_MESSAGE_UNSUPPORTED_NODE_TYPE_ (fdp);    

	  if (content)
	    {
	      if (RL_SCM_NAMED_FIELDS == named_node)
		{
		  if (rl_ra_printf (&rl_ra_str, RL_SCM_INDENT_TEMPLATE RL_SCM_NAMED_FIELD_START "%s" RL_SCM_NAMED_FIELD_END,
				    level * RL_SCM_INDENT_SPACES, "", content, ptrs->ra.data[idx].fd.name) < 0)
		    return (NULL);
		}
	      else
		{
		  if (in_comment)
		    if (rl_ra_printf (&rl_ra_str, RL_SCM_INDENT_TEMPLATE, level * RL_SCM_INDENT_SPACES, "") < 0)
		      return (NULL);
		  if (rl_ra_printf (&rl_ra_str, " %s", content) < 0)
		    return (NULL);
		}
	      RL_FREE (content);
	    }
	}
      else
	{
	  if ((idx != 0) || in_comment)
	    if (rl_ra_printf (&rl_ra_str, RL_SCM_INDENT_TEMPLATE, level * RL_SCM_INDENT_SPACES, "") < 0)
	      return (NULL);
	  if (RL_SCM_NAMED_FIELDS == named_node)
	    if (rl_ra_printf (&rl_ra_str, RL_SCM_NAMED_FIELD_START) < 0)
	      return (NULL);
	  if (RL_SCM_IS_ARRAY == scm_is_array[fdp->rl_type_ext])
	    if (rl_ra_printf (&rl_ra_str, RL_SCM_ARRAY_PREFIX) < 0)
	      return (NULL);
	  if (rl_ra_printf (&rl_ra_str, RL_SCM_COMPAUND_START) < 0)
	    return (NULL);
	}
      
      if (ptrs->ra.data[idx].first_child >= 0)
	idx = ptrs->ra.data[idx].first_child;
      else
	{
	  while ((ptrs->ra.data[idx].next < 0) && (ptrs->ra.data[idx].parent >= 0))
	    {
	      idx = ptrs->ra.data[idx].parent;
	      level = RL_LIMIT_LEVEL (ptrs->ra.data[idx].level);
	      named_node = RL_SCM_UNNAMED_FIELDS;
	      parent = ptrs->ra.data[idx].parent;

	      if (parent >= 0)
		named_node = scm_named_fields[ptrs->ra.data[parent].fd.rl_type];
	      
	      if (rl_ra_printf (&rl_ra_str, RL_SCM_COMPAUND_END) < 0)
		return (NULL);
	      if (RL_SCM_NAMED_FIELDS == named_node)
		if (rl_ra_printf (&rl_ra_str, RL_SCM_NAMED_FIELD_END, ptrs->ra.data[idx].fd.name) < 0)
		  return (NULL);
	    }
	  idx = ptrs->ra.data[idx].next;
	}
    }

  return (rl_ra_str.data);
}

/**
 * RL_NONE type saving handler.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors
 * @return stringified int value
 */
static char *
scm_save_none (int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  return (NULL);
}

/**
 * RL_XXX type saving handler. Make a string from *(XXX_t*)data.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors 
 */
#define SCM_SAVE_TYPE(TYPE) static char * scm_save_ ## TYPE (int idx, rl_ra_rl_ptrdes_t * ptrs) { return (rl_stringify_ ## TYPE (&ptrs->ra.data[idx])); }

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

#define RL_SCM_BITMASK_OR_DELIMITER " "
#define RL_SCM_BITMASK_TEMPLATE "(bit-or %s)"

/**
 * RL_BITMASK type saving handler. Look up type descriptor and save as
 * stringified bitmask value or as integer otherwise.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors 
 * @return stringified enum value
 */
static char *
scm_save_bitmask (int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  char * str = rl_stringify_bitmask (&ptrs->ra.data[idx], RL_SCM_BITMASK_OR_DELIMITER);
  if (str)
    {
      char * str_ = RL_MALLOC ((sizeof (RL_SCM_BITMASK_TEMPLATE) - 1) + (strlen (str) - 2) + 1);
      if (NULL == str_)
	{
	  RL_MESSAGE (RL_LL_FATAL, RL_MESSAGE_OUT_OF_MEMORY);
	  return (NULL);
	}	
      sprintf (str_, RL_SCM_BITMASK_TEMPLATE, str);
      RL_FREE (str);
      str = str_;
    }
  return (str);
}

/**
 * RL_CHAR type saving handler. Stringify char.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors 
 * @return stringified float value
 */
static char *
scm_save_char (int idx, rl_ra_rl_ptrdes_t * ptrs)
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
  return (RL_STRDUP (str));
}

/**
 * Quote string.
 * @param str string pointer
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
  str_ = RL_MALLOC (length + 3);
  if (NULL == str_)
    {
      RL_MESSAGE (RL_LL_FATAL, RL_MESSAGE_OUT_OF_MEMORY);
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
 * RL_CHAR_ARRAY type saving handler. Save char array.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors 
 * @return char array value
 */
static char *
scm_save_char_array (int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  return (scm_quote_string (ptrs->ra.data[idx].data, '"'));
}

/**
 * RL_STRING type saving handler. Save string.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors 
 * @return string value
 */
static char *
scm_save_string (int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  char * str = *(char**)ptrs->ra.data[idx].data;
  if ((NULL == str) || (ptrs->ra.data[idx].ref_idx >= 0))
    return (RL_STRDUP (RL_SCM_FALSE));
  else
    return (scm_quote_string (str, '"'));
}

/**
 * RL_POINTER type saving handler. Save pointer as a string.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors
 * @return stringified pointer
 */
static char *
scm_save_pointer (int idx, rl_ra_rl_ptrdes_t * ptrs)
{ 
  return (RL_STRDUP (RL_SCM_FALSE));
}

static char *
scm_save_empty (int idx, rl_ra_rl_ptrdes_t * ptrs)
{ 
  return (RL_STRDUP (RL_SCM_EMPTY));
}

/**
 * Init IO handlers Table
 */
static void __attribute__((constructor)) rl_init_save_scm (void)
{
  rl_conf.io_handlers[RL_TYPE_NONE].save.scm = scm_save_none;
  rl_conf.io_handlers[RL_TYPE_VOID].save.scm = scm_save_none;
  rl_conf.io_handlers[RL_TYPE_ENUM].save.scm = scm_save_enum;
  rl_conf.io_handlers[RL_TYPE_BITFIELD].save.scm = scm_save_bitfield;
  rl_conf.io_handlers[RL_TYPE_BITMASK].save.scm = scm_save_bitmask;
  rl_conf.io_handlers[RL_TYPE_INT8].save.scm = scm_save_int8;
  rl_conf.io_handlers[RL_TYPE_UINT8].save.scm = scm_save_uint8;
  rl_conf.io_handlers[RL_TYPE_INT16].save.scm = scm_save_int16;
  rl_conf.io_handlers[RL_TYPE_UINT16].save.scm = scm_save_uint16;
  rl_conf.io_handlers[RL_TYPE_INT32].save.scm = scm_save_int32;
  rl_conf.io_handlers[RL_TYPE_UINT32].save.scm = scm_save_uint32;
  rl_conf.io_handlers[RL_TYPE_INT64].save.scm = scm_save_int64;
  rl_conf.io_handlers[RL_TYPE_UINT64].save.scm = scm_save_uint64;
  rl_conf.io_handlers[RL_TYPE_FLOAT].save.scm = scm_save_float;
  rl_conf.io_handlers[RL_TYPE_DOUBLE].save.scm = scm_save_double;
  rl_conf.io_handlers[RL_TYPE_LONG_DOUBLE].save.scm = scm_save_long_double_t;
  rl_conf.io_handlers[RL_TYPE_CHAR].save.scm = scm_save_char;
  rl_conf.io_handlers[RL_TYPE_CHAR_ARRAY].save.scm = scm_save_char_array;
  rl_conf.io_handlers[RL_TYPE_STRING].save.scm = scm_save_string;
  rl_conf.io_handlers[RL_TYPE_STRUCT].save.scm = scm_save_empty;
  rl_conf.io_handlers[RL_TYPE_FUNC].save.scm = scm_save_none;
  rl_conf.io_handlers[RL_TYPE_FUNC_TYPE].save.scm = scm_save_none;
  rl_conf.io_handlers[RL_TYPE_UNION].save.scm = scm_save_empty;
  rl_conf.io_handlers[RL_TYPE_ANON_UNION].save.scm = scm_save_empty;

  rl_conf.io_ext_handlers[RL_TYPE_EXT_ARRAY].save.scm = scm_save_empty;
  rl_conf.io_ext_handlers[RL_TYPE_EXT_RARRAY].save.scm = scm_save_empty;
  rl_conf.io_ext_handlers[RL_TYPE_EXT_POINTER].save.scm = scm_save_pointer;
}
