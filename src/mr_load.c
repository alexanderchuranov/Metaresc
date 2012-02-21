/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <stdio.h>
#include <string.h>

#include <metaresc.h>

#include <mr_load.h>

#ifdef HAVE_CONFIG_H
# include <mr_config.h>
#endif /* HAVE_CONFIG_H */

/**
 * Post load references setting. If node was marked as references
 * it should be substitude with actual pointer. This substition
 * can't be made during structure loading because of forward references.
 * @param mr_load_data structures that holds context of loading
 * @return Status. !0 - ok. 0 - rl nodes indexes colision
 */
static int
mr_set_crossrefs (mr_load_data_t * mr_load_data)
{
  int i;
  int count = mr_load_data->ptrs.ra.size / sizeof (mr_load_data->ptrs.ra.data[0]);
  int max_idx = -1;
  int * table;

  for (i = 0; i < count; ++i)
    if (mr_load_data->ptrs.ra.data[i].idx > max_idx)
      max_idx = mr_load_data->ptrs.ra.data[i].idx;

  if (max_idx < 0)
    return (!0);

  table = (int*)MR_MALLOC (sizeof (int) * (max_idx + 1));
  if (NULL == table)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (0);
    }

  for (i = 0; i <= max_idx; ++i)
    table[i] = -1;
  for (i = 0; i < count; ++i)
    if (mr_load_data->ptrs.ra.data[i].idx >= 0)
      {
	/* checking indexes collision */
	if (table[mr_load_data->ptrs.ra.data[i].idx] >= 0)
	  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_IDS_COLLISION, MR_REF_IDX, mr_load_data->ptrs.ra.data[i].idx);
	table[mr_load_data->ptrs.ra.data[i].idx] = i;
      }

  /* set all cross refernces */
  for (i = 0; i < count; ++i)
    if (mr_load_data->ptrs.ra.data[i].ref_idx >= 0)
      {
	int idx = table[mr_load_data->ptrs.ra.data[i].ref_idx];
	if (idx < 0)
	  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNDEFINED_REF_IDX, MR_REF_IDX, mr_load_data->ptrs.ra.data[i].ref_idx);
	else
	  {
	    void * data;
	    if (mr_load_data->ptrs.ra.data[i].flags & MR_PDF_CONTENT_REFERENCE)
	      data = *(void**)(mr_load_data->ptrs.ra.data[idx].data);
	    else
	      data = mr_load_data->ptrs.ra.data[idx].data;
	    
	    if ((MR_TYPE_EXT_POINTER == mr_load_data->ptrs.ra.data[i].fd.mr_type_ext) ||
		(MR_TYPE_STRING == mr_load_data->ptrs.ra.data[i].fd.mr_type))
	      *(void**)mr_load_data->ptrs.ra.data[i].data = data;
	  }
      }
  MR_FREE (table);
  return (!0);
}

/**
 * Read enum value from string
 * @param data pointer on place to save value
 * @param str string with enum
 * @return A pointer on the rest of parsed string
 */
static char *
mr_get_enum (uint64_t * data, char * str)  
{
  char * name = str;
  int size;
	  
  while (isalnum (*str) || (*str == '_'))
    ++str;
  size = str - name;

  {
    char name_[size + 1];
    memcpy (name_, name, size);
    name_[size] = 0;
    
#ifdef MR_LINEAR_SEARCH
    /**
     * Type descriptor iterator. Finds enum descriptors and value for enum name.
     * @param tdp type descriptor pointer
     * @param args enum symantic value
     * @return Flag that enum was found
     */
    int enum2int (mr_td_t * tdp, void * args)
    {
      int i;
      if (MR_TYPE_ENUM == tdp->mr_type)
	for (i = tdp->fields.size / sizeof (tdp->fields.data[0]) - 1; i >= 0; --i)
	  if (0 == strcmp (tdp->fields.data[i].name, name_))
	    {
	      *data = tdp->fields.data[i].value;
	      return (!0);
	    }
      return (0);
    }

    if (mr_td_foreach (enum2int, name_))
      return (str);
#else /* ! MR_LINEAR_SEARCH */
    if (EXIT_SUCCESS == mr_get_enum_by_name (data, name_))
      return (str);
#endif /* MR_LINEAR_SEARCH */

    MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNKNOWN_ENUM, name);
    return (NULL);
  }
}

