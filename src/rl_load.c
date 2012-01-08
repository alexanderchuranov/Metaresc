/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

#include <stdio.h>
#include <string.h>

#include <reslib.h>

#include <rl_load.h>

#ifdef HAVE_CONFIG_H
# include <rlconfig.h>
#endif /* HAVE_CONFIG_H */

/**
 * Post load references setting. If node was marked as references
 * it should be substitude with actual pointer. This substition
 * can't be made during structure loading because of forward references.
 * @param ptrs list of pointers descriptors of loaded stuctures
 * @return Status. !0 - ok. 0 - rl nodes indexes colision
 */
static int
rl_set_crossrefs (rl_load_data_t * rl_load_data)
{
  int i;
  int count = rl_load_data->ptrs.ra.size / sizeof (rl_load_data->ptrs.ra.data[0]);
  int max_idx = -1;
  int * table;

  for (i = 0; i < count; ++i)
    if (rl_load_data->ptrs.ra.data[i].idx > max_idx)
      max_idx = rl_load_data->ptrs.ra.data[i].idx;

  if (max_idx < 0)
    return (!0);

  table = (int*)RL_MALLOC (sizeof (int) * (max_idx + 1));
  if (NULL == table)
    {
      RL_MESSAGE (RL_LL_FATAL, RL_MESSAGE_OUT_OF_MEMORY);
      return (0);
    }

  for (i = 0; i <= max_idx; ++i)
    table[i] = -1;
  for (i = 0; i < count; ++i)
    if (rl_load_data->ptrs.ra.data[i].idx >= 0)
      {
	/* checking indexes collision */
	if (table[rl_load_data->ptrs.ra.data[i].idx] >= 0)
	  RL_MESSAGE (RL_LL_WARN, RL_MESSAGE_IDS_COLLISION, RL_REF_IDX, rl_load_data->ptrs.ra.data[i].idx);
	table[rl_load_data->ptrs.ra.data[i].idx] = i;
      }

  /* set all cross refernces */
  for (i = 0; i < count; ++i)
    if (rl_load_data->ptrs.ra.data[i].ref_idx >= 0)
      {
	int idx = table[rl_load_data->ptrs.ra.data[i].ref_idx];
	if (idx < 0)
	  RL_MESSAGE (RL_LL_WARN, RL_MESSAGE_UNDEFINED_REF_IDX, RL_REF_IDX, rl_load_data->ptrs.ra.data[i].ref_idx);
	else
	  {
	    void * data;
	    if (rl_load_data->ptrs.ra.data[i].flags & RL_PDF_CONTENT_REFERENCE)
	      data = *(void**)(rl_load_data->ptrs.ra.data[idx].data);
	    else
	      data = rl_load_data->ptrs.ra.data[idx].data;
	    
	    if ((RL_TYPE_EXT_POINTER == rl_load_data->ptrs.ra.data[i].fd.rl_type_ext) ||
		(RL_TYPE_STRING == rl_load_data->ptrs.ra.data[i].fd.rl_type))
	      *(void**)rl_load_data->ptrs.ra.data[i].data = data;
	  }
      }
  RL_FREE (table);
  return (!0);
}

static char *
rl_get_enum (uint64_t * data, char * str)  
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
    
#ifdef RL_LINEAR_SEARCH
    /**
     * Type descriptor iterator. Finds enum descriptors and value for enum name.
     * @param tdp type descriptor pointer
     * @param args enum symantic value
     * @return Flag that enum was found
     */
    int enum2int (rl_td_t * tdp, void * args)
    {
      int i;
      if (RL_TYPE_ENUM == tdp->rl_type)
	for (i = tdp->fields.size / sizeof (tdp->fields.data[0]) - 1; i >= 0; --i)
	  if (0 == strcmp (tdp->fields.data[i].name, name_))
	    {
	      *data = tdp->fields.data[i].value;
	      return (!0);
	    }
      return (0);
    }

    if (rl_td_foreach (enum2int, name_))
      return (str);
#else /* ! RL_LINEAR_SEARCH */
    if (EXIT_SUCCESS == rl_get_enum_by_name (data, name_))
      return (str);
