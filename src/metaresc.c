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
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <limits.h>

#include <metaresc.h>
#include <mr_tsearch.h>
#include <mr_save.h>
#include <mr_ic.h>
#include <mr_stringify.h>

#define MR_MODE DESC /* we'll need descriptors of our own types */
#include <mr_protos.h>

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

#undef MR_MODE
TYPEDEF_STRUCT (struct_mr_rarray_t, (mr_rarray_t, ra));
TYPEDEF_STRUCT (mr_ra_void_t, RARRAY (void, ra));

#undef MR_COMPARE_FIELDS_EXT
#define MR_COMPARE_FIELDS_EXT(TYPE1, NAME1, TYPE2, NAME2) (!__builtin_types_compatible_p (typeof (((TYPE1*)NULL)->NAME1), typeof (((TYPE2*)NULL)->NAME2)))

/*
  if this assert fails then mr_rarray_t definition in mr_protos.h differs from MR_RARRAY_PROTO define in metaresc.h
 */
MR_COMPILETIME_ASSERT (MR_COMPARE_COMPAUND_TYPES (struct_mr_rarray_t, mr_ra_void_t, ra.data, ra.size, ra.alloc_size, ra.ext, ra.ptr_type));

static int
mr_td_visitor (mr_ptr_t key, const void * context)
{
  mr_td_t * tdp = key.ptr;
  mr_ic_free (&tdp->lookup_by_name, NULL);
  mr_ic_free (&tdp->lookup_by_value, NULL);
  return (EXIT_SUCCESS);
}

/**
 * Memory cleanp handler.
 */
static void __attribute__((destructor))
mr_cleanup (void)
{
  mr_ic_foreach (&mr_conf.lookup_by_name, mr_td_visitor, NULL);
  mr_ic_free (&mr_conf.enum_by_name, NULL);
  mr_ic_free (&mr_conf.lookup_by_name, NULL);
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
  static int messages_inited = 0;
  const char * format = "Unknown MR_MESSAGE_ID.";
  char * message = NULL;

  if (!messages_inited)
    {
      mr_td_t * tdp = mr_get_td_by_name ("mr_message_id_t");
      if (tdp)
	{
	  int i;
	  for (i = 0; MR_TYPE_ENUM_VALUE == tdp->fields.data[i].fdp->mr_type; ++i)
	    messages[tdp->fields.data[i].fdp->param.enum_value] = tdp->fields.data[i].fdp->comment;
	  messages_inited = !0;
	}
    }

  if ((message_id >= 0) && (message_id <= sizeof (messages) / sizeof (messages[0])) && messages[message_id])
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

      if ((log_level >= 0) && (log_level <= sizeof (log_level_str) / sizeof (log_level_str[0])) && log_level_str[log_level])
	log_level_str_ = log_level_str[log_level];

      message = mr_message_format (message_id, args);
      if (message)
	{
	  fprintf (stderr, "%s: in %s %s() line %d: %s\n", log_level_str_, file_name, func_name, line, message);
	  fflush (stderr);
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
 * @return status EXIT_SUCCESS or EXIT_FAILURE
 */
int
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
  return (EXIT_SUCCESS);
}

/**
 * Saves bit-field into memory
 * @param ptrdes pointer descriptor
 * @param value pointer on a memory for a bit-field store
 * @return status EXIT_SUCCESS or EXIT_FAILURE
 */
int
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
  return (EXIT_SUCCESS);
}

/**
 * Rarray memory allocation/reallocation
 * @param rarray a pointer on resizable array
 * @param size size of array elements
 * @return Pointer on a new element of rarray
 */