/**
 * Read int value from string (may be as ENUM)
 * @param data pointer on place to save int
 * @param str string with int
 * @return A pointer on the rest of parsed string
 */
static char *
mr_get_int (uint64_t * data, char * str)
{
  int offset;
  while (isspace (*str))
    ++str;
  if (isalpha (*str))
    str = mr_get_enum (data, str);
  else if ('0' == *str)
    {
      if ('x' == str[1])
	{
	  if (1 != sscanf (str, "%" SCNx64 "%n", data, &offset))
	    {
	      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_INT, str);
	      return (NULL);
	    }
	}
      else
	{
	  if (1 != sscanf (str, "%" SCNo64 "%n", data, &offset))
	    {
	      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_INT, str);
	      return (NULL);
	    }
	}	
      str += offset;
    }
  else
    {
      if ((1 == sscanf (str, "%" SCNu64 "%n", data, &offset)) || (1 == sscanf (str, "%" SCNd64 "%n", data, &offset)))
	str += offset;
      else 
	{
	  MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_INT, str);
	  return (NULL);
	}
    }
  return (str);
}

/**
 * MR_NONE load handler (dummy)
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read (0 - failure, !0 - success)
 */
static int
mr_load_none (int idx, mr_load_data_t * mr_load_data)
{
  return (!0);
}

/**
 * MR_INTEGER load handler
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read (0 - failure, !0 - success)
 */
static int
mr_load_integer (int idx, mr_load_data_t * mr_load_data)
{
  char * str = mr_load_data->ptrs.ra.data[idx].value;
  uint64_t value;
  
  if (NULL == str)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_INT, mr_load_data->ptrs.ra.data[idx].value);
      return (0);
    }
  str = mr_get_int (&value, str);
  if (str)
    {
      while (isspace (*str))
	++str;
    }
  if ((NULL == str) || (*str != 0))
    {
      if (str)
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_INT, mr_load_data->ptrs.ra.data[idx].value);
      return (0);
    }
  memcpy (mr_load_data->ptrs.ra.data[idx].data, &value, mr_load_data->ptrs.ra.data[idx].fd.size);
  return (!0);
}

/**
 * MR_ENUM load handler
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read (0 - failure, !0 - success)
 */
static int
mr_load_enum (int idx, mr_load_data_t * mr_load_data)
{
  char * str = mr_load_data->ptrs.ra.data[idx].value;
  char * tail;
  mr_td_t * tdp = mr_get_td_by_name (mr_load_data->ptrs.ra.data[idx].fd.type);
  mr_fd_t * fdp;

  if (NULL == str)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_ENUM, mr_load_data->ptrs.ra.data[idx].value);
      return (0);
    }
  if (tdp)
    {
      while (isspace (*str))
	++str;
      tail = &str[strlen (str) - 1];
      while ((tail > str) && isspace (*tail))
	--tail;
      *++tail = 0;
      fdp = mr_get_fd_by_name (tdp, str);
      if (fdp)
	{
	  memcpy (mr_load_data->ptrs.ra.data[idx].data, &fdp->param.enum_value, mr_load_data->ptrs.ra.data[idx].fd.size);
	  return (!0);
	}
    }
  return (mr_load_integer (idx, mr_load_data));
}

/**
 * MR_BITFIELD load handler. Load int from string and save it to
 * bit shifted field.
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read (0 - failure, !0 - success)
 */
static int
mr_load_bitfield (int idx, mr_load_data_t * mr_load_data)
{
  mr_ptrdes_t * ptrdes = &mr_load_data->ptrs.ra.data[idx];
  uint64_t value = 0;
  char * str = mr_get_int (&value, ptrdes->value);

  if ((NULL == str) || (*str != 0))
    {
      if (str)
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_INT, ptrdes->value);
      return (0);
    }

  return (EXIT_SUCCESS == mr_load_bitfield_value (ptrdes, &value));
}