#endif /* RL_LINEAR_SEARCH */

    RL_MESSAGE (RL_LL_WARN, RL_MESSAGE_UNKNOWN_ENUM, name);
    return (NULL);
  }
}

/**
 * Read int value from string (may be as ENUM)
 * @param data pointer on place to save int
 * @param str string with int
 * @return A pointer rest of parsed string
 */
static char *
rl_get_int (uint64_t * data, char * str)
{
  int offset;
  while (isspace (*str))
    ++str;
  if (isalpha (*str))
    str = rl_get_enum (data, str);
  else if ('0' == *str)
    {
      if ('x' == str[1])
	{
	  if (1 != sscanf (str, "%" SCNx64 "%n", data, &offset))
	    {
	      RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_READ_INT, str);
	      return (NULL);
	    }
	}
      else
	{
	  if (1 != sscanf (str, "%" SCNo64 "%n", data, &offset))
	    {
	      RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_READ_INT, str);
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
	  RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_READ_INT, str);
	  return (NULL);
	}
    }
  return (str);
}

/**
 * RL_NONE load handler (dummy)
 * @param idx node index
 * @param ptrs pointers descriptors resizeable array
 * @return Status of read (0 - failure, !0 - success)
 */
static int
rl_load_none (int idx, rl_load_data_t * rl_load_data)
{
  return (!0);
}

/**
 * RL_INTEGER load handler
 * @param idx node index
 * @param ptrs pointers descriptors resizeable array
 * @return Status of read (0 - failure, !0 - success)
 */
static int
rl_load_integer (int idx, rl_load_data_t * rl_load_data)
{
  char * str = rl_load_data->ptrs.ra.data[idx].value;
  uint64_t value;
  
  if (NULL == str)
    {
      RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_READ_INT, rl_load_data->ptrs.ra.data[idx].value);
      return (0);
    }
  str = rl_get_int (&value, str);
  if (str)
    {
      while (isspace (*str))
	++str;
    }
  if ((NULL == str) || (*str != 0))
    {
      if (str)
	RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_READ_INT, rl_load_data->ptrs.ra.data[idx].value);
      return (0);
    }
  memcpy (rl_load_data->ptrs.ra.data[idx].data, &value, rl_load_data->ptrs.ra.data[idx].fd.size);
  return (!0);
}

/**
 * RL_ENUM load handler
 * @param idx node index
 * @param ptrs pointers descriptors resizeable array
 * @return Status of read (0 - failure, !0 - success)
 */
static int
rl_load_enum (int idx, rl_load_data_t * rl_load_data)
{
  char * str = rl_load_data->ptrs.ra.data[idx].value;
  char * tail;
  rl_td_t * tdp = rl_get_td_by_name (rl_load_data->ptrs.ra.data[idx].fd.type);
  rl_fd_t * fdp;

  if (NULL == str)
    {
      RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_READ_ENUM, rl_load_data->ptrs.ra.data[idx].value);
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
      fdp = rl_get_fd_by_name (tdp, str);
      if (fdp)
	{
	  memcpy (rl_load_data->ptrs.ra.data[idx].data, &fdp->param.enum_value, rl_load_data->ptrs.ra.data[idx].fd.size);
	  return (!0);
	}
    }
  return (rl_load_integer (idx, rl_load_data));
}

/**
 * RL_BITFIELD load handler. Load int from string and save it to
 * bit shifted field.
 * @param idx node index
 * @param ptrs pointers descriptors resizeable array
 * @return Status of read (0 - failure, !0 - success)
 */
static int
rl_load_bitfield (int idx, rl_load_data_t * rl_load_data)
{
  rl_ptrdes_t * ptrdes = &rl_load_data->ptrs.ra.data[idx];
  uint64_t value = 0;
  char * str = rl_get_int (&value, ptrdes->value);

  if ((NULL == str) || (*str != 0))
    {
      if (str)
	RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_READ_INT, ptrdes->value);
      return (0);
    }

  return (EXIT_SUCCESS == rl_load_bitfield_value (ptrdes, &value));
}

