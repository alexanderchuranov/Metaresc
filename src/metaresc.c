/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

#define __USE_GNU
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <setjmp.h>
#include <ctype.h>

#include <metaresc.h>
#include <mr_save.h>
#include <mr_ic.h>
#include <mr_stringify.h>
#include <mr_hsort.h>

#define MR_MODE DESC /* we'll need descriptors of our own types */
#include <mr_protos.h>
#undef MR_MODE

#undef MR_DESCRIPTOR_PREFIX
#define MR_DESCRIPTOR_PREFIX(ID, MR_TYPE_NAME) mr_td_ ## ID
#undef MR_CONSTRUCTOR_PREFIX
#define MR_CONSTRUCTOR_PREFIX(ID, MR_TYPE_NAME) mr_init_ ## ID

MR_FOREACH (MR_TYPEDEF_DESC_BI,
	    MR_BUILTIN_TYPES,
	    char *, char*, va_list,
	    uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t, mr_uint128_t, mr_int128_t,
	    mr_uintmax_t, mr_intmax_t, mr_string_t, mr_hash_value_t, mr_offset_t, mr_size_t, size_t, ssize_t,
	    string_t, long_int_t, long_long_int_t, uintptr_t, intptr_t, mr_enum_value_type_t, long_double_t,
	    complex_float_t, complex_double_t, complex_long_double_t);

void * mr_calloc (const char * filename, const char * function, int line, size_t count, size_t size) { return (calloc (count, size)); }
void * mr_realloc (const char * filename, const char * function, int line, void * ptr, size_t size) { return (realloc (ptr, size)); }
void mr_free (const char * filename, const char * function, int line, void * ptr) { free (ptr); }

#define MR_INT_TYPES (0 MR_FOREACH (MR_ONE_SHIFT, MR_TYPE_CHAR, MR_TYPE_BOOL, MR_TYPE_INT8, MR_TYPE_UINT8, MR_TYPE_INT16, MR_TYPE_UINT16, MR_TYPE_INT32, MR_TYPE_UINT32, MR_TYPE_INT64, MR_TYPE_UINT64, MR_TYPE_INT128, MR_TYPE_UINT128, MR_TYPE_ENUM))

#define MR_BASIC_TYPES (0 MR_FOREACH (MR_ONE_SHIFT, MR_TYPE_STRING, MR_TYPE_CHAR_ARRAY, MR_TYPE_CHAR, MR_TYPE_BOOL, MR_TYPE_INT8, MR_TYPE_UINT8, MR_TYPE_INT16, MR_TYPE_UINT16, MR_TYPE_INT32, MR_TYPE_UINT32, MR_TYPE_INT64, MR_TYPE_UINT64, MR_TYPE_INT128, MR_TYPE_UINT128, MR_TYPE_FLOAT, MR_TYPE_COMPLEX_FLOAT, MR_TYPE_DOUBLE, MR_TYPE_COMPLEX_DOUBLE, MR_TYPE_LONG_DOUBLE, MR_TYPE_COMPLEX_LONG_DOUBLE))

#define MR_TYPED_TYPES (0 MR_FOREACH (MR_ONE_SHIFT, MR_TYPE_STRUCT, MR_TYPE_ENUM, MR_TYPE_UNION, MR_TYPE_ANON_UNION, MR_TYPE_NAMED_ANON_UNION))

/** Metaresc configuration structure */
mr_conf_t mr_conf = {
  .mr_mem = { /**< all memory functions may be replaced on user defined */
    .calloc = mr_calloc, /**< Pointer to malloc function. */
    .realloc = mr_realloc, /**< Pointer to realloc function. */
    .free = mr_free, /**< Pointer to free function. */
  },
  .log_level = MR_LL_ERROR, /**< default log level ERROR */
  .msg_handler = NULL, /**< pointer on user defined message handler */
  .cache_func_resolve = true,
  .type_by_name = {
    .ic_type = MR_IC_UNINITIALIZED,
  },
  .enum_by_name = {
    .ic_type = MR_IC_UNINITIALIZED,
  },
  .fields_names = {
    .ic_type = MR_IC_UNINITIALIZED,
  },
  .output_format = { [0 ... MR_TYPE_LAST - 1] = NULL, },
  .list = NULL,
};

MR_MEM_INIT ( , __attribute__((constructor,weak)));

#ifndef HAVE_BUILTIN_DUMP_STRUCT_EXTRA_ARGS
__thread mr_get_struct_type_name_t mr_get_struct_type_name_ctx;

int
mr_get_struct_type_name (const char * fmt, ...)
{
  return (mr_get_struct_type_name_extra (&mr_get_struct_type_name_ctx, fmt));
}
#else /* HAVE_BUILTIN_DUMP_STRUCT_EXTRA_ARGS */
int
mr_get_struct_type_name (const char * fmt, char * value)
{
  return (0);
}

TYPEDEF_UNION (mr_dump_struct_types_union_t,
	       uint8_t _uint8,
	       uint16_t _uint16,
	       uint32_t _uint32,
	       uint64_t _uint64,
	       float _float,
	       double _double,
	       long double _long_double,
	       (signed char, sc),
	       (unsigned char, uc),
	       (signed short, ss),
	       (unsigned short, us),
	       (signed int, si),
	       (unsigned int, ui),
	       (signed long, sl),
	       (unsigned long, ul),
	       (signed long long, sll),
	       (unsigned long long, ull),
	       (void *, _ptr),
	       (uint8_t, dump, [sizeof (long double)]),
	       );

static void
mr_dump_struct_type_add_field (mr_dump_struct_type_ctx_t * ctx,
			       char * type,
			       char * name,
			       mr_type_t mr_type,
			       mr_dump_struct_types_union_t * value)
{
  int i;
  mr_offset_t offset = 0;
  int fields_count = ctx->btdp->td.param.struct_param.fields_size / sizeof (ctx->btdp->td.param.struct_param.fields[0]);
  
  switch (mr_type)
    {
    case MR_TYPE_INT32:
      if (0 == (value->_uint32 >> __CHAR_BIT__))
	mr_type = MR_TYPE_BOOL;
      __attribute__ ((fallthrough));

    case MR_TYPE_INT8:
    case MR_TYPE_UINT8:
    case MR_TYPE_INT16:
    case MR_TYPE_UINT16:
    case MR_TYPE_UINT32:
    case MR_TYPE_INT64:
    case MR_TYPE_UINT64:
    case MR_TYPE_LONG_DOUBLE:
    case MR_TYPE_STRING:
    case MR_TYPE_POINTER:
      offset = value->_uint8;
      break;

    case MR_TYPE_DOUBLE:
      if (0 == value->_uint16)
	{
	  mr_type = MR_TYPE_FLOAT;
	  value->_float = value->_double;
	}
      offset = value->_uint8;
      break;

    case MR_TYPE_NONE:
      offset = value->_ptr - ctx->struct_ptr;
      break;

    default:
      return;
    }

  if (ctx->offset_byte != 0)
    {
      if (MR_TYPE_NONE == mr_type)
	return;
      for (i = 0; i < fields_count; ++i)
	if (0 == strcmp (ctx->btdp->td.param.struct_param.fields[i]->name.str, name))
	  break;
      if (i < fields_count)
	ctx->btdp->td.param.struct_param.fields[i]->offset += offset << (__CHAR_BIT__ * ctx->offset_byte);
      return;
    }

  mr_basic_type_td_t * basic_type_td = MR_REALLOC (ctx->btdp, sizeof (*basic_type_td) + (sizeof (basic_type_td->fd[1]) + sizeof (basic_type_td->fd_ptr)) * fields_count);
  if (NULL == basic_type_td)
    {
      MR_FREE (ctx->btdp);
      ctx->btdp = NULL;
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      longjmp (ctx->_jmp_buf, !0);
    }

  mr_fd_t * fdp = &basic_type_td->fd[fields_count - 1];
  memset (fdp, 0, sizeof (*fdp));
  fdp->mr_type = mr_type;
  fdp->type = type;
  fdp->name.str = name;
  fdp->offset = offset;
  fdp->size = mr_type_size (mr_type);

  basic_type_td->td.param.struct_param.fields = (mr_fd_t**)&basic_type_td->fd[fields_count];
  for (i = 0; i < fields_count; ++i)
    basic_type_td->td.param.struct_param.fields[i] = &basic_type_td->fd[i];
  basic_type_td->td.param.struct_param.fields[i] = NULL;
  basic_type_td->td.param.struct_param.fields_size += sizeof (basic_type_td->td.param.struct_param.fields[0]);
  ctx->btdp = basic_type_td;
}