/**
 * MR_BITMASK load handler. Handles logical OR operation.
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read (0 - failure, !0 - success)
 */
static int
mr_load_bitmask (int idx, mr_load_data_t * mr_load_data)
{
  char * str = mr_load_data->ptrs.ra.data[idx].value;
  int64_t value = 0;
  
  if (NULL == str)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_BITMASK, mr_load_data->ptrs.ra.data[idx].value);
      return (0);
    }
  for (;;)
    {
      uint64_t bit;
      str = mr_get_int (&bit, str);
      if (NULL == str)
	return (0);
      value |= bit;
      while (isspace (*str))
	++str;
      if (*str != '|')
	break;
      ++str;
    }
  if (*str != 0)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_BITMASK, mr_load_data->ptrs.ra.data[idx].value);
      return (0);
    }
  memcpy (mr_load_data->ptrs.ra.data[idx].data, &value, mr_load_data->ptrs.ra.data[idx].fd.size);
  return (!0);
}

/**
 * MR_FLOAT, MR_DOUBLE, MR_LONG_DOUBLE load handler.
 */
#define MR_LOAD_FLOAT_TYPE(TYPE, FORMAT, ERROR_ENUM)			\
  static int								\
  mr_load_ ## TYPE (int idx, mr_load_data_t * mr_load_data)		\
  {									\
    char * str = mr_load_data->ptrs.ra.data[idx].value;			\
    int offset;								\
    if (NULL == str)							\
      {									\
	MR_MESSAGE (MR_LL_ERROR, ERROR_ENUM, mr_load_data->ptrs.ra.data[idx].value); \
	return (0);							\
      }									\
    if (1 != sscanf (str, FORMAT "%n", (TYPE*)mr_load_data->ptrs.ra.data[idx].data, &offset)) \
      {									\
	MR_MESSAGE (MR_LL_ERROR, ERROR_ENUM, mr_load_data->ptrs.ra.data[idx].value); \
	return (0);							\
      }									\
    str += offset;							\
    while (isspace (*str))						\
      ++str;								\
    if (*str != 0)							\
      {									\
	MR_MESSAGE (MR_LL_ERROR, ERROR_ENUM, mr_load_data->ptrs.ra.data[idx].value); \
	return (0);							\
      }									\
    return (!0);							\
  }

MR_LOAD_FLOAT_TYPE (float, "%f", MR_MESSAGE_READ_FLOAT)
MR_LOAD_FLOAT_TYPE (double, "%lg", MR_MESSAGE_READ_DOUBLE)
MR_LOAD_FLOAT_TYPE (long_double_t, "%Lg", MR_MESSAGE_READ_LONG_DOUBLE)

/**
 * MR_CHAR load handler. Handles nonprint characters in octal format.
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read (0 - failure, !0 - success)
 */
static int
mr_load_char (int idx, mr_load_data_t * mr_load_data)
{
  char * str = mr_load_data->ptrs.ra.data[idx].value;
  
  if (NULL == str)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_CHAR, str);
      return (0);
    }
  else if ((0 == *str) || (0 == str[1]))
    *(char*)mr_load_data->ptrs.ra.data[idx].data = str[0];
  else if ('\\' != *str)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_CHAR, str);
      return (0);
    }
  else
    {
      int offset;
      int val = 0;
      if (1 != sscanf (str + 1, "%o%n", &val, &offset))
	{
	  MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_CHAR, str);
	  return (0);
	}
      else if (str[offset + 1] != 0)
	{
	  MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_READ_CHAR, str);
	  return (0);
	}
      *(char*)mr_load_data->ptrs.ra.data[idx].data = val;
    }
  return (!0);
}     

/**
 * MR_STRING load handler. Allocate memory for a string.
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read (0 - failure, !0 - success)
 */