/**
 * RL_BITMASK load handler. Handles logical OR operation.
 * @param idx node index
 * @param ptrs pointers descriptors resizeable array
 * @return Status of read (0 - failure, !0 - success)
 */
static int
rl_load_bitmask (int idx, rl_load_data_t * rl_load_data)
{
  char * str = rl_load_data->ptrs.ra.data[idx].value;
  int64_t value = 0;
  
  if (NULL == str)
    {
      RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_READ_BITMASK, rl_load_data->ptrs.ra.data[idx].value);
      return (0);
    }
  for (;;)
    {
      uint64_t bit;
      str = rl_get_int (&bit, str);
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
      RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_READ_BITMASK, rl_load_data->ptrs.ra.data[idx].value);
      return (0);
    }
  memcpy (rl_load_data->ptrs.ra.data[idx].data, &value, rl_load_data->ptrs.ra.data[idx].fd.size);
  return (!0);
}

/**
 * RL_FLOAT, RL_DOUBLE, RL_LONG_DOUBLE load handler.
 * @param idx node index
 * @param ptrs pointers descriptors resizeable array
 * @return Status of read (0 - failure, !0 - success)
 */
#define RL_LOAD_FLOAT_TYPE(TYPE, FORMAT, ERROR_ENUM)			\
  static int								\
  rl_load_ ## TYPE (int idx, rl_load_data_t * rl_load_data)		\
  {									\
    char * str = rl_load_data->ptrs.ra.data[idx].value;			\
    int offset;								\
    if (NULL == str)							\
      {									\
	RL_MESSAGE (RL_LL_ERROR, ERROR_ENUM, rl_load_data->ptrs.ra.data[idx].value); \
	return (0);							\
      }									\
    if (1 != sscanf (str, FORMAT "%n", (TYPE*)rl_load_data->ptrs.ra.data[idx].data, &offset)) \
      {									\
	RL_MESSAGE (RL_LL_ERROR, ERROR_ENUM, rl_load_data->ptrs.ra.data[idx].value); \
	return (0);							\
      }									\
    str += offset;							\
    while (isspace (*str))						\
      ++str;								\
    if (*str != 0)							\
      {									\
	RL_MESSAGE (RL_LL_ERROR, ERROR_ENUM, rl_load_data->ptrs.ra.data[idx].value); \
	return (0);							\
      }									\
    return (!0);							\
  }

RL_LOAD_FLOAT_TYPE (float, "%f", RL_MESSAGE_READ_FLOAT)
RL_LOAD_FLOAT_TYPE (double, "%lg", RL_MESSAGE_READ_DOUBLE)
RL_LOAD_FLOAT_TYPE (long_double_t, "%Lg", RL_MESSAGE_READ_LONG_DOUBLE)

/**
 * RL_CHAR load handler. Handles nonprint characters in octal format.
 * @param idx node index
 * @param ptrs pointers descriptors resizeable array
 * @return Status of read (0 - failure, !0 - success)
 */
static int
rl_load_char (int idx, rl_load_data_t * rl_load_data)
{
  char * str = rl_load_data->ptrs.ra.data[idx].value;
  int status = !0;
  
  if (NULL == str)
    {
      RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_READ_CHAR, str);
      status = 0;
    }
  if (*str != '\\')
    {
      if (strlen (str) != 1)
	{
	  RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_READ_CHAR, str);
	  status = 0;
	}
      *(char*)rl_load_data->ptrs.ra.data[idx].data = str[0];
    }
  else 
    {
      int offset;
      int val = 0;
      if (1 != sscanf (str + 1, "%o%n", &val, &offset))
	{
	  RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_READ_CHAR, str);
	  status = 0;
	}
      else if (str[offset + 1] != 0)
	{
	  RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_READ_CHAR, str);
	  status = 0;
	}
      *(char*)rl_load_data->ptrs.ra.data[idx].data = val;
    }
  return (status);
}     

/**
 * RL_STRING load handler. Allocate memory for a string.
 * @param idx node index
 * @param ptrs pointers descriptors resizeable array
 * @return Status of read (0 - failure, !0 - success)
 */
