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
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdarg.h>

#include <mr_tsearch.h>
#include <metaresc.h>

#define MR_MODE DESC /* we'll need descriptors of our own types */
#include <mr_protos.h>

static void * mr_malloc (const char * filename, const char * function, int line, size_t size) { return (malloc (size)); }
static void * mr_realloc (const char * filename, const char * function, int line, void * ptr, size_t size) { return (realloc (ptr, size)); }
static char * mr_strdup (const char * filename, const char * function, int line, const char * str) { return (strdup (str)); }
static void mr_free (const char * filename, const char * function, int line, void * ptr) { free (ptr); }

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
  .des = {
    .collection = {
      .data = NULL,
      .size = 0,
      .alloc_size = 0,
    },
    .ic_type = MR_IC_NONE,
    .key_type = "mr_td_t",
    .compar_fn = NULL,
    .index = NULL,
    .add = NULL,
    .find = NULL,
    .free = NULL,
    .ext = { .ptr = NULL, },
  },
  .enum_by_name = { .root = NULL, .key_type = "mr_fd_t", },
  .output_format = { [0 ... MR_MAX_TYPES - 1] = NULL, },
  .io_handlers =
  { [0 ... MR_MAX_TYPES - 1] =
    {
      .load =
      {
	.rl = NULL,
	.xdr = NULL,
      },
      .save =
      {
	.rl = NULL,
	.xdr = NULL,
	.xml = NULL,
	.cinit = NULL,
	.scm = NULL,
      },
    }
  },  
  .io_ext_handlers =
  { [0 ... MR_MAX_TYPES - 1] =
    {
      .load =
      {
	.rl = NULL,
	.xdr = NULL,
      },
      .save =
      {
	.rl = NULL,
	.xdr = NULL,
	.xml = NULL,
	.cinit = NULL,
	.scm = NULL,
      },
    }
  },
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

/**
 * Memory cleanp handler.
 */
static void
dummy_free_func (mr_ptr_t key, const void * context) {}
  
static int
free_lookup_tree (mr_ptr_t key, const void * context)
{
  mr_td_t * tdp = key.ptr;
  mr_tdestroy (tdp->lookup_by_value.root, dummy_free_func, NULL);
  tdp->lookup_by_value.root = NULL;
  if (tdp->lookup_by_name.data)
    MR_FREE (tdp->lookup_by_name.data);
  tdp->lookup_by_name.data = NULL;
  tdp->lookup_by_name.size = tdp->lookup_by_name.alloc_size = 0;
  return (0);
}

static mr_ptr_t
mr_ic_none_find (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  int i;
  for (i = ic->collection.size / sizeof (ic->collection.data[0]) - 1; i >= 0; --i)
    if (0 == ic->compar_fn (key, ic->collection.data[i], context))
      return (ic->collection.data[i]);
  return ((mr_ptr_t){ NULL });
}

static int
mr_ic_none_new (mr_ic_t * ic, mr_compar_fn_t compar_fn, char * key_type)
{
  if ((NULL == ic) || (NULL == compar_fn))
    return (!0);
  ic->key_type = key_type;
  ic->compar_fn = compar_fn;
  ic->index = NULL;
  ic->add = NULL;
  ic->find = mr_ic_none_find;
  ic->free = NULL;
  ic->ext.ptr = NULL;
  return (0);
}

static inline mr_ptr_t
mr_ic_add (mr_ic_t * ic, mr_ptr_t key, const void * context)
{
  mr_ptr_t * new_element;
  if (NULL == ic)
    return ((mr_ptr_t){ NULL });
  
  new_element = mr_rarray_append ((mr_rarray_t*)&ic->collection, sizeof (ic->collection.data[0]));
  if (NULL == new_element)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return ((mr_ptr_t){ NULL });
    }
  *new_element = key;
  
  if (ic->add)
    return (ic->add (ic, key, context));
  return (key);
}

static inline int
mr_ic_index (mr_ic_t * ic, const void * context)
{
  if (NULL == ic)
    return (!0);
  if (ic->index)
    return (ic->index (ic, context));
  return (0);
}