static int
mr_load_string (int idx, mr_load_data_t * mr_load_data)
{
  char * str = mr_load_data->ptrs.ra.data[idx].value;
  if ((mr_load_data->ptrs.ra.data[idx].flags & MR_PDF_IS_NULL) || (mr_load_data->ptrs.ra.data[idx].ref_idx >= 0))
    *(char**)mr_load_data->ptrs.ra.data[idx].data = NULL;
  else
    *(char**)mr_load_data->ptrs.ra.data[idx].data = str ? MR_STRDUP (str) : NULL;
  return (!0);
}

/**
 * MR_CHAR_ARRAY load handler.
 * Save string in place (truncate string if needed).
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read (0 - failure, !0 - success)
 */
static int
mr_load_char_array (int idx, mr_load_data_t * mr_load_data)
{
  char * str = mr_load_data->ptrs.ra.data[idx].value;
  int max_size = mr_load_data->ptrs.ra.data[idx].fd.param.array_param.count * mr_load_data->ptrs.ra.data[idx].fd.size;
  if (str)
    {
      int str_len = strlen (str);
      if ((0 == strcmp (mr_load_data->ptrs.ra.data[idx].fd.type, "string_t")) &&
	  (mr_load_data->ptrs.ra.data[idx].parent >= 0) &&
	  (MR_TYPE_EXT_POINTER == mr_load_data->ptrs.ra.data[mr_load_data->ptrs.ra.data[idx].parent].fd.mr_type_ext))
	{
	  void * data = MR_REALLOC (mr_load_data->ptrs.ra.data[idx].data, str_len + 1);
	  mr_load_data->ptrs.ra.data[idx].data = data;
	  *(void**)mr_load_data->ptrs.ra.data[mr_load_data->ptrs.ra.data[idx].parent].data = data;
	  if (NULL == data)
	    {
	      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	      return (0);
	    }
	}
      else if (str_len >= max_size)
	{
	  str[max_size - 1] = 0;
	  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_STRING_TRUNCATED);
	}
      strcpy (mr_load_data->ptrs.ra.data[idx].data, str);
    }
  else
    *(char*)mr_load_data->ptrs.ra.data[idx].data = 0;
  return (!0);
}

/**
 * MR_STRUCT load handler.
 * Save content of subnodes to structure fileds.
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @param tdp type descriptor
 * @return Status of read (0 - failure, !0 - success)
 */
static int
mr_load_struct_inner (int idx, mr_load_data_t * mr_load_data, mr_td_t * tdp)
{
  char * data = mr_load_data->ptrs.ra.data[idx].data;
  int first_child = mr_load_data->ptrs.ra.data[idx].first_child;
  mr_fd_t * fdp;
  
  /* get pointer on structure descriptor */
  if (NULL == tdp)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_NO_TYPE_DESCRIPTOR, mr_load_data->ptrs.ra.data[idx].fd.type);
      return (0);
    }

  /* for C init style we can get union descriptor only from type cast */
  if ((0 == strcmp (tdp->type, "mr_ptr_t")) && (first_child >= 0) &&
      mr_load_data->ptrs.ra.data[first_child].fd.type && (NULL == mr_load_data->ptrs.ra.data[first_child].fd.name))
    {
      mr_load_data->ptrs.ra.data[first_child].fd.name = mr_load_data->ptrs.ra.data[first_child].fd.type;
      mr_load_data->ptrs.ra.data[first_child].fd.type = NULL;
    }

  fdp = tdp->fields.data;
  
  /* loop on all subnodes */
  for (idx = first_child; idx >= 0; idx = mr_load_data->ptrs.ra.data[idx].next)
    {
      if (mr_load_data->ptrs.ra.data[idx].fd.name)
	fdp = mr_get_fd_by_name (tdp, mr_load_data->ptrs.ra.data[idx].fd.name);
      if (NULL == fdp)
	{
	  MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNKNOWN_SUBNODE, tdp->type, mr_load_data->ptrs.ra.data[idx].fd.name);
	  return (0);
	}
      
      /* recursively load subnode */
      if (!mr_load (&data[fdp->offset], fdp, idx, mr_load_data))
	return (0);
      
      ++fdp;
      if (MR_TYPE_TRAILING_RECORD == fdp->mr_type)
	fdp = NULL;
    }
  return (!0);
}

