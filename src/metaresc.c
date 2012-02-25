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
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdarg.h>

#include <tsearch.h>
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
    .data = NULL,
    .size = 0,
    .alloc_size = 0,
  },
#ifndef MR_TREE_LOOKUP
  .hash = {
    .ra = { /**< resizable array with hash for type descriptors */
      .data = NULL, /* descriptors mr_rarray_t */
      .size = 0,
      .alloc_size = 0,
    },
  },
#else /* MR_TREE_LOOKUP */
  .tree = NULL,
#endif /* MR_TREE_LOOKUP */
  .enum_by_name = NULL,
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
static void __attribute__((destructor)) mr_cleanup (void)
{
  mr_td_t * void_ptr_tdp = mr_get_td_by_name ("mr_ptr_t");
  
  void dummy_free_func (void * nodep) {}
  
  int free_lookup_tree (mr_td_t * tdp, void * arg)
  {
    tdestroy (tdp->lookup_by_value, dummy_free_func);
    tdp->lookup_by_value = NULL;
    if (tdp->lookup_by_name.data)
      MR_FREE (tdp->lookup_by_name.data);
    tdp->lookup_by_name.data = NULL;
    tdp->lookup_by_name.size = tdp->lookup_by_name.alloc_size = 0;
    return (0);
  }

  if ((void_ptr_tdp) && (void_ptr_tdp->fields.alloc_size > 0) && (void_ptr_tdp->fields.data))
    {
      MR_FREE (void_ptr_tdp->fields.data);
      void_ptr_tdp->fields.data = NULL;
    }
  
  tdestroy (mr_conf.enum_by_name, dummy_free_func);
  mr_conf.enum_by_name = NULL;
  mr_td_foreach (free_lookup_tree, NULL);
  
#ifndef MR_TREE_LOOKUP
  if (mr_conf.hash.ra.data)
    MR_FREE (mr_conf.hash.ra.data);
  mr_conf.hash.ra.data = NULL;
  mr_conf.hash.ra.size = mr_conf.hash.ra.alloc_size = 0;
#else /* MR_TREE_LOOKUP */
  tdestroy (mr_conf.tree, dummy_free_func);
  mr_conf.tree = NULL;
#endif /* MR_TREE_LOOKUP */

  if (mr_conf.des.data)
    MR_FREE (mr_conf.des.data);
  mr_conf.des.data = NULL;
  mr_conf.des.size = mr_conf.des.alloc_size = 0;
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
	  mr_fd_t * fdp = tdp->fields.data;
	  for (i = 0; fdp[i].mr_type != MR_TYPE_TRAILING_RECORD; ++i)
	    messages[fdp[i].param.enum_value] = fdp[i].comment;
	}
      messages_inited = !0;
    }

  if ((message_id >= 0) && (message_id <= sizeof (messages) / sizeof (messages[0])) && messages[message_id])
    format = messages[message_id];

  vasprintf (&message, format, args);

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
  ptrdes->fd.name = NULL;
  ptrdes->fd.hash_value = 0;
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
  ptrdes->flags = MR_PDF_NONE;
  ptrdes->union_field_name = NULL;
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
 * Comparator for mr_ra_mr_ptrdes_t sorting by idx field
 * @param a pointer on one mr_ptrdes_t
 * @param b pointer on another mr_ptrdes_t
 * @return comparation sign
 */