void *
mr_rarray_append (mr_rarray_t * rarray, int size)
{
  if (NULL == rarray->data)
    {
      rarray->alloc_size = rarray->size = 0;
      rarray->data = MR_MALLOC (size);
      if (NULL == rarray->data)
	MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      else
	{
	  memset (rarray->data, 0, size);
	  rarray->alloc_size = rarray->size = size;
	}
      return (rarray->data);
    }    

  rarray->size += size;
  if (rarray->size > rarray->alloc_size)
    {
      float mas = mr_conf.mr_mem.mem_alloc_strategy;
      int alloc_size;
      void * data;
      if (mas < 1)
	mas = 1;
      if (mas > 2)
	mas = 2;
      alloc_size = (((int)((rarray->alloc_size + 1) * mas) + size - 1) / size) * size;
      if (rarray->size > alloc_size)
	alloc_size = rarray->size;
      data = MR_REALLOC (rarray->data, alloc_size);
      if (NULL == data)
	{
	  MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	  return (NULL);
	}
      rarray->alloc_size = alloc_size;
      rarray->data = data;
    }
  return (&((char*)rarray->data)[rarray->size - size]);
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
      if (mr_ra_str->data)
	MR_FREE (mr_ra_str->data);
      mr_ra_str->data = NULL;
      mr_ra_str->size = mr_ra_str->alloc_size = 0;
      return (-1);
    }
  tail = mr_rarray_append (mr_ra_str, length);
  if (tail)
    strcat (--tail, str);
  else if (mr_ra_str->data)
    {
      MR_FREE (mr_ra_str->data);
      mr_ra_str->data = NULL;
      mr_ra_str->size = mr_ra_str->alloc_size = 0;
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
  mr_ptrdes_t * ptrdes = mr_rarray_append ((mr_rarray_t*)ptrs, sizeof (ptrs->ra.data[0]));
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
  ptrdes->fd.comment = NULL;
  ptrdes->fd.ext.ptr = NULL;
  ptrdes->fd.ptr_type = NULL;
  ptrdes->level = 0;
  ptrdes->idx = -1; /* NB! To be initialized in depth search in mr_save */
  ptrdes->ref_idx = -1;
  ptrdes->parent = -1;
  ptrdes->first_child = -1;
  ptrdes->last_child = -1;
  ptrdes->prev = -1;
  ptrdes->next = -1;
  ptrdes->flags.is_null = MR_FALSE;
  ptrdes->flags.is_referenced = MR_FALSE;
  ptrdes->flags.is_content_reference = MR_FALSE;
  ptrdes->flags.is_opaque_data = MR_FALSE;
  ptrdes->value = NULL;
  ptrdes->ext.ptr = NULL;
  ptrdes->ptr_type = NULL;
  return (ptrs->ra.size / sizeof (ptrs->ra.data[0]) - 1);
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

  ptrs->ra.data[child].parent = parent;
  if (parent < 0)
    return;

  last_child = ptrs->ra.data[parent].last_child;
  if (last_child < 0)
    ptrs->ra.data[parent].first_child = child;
  else
    {
      ptrs->ra.data[last_child].next = child;
      ptrs->ra.data[child].prev = last_child;
      ptrs->ra.data[child].next = -1;
    }
  ptrs->ra.data[parent].last_child = child;
}

/**
 * Recursively free all allocated memory. Needs to be done from bottom to top.
 * @param ptrs resizable array with serialized data
 * @return status, EXIT_SUCCESS or EXIT_FAILURE
 */
int
mr_free_recursively (mr_ra_mr_ptrdes_t ptrs)
{
  int i;

  if (NULL == ptrs.ra.data)
    return (0);

  for (i = ptrs.ra.size / sizeof (ptrs.ra.data[0]) - 1; i >= 0; --i)
    {
      ptrs.ra.data[i].ext.ptr = NULL;
      if ((ptrs.ra.data[i].ref_idx < 0) && (ptrs.ra.data[i].idx >= 0))
	if ((MR_TYPE_EXT_POINTER == ptrs.ra.data[i].fd.mr_type_ext) ||
	    (MR_TYPE_EXT_RARRAY_DATA == ptrs.ra.data[i].fd.mr_type_ext) ||
	    ((MR_TYPE_EXT_NONE == ptrs.ra.data[i].fd.mr_type_ext) &&
	     (MR_TYPE_STRING == ptrs.ra.data[i].fd.mr_type)))
	  ptrs.ra.data[i].ext.ptr = *(void**)ptrs.ra.data[i].data;
    }

  for (i = ptrs.ra.size / sizeof (ptrs.ra.data[0]) - 1; i >= 0; --i)
    if (ptrs.ra.data[i].ext.ptr)
      MR_FREE (ptrs.ra.data[i].ext.ptr);

  MR_FREE (ptrs.ra.data);
  return (!0);
}

