/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#ifdef HAVE_CONFIG_H
# include <mr_config.h>
#endif /* HAVE_CONFIG_H */

#define __USE_GNU
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <limits.h>
#include <execinfo.h>

#include <metaresc.h>
#include <mr_tsearch.h>
#include <mr_save.h>
#include <mr_ic.h>
#include <mr_stringify.h>

#define MR_MODE DESC /* we'll need descriptors of our own types */
#include <mr_protos.h>

#define MR_TYPEDEF_DESC_BI_(TYPE, MR_TYPE, ...) MR_TYPEDEF_DESC (TYPE, MR_TYPE) MR_TYPEDEF_END_DESC (TYPE, __VA_ARGS__);
#define MR_TYPEDEF_DESC_BI(TYPE, ...) MR_TYPEDEF_DESC_BI_ (TYPE, MR_TYPE_DETECT (TYPE))

/* meta data for type 'char' - required as a descriminator for mr_ptr union */
MR_TYPEDEF_DESC_BI_ (char, MR_TYPE_CHAR_ARRAY, "type descriptor for 'char'", .size = 0);
/* meta data for all scallar types */
MR_TYPEDEF_DESC_BI (char_t, "alias for type 'char' for serialization as array of char");
MR_TYPEDEF_DESC_BI (bool);
MR_TYPEDEF_DESC_BI (uint8_t);
MR_TYPEDEF_DESC_BI (int8_t);
MR_TYPEDEF_DESC_BI (uint16_t);
MR_TYPEDEF_DESC_BI (int16_t);
MR_TYPEDEF_DESC_BI (uint32_t);
MR_TYPEDEF_DESC_BI (int32_t);
MR_TYPEDEF_DESC_BI (uint64_t);
MR_TYPEDEF_DESC_BI (int64_t);
MR_TYPEDEF_DESC_BI (int);
MR_TYPEDEF_DESC_BI (short);
MR_TYPEDEF_DESC_BI (long);
MR_TYPEDEF_DESC_BI (long_long_int_t);
MR_TYPEDEF_DESC_BI (float);
MR_TYPEDEF_DESC_BI (double);
MR_TYPEDEF_DESC_BI (long_double_t);
MR_TYPEDEF_DESC_BI (complex_float_t);
MR_TYPEDEF_DESC_BI (complex_double_t);
MR_TYPEDEF_DESC_BI (complex_long_double_t);

/* MR_IC_NONE:        ( / 363893 11767.0) ratio: 30.92 */
/* MR_IC_RBTREE:       ( / 82415 15641.0) ratio: 5.26  */
/* MR_IC_SORTED_ARRAY: ( / 80284 15272.0) ratio: 5.25  */
/* MR_IC_HASH_NEXT:    ( / 39304 30176.0) ratio: 1.30  */
/* MR_IC_HASH_TREE:    ( / 38277 23013.0) ratio: 1.66  */

#define MR_IC_STATIC_DEFAULT MR_IC_HASH_NEXT

void * mr_malloc (const char * filename, const char * function, int line, size_t size) { return (malloc (size)); }
void * mr_realloc (const char * filename, const char * function, int line, void * ptr, size_t size) { return (realloc (ptr, size)); }
char * mr_strdup (const char * filename, const char * function, int line, const char * str) { return (strdup (str)); }
void mr_free (const char * filename, const char * function, int line, void * ptr) { free (ptr); }

/** Metaresc configuration structure */
mr_conf_t mr_conf = {
  .mr_mem = { /**< all memory functions may be replaced on user defined */
    .mem_alloc_strategy = 2, /**< Memory allocation strategy. Default is to double buffer every time. */
    .malloc = mr_malloc, /**< Pointer to malloc function. */
    .realloc = mr_realloc, /**< Pointer to realloc function. */
    .strdup = mr_strdup, /**< Pointer to strdup function. */
    .free = mr_free, /**< Pointer to free function. */
  },
  .log_level = MR_LL_ALL, /**< default log level ALL */
  .msg_handler = NULL, /**< pointer on user defined message handler */
  .lookup_by_name = {
    .ic_type = MR_IC_NONE,
    .find = NULL,
    .ext = { .ptr = NULL, },
  },
  .output_format = { [0 ... MR_TYPE_LAST - 1] = NULL, },
};

MR_MEM_INIT ( , __attribute__((constructor,weak)));

static mr_status_t
mr_td_visitor (mr_ptr_t key, const void * context)
{
  mr_td_t * tdp = key.ptr;
  mr_ic_free (&tdp->lookup_by_name);
  mr_ic_free (&tdp->lookup_by_value);
  return (MR_SUCCESS);
}

/**
 * Memory cleanp handler.
 */
static void __attribute__((destructor))
mr_cleanup (void)
{
  mr_ic_foreach (&mr_conf.lookup_by_name, mr_td_visitor, NULL);
  mr_ic_free (&mr_conf.enum_by_name);
  mr_ic_free (&mr_conf.lookup_by_name);
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
  static bool messages_inited = FALSE;
  const char * format = "Unknown MR_MESSAGE_ID.";
  char * message = NULL;

  if (!messages_inited)
    {
      mr_td_t * tdp = &MR_DESCRIPTOR_PREFIX (mr_message_id_t);
      if (tdp)
	{
	  int i;
	  for (i = 0; MR_TYPE_ENUM_VALUE == tdp->fields[i].fdp->mr_type; ++i)
	    messages[tdp->fields[i].fdp->param.enum_value] = tdp->fields[i].fdp->meta;
	  messages_inited = TRUE;
	}
    }

  if ((message_id <= sizeof (messages) / sizeof (messages[0])) && messages[message_id])
    format = messages[message_id];

  int __attribute__ ((unused)) unused = vasprintf (&message, format, args);

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
  else if (log_level > mr_conf.log_level)
    {
      const char * log_level_str_ = "Unknown";
#define LL_INIT(LEVEL) [MR_LL_##LEVEL] = #LEVEL
      static const char * log_level_str[] =
	{ LL_INIT (ALL), LL_INIT (TRACE), LL_INIT (DEBUG), LL_INIT (INFO), LL_INIT (WARN), LL_INIT (ERROR), LL_INIT (FATAL), LL_INIT (OFF) };

      if (((int)log_level >= 0)
	  && ((int)log_level <= sizeof (log_level_str) / sizeof (log_level_str[0]))
	  && log_level_str[log_level])
	log_level_str_ = log_level_str[log_level];

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
      
      message = mr_message_format (message_id, args);
      if (message)
	{
	  fprintf (stderr, "%s: in %s %s() line %d: %s\n", log_level_str_, file_name, func_name, line, message);
	  free (message);
	}
    }
  va_end (args);
}

/**
 * Helper function for serialization macroses. Extracts variable name that was passed for serialization.
 * Possible variants are: var_name, &var_name, &var_name[idx], &((type*)var_name)[idx], etc
 *
 * @param name string with var_name
 * @return variable name var_name
 */
char *
mr_normalize_name (char * name)
{
  char * ptr;
  ptr = strchr (name, '['); /* lookup for a bracket */
  if (NULL == ptr)          /* if bracket was not found */
    ptr = strchr (name, 0); /* use end of the string */
  --ptr;
  /* skip all invalid characters */
  while ((ptr >= name) && !(isalnum (*ptr) || ('_' == *ptr)))
    --ptr;
  *(ptr + 1) = 0; /* put end-of-string marker */
  /* all valid characters to the left forms the var_name */
  while ((ptr >= name) && (isalnum (*ptr) || ('_' == *ptr)))
    --ptr;
  return (++ptr);
}

#ifndef HAVE_STRNDUP
/**
 * Allocate new string and copy first 'size' chars from str.
 * For compilers without GNU extension
 *
 * @param str string to duplicate
 * @param size size to duplicate
 * @return A pointer on newly allocated string
 */
char *
strndup (const char * str, size_t size)
{
  char * copy;
  if (strlen (str) < size)
    size = strlen (str);
  copy = (char*)MR_MALLOC (size + 1);
  if (NULL == copy)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (NULL);
    }
  memcpy (copy, str, size);
  copy[size] = 0;
  return (copy);
}
#endif /* HAVE_STRNDUP */