/**
 * Type descriptors collection iterator.
 * @param func function for type descriptors processing
 * @param args auxiliary arguments
 * @return flag that cycle was not completed
 */
static inline int
mr_ic_foreach (mr_ic_t * ic, mr_visit_fn_t visit_fn, const void * context)
{
  int count, i;
  if (NULL == ic)
    return (!0);
  
  count = ic->collection.size / sizeof (ic->collection.data[0]);
  for (i = 0; i < count; ++i)
    if (visit_fn (ic->collection.data[i], context))
      return (!0);
  return (0);
}

static inline mr_ptr_t
mr_ic_find (mr_ic_t * ic,mr_ptr_t key, const void * context)
{
  if (NULL == ic)
    return ((mr_ptr_t){ NULL });
  if (ic->find)
    return (ic->find (ic, key, context));
  return ((mr_ptr_t){ NULL });
}

static inline void
mr_ic_free (mr_ic_t * ic, mr_free_fn_t free_fn, const void * context)
{
  if (NULL == ic)
    return;
  if (ic->free)
    ic->free (ic, free_fn, context);
}

static void __attribute__((destructor))
mr_cleanup (void)
{
  mr_td_t * mr_ptr_t_td = mr_get_td_by_name ("mr_ptr_t");
  
  if ((mr_ptr_t_td) && (mr_ptr_t_td->fields.alloc_size > 0) && (mr_ptr_t_td->fields.data))
    {
      MR_FREE (mr_ptr_t_td->fields.data);
      mr_ptr_t_td->fields.data = NULL;
    }
  
  mr_tdestroy (mr_conf.enum_by_name.root, dummy_free_func, NULL);
  mr_conf.enum_by_name.root = NULL;

  mr_ic_foreach (&mr_conf.des, free_lookup_tree, NULL);
  mr_ic_free (&mr_conf.des, dummy_free_func, NULL);
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
	  mr_fd_t * fdp;
	  for (fdp = tdp->fields.data; fdp->mr_type != MR_TYPE_TRAILING_RECORD; ++fdp)
	    messages[fdp->param.enum_value] = fdp->comment;
	}
      messages_inited = !0;
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
 * Helper function for serialization macroses. Detects mr_type for enums, structures and unions.
 * Enums are detected at compile time as integers, and structures & unions as MR_TYPE_NONE
 *
 * @param fdp pointer on field descriptor
 */
void
mr_detect_type (mr_fd_t * fdp)
{
  mr_td_t * tdp;
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
  for (i = 8 - ptrdes->fd.param.bitfield_param.shift; i < ptrdes->fd.param.bitfield_param.width; i += 8)
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
  if (ptrdes->fd.param.bitfield_param.shift + ptrdes->fd.param.bitfield_param.width >= 8)
    *ptr &= ((1 << ptrdes->fd.param.bitfield_param.shift) - 1);
  else
    *ptr &= (-1 - ((1 << (ptrdes->fd.param.bitfield_param.shift + ptrdes->fd.param.bitfield_param.width)) - 1)) | ((1 << ptrdes->fd.param.bitfield_param.shift) - 1);
  *ptr++ |= *value << ptrdes->fd.param.bitfield_param.shift;
  for (i = 8 - ptrdes->fd.param.bitfield_param.shift; i < ptrdes->fd.param.bitfield_param.width; i += 8)
    if (ptrdes->fd.param.bitfield_param.width - i >= 8)
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
  ptrdes->fd.hashed_name.name = NULL;
  ptrdes->fd.hashed_name.hash_value = 0;
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
  ptrdes->union_field_name = NULL;
  ptrdes->union_discriminator = NULL;
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
			    ptrs.ra.data[i].fd.type, ptrs.ra.data[i].fd.hashed_name.name);
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
  i = 0;
  while (i >= 0)
    {
      /* is new address is not set yet, then it could be calculated as relative address from the parent node */
      if (NULL == ptrs.ra.data[i].ext.ptr)
	{
	  int parent = ptrs.ra.data[i].parent;
	  ptrs.ra.data[i].ext.ptr = &((char*)ptrs.ra.data[parent].ext.ptr)[ptrs.ra.data[i].data - ptrs.ra.data[parent].data];
	}
      /* depth search iterator */
      if (ptrs.ra.data[i].first_child >= 0)
	i = ptrs.ra.data[i].first_child;
      else
	{
	  while ((ptrs.ra.data[i].next < 0) && (ptrs.ra.data[i].parent >= 0))
	    i = ptrs.ra.data[i].parent;
	  i = ptrs.ra.data[i].next;
	}
    }      
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
static uint64_t
mr_hash_str (char * str)
{
  uint64_t hash_value = 0;
  if (NULL == str)
    return (hash_value);
  while (*str)
    hash_value = (hash_value + (unsigned char)*str++) * 0xDeadBeef;
  return (hash_value);
}