static int
calc_relative_addr (mr_ra_mr_ptrdes_t * ptrs, int idx, void * context)
{
  /* is new address is not set yet, then it could be calculated as relative address from the parent node */
  if (NULL == ptrs->ra.data[idx].ext.ptr)
    {
      int parent = ptrs->ra.data[idx].parent;
      ptrs->ra.data[idx].ext.ptr = &((char*)ptrs->ra.data[parent].ext.ptr)[ptrs->ra.data[idx].data - ptrs->ra.data[parent].data];
    }
  return (0);
}

/**
 * Recursively copy 
 * @param ptrs resizable array with serialized data
 * @return status, 0 - failure, !0 - success
 */
int
mr_copy_recursively (mr_ra_mr_ptrdes_t ptrs, void * dst)
{
  int i;

  if ((NULL == ptrs.ra.data) || (NULL == dst))
    return (0);

  for (i = ptrs.ra.size / sizeof (ptrs.ra.data[0]) - 1; i > 0; --i)
    ptrs.ra.data[i].ext.ptr = NULL;

  /* NB index 0 is excluded */  
  for (i = ptrs.ra.size / sizeof (ptrs.ra.data[0]) - 1; i > 0; --i)
    /*
      skip nodes that are not in final save graph (ptrs.ra.data[i].idx >= 0)
      nodes are references on other nodes (ptrs.ra.data[i].ref_idx < 0)
      or NULL pointers !ptrs.ra.data[i].flags.is_null
    */
    if ((ptrs.ra.data[i].idx >= 0) && (ptrs.ra.data[i].ref_idx < 0) && !ptrs.ra.data[i].flags.is_null) 
      switch (ptrs.ra.data[i].fd.mr_type_ext)
	{
	case MR_TYPE_EXT_NONE:
	  if (MR_TYPE_STRING != ptrs.ra.data[i].fd.mr_type)
	    break;
	  /* calc string length for further malloc */
	  ptrs.ra.data[i].fd.size = strlen (*(void**)ptrs.ra.data[i].data) + 1;
	  /* string node should be followed with unlinked char array node */
	  if ((ptrs.ra.data[i].first_child >= 0) ||
	      (ptrs.ra.data[i + 1].parent != i) ||
	      (*(void**)ptrs.ra.data[i].data != ptrs.ra.data[i + 1].data))
	    {
	      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_STRING_SAVE_DATA);
	      return (0);
	    }
	  /* link it back. we need to save address of allocated memory into this node */
	  ptrs.ra.data[i].first_child = ptrs.ra.data[i].last_child = i + 1;

	case MR_TYPE_EXT_POINTER:
	case MR_TYPE_EXT_RARRAY_DATA:
	  {
	    int idx;
	    char * copy;
	    int alloc_size = ptrs.ra.data[i].fd.size; /* default allocation size */
	    if (ptrs.ra.data[i].first_child < 0)
	      {
		MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_POINTER_NODE_CHILD_MISSING,
			    ptrs.ra.data[i].fd.type, ptrs.ra.data[i].fd.name.str);
		return (0);
	      }
	    /* rarrays require allocation of memoty chunk according alloc_size field */
	    if (MR_TYPE_EXT_RARRAY_DATA == ptrs.ra.data[i].fd.mr_type_ext)
	      {
		mr_rarray_t * ra = (mr_rarray_t*)&((char*)ptrs.ra.data[i].data)[-offsetof (mr_rarray_t, data)];
		ptrs.ra.data[i].fd.size = ra->size;
		/* statically allocated rarrays has negative alloc_size */
		if (ra->alloc_size > ra->size)
		  alloc_size = ra->alloc_size;
	      }
	    copy = MR_MALLOC (alloc_size);
	    if (NULL == copy)
	      {
		MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
		return (0);
	      }
	    /* copy data from source */
	    memcpy (copy, *(void**)ptrs.ra.data[i].data, ptrs.ra.data[i].fd.size);
	    /* zeros the rest of allocated memeory (only for rarrays) */
	    memset (&copy[ptrs.ra.data[i].fd.size], 0, alloc_size - ptrs.ra.data[i].fd.size);
	    /* go thru all childs and calculate their addresses in newly allocated chunk */
	    for (idx = ptrs.ra.data[i].first_child; idx >= 0; idx = ptrs.ra.data[idx].next)
	      ptrs.ra.data[idx].ext.ptr = &copy[(char*)ptrs.ra.data[idx].data - *(char**)ptrs.ra.data[i].data];
	  }
	  break;
	default:
	  break;
	}
  /* copy first level struct */
  memcpy (dst, ptrs.ra.data[0].data, ptrs.ra.data[0].fd.size);
  ptrs.ra.data[0].ext.ptr = dst;

  /* depth search thru the graph and calculate new addresses for all nodes */
  mr_ptrs_ds (&ptrs, calc_relative_addr, NULL);
      
  /* now we should update pointers in a copy */
  for (i = ptrs.ra.size / sizeof (ptrs.ra.data[0]) - 1; i > 0; --i)
    if ((ptrs.ra.data[i].idx >= 0) && !ptrs.ra.data[i].flags.is_null) /* skip NULL and invalid nodes */
      switch (ptrs.ra.data[i].fd.mr_type_ext)
	{
	case MR_TYPE_EXT_NONE:
	  if (MR_TYPE_STRING != ptrs.ra.data[i].fd.mr_type)
	    break;
	case MR_TYPE_EXT_POINTER:
	case MR_TYPE_EXT_RARRAY_DATA:
	  {
	    int ptr_idx;
	    /* get index of the node that is referenced by the pointer */
	    if (ptrs.ra.data[i].ref_idx < 0)
	      ptr_idx = ptrs.ra.data[i].first_child;
	    else
	      ptr_idx = ptrs.ra.data[i].flags.is_content_reference ? ptrs.ra.data[ptrs.ra.data[i].ref_idx].first_child : ptrs.ra.data[i].ref_idx;
	    /* update pointer in the copy */
	    *(void**)ptrs.ra.data[i].ext.ptr = ptrs.ra.data[ptr_idx].ext.ptr;
	  }
	  break;
	default:
	  break;
	}
  MR_FREE (ptrs.ra.data);
  return (!0);
}