/**
 * Extract bits of bit-field, extend sign bits if needed.
 * @param ptrdes pointer descriptor
 * @param value pointer on variable for bit-field value
 * @return status 
 */
mr_status_t
mr_save_bitfield_value (mr_ptrdes_t * ptrdes, uint64_t * value)
{
  uint8_t * ptr = ptrdes->data;
  int i;

  *value = *ptr++ >> ptrdes->fd.param.bitfield_param.shift;
  for (i = CHAR_BIT - ptrdes->fd.param.bitfield_param.shift; i < ptrdes->fd.param.bitfield_param.width; i += CHAR_BIT)
    *value |= ((uint64_t)*ptr++) << i;
  *value &= (2LL << (ptrdes->fd.param.bitfield_param.width - 1)) - 1;
  switch (ptrdes->fd.mr_type_aux)
    {
    case MR_TYPE_INT8:
    case MR_TYPE_INT16:
    case MR_TYPE_INT32:
    case MR_TYPE_INT64:
      /* extend sign bit */
      if (*value & (1 << (ptrdes->fd.param.bitfield_param.width - 1)))
	*value |= -1 - ((2LL << (ptrdes->fd.param.bitfield_param.width - 1)) - 1);
      break;
    default:
      break;
    }
  return (MR_SUCCESS);
}

/**
 * Saves bit-field into memory
 * @param ptrdes pointer descriptor
 * @param value pointer on a memory for a bit-field store
 * @return status
 */
mr_status_t
mr_load_bitfield_value (mr_ptrdes_t * ptrdes, uint64_t * value)
{
  uint8_t * ptr = ptrdes->data;
  int i;

  *value &= (2LL << (ptrdes->fd.param.bitfield_param.width - 1)) - 1;
  if (ptrdes->fd.param.bitfield_param.shift + ptrdes->fd.param.bitfield_param.width >= CHAR_BIT)
    *ptr &= ((1 << ptrdes->fd.param.bitfield_param.shift) - 1);
  else
    *ptr &= (-1 - ((1 << (ptrdes->fd.param.bitfield_param.shift + ptrdes->fd.param.bitfield_param.width)) - 1)) | ((1 << ptrdes->fd.param.bitfield_param.shift) - 1);
  *ptr++ |= *value << ptrdes->fd.param.bitfield_param.shift;
  for (i = CHAR_BIT - ptrdes->fd.param.bitfield_param.shift; i < ptrdes->fd.param.bitfield_param.width; i += CHAR_BIT)
    if (ptrdes->fd.param.bitfield_param.width - i >= CHAR_BIT)
      *ptr++ = *value >> i;
    else
      {
	*ptr &= -1 - ((1 << (ptrdes->fd.param.bitfield_param.width - i)) - 1);
	*ptr++ |= *value >> i;
      }
  return (MR_SUCCESS);
}

/**
 * Rarray memory allocation/reallocation
 * @param rarray a pointer on resizable array
 * @param size size of array elements
 * @return Pointer on a new element of rarray
 */
void *
mr_rarray_allocate_element (void ** data, ssize_t * size, ssize_t * alloc_size, int element_size)
{
  if ((NULL == data) || (NULL == size) || (NULL == alloc_size))
    return (NULL);
  
  if (NULL == *data)
    {
      *alloc_size = *size = 0;
      *data = MR_MALLOC (element_size);
      if (NULL == *data)
	MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      else
	{
	  memset (*data, 0, element_size);
	  *alloc_size = *size = element_size;
	}
      return (*data);
    }

  *size += element_size;
  if (*size > *alloc_size)
    {
      float mas = mr_conf.mr_mem.mem_alloc_strategy;
      int realloc_size;
      void * data_;
      if (mas < 1)
	mas = 1;
      if (mas > 2)
	mas = 2;
      realloc_size = (((int)((*alloc_size + 1) * mas) + *size - 1) / *size) * *size;
      if (realloc_size < *size)
	realloc_size = *size;
      data_ = MR_REALLOC (*data, realloc_size);
      if (NULL == data_)
	{
	  MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	  return (NULL);
	}
      *alloc_size = realloc_size;
      *data = data_;
    }
  return (&((char*)(*data))[*size - element_size]);
}

void *
mr_rarray_append (mr_rarray_t * rarray, int size)
{
  return (mr_rarray_allocate_element (&rarray->data.ptr, &rarray->MR_SIZE, &rarray->alloc_size, size));
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
  int length;
  char * str;
  char * tail;

  va_start (args, format);
  length = vasprintf (&str, format, args);
  va_end (args);
  if (NULL == str)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      if (mr_ra_str->data.ptr)
	MR_FREE (mr_ra_str->data.ptr);
      mr_ra_str->data.ptr = NULL;
      mr_ra_str->MR_SIZE = mr_ra_str->alloc_size = 0;
      return (-1);
    }
  tail = mr_rarray_append (mr_ra_str, length);
  if (tail)
    strcat (--tail, str);
  else if (mr_ra_str->data.ptr)
    {
      MR_FREE (mr_ra_str->data.ptr);
      mr_ra_str->data.ptr = NULL;
      mr_ra_str->MR_SIZE = mr_ra_str->alloc_size = 0;
    }
  free (str);
  return (tail ? length : -1);
}

/**
 * Allocate element for pointer descriptor in resizable array.
 * @param ptrs resizable array with pointers on already saved structures
 * @return Index of pointer in the list or -1 in case of memory operation error.
 * On higher level we need index because array is always reallocating and
 * pointer on element is changing (index remains constant).
 */
int
mr_add_ptr_to_list (mr_ra_mr_ptrdes_t * ptrs)
{
  mr_ptrdes_t * ptrdes = mr_rarray_allocate_element ((void**)&ptrs->ra, &ptrs->size, &ptrs->alloc_size, sizeof (ptrs->ra[0]));
  if (NULL == ptrdes)
    return (-1);
  memset (ptrdes, 0, sizeof (*ptrdes));
  ptrdes->data = NULL;
  ptrdes->fd.type = NULL;
  ptrdes->fd.name.str = NULL;
  ptrdes->fd.name.hash_value = 0;
  ptrdes->fd.size = 0;
  ptrdes->fd.offset = 0;
  ptrdes->fd.mr_type = MR_TYPE_VOID;
  ptrdes->fd.mr_type_ext = MR_TYPE_EXT_NONE;
  ptrdes->fd.param.array_param.count = 0;
  ptrdes->fd.param.array_param.row_count = 0;
  ptrdes->fd.param.enum_value = 0;
  ptrdes->fd.meta = NULL;
  ptrdes->fd.res.ptr = NULL;
  ptrdes->fd.res_type = NULL;
  ptrdes->level = 0;
  ptrdes->idx = -1; /* NB! To be initialized in depth search in mr_save */
  ptrdes->ref_idx = -1;
  ptrdes->parent = -1;
  ptrdes->first_child = -1;
  ptrdes->last_child = -1;
  ptrdes->prev = -1;
  ptrdes->next = -1;
  ptrdes->flags.is_null = FALSE;
  ptrdes->flags.is_referenced = FALSE;
  ptrdes->flags.is_content_reference = FALSE;
  ptrdes->flags.is_opaque_data = FALSE;
  ptrdes->mr_value.value_type = MR_VT_UNKNOWN;
  ptrdes->res.ptr = NULL;
  ptrdes->res_type = NULL;
  return (ptrs->size / sizeof (ptrs->ra[0]) - 1);
}

/**
 * Setup referencies between parent and child node in serialization tree
 * @param parent index of parent node
 * @param child index of child node
 * @param ptrs resizable array with pointers descriptors
 */