static int
rl_load_string (int idx, rl_load_data_t * rl_load_data)
{
  char * str = rl_load_data->ptrs.ra.data[idx].value;
  if ((rl_load_data->ptrs.ra.data[idx].flags & RL_PDF_IS_NULL) || (rl_load_data->ptrs.ra.data[idx].ref_idx >= 0))
    *(char**)rl_load_data->ptrs.ra.data[idx].data = NULL;
  else
    *(char**)rl_load_data->ptrs.ra.data[idx].data = str ? RL_STRDUP (str) : NULL;
  return (!0);
}

/**
 * RL_CHAR_ARRAY load handler.
 * Save string in place (truncate string if needed).
 * @param idx node index
 * @param ptrs pointers descriptors resizeable array
 * @return Status of read (0 - failure, !0 - success)
 */
static int
rl_load_char_array (int idx, rl_load_data_t * rl_load_data)
{
  char * str = rl_load_data->ptrs.ra.data[idx].value;
  int max_size = rl_load_data->ptrs.ra.data[idx].fd.param.array_param.count * rl_load_data->ptrs.ra.data[idx].fd.size;
  if (str)
    {
      int str_len = strlen (str);
      if ((0 == strcmp (rl_load_data->ptrs.ra.data[idx].fd.type, "string_t")) &&
	  (rl_load_data->ptrs.ra.data[idx].parent >= 0) &&
	  (RL_TYPE_EXT_POINTER == rl_load_data->ptrs.ra.data[rl_load_data->ptrs.ra.data[idx].parent].fd.rl_type_ext))
	{
	  void * data = RL_REALLOC (rl_load_data->ptrs.ra.data[idx].data, str_len + 1);
	  rl_load_data->ptrs.ra.data[idx].data = data;
	  *(void**)rl_load_data->ptrs.ra.data[rl_load_data->ptrs.ra.data[idx].parent].data = data;
	  if (NULL == data)
	    {
	      RL_MESSAGE (RL_LL_FATAL, RL_MESSAGE_OUT_OF_MEMORY);
	      return (0);
	    }
	}
      else if (str_len >= max_size)
	{
	  str[max_size - 1] = 0;
	  RL_MESSAGE (RL_LL_WARN, RL_MESSAGE_STRING_TRUNCATED);
	}
      strcpy (rl_load_data->ptrs.ra.data[idx].data, str);
    }
  else
    *(char*)rl_load_data->ptrs.ra.data[idx].data = 0;
  return (!0);
}

static int
rl_load_struct_inner (int idx, rl_load_data_t * rl_load_data, rl_td_t * tdp)
{
  char * data = rl_load_data->ptrs.ra.data[idx].data;
  int first_child = rl_load_data->ptrs.ra.data[idx].first_child;
  rl_fd_t * fdp;
  
  /* get pointer on structure descriptor */
  if (NULL == tdp)
    {
      RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_NO_TYPE_DESCRIPTOR, rl_load_data->ptrs.ra.data[idx].fd.type);
      return (0);
    }

  if ((0 == strcmp (tdp->type, "rl_ptr_t")) && (first_child >= 0) && rl_load_data->ptrs.ra.data[first_child].fd.type)
    fdp = rl_get_fd_by_name (tdp, rl_load_data->ptrs.ra.data[first_child].fd.type);
  else
    fdp = tdp->fields.data;
  
  /* loop on all subnodes */
  for (idx = first_child; idx >= 0; idx = rl_load_data->ptrs.ra.data[idx].next)
    {
      if (rl_load_data->ptrs.ra.data[idx].fd.name)
	fdp = rl_get_fd_by_name (tdp, rl_load_data->ptrs.ra.data[idx].fd.name);
      if (NULL == fdp)
	{
	  RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_UNKNOWN_SUBNODE, rl_load_data->ptrs.ra.data[idx].fd.type, rl_load_data->ptrs.ra.data[idx].fd.name);
	  return (0);
	}
      
      /* recursively load subnode */
      if (!rl_load (&data[fdp->offset], fdp, idx, rl_load_data))
	return (0);
      
      ++fdp;
      if (RL_TYPE_TRAILING_RECORD == fdp->rl_type)
	fdp = NULL;
    }
  return (!0);
}