/**
 * Default hash function.
 * @param str a pointer on null terminated string
 * @return Hash function value.
 */
unsigned int
mr_hash_str (char * str)
{
  unsigned int hash_value = 0;
  if (NULL == str)
    return (hash_value);
  while (*str)
    hash_value = (hash_value + (unsigned char)*str++) * 0xDeadBeef;
  return (hash_value);
}

unsigned int
mr_hashed_string_get_hash (mr_ptr_t x, const void * context)
{
  mr_hashed_string_t * x_ = x.ptr;
  if (0 == x_->hash_value)
    x_->hash_value = mr_hash_str (x_->str);
  return (x_->hash_value);
}

/**
 * Comparator for mr_hashed_string_t
 * @param a pointer on one mr_hashed_string_t
 * @param b pointer on another mr_hashed_string_t
 * @return comparation sign
 */
int
mr_hashed_string_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const mr_hashed_string_t * x_ = x.ptr;
  const mr_hashed_string_t * y_ = y.ptr;
  typeof (((mr_hashed_string_t*)NULL)->hash_value) x_hash_value = mr_hashed_string_get_hash ((mr_ptr_t)x, context);
  typeof (((mr_hashed_string_t*)NULL)->hash_value) y_hash_value = mr_hashed_string_get_hash ((mr_ptr_t)y, context);
  int diff = (x_hash_value > y_hash_value) - (x_hash_value < y_hash_value);
  if (diff)
    return (diff);
  return (strcmp (x_->str, y_->str));
}

static unsigned int
mr_fd_name_get_hash (mr_ptr_t x, const void * context)
{
  mr_fd_t * x_ = x.ptr;
  return (mr_hashed_string_get_hash (&x_->name, context));
}

/**
 * Comparator for mr_fd_t
 * @param a pointer on one mr_fd_t
 * @param b pointer on another mr_fd_t
 * @return comparation sign
 */