void
mr_add_child (int parent, int child, mr_ra_mr_ptrdes_t * ptrs)
{
  int last_child;

  if (child < 0)
    return;

  ptrs->ra[child].parent = parent;
  if (parent < 0)
    return;

  last_child = ptrs->ra[parent].last_child;
  if (last_child < 0)
    ptrs->ra[parent].first_child = child;
  else
    {
      ptrs->ra[last_child].next = child;
      ptrs->ra[child].prev = last_child;
      ptrs->ra[child].next = -1;
    }
  ptrs->ra[parent].last_child = child;
}

void
mr_assign_int (mr_ptrdes_t * dst, mr_ptrdes_t * src)
{
  uint64_t value = 0;
  mr_type_t mr_type;

  if (MR_TYPE_VOID == src->fd.mr_type)
    mr_type = src->fd.mr_type_aux;
  else
    mr_type = src->fd.mr_type;
  
  switch (mr_type)
    {
    case MR_TYPE_VOID:
      switch (src->fd.size)
	{
	case sizeof (int8_t):
	  value = *(int8_t*)src->data;
	  break;
	case sizeof (int16_t):
	  value = *(int16_t*)src->data;
	  break;
	case sizeof (int32_t):
	  value = *(int32_t*)src->data;
	  break;
	case sizeof (int64_t):
	  value = *(int64_t*)src->data;
	  break;
	}
      break;
    case MR_TYPE_BOOL:
      value = *(bool*)src->data;
      break;
    case MR_TYPE_CHAR:
      value = *(char*)src->data;
      break;
    case MR_TYPE_UINT8:
      value = *(uint8_t*)src->data;
      break;
    case MR_TYPE_INT8:
      value = *(int8_t*)src->data;
      break;
    case MR_TYPE_UINT16:
      value = *(uint16_t*)src->data;
      break;
    case MR_TYPE_INT16:
      value = *(int16_t*)src->data;
      break;
    case MR_TYPE_UINT32:
      value = *(uint32_t*)src->data;
      break;
    case MR_TYPE_INT32:
      value = *(int32_t*)src->data;
      break;
    case MR_TYPE_UINT64:
      value = *(uint64_t*)src->data;
      break;
    case MR_TYPE_INT64:
      value = *(int64_t*)src->data;
      break;
    case MR_TYPE_BITFIELD:
      mr_save_bitfield_value (src, &value); /* get value of the bitfield */
      break;
    default:
      break;
    }
  
  if (MR_TYPE_VOID == dst->fd.mr_type)
    mr_type = dst->fd.mr_type_aux;
  else
    mr_type = dst->fd.mr_type;

  switch (mr_type)
    {
    case MR_TYPE_VOID:
      switch (dst->fd.size)
	{
	case sizeof (int8_t):
	  *(int8_t*)dst->data = value;
	  break;
	case sizeof (int16_t):
	  *(int16_t*)dst->data = value;
	  break;
	case sizeof (int32_t):
	  *(int32_t*)dst->data = value;
	  break;
	case sizeof (int64_t):
	  *(int64_t*)dst->data = value;
	  break;
	}
      break;
    case MR_TYPE_BOOL:
      *(bool*)dst->data = value;
      break;
    case MR_TYPE_CHAR:
      *(char*)dst->data = value;
      break;
    case MR_TYPE_UINT8:
      *(uint8_t*)dst->data = value;
      break;
    case MR_TYPE_INT8:
      *(int8_t*)dst->data = value;
      break;
    case MR_TYPE_UINT16:
      *(uint16_t*)dst->data = value;
      break;
    case MR_TYPE_INT16:
      *(int16_t*)dst->data = value;
      break;
    case MR_TYPE_UINT32:
      *(uint32_t*)dst->data = value;
      break;
    case MR_TYPE_INT32:
      *(int32_t*)dst->data = value;
      break;
    case MR_TYPE_UINT64:
      *(uint64_t*)dst->data = value;
      break;
    case MR_TYPE_INT64:
      *(int64_t*)dst->data = value;
      break;
    case MR_TYPE_BITFIELD:
      mr_load_bitfield_value (dst, &value); /* set value of the bitfield */
      break;
    default:
      break;
    }
}

/**
 * Checks that string is a valid field name [_a-zA-A][_a-zA-Z0-9]*
 * @param name union meta field
 */
bool
mr_is_valid_field_name (char * name)
{
  if (NULL == name)
    return (FALSE);
  if (!isalpha (*name) && ('_' != *name))
    return (FALSE);
  for (++name; *name; ++name)
    if (!isalnum (*name) && ('_' != *name))
      return (FALSE);
  return (TRUE);
}

static mr_fd_t *
mr_get_fd_by_offset (mr_td_t * tdp, typeof (((mr_ptr_t*)0)->offset) offset)
{
  int i;
  for (i = tdp->fields_size / sizeof (tdp->fields[0]) - 1; i >= 0; --i)
    if (tdp->fields[i].fdp->offset == offset)
      return (tdp->fields[i].fdp);
  return (NULL);
}

void
mr_pointer_get_size_ptrdes (mr_ptrdes_t * ptrdes, int idx, mr_ra_mr_ptrdes_t * ptrs)
{
  char * name = NULL;
  memset (ptrdes, 0, sizeof (*ptrdes));
  
  if ((NULL != ptrs->ra[idx].fd.res_type) && (0 == strcmp ("char", ptrs->ra[idx].fd.res_type)) &&
      mr_is_valid_field_name (ptrs->ra[idx].fd.res.ptr))
    name = ptrs->ra[idx].fd.res.ptr;

  if ((NULL != ptrs->ra[idx].fd.res_type) && (0 == strcmp ("string", ptrs->ra[idx].fd.res_type)) &&
      mr_is_valid_field_name (ptrs->ra[idx].fd.res.ptr))
    name = ptrs->ra[idx].fd.res.ptr;

  if ((NULL != ptrs->ra[idx].fd.res_type) && (0 == strcmp ("offset", ptrs->ra[idx].fd.res_type)))
    name = "";
  
  if (name != NULL)
    {
      int parent;
      /* traverse through parents up to first structure */
      for (parent = ptrs->ra[idx].parent; parent >= 0; parent = ptrs->ra[parent].parent)
	if ((MR_TYPE_EXT_NONE == ptrs->ra[parent].fd.mr_type_ext) &&
	    (MR_TYPE_STRUCT == ptrs->ra[parent].fd.mr_type))
	  break;
      
      if (parent >= 0)
	{
	  mr_fd_t * parent_fdp;
	  mr_td_t * parent_tdp = mr_get_td_by_name (ptrs->ra[parent].fd.type);
  
	  if (NULL == parent_tdp)
	    return;

	  /* lookup for a size field in this parent */
	  if (0 == strcmp ("", name))
	    parent_fdp = mr_get_fd_by_offset (parent_tdp, ptrs->ra[idx].fd.res.offset);
	  else
	    parent_fdp = mr_get_fd_by_name (parent_tdp, name);
	      
	  if (NULL == parent_fdp)
	    return;

	  ptrdes->fd = *parent_fdp;
	  ptrdes->data = (char*)ptrs->ra[parent].data + parent_fdp->offset; /* get an address of size field */
	}
    }
}

void
mr_pointer_set_size (int idx, mr_ra_mr_ptrdes_t * ptrs)
{
  mr_ptrdes_t src, dst;
  mr_pointer_get_size_ptrdes (&dst, idx, ptrs);
      
  if (dst.data != NULL)
    {
      src.data = &ptrs->ra[idx].size;
      src.fd.mr_type = MR_TYPE_DETECT (typeof (ptrs->ra[idx].size));
      mr_assign_int (&dst, &src);
    }
}

/**
 * Recursively free all allocated memory. Needs to be done from bottom to top.
 * @param ptrs resizable array with serialized data
 * @return status
 */