static int mr_cmp_idx (const void * a, const void * b)
{
  return (((const mr_ptrdes_t*)a)->idx - ((const mr_ptrdes_t*)b)->idx);
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
  int to_free = 0;
  int count = ptrs.ra.size / sizeof (ptrs.ra.data[0]);
      
  for (i = 0; i < count; ++i)
    switch (ptrs.ra.data[i].fd.mr_type_ext)
      {
      case MR_TYPE_EXT_POINTER:
      case MR_TYPE_EXT_RARRAY_DATA:
	if ((NULL != *(void**)ptrs.ra.data[i].data) && (ptrs.ra.data[i].ref_idx < 0))
	  ptrs.ra.data[to_free++] = ptrs.ra.data[i];
	break;
      case MR_TYPE_EXT_NONE:
	if ((MR_TYPE_STRING == ptrs.ra.data[i].fd.mr_type) &&
	    (NULL != *(char**)ptrs.ra.data[i].data) && (ptrs.ra.data[i].ref_idx < 0))
	  ptrs.ra.data[to_free++] = ptrs.ra.data[i];
	break;
      default:
	break;
      }
  /* sort out nodes to the end of the array */
  qsort (ptrs.ra.data, to_free, sizeof (ptrs.ra.data[0]), mr_cmp_idx);
  for (i = to_free - 1; i >= 0; --i)
    switch (ptrs.ra.data[i].fd.mr_type_ext)
      {
      case MR_TYPE_EXT_POINTER:
      case MR_TYPE_EXT_RARRAY_DATA:
	MR_FREE (*(void**)ptrs.ra.data[i].data);
	break;
      case MR_TYPE_EXT_NONE:
	MR_FREE (*(char**)ptrs.ra.data[i].data);
	break;
      default:
	break;
      }
  if (ptrs.ra.data)
    MR_FREE (ptrs.ra.data);
  return (EXIT_SUCCESS);
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

/**
 * Type descriptors collection iterator.
 * @param func function for type descriptors processing
 * @param args auxiliary arguments
 * @return flag that cycle was not completed
 */
int
mr_td_foreach (int (*func) (mr_td_t*, void*), void * args)
{
  int count = mr_conf.des.size / sizeof (mr_conf.des.data[0]);
  int i;

  for (i = 0; i < count; ++i)
    if (func (mr_conf.des.data[i].tdp, args))
      return (!0);
  return (0);
}

#ifndef MR_TREE_LOOKUP
/**
 * Update hash with type descriptors
 * @param tdp root of linked list with type descriptors
 * @param hash mr_rarray_t with hash table for type descriptors
 * @return void
 */
static void
mr_update_td_hash (mr_td_t * tdp, mr_ra_mr_td_ptr_t * hash)
{
  int size;
  mr_td_ptr_t * x;

  tdp->hash_value = mr_hash_str (tdp->type);

  if (NULL == hash->ra.data)
    hash->ra.alloc_size = hash->ra.size = 0;

  if (0 == hash->ra.size)
    {
      /* hash size is not defined. Let it be doubled number of elements in the list */
      size = 0;
      int td_count (mr_td_t * tdp, void * args) { ++size; return (0); }
      mr_td_foreach (td_count, NULL);
      hash->ra.size = 2 * size * sizeof (hash->ra.data[0]);
      if (hash->ra.data)
	MR_FREE (hash->ra.data);
      hash->ra.data = NULL;
    }
  else
    {
      /* Lets calculate hash bucket for new element */
      x = &hash->ra.data[tdp->hash_value % (hash->ra.size / sizeof (hash->ra.data[0]))];
      if (NULL == x->tdp)
	x->tdp = tdp; /* bucket was free and hash resize is not required */ 
      else
	{
	  if (x->tdp->hash_value == tdp->hash_value) /* hashes matched and non-collision hash table is not possible */
	    {
	      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_TYPES_HASHES_MATCHED, x->tdp->type, tdp->type);
	      return;
	    }
	  /* we have a collision, so we will need to find new hash size to avoid collisions */ 
	  MR_FREE (hash->ra.data);
	  hash->ra.data = NULL;
	  hash->ra.size = hash->ra.alloc_size = 0;
	}
    }

  while (NULL == hash->ra.data)
    {
      mr_td_ptr_t * array;
      int i;
      /* we need to find next prime number greater then hash->ra.size */ 
      size = ((hash->ra.size / sizeof (hash->ra.data[0])) | 1) + 2;
      while (!is_prime (size))
	size += 2;
      hash->ra.alloc_size = hash->ra.size = size * sizeof (hash->ra.data[0]);
      array = MR_MALLOC (hash->ra.alloc_size);
      /* check memory allocation */
      if (NULL == array)
	{
	  MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	  hash->ra.alloc_size = hash->ra.size = 0;
	  return;
	}
      for (i = 0; i < size; ++i)
	array[i].tdp = NULL;
      /* populate list elements into hash table */
      int td_populate (mr_td_t * tdp, void * args)
      {
	mr_td_ptr_t * x = &array[tdp->hash_value % size];
	/* check for collision */
	if (x->tdp)
	  return (!0);
	x->tdp = tdp;
	return (0);
      }
      /* check that all elements were successfully populated into the hash table */
      if (mr_td_foreach (td_populate, NULL))
	MR_FREE (array); /* otherwise try to find new hash size */
      else
	hash->ra.data = array;
    }
}