/**
 * RL_STRUCT load handler.
 * Save content of subnodes to struct fileds.
 * Save string in place (truncate string if needed).
 * @param idx node index
 * @param ptrs pointers descriptors resizeable array
 * @return Status of read (0 - failure, !0 - success)
 */
static int
rl_load_struct (int idx, rl_load_data_t * rl_load_data)
{
  return (rl_load_struct_inner (idx, rl_load_data, rl_get_td_by_name (rl_load_data->ptrs.ra.data[idx].fd.type)));
}

/**
 * RL_ARRAY load handler.
 * Save content of subnodes to array elements.
 * @param idx node index
 * @param ptrs pointers descriptors resizeable array
 * @return Status of read (0 - failure, !0 - success)
 */
static int
rl_load_array (int idx, rl_load_data_t * rl_load_data)
{
  char * data = rl_load_data->ptrs.ra.data[idx].data;
  rl_fd_t fd_ = rl_load_data->ptrs.ra.data[idx].fd;
  int row_count = fd_.param.array_param.row_count;
  int count = fd_.param.array_param.count;
  int i = 0;
  
  if (1 == fd_.param.array_param.row_count)
    fd_.rl_type_ext = RL_TYPE_EXT_NONE; /* prepare copy of filed descriptor for array elements loading */
  else
    {
      fd_.param.array_param.count = row_count;
      fd_.param.array_param.row_count = 1;
    }
  
  /* loop on subnodes */
  for (idx = rl_load_data->ptrs.ra.data[idx].first_child; idx >= 0; idx = rl_load_data->ptrs.ra.data[idx].next)
    {
      /* check if array index is in range */
      if ((i < 0) || (i >= count))
	{
	  RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_RANGE_CHECK, fd_.name);
	  return (0);
	}
      /* load recursively */
      if (!rl_load (&data[i * fd_.size], &fd_, idx, rl_load_data))
	return (0);
      i += row_count;
    }
  return (!0);
}

/**
 * RL_RARRAY load handler.
 * Save content of subnodes to resizeable array elements
 * (allocate/reallocate required memory).
 * @param idx node index
 * @param ptrs pointers descriptors resizeable array
 * @return Status of read (0 - failure, !0 - success)
 */
static int
rl_load_rarray (int idx, rl_load_data_t * rl_load_data)
{
  rl_rarray_t * ra = rl_load_data->ptrs.ra.data[idx].data;
  int data_idx;
  int count;

  memset (ra, 0, sizeof (*ra));
  
#define RL_LOAD_RARRAY_ACTION(TD) ({					\
      int __status = 0;							\
      rl_fd_t * fdp = rl_get_fd_by_name (&TD, "data");			\
      if (NULL == fdp)							\
	RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_RARRAY_FAILED);		\
      else								\
	{								\
	  fdp->type = rl_load_data->ptrs.ra.data[idx].fd.type;		\
	  __status = rl_load_struct_inner (idx, rl_load_data, &TD);	\
	}								\
      __status;								\
    })
  
  if (!RL_LOAD_RARRAY (RL_LOAD_RARRAY_ACTION))
    return (0);

  for (data_idx = rl_load_data->ptrs.ra.data[idx].first_child; data_idx >= 0; data_idx = rl_load_data->ptrs.ra.data[data_idx].next)
    if (0 == strcmp ("data", rl_load_data->ptrs.ra.data[data_idx].fd.name))
      break;

  if (data_idx < 0)
    {
      RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_RARRAY_FAILED);
      return (0);
    }

  /* initialize resizeable array */
  count = ra->size / rl_load_data->ptrs.ra.data[idx].fd.size;
  ra->size = count * rl_load_data->ptrs.ra.data[idx].fd.size;
  
  if ((rl_load_data->ptrs.ra.data[data_idx].ref_idx < 0) && (ra->size > 0))
    {
      int i = 0;
      rl_fd_t fd_ = rl_load_data->ptrs.ra.data[idx].fd;

      fd_.rl_type_ext = RL_TYPE_EXT_NONE;

      ra->data = RL_MALLOC (ra->size);
      if (NULL == ra->data)
	{
	  ra->alloc_size = ra->size = 0;
	  RL_MESSAGE (RL_LL_FATAL, RL_MESSAGE_OUT_OF_MEMORY);
	  return (0);
	}
      memset (ra->data, 0, ra->size);
      ra->alloc_size = ra->size;
      
      for (idx = rl_load_data->ptrs.ra.data[data_idx].first_child; idx >= 0; idx = rl_load_data->ptrs.ra.data[idx].next) /* loop on subnodes */
	{
	  if (!rl_load (((char*)ra->data) + i * fd_.size, &fd_, idx, rl_load_data))
	    return (0);
	  if (++i > count)
	    {
	      RL_MESSAGE (RL_LL_WARN, RL_MESSAGE_RANGE_CHECK, fd_.name);
	      break;
	    }
	}
    }
  return (!0);
}