mr_status_t
mr_free_recursively (mr_ra_mr_ptrdes_t ptrs)
{
  int i;

  if (NULL == ptrs.ra)
    return (MR_FAILURE);

  for (i = ptrs.size / sizeof (ptrs.ra[0]) - 1; i >= 0; --i)
    {
      ptrs.ra[i].res.ptr = NULL;
      if ((ptrs.ra[i].ref_idx < 0) && (ptrs.ra[i].idx >= 0))
	if ((MR_TYPE_EXT_POINTER == ptrs.ra[i].fd.mr_type_ext) ||
	    ((MR_TYPE_EXT_NONE == ptrs.ra[i].fd.mr_type_ext) &&
	     (MR_TYPE_STRING == ptrs.ra[i].fd.mr_type)))
	  ptrs.ra[i].res.ptr = *(void**)ptrs.ra[i].data;
    }

  for (i = ptrs.size / sizeof (ptrs.ra[0]) - 1; i >= 0; --i)
    if (ptrs.ra[i].res.ptr)
      MR_FREE (ptrs.ra[i].res.ptr);

  MR_FREE (ptrs.ra);
  return (MR_SUCCESS);
}

static mr_status_t
calc_relative_addr (mr_ra_mr_ptrdes_t * ptrs, int idx, void * context)
{
  /* is new address is not set yet, then it could be calculated as relative address from the parent node */
  if (NULL == ptrs->ra[idx].res.ptr)
    {
      int parent = ptrs->ra[idx].parent;
      ptrs->ra[idx].res.ptr = &((char*)ptrs->ra[parent].res.ptr)[ptrs->ra[idx].data - ptrs->ra[parent].data];
    }
  return (MR_SUCCESS);
}

/**
 * Recursively copy
 * @param ptrs resizable array with serialized data
 * @return status
 */
mr_status_t
mr_copy_recursively (mr_ra_mr_ptrdes_t ptrs, void * dst)
{
  int i;

  if ((NULL == ptrs.ra) || (NULL == dst))
    return (MR_FAILURE);

  for (i = ptrs.size / sizeof (ptrs.ra[0]) - 1; i > 0; --i)
    ptrs.ra[i].res.ptr = NULL;

  /* NB index 0 is excluded */
  for (i = ptrs.size / sizeof (ptrs.ra[0]) - 1; i > 0; --i)
    /*
      skip nodes that are not in final save graph (ptrs.ra[i].idx >= 0)
      nodes are references on other nodes (ptrs.ra[i].ref_idx < 0)
    */
    if ((ptrs.ra[i].idx >= 0) && (ptrs.ra[i].ref_idx < 0))
      {
	if (TRUE == ptrs.ra[i].flags.is_null)
	  {
	    /* explicitly set to NULL pointers that were attributed as is_null */
	    *(void**)ptrs.ra[i].data = NULL;
	    continue;
	  }
	
	switch (ptrs.ra[i].fd.mr_type_ext)
	  {
	  case MR_TYPE_EXT_NONE:
	    if (MR_TYPE_STRING != ptrs.ra[i].fd.mr_type)
	      break;
	    /* calc string length for further malloc */
	    ptrs.ra[i].fd.size = strlen (*(void**)ptrs.ra[i].data) + 1;
	    /* string node should be followed with unlinked char array node */
	    if ((ptrs.ra[i].first_child >= 0) ||
		(ptrs.ra[i + 1].parent != i) ||
		(*(void**)ptrs.ra[i].data != ptrs.ra[i + 1].data))
	      {
		MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_STRING_SAVE_DATA);
		return (MR_FAILURE);
	      }
	    /* link it back. we need to save address of allocated memory into this node */
	    ptrs.ra[i].first_child = ptrs.ra[i].last_child = i + 1;

	  case MR_TYPE_EXT_POINTER:
	    {
	      int idx;
	      char * copy;
	      ssize_t size = ptrs.ra[i].fd.size; /* default allocation size */

	      if (ptrs.ra[i].first_child < 0)
		{
		  MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_POINTER_NODE_CHILD_MISSING,
			      ptrs.ra[i].fd.type, ptrs.ra[i].fd.name.str);
		  return (MR_FAILURE);
		}
	    
	      if (MR_TYPE_EXT_POINTER == ptrs.ra[i].fd.mr_type_ext)
		{
		  size = ptrs.ra[i].size;
		  if ((MR_TYPE_CHAR_ARRAY == ptrs.ra[i].fd.mr_type) && (0 == ptrs.ra[i].fd.size))
		    size = strlen (*(void**)ptrs.ra[i].data) + 1;
		}
	    
	      if (size < 0)
		{
		  MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_WRONG_SIZE_FOR_DYNAMIC_ARRAY, size);
		  return (MR_FAILURE);
		}
	    
	      copy = MR_MALLOC (size);
	      if (NULL == copy)
		{
		  MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
		  return (MR_FAILURE);
		}

	      /* copy data from source */
	      memcpy (copy, *(void**)ptrs.ra[i].data, size);
	      /* go thru all childs and calculate their addresses in newly allocated chunk */
	      for (idx = ptrs.ra[i].first_child; idx >= 0; idx = ptrs.ra[idx].next)
		ptrs.ra[idx].res.ptr = &copy[(char*)ptrs.ra[idx].data - *(char**)ptrs.ra[i].data];
	    }
	    break;
	  default:
	    break;
	  }
      }
  /* copy first level struct */
  memcpy (dst, ptrs.ra[0].data, ptrs.ra[0].fd.size);
  ptrs.ra[0].res.ptr = dst;

  /* depth search thru the graph and calculate new addresses for all nodes */
  mr_ptrs_ds (&ptrs, calc_relative_addr, NULL);

  /* now we should update pointers in a copy */
  for (i = ptrs.size / sizeof (ptrs.ra[0]) - 1; i > 0; --i)
    if ((ptrs.ra[i].idx >= 0) && (TRUE != ptrs.ra[i].flags.is_null)) /* skip NULL and invalid nodes */
      switch (ptrs.ra[i].fd.mr_type_ext)
	{
	case MR_TYPE_EXT_NONE:
	  if (MR_TYPE_STRING != ptrs.ra[i].fd.mr_type)
	    break;
	case MR_TYPE_EXT_POINTER:
	  {
	    int ptr_idx;
	    /* get index of the node that is referenced by the pointer */
	    if (ptrs.ra[i].ref_idx < 0)
	      ptr_idx = ptrs.ra[i].first_child;
	    else
	      ptr_idx = ptrs.ra[i].flags.is_content_reference ? ptrs.ra[ptrs.ra[i].ref_idx].first_child : ptrs.ra[i].ref_idx;
	    /* update pointer in the copy */
	    *(void**)ptrs.ra[i].res.ptr = ptrs.ra[ptr_idx].res.ptr;
	  }
	  break;
	default:
	  break;
	}
  MR_FREE (ptrs.ra);
  return (MR_SUCCESS);
}

/**
 * Default hash function.
 * @param str a pointer on null terminated string
 * @return Hash function value.
 */
mr_hash_value_t
mr_hash_str (char * str)
{
  mr_hash_value_t hash_value = 0;
  if (NULL == str)
    return (hash_value);
  while (*str)
    hash_value = (hash_value + (unsigned char)*str++) * 0xDeadBeef;
  return (hash_value);
}

mr_hash_value_t
mr_hashed_string_get_hash (mr_ptr_t x, const void * context)
{
  mr_hashed_string_t * x_ = x.ptr;
  if (0 == x_->hash_value)
    x_->hash_value = mr_hash_str (x_->str);
  return (x_->hash_value);
}

/**
 * Comparator for mr_hashed_string_t
 * @param x pointer on one mr_hashed_string_t
 * @param y pointer on another mr_hashed_string_t
 * @return comparation sign
 */
int
mr_hashed_string_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const mr_hashed_string_t * x_ = x.ptr;
  const mr_hashed_string_t * y_ = y.ptr;
  mr_hash_value_t x_hash_value = mr_hashed_string_get_hash ((mr_ptr_t)x, context);
  mr_hash_value_t y_hash_value = mr_hashed_string_get_hash ((mr_ptr_t)y, context);
  int diff = (x_hash_value > y_hash_value) - (x_hash_value < y_hash_value);
  if (diff)
    return (diff);
  return (strcmp (x_->str, y_->str));
}