int
mr_fd_name_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const mr_fd_t * x_ = x.ptr;
  const mr_fd_t * y_ = y.ptr;
  return (mr_hashed_string_cmp ((void*)&x_->name, (void*)&y_->name, context));
}

static unsigned int
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
 * @return status EXIT_SUCCESS or EXIT_FAILURE
 */
static int
mr_anon_unions_extract (mr_td_t * tdp)
{
  int count = tdp->fields.size / sizeof (tdp->fields.data[0]);
  int i, j;

  for (i = 0; i < count; ++i)
    {
      mr_fd_t * fdp = tdp->fields.data[i].fdp;
      if ((MR_TYPE_ANON_UNION == fdp->mr_type) || (MR_TYPE_NAMED_ANON_UNION == fdp->mr_type))
	{
	  static int mr_type_anonymous_union_cnt = 0;
	  mr_td_t * tdp_ = fdp->ext.ptr; /* statically allocated memory for new type descriptor */
	  mr_fd_t ** first = &tdp->fields.data[i + 1].fdp;
	  mr_fd_t * last;
	  int opened = 1;

	  for (j = i + 1; j < count; ++j)
	    {
	      mr_fd_t * fdp_ = tdp->fields.data[j].fdp;
	      if ((MR_TYPE_ANON_UNION == fdp_->mr_type) ||
		  (MR_TYPE_NAMED_ANON_UNION == fdp_->mr_type))
		++opened;
	      if (MR_TYPE_END_ANON_UNION == fdp_->mr_type)
		if (0 == --opened)
		  break;
	    }
	  if (j >= count)
	    return (EXIT_FAILURE);

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
		if (fields[j]->offset != 0) /* MR_NONE and MR_END_ANON_UNION has zero offset */
		  fdp->offset = fields[j]->offset;
		fields[j]->offset = 0; /* reset offset to zero */
		if (tdp_->size < fields[j]->size)
		  tdp_->size = fields[j]->size; /* find union max size member */
	      }

	    last = tdp->fields.data[count].fdp;	  
	    last->mr_type = MR_TYPE_TRAILING_RECORD; /* trailing record */
	    tdp_->mr_type = fdp->mr_type; /*MR_TYPE_ANON_UNION or MR_TYPE_NAMED_ANON_UNION */
	    sprintf (tdp_->type.str, MR_TYPE_ANONYMOUS_UNION_TEMPLATE, mr_type_anonymous_union_cnt++);
	    tdp_->type.hash_value = mr_hash_str (tdp_->type.str);
	    tdp_->attr = fdp->comment; /* anonymous union stringified attributes are saved into comments field */
	    tdp_->comment = last->comment; /* copy comment from MR_END_ANON_UNION record */
	    tdp_->fields.data = &tdp->fields.data[count - fields_count + 1];

	    fdp->comment = last->comment; /* copy comment from MR_END_ANON_UNION record */
	    tdp->fields.size -= fields_count * sizeof (tdp->fields.data[0]);
	    count -= fields_count;
	    fdp->type = tdp_->type.str;
	    fdp->size = tdp_->size;
	    /* set name of anonymous union to temporary type name */
	    if ((NULL == fdp->name.str) || (0 == fdp->name.str[0]))
	      fdp->name.str = fdp->type;
	    fdp->name.hash_value = mr_hash_str (fdp->name.str);

	    if (EXIT_SUCCESS != mr_add_type (tdp_, NULL, NULL))
	      {
		MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_ANON_UNION_TYPE_ERROR, tdp->type.str);
		return (EXIT_FAILURE);
	      }
	  }

	}
    }
  return (EXIT_SUCCESS);
}

/**
 * calculate a hash value for mr_fd_t
 * @param x pointer on mr_fd_t
 * @param context pointer on a context
 * @return hash value
 */
unsigned int
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
 * @return status EXIT_SUCCESS or EXIT_FAILURE
 */