int
mr_dump_struct_type_detection (mr_dump_struct_type_ctx_t * ctx, const char * fmt, ...)
{
  va_list args;
  va_start (args, fmt);

#define FMT "%s%s %s = "
#define FIRST_LEVEL_INDENT "  "

  if (strncmp (fmt, FMT, sizeof (FMT) - sizeof ("")) == 0)
    {
      char * indent = va_arg (args, char *);
      char * type = va_arg (args, char *);
      char * name = va_arg (args, char *);
      if (strcmp (indent, FIRST_LEVEL_INDENT) == 0)
	{
	  mr_dump_struct_types_union_t value;
	  mr_type_t mr_type = MR_TYPE_LAST;

	  memset (&value, 0, sizeof (value));
	  fmt += sizeof (FMT) - sizeof ("");

#define CASE_(FIELD, MR_TYPE) FIELD = va_arg (args, typeof (0 + (typeof (FIELD))0)); mr_type = MR_TYPE;
#define CASE(FIELD, ...) MR_IF_ELSE (MR_IS_EMPTY (__VA_ARGS__)) \
	    (CASE_ (FIELD, MR_TYPE_DETECT (typeof (FIELD))))	\
	    (CASE_ (FIELD, __VA_ARGS__))
	  
	  if (strcmp (fmt, "\"%.32s\"\n") == 0) { CASE (value._ptr, MR_TYPE_STRING) }
	  else if (strcmp (fmt, "*%p\n") == 0) { CASE (value._ptr, MR_TYPE_NONE) }
	  else if (strcmp (fmt, "%p\n") == 0) { CASE (value._ptr, MR_TYPE_POINTER) }
	  else if (strcmp (fmt, "%hhd\n") == 0) { CASE (value.sc) }
	  else if (strcmp (fmt, "%hhu\n") == 0) { CASE (value.uc) }
	  else if (strcmp (fmt, "%hd\n") == 0) { CASE (value.ss) }
	  else if (strcmp (fmt, "%hu\n") == 0) { CASE (value.us) }
	  else if (strcmp (fmt, "%d\n") == 0) { CASE (value.si) }
	  else if (strcmp (fmt, "%u\n") == 0) { CASE (value.ui) }
	  else if (strcmp (fmt, "%ld\n") == 0) { CASE (value.sl) }
	  else if (strcmp (fmt, "%lu\n") == 0) { CASE (value.ul) }
	  else if (strcmp (fmt, "%lld\n") == 0) { CASE (value.sll) }
	  else if (strcmp (fmt, "%llu\n") == 0) { CASE (value.ull) }
	  else if (strcmp (fmt, "%f\n") == 0) { CASE (value._double) }
	  else if (strcmp (fmt, "%Lf\n") == 0) { CASE (value._long_double) }

	  if (mr_type != MR_TYPE_LAST)
	    mr_dump_struct_type_add_field (ctx, type, name, mr_type, &value);
	}
    }

  va_end (args);
  return (0);
}
#endif /* HAVE_BUILTIN_DUMP_STRUCT_EXTRA_ARGS */

int
mr_get_struct_type_name_extra (mr_get_struct_type_name_t * ctx, const char * fmt, ...)
{
  ctx->type_name = NULL;
  
  if (0 == strcmp (fmt, "%s"))
    {
      va_list args;
      va_start (args, fmt);
      fmt = va_arg (args, char *);
      va_end (args);
    }

  if (strncmp (fmt, MR_STRUCT_KEYWORD " ", sizeof (MR_STRUCT_KEYWORD)) == 0)
    fmt += sizeof (MR_STRUCT_KEYWORD);

  if (strncmp (fmt, MR_UNION_KEYWORD " ", sizeof (MR_UNION_KEYWORD)) == 0)
    fmt += sizeof (MR_UNION_KEYWORD);

  char * tail = strchr (fmt, ' ');
  if (NULL == tail)
    tail = strchr (fmt, 0);

  mr_substr_t substr;
  substr.str = (char*)fmt;
  substr.length = tail - fmt;
  ctx->type_name = mr_get_static_field_name_from_substring (&substr);
  longjmp (ctx->_jmp_buf, !0);
  return (0);
}

static mr_status_t
mr_conf_cleanup_visitor (mr_ptr_t key, const void * context)
{
  mr_td_t * tdp = key.ptr;
  mr_ic_free (&tdp->param.struct_param.field_by_name);

  int i, count = tdp->param.struct_param.fields_size / sizeof (tdp->param.struct_param.fields[0]);
  for (i = 0; i < count; ++i)
    {
      mr_fd_t * fdp = tdp->param.struct_param.fields[i];

      if ((MR_TYPE_ARRAY == fdp->mr_type) && (MR_TYPE_POINTER == fdp->mr_type_aux))
	{
	  mr_ic_t ** icpp = &fdp->param.array_param.pointer_param->param.union_param;
	  if (*icpp)
	    {
	      mr_ic_free (*icpp);
	      MR_FREE (*icpp);
	      *icpp = NULL;
	    }
	}
      if ((MR_TYPE_POINTER == fdp->mr_type) && (MR_TYPE_POINTER == fdp->mr_type_aux))
	{
	  mr_ic_t ** icpp = &fdp->param.pointer_param.pointer_param->param.union_param;
	  if (*icpp)
	    {
	      mr_ic_free (*icpp);
	      MR_FREE (*icpp);
	      *icpp = NULL;
	    }
	}

      switch (fdp->mr_type)
	{
	case MR_TYPE_ARRAY:
	case MR_TYPE_POINTER:
	  if (MR_TYPE_UNION != fdp->mr_type_aux)
	    continue;
	  __attribute__ ((fallthrough));
	case MR_TYPE_UNION:
	case MR_TYPE_ANON_UNION:
	case MR_TYPE_NAMED_ANON_UNION:
	  if (fdp->param.union_param)
	    {
	      mr_ic_free (fdp->param.union_param);
	      MR_FREE (fdp->param.union_param);
	      fdp->param.union_param = NULL;
	    }
	  __attribute__ ((fallthrough));
	default:
	  break;
	}
    }
  
  if (tdp->is_dynamically_allocated)
    MR_FREE (tdp);

  return (MR_SUCCESS);
}

/**
 * Memory cleanp handler.
 */
static void __attribute__((destructor))
mr_cleanup (void)
{
  mr_ic_foreach (&mr_conf.type_by_name, mr_conf_cleanup_visitor, NULL);
  mr_ic_free (&mr_conf.enum_by_name);
  mr_ic_free (&mr_conf.type_by_name);
  mr_ic_free (&mr_conf.fields_names);
#ifdef HAVE_LIBXML2
  xmlCleanupParser ();
#endif /* HAVE_LIBXML2 */
}

/**
 * Allocate new string and copy first 'size' chars from str.
 * For compilers without GNU extension
 *
 * @param str string to duplicate
 * @param size size to duplicate
 * @return A pointer on newly allocated string
 */
char *
mr_strndup (const char * str, size_t size)
{
  char * _str = (char*)str;
  int _size;
  for (_size = 0; (_size < size) && *_str++; ++_size);
  _str = MR_CALLOC (_size + 1, sizeof (*_str));
  if (NULL == _str)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (NULL);
    }
  memcpy (_str, str, _size);
  _str[_size] = 0;
  return (_str);
}

char *
mr_strdup (const char * str)
{
  int _size = strlen (str) + 1;
  char * _str = MR_CALLOC (_size, sizeof (*_str));
  if (NULL == _str)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (NULL);
    }
  memcpy (_str, str, _size);
  return (_str);
}

/**
 * Extract bits of bit-field, extend sign bits if needed.
 * @param ptrdes pointer descriptor
 * @param value pointer on variable for bit-field value
 * @return status 
 */