mr_hash_value_t
mr_fd_name_get_hash (mr_ptr_t x, const void * context)
{
  mr_fd_t * x_ = x.ptr;
  return (mr_hashed_string_get_hash (&x_->name, context));
}

/**
 * Comparator for mr_fd_t
 * @param x pointer on one mr_fd_t
 * @param y pointer on another mr_fd_t
 * @return comparation sign
 */
int
mr_fd_name_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const mr_fd_t * x_ = x.ptr;
  const mr_fd_t * y_ = y.ptr;
  return (mr_hashed_string_cmp ((void*)&x_->name, (void*)&y_->name, context));
}

mr_hash_value_t
mr_td_name_get_hash (mr_ptr_t x, const void * context)
{
  mr_td_t * x_ = x.ptr;
  return (mr_hashed_string_get_hash (&x_->type, context));
}

/**
 * Comparator for mr_td_t
 * @param a pointer on one mr_td_t
 * @param b pointer on another mr_td_t
 * @return comparation sign
 */
int
mr_td_name_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const mr_td_t * x_ = x.ptr;
  const mr_td_t * y_ = y.ptr;
  return (mr_hashed_string_cmp ((void*)&x_->type, (void*)&y_->type, context));
}

/**
 * Type descriptor lookup function. Lookup by type name.
 * @param type stringified type name
 * @return pointer on type descriptor
 */
mr_td_t *
mr_get_td_by_name (char * type)
{
  mr_td_t td = { .type = { .str = type, .hash_value = mr_hash_str (type), } };
  mr_ptr_t * result = mr_ic_find (&mr_conf.lookup_by_name, &td, NULL);
  return (result ? result->ptr : NULL);
}

/**
 * Preprocessign of a new type. Anonymous unions should be extracted into new independant types.
 * @param tdp pointer on a new type descriptor
 * @return status
 */
static mr_status_t
mr_anon_unions_extract (mr_td_t * tdp)
{
  int count = tdp->fields_size / sizeof (tdp->fields[0]);
  int i, j;

  for (i = 0; i < count; ++i)
    {
      mr_fd_t * fdp = tdp->fields[i].fdp;
      if ((MR_TYPE_ANON_UNION == fdp->mr_type) || (MR_TYPE_NAMED_ANON_UNION == fdp->mr_type))
	{
	  static int mr_type_anonymous_union_cnt = 0;
	  mr_td_t * tdp_ = fdp->res.ptr; /* statically allocated memory for new type descriptor */
	  mr_fd_t ** first = &tdp->fields[i + 1].fdp;
	  mr_fd_t * last;
	  int opened = 1;

	  for (j = i + 1; j < count; ++j)
	    {
	      mr_fd_t * fdp_ = tdp->fields[j].fdp;
	      if ((MR_TYPE_ANON_UNION == fdp_->mr_type) ||
		  (MR_TYPE_NAMED_ANON_UNION == fdp_->mr_type))
		++opened;
	      if (MR_TYPE_END_ANON_UNION == fdp_->mr_type)
		if (0 == --opened)
		  break;
	    }
	  if (j >= count)
	    return (MR_FAILURE);

	  {
	    int fields_count = j - i; /* additional trailing element with mr_type = MR_TYPE_TRAILING_RECORD */
	    mr_fd_t * fields[fields_count];
	    /*
	      0  1  2  3  4  5  6  7  8
	      F1 AH U1 U2 AE F2 F3 F4 T
	      i = 1
	      j = 4
	      first = 2
	      fields_count = 3
	      count = 8
	    */
	    memcpy (fields, first, fields_count * sizeof (first[0]));
	    memmove (first, &first[fields_count], (count - j) * sizeof (first[0])); /* blocks overlap possible */
	    memcpy (&first[count - j], fields, fields_count * sizeof (first[0]));

	    tdp_->size = 0;
	    for (j = 0; j < fields_count - 1; ++j)
	      {
		/* offset of union memebers may differ from offset of anonymous union place holder */
		if (fields[j]->offset != 0) /* MR_VOID and MR_END_ANON_UNION has zero offset */
		  fdp->offset = fields[j]->offset;
		fields[j]->offset = 0; /* reset offset to zero */
		if (tdp_->size < fields[j]->size)
		  tdp_->size = fields[j]->size; /* find union max size member */
	      }

	    last = tdp->fields[count].fdp;
	    last->mr_type = MR_TYPE_TRAILING_RECORD; /* trailing record */
	    tdp_->mr_type = fdp->mr_type; /* MR_TYPE_ANON_UNION or MR_TYPE_NAMED_ANON_UNION */
	    sprintf (tdp_->type.str, MR_TYPE_ANONYMOUS_UNION_TEMPLATE, mr_type_anonymous_union_cnt++);
	    tdp_->type.hash_value = mr_hash_str (tdp_->type.str);
	    tdp_->attr = fdp->meta; /* anonymous union stringified attributes are saved into metas field */
	    tdp_->meta = last->meta; /* copy meta from MR_END_ANON_UNION record */
	    tdp_->fields = &tdp->fields[count - fields_count + 1];

	    fdp->meta = last->meta; /* copy meta from MR_END_ANON_UNION record */
	    tdp->fields_size -= fields_count * sizeof (tdp->fields[0]);
	    count -= fields_count;
	    fdp->type = tdp_->type.str;
	    fdp->size = tdp_->size;
	    /* set name of anonymous union to temporary type name */
	    if ((NULL == fdp->name.str) || (0 == fdp->name.str[0]))
	      fdp->name.str = fdp->type;
	    fdp->name.hash_value = mr_hash_str (fdp->name.str);

	    if (MR_SUCCESS != mr_add_type (tdp_, NULL, NULL))
	      {
		MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_ANON_UNION_TYPE_ERROR, tdp->type.str);
		return (MR_FAILURE);
	      }
	  }
	}
    }
  return (MR_SUCCESS);
}

/**
 * Gets enum value as integer
 * @param ptrdes descriptor of the saved field
 * @return enum value
 */
int64_t
mr_get_enum_value (mr_td_t * tdp, void * data)
{
  int64_t enum_value = 0;
  /*
    GCC caluculates sizeof for the type according alignment, but initialize only effective bytes
    i.e. for typedef enum __attribute__ ((packed, aligned (sizeof (uint16_t)))) {} enum_t;
    sizeof (enum_t) == 2, but type has size only 1 byte
  */
  switch (tdp->mr_type_effective)
    {
    case MR_TYPE_UINT8:
      enum_value = *(uint8_t*)data;
      break;
    case MR_TYPE_INT8:
      enum_value = *(int8_t*)data;
      break;
    case MR_TYPE_UINT16:
      enum_value = *(uint16_t*)data;
      break;
    case MR_TYPE_INT16:
      enum_value = *(int16_t*)data;
      break;
    case MR_TYPE_UINT32:
      enum_value = *(uint32_t*)data;
      break;
    case MR_TYPE_INT32:
      enum_value = *(int32_t*)data;
      break;
    case MR_TYPE_UINT64:
      enum_value = *(uint64_t*)data;
      break;
    case MR_TYPE_INT64:
      enum_value = *(int64_t*)data;
      break;
    default:
#if __BYTE_ORDER == __LITTLE_ENDIAN
      memcpy (&enum_value, data, MR_MIN (tdp->size_effective, sizeof (enum_value)));
#else
#error Support for non little endian architectures to be implemented
#endif /*__BYTE_ORDER == __LITTLE_ENDIAN */
      break;
    }
  return (enum_value);
}

/**
 * calculate a hash value for mr_fd_t
 * @param x pointer on mr_fd_t
 * @param context pointer on a context
 * @return hash value
 */
mr_hash_value_t
mr_enumfd_get_hash (mr_ptr_t x, const void * context)
{
  mr_fd_t * fdp = x.ptr;
  return (fdp->param.enum_value);
}

/**
 * comparator for mr_fd_t sorting by enum value
 * @param x pointer on one mr_fd_t
 * @param y pointer on another mr_fd_t
 * @return comparation sign
 */