static int
mr_add_enum (mr_td_t * tdp)
{
  int i, count = tdp->fields.size / sizeof (tdp->fields.data[0]);
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

  mr_ic_hash_new (&tdp->lookup_by_value, mr_enumfd_get_hash, cmp_enums_by_value, "mr_fd_t");
  mr_ic_index (&tdp->lookup_by_value, (mr_ic_rarray_t*)&tdp->fields, NULL);

  for (i = 0; i < count; ++i)
    {
      /* adding to global lookup table by enum literal names */
      mr_ptr_t key = { .ptr = tdp->fields.data[i].fdp };
      mr_ptr_t * result = mr_ic_add (&mr_conf.enum_by_name, key, NULL);  
      if (NULL == result)
	return (EXIT_FAILURE);
      if (result->ptr != key.ptr)
	{
	  mr_fd_t * fdp = result->ptr;
	  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_DUPLICATED_ENUMS, fdp->name.str, key.ptr);
	  return (EXIT_FAILURE);
	}
    }

  return (EXIT_SUCCESS);
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
 * @return status EXIT_SUCCESS or EXIT_FAILURE
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
 * @return status EXIT_SUCCESS or EXIT_FAILURE
 */
static int
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
  static int isdelimiter [1 << (CHAR_BIT * sizeof (uint8_t))] =
    {
      [0 ... (1 << (CHAR_BIT * sizeof (char))) - 1] = 0,
      [0] = !0,
      [(uint8_t)' '] = !0,
      [(uint8_t)'*'] = !0,
    };
  int i;
  char * ptr;
  int prev_is_space = 0;
  int modified = 0;
  
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
	      modified = !0;
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
  return (EXIT_SUCCESS);
}

/**
 * Bitfield initialization. We need to calculate offset and shift. Width was initialized by macro.
 * @param fdp pointer on a field descriptor
 * @return status EXIT_SUCCESS or EXIT_FAILURE
 */
static int
mr_init_bitfield (mr_fd_t * fdp)
{
  int i, j;
  if ((NULL == fdp->param.bitfield_param.bitfield.data) ||
      (0 == fdp->param.bitfield_param.bitfield.size))
    return (EXIT_SUCCESS);
  
  for (i = 0; i < fdp->param.bitfield_param.bitfield.size; ++i)
    if (fdp->param.bitfield_param.bitfield.data[i])
      break;
  /* if bitmask is clear then there is no need to initialize anything */
  if (!fdp->param.bitfield_param.bitfield.data[i])
    return (EXIT_SUCCESS);
  
  fdp->offset = i;
  for (i = 0; i < CHAR_BIT; ++i)
    if (fdp->param.bitfield_param.bitfield.data[fdp->offset] & (1 << i))
      break;
  fdp->param.bitfield_param.shift = i;
  fdp->param.bitfield_param.width = 0;
  for (j = fdp->offset; j < fdp->param.bitfield_param.bitfield.size; ++j)
    {
      for ( ; i < CHAR_BIT; ++i)
	if (fdp->param.bitfield_param.bitfield.data[j] & (1 << i))
	  ++fdp->param.bitfield_param.width;
	else
	  break;
      if (i < CHAR_BIT)
	break;
      i = 0;
    }
  return (EXIT_SUCCESS);
}

/**
 * New type descriptor preprocessing. Check fields names duplocation, nornalize types name, initialize bitfields. Called once for each type.
 * @param tdp pointer on a type descriptor
 * @return status EXIT_SUCCESS or EXIT_FAILURE
 */
static int
mr_check_fields (mr_td_t * tdp)
{
  int i, count = tdp->fields.size / sizeof (tdp->fields.data[0]);
  for (i = 0; i < count; ++i)
    {
      mr_fd_t * fdp = tdp->fields.data[i].fdp;
      /*
	Check names of the fileds.
	MR_NONE definitions may contain brackets (for arrays) or braces (for function pointers) or collon (for bitfields).
      */
      char * name = fdp->name.str;
      if (name)
	{
	  for (; isalnum (*name) || (*name == '_'); ++name); /* skip valid characters */
	  if (*name) /* strings with field names might be in read-only memory. For MR_NONE names are saved in writable memory. */
	    *name = 0; /* truncate on first invalid charecter */
	}
      mr_normalize_type (fdp);
      if (MR_TYPE_BITFIELD == fdp->mr_type)
	mr_init_bitfield (fdp);
    }
  return (EXIT_SUCCESS);
}