/**
 * MR_STRUCT load handler. Wrapper over mr_load_struct_inner.
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read (0 - failure, !0 - success)
 */
static int
mr_load_struct (int idx, mr_load_data_t * mr_load_data)
{
  return (mr_load_struct_inner (idx, mr_load_data, mr_get_td_by_name (mr_load_data->ptrs.ra.data[idx].fd.type)));
}

/**
 * MR_ARRAY load handler.
 * Save content of subnodes to array elements.
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read (0 - failure, !0 - success)
 */
static int
mr_load_array (int idx, mr_load_data_t * mr_load_data)
{
  char * data = mr_load_data->ptrs.ra.data[idx].data;
  mr_fd_t fd_ = mr_load_data->ptrs.ra.data[idx].fd;
  int row_count = fd_.param.array_param.row_count;
  int count = fd_.param.array_param.count;
  int i = 0;
  
  if (1 == fd_.param.array_param.row_count)
    fd_.mr_type_ext = MR_TYPE_EXT_NONE; /* prepare copy of filed descriptor for array elements loading */
  else
    {
      fd_.param.array_param.count = row_count;
      fd_.param.array_param.row_count = 1;
    }
  
  /* loop on subnodes */
  for (idx = mr_load_data->ptrs.ra.data[idx].first_child; idx >= 0; idx = mr_load_data->ptrs.ra.data[idx].next)
    {
      /* check if array index is in range */
      if ((i < 0) || (i >= count))
	{
	  MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_RANGE_CHECK, fd_.name);
	  return (0);
	}
      /* load recursively */
      if (!mr_load (&data[i * fd_.size], &fd_, idx, mr_load_data))
	return (0);
      i += row_count;
    }
  return (!0);
}

/**
 * MR_RARRAY load handler.
 * Save content of subnodes to resizeable array elements
 * (allocate/reallocate required memory).
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read (0 - failure, !0 - success)
 */
static int
mr_load_rarray (int idx, mr_load_data_t * mr_load_data)
{
  mr_rarray_t * ra = mr_load_data->ptrs.ra.data[idx].data;
  int data_idx, idx_;
  int count = 0;

  memset (ra, 0, sizeof (*ra));
  
#define MR_LOAD_RARRAY_ACTION(TD) ({					\
      int __status = 0;							\
      mr_fd_t * fdp = mr_get_fd_by_name (&TD, "data");			\
      if (NULL == fdp)							\
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_RARRAY_FAILED);		\
      else								\
	{								\
	  fdp->type = mr_load_data->ptrs.ra.data[idx].fd.type;		\
	  fdp->size = mr_load_data->ptrs.ra.data[idx].fd.size;		\
	  __status = mr_load_struct_inner (idx, mr_load_data, &TD);	\
	}								\
      __status;								\
    })
  
  if (!MR_LOAD_RARRAY (MR_LOAD_RARRAY_ACTION))
    return (0);

  for (data_idx = mr_load_data->ptrs.ra.data[idx].first_child; data_idx >= 0; data_idx = mr_load_data->ptrs.ra.data[data_idx].next)
    if (0 == strcmp ("data", mr_load_data->ptrs.ra.data[data_idx].fd.name))
      break;

  if (data_idx < 0)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_RARRAY_FAILED);
      return (0);
    }

  /* initialize resizeable array */
  for (idx_ = mr_load_data->ptrs.ra.data[data_idx].first_child; idx_ >= 0; idx_ = mr_load_data->ptrs.ra.data[idx_].next) /* loop on subnodes */
    ++count;
  
  if ((mr_load_data->ptrs.ra.data[data_idx].ref_idx < 0) && (count > 0))
    {
      int i = 0;
      mr_fd_t fd_ = mr_load_data->ptrs.ra.data[idx].fd;

      fd_.mr_type_ext = MR_TYPE_EXT_NONE;

      ra->size = count * mr_load_data->ptrs.ra.data[idx].fd.size;
      ra->data = MR_MALLOC (ra->size);
      if (NULL == ra->data)
	{
	  ra->alloc_size = ra->size = 0;
	  MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	  return (0);
	}
      memset (ra->data, 0, ra->size);
      ra->alloc_size = ra->size;
      
      for (idx_ = mr_load_data->ptrs.ra.data[data_idx].first_child; idx_ >= 0; idx_ = mr_load_data->ptrs.ra.data[idx_].next) /* loop on subnodes */
	{
	  if (!mr_load (((char*)ra->data) + i * fd_.size, &fd_, idx_, mr_load_data))
	    return (0);
	  if (++i > count)
	    {
	      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_RANGE_CHECK, fd_.name);
	      break;
	    }
	}
    }
  return (!0);
}