mr_status_t
mr_save_bitfield_value (mr_ptrdes_t * ptrdes, uint64_t * value)
{
  uint8_t * ptr = ptrdes->data.ptr;
  uint64_t _value;
  int i;

  _value = *ptr++ >> ptrdes->fdp->param.bitfield_param.shift;
  for (i = __CHAR_BIT__ - ptrdes->fdp->param.bitfield_param.shift; i < ptrdes->fdp->param.bitfield_param.width; i += __CHAR_BIT__)
    _value |= ((uint64_t)*ptr++) << i;
  _value &= (2LL << (ptrdes->fdp->param.bitfield_param.width - 1)) - 1;

  mr_type_t mr_type = ptrdes->mr_type_aux;
  if ((MR_TYPE_ENUM == mr_type) && (ptrdes->tdp != NULL))
    mr_type = ptrdes->tdp->param.enum_param.mr_type_effective;

  switch (mr_type)
    {
    case MR_TYPE_INT8:
    case MR_TYPE_INT16:
    case MR_TYPE_INT32:
    case MR_TYPE_INT64:
      /* extend sign bit */
      if (_value & (1 << (ptrdes->fdp->param.bitfield_param.width - 1)))
	_value |= -1 - ((2LL << (ptrdes->fdp->param.bitfield_param.width - 1)) - 1);
      break;
    default:
      break;
    }
  *value = _value;
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
  uint8_t * ptr = ptrdes->data.ptr;
  uint64_t _value = *value;
  int i;

  _value &= (2LL << (ptrdes->fdp->param.bitfield_param.width - 1)) - 1;
  if (ptrdes->fdp->param.bitfield_param.shift + ptrdes->fdp->param.bitfield_param.width >= __CHAR_BIT__)
    *ptr &= ((1 << ptrdes->fdp->param.bitfield_param.shift) - 1);
  else
    *ptr &= (-1 - ((1 << (ptrdes->fdp->param.bitfield_param.shift + ptrdes->fdp->param.bitfield_param.width)) - 1)) | ((1 << ptrdes->fdp->param.bitfield_param.shift) - 1);
  *ptr++ |= _value << ptrdes->fdp->param.bitfield_param.shift;
  for (i = __CHAR_BIT__ - ptrdes->fdp->param.bitfield_param.shift; i < ptrdes->fdp->param.bitfield_param.width; i += __CHAR_BIT__)
    if (ptrdes->fdp->param.bitfield_param.width - i >= __CHAR_BIT__)
      *ptr++ = _value >> i;
    else
      {
	*ptr &= -1 - ((1 << (ptrdes->fdp->param.bitfield_param.width - i)) - 1);
	*ptr++ |= _value >> i;
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
mr_rarray_allocate_element (void ** data, ssize_t * size, ssize_t * alloc_size, ssize_t element_size)
{
  if ((NULL == data) || (NULL == size) || (NULL == alloc_size) ||
      (*size < 0) || (element_size < 0))
    return (NULL);
  
  char * _data = *data;
  ssize_t _size = *size;
  ssize_t new_size = _size + element_size;
  if (new_size > *alloc_size)
    {
      ssize_t realloc_size = ((((new_size + 1) << 1) + element_size - 1) / element_size) * element_size;
      if (realloc_size < new_size)
	realloc_size = new_size;
      _data = MR_REALLOC (_data, realloc_size);
      if (NULL == _data)
	{
	  MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	  return (NULL);
	}
      *alloc_size = realloc_size;
      *data = _data;
    }
  *size = new_size;
  return (&_data[_size]);
}

void *
mr_rarray_append (mr_rarray_t * rarray, ssize_t size)
{
  return (mr_rarray_allocate_element (&rarray->data.ptr, &rarray->MR_SIZE, &rarray->alloc_size, size));
}

/**
 * Allocate element for pointer descriptor in resizable array.
 * @param ptrs resizable array with pointers on already saved structures
 * @return Index of pointer in the list or -1 in case of memory operation error.
 * On higher level we need index because array is always reallocating and
 * pointer on element is changing (index remains constant).
 */
int
mr_add_ptr_to_list (mr_ra_ptrdes_t * ptrs)
{
  mr_ptrdes_t * ptrdes = mr_rarray_allocate_element ((void*)&ptrs->ra, &ptrs->size, &ptrs->alloc_size, sizeof (ptrs->ra[0]));
  if (NULL == ptrdes)
    return (-1);
  memset (ptrdes, 0, sizeof (*ptrdes));
  ptrdes->idx = -1; /* NB! To be initialized in depth search in mr_save */
  ptrdes->ref_idx = -1;
  ptrdes->parent = -1;
  ptrdes->first_child = -1;
  ptrdes->last_child = -1;
  ptrdes->prev = -1;
  ptrdes->next = -1;
  return (ptrs->size / sizeof (ptrs->ra[0]) - 1);
}

/**
 * Setup referencies between parent and child node in serialization tree
 * @param parent index of parent node
 * @param child index of child node
 * @param ptrs resizable array with pointers descriptors
 */
void
mr_add_child (int parent, int child, mr_ptrdes_t * ra)
{
  int last_child;

  if (child < 0)
    return;

  ra[child].parent = parent;
  if (parent < 0)
    return;

  last_child = ra[parent].last_child;
  if (last_child < 0)
    ra[parent].first_child = child;
  else
    {
      ra[last_child].next = child;
      ra[child].prev = last_child;
      ra[child].next = -1;
    }
  ra[parent].last_child = child;
}

mr_hash_value_t
mr_hashed_string_get_hash (const mr_hashed_string_t * x)
{
  mr_hashed_string_t * x_ = (mr_hashed_string_t*)x;
  if (0 == x_->hash_value)
    x_->hash_value = mr_hash_str (x_->str);
  return (x_->hash_value);
}

mr_hash_value_t
mr_hashed_string_get_hash_ic (mr_ptr_t x, const void * context)
{
  mr_hashed_string_t * x_ = x.ptr;
  return (mr_hashed_string_get_hash (x_));
}

/**
 * Comparator for mr_hashed_string_t
 * @param x pointer on one mr_hashed_string_t
 * @param y pointer on another mr_hashed_string_t
 * @return comparation sign
 */
int
mr_hashed_string_cmp (const mr_hashed_string_t * x, const mr_hashed_string_t * y)
{
  mr_hash_value_t x_hash_value = mr_hashed_string_get_hash (x);
  mr_hash_value_t y_hash_value = mr_hashed_string_get_hash (y);
  int diff = (x_hash_value > y_hash_value) - (x_hash_value < y_hash_value);
  if (diff)
    return (diff);
  
  if (x->str == y->str)
    return (0);
  
  if (NULL == x->str)
    return (-1);
  if (NULL == y->str)
    return (1);
  
  return (strcmp (x->str, y->str));
}

int
mr_hashed_string_cmp_ic (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const mr_hashed_string_t * x_ = x.ptr;
  const mr_hashed_string_t * y_ = y.ptr;
  return (mr_hashed_string_cmp (x_, y_));
}

mr_hash_value_t
mr_fd_name_get_hash (mr_ptr_t x, const void * context)
{
  mr_fd_t * x_ = x.ptr;
  return (mr_hashed_string_get_hash (&x_->name));
}

mr_hash_value_t
mr_ed_name_get_hash (mr_ptr_t x, const void * context)
{
  mr_ed_t * x_ = x.ptr;
  return (mr_hashed_string_get_hash (&x_->name));
}

/**
 * Comparator for mr_fd_t by name field
 * @param x pointer on one mr_fd_t
 * @param y pointer on another mr_fd_t
 * @return comparation sign
 */
int
mr_fd_name_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const mr_fd_t * x_ = x.ptr;
  const mr_fd_t * y_ = y.ptr;
  return (mr_hashed_string_cmp (&x_->name, &y_->name));
}

int
mr_ed_name_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const mr_ed_t * x_ = x.ptr;
  const mr_ed_t * y_ = y.ptr;
  return (mr_hashed_string_cmp (&x_->name, &y_->name));
}

/**
 * Comparator for mr_fd_t by offset field
 * @param x pointer on one mr_fd_t
 * @param y pointer on another mr_fd_t
 * @return comparation sign
 */
int
mr_fd_offset_cmp (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const mr_fd_t * x_ = x.ptr;
  const mr_fd_t * y_ = y.ptr;
  return ((x_->offset > y_->offset) - (x_->offset < y_->offset));
}

int
mr_fd_offset_cmp_sorting (const mr_ptr_t x, const mr_ptr_t y, const void * context)
{
  const mr_fd_t ** x_ = x.ptr;
  const mr_fd_t ** y_ = y.ptr;
  const mr_fd_t * x_fdp = *x_;
  const mr_fd_t * y_fdp = *y_;
  
  int diff = ((x_fdp->offset > y_fdp->offset) - (x_fdp->offset < y_fdp->offset));
  if (diff)
    return (diff);

  diff = ((x_fdp->size > y_fdp->size) - (x_fdp->size < y_fdp->size));
  if (diff)
    return (diff);

  if ((x_fdp->mr_type == MR_TYPE_BITFIELD) && (y_fdp->mr_type == MR_TYPE_BITFIELD))
    {
      diff = ((x_fdp->param.bitfield_param.shift > y_fdp->param.bitfield_param.shift) -
	      (x_fdp->param.bitfield_param.shift < y_fdp->param.bitfield_param.shift));
      if (diff)
	return (diff);

      diff = ((x_fdp->param.bitfield_param.width > y_fdp->param.bitfield_param.width) -
	      (x_fdp->param.bitfield_param.width < y_fdp->param.bitfield_param.width));
      if (diff)
	return (diff);
    }

  return (0);
}

mr_hash_value_t
mr_td_name_get_hash (mr_ptr_t x, const void * context)
{
  mr_td_t * x_ = x.ptr;
  return (mr_hashed_string_get_hash (&x_->type));
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
  return (mr_hashed_string_cmp (&x_->type, &y_->type));
}

/**
 * Type descriptor lookup function. Lookup by type name.
 * @param type stringified type name
 * @return pointer on type descriptor
 */
mr_td_t *
mr_get_td_by_name_internal (char * type)
{
  mr_hashed_string_t hashed_type = { .str = type, .hash_value = mr_hash_str (type), };
  uintptr_t key = (uintptr_t)&hashed_type - offsetof (mr_td_t, type);
  mr_ptr_t * result = mr_ic_find (&mr_conf.type_by_name, key);
  return (result ? result->ptr : NULL);
}

mr_td_t *
mr_get_td_by_name (char * type)
{
  mr_conf_init ();
  return (mr_get_td_by_name_internal (type));
}

static void mr_init_struct (mr_td_t * tdp); /* recursive call */
/**
 * Preprocessign of a new type. Anonymous unions should be extracted into new independant types.
 * @param tdp pointer on a new type descriptor
 * @return status
 */