/**
 * Initialize AUTO fields. Detect types, size, pointers etc.
 * @param fdp pointer on a field descriptor
 * @return status EXIT_SUCCESS or EXIT_FAILURE
 */
static int
mr_auto_field_detect (mr_fd_t * fdp)
{
  static size_t types_sizes[] =
    {
      [0 ... MR_TYPE_LAST - 1] = 0,
      [MR_TYPE_NONE] = 0,
      [MR_TYPE_VOID] = sizeof (void),
      [MR_TYPE_INT8] = sizeof (int8_t),
      [MR_TYPE_UINT8] = sizeof (uint8_t),
      [MR_TYPE_INT16] = sizeof (int16_t),
      [MR_TYPE_UINT16] = sizeof (uint16_t),
      [MR_TYPE_INT32] = sizeof (int32_t),
      [MR_TYPE_UINT32] = sizeof (uint32_t),
      [MR_TYPE_INT64] = sizeof (int64_t),
      [MR_TYPE_UINT64] = sizeof (uint64_t),
      [MR_TYPE_FLOAT] = sizeof (float),
      [MR_TYPE_DOUBLE] = sizeof (double),
      [MR_TYPE_LONG_DOUBLE] = sizeof (long double),
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
  return (EXIT_SUCCESS);
}

/**
 * Initialize fields that are pointers on functions. Detects types of arguments.
 * @param fdp pointer on a field descriptor
 * @return status EXIT_SUCCESS or EXIT_FAILURE
 */
static int
mr_func_field_detect (mr_fd_t * fdp)
{
  int i;
  for (i = 0; fdp->param.func_param.data[i].mr_type != MR_TYPE_TRAILING_RECORD; ++i)
    {
      mr_normalize_type (&fdp->param.func_param.data[i]);
      switch (fdp->param.func_param.data[i].mr_type)
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
	  mr_auto_field_detect (&fdp->param.func_param.data[i]);
	  break;
	default:
	  break;
	}
    }
  fdp->param.func_param.size = i * sizeof (fdp->param.func_param.data[0]);
  return (EXIT_SUCCESS);
}

static int
mr_detect_field_type (mr_fd_t * fdp)
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
	MR_POINTER_STRUCT refers to forward declarations of structures and can't calculate type size at compile time.
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
  return (EXIT_SUCCESS);
}

/**
 * Initialize fields descriptors. Everytnig that was not properly initialized in macro.
 * @param tdp pointer on a type descriptor
 * @param args auxiliary arguments
 * @return status EXIT_SUCCESS or EXIT_FAILURE
 */
static int
mr_detect_fields_types (mr_td_t * tdp)
{
  int i, count = tdp->fields.size / sizeof (tdp->fields.data[0]);
  for (i = 0; i < count; ++i)
    mr_detect_field_type (tdp->fields.data[i].fdp);
  return (EXIT_SUCCESS);
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
static int
mr_register_type_pointer (mr_td_t * tdp)
{
  mr_fd_t * fdp;
  mr_td_t * union_tdp = mr_get_td_by_name ("mr_ptr_t");
  /* check that mr_ptr_t is already a registered type */
  if (NULL == union_tdp)
    return (EXIT_FAILURE);
  /* check that requested type is already registered */
  if (NULL != mr_get_fd_by_name (union_tdp, tdp->type.str))
    return (EXIT_SUCCESS);
  
  /* statically allocated trailing record is used for field descriptor */
  fdp = tdp->fields.data[tdp->fields.size / sizeof (tdp->fields.data[0])].fdp;
  if (NULL == fdp)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
      return (EXIT_FAILURE);
    }
  
  memset (fdp, 0, sizeof (*fdp));
  fdp->type = tdp->type.str;
  fdp->name = tdp->type;
  fdp->size = tdp->size;
  fdp->offset = 0;
  fdp->mr_type = tdp->mr_type;
  fdp->mr_type_aux = MR_TYPE_VOID;
  fdp->mr_type_ext = MR_TYPE_EXT_POINTER;
  return ((NULL == mr_ic_add (&union_tdp->lookup_by_name, fdp, NULL)) ? EXIT_SUCCESS : EXIT_FAILURE);
}