#else /* MR_TREE_LOOKUP */

/**
 * Comparator for mr_td_t sorting by type field
 * @param a pointer on one mr_td_t
 * @param b pointer on another mr_td_t
 * @return comparation sign
 */
static int
cmp_tdp (const void * x, const void * y)
{
  return (strcmp (((const mr_td_t *) x)->type, ((const mr_td_t *) y)->type));
}

/**
 * Addition of a new type descriptor to lookup structure. Implementation as a RB-tree.
 * @param tdp new type descriptor
 * @param tree pointer on a root pointer of the lookup tree
 */
static void
mr_update_td_tree (mr_td_t * tdp, mr_red_black_tree_node_t ** tree)
{
  mr_td_t ** tdpp = tsearch (tdp, (void*)tree, cmp_tdp);
  if (NULL == tdpp)
    MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
}
#endif /* MR_TREE_LOOKUP */

/**
 * Type descriptor lookup function. Lookup by type name.
 * @param type stringified type name
 * @return pointer on type descriptor
 */
mr_td_t *
mr_get_td_by_name (char * type)
{
  mr_td_t * tdp;

  int td_cmp (mr_td_t * tdp_, void * args)
    {
      if (0 == strcmp (type, tdp_->type))
	{
	  *(mr_td_t**)args = tdp_;
	  return (!0);
	}
      return (0);
    }
  
#ifndef MR_TREE_LOOKUP
  if (mr_conf.hash.ra.data && mr_conf.hash.ra.size)
    {
      uint64_t hash_value = mr_hash_str (type);
      tdp = mr_conf.hash.ra.data[hash_value % (mr_conf.hash.ra.size / sizeof (mr_conf.hash.ra.data[0]))].tdp;
      if (tdp && (hash_value == tdp->hash_value) && (0 == strcmp (type, tdp->type)))
	return (tdp);
      return (NULL);
    }
#else /* MR_TREE_LOOKUP */
  if (mr_conf.tree)
    {
      mr_td_t td = { .type = type };
      mr_td_t ** tdpp = tfind (&td, (void*)&mr_conf.tree, cmp_tdp);
      if (tdpp)
	return (*tdpp);
      else
	return (NULL);
    }
#endif /* MR_TREE_LOOKUP */
  if (mr_td_foreach (td_cmp, &tdp))
    return (tdp);
  return (NULL);
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
    if (MR_TYPE_ANON_UNION == tdp->fields.data[i].mr_type)
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
	      /* offset of union memebers may differ from offset of anonymous unionplace holder */
	      if (fd.offset != 0) /* MR_NONE and MR_END_ANON_UNION has zero offset */
		tdp->fields.data[i].offset = fd.offset;
	      fd.offset = 0; /* reset offset to zero */
	      tdp->fields.data[count] = fd;
	      if (fd.size > tdp_->size)
		tdp_->size = fd.size; /* find union max size member */
	    }
	  tdp->fields.data[count].mr_type = MR_TYPE_TRAILING_RECORD; /* trailing record */
	  tdp_->mr_type = MR_TYPE_ANON_UNION;
	  sprintf (tdp_->type, MR_TYPE_ANONYMOUS_UNION_TEMPLATE, mr_type_anonymous_union_cnt++);
	  tdp_->attr = tdp->fields.data[i].comment; /* anonymous union stringified attributes are saved into comments field */
	  tdp_->comment = tdp->fields.data[count].comment; /* copy comment from MR_END_ANON_UNION record */
	  tdp_->fields.data = &tdp->fields.data[count - fields_count + 1];

	  tdp->fields.data[i].comment = tdp->fields.data[count].comment; /* copy comment from MR_END_ANON_UNION record */
	  tdp->fields.size -= fields_count * sizeof (tdp->fields.data[0]);
	  count -= fields_count;
	  tdp->fields.data[i].type = tdp_->type;
	  tdp->fields.data[i].size = tdp_->size;

	  if (mr_add_type (tdp_, NULL, NULL))
	    {
	      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_ANON_UNION_TYPE_ERROR, tdp_->type);
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
cmp_enums_by_value (const void * x, const void * y)
{
  return ((((const mr_fd_t *) x)->param.enum_value > ((const mr_fd_t *) y)->param.enum_value) - (((const mr_fd_t *) x)->param.enum_value < ((const mr_fd_t *) y)->param.enum_value));
}