static mr_status_t
mr_anon_unions_extract (mr_td_t * tdp)
{
  int count = tdp->param.struct_param.fields_size / sizeof (tdp->param.struct_param.fields[0]);
  int i, j;

  for (i = 0; i < count; ++i)
    {
      mr_fd_t * fdp = tdp->param.struct_param.fields[i];
      if ((MR_TYPE_ANON_UNION == fdp->mr_type) || (MR_TYPE_NAMED_ANON_UNION == fdp->mr_type))
	{
	  static int mr_type_anonymous_union_cnt = 0;
	  mr_td_t * tdp_ = fdp->res.ptr; /* statically allocated memory for new type descriptor */
	  mr_fd_t ** first = &tdp->param.struct_param.fields[i + 1];
	  mr_fd_t * last;
	  int opened = 1;

	  for (j = i + 1; j < count; ++j)
	    {
	      mr_fd_t * fdp_ = tdp->param.struct_param.fields[j];
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
	    int fields_count = j - i; /* additional trailing element with mr_type = MR_TYPE_LAST */
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

	    last = tdp->param.struct_param.fields[count];
	    tdp->param.struct_param.fields[count] = NULL;
	    tdp_->mr_type = fdp->mr_type; /* MR_TYPE_ANON_UNION or MR_TYPE_NAMED_ANON_UNION */
	    sprintf (tdp_->type.str, MR_TYPE_ANONYMOUS_UNION_TEMPLATE, mr_type_anonymous_union_cnt++);
	    tdp_->type.hash_value = mr_hash_str (tdp_->type.str);
	    tdp_->param.struct_param.fields = &tdp->param.struct_param.fields[count - fields_count + 1];

	    fdp->meta = last->meta; /* copy meta from MR_END_ANON_UNION record */
	    fdp->res = last->res;
	    fdp->res_type = last->res_type;
	    fdp->MR_SIZE = last->MR_SIZE;
	    tdp->param.struct_param.fields_size -= fields_count * sizeof (tdp->param.struct_param.fields[0]);
	    count -= fields_count;
	    fdp->type = tdp_->type.str;
	    fdp->size = tdp_->size;
	    
	    /* set name of anonymous union to temporary type name */
	    if (NULL == fdp->name.str)
	      fdp->name.str = fdp->type;
	    else if (0 == fdp->name.str[0])
	      fdp->name.str = fdp->type;
	      
	    fdp->name.hash_value = mr_hash_str (fdp->name.str);

	    mr_init_struct (tdp_);
	    mr_ic_add (&mr_conf.type_by_name, tdp_);
	    mr_add_type (tdp_);
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
mr_enum_value_type_t
mr_get_enum_value (mr_td_t * tdp, void * data)
{
  mr_enum_value_type_t enum_value = 0;
  /*
    GCC caluculates sizeof for the type according alignment, but initialize only effective bytes
    i.e. for typedef enum __attribute__ ((packed, aligned (sizeof (uint16_t)))) {} enum_t;
    sizeof (enum_t) == 2, but type has size only 1 byte
  */
  switch (tdp->param.enum_param.mr_type_effective)
    {
#define CASE_GET_ENUM_BY_TYPE(TYPE) case MR_TYPE_DETECT (TYPE): enum_value = *(TYPE*)data; break;

      MR_FOREACH (CASE_GET_ENUM_BY_TYPE, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t);
#ifdef HAVE_INT128
      MR_FOREACH (CASE_GET_ENUM_BY_TYPE, mr_uint128_t, mr_int128_t);
#endif /* HAVE_INT128 */

    default:
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
      memcpy (&enum_value, data, MR_MIN (tdp->param.enum_param.size_effective, sizeof (enum_value)));
#else /*  __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ */
#error Support for non little endian architectures to be implemented
#endif /* __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ */
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
  mr_ed_t * edp = x.ptr;
  return (edp->value._unsigned);
}

/**
 * comparator for mr_fd_t sorting by enum value
 * @param x pointer on one mr_fd_t
 * @param y pointer on another mr_fd_t
 * @return comparation sign
 */
int
mr_ed_enum_value_cmp (mr_ptr_t x, mr_ptr_t y, const void * context)
{
  mr_ed_t * x_ = x.ptr;
  mr_ed_t * y_ = y.ptr;
  return ((x_->value._unsigned > y_->value._unsigned) -
	  (x_->value._unsigned < y_->value._unsigned));
}

int
mr_ed_enum_value_cmp_sorting (mr_ptr_t x, mr_ptr_t y, const void * context)
{
  return (mr_ed_enum_value_cmp (*(void**)x.ptr, *(void**)y.ptr, context));
}

/**
 * New enum descriptor preprocessing. Enum literal values should be added to global lookup table and enum type descriptor should have a lookup by enum values.
 * @param tdp pointer on a new enum type descriptor
 */
void
mr_init_enum (mr_td_t * tdp)
{
  if (tdp->mr_type != MR_TYPE_ENUM)
    return;

  int i, count;
  for (count = 0; tdp->param.enum_param.enums[count] != NULL; ++count)
    tdp->param.enum_param.enums[count]->mr_type = tdp->param.enum_param.mr_type_effective;
  tdp->param.enum_param.enums_size = count * sizeof (tdp->param.enum_param.enums[0]);

  /*
    Old versions of GCC (e.g. 4.x) might have mismatched sizeof and effectivily used size for enums.
    Enums with __attribute__((packed, aligned (XXX))) have sizeof according to alignment, but compiler uses only bytes required for maximal enum value.
    E.g.
    typedef enum _enum_t __attribute__ ((packed, aligned (sizeof (short)))) {
    ZERO,
    MAX_VAL = 255,
    };
    Variable of this type will have sizeof == sizeof (short), but compiler will use only 1 byte, and second byte will be uninitialized.
    
    Here we determine effective type size.
  */
  switch (tdp->param.enum_param.mr_type_effective)
    {
#define CASE_SET_SIZE_BY_TYPE(TYPE) case MR_TYPE_DETECT (TYPE): tdp->param.enum_param.size_effective = sizeof (TYPE); break;

      MR_FOREACH (CASE_SET_SIZE_BY_TYPE, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t, mr_uint128_t, mr_int128_t);

    default:
      tdp->param.enum_param.size_effective = tdp->size;

      switch (tdp->size)
	{
#define CASE_SET_TYPE_BY_SIZE(TYPE) case sizeof (TYPE): tdp->param.enum_param.mr_type_effective = MR_TYPE_DETECT (TYPE); break;

	  MR_FOREACH (CASE_SET_TYPE_BY_SIZE, uint8_t, uint16_t, uint32_t, uint64_t, mr_uint128_t);
	default:
	  tdp->param.enum_param.mr_type_effective = MR_TYPE_DETECT (int);
	  tdp->param.enum_param.size_effective = sizeof (int);
	  tdp->size = sizeof (int);
	  break;
	}
      break;
    }

  mr_hsort (tdp->param.enum_param.enums, count, sizeof (tdp->param.enum_param.enums[0]), mr_ed_enum_value_cmp_sorting, NULL);

  int non_zero_cnt = 0;
  tdp->param.enum_param.is_bitmask = true;
  for (i = 0; i < count; ++i)
    {
      mr_enum_value_type_t value = tdp->param.enum_param.enums[i]->value._unsigned;

      /*
	there is a corner case when enum has a single negative value which is a highest sign bit
	Compiler will extend sign bit to higher positoins and this value will not be classified
	as a power of two. That's why we need to truncate value to expected bit width before checking.
       */
      value &= (2LL << (tdp->param.enum_param.size_effective * __CHAR_BIT__ - 1)) - 1;

      if (value != 0)
	++non_zero_cnt;
      
      if ((value & (value - 1)) != 0)
	tdp->param.enum_param.is_bitmask = false;
      
      /* adding to global lookup table by enum literal names */
      mr_ptr_t * result = mr_ic_add (&mr_conf.enum_by_name, tdp->param.enum_param.enums[i]);
      if (NULL == result)
	continue;

      mr_ed_t * edp = result->ptr;
      if ((edp != tdp->param.enum_param.enums[i]) &&
	  (edp->value._unsigned != tdp->param.enum_param.enums[i]->value._unsigned))
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_CONFLICTED_ENUMS, edp->name.str, tdp->type.str, value, edp->value._unsigned);
    }

  if (non_zero_cnt < 2)
    tdp->param.enum_param.is_bitmask = false;
}

void
mr_init_func (mr_td_t * tdp)
{
  if (tdp->mr_type != MR_TYPE_FUNC_TYPE)
    return;

  int i;
  for (i = 0; tdp->param.func_param.args[i] != NULL; ++i);
  tdp->param.func_param.size = i * sizeof (tdp->param.func_param.args[0]);
}

/**
 * Get enum by value.
 *
 * @param tdp pointer on a type descriptor
 * @param value enums value
 * @return pointer on enum value descriptor (mr_fd_t*) or NULL is value was not found
 */
mr_ed_t *
mr_get_enum_by_value (mr_td_t * tdp, mr_enum_value_type_t value)
{
  unsigned idx;
  uintptr_t key = (uintptr_t)&value - offsetof (mr_ed_t, value);
  mr_ic_rarray_t ic_rarray = { .ra = (mr_ptr_t*)tdp->param.enum_param.enums, .size = tdp->param.enum_param.enums_size, };
  int diff = mr_ic_sorted_array_find_idx (key, &ic_rarray, mr_ed_enum_value_cmp, NULL, &idx);
  return (diff ? NULL : tdp->param.enum_param.enums[idx]);
}

/**
 * Enum literal name lookup function.
 * @param value address for enum value to store
 * @param name literal name of enum to lookup
 * @return status
 */
mr_ed_t *
mr_get_enum_by_name (char * name)
{
  mr_hashed_string_t hashed_name = { .str = name, .hash_value = mr_hash_str (name), }; 
  uintptr_t key = (uintptr_t)&hashed_name - offsetof (mr_ed_t, name);
  mr_ptr_t * result = mr_ic_find (&mr_conf.enum_by_name, key);
  return (result ? result->ptr : NULL);
}

/**
 * Type name clean up. We need to drop all key words.
 * @param fdp pointer on a field descriptor
 * @return status
 */
static bool
mr_normalize_type (char * type)
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
  static bool isdelimiter [1 << (__CHAR_BIT__ * sizeof (uint8_t))] =
    {
      [0] = true,
      [(uint8_t)' '] = true,
      [(uint8_t)'\t'] = true,
      [(uint8_t)'*'] = true,
    };
  int i;
  char * ptr;
  bool prev_is_space = false;
  bool modified = false;

  if (NULL == type)
    return (false);

  for (i = 0; i < sizeof (keywords) / sizeof (keywords[0]); ++i)
    {
      int length = strlen (keywords[i]);
      ptr = type;
      for (;;)
	{
	  char * found = strstr (ptr, keywords[i]);
	  if (!found)
	    break;
	  if (isdelimiter[(uint8_t)found[length]] && ((found == type) || isdelimiter[(uint8_t)found[-1]]))
	    {
	      memset (found, ' ', length); /* replaced all keywords on spaces */
	      modified = true;
	    }
	  ++ptr; /* keyword might be a part of type name and we need to start search of keyword from next symbol */
	}
    }
  if (modified)
    {
      /* we need to drop all space characters */
      ptr = type;
      for (i = 0; isspace (type[i]); ++i);
      for (; type[i]; ++i)
	if (isspace (type[i]))
	  prev_is_space = true;
	else
	  {
	    if (prev_is_space)
	      *ptr++ = ' ';
	    *ptr++ = type[i];
	    prev_is_space = false;
	  }
      *ptr = 0;
    }
  return (modified);
}

/**
 * Bitfield initialization. We need to calculate offset and shift. Width was initialized by macro.
 * @param fdp pointer on a field descriptor
 * @return status
 */
static void
mr_fd_init_bitfield_params (mr_fd_t * fdp)
{
  int i, j;
  if (fdp->param.bitfield_param.initialized)
    return;
  
  fdp->param.bitfield_param.initialized = true;

  if (NULL == fdp->param.bitfield_param.bitfield)
    return;
  
  for (i = 0; i < fdp->param.bitfield_param.size; ++i)
    if (fdp->param.bitfield_param.bitfield[i])
      break;
  /* if bitmask is clear then there is no need to initialize anything */
  if (i >= fdp->param.bitfield_param.size)
    return;

  fdp->offset = i;
  for (i = 0; i < __CHAR_BIT__; ++i)
    if (fdp->param.bitfield_param.bitfield[fdp->offset] & (1 << i))
      break;
  fdp->param.bitfield_param.shift = i;
  fdp->param.bitfield_param.width = 0;
  for (j = fdp->offset; j < fdp->param.bitfield_param.size; ++j)
    {
      for ( ; i < __CHAR_BIT__; ++i)
	if (fdp->param.bitfield_param.bitfield[j] & (1 << i))
	  ++fdp->param.bitfield_param.width;
	else
	  break;
      if (i < __CHAR_BIT__)
	break;
      i = 0;
    }
}

mr_size_t
mr_type_size (mr_type_t mr_type)
{
#define MR_TYPE_SIZE(TYPE) [MR_TYPE_DETECT (TYPE)] = sizeof (TYPE),
  static size_t types_sizes[MR_TYPE_LAST] =
    {
      [MR_TYPE_VOID] = sizeof (void*),
      [MR_TYPE_POINTER] = sizeof (void*),
      [MR_TYPE_FUNC] = sizeof (void*),
      [MR_TYPE_FUNC_TYPE] = sizeof (void*),
      MR_FOREACH (MR_TYPE_SIZE,
		  string_t, char, bool,
		  int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, mr_int128_t, mr_uint128_t,
		  float, complex_float_t, double, complex_double_t, long_double_t, complex_long_double_t)
    };

  if ((mr_type >= 0) && (mr_type < MR_TYPE_LAST))
    return (types_sizes[mr_type]);
  return (0);
}

/**
 * Add type to union mr_void_ptr_t.
 * @param tdp a pointer on statically initialized type descriptor
 * @return status
 */
static mr_status_t
mr_register_type_pointer (mr_td_t * tdp)
{
  static mr_td_t * union_tdp = NULL;
  /* check that mr_ptr_t have already a registered */
  if (NULL == union_tdp)
    {
      union_tdp = mr_get_td_by_name_internal ("mr_ptr_t");
      if (NULL == union_tdp)
	return (MR_SUCCESS);
    }
  /* check that requested type is already registered */
  if (NULL != mr_get_fd_by_name (union_tdp, tdp->type.str))
    return (MR_SUCCESS);

  /* statically allocated trailing record is used for field descriptor */
  mr_fd_t * fdp = &tdp->mr_ptr_fd;
  if (NULL == fdp)
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
      return (MR_FAILURE);
    }

  *fdp = *union_tdp->param.struct_param.fields[0];
  fdp->type = tdp->type.str;
  fdp->name = tdp->type;
  fdp->size = sizeof (void *);
  fdp->offset = 0;
  fdp->mr_type = MR_TYPE_POINTER;
  fdp->mr_type_aux = tdp->mr_type;
  fdp->tdp = tdp;

  mr_ic_add (&mr_conf.fields_names, &tdp->type);

  return ((NULL == mr_ic_add (&union_tdp->param.struct_param.field_by_name, fdp)) ? MR_FAILURE : MR_SUCCESS);
}

static bool
mr_type_is_a_pointer (char * type)
{
  if ((NULL == type) || (0 == type[0]))
    return (false);
  /* auto detect pointers */
  char * end = &type[strlen (type) - 1];
  if ('*' == *end)
    {
      /* remove whitespaces before * */
      while (isspace (end[-1]))
	--end;
      *end = 0; /* trancate type name */
      return (true);
    }
  return (false);
}

static void
mr_fd_of_array_type (mr_fd_t * fdp)
{
  if (NULL == fdp->type)
    return;

  char * end = &fdp->type[strlen (fdp->type) - 1];
  int row_count = 0, count = 1;
  while (']' == *end)
    {
      char * open_bracket;
      for (open_bracket = end - 1; open_bracket >= fdp->type; --open_bracket)
	if ('[' == *open_bracket)
	  break;
      if (open_bracket < fdp->type)
	break;
      int last_row_count = atoi (open_bracket + 1);
      row_count = count;
      count *= last_row_count;
      end = open_bracket - 1;
    }

  if (row_count != 0)
    {
      fdp->mr_type = MR_TYPE_ARRAY;
      fdp->param.array_param.count = count;
      fdp->param.array_param.row_count = row_count;
    }
}

static mr_td_t *
mr_add_basic_type (char * type, mr_type_t mr_type)
{
  if (!((mr_type > MR_TYPE_NONE) && (mr_type < MR_TYPE_LAST)) ||
      !((MR_BASIC_TYPES >> mr_type) & 1) ||
      (NULL == type))
    return (NULL);

  mr_basic_type_td_t * basic_type_td = MR_CALLOC (1, sizeof (*basic_type_td) + strlen (type) + sizeof (""));
  if (NULL == basic_type_td)
    return (NULL);

  basic_type_td->td.is_dynamically_allocated = true;
  basic_type_td->td.type.str = (void*)&basic_type_td[1];
  strcpy (basic_type_td->td.type.str, type);

  basic_type_td->td.mr_type = mr_type;
  basic_type_td->td.size = mr_type_size (mr_type);

  mr_ic_add (&mr_conf.type_by_name, &basic_type_td->td);
  mr_register_type_pointer (&basic_type_td->td);
  return (&basic_type_td->td);
}

static void
mr_detect_structured_type (mr_structured_type_t * stype)
{
#define MR_TYPES_SKIP_RESOLUTION (0 MR_FOREACH (MR_ONE_SHIFT, MR_TYPE_VOID, MR_TYPE_FUNC))
  if (((MR_TYPES_SKIP_RESOLUTION >> stype->mr_type) & 1) || (NULL == stype->type))
    return;

  int type_name_length = strlen (stype->type);
  char type[type_name_length + 1];

  strcpy (type, stype->type);
  mr_normalize_type (type);
  char * bracket = strchr (type, '[');
  if (bracket)
    *bracket = 0;

  mr_td_t * tdp = mr_get_td_by_name_internal (type);
  if (NULL == tdp)
    {
      if ((MR_BASIC_TYPES >> stype->mr_type) & 1)
	tdp = mr_add_basic_type (type, stype->mr_type);
      else if ((((0 MR_FOREACH (MR_ONE_SHIFT, MR_TYPE_BITFIELD, MR_TYPE_ARRAY)) >> stype->mr_type) & 1)
	       && ((MR_BASIC_TYPES >> stype->mr_type_aux) & 1))
	tdp = mr_add_basic_type (type, stype->mr_type_aux);
      else if (mr_type_is_a_pointer (type))
	{
	  if (stype->mr_type == MR_TYPE_ARRAY)
	    stype->mr_type_aux = MR_TYPE_POINTER;
	  else
	    stype->mr_type = MR_TYPE_POINTER;

	  tdp = mr_get_td_by_name_internal (type);
	  if (NULL == tdp)
	    {
	      if (stype->mr_type == MR_TYPE_ARRAY)
		{
		  if ((MR_BASIC_TYPES >> stype->mr_type_ptr) & 1)
		    tdp = mr_add_basic_type (type, stype->mr_type_ptr);
		}
	      else
		{
		  if ((MR_BASIC_TYPES >> stype->mr_type_aux) & 1)
		    tdp = mr_add_basic_type (type, stype->mr_type_aux);
		  else if (mr_type_is_a_pointer (type))
		    {
		      stype->mr_type_aux = MR_TYPE_POINTER;
		      tdp = mr_get_td_by_name_internal (type);
		    }
		}
	    }
	}
    }

  stype->tdp = tdp;

  /* pointers on a basic types were detected by MR_TYPE_DETECT_PTR into mr_type_aux */
  if ((stype->mr_type == MR_TYPE_NONE) && (stype->mr_type_aux != MR_TYPE_NONE))
    stype->mr_type = MR_TYPE_POINTER;

  if ((stype->mr_type == MR_TYPE_ARRAY) && (stype->mr_type_aux == MR_TYPE_NONE) && (stype->mr_type_ptr != MR_TYPE_NONE))
    stype->mr_type_aux = MR_TYPE_POINTER;

  if (tdp)
    {
      stype->type = tdp->type.str;
      switch (stype->mr_type)
	{
	case MR_TYPE_NONE:
	  /* Enum detection */
	case MR_TYPE_INT8:
	case MR_TYPE_UINT8:
	case MR_TYPE_INT16:
	case MR_TYPE_UINT16:
	case MR_TYPE_INT32:
	case MR_TYPE_UINT32:
	case MR_TYPE_INT64:
	case MR_TYPE_UINT64:
	case MR_TYPE_INT128:
	case MR_TYPE_UINT128:
	  stype->mr_type = tdp->mr_type;
	  break;

	case MR_TYPE_BITFIELD:
	  stype->mr_type_aux = tdp->mr_type; /* enums case */
	  break;

	  /*
	    pointers and arrays needs to detect mr_type_aux for basic types
	  */
	case MR_TYPE_POINTER:
	case MR_TYPE_ARRAY:
	  if (stype->mr_type_aux == MR_TYPE_POINTER)
	    stype->mr_type_ptr = tdp->mr_type;
	  else
	    stype->mr_type_aux = tdp->mr_type;
	  break;

	default:
	  break;
	}
    }

#define MR_TYPE_NAME(TYPE) [MR_TYPE_DETECT (TYPE)] = MR_STRINGIFY_READONLY (TYPE),
  static char * type_name[] = {
    MR_FOREACH (MR_TYPE_NAME,
		string_t, char, bool,
		int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, mr_int128_t, mr_uint128_t,
		float, complex_float_t, double, complex_double_t, long_double_t, complex_long_double_t)
  };
  mr_type_t mr_type = stype->mr_type;
  if (((0 MR_FOREACH (MR_ONE_SHIFT, MR_TYPE_POINTER, MR_TYPE_ARRAY)) >> stype->mr_type) & 1)
    mr_type = (MR_TYPE_POINTER == stype->mr_type_aux) ? stype->mr_type_ptr : stype->mr_type_aux;

  if ((mr_type > 0) && (mr_type < sizeof (type_name) / sizeof (type_name[0])))
    if (type_name[mr_type])
      stype->tdp = mr_get_td_by_name_internal (type_name[mr_type]);

  /* if field type was not detected, but it's mr_type_class is a MR_POINTER_TYPE_CLASS, then we will treat it as void pointer */
  if ((MR_TYPE_NONE == stype->mr_type) && (MR_POINTER_TYPE_CLASS == stype->mr_type_class))
    stype->mr_type = MR_TYPE_POINTER;
}

static void
mr_fd_detect_field_type (mr_fd_t * fdp)
{
  if (NULL == fdp)
    return;

  mr_fd_of_array_type (fdp);

  mr_structured_type_t stype;
  memset (&stype, 0, sizeof (stype));

  stype.type = fdp->type;
  stype.mr_type = fdp->mr_type;
  stype.mr_type_aux = fdp->mr_type_aux;
  stype.mr_type_ptr = fdp->mr_type_ptr;
  stype.mr_type_class = fdp->mr_type_class;

  mr_detect_structured_type (&stype);

  fdp->type = stype.type;
  fdp->mr_type = stype.mr_type;
  fdp->mr_type_aux = stype.mr_type_aux;
  fdp->mr_type_ptr = stype.mr_type_ptr;
  fdp->tdp = stype.tdp;
}

/**
 * Initialize fields that are pointers on functions. Detects types of arguments.
 * @param fdp pointer on a field descriptor
 * @return status
 */
static void
mr_func_field_detect (mr_fd_t * fdp)
{
  int i;
  for (i = 0; fdp->param.func_param.args[i] != NULL; ++i)
    mr_detect_structured_type (fdp->param.func_param.args[i]);
  fdp->param.func_param.size = i * sizeof (fdp->param.func_param.args[0]);
}

static mr_status_t
mr_fd_detect_res_size (mr_fd_t * fdp)
{
  if ((0 == fdp->MR_SIZE) && (fdp->res_type != NULL) && (fdp->res.ptr != NULL))
    {
      mr_td_t * res_tdp = mr_get_td_by_name_internal (fdp->res_type);
      if (res_tdp != NULL)
	fdp->MR_SIZE = res_tdp->size;
    }
  return (MR_SUCCESS);
}

mr_hash_value_t
mr_ud_override_hash (mr_ptr_t x, const void * context)
{
  mr_ud_override_t * udo = x.ptr;
  return (mr_hash_block (&udo->value, sizeof (udo->value)));
}

int
mr_ud_override_cmp (mr_ptr_t x, mr_ptr_t y, const void * context)
{
  mr_ud_override_t * _x = x.ptr;
  mr_ud_override_t * _y = y.ptr;
  return ((_x->value > _y->value) - (_x->value < _y->value));
}

static mr_status_t
mr_fd_init_ud_overrides (mr_fd_t * fdp)
{
  switch (fdp->mr_type)
    {
    case MR_TYPE_UNION:
    case MR_TYPE_ANON_UNION:
    case MR_TYPE_NAMED_ANON_UNION:
      break;
    case MR_TYPE_ARRAY:
    case MR_TYPE_POINTER:
      if (MR_TYPE_UNION == fdp->mr_type_aux)
	break;
      __attribute__ ((fallthrough));
    default:
      return (MR_SUCCESS);
    }

  fdp->param.union_param = NULL;
  
  if ((NULL == fdp->res.ptr) || (NULL == fdp->res_type) || (0 == fdp->MR_SIZE))
    return (MR_SUCCESS);

  if (strcmp (fdp->res_type, "mr_ud_override_t") != 0)
    return (MR_SUCCESS);

  if (NULL == fdp->tdp)
    return (MR_FAILURE);
  
  fdp->param.union_param = MR_CALLOC (1, sizeof (mr_ic_t));
  if (NULL == fdp->param.union_param)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (MR_FAILURE);
    }

  mr_ic_new (fdp->param.union_param, mr_ud_override_hash, mr_ud_override_cmp, "mr_ud_override_t", MR_IC_HASH, NULL);

  mr_status_t status = MR_SUCCESS;
  bool is_empty = true;
  int i, count = fdp->MR_SIZE / sizeof (mr_ud_override_t);
  mr_ud_override_t * ud_overrides = fdp->res.ptr;
  for (i = 0; i < count; ++i)
    {
      ud_overrides[i].typed_value.ptr = &ud_overrides[i].value;
      ud_overrides[i].fdp = mr_get_fd_by_name (fdp->tdp, ud_overrides[i].discriminator);
      if (NULL == ud_overrides[i].fdp)
	{
	  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_INVALID_OVERRIDE, ud_overrides[i].value, ud_overrides[i].discriminator);
	  continue;
	}
      
      mr_ptr_t * add = mr_ic_add (fdp->param.union_param, &ud_overrides[i]);
      if (NULL == add)
	{
	  status = MR_FAILURE;
	  break;
	}
      
      mr_ud_override_t * udo = add->ptr;
      if (strcmp (udo->discriminator, ud_overrides[i].discriminator) != 0)
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_CONFLICTED_OVERRIDES, udo->value, udo->discriminator, ud_overrides[i].discriminator);
            
      is_empty = false;
    }

  if (is_empty)
    {
      mr_ic_free (fdp->param.union_param);
      MR_FREE (fdp->param.union_param);
      fdp->param.union_param = NULL;
    }
  
  return (status);
}