/**
 * MR_TYPE_EXT_POINTER load handler. Initiated as postponed call thru mr_load_pointer via stack.
 * Loads element into newly allocate memory.
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read (0 - failure, !0 - success)
 */
static int
mr_load_pointer_postponed (int idx, mr_load_data_t * mr_load_data)
{
  void ** data = mr_load_data->ptrs.ra.data[idx].data;
  mr_fd_t fd_ = mr_load_data->ptrs.ra.data[idx].fd;
  fd_.mr_type_ext = MR_TYPE_EXT_NONE;
  /* allocate memory */
  *data = MR_MALLOC (fd_.size);
  if (NULL == *data)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (0);
    }
  memset (*data, 0, fd_.size);
  /* load recursively */
  return (mr_load (*data, &fd_, mr_load_data->ptrs.ra.data[idx].first_child, mr_load_data));
}

/**
 * MR_POINTER_STRUCT load handler. Schedule element postponed loading via stack.
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read (0 - failure, !0 - success)
 */
static int
mr_load_pointer (int idx, mr_load_data_t * mr_load_data)
{
  void ** data = mr_load_data->ptrs.ra.data[idx].data;
  /* default initializer */
  *data = NULL;
  if (mr_load_data->ptrs.ra.data[idx].ref_idx >= 0)
    return (!0);
  if (mr_load_data->ptrs.ra.data[idx].first_child < 0)
    return (!0);
  /* check whether pointer should have offsprings or not */
  if ((MR_TYPE_NONE != mr_load_data->ptrs.ra.data[idx].fd.mr_type) && (MR_TYPE_VOID != mr_load_data->ptrs.ra.data[idx].fd.mr_type))
    {
      int * idx_ = mr_rarray_append ((void*)&mr_load_data->mr_ra_idx, sizeof (mr_load_data->mr_ra_idx.data[0]));
      if (NULL == idx_)
	return (0);
      *idx_ = idx;
      return (!0);
    }

  return (!0);
}

/**
 * MR_ANON_UNION load handler.
 * Load anonymous union
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read (0 - failure, !0 - success)
 */
static int
mr_load_anon_union (int idx, mr_load_data_t * mr_load_data)
{
  /*
    Anonimous unions in C init style saved as named field folowed by union itself. Named field has type of zero length static string and must be inited by empty string. Here is an example.
    .anon_union_79 = "", {
    .union_float = 3.1415927410,
    },
  */
  int next = mr_load_data->ptrs.ra.data[idx].next;
  if ((mr_load_data->ptrs.ra.data[idx].first_child < 0) && /* if node has no childs, then it is C init style anonumous union */
      mr_load_data->ptrs.ra.data[idx].value && (0 == mr_load_data->ptrs.ra.data[idx].value[0]) && /* content must be an empty string */
      (next >= 0) && (NULL == mr_load_data->ptrs.ra.data[next].fd.name)) /* there should be a next node without name */
    {
      if (mr_load_data->ptrs.ra.data[idx].fd.name) /* sainity check - this field can't be NULL */
	mr_load_data->ptrs.ra.data[next].fd.name = MR_STRDUP (mr_load_data->ptrs.ra.data[idx].fd.name);
      return (!0); /* now next node has a name and will be loaded by top level procedure */
    }
  return (mr_load_struct (idx, mr_load_data));
}

/**
 * Cleanup helper. Deallocates all dynamically allocated resources.
 * @param ptrs resizeable array with pointers descriptors
 * @return Status of read (0 - failure, !0 - success)
 */