int
cmp_enums_by_value (mr_ptr_t x, mr_ptr_t y, const void * context)
{
  mr_fd_t * x_ = x.ptr;
  mr_fd_t * y_ = y.ptr;
  return ((x_->param.enum_value > y_->param.enum_value) - (x_->param.enum_value < y_->param.enum_value));
}

/**
 * New enum descriptor preprocessing. Enum literal values should be added to global lookup table and enum type descriptor should have a lookup by enum values.
 * @param tdp pointer on a new enum type descriptor
 * @return status
 */
static mr_status_t
mr_add_enum (mr_td_t * tdp)
{
  mr_ic_rarray_t mr_ic_rarray;
  int i, count = tdp->fields_size / sizeof (tdp->fields[0]);
  /*
    Enums with __attribute__((packed, aligned (XXX))) GCC generates size according alignment, but not real size which is 1 byte due to packing.
    Here we determine effective type size.
    Clang calculates size and effective size according alignment.
  */
  switch (tdp->mr_type_effective)
    {
    case MR_TYPE_INT8:
    case MR_TYPE_UINT8:
      tdp->size_effective = sizeof (uint8_t);
      break;
    case MR_TYPE_INT16:
    case MR_TYPE_UINT16:
      tdp->size_effective = sizeof (uint16_t);
      break;
    case MR_TYPE_INT32:
    case MR_TYPE_UINT32:
      tdp->size_effective = sizeof (uint32_t);
      break;
    case MR_TYPE_INT64:
    case MR_TYPE_UINT64:
      tdp->size_effective = sizeof (uint64_t);
      break;
    default:
      tdp->size_effective = tdp->size;
      break;
    }

  mr_ic_new (&tdp->lookup_by_value, mr_enumfd_get_hash, cmp_enums_by_value, "mr_fd_t", MR_IC_STATIC_DEFAULT);
  mr_ic_rarray.ra = (mr_ptr_t*)tdp->fields;
  mr_ic_rarray.size = tdp->fields_size;
  mr_ic_rarray.alloc_size = -1;
  mr_ic_index (&tdp->lookup_by_value, &mr_ic_rarray, NULL);

  for (i = 0; i < count; ++i)
    {
      /* adding to global lookup table by enum literal names */
      mr_ptr_t key = { .ptr = tdp->fields[i].fdp };
      mr_ptr_t * result = mr_ic_add (&mr_conf.enum_by_name, key, NULL);
      if (NULL == result)
	return (MR_FAILURE);
      if (result->ptr != key.ptr)
	{
	  mr_fd_t * fdp = result->ptr;
	  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_DUPLICATED_ENUMS, fdp->name.str, key.ptr);
	  return (MR_FAILURE);
	}
    }

  return (MR_SUCCESS);
}

/**
 * Get enum by value. Enums type descriptors contains red-black tree with all enums forted by value.
 *
 * @param tdp pointer on a type descriptor
 * @param value enums value
 * @return pointer on enum value descriptor (mr_fd_t*) or NULL is value was not found
 */
mr_fd_t *
mr_get_enum_by_value (mr_td_t * tdp, int64_t value)
{
  mr_fd_t fd = { .param = { .enum_value = value, }, };
  mr_ptr_t * result = mr_ic_find (&tdp->lookup_by_value, &fd, NULL);
  return (result ? result->ptr : NULL);
}

/**
 * Enum literal name lookup function.
 * @param value address for enum value to store
 * @param name literal name of enum to lookup
 * @return status
 */
mr_fd_t *
mr_get_enum_by_name (char * name)
{
  mr_fd_t fd = { .name = { .str = name, .hash_value = mr_hash_str (name), } };
  mr_ptr_t * result = mr_ic_find (&mr_conf.enum_by_name, &fd, NULL);
  return (result ? result->ptr : NULL);
}

/**
 * Type name clean up. We need to drop all key words.
 * @param fdp pointer on a field descriptor
 * @return status
 */
static mr_status_t
mr_normalize_type (mr_fd_t * fdp)
{
  static char * keywords[] =
    {
      "struct",
      "union",
      "enum",
      "const",
      "__const",
      "__const__",
      "volatile",
      "__volatile",
      "__volatile__",
      "restrict",
      "__restrict",
      "__restrict__",
    };
  static bool isdelimiter [1 << (CHAR_BIT * sizeof (uint8_t))] =
    {
      [0 ... (1 << (CHAR_BIT * sizeof (char))) - 1] = FALSE,
      [0] = TRUE,
      [(uint8_t)' '] = TRUE,
      [(uint8_t)'*'] = TRUE,
    };
  int i;
  char * ptr;
  int prev_is_space = 0;
  bool modified = FALSE;

  for (i = 0; i < sizeof (keywords) / sizeof (keywords[0]); ++i)
    {
      int length = strlen (keywords[i]);
      ptr = fdp->type;
      for (;;)
	{
	  char * found = strstr (ptr, keywords[i]);
	  if (!found)
	    break;
	  if (isdelimiter[(uint8_t)found[length]] && ((found == fdp->type) || isdelimiter[(uint8_t)found[-1]]))
	    {
	      memset (found, ' ', length); /* replaced all keywords on spaces */
	      modified = TRUE;
	    }
	  ++ptr; /* keyword might be a part of type name and we need to start search of keyword from next symbol */
	}
    }
  if (modified)
    {
      /* we need to drop all space characters */
      ptr = fdp->type;
      for (i = 0; isspace (fdp->type[i]); ++i);
      for (; fdp->type[i]; ++i)
	if (isspace (fdp->type[i]))
	  prev_is_space = !0;
	else
	  {
	    if (prev_is_space)
	      *ptr++ = ' ';
	    *ptr++ = fdp->type[i];
	    prev_is_space = 0;
	  }
      *ptr = 0;
    }
  return (MR_SUCCESS);
}

/**
 * Bitfield initialization. We need to calculate offset and shift. Width was initialized by macro.
 * @param fdp pointer on a field descriptor
 * @return status
 */
static mr_status_t
mr_init_bitfield (mr_fd_t * fdp)
{
  int i, j;
  if ((NULL == fdp->param.bitfield_param.bitfield) ||
      (0 == fdp->param.bitfield_param.size))
    return (MR_SUCCESS);

  for (i = 0; i < fdp->param.bitfield_param.size; ++i)
    if (fdp->param.bitfield_param.bitfield[i])
      break;
  /* if bitmask is clear then there is no need to initialize anything */
  if (!fdp->param.bitfield_param.bitfield[i])
    return (MR_SUCCESS);

  fdp->offset = i;
  for (i = 0; i < CHAR_BIT; ++i)
    if (fdp->param.bitfield_param.bitfield[fdp->offset] & (1 << i))
      break;
  fdp->param.bitfield_param.shift = i;
  fdp->param.bitfield_param.width = 0;
  for (j = fdp->offset; j < fdp->param.bitfield_param.size; ++j)
    {
      for ( ; i < CHAR_BIT; ++i)
	if (fdp->param.bitfield_param.bitfield[j] & (1 << i))
	  ++fdp->param.bitfield_param.width;
	else
	  break;
      if (i < CHAR_BIT)
	break;
      i = 0;
    }
  return (MR_SUCCESS);
}

/**
 * New type descriptor preprocessing. Check fields names duplication, nornalize types name, initialize bitfields. Called once for each type.
 * @param tdp pointer on a type descriptor
 * @return status
 */
static mr_status_t
mr_check_fields (mr_td_t * tdp)
{
  int i, count = tdp->fields_size / sizeof (tdp->fields[0]);
  for (i = 0; i < count; ++i)
    {
      mr_fd_t * fdp = tdp->fields[i].fdp;
      /*
	Check names of the fileds.
	MR_VOID definitions may contain brackets (for arrays) or braces (for function pointers) or collon (for bitfields).
      */
      char * name = fdp->name.str;
      if (name)
	{
	  for (; isalnum (*name) || (*name == '_'); ++name); /* skip valid characters */
	  if (*name) /* strings with field names might be in read-only memory. For MR_VOID names are saved in writable memory. */
	    *name = 0; /* truncate on first invalid charecter */
	}
      mr_normalize_type (fdp);
      if (MR_TYPE_BITFIELD == fdp->mr_type)
	mr_init_bitfield (fdp);
    }
  return (MR_SUCCESS);
}