static void
mr_fd_init_pointer_params (mr_fd_t * fdp, uint64_t invalid_mr_types, mr_fd_t * pointer_param)
{
  if ((invalid_mr_types >> fdp->mr_type_aux) & 1)
    fdp->mr_type = MR_TYPE_VOID;
  else if (MR_TYPE_POINTER == fdp->mr_type_aux)
    {
      if (NULL == pointer_param)
	fdp->mr_type = MR_TYPE_VOID;
      else
	{
	  *pointer_param = *fdp;
	  pointer_param->mr_type = MR_TYPE_POINTER;
	  pointer_param->mr_type_aux = fdp->mr_type_ptr;
	  pointer_param->mr_type_class = MR_POINTER_TYPE_CLASS;
	  pointer_param->size = sizeof (void*);
	  pointer_param->unnamed = true;
	  pointer_param->param.array_param.pointer_param = NULL;

	  mr_fd_init_ud_overrides (pointer_param);
	}
    }
}

/**
 * Nornalize field name
 * @param tdp pointer on a type descriptor
 */
static void
mr_normalize_field_name (mr_fd_t * fdp)
{
  /*
    Check names of the fields.
    MR_VOID definitions may contain brackets (for arrays) or braces (for function pointers) or collon (for bitfields).
  */
  char * name = fdp->name.str;
  if (name)
    {
      for (; !(isalnum (*name) || (*name == '_')); ++name); /* skip invalid characters */
      fdp->name.str = name;
      for (; isalnum (*name) || (*name == '_'); ++name); /* skip valid characters */
      if (*name) /* strings with field names might be in read-only memory. For VOID names are saved in writable memory. */
	*name = 0; /* truncate on first invalid charecter */

      mr_ic_add (&mr_conf.fields_names, &fdp->name);
    }
}