int
mr_free_ptrs (mr_ra_mr_ptrdes_t ptrs)
{
  if (ptrs.ra.data)
    {
      int count = ptrs.ra.size / sizeof (ptrs.ra.data[0]);
      int i;
      for (i = 0; i < count; ++i)
	{
	  if (ptrs.ra.data[i].value)
	    MR_FREE (ptrs.ra.data[i].value);
	  ptrs.ra.data[i].value = NULL;
	  if (ptrs.ra.data[i].fd.type)
	    MR_FREE (ptrs.ra.data[i].fd.type);
	  ptrs.ra.data[i].fd.type = NULL;
	  if (ptrs.ra.data[i].fd.name)
	    MR_FREE (ptrs.ra.data[i].fd.name);
	  ptrs.ra.data[i].fd.name = NULL;
	}
      MR_FREE (ptrs.ra.data);
      ptrs.ra.data = NULL;
      ptrs.ra.size = ptrs.ra.alloc_size = 0;
    }
  return (EXIT_SUCCESS);
}

/**
 * Public function. Load router. Load any object from internal representation graph.
 * @param data pointer on place to save data
 * @param fdp filed descriptor
 * @param idx node index
 * @param mr_load_data structures that holds context of loading
 * @return Status of read (0 - failure, !0 - success)
 */
int
mr_load (void * data, mr_fd_t * fdp, int idx, mr_load_data_t * mr_load_data)
{
  int status = 0;

  if (0 == idx)
    {
      mr_load_data->mr_ra_idx.data = NULL;
      mr_load_data->mr_ra_idx.size = 0;
      mr_load_data->mr_ra_idx.alloc_size = 0;
    }
  
  if ((idx < 0) || (idx >= mr_load_data->ptrs.ra.size / sizeof (mr_load_data->ptrs.ra.data[0])))
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_SAVE_IDX_RANGE_CHECK);
      return (0);
    }
  
  mr_load_data->ptrs.ra.data[idx].data = data;
  if (mr_load_data->ptrs.ra.data[idx].fd.name && fdp->name)
    if (strcmp (fdp->name, mr_load_data->ptrs.ra.data[idx].fd.name))
      {
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_NODE_NAME_MISSMATCH, fdp->name, mr_load_data->ptrs.ra.data[idx].fd.name);
	return (0);
      }

  if (mr_load_data->ptrs.ra.data[idx].fd.type && fdp->type)
    if (strcmp (fdp->type, mr_load_data->ptrs.ra.data[idx].fd.type))
      {
	MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_NODE_TYPE_MISSMATCH, fdp->type, mr_load_data->ptrs.ra.data[idx].fd.type);
	return (0);
      }
  
  if ((NULL == mr_load_data->ptrs.ra.data[idx].fd.name) && (fdp->name))
    mr_load_data->ptrs.ra.data[idx].fd.name = MR_STRDUP (fdp->name);
  if ((NULL == mr_load_data->ptrs.ra.data[idx].fd.type) && (fdp->type))
    mr_load_data->ptrs.ra.data[idx].fd.type = MR_STRDUP (fdp->type);
  mr_load_data->ptrs.ra.data[idx].fd.size = fdp->size;
  mr_load_data->ptrs.ra.data[idx].fd.mr_type = fdp->mr_type;
  mr_load_data->ptrs.ra.data[idx].fd.mr_type_aux = fdp->mr_type_aux;
  mr_load_data->ptrs.ra.data[idx].fd.mr_type_ext = fdp->mr_type_ext;
  mr_load_data->ptrs.ra.data[idx].fd.param = fdp->param;
  
  /* route loading */
  if ((fdp->mr_type_ext >= 0) && (fdp->mr_type_ext < MR_MAX_TYPES)
      && mr_conf.io_ext_handlers[fdp->mr_type_ext].load.rl)
    status = mr_conf.io_ext_handlers[fdp->mr_type_ext].load.rl (idx, mr_load_data);
  else if ((fdp->mr_type >= 0) && (fdp->mr_type < MR_MAX_TYPES)
	   && mr_conf.io_handlers[fdp->mr_type].load.rl)
    status = mr_conf.io_handlers[fdp->mr_type].load.rl (idx, mr_load_data);
  else
    MR_MESSAGE_UNSUPPORTED_NODE_TYPE_ (fdp);    

  /* set cross references at the upper level */
  if (0 == idx)
    {
      while (mr_load_data->mr_ra_idx.size > 0)
	{
	  mr_load_data->mr_ra_idx.size -= sizeof (mr_load_data->mr_ra_idx.data[0]);
	  mr_load_pointer_postponed (mr_load_data->mr_ra_idx.data[mr_load_data->mr_ra_idx.size / sizeof (mr_load_data->mr_ra_idx.data[0])], mr_load_data);
	}
      if (status)
	status = mr_set_crossrefs (mr_load_data);
      if (mr_load_data->mr_ra_idx.data)
	{
	  MR_FREE (mr_load_data->mr_ra_idx.data);
	  mr_load_data->mr_ra_idx.data = NULL;
	  mr_load_data->mr_ra_idx.size = 0;
	  mr_load_data->mr_ra_idx.alloc_size = 0;
	}
    }
  
  return (status);
}     

