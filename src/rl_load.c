/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

#include <stdio.h>
#include <string.h>
#define RL_MODE PROTO /* explicitly set type of inclusion */
#include <reslib.h>

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
rl_set_crossrefs (rl_ra_rl_ptrdes_t * ptrs)
{
  int i;
  int count = ptrs->ra.size / sizeof (ptrs->ra.data[0]);
  int max_idx = -1;
  int * table;

  for (i = 0; i < count; ++i)
    if (ptrs->ra.data[i].idx > max_idx)
      max_idx = ptrs->ra.data[i].idx;

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
    if (ptrs->ra.data[i].idx >= 0)
      {
	/* checking indexes collision */
	if (table[ptrs->ra.data[i].idx] >= 0)
	  RL_MESSAGE (RL_LL_WARN, RL_MESSAGE_IDS_COLLISION, RL_REF_IDX, ptrs->ra.data[i].idx);
	table[ptrs->ra.data[i].idx] = i;
      }

  /* set all cross refernces */
  for (i = 0; i < count; ++i)
    if (ptrs->ra.data[i].ref_idx >= 0)
      {
	int idx = table[ptrs->ra.data[i].ref_idx];
	if (idx < 0)
	  RL_MESSAGE (RL_LL_WARN, RL_MESSAGE_UNDEFINED_REF_IDX, RL_REF_IDX, ptrs->ra.data[i].ref_idx);
	else
	  *(void**)ptrs->ra.data[i].data = ptrs->ra.data[idx].data;
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
      int count = tdp->fields.size / sizeof (tdp->fields.data[0]);
      char * name_ = args;

      if (RL_TYPE_ENUM == tdp->rl_type)
	for (i = 0; i < count; ++i)
	  if (0 == strcmp (tdp->fields.data[i].name, name_))
	    {
	      *data = tdp->fields.data[i].value;
	      return (!0);
	    }
      return (0);
    }

    if (rl_td_foreach (enum2int, name_))
#else /* ! RL_LINEAR_SEARCH */
      if (rl_get_enum_by_name (data, name_))
#endif /* RL_LINEAR_SEARCH */
	return (str);
      else
	{
	  RL_MESSAGE (RL_LL_WARN, RL_MESSAGE_UNKNOWN_ENUM, name);
	  return (NULL);
	}
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
rl_load_none (int idx, rl_ra_rl_ptrdes_t * ptrs)
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
rl_load_integer (int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  char * str = ptrs->ra.data[idx].value;
  uint64_t value;
  
  if (NULL == str)
    {
      RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_READ_INT, ptrs->ra.data[idx].value);
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
	RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_READ_INT, ptrs->ra.data[idx].value);
      return (0);
    }
  memcpy (ptrs->ra.data[idx].data, &value, ptrs->ra.data[idx].fd.size);
  return (!0);
}

/**
 * RL_ENUM load handler
 * @param idx node index
 * @param ptrs pointers descriptors resizeable array
 * @return Status of read (0 - failure, !0 - success)
 */
static int
rl_load_enum (int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  char * str = ptrs->ra.data[idx].value;
  char * tail;
  rl_td_t * tdp = rl_get_td_by_name (ptrs->ra.data[idx].fd.type);
  rl_fd_t * fdp;

  if (NULL == str)
    {
      RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_READ_ENUM, ptrs->ra.data[idx].value);
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
	  memcpy (ptrs->ra.data[idx].data, &fdp->param.enum_value, ptrs->ra.data[idx].fd.size);
	  return (!0);
	}
    }
  return (rl_load_integer (idx, ptrs));
}

/**
 * RL_BITFIELD load handler. Load int from string and save it to
 * bit shifted field.
 * @param idx node index
 * @param ptrs pointers descriptors resizeable array
 * @return Status of read (0 - failure, !0 - success)
 */