static int
rl_load_pointer_postponed (int idx, rl_load_data_t * rl_load_data)
{
  void ** data = rl_load_data->ptrs.ra.data[idx].data;
  rl_fd_t fd_ = rl_load_data->ptrs.ra.data[idx].fd;
  fd_.rl_type_ext = RL_TYPE_EXT_NONE;
  /* allocate memory */
  *data = RL_MALLOC (fd_.size);
  if (NULL == *data)
    {
      RL_MESSAGE (RL_LL_FATAL, RL_MESSAGE_OUT_OF_MEMORY);
      return (0);
    }
  memset (*data, 0, fd_.size);
  /* load recursively */
  return (rl_load (*data, &fd_, rl_load_data->ptrs.ra.data[idx].first_child, rl_load_data));
}

/**
 * RL_POINTER_STRUCT load handler.
 * Loads structure into newly allocate memory.
 * @param idx node index
 * @param ptrs pointers descriptors resizeable array
 * @return Status of read (0 - failure, !0 - success)
 */
static int
rl_load_pointer (int idx, rl_load_data_t * rl_load_data)
{
  void ** data = rl_load_data->ptrs.ra.data[idx].data;
  /* default initializer */
  *data = NULL;
  if (rl_load_data->ptrs.ra.data[idx].ref_idx >= 0)
    return (!0);
  if (rl_load_data->ptrs.ra.data[idx].first_child < 0)
    return (!0);
  /* check whether pointer should have offsprings or not */
  if ((RL_TYPE_NONE != rl_load_data->ptrs.ra.data[idx].fd.rl_type) && (RL_TYPE_VOID != rl_load_data->ptrs.ra.data[idx].fd.rl_type))
    {
      int * idx_ = rl_rarray_append ((void*)&rl_load_data->rl_ra_idx, sizeof (rl_load_data->rl_ra_idx.data[0]));
      if (NULL == idx_)
	return (0);
      *idx_ = idx;
      return (!0);
    }

  return (!0);
}

/**
 * RL_ANON_UNION load handler.
 * Load anonymous union
 * @param idx node index
 * @param ptrs pointers descriptors resizeable array
 * @return Status of read (0 - failure, !0 - success)
 */
static int
rl_load_anon_union (int idx, rl_load_data_t * rl_load_data)
{
  /*
    Anonimous unions in C init style saved as named field folowed by union itself. Named field has type of zero length static string and must be inited by empty string. Here is an example.
    .anon_union_79 = "", {
    .union_float = 3.1415927410,
    },
  */
  int next = rl_load_data->ptrs.ra.data[idx].next;
  if ((rl_load_data->ptrs.ra.data[idx].first_child < 0) && /* if node has no childs, then it is C init style anonumous union */
      rl_load_data->ptrs.ra.data[idx].value && (0 == rl_load_data->ptrs.ra.data[idx].value[0]) && /* content must be an empty string */
      (next >= 0) && (NULL == rl_load_data->ptrs.ra.data[next].fd.name)) /* there should be a next node without name */
    {
      if (rl_load_data->ptrs.ra.data[idx].fd.name) /* sainity check - this field can't be NULL */
	rl_load_data->ptrs.ra.data[next].fd.name = RL_STRDUP (rl_load_data->ptrs.ra.data[idx].fd.name);
      return (!0); /* now next node has a name and will be loaded by top level procedure */
    }
  return (rl_load_struct (idx, rl_load_data));
}