/**
 * Init IO handlers Table
 */
static void __attribute__((constructor)) mr_init_load_rl (void)
{
  mr_conf.io_handlers[MR_TYPE_NONE].load.rl = mr_load_none;
  mr_conf.io_handlers[MR_TYPE_VOID].load.rl = mr_load_none;
  mr_conf.io_handlers[MR_TYPE_ENUM].load.rl = mr_load_enum;
  mr_conf.io_handlers[MR_TYPE_BITFIELD].load.rl = mr_load_bitfield;
  mr_conf.io_handlers[MR_TYPE_BITMASK].load.rl = mr_load_bitmask;
  mr_conf.io_handlers[MR_TYPE_INT8].load.rl = mr_load_integer;
  mr_conf.io_handlers[MR_TYPE_UINT8].load.rl = mr_load_integer;
  mr_conf.io_handlers[MR_TYPE_INT16].load.rl = mr_load_integer;
  mr_conf.io_handlers[MR_TYPE_UINT16].load.rl = mr_load_integer;
  mr_conf.io_handlers[MR_TYPE_INT32].load.rl = mr_load_integer;
  mr_conf.io_handlers[MR_TYPE_UINT32].load.rl = mr_load_integer;
  mr_conf.io_handlers[MR_TYPE_INT64].load.rl = mr_load_integer;
  mr_conf.io_handlers[MR_TYPE_UINT64].load.rl = mr_load_integer;
  mr_conf.io_handlers[MR_TYPE_FLOAT].load.rl = mr_load_float;
  mr_conf.io_handlers[MR_TYPE_DOUBLE].load.rl = mr_load_double;
  mr_conf.io_handlers[MR_TYPE_LONG_DOUBLE].load.rl = mr_load_long_double_t;
  mr_conf.io_handlers[MR_TYPE_CHAR].load.rl = mr_load_char;
  mr_conf.io_handlers[MR_TYPE_CHAR_ARRAY].load.rl = mr_load_char_array;
  mr_conf.io_handlers[MR_TYPE_STRING].load.rl = mr_load_string;
  mr_conf.io_handlers[MR_TYPE_STRUCT].load.rl = mr_load_struct;
  mr_conf.io_handlers[MR_TYPE_FUNC].load.rl = mr_load_none;
  mr_conf.io_handlers[MR_TYPE_FUNC_TYPE].load.rl = mr_load_none;
  mr_conf.io_handlers[MR_TYPE_UNION].load.rl = mr_load_struct;
  mr_conf.io_handlers[MR_TYPE_ANON_UNION].load.rl = mr_load_anon_union;

  mr_conf.io_ext_handlers[MR_TYPE_EXT_ARRAY].load.rl = mr_load_array;
  mr_conf.io_ext_handlers[MR_TYPE_EXT_RARRAY].load.rl = mr_load_rarray;
  mr_conf.io_ext_handlers[MR_TYPE_EXT_POINTER].load.rl = mr_load_pointer;
};