/**
 * Function for checking if a number is prime.
 * @param x an integer number.
 * @return Returns TRUE if number is prime, FALSE otherwise. 
 */
static int
is_prime (int x)
{
  int i;
  if (1 == x)
    return (0);
  for (i = 2; i * i <= x; ++i)
    if (x % i == 0)
      return (0);
  return (!0);
}

#ifdef MR_TREE_LOOKUP_
/**
 * Update hash with type descriptors
 * @param tdp root of linked list with type descriptors
 * @param hash mr_rarray_t with hash table for type descriptors
 * @return void
 */

static int
td_count (mr_td_t * tdp, void * args)
{
  ++*((int*)args);
  return (0);
}

int td_populate (mr_td_t * tdp, void * args)
{
  mr_ra_mr_td_ptr_t * hash = args;
  mr_td_ptr_t * x = &hash->ra.data[tdp->hashed_name.hash_value % (hash->ra.size / sizeof (hash->ra.data[0]))];
  /* check for collision */
  if (x->tdp)
    return (!0);
  x->tdp = tdp;
  return (0);
}

static void
mr_update_td_hash (mr_td_t * tdp, mr_ra_mr_td_ptr_t * hash)
{
  int count;
  
  tdp->hashed_name.hash_value = mr_hash_str (tdp->hashed_name.name);

  if (NULL == hash->ra.data)
    hash->ra.alloc_size = hash->ra.size = 0;

  if (0 == hash->ra.size)
    {
      /* hash size is not defined. Let it be doubled number of elements in the list */
      count = 0;
      mr_td_foreach (td_count, &count);
      hash->ra.size = 2 * count * sizeof (hash->ra.data[0]);
      if (hash->ra.data)
	MR_FREE (hash->ra.data);
      hash->ra.data = NULL;
    }
  else
    {
      /* Lets calculate hash bucket for new element */
      mr_td_ptr_t * x = &hash->ra.data[tdp->hashed_name.hash_value % (hash->ra.size / sizeof (hash->ra.data[0]))];
      if (NULL == x->tdp)
	x->tdp = tdp; /* bucket was free and hash resize is not required */ 
      else
	{
	  if (x->tdp->hashed_name.hash_value == tdp->hashed_name.hash_value) /* hashes matched and non-collision hash table is not possible */
	    {
	      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_TYPES_HASHES_MATCHED, x->tdp->hashed_name.name, tdp->hashed_name.name);
	      return;
	    }
	  /* we have a collision, so we will need to find new hash size to avoid collisions */ 
	  MR_FREE (hash->ra.data);
	  hash->ra.data = NULL;
	  hash->ra.size = hash->ra.alloc_size = 0;
	}
    }
  /* we will find new size for hash table to avoid collisions */
  count = (hash->ra.size / sizeof (hash->ra.data[0])) | 1; /* for optimization of new size search we will probe only prime numbers */
  while (NULL == hash->ra.data)
    {
      /* we need to find next prime number greater then count */
      do
	count += 2;
      while (!is_prime (count));
      
      hash->ra.alloc_size = hash->ra.size = count * sizeof (hash->ra.data[0]);
      hash->ra.data = MR_MALLOC (hash->ra.alloc_size);
      /* check memory allocation */
      if (NULL == hash->ra.data)
	{
	  MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	  hash->ra.alloc_size = hash->ra.size = 0;
	  return;
	}
      memset (hash->ra.data, 0, hash->ra.size);
      /* populate list elements into hash table */
      /* check that all elements were successfully populated into the hash table */
      if (mr_td_foreach (td_populate, hash))
	{
	  MR_FREE (hash->ra.data); /* otherwise try to find new hash size */
	  hash->ra.data = NULL;
	}
    }
}