#define MR_VALID_STRUCT_TYPES (0 MR_FOREACH (MR_ONE_SHIFT, MR_TYPE_STRUCT, MR_TYPE_UNION, MR_TYPE_ANON_UNION, MR_TYPE_NAMED_ANON_UNION))

/**
 * Initialize fields descriptors. Everytnig that was not properly initialized in macro.
 * @param tdp pointer on a type descriptor
 * @param args auxiliary arguments
 * @return status
 */
static void
mr_init_struct (mr_td_t * tdp)
{
  if (!((MR_VALID_STRUCT_TYPES >> tdp->mr_type) & 1))
    return;

  int count;
  for (count = 0; tdp->param.struct_param.fields[count] != NULL; ++count);
  tdp->param.struct_param.fields_size = count * sizeof (tdp->param.struct_param.fields[0]);

  mr_anon_unions_extract (tdp); /* important to extract unions before building index over fields */

  mr_ic_new (&tdp->param.struct_param.field_by_name, mr_fd_name_get_hash, mr_fd_name_cmp, "mr_fd_t", MR_IC_STATIC_ARRAY, NULL);
  mr_ic_index (&tdp->param.struct_param.field_by_name, (mr_ptr_t*)tdp->param.struct_param.fields, tdp->param.struct_param.fields_size);
}