/**
 * Initialize AUTO fields. Detect types, size, pointers etc.
 * @param fdp pointer on a field descriptor
 * @return status
 */
static mr_status_t
mr_auto_field_detect (mr_fd_t * fdp)
{
  static size_t types_sizes[] =
    {
      [0 ... MR_TYPE_LAST - 1] = 0,
      [MR_TYPE_NONE] = 0,
      [MR_TYPE_VOID] = sizeof (void),
      [MR_TYPE_BOOL] = sizeof (bool),
      [MR_TYPE_INT8] = sizeof (int8_t),
      [MR_TYPE_UINT8] = sizeof (uint8_t),
      [MR_TYPE_INT16] = sizeof (int16_t),
      [MR_TYPE_UINT16] = sizeof (uint16_t),
      [MR_TYPE_INT32] = sizeof (int32_t),
      [MR_TYPE_UINT32] = sizeof (uint32_t),
      [MR_TYPE_INT64] = sizeof (int64_t),
      [MR_TYPE_UINT64] = sizeof (uint64_t),
      [MR_TYPE_FLOAT] = sizeof (float),
      [MR_TYPE_COMPLEX_FLOAT] = sizeof (complex float),
      [MR_TYPE_DOUBLE] = sizeof (double),
      [MR_TYPE_COMPLEX_DOUBLE] = sizeof (complex double),
      [MR_TYPE_LONG_DOUBLE] = sizeof (long double),
      [MR_TYPE_COMPLEX_LONG_DOUBLE] = sizeof (complex long double),
      [MR_TYPE_CHAR] = sizeof (char),
      [MR_TYPE_CHAR_ARRAY] = sizeof (char),
      [MR_TYPE_STRING] = sizeof (char*),
      [MR_TYPE_STRUCT] = sizeof (void),
      [MR_TYPE_UNION] = sizeof (void),
      [MR_TYPE_ANON_UNION] = sizeof (void),
      [MR_TYPE_NAMED_ANON_UNION] = sizeof (void),
    };

  mr_td_t * tdp = mr_get_td_by_name (fdp->type);
  /* check if type is in registery */
  if (tdp)
    {
      fdp->mr_type = tdp->mr_type;
      fdp->size = tdp->size; /* size of forward pointers could be resolved only at the time of type registration */
    }
  else if (MR_TYPE_EXT_NONE == fdp->mr_type_ext)
    {
      /* auto detect pointers */
      char * end = strchr (fdp->type, 0) - 1;
      if ('*' == *end)
	{
	  /* remove whitespaces before * */
	  while (isspace (end[-1]))
	    --end;
	  *end = 0; /* trancate type name */
	  fdp->mr_type_ext = MR_TYPE_EXT_POINTER;
	  fdp->mr_type = fdp->mr_type_aux;
	  fdp->size = types_sizes[fdp->mr_type];
	  /* autodetect structures and enums */
	  switch (fdp->mr_type)
	    {
	    case MR_TYPE_NONE:
	    case MR_TYPE_INT8:
	    case MR_TYPE_UINT8:
	    case MR_TYPE_INT16:
	    case MR_TYPE_UINT16:
	    case MR_TYPE_INT32:
	    case MR_TYPE_UINT32:
	    case MR_TYPE_INT64:
	    case MR_TYPE_UINT64:
	    case MR_TYPE_CHAR_ARRAY: /* NB! need to detect size of char array */
	      tdp = mr_get_td_by_name (fdp->type);
	      if (tdp)
		{
		  fdp->mr_type = tdp->mr_type;
		  fdp->size = tdp->size;
		}
	      break;

	    default:
	      break;
	    }
	}
    }
  return (MR_SUCCESS);
}

/**
 * Initialize fields that are pointers on functions. Detects types of arguments.
 * @param fdp pointer on a field descriptor
 * @return status
 */
static mr_status_t
mr_func_field_detect (mr_fd_t * fdp)
{
  int i;
  for (i = 0; fdp->param.func_param.args[i].mr_type != MR_TYPE_TRAILING_RECORD; ++i)
    {
      mr_normalize_type (&fdp->param.func_param.args[i]);
      switch (fdp->param.func_param.args[i].mr_type)
	{
	case MR_TYPE_NONE:
	case MR_TYPE_INT8:
	case MR_TYPE_UINT8:
	case MR_TYPE_INT16:
	case MR_TYPE_UINT16:
	case MR_TYPE_INT32:
	case MR_TYPE_UINT32:
	case MR_TYPE_INT64:
	case MR_TYPE_UINT64:
	  mr_auto_field_detect (&fdp->param.func_param.args[i]);
	  break;
	default:
	  break;
	}
    }
  fdp->param.func_param.size = i * sizeof (fdp->param.func_param.args[0]);
  return (MR_SUCCESS);
}

static mr_status_t
mr_fd_detect_field_type (mr_fd_t * fdp)
{
  mr_td_t * tdp;
  switch (fdp->mr_type)
    {
      /* Enum detection */
    case MR_TYPE_INT8:
    case MR_TYPE_UINT8:
    case MR_TYPE_INT16:
    case MR_TYPE_UINT16:
    case MR_TYPE_INT32:
    case MR_TYPE_UINT32:
    case MR_TYPE_INT64:
    case MR_TYPE_UINT64:
      tdp = mr_get_td_by_name (fdp->type);
      if (tdp)
	fdp->mr_type = tdp->mr_type;
      break;

    case MR_TYPE_BITFIELD:
      tdp = mr_get_td_by_name (fdp->type);
      if (tdp)
	fdp->mr_type_aux = tdp->mr_type;
      break;

      /*
	pointer on structure refers to forward declarations and can't calculate type size at compile time.
      */
    case MR_TYPE_STRUCT:
    case MR_TYPE_CHAR_ARRAY:
      if (MR_TYPE_EXT_POINTER == fdp->mr_type_ext)
	{
	  tdp = mr_get_td_by_name (fdp->type);
	  if (tdp)
	    fdp->size = tdp->size;
	}
      break;

    case MR_TYPE_NONE: /* MR_AUTO type resolution */
      mr_auto_field_detect (fdp);
      break;

    case MR_TYPE_FUNC:
      fdp->size = sizeof (void*);
      mr_func_field_detect (fdp);
      break;

    default:
      break;
    }
  return (MR_SUCCESS);
}

static mr_status_t
mr_fd_detect_res_size (mr_fd_t * fdp)
{
  if ((0 == fdp->MR_SIZE) && (fdp->res_type != NULL))
    {
      mr_td_t * res_tdp = mr_get_td_by_name (fdp->res_type);
      if (res_tdp != NULL)
	fdp->MR_SIZE = res_tdp->size;
    }
  return (MR_SUCCESS);
}

/**
 * Initialize fields descriptors. Everytnig that was not properly initialized in macro.
 * @param tdp pointer on a type descriptor
 * @param args auxiliary arguments
 * @return status
 */
static mr_status_t
mr_detect_fields_types (mr_td_t * tdp)
{
  int i, count = tdp->fields_size / sizeof (tdp->fields[0]);
  for (i = 0; i < count; ++i)
    {
      mr_fd_detect_field_type (tdp->fields[i].fdp);
      mr_fd_detect_res_size (tdp->fields[i].fdp);
    }
  return (MR_SUCCESS);
}

/**
 * Lookup field descriptor by field name
 * @param tdp a pointer on a type descriptor
 * @param name name of the field
 * @return pointer on field descriptor or NULL
 */
mr_fd_t *
mr_get_fd_by_name (mr_td_t * tdp, char * name)
{
  mr_fd_t fd = { .name = { .str = name, .hash_value = mr_hash_str (name), } };
  mr_ptr_t * result = mr_ic_find (&tdp->lookup_by_name, &fd, NULL);
  return (result ? result->ptr : NULL);
}