/**
 * Addition of a new type descriptor to lookup structure. Implementation as a RB-tree.
 * @param tdp new type descriptor
 * @param tree pointer on a root pointer of the lookup tree
 */
static void
mr_update_td_tree (mr_td_t * tdp, mr_red_black_tree_node_t ** tree)
{
  mr_td_t ** tdpp = mr_tsearch (tdp, tree, cmp_tdp, NULL);
  if (NULL == tdpp)
    MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
}

#else /* MR_TREE_LOOKUP */
#endif /* MR_TREE_LOOKUP */

static inline uint64_t
mr_hashed_name_get_hash (mr_hashed_name_t * x)
{
  if (0 == x->hash_value)
    x->hash_value = mr_hash_str (x->name);
  return (x->hash_value);
}

/**
 * Comparator for mr_hashed_name_t
 * @param a pointer on one mr_hashed_name_t
 * @param b pointer on another mr_hashed_name_t
 * @return comparation sign
 */
static int
mr_hashed_name_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const mr_hashed_name_t * x_ = x.ptr;
  const mr_hashed_name_t * y_ = y.ptr;
  uint64_t x_hash_value = mr_hashed_name_get_hash ((mr_hashed_name_t*)x_);
  uint64_t y_hash_value = mr_hashed_name_get_hash ((mr_hashed_name_t*)y_);
  int diff = (x_hash_value > y_hash_value) - (x_hash_value < y_hash_value);
  if (diff)
    return (diff);
  return (strcmp (x_->name, y_->name));
}

/**
 * Type descriptor lookup function. Lookup by type name.
 * @param type stringified type name
 * @return pointer on type descriptor
 */