/**
 * comparator for mr_fd_t sorting by enum name
 * @param x pointer on one mr_fd_t
 * @param y pointer on another mr_fd_t
 * @return comparation sign
 */
static int
cmp_enums_by_name (const void * x, const void * y)
{
  return (strcmp (((const mr_fd_t *) x)->name, ((const mr_fd_t *) y)->name));
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
  
  tdp->lookup_by_value = NULL;
  for (i = 0; i < count; ++i)
    {
      /* adding to global lookup table by enum literal names */
      mr_fd_t ** fdpp = tsearch (&tdp->fields.data[i], (void*)&mr_conf.enum_by_name, cmp_enums_by_name);  
      if (NULL == fdpp)
	{
	  MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	  return (EXIT_FAILURE);
	}
      if (*fdpp != &tdp->fields.data[i])
	{
	  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_DUPLICATED_ENUMS, (*fdpp)->name, tdp->type);
	  return (EXIT_FAILURE);
	}
      /* adding to local lookup table by enum values */
      fdpp = tsearch (&tdp->fields.data[i], (void*)&tdp->lookup_by_value, cmp_enums_by_value);  
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
  mr_fd_t ** fdpp = tfind (&fd, (void*)&tdp->lookup_by_value, cmp_enums_by_value);
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
  mr_fd_t fd = { .name = name };
  mr_fd_t ** fdpp = tfind (&fd, (void*)&mr_conf.enum_by_name, cmp_enums_by_name);
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
  int i;
  for (i = 0; i < fdp->param.bitfield_param.bitfield.size; ++i)
    if (fdp->param.bitfield_param.bitfield.data[i])
      break;
  fdp->offset = i;
  for (i = 0; i < 8; ++i)
    if (fdp->param.bitfield_param.bitfield.data[fdp->offset] & (1 << i))
      break;
  fdp->param.bitfield_param.shift = i;
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
      char * name = fdp->name;
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
      if (tdp->fields.data[i].name && tdp->fields.data[j].name && (0 == strcmp (tdp->fields.data[i].name, tdp->fields.data[j].name)))
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_DUPLICATED_FIELDS, tdp->fields.data[i].name, tdp->type);
  
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
    tdp->fields.data[i].hash_value = mr_hash_str (tdp->fields.data[i].name);

  /* sanity check for hash value collision */
  for (i = 0; i < fields_count; ++i)
    for (j = i + 1; j < fields_count; ++j)
      if (tdp->fields.data[i].hash_value == tdp->fields.data[j].hash_value)
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
	  mr_fd_ptr_t * x = &array[tdp->fields.data[i].hash_value % size];
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
mr_detect_fields_types (mr_td_t * tdp, void * args)
{
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
      if (fdp && (hash_value == fdp->hash_value) && (0 == strcmp (name, fdp->name)))
	return (fdp);
    }
  else
    {
      int i;
      int fields_count = tdp->fields.size / sizeof (tdp->fields.data[0]);
      for (i = 0; i < fields_count; ++i)
	if (0 == strcmp (name, tdp->fields.data[i].name))
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
  if (mr_get_fd_by_name (union_tdp, tdp->type))
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
  fdp->type = tdp->type;
  fdp->name = tdp->type;
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
mr_register_type_pointer_wrapper (mr_td_t * tdp, void * arg)
{
  mr_register_type_pointer (tdp);
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
  if (mr_get_td_by_name (tdp->type))
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
  
  /* NB! not thread safe - only calls from __constructor__ assumed */
  {
    mr_td_t ** tdpp = mr_rarray_append ((void*)&mr_conf.des, sizeof (mr_conf.des.data[0]));
    if (NULL == tdpp)
      {
	MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	return (EXIT_FAILURE);
      }
    *tdpp = tdp;
  }
#ifndef MR_TREE_LOOKUP
  mr_update_td_hash (tdp, &mr_conf.hash);
#else /* MR_TREE_LOOKUP */
  mr_update_td_tree (tdp, &mr_conf.tree);
#endif /*  MR_TREE_LOOKUP */
  tdp->lookup_by_value = NULL; /* should be in mr_add_enum, but produces warning for non-enum types due to uninitialized memory */
  if (MR_TYPE_ENUM == tdp->mr_type)
    mr_add_enum (tdp);

  mr_td_foreach (mr_detect_fields_types, tdp);
  mr_td_foreach (mr_register_type_pointer_wrapper, tdp);
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