/**
 * Add type description into repository
 * @param tdp a pointer on statically initialized type descriptor
 * @param comment comments
 * @param ... auxiliary void pointer
 * @return status, 0 - new type was added, !0 - type was already registered
 */
int
mr_add_type (mr_td_t * tdp, char * comment, ...)
{
  va_list args;
  void * ext;
  int count;

  if (NULL == tdp)
    return (EXIT_FAILURE); /* assert */
  /* check whether this type is already in the list */
  if (mr_get_td_by_name (tdp->type.str))
    return (EXIT_SUCCESS); /* this type is already registered */

  va_start (args, comment);
  ext = va_arg (args, void*);
  va_end (args);

  for (count = 0; ; ++count)
    {
      mr_fd_t * fdp = tdp->fields.data[count].fdp;
      if (NULL == fdp)
	{
	  MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
	  return (EXIT_FAILURE);
	}
      if (MR_TYPE_TRAILING_RECORD == fdp->mr_type)
	break;
    }
  tdp->fields.size = count * sizeof (tdp->fields.data[0]);
  tdp->fields.alloc_size = -1;
  tdp->fields.ext.ptr = NULL;
  
  if ((NULL != comment) && comment[0])
    tdp->comment = comment;

  if (NULL != ext)
    tdp->ext.ptr = ext;

  if (EXIT_SUCCESS != mr_anon_unions_extract (tdp)) /* important to extract unions before building index over fields */
    return (EXIT_FAILURE);
  
  /* MR_IC_NONE: compares 168952 matches 9703 ratio: 17.41 */
  /* MR_IC_RBTREE: compares 57272 matches 12029 ratio: 4.76 */
  /* MR_IC_SORTED_ARRAY: compares 54947 matches 10987 ratio: 5.00 */
  /* MR_IC_HASH: compares 15453 matches 14019 ratio: 1.10 */
  
  mr_check_fields (tdp);
  mr_ic_hash_new (&tdp->lookup_by_name, mr_fd_name_get_hash, mr_fd_name_cmp, "mr_fd_t");
  mr_ic_index (&tdp->lookup_by_name, (mr_ic_rarray_t*)&tdp->fields, NULL);
  
  if (NULL == mr_conf.enum_by_name.find)
    mr_ic_hash_new (&mr_conf.enum_by_name, mr_fd_name_get_hash, mr_fd_name_cmp, "mr_fd_t");

  if (NULL == mr_conf.lookup_by_name.find)
    mr_ic_hash_new (&mr_conf.lookup_by_name, mr_td_name_get_hash, mr_td_name_cmp, "mr_td_t");
  
  if (NULL == mr_ic_add (&mr_conf.lookup_by_name, tdp, NULL))
    return (EXIT_FAILURE);

  memset (&tdp->lookup_by_value, 0, sizeof (tdp->lookup_by_value));
  if (MR_TYPE_ENUM == tdp->mr_type)
    mr_add_enum (tdp);

  return (EXIT_SUCCESS);
}

static int
mr_conf_init_visitor (mr_ptr_t key, const void * context)
{
  mr_td_t * tdp = key.ptr;
  mr_detect_fields_types (tdp);
  mr_register_type_pointer (tdp);
  return (EXIT_SUCCESS);
}  

static void
mr_conf_init ()
{
  static int initialized = 0;
  if (!initialized)
    {
      mr_ic_foreach (&mr_conf.lookup_by_name, mr_conf_init_visitor, NULL);
      initialized = !0;
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
	fdp->mr_type = tdp->mr_type;				
      break;							
    default: break;							
    }								
}

/**
 * Helper function for building tree within parsing.
 * @param mr_load structure with current parsing context
 * @return index of newly allocated element in mr_load->ptrs resizeable array
 */
int
mr_parse_add_node (mr_load_t * mr_load)
{
  int idx = mr_add_ptr_to_list ((mr_ra_mr_ptrdes_t*)mr_load->ptrs);
  if (idx < 0)
    return (idx);
  mr_add_child (mr_load->parent, idx, (mr_ra_mr_ptrdes_t*)mr_load->ptrs);
  return (idx);
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