int
rl_free_ptrs (rl_ra_rl_ptrdes_t ptrs)
{
  if (ptrs.ra.data)
    {
      int count = ptrs.ra.size / sizeof (ptrs.ra.data[0]);
      int i;
      for (i = 0; i < count; ++i)
	{
	  if (ptrs.ra.data[i].value)
	    RL_FREE (ptrs.ra.data[i].value);
	  ptrs.ra.data[i].value = NULL;
	  if (ptrs.ra.data[i].fd.type)
	    RL_FREE (ptrs.ra.data[i].fd.type);
	  ptrs.ra.data[i].fd.type = NULL;
	  if (ptrs.ra.data[i].fd.name)
	    RL_FREE (ptrs.ra.data[i].fd.name);
	  ptrs.ra.data[i].fd.name = NULL;
	}
      RL_FREE (ptrs.ra.data);
      ptrs.ra.data = NULL;
      ptrs.ra.size = ptrs.ra.alloc_size = 0;
    }
  return (EXIT_SUCCESS);
}

/**
 * Public function. Load router. Load any object from RL nodes.
 * @param data pointer on place to save int
 * @param fdp pointer on filed descriptor
 * @param idx node index
 * @param ptrs pointers descriptors resizeable array
 * @return Status of read (0 - failure, !0 - success)
 */
int
rl_load (void * data, rl_fd_t * fdp, int idx, rl_load_data_t * rl_load_data)
{
  int status = 0;

  if (0 == idx)
    {
      rl_load_data->rl_ra_idx.data = NULL;
      rl_load_data->rl_ra_idx.size = 0;
      rl_load_data->rl_ra_idx.alloc_size = 0;
    }
  
  if ((idx < 0) || (idx >= rl_load_data->ptrs.ra.size / sizeof (rl_load_data->ptrs.ra.data[0])))
    {
      RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_SAVE_IDX_RANGE_CHECK);
      return (0);
    }
  
  rl_load_data->ptrs.ra.data[idx].data = data;
  if (rl_load_data->ptrs.ra.data[idx].fd.name && fdp->name)
    if (strcmp (fdp->name, rl_load_data->ptrs.ra.data[idx].fd.name))
      {
	RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_NODE_NAME_MISSMATCH, fdp->name, rl_load_data->ptrs.ra.data[idx].fd.name);
	return (0);
      }

  if (rl_load_data->ptrs.ra.data[idx].fd.type && fdp->type)
    if (strcmp (fdp->type, rl_load_data->ptrs.ra.data[idx].fd.type))
      {
	RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_NODE_TYPE_MISSMATCH, fdp->type, rl_load_data->ptrs.ra.data[idx].fd.type);
	return (0);
      }
  
  if ((NULL == rl_load_data->ptrs.ra.data[idx].fd.name) && (fdp->name))
    rl_load_data->ptrs.ra.data[idx].fd.name = RL_STRDUP (fdp->name);
  if ((NULL == rl_load_data->ptrs.ra.data[idx].fd.type) && (fdp->type))
    rl_load_data->ptrs.ra.data[idx].fd.type = RL_STRDUP (fdp->type);
  rl_load_data->ptrs.ra.data[idx].fd.size = fdp->size;
  rl_load_data->ptrs.ra.data[idx].fd.rl_type = fdp->rl_type;
  rl_load_data->ptrs.ra.data[idx].fd.rl_type_aux = fdp->rl_type_aux;
  rl_load_data->ptrs.ra.data[idx].fd.rl_type_ext = fdp->rl_type_ext;
  rl_load_data->ptrs.ra.data[idx].fd.param = fdp->param;
  
  /* route loading */
  if ((fdp->rl_type_ext >= 0) && (fdp->rl_type_ext < RL_MAX_TYPES)
      && rl_conf.io_ext_handlers[fdp->rl_type_ext].load.rl)
    status = rl_conf.io_ext_handlers[fdp->rl_type_ext].load.rl (idx, rl_load_data);
  else if ((fdp->rl_type >= 0) && (fdp->rl_type < RL_MAX_TYPES)
	   && rl_conf.io_handlers[fdp->rl_type].load.rl)
    status = rl_conf.io_handlers[fdp->rl_type].load.rl (idx, rl_load_data);
  else
    RL_MESSAGE_UNSUPPORTED_NODE_TYPE_ (fdp);    

  /* set cross references at the upper level */
  if (0 == idx)
    {
      while (rl_load_data->rl_ra_idx.size > 0)
	{
	  rl_load_data->rl_ra_idx.size -= sizeof (rl_load_data->rl_ra_idx.data[0]);
	  rl_load_pointer_postponed (rl_load_data->rl_ra_idx.data[rl_load_data->rl_ra_idx.size / sizeof (rl_load_data->rl_ra_idx.data[0])], rl_load_data);
	}
      if (status)
	status = rl_set_crossrefs (rl_load_data);
      if (rl_load_data->rl_ra_idx.data)
	{
	  RL_FREE (rl_load_data->rl_ra_idx.data);
	  rl_load_data->rl_ra_idx.data = NULL;
	  rl_load_data->rl_ra_idx.size = 0;
	  rl_load_data->rl_ra_idx.alloc_size = 0;
	}
    }
  
  return (status);
}     