/**
 * Add type to union mr_void_ptr_t.
 * @param tdp a pointer on statically initialized type descriptor
 * @return status
 */
static mr_status_t
mr_register_type_pointer (mr_td_t * tdp)
{
  mr_fd_t * fdp;
  mr_td_t * union_tdp = mr_get_td_by_name ("mr_ptr_t");
  /* check that mr_ptr_t is already a registered type */
  if (NULL == union_tdp)
    return (MR_FAILURE);
  /* check that requested type is already registered */
  if (NULL != mr_get_fd_by_name (union_tdp, tdp->type.str))
    return (MR_SUCCESS);

  /* statically allocated trailing record is used for field descriptor */
  fdp = tdp->fields[tdp->fields_size / sizeof (tdp->fields[0])].fdp;
  if (NULL == fdp)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
      return (MR_FAILURE);
    }

  *fdp = *union_tdp->fields[0].fdp;
  fdp->type = tdp->type.str;
  fdp->name = tdp->type;
  fdp->size = tdp->size;
  fdp->offset = 0;
  fdp->mr_type = tdp->mr_type;
  fdp->mr_type_aux = MR_TYPE_VOID;
  fdp->mr_type_ext = MR_TYPE_EXT_POINTER;
  return ((NULL == mr_ic_add (&union_tdp->lookup_by_name, fdp, NULL)) ? MR_SUCCESS : MR_FAILURE);
}

/**
 * Add type description into repository
 * @param tdp a pointer on statically initialized type descriptor
 * @param meta meta info for the type
 * @param ... auxiliary void pointer
 * @return status
 */
mr_status_t
mr_add_type (mr_td_t * tdp, char * meta, ...)
{
  mr_ic_rarray_t mr_ic_rarray;
  va_list args;
  void * res;
  int count;

  if (NULL == tdp)
    return (MR_FAILURE); /* assert */
  /* check whether this type is already in the list */
  if (mr_get_td_by_name (tdp->type.str))
    return (MR_SUCCESS); /* this type is already registered */

  va_start (args, meta);
  res = va_arg (args, void*);
  va_end (args);

  for (count = 0; ; ++count)
    {
      mr_fd_t * fdp = tdp->fields[count].fdp;
      if (NULL == fdp)
	{
	  MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
	  return (MR_FAILURE);
	}
      if (MR_TYPE_TRAILING_RECORD == fdp->mr_type)
	break;
    }
  tdp->fields_size = count * sizeof (tdp->fields[0]);

  if ((NULL != meta) && meta[0])
    tdp->meta = meta;

  if (NULL != res)
    tdp->res.ptr = res;

  if (MR_SUCCESS != mr_anon_unions_extract (tdp)) /* important to extract unions before building index over fields */
    return (MR_FAILURE);

  mr_check_fields (tdp);
  mr_ic_new (&tdp->lookup_by_name, mr_fd_name_get_hash, mr_fd_name_cmp, "mr_fd_t", MR_IC_STATIC_DEFAULT);
  mr_ic_rarray.ra = (mr_ptr_t*)tdp->fields;
  mr_ic_rarray.size = tdp->fields_size;
  mr_ic_rarray.alloc_size = -1;
  mr_ic_index (&tdp->lookup_by_name, &mr_ic_rarray, NULL);

  if (NULL == mr_conf.enum_by_name.find)
    mr_ic_new (&mr_conf.enum_by_name, mr_fd_name_get_hash, mr_fd_name_cmp, "mr_fd_t", MR_IC_STATIC_DEFAULT);

  if (NULL == mr_conf.lookup_by_name.find)
    mr_ic_new (&mr_conf.lookup_by_name, mr_td_name_get_hash, mr_td_name_cmp, "mr_td_t", MR_IC_STATIC_DEFAULT);

  if (NULL == mr_ic_add (&mr_conf.lookup_by_name, tdp, NULL))
    return (MR_FAILURE);

  if (MR_TYPE_ENUM == tdp->mr_type)
    mr_add_enum (tdp);

  return (MR_SUCCESS);
}

static mr_status_t
mr_td_detect_res_size (mr_td_t * tdp)
{
  if ((0 == tdp->MR_SIZE) && (tdp->res_type != NULL))
    {
      mr_td_t * res_tdp = mr_get_td_by_name (tdp->res_type);
      if (res_tdp != NULL)
	tdp->MR_SIZE = res_tdp->size;
    }
  return (MR_SUCCESS);
}

static mr_status_t
mr_conf_init_visitor (mr_ptr_t key, const void * context)
{
  mr_td_t * tdp = key.ptr;
  mr_detect_fields_types (tdp);
  mr_register_type_pointer (tdp);
  mr_td_detect_res_size (tdp);
  return (MR_SUCCESS);
}

static void
mr_conf_init ()
{
  static bool initialized = FALSE;
  if (!initialized)
    {
      mr_ic_foreach (&mr_conf.lookup_by_name, mr_conf_init_visitor, NULL);
      initialized = TRUE;
    }
}

/**
 * Helper function for serialization macroses. Detects mr_type for enums, structures and unions.
 * Enums are detected at compile time as integers, and structures & unions as MR_TYPE_NONE
 *
 * @param fdp pointer on field descriptor
 */
void
mr_detect_type (mr_fd_t * fdp)
{
  mr_td_t * tdp;

  mr_conf_init ();

  switch (fdp->mr_type)
    {
    case MR_TYPE_UINT8:
    case MR_TYPE_INT8:
    case MR_TYPE_UINT16:
    case MR_TYPE_INT16:
    case MR_TYPE_UINT32:
    case MR_TYPE_INT32:
    case MR_TYPE_UINT64:
    case MR_TYPE_INT64:
    case MR_TYPE_NONE:
      /* we need to detect only enums, structs and unions. string_t is declared as MR_TYPE_CHAR_ARRAY, but detected as MR_TYPE_STRING */
      tdp = mr_get_td_by_name (fdp->type);
      if (tdp)
	{
	  fdp->mr_type = tdp->mr_type;
	  fdp->size = tdp->size;
	}
      break;
    default:
      break;
    }
}

/**
 * Read into newly allocated string one xml object.
 * @param fd file descriptor
 * @return Newly allocated string with xml or NULL in case of any errors
 */
char *
mr_read_xml_doc (FILE * fd)
{
  int size, max_size;
  char * str;
  char c_ = 0;
  int opened_tags = 0;
  int tags_to_close = -1;
  int count = 2;

  max_size = 2 * 1024; /* initial string size */
  str = (char*)MR_MALLOC (max_size);
  if (NULL == str)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (NULL);
    }
  size = -1;

  for (;;)
    {
      int c = fgetc (fd);
      if ((c == EOF) || (c == 0))
	{
	  MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_END);
	  MR_FREE (str);
	  return (NULL);
	}

      str[++size] = c;
      if (size == max_size - 1)
	{
	  void * str_;
	  max_size <<= 1; /* double input bufer size */
	  str_ = MR_REALLOC (str, max_size);
	  if (NULL == str_)
	    {
	      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	      MR_FREE (str);
	      return (NULL);
	    }
	  str = (char*) str_;
	}

      if ((0 == opened_tags) && !(('<' == c) || isspace (c)))
	{
	  MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_DATA);
	  MR_FREE (str);
	  return (NULL);
	}

      if ('<' == c)
	opened_tags += 2;
      if (('/' == c_) && ('>' == c))
	tags_to_close = -2;
      if (('?' == c_) && ('>' == c))
	tags_to_close = -2;
      if (('<' == c_) && ('/' == c))
	tags_to_close = -3;
      if ('>' == c)
	{
	  opened_tags += tags_to_close;
	  tags_to_close = -1;
	  if (opened_tags < 0)
	    {
	      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNBALANCED_TAGS);
	      MR_FREE (str);
	      return (NULL);
	    }
	  if (0 == opened_tags)
	    if (0 == --count)
	      {
		str[++size] = 0;
		return (str);
	      }
	}
      if (!isspace (c))
	c_ = c;
    }
}