mr_td_t *
mr_get_td_by_name (char * type)
{
  mr_hashed_name_t hashed_name = { .name = type, .hash_value = mr_hash_str (type), };
  return (mr_ic_find (&mr_conf.des, &hashed_name, NULL).ptr);
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
    if ((MR_TYPE_ANON_UNION == tdp->fields.data[i].mr_type) || (MR_TYPE_NAMED_ANON_UNION == tdp->fields.data[i].mr_type))
      {
	for (j = i + 1; j < count; ++j)
	  if (MR_TYPE_END_ANON_UNION == tdp->fields.data[j].mr_type)
	    break;
	if (j >= count)
	  return (EXIT_FAILURE);
	{
	  int fields_count = j - i; /* additional trailing element with mr_type = MR_TYPE_TRAILING_RECORD */
	  static int mr_type_anonymous_union_cnt = 0;
	  mr_td_t * tdp_ = tdp->fields.data[i].ext.ptr; /* statically allocated memory for new type descriptor */
	  mr_fd_t * first = &tdp->fields.data[i + 1];

	  tdp_->size = 0;
	  /* rotate fields until all union fields will be shifted to the end of the array */
	  for (j = 0; j < fields_count; ++j)
	    {
	      mr_fd_t fd = *first;
	      memmove (first, &first[1], (count - i - 1) * sizeof (*first));
	      /* offset of union memebers may differ from offset of anonymous union place holder */
	      if (fd.offset != 0) /* MR_NONE and MR_END_ANON_UNION has zero offset */
		tdp->fields.data[i].offset = fd.offset;
	      fd.offset = 0; /* reset offset to zero */
	      tdp->fields.data[count] = fd;
	      if (fd.size > tdp_->size)
		tdp_->size = fd.size; /* find union max size member */
	    }
	  tdp->fields.data[count].mr_type = MR_TYPE_TRAILING_RECORD; /* trailing record */
	  tdp_->mr_type = tdp->fields.data[i].mr_type;
	  sprintf (tdp_->hashed_name.name, MR_TYPE_ANONYMOUS_UNION_TEMPLATE, mr_type_anonymous_union_cnt++);
	  tdp_->attr = tdp->fields.data[i].comment; /* anonymous union stringified attributes are saved into comments field */
	  tdp_->comment = tdp->fields.data[count].comment; /* copy comment from MR_END_ANON_UNION record */
	  tdp_->fields.data = &tdp->fields.data[count - fields_count + 1];

	  tdp->fields.data[i].comment = tdp->fields.data[count].comment; /* copy comment from MR_END_ANON_UNION record */
	  tdp->fields.size -= fields_count * sizeof (tdp->fields.data[0]);
	  count -= fields_count;
	  tdp->fields.data[i].type = tdp_->hashed_name.name;
	  tdp->fields.data[i].size = tdp_->size;
	  /* set name of anonymous union to temporary type name */
	  if ((NULL == tdp->fields.data[i].hashed_name.name) || (0 == tdp->fields.data[i].hashed_name.name[0]))
	    {
	      tdp->fields.data[i].hashed_name.name = tdp->fields.data[i].type;
	      tdp->fields.data[i].hashed_name.hash_value = 0;
	    }

	  if (mr_add_type (tdp_, NULL, NULL))
	    {
	      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_ANON_UNION_TYPE_ERROR, tdp_->hashed_name.name);
	      return (EXIT_SUCCESS);
	    }
	}
      }
  return (EXIT_SUCCESS);
}

/**
 * comparator for mr_fd_t sorting by enum value
 * @param x pointer on one mr_fd_t
 * @param y pointer on another mr_fd_t
 * @return comparation sign
 */
static int
cmp_enums_by_value (void * x, void * y, const void * context)
{
  return ((((const mr_fd_t *) x)->param.enum_value > ((const mr_fd_t *) y)->param.enum_value) -
	  (((const mr_fd_t *) x)->param.enum_value < ((const mr_fd_t *) y)->param.enum_value));
}

/**
 * comparator for mr_fd_t sorting by enum name
 * @param x pointer on one mr_fd_t
 * @param y pointer on another mr_fd_t
 * @return comparation sign
 */
static int
cmp_enums_by_name (void * x, void * y, const void * context)
{
  return (strcmp (((const mr_fd_t *) x)->hashed_name.name, ((const mr_fd_t *) y)->hashed_name.name));
}

/**
 * New enum descriptor preprocessing. Enum literal values should be added to global lookup table and enum type descriptor should have a lookup by enum values.
 * @param tdp pointer on a new enum type descriptor
 * @return status EXIT_SUCCESS or EXIT_FAILURE
 */
static int
mr_add_enum (mr_td_t * tdp)
{
  int count = tdp->fields.size / sizeof (tdp->fields.data[0]);
  int i;

  /*
    Enums with __attribute__((packed, aligned (XXX))) generates size according alignment, but not real size which is 1 byte due to packing.
    Here we determine effective type size.
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
  
  tdp->lookup_by_value.root = NULL;
  tdp->lookup_by_value.key_type = "mr_fd_t";
  
  for (i = 0; i < count; ++i)
    {
      /* adding to global lookup table by enum literal names */
      mr_fd_t ** fdpp = mr_tsearch (&tdp->fields.data[i], &mr_conf.enum_by_name.root, cmp_enums_by_name, NULL);  
      if (NULL == fdpp)
	{
	  MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	  return (EXIT_FAILURE);
	}
      if (*fdpp != &tdp->fields.data[i])
	{
	  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_DUPLICATED_ENUMS, (*fdpp)->hashed_name.name, tdp->hashed_name.name);
	  return (EXIT_FAILURE);
	}
      /* adding to local lookup table by enum values */
      fdpp = mr_tsearch (&tdp->fields.data[i], &tdp->lookup_by_value.root, cmp_enums_by_value, NULL);  
      if (NULL == fdpp)
	{
	  MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
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
  mr_fd_t ** fdpp = mr_tfind (&fd, &tdp->lookup_by_value.root, cmp_enums_by_value, NULL);
  if (fdpp)
    return (*fdpp);
  return (NULL);
}