static void
mr_detect_func_args_types (mr_td_t * tdp)
{
  if (tdp->mr_type != MR_TYPE_FUNC_TYPE)
    return;

  int i;
  for (i = tdp->param.func_param.size / sizeof (tdp->param.func_param.args[0]) - 1; i >= 0; --i)
    mr_detect_structured_type (tdp->param.func_param.args[i]);
}

static void
mr_detect_struct_fields (mr_td_t * tdp)
{
  if (!((MR_VALID_STRUCT_TYPES >> tdp->mr_type) & 1))
    return;

  int i, count = tdp->param.struct_param.fields_size / sizeof (tdp->param.struct_param.fields[0]);
  for (i = 0; i < count; ++i)
    {
      mr_fd_t * fdp = tdp->param.struct_param.fields[i];
      if (fdp->self_ptr)
	{
	  fdp->type = tdp->type.str;
	  fdp->mr_type = MR_TYPE_POINTER;
	  fdp->mr_type_aux = tdp->mr_type;
	}

      mr_normalize_field_name (fdp);
      mr_fd_detect_field_type (fdp);
      mr_fd_detect_res_size (fdp);
      mr_fd_init_ud_overrides (fdp);

#define INVALID_ARRAY_AUX_TYPE (0 MR_FOREACH (MR_ONE_SHIFT MR_TYPE_NONE MR_TYPE_VOID MR_TYPE_BITFIELD MR_TYPE_ARRAY MR_TYPE_ANON_UNION MR_TYPE_NAMED_ANON_UNION MR_TYPE_END_ANON_UNION))
#define INVALID_POINTER_AUX_TYPE (0 MR_FOREACH (MR_ONE_SHIFT MR_TYPE_BITFIELD MR_TYPE_ARRAY MR_TYPE_ANON_UNION MR_TYPE_NAMED_ANON_UNION MR_TYPE_END_ANON_UNION))

      switch (fdp->mr_type)
	{
	case MR_TYPE_ARRAY:
	  mr_fd_init_pointer_params (fdp, INVALID_ARRAY_AUX_TYPE, fdp->param.array_param.pointer_param);
	  break;
	case MR_TYPE_FUNC:
	  mr_func_field_detect (fdp);
	  break;
	case MR_TYPE_BITFIELD:
	  mr_fd_init_bitfield_params (fdp);
	  break;
	case MR_TYPE_POINTER:
	  mr_fd_init_pointer_params (fdp, INVALID_POINTER_AUX_TYPE, fdp->param.pointer_param.pointer_param);

	  if (fdp->param.pointer_param.pointer_param)
	    fdp->param.pointer_param.pointer_param->res_type = NULL; /* size specification should work only for a top level pointer */
	  break;
	default:
	  break;
	}
    }

  /*
    fields descriptors might be generated in an arbitrary order
    if user used macro language only for meta data generation and
    types were defined with a standard typedefs.
    Here we sort structures fields by offset, and field size as second dimension.
    Zero size fields will have the same offsets with the field declared afterwards.
  */
  if (tdp->mr_type == MR_TYPE_STRUCT)
    mr_hsort (tdp->param.struct_param.fields, count, sizeof (tdp->param.struct_param.fields[0]), mr_fd_offset_cmp_sorting, NULL);
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
  mr_hashed_string_t hashed_name = { .str = name, .hash_value = mr_hash_str (name), };
  uintptr_t key = (uintptr_t)&hashed_name - offsetof (mr_fd_t, name);
  mr_ptr_t * result = mr_ic_find (&tdp->param.struct_param.field_by_name, key);
  return (result ? result->ptr : NULL);
}

void __attribute__ ((sentinel(0)))
mr_type_void_fields_impl (char * type, char * name, ...)
{
  va_list args;
  va_start (args, name);

  mr_conf_init ();

  if (type == NULL)
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
      return;
    }
  
  mr_td_t * tdp = mr_get_td_by_name_internal (type);
  if (tdp == NULL)
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_NO_TYPE_DESCRIPTOR, type);
      return;
    }

  for ( ; name; name = va_arg (args, char *))
    {
      mr_fd_t * fdp = mr_get_fd_by_name (tdp, name);
      if (NULL == fdp)
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_FIELD_NOT_FOUND, name, type);
      else if (fdp->mr_type != MR_TYPE_VOID)
	{
	  if ((fdp->mr_type != MR_TYPE_BITFIELD) &&
	      (fdp->mr_type != MR_TYPE_ARRAY) &&
	      (fdp->mr_type != MR_TYPE_POINTER))	  
	    fdp->mr_type_aux = fdp->mr_type;
	  fdp->mr_type = MR_TYPE_VOID;
	}
    }

  va_end (args);
}

static mr_status_t
fields_names_visitor (mr_ptr_t key, const void * context)
{
  mr_hashed_string_t * field_name = key.ptr;
  int field_name_length = strlen (field_name->str);
  int * max_field_name_length = (int*)context;
  if (field_name_length > *max_field_name_length)
    *max_field_name_length = field_name_length;
  return (MR_SUCCESS);
}

char *
mr_get_static_field_name_from_string (char * name)
{
  mr_hashed_string_t hashed_string = { .str = name, .hash_value = 0, };
  mr_ptr_t * find = mr_ic_find (&mr_conf.fields_names, &hashed_string);
  mr_hashed_string_t * field_name = find ? find->ptr : NULL;
  return (field_name ? field_name->str : NULL);
}

char *
mr_get_static_field_name_from_substring (mr_substr_t * substr)
{
  static int max_field_name_length = 0;
  if (0 == max_field_name_length)
    mr_ic_foreach (&mr_conf.fields_names, fields_names_visitor, &max_field_name_length);
  
  /* protection for buffer overrun attack */
  if (substr->length > max_field_name_length)
    return (NULL);
  
  char name[substr->length + 1];
  memcpy (name, substr->str, substr->length);
  name[substr->length] = 0;
  return (mr_get_static_field_name_from_string (name));
}