/**
 * Init IO handlers Table
 */
static void __attribute__((constructor)) rl_init_load_rl (void)
{
  rl_conf.io_handlers[RL_TYPE_NONE].load.rl = rl_load_none;
  rl_conf.io_handlers[RL_TYPE_VOID].load.rl = rl_load_none;
  rl_conf.io_handlers[RL_TYPE_ENUM].load.rl = rl_load_enum;
  rl_conf.io_handlers[RL_TYPE_BITFIELD].load.rl = rl_load_bitfield;
  rl_conf.io_handlers[RL_TYPE_BITMASK].load.rl = rl_load_bitmask;
  rl_conf.io_handlers[RL_TYPE_INT8].load.rl = rl_load_integer;
  rl_conf.io_handlers[RL_TYPE_UINT8].load.rl = rl_load_integer;
  rl_conf.io_handlers[RL_TYPE_INT16].load.rl = rl_load_integer;
  rl_conf.io_handlers[RL_TYPE_UINT16].load.rl = rl_load_integer;
  rl_conf.io_handlers[RL_TYPE_INT32].load.rl = rl_load_integer;
  rl_conf.io_handlers[RL_TYPE_UINT32].load.rl = rl_load_integer;
  rl_conf.io_handlers[RL_TYPE_INT64].load.rl = rl_load_integer;
  rl_conf.io_handlers[RL_TYPE_UINT64].load.rl = rl_load_integer;
  rl_conf.io_handlers[RL_TYPE_FLOAT].load.rl = rl_load_float;
  rl_conf.io_handlers[RL_TYPE_DOUBLE].load.rl = rl_load_double;
  rl_conf.io_handlers[RL_TYPE_LONG_DOUBLE].load.rl = rl_load_long_double_t;
  rl_conf.io_handlers[RL_TYPE_CHAR].load.rl = rl_load_char;
  rl_conf.io_handlers[RL_TYPE_CHAR_ARRAY].load.rl = rl_load_char_array;
  rl_conf.io_handlers[RL_TYPE_STRING].load.rl = rl_load_string;
  rl_conf.io_handlers[RL_TYPE_STRUCT].load.rl = rl_load_struct;
  rl_conf.io_handlers[RL_TYPE_FUNC].load.rl = rl_load_none;
  rl_conf.io_handlers[RL_TYPE_FUNC_TYPE].load.rl = rl_load_none;
  rl_conf.io_handlers[RL_TYPE_UNION].load.rl = rl_load_struct;
  rl_conf.io_handlers[RL_TYPE_ANON_UNION].load.rl = rl_load_anon_union;

  rl_conf.io_ext_handlers[RL_TYPE_EXT_ARRAY].load.rl = rl_load_array;
  rl_conf.io_ext_handlers[RL_TYPE_EXT_RARRAY].load.rl = rl_load_rarray;
  rl_conf.io_ext_handlers[RL_TYPE_EXT_POINTER].load.rl = rl_load_pointer;
};