/**
 * Enum literal name lookup function.
 * @param value address for enum value to store
 * @param name literal name of enum to lookup
 * @return status EXIT_SUCCESS or EXIT_FAILURE
 */
int
mr_get_enum_by_name (uint64_t * value, char * name)
{
  mr_fd_t fd = { .hashed_name = { .name = name } };
  mr_fd_t ** fdpp = mr_tfind (&fd, &mr_conf.enum_by_name.root, cmp_enums_by_name, NULL);
  if (fdpp)
    *value = (*fdpp)->param.enum_value;
  return (fdpp ? EXIT_SUCCESS : EXIT_FAILURE);
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
  static int isdelimiter [1 << (8 * sizeof (uint8_t))] =
    {
      [0 ... (1 << (8 * sizeof (char))) - 1] = 0,
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
  for (i = 0; i < 8; ++i)
    if (fdp->param.bitfield_param.bitfield.data[fdp->offset] & (1 << i))
      break;
  fdp->param.bitfield_param.shift = i;
  fdp->param.bitfield_param.width = 0;
  for (j = fdp->offset; j < fdp->param.bitfield_param.bitfield.size; ++j)
    {
      for ( ; i < 8; ++i)
	if (fdp->param.bitfield_param.bitfield.data[j] & (1 << i))
	  ++fdp->param.bitfield_param.width;
	else
	  break;
      if (i < 8)
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
  int i, j;
  int count = tdp->fields.size / sizeof (tdp->fields.data[0]);
  for (i = 0; i < count; ++i)
    {
      mr_fd_t * fdp = &tdp->fields.data[i];
      /*
	Check names of the fileds.
	MR_NONE definitions may contain brackets (for arrays) or braces (for function pointers) or collon (for bitfields).
      */
      char * name = fdp->hashed_name.name;
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
  /* check for name duplicates */
  for (i = 0; i < count; ++i)
    for (j = i + 1; j < count; ++j)
      if (tdp->fields.data[i].hashed_name.name && tdp->fields.data[j].hashed_name.name && (0 == strcmp (tdp->fields.data[i].hashed_name.name, tdp->fields.data[j].hashed_name.name)))
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_DUPLICATED_FIELDS, tdp->fields.data[i].hashed_name.name, tdp->hashed_name.name);
  
  return (EXIT_SUCCESS);
}

/**
 * Initialize non-collision hash table for fields lookup by fields name.
 * @param tdp pointer on a type descriptor
 * @return status EXIT_SUCCESS or EXIT_FAILURE
 */
static int
mr_build_field_names_hash (mr_td_t * tdp)
{
  int i, j;
  int fields_count = tdp->fields.size / sizeof (tdp->fields.data[0]);

  tdp->lookup_by_name.size = tdp->lookup_by_name.alloc_size = 0;
  tdp->lookup_by_name.data = NULL;
  for (i = 0; i < fields_count; ++i)
    tdp->fields.data[i].hashed_name.hash_value = mr_hash_str (tdp->fields.data[i].hashed_name.name);

  /* sanity check for hash value collision */
  for (i = 0; i < fields_count; ++i)
    for (j = i + 1; j < fields_count; ++j)
      if (tdp->fields.data[i].hashed_name.hash_value == tdp->fields.data[j].hashed_name.hash_value)
	return (EXIT_FAILURE);

  if (0 == fields_count)
    return (EXIT_SUCCESS);
  
  while (NULL == tdp->lookup_by_name.data)
    {
      mr_fd_ptr_t * array;
      int size = ((tdp->lookup_by_name.size / sizeof (tdp->lookup_by_name.data[0])) | 1) + 2;
      /* we need to find next prime number greater then current hash table size */ 
      while (!is_prime (size))
	size += 2;
      tdp->lookup_by_name.alloc_size = tdp->lookup_by_name.size = size * sizeof (tdp->lookup_by_name.data[0]);
      array = MR_MALLOC (tdp->lookup_by_name.alloc_size);
      /* check memory allocation */
      if (NULL == array)
	{
	  MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	  tdp->lookup_by_name.alloc_size = tdp->lookup_by_name.size = 0;
	  return (EXIT_FAILURE);
	}
      for (i = 0; i < size; ++i)
	array[i].fdp = NULL;
      /* populate list elements into hash table */
      for (i = 0; i < fields_count; ++i)
	{
	  mr_fd_ptr_t * x = &array[tdp->fields.data[i].hashed_name.hash_value % size];
	  /* check for collision */
	  if (x->fdp)
	    break;
	  else
	    x->fdp = &tdp->fields.data[i];
	}
      /* check that all elements were successfully populated into the hash table */
      if (i >= fields_count)
	tdp->lookup_by_name.data = array;
      else
	MR_FREE (array); /* otherwise try to find new hash size */
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
      [0 ... MR_MAX_TYPES - 1] = 0,
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

/**
 * Initialize fields descriptors. Everytnig that was not properly initialized in macro.
 * Called on each new type registration for all already registered types.
 * @param tdp pointer on a type descriptor
 * @param args auxiliary arguments
 * @return status EXIT_SUCCESS or EXIT_FAILURE
 */
static int
mr_detect_fields_types (mr_ptr_t key, const void * context)
{
  mr_td_t * tdp = key.ptr;
  int i;
  mr_td_t * tdp_;
  int fields_count = tdp->fields.size / sizeof (tdp->fields.data[0]);

  for (i = 0; i < fields_count; ++i)
    switch (tdp->fields.data[i].mr_type)
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
	tdp_ = mr_get_td_by_name (tdp->fields.data[i].type);
	if (tdp_)
	  tdp->fields.data[i].mr_type = tdp_->mr_type;
	break;

      case MR_TYPE_BITFIELD:
	tdp_ = mr_get_td_by_name (tdp->fields.data[i].type);
	if (tdp_)
	  tdp->fields.data[i].mr_type_aux = tdp_->mr_type;
	break;

	/*
	  MR_POINTER_STRUCT refers to forward declarations of structures and can't calculate type size at compile time.
	 */
      case MR_TYPE_STRUCT:
      case MR_TYPE_CHAR_ARRAY:
	if (MR_TYPE_EXT_POINTER == tdp->fields.data[i].mr_type_ext)
	  {
	    tdp_ = mr_get_td_by_name (tdp->fields.data[i].type);
	    if (tdp_)
	      tdp->fields.data[i].size = tdp_->size;
	  }
	break;
	
      case MR_TYPE_NONE: /* MR_AUTO type resolution */
	mr_auto_field_detect (&tdp->fields.data[i]);
	break;
	  
      case MR_TYPE_FUNC:
	mr_func_field_detect (&tdp->fields.data[i]);
	break;
	  
      default:
	break;
      }
  return (0);
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
  if (tdp->lookup_by_name.data)
    {
      uint64_t hash_value = mr_hash_str (name);
      mr_fd_t * fdp = tdp->lookup_by_name.data[hash_value % (tdp->lookup_by_name.size / sizeof (tdp->lookup_by_name.data[0]))].fdp;
      if (fdp && (hash_value == fdp->hashed_name.hash_value) && (0 == strcmp (name, fdp->hashed_name.name)))
	return (fdp);
    }
  else
    {
      int i;
      int fields_count = tdp->fields.size / sizeof (tdp->fields.data[0]);
      for (i = 0; i < fields_count; ++i)
	if (0 == strcmp (name, tdp->fields.data[i].hashed_name.name))
	  return (&tdp->fields.data[i]);
    }
  return (NULL);
}

/**
 * Add type to union mr_void_ptr_t.
 * @param tdp a pointer on statically initialized type descriptor
 * @return status, EXIT_SUCCESS or EXIT_FAILURE
 */
static int
mr_register_type_pointer (mr_td_t * tdp)
{
  mr_fd_t * fdp;
  mr_td_t * union_tdp = mr_get_td_by_name ("mr_ptr_t");
  if (NULL == union_tdp)
    return (EXIT_FAILURE);
  if (mr_get_fd_by_name (union_tdp, tdp->hashed_name.name))
    return (EXIT_FAILURE);
  if (union_tdp->fields.alloc_size < 0)
    {
      /* reallocate descriptors of union fields into heap */
      int alloc_size = sizeof (union_tdp->fields.data[0]) + union_tdp->fields.size; /* allocate one additional slot */
      mr_fd_t * fields_data = MR_MALLOC (alloc_size);
      if (NULL == fields_data)
	return (EXIT_FAILURE);
      memcpy (fields_data, union_tdp->fields.data, union_tdp->fields.size);
      union_tdp->fields.data = fields_data;
      union_tdp->fields.alloc_size = alloc_size;
    }
  fdp = mr_rarray_append ((void*)&union_tdp->fields, sizeof (union_tdp->fields.data[0]));
  if (NULL == fdp)
    return (EXIT_FAILURE);
  memset (fdp, 0, sizeof (*fdp));
  fdp->type = tdp->hashed_name.name;
  fdp->hashed_name.name = tdp->hashed_name.name;
  fdp->size = tdp->size;
  fdp->offset = 0;
  fdp->mr_type = tdp->mr_type;
  fdp->mr_type_aux = MR_TYPE_VOID;
  fdp->mr_type_ext = MR_TYPE_EXT_POINTER;
  if (union_tdp->lookup_by_name.data)
    MR_FREE (union_tdp->lookup_by_name.data);
  union_tdp->lookup_by_name.data = NULL;
  /* we need to rebuild hash table each time because array with fields descriptors might be reallocated */
  return (mr_build_field_names_hash (union_tdp));
}

static int
mr_register_type_pointer_wrapper (mr_ptr_t key, const void * arg)
{
  mr_register_type_pointer (key.ptr);
  return (0);
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
  if (mr_get_td_by_name (tdp->hashed_name.name))
    return (EXIT_SUCCESS); /* this type is already registered */

  va_start (args, comment);
  ext = va_arg (args, void*);
  va_end (args);

  for (count = 0; MR_TYPE_TRAILING_RECORD != tdp->fields.data[count].mr_type; ++count);
  tdp->fields.size = count * sizeof (tdp->fields.data[0]);
  tdp->fields.alloc_size = -1;
  tdp->fields.ext.ptr = NULL;
  
  if ((NULL != comment) && comment[0])
    tdp->comment = comment;

  if (NULL != ext)
    tdp->ext.ptr = ext;

  mr_anon_unions_extract (tdp);
  mr_check_fields (tdp);
  mr_build_field_names_hash (tdp);

  if (NULL == mr_conf.des.find)
    mr_ic_none_new (&mr_conf.des, mr_hashed_name_cmp, "mr_td_t");
  
  /* NB! not thread safe - only calls from __constructor__ assumed */
  if (NULL == mr_ic_add (&mr_conf.des, (void*)tdp, NULL).ptr)
    return (EXIT_FAILURE);

  tdp->lookup_by_value.root = NULL; /* should be in mr_add_enum, but produces warning for non-enum types due to uninitialized memory */
  if (MR_TYPE_ENUM == tdp->mr_type)
    mr_add_enum (tdp);

  mr_ic_foreach (&mr_conf.des, mr_detect_fields_types, tdp);
  mr_ic_foreach (&mr_conf.des, mr_register_type_pointer_wrapper, tdp);
  return (EXIT_SUCCESS);
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