static int
rl_load_bitfield (int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  rl_ptrdes_t * ptrdes = &ptrs->ra.data[idx];
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
rl_load_bitmask (int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  char * str = ptrs->ra.data[idx].value;
  int64_t value = 0;
  
  if (NULL == str)
    {
      RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_READ_BITMASK, ptrs->ra.data[idx].value);
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
      RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_READ_BITMASK, ptrs->ra.data[idx].value);
      return (0);
    }
  memcpy (ptrs->ra.data[idx].data, &value, ptrs->ra.data[idx].fd.size);
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
  rl_load_ ## TYPE (int idx, rl_ra_rl_ptrdes_t * ptrs)			\
  {									\
    char * str = ptrs->ra.data[idx].value;				\
    int offset;								\
    if (NULL == str)							\
      {									\
	RL_MESSAGE (RL_LL_ERROR, ERROR_ENUM, ptrs->ra.data[idx].value);	\
	return (0);							\
      }									\
    if (1 != sscanf (str, FORMAT "%n", (TYPE*)ptrs->ra.data[idx].data, &offset)) \
      {									\
	RL_MESSAGE (RL_LL_ERROR, ERROR_ENUM, ptrs->ra.data[idx].value);	\
	return (0);							\
      }									\
    str += offset;							\
    while (isspace (*str))						\
      ++str;								\
    if (*str != 0)							\
      {									\
	RL_MESSAGE (RL_LL_ERROR, ERROR_ENUM, ptrs->ra.data[idx].value);	\
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
rl_load_char (int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  char * str = ptrs->ra.data[idx].value;
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
      *(char*)ptrs->ra.data[idx].data = str[0];
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
      *(char*)ptrs->ra.data[idx].data = val;
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
rl_load_string (int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  char * str = ptrs->ra.data[idx].value;
  if (ptrs->ra.data[idx].flags & RL_PDF_IS_NULL)
    *(char**)ptrs->ra.data[idx].data = NULL;
  else
    *(char**)ptrs->ra.data[idx].data = str ? RL_STRDUP (str) : NULL;
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
rl_load_char_array (int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  char * str = ptrs->ra.data[idx].value;
  int max_size = ptrs->ra.data[idx].fd.param.array_param.count * ptrs->ra.data[idx].fd.size;
  if (str)
    {
      if (strlen (str) >= max_size)
	{
	  str[max_size - 1] = 0;
	  RL_MESSAGE (RL_LL_WARN, RL_MESSAGE_STRING_TRUNCATED);
	}
      strcpy (ptrs->ra.data[idx].data, str);
    }
  else
    memset (ptrs->ra.data[idx].data, 0, max_size);
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
rl_load_struct (int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  rl_td_t * tdp = rl_get_td_by_name (ptrs->ra.data[idx].fd.type); /* look up for type descriptor */
  char * data = ptrs->ra.data[idx].data;
  rl_fd_t * fdp;
  
  /* get pointer on structure descriptor */
  if (NULL == tdp)
    {
      RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_NO_TYPE_DESCRIPTOR, ptrs->ra.data[idx].fd.type);
      return (0);
    }

  fdp = tdp->fields.data;
  /* loop on all subnodes */
  for (idx = ptrs->ra.data[idx].first_child; idx >= 0; idx = ptrs->ra.data[idx].next)
    {
      if (ptrs->ra.data[idx].fd.name)
	fdp = rl_get_fd_by_name (tdp, ptrs->ra.data[idx].fd.name);
      if (NULL == fdp)
	{
	  RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_UNKNOWN_SUBNODE, ptrs->ra.data[idx].fd.type, ptrs->ra.data[idx].fd.name);
	  return (0);
	}
      /* recursively load subnode */
      if (!rl_load (&data[fdp->offset], fdp, idx, ptrs))
	return (0);
      
      ++fdp;
      if (RL_TYPE_TRAILING_RECORD == fdp->rl_type)
	fdp = NULL;
    }
  return (!0);
}

/**
 * RL_ARRAY load handler.
 * Save content of subnodes to array elements.
 * @param idx node index
 * @param ptrs pointers descriptors resizeable array
 * @return Status of read (0 - failure, !0 - success)
 */
static int
rl_load_array (int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  char * data = ptrs->ra.data[idx].data;
  rl_fd_t fd_ = ptrs->ra.data[idx].fd;
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
  for (idx = ptrs->ra.data[idx].first_child; idx >= 0; idx = ptrs->ra.data[idx].next)
    {
      /* check if array index is in range */
      if ((i < 0) || (i >= count))
	{
	  RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_RANGE_CHECK, fd_.name);
	  return (0);
	}
	/* load recursively */
	if (!rl_load (&data[i * fd_.size], &fd_, idx, ptrs))
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
rl_load_rarray (int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  rl_rarray_t * ra = ptrs->ra.data[idx].data;
  rl_fd_t fd_ = ptrs->ra.data[idx].fd;
  char * name = NULL;
  int count = 0;
  int idx_;

  /* initialize resizeable array */
  ra->ext = rl_get_td_by_name (fd_.name);
  ra->size = ra->alloc_size = 0;
  ra->data = NULL;

  /* check that all childs has one and the same name */
  for (idx_ = ptrs->ra.data[idx].first_child; idx_ >= 0; idx_ = ptrs->ra.data[idx_].next)
    if (ptrs->ra.data[idx_].fd.name)
      {
	if (name && (strcmp (name, ptrs->ra.data[idx_].fd.name)))
	  break;
	name = ptrs->ra.data[idx_].fd.name;
      }

  if (idx_ >= 0) /* cinit style rl_rarray_t - some nodes have different names */
    {
      for (idx_ = ptrs->ra.data[idx].first_child; idx_ >= 0; idx_ = ptrs->ra.data[idx_].next)
	if (!strcmp (ptrs->ra.data[idx_].fd.name, "data"))
	  break;
      if (idx_ < 0)
	{
	  RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_RARRAY_LOAD_FAILED);
	  return (0);
	}
      idx = idx_;
    }

  for (idx_ = ptrs->ra.data[idx].first_child; idx_ >= 0; idx_ = ptrs->ra.data[idx_].next)
    ++count;
  if (count > 0)
    {
      ra->size = count * fd_.size;
      ra->data = RL_MALLOC (ra->size);
      if (NULL == ra->data)
	{
	  ra->size = 0;
	  RL_MESSAGE (RL_LL_FATAL, RL_MESSAGE_OUT_OF_MEMORY);
	  return (0);
	}
      memset (ra->data, 0, ra->size);
      count = 0;
      /* prepare copy of filed descriptor for array elements loading */
      fd_.rl_type_ext = RL_TYPE_EXT_NONE;
      
      for (idx = ptrs->ra.data[idx].first_child; idx >= 0; idx = ptrs->ra.data[idx].next) /* loop on subnodes */
	{
	  if (!rl_load (((char*)ra->data) + count * fd_.size, &fd_, idx, ptrs))
	    return (0);
	  ++count;
	}
    }
  return (!0);
}

static int
rl_load_pointer_postponed (int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  void ** data = ptrs->ra.data[idx].data;
  rl_fd_t fd_ = ptrs->ra.data[idx].fd;
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
  return (rl_load (*data, &fd_, ptrs->ra.data[idx].first_child, ptrs));
}

/**
 * RL_POINTER_STRUCT load handler.
 * Loads structure into newly allocate memory.
 * @param idx node index
 * @param ptrs pointers descriptors resizeable array
 * @return Status of read (0 - failure, !0 - success)
 */
static int
rl_load_pointer (int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  void ** data = ptrs->ra.data[idx].data;
  /* default initializer */
  *data = NULL;
  if (ptrs->ra.data[idx].ref_idx >= 0)
    return (!0);
  if (ptrs->ra.data[idx].first_child < 0)
    return (!0);
  /* check whether pointer should have offsprings or not */
  if ((RL_TYPE_NONE == ptrs->ra.data[idx].fd.rl_type) || (RL_TYPE_VOID == ptrs->ra.data[idx].fd.rl_type))
    RL_MESSAGE (RL_LL_WARN, RL_MESSAGE_UNEXPECTED_DATA);
  else
    {
      int * idx_ = rl_rarray_append (ptrs->ra.ext, sizeof (int));
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
rl_load_anon_union (int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  /*
    Anonimous unions in C init style saved as named field folowed by union itself. Named field has type of zero length static string and must be inited by empty string. Here is an example.
    .anon_union_79 = "", {
      .union_float = 3.1415927410,
    },
   */
  int next = ptrs->ra.data[idx].next;
  if ((ptrs->ra.data[idx].first_child < 0) && /* if node has no childs, then it is C init style anonumous union */
      ptrs->ra.data[idx].value && (0 == ptrs->ra.data[idx].value[0]) && /* content must be an empty string */
      (next >= 0) && (NULL == ptrs->ra.data[next].fd.name)) /* there should be a next node without name */
    {
      if (ptrs->ra.data[idx].fd.name) /* sainity check - this field can't be NULL */
	ptrs->ra.data[next].fd.name = RL_STRDUP (ptrs->ra.data[idx].fd.name);
      return (!0); /* now next node has a name and will be loaded by top level procedure */
    }
  return (rl_load_struct (idx, ptrs));
}

void
rl_free_ptrs (rl_ra_rl_ptrdes_t * ptrs)
{
  if (ptrs->ra.data)
    {
      int count = ptrs->ra.size / sizeof (ptrs->ra.data[0]);
      int i;
      for (i = 0; i < count; ++i)
	{
	  if (ptrs->ra.data[i].value)
	    RL_FREE (ptrs->ra.data[i].value);
	  ptrs->ra.data[i].value = NULL;
	  if (ptrs->ra.data[i].fd.type)
	    RL_FREE (ptrs->ra.data[i].fd.type);
	  ptrs->ra.data[i].fd.type = NULL;
	  if (ptrs->ra.data[i].fd.name)
	    RL_FREE (ptrs->ra.data[i].fd.name);
	  ptrs->ra.data[i].fd.name = NULL;
	}
      RL_FREE (ptrs->ra.data);
      ptrs->ra.data = NULL;
      ptrs->ra.size = ptrs->ra.alloc_size = 0;
    }
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
rl_load (void * data, rl_fd_t * fdp, int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  int status = 0;
  RL_RARRAY_PROTO ( ,int, rl_ra_idx);

  rl_ra_idx.data = NULL;
  rl_ra_idx.size = 0;
  if (NULL == ptrs->ra.ext)
    ptrs->ra.ext = &rl_ra_idx;

  if ((idx < 0) || (idx >= ptrs->ra.size / sizeof (ptrs->ra.data[0])))
    {
      RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_SAVE_IDX_RANGE_CHECK);
      return (0);
    }
  
  ptrs->ra.data[idx].data = data;
  if (ptrs->ra.data[idx].fd.name && fdp->name)
    if (strcmp (fdp->name, ptrs->ra.data[idx].fd.name))
      {
	RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_NODE_NAME_MISSMATCH, fdp->name, ptrs->ra.data[idx].fd.name);
	return (0);
      }
  if (ptrs->ra.data[idx].fd.type && fdp->type)
    if (strcmp (fdp->type, ptrs->ra.data[idx].fd.type))
      {
	RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_NODE_TYPE_MISSMATCH, fdp->type, ptrs->ra.data[idx].fd.type);
	return (0);
      }
  
  if ((NULL == ptrs->ra.data[idx].fd.name) && (fdp->name))
    ptrs->ra.data[idx].fd.name = RL_STRDUP (fdp->name);
  if ((NULL == ptrs->ra.data[idx].fd.type) && (fdp->type))
    ptrs->ra.data[idx].fd.type = RL_STRDUP (fdp->type);
  ptrs->ra.data[idx].fd.size = fdp->size;
  ptrs->ra.data[idx].fd.param.array_param.count = fdp->param.array_param.count;
  ptrs->ra.data[idx].fd.param.array_param.row_count = fdp->param.array_param.row_count;
  ptrs->ra.data[idx].fd.rl_type = fdp->rl_type;
  ptrs->ra.data[idx].fd.rl_type_aux = fdp->rl_type_aux;
  ptrs->ra.data[idx].fd.rl_type_ext = fdp->rl_type_ext;
  ptrs->ra.data[idx].fd.param.bitfield_param.width = fdp->param.bitfield_param.width;
  ptrs->ra.data[idx].fd.param.bitfield_param.shift = fdp->param.bitfield_param.shift;
  
  /* route loading */
  if ((fdp->rl_type_ext >= 0) && (fdp->rl_type_ext < RL_MAX_TYPES)
      && rl_conf.io_ext_handlers[fdp->rl_type_ext].load.rl)
    status = rl_conf.io_ext_handlers[fdp->rl_type_ext].load.rl (idx, ptrs);
  else if ((fdp->rl_type >= 0) && (fdp->rl_type < RL_MAX_TYPES)
	   && rl_conf.io_handlers[fdp->rl_type].load.rl)
    status = rl_conf.io_handlers[fdp->rl_type].load.rl (idx, ptrs);
  else
    RL_MESSAGE_UNSUPPORTED_NODE_TYPE_ (fdp);    

  /* set cross references at the upper level */
  if (0 == idx)
    {
      while (rl_ra_idx.size > 0)
	{
	  rl_ra_idx.size -= sizeof (rl_ra_idx.data[0]);
	  rl_load_pointer_postponed (rl_ra_idx.data[rl_ra_idx.size / sizeof (rl_ra_idx.data[0])], ptrs);
	}
      if (status)
	status = rl_set_crossrefs (ptrs);
      if (rl_ra_idx.data)
	RL_FREE (rl_ra_idx.data);
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