/**
 * Add type description into repository
 * @param tdp a pointer on statically initialized type descriptor
 */
void
mr_add_type (mr_td_t * tdp)
{
  if (NULL == tdp)
    return;
  if (MR_TYPE_NONE == tdp->mr_type)
    return; /* skip types that were not properly detected */

  if ((tdp->next != NULL) || (mr_conf.list == tdp))
    return;

  tdp->next = mr_conf.list;
  mr_conf.list = tdp;
}

static void
mr_td_detect_res_size (mr_td_t * tdp)
{
  if ((0 == tdp->MR_SIZE) && (tdp->res_type != NULL) && (tdp->res.ptr != NULL))
    {
      mr_td_t * res_tdp = mr_get_td_by_name_internal (tdp->res_type);
      if (res_tdp != NULL)
	tdp->MR_SIZE = res_tdp->size;
    }
}

static mr_status_t
mr_validate_fd (mr_fd_t * fdp)
{
  mr_status_t status = MR_SUCCESS;
  if (fdp->tdp == NULL)
    fdp->tdp = mr_get_td_by_name_internal (fdp->type);

  switch (fdp->mr_type)
    {
    case MR_TYPE_NONE:
    case MR_TYPE_VOID:
    case MR_TYPE_FUNC:
    case MR_TYPE_LAST:
      break;
      
    case MR_TYPE_CHAR_ARRAY:
      if (fdp->tdp)
	if (fdp->tdp->mr_type == MR_TYPE_CHAR)
	  break;
      __attribute__ ((fallthrough));
      
    case MR_TYPE_STRING:
    case MR_TYPE_CHAR:
    case MR_TYPE_BOOL:
    case MR_TYPE_INT8:
    case MR_TYPE_UINT8:
    case MR_TYPE_INT16:
    case MR_TYPE_UINT16:
    case MR_TYPE_INT32:
    case MR_TYPE_UINT32:
    case MR_TYPE_INT64:
    case MR_TYPE_UINT64:
    case MR_TYPE_INT128:
    case MR_TYPE_UINT128:
    case MR_TYPE_FLOAT:
    case MR_TYPE_COMPLEX_FLOAT:
    case MR_TYPE_DOUBLE:
    case MR_TYPE_COMPLEX_DOUBLE:
    case MR_TYPE_LONG_DOUBLE:
    case MR_TYPE_COMPLEX_LONG_DOUBLE:
    case MR_TYPE_FUNC_TYPE:
      if (fdp->tdp)
	if (fdp->mr_type != fdp->tdp->mr_type)
	  status = MR_FAILURE;
      break;
      
    case MR_TYPE_BITFIELD:
      if (!((MR_INT_TYPES >> fdp->mr_type_aux) & 1))
	status = MR_FAILURE;
      if (((MR_TYPED_TYPES >> fdp->mr_type_aux) & 1) && (fdp->tdp == NULL))
	status = MR_FAILURE;
      if (((MR_TYPED_TYPES >> fdp->mr_type_aux) & 1) && fdp->tdp)
	if (fdp->mr_type_aux != fdp->tdp->mr_type)
	  status = MR_FAILURE;
      break;
      
    case MR_TYPE_ARRAY:
    case MR_TYPE_POINTER:
      if (MR_TYPE_POINTER == fdp->mr_type_aux)
	{
	  if (MR_TYPE_ARRAY == fdp->mr_type)
	    status = mr_validate_fd (fdp->param.array_param.pointer_param);
	  if (MR_TYPE_POINTER == fdp->mr_type)
	    status = mr_validate_fd (fdp->param.pointer_param.pointer_param);
	  break;
	}
      
      if (fdp->tdp)
	if (fdp->mr_type_aux != fdp->tdp->mr_type)
	  status = MR_FAILURE;
      if (((MR_TYPED_TYPES >> fdp->mr_type_aux) & 1) && (fdp->tdp == NULL))
	  status = MR_FAILURE;
      break;
      
    case MR_TYPE_STRUCT:
    case MR_TYPE_ENUM:
    case MR_TYPE_UNION:
    case MR_TYPE_ANON_UNION:
    case MR_TYPE_NAMED_ANON_UNION:
    case MR_TYPE_END_ANON_UNION:
      if (fdp->tdp == NULL)
	status = MR_FAILURE;
      else
	if (fdp->mr_type != fdp->tdp->mr_type)
	  status = MR_FAILURE;
      break;
    }

  if (status != MR_SUCCESS)
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_TYPE_NOT_MATCHED,
		  fdp->name.str, fdp->type,
		  fdp->mr_type, fdp->mr_type_aux, fdp->tdp ? fdp->tdp->mr_type : MR_TYPE_VOID);
      if (fdp->mr_type != MR_TYPE_POINTER)
	fdp->mr_type = MR_TYPE_VOID;
      fdp->mr_type_aux = MR_TYPE_VOID;
    }
  return (status);
}

static void
mr_validate_td (mr_td_t * tdp)
{
  if (!((MR_VALID_STRUCT_TYPES >> tdp->mr_type) & 1))
    return;

  int i, count = tdp->param.struct_param.fields_size / sizeof (tdp->param.struct_param.fields[0]);
  for (i = 0; i < count; ++i)
    mr_validate_fd (tdp->param.struct_param.fields[i]);
}

inline void
mr_conf_init ()
{
  static volatile bool initialized = false;
  static volatile bool init_in_progress = false;

  if (initialized)
    return;

  if (!__atomic_test_and_set (&init_in_progress, __ATOMIC_RELAXED))
    {
      mr_ic_new (&mr_conf.enum_by_name, mr_ed_name_get_hash, mr_ed_name_cmp, "mr_ed_t", MR_IC_HASH, NULL);
      mr_ic_new (&mr_conf.type_by_name, mr_td_name_get_hash, mr_td_name_cmp, "mr_td_t", MR_IC_HASH, NULL);
      mr_ic_new (&mr_conf.fields_names, mr_hashed_string_get_hash_ic, mr_hashed_string_cmp_ic, "mr_hashed_string_t", MR_IC_HASH, NULL);

      mr_td_t * tdp;
      for (tdp = mr_conf.list; tdp; tdp = tdp->next)
	{
	  mr_normalize_type (tdp->type.str);
	  tdp->type.hash_value = 0;

	  /* check whether this type is already in the list */
	  if (mr_get_td_by_name_internal (tdp->type.str))
	    continue; /* this type is already registered */

	  mr_init_struct (tdp);
	  mr_init_enum (tdp);
	  mr_init_func (tdp);
	  mr_ic_add (&mr_conf.type_by_name, tdp);
	}

      for (tdp = mr_conf.list; tdp; tdp = tdp->next)
	{
	  if (tdp != mr_get_td_by_name_internal (tdp->type.str))
	    continue; /* this type is a duplicate */

	  mr_detect_func_args_types (tdp);
	  mr_detect_struct_fields (tdp);
	  mr_td_detect_res_size (tdp);
	  mr_register_type_pointer (tdp);
	  mr_validate_td (tdp);
	}

      initialized = true;
    }
  while (!initialized);
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
  mr_conf_init ();
  mr_fd_detect_field_type (fdp);
  
  if (fdp->tdp)
    fdp->name.str = fdp->tdp->type.str;
  
  mr_fd_t * ptr_fdp = fdp->param.array_param.pointer_param;
  if (ptr_fdp)
    {
      *ptr_fdp = *fdp;
      ptr_fdp->mr_type = MR_TYPE_POINTER;
      ptr_fdp->mr_type_aux = fdp->mr_type_ptr;
      ptr_fdp->size = sizeof (void*);
    }
  
  mr_validate_fd (fdp);
}

mr_uintmax_t
mr_strtouintmax (char * str, char ** endptr, int base)
{
  mr_uintmax_t acc = 0;
  char * s = str;

  while (isspace (s[0]))
    ++s;

  bool neg = (s[0] == '-');

  if ((s[0] == '+') || (s[0] == '-'))
    ++s;

  if (!isdigit (*s))
    s = str;
  else
    {
      if (((base == 0) || (base == 16)) &&
	  (s[0] == '0') && ((s[1] == 'x') || (s[1] == 'X')))
	{
	  s += 2;
	  base = 16;
	}
      if (base == 0)
	base = (s[0] == '0') ? 8 : 10;

      for ( ; ; ++s)
	{
	  unsigned int c = s[0];

	  if (isdigit (c))
	    c -= '0';
	  else if (isalpha (c))
	    c -= isupper (c) ? 'A' - 10 : 'a' - 10;
	  else
	    break;

	  if (c >= base)
	    break;

	  acc = acc * base + c;
	}

      if (neg)
	acc = -acc;
    }

  if (endptr != NULL)
    *endptr = s;

  return (acc);
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
  str = (char*)MR_CALLOC (max_size, sizeof (*str));
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
	  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_END);
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
	  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_DATA);
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
	      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNBALANCED_TAGS);
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
