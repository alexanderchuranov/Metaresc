/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <rpc/types.h>
#include <rpc/xdr.h>
#include <netinet/in.h>
#ifdef HAVE_CONFIG_H
# include <mr_config.h>
#endif /* HAVE_CONFIG_H */

#include <metaresc.h>
#include <mr_stringify.h>
#include <mr_ic.h>
#include <mr_load.h>
#include <mr_save.h>

TYPEDEF_FUNC (mr_status_t, xdr_handler_t, (XDR *, int, mr_ra_ptrdes_t *))

static xdr_handler_t xdr_load_handler[];
static xdr_handler_t xdr_save_handler[];

/**
 * Loads int32_t from binary XDR stream.
 * @param xdrs XDR stream descriptor
 * @param lp pointer on int32_t
 * @return status of operation. 0 - failure, !0 - success.
 */
static bool_t
xdrra_getlong (XDR * xdrs, long * lp)
{
  mr_rarray_t * ra = (mr_rarray_t*)xdrs->x_private;
  char * base = ra->data.ptr;
  if (NULL == ra)
    return (false);
  if (ra->MR_SIZE - xdrs->x_handy < sizeof (int32_t))
    return (false);
  *(int32_t*)lp = (int32_t) ntohl ((*((int32_t *)(&base[xdrs->x_handy]))));
  xdrs->x_handy += sizeof (int32_t);
  return (true);
}

/**
 * Saves int32_t into binary XDR stream.
 * @param xdrs XDR stream descriptor
 * @param lp pointer on int32_t
 * @return status of operation. 0 - failure, !0 - success.
 */
static bool_t
xdrra_putlong (XDR * xdrs, const long * lp)
{
  mr_rarray_t * ra = (mr_rarray_t*)xdrs->x_private;
  int32_t * data = mr_rarray_append (ra, sizeof (int32_t));
  if (NULL == ra)
    return (false);
  if (NULL == data)
    return (false);
  *data = htonl (*(int32_t*)lp);
  xdrs->x_handy += sizeof (int32_t);
  return (true);
}

/**
 * Loads opaque bytes stream from binary XDR stream.
 * @param xdrs XDR stream descriptor
 * @param addr pointer of bytes stream
 * @param len length of byte array
 * @return status of operation. 0 - failure, !0 - success.
 */
static bool_t
xdrra_getbytes (XDR * xdrs, caddr_t addr, u_int len)
{
  mr_rarray_t * ra = (mr_rarray_t*)xdrs->x_private;
  char * base = ra->data.ptr;
  if (NULL == ra)
    return (false);
  if (ra->MR_SIZE - xdrs->x_handy < len)
    return (false);
  memcpy (addr, &base[xdrs->x_handy], len);
  xdrs->x_handy += len;
  return (true);
}

/**
 * Saves opaque bytes stream into binary XDR stream.
 * @param xdrs XDR stream descriptor
 * @param addr pointer of bytes stream
 * @param len length of byte array
 * @return status of operation. 0 - failure, !0 - success.
 */
static bool_t
xdrra_putbytes (XDR * xdrs, const char * addr, u_int len)
{
  mr_rarray_t * ra = (mr_rarray_t*)xdrs->x_private;
  int32_t * data;
  if (NULL == ra)
    return (false);
  data = mr_rarray_append (ra, len);
  if (NULL == data)
    return (false);
  memcpy (data, addr, len);
  xdrs->x_handy += len;
  return (true);
}

/**
 * Gets current stream position.
 * @param xdrs XDR stream descriptor
 * @return position of cursor
 */
static u_int
xdrra_getpostn (__const XDR * xdrs)
{
  return (xdrs->x_handy);
}

/**
 * Sets stream cursor position.
 * @param xdrs XDR stream descriptor
 * @param pos position to set
 * @return status
 */
static bool_t
xdrra_setpostn (XDR * xdrs, u_int pos)
{
  xdrs->x_handy = pos;
  return (true);
}

/**
 * Returns pointer on opaque data from XDR stream.
 * @param xdrs XDR stream descriptor
 * @param len length of opaque data
 * @return pointer on buffer
 */
static int32_t *
xdrra_inline (XDR * xdrs, u_int len)
{
  mr_rarray_t * ra = (mr_rarray_t*)xdrs->x_private;
  char * base = ra->data.ptr;
  if (NULL == ra)
    return (false);
  if (ra->MR_SIZE - xdrs->x_handy < len)
    return (NULL);
  return ((int32_t*)&base[xdrs->x_handy]);
}

/**
 * Deallocates XDR reader/writer for resizeable array.
 * @param xdrs XDR stream descriptor
 */
static void
xdrra_destroy (XDR * xdrs)
{
  mr_rarray_t * ra = (mr_rarray_t*)xdrs->x_private;
  if (NULL == ra)
    return;
  if (ra->data.ptr)
    MR_FREE (ra->data.ptr);
  ra->data.ptr = NULL;
  ra->MR_SIZE = ra->alloc_size = 0;
}

#ifdef HAVE_STRUCT_XDR_OPS_X_GETINT32
/**
 * Loads int32 from XDR stream.
 * @param xdrs XDR stream descriptor
 * @param ip pointer on int32
 * @return status
 */
static bool_t
xdrra_getint32 (XDR * xdrs, int32_t * ip)
{
  return (xdrra_getlong (xdrs, (long*)ip));
}
#endif /* HAVE_STRUCT_XDR_OPS_X_GETINT32 */

#ifdef HAVE_STRUCT_XDR_OPS_X_PUTINT32
/**
 * Puts int32 into XDR stream.
 * @param xdrs XDR stream descriptor
 * @param ip pointer on int32
 * @return status
 */
static bool_t
xdrra_putint32 (XDR * xdrs, const int32_t * ip)
{
  return (xdrra_putlong (xdrs, (long*)ip));
}
#endif /* HAVE_STRUCT_XDR_OPS_X_PUTINT32 */

/**
 * Constructor for XDR reader/writer for resizeable array.
 * @param xdrs XDR stream descriptor
 * @param ra resizeable array
 * @param op XDR operation XDR_DECODE XDR_ENCODE
 */
void
xdrra_create (XDR * xdrs, mr_rarray_t * ra, enum xdr_op op)
{
  static struct xdr_ops xdrra_ops =
    {
      .x_getlong = (__typeof__ (xdrra_ops.x_getlong))xdrra_getlong,
      .x_putlong = (__typeof__ (xdrra_ops.x_putlong))xdrra_putlong,
      .x_getbytes = xdrra_getbytes,
      .x_putbytes = xdrra_putbytes,
      .x_getpostn = (__typeof__ (xdrra_ops.x_getpostn))xdrra_getpostn,
      .x_setpostn = xdrra_setpostn,
      .x_inline = xdrra_inline,
      .x_destroy = xdrra_destroy,
#ifdef HAVE_STRUCT_XDR_OPS_X_GETINT32
      .x_getint32 = xdrra_getint32,
#endif /* HAVE_STRUCT_XDR_OPS_X_GETINT32 */
#ifdef HAVE_STRUCT_XDR_OPS_X_PUTINT32
      .x_putint32 = xdrra_putint32,
#endif /* HAVE_STRUCT_XDR_OPS_X_PUTINT32 */
    };

  xdrs->x_op = op;
  xdrs->x_ops = &xdrra_ops;
  xdrs->x_private = (caddr_t)ra;
  xdrs->x_handy = 0;
}

/**
 * Set cross refernces within loaded data.
 * @param ptrs array with descriptor of loaded data
 * @return status
 */
static mr_status_t
mr_set_crossrefs (mr_ra_ptrdes_t * ptrs)
{
  int count = ptrs->size / sizeof (ptrs->ra[0]);
  mr_status_t status = MR_SUCCESS;
  int i;

  /* set all cross refernces */
  for (i = 0; i < count; ++i)
    if (ptrs->ra[i].ref_idx >= 0)
      {
	if (ptrs->ra[i].ref_idx >= count)
	  {
	    MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNDEFINED_REF_IDX, "ref_idx", ptrs->ra[i].ref_idx);
	    status = MR_FAILURE;
	  }
	else
	  {
	    int idx = ptrs->ra[i].ref_idx;
	    void * data = ptrs->ra[idx].data.ptr;

	    if ((MR_TYPE_POINTER != ptrs->ra[i].fd.mr_type) &&
		(MR_TYPE_STRING != ptrs->ra[i].fd.mr_type))
	      {
		MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_TARGET_TYPE, ptrs->ra[i].fd.mr_type);
		status = MR_FAILURE;
		continue;
	      }

	    if (ptrs->ra[i].flags.is_content_reference)
	      {
		if (NULL == data)
		  {
		    MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
		    status = MR_FAILURE;
		  }
		else
		  data = *(void**)data;
	      }

	    if (NULL == ptrs->ra[i].data.ptr)
	      {
		MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_NULL_POINTER);
		status = MR_FAILURE;
	      }
	    else
	      *(void**)ptrs->ra[i].data.ptr = data;
	  }
      }
  return (status);
}

static mr_status_t
xdr_load_inner (void * data, mr_fd_t * fdp, XDR * xdrs, mr_ra_ptrdes_t * ptrs, int parent)
{
  mr_status_t status = MR_FAILURE;
  int idx = mr_add_ptr_to_list (ptrs);
  if (idx < 0)
    return (MR_FAILURE);
  
  ptrs->ra[idx].data.ptr = data;
  ptrs->ra[idx].fd = *fdp;
  mr_add_child (parent, idx, ptrs->ra);

  if ((fdp->mr_type < MR_TYPE_LAST) && xdr_load_handler[fdp->mr_type])
    status = xdr_load_handler[fdp->mr_type] (xdrs, idx, ptrs);
  else
    MR_MESSAGE_UNSUPPORTED_NODE_TYPE_ (fdp);

  return (status);
}

mr_status_t
xdr_load (void * data, mr_fd_t * fdp, XDR * xdrs)
{
  mr_ra_ptrdes_t ptrs = { .ra = NULL, .size = 0, .alloc_size = 0, };
  mr_status_t status = xdr_load_inner (data, fdp, xdrs, &ptrs, -1);
  mr_status_t crossref_status = mr_set_crossrefs (&ptrs);

  if (MR_SUCCESS == status)
    status = crossref_status;

  if (ptrs.ra != NULL)
    MR_FREE (ptrs.ra);
  return (status);
}

/**
 * Handler for RL_TYPE_NONE.
 * @param xdrs XDR stream descriptor
 * @param idx index of node in ptrs
 * @param ptrs array with descriptor of loaded data
 * @return status
 */
static mr_status_t
xdr_none (XDR * xdrs, int idx, mr_ra_ptrdes_t * ptrs)
{
  return (xdr_void () ? MR_SUCCESS : MR_FAILURE);
}

/*
  LIBC implementation of xdr_char is too old and has some pitfalls. For readonly memory (statically inited strings) XDR_ENCODE operation produce GPF due to unnecessary write.

bool_t xdr_char (xdrs, cp)
     XDR *xdrs;
     char *cp;
{
  int i;

  i = (*cp);
  if (!xdr_int (xdrs, &i)) {
    return (false);
  }
  *cp = i;
  return (true);
}

  Here is workaround.
*/

/**
 * Handler for type char.
 * @param xdrs XDR stream descriptor
 * @param cp pointer on char
 * @return status
 */
static bool_t __attribute__((unused))
xdr_char_ (XDR * xdrs, char * cp)
{
  int32_t x = 0;
  if (XDR_ENCODE == xdrs->x_op)
    x = *cp;
  if (!xdr_int (xdrs, &x))
    return (false);
  if (XDR_DECODE == xdrs->x_op)
    *cp = x;
  return (true);
}

/**
 * Handler for type ssize_t.
 * @param xdrs XDR stream descriptor
 * @param cp pointer on ssize_t
 * @return status
 */
static bool_t __attribute__((unused))
xdr_ssize_t (XDR * xdrs, ssize_t * cp)
{
  int64_t x = 0;
  if (XDR_ENCODE == xdrs->x_op)
    x = *cp;
  if (!xdr_int64_t (xdrs, &x))
    return (false);
  if (XDR_DECODE == xdrs->x_op)
    *cp = x;
  return (true);
}

#ifndef HAVE_XDR_UINT8_T
#define xdr_uint8_t xdr_char_
#endif /* HAVE_XDR_UINT8_T */

#ifndef HAVE_XDR_INT8_T
#define xdr_int8_t xdr_char_
#endif /* HAVE_XDR_INT8_T */

#ifndef HAVE_XDR_UINT16_T
#  ifdef HAVE_XDR_U_INT16_T
#    define xdr_uint16_t xdr_u_int16_t
#  else /* HAVE_XDR_U_INT16_T */
#    define xdr_uint16_t xdr_u_short
#  endif /* HAVE_XDR_U_INT16_T */
#endif /* HAVE_XDR_UINT16_T */

#ifndef HAVE_XDR_UINT32_T
#  ifdef HAVE_XDR_U_INT32_T
#    define xdr_uint32_t xdr_u_int32_t
#  else /* HAVE_XDR_U_INT32_T */
#    define xdr_uint32_t xdr_u_int
#  endif /* HAVE_XDR_U_INT32_T */
#endif /* HAVE_XDR_UINT32_T */

#ifndef HAVE_XDR_UINT64_T
#  ifdef HAVE_XDR_U_INT64_T
#    define xdr_uint64_t xdr_u_int64_t
#  else /* HAVE_XDR_U_INT64_T */
#    define xdr_uint64_t xdr_u_longlong_t
#  endif /* HAVE_XDR_U_INT64_T */
#endif /* HAVE_XDR_UINT64_T */

/**
 * Handler for unsigned integer types.
 * @param xdrs XDR stream descriptor
 * @param idx index of node in ptrs
 * @param ptrs array with descriptor of loaded data
 * @return status
 */
static mr_status_t
xdr_uint_ (XDR * xdrs, int idx, mr_ra_ptrdes_t * ptrs)
{
  bool_t status = false;
  switch (ptrs->ra[idx].fd.size)
    {
    case sizeof (uint8_t):
      status = xdr_uint8_t (xdrs, ptrs->ra[idx].data.ptr);
      break;
    case sizeof (uint16_t):
      status = xdr_uint16_t (xdrs, ptrs->ra[idx].data.ptr);
      break;
    case sizeof (uint32_t):
      status = xdr_uint32_t (xdrs, ptrs->ra[idx].data.ptr);
      break;
    case sizeof (uint64_t):
      status = xdr_uint64_t (xdrs, ptrs->ra[idx].data.ptr);
      break;
    }
  return (status ? MR_SUCCESS : MR_FAILURE);
}

/**
 * Handler for signed integer types.
 * @param xdrs XDR stream descriptor
 * @param idx index of node in ptrs
 * @param ptrs array with descriptor of loaded data
 * @return status
 */
static mr_status_t
xdr_int_ (XDR * xdrs, int idx, mr_ra_ptrdes_t * ptrs)
{
  bool_t status = false;
  switch (ptrs->ra[idx].fd.size)
    {
    case sizeof (uint8_t):
      status = xdr_int8_t (xdrs, ptrs->ra[idx].data.ptr);
      break;
    case sizeof (uint16_t):
      status = xdr_int16_t (xdrs, ptrs->ra[idx].data.ptr);
      break;
    case sizeof (uint32_t):
      status = xdr_int32_t (xdrs, ptrs->ra[idx].data.ptr);
      break;
    case sizeof (uint64_t):
      status = xdr_int64_t (xdrs, ptrs->ra[idx].data.ptr);
      break;
    }
  return (status ? MR_SUCCESS : MR_FAILURE);
}

/**
 * Handler for type float.
 * @param xdrs XDR stream descriptor
 * @param idx index of node in ptrs
 * @param ptrs array with descriptor of loaded data
 * @return status
 */
static mr_status_t
xdr_float_ (XDR * xdrs, int idx, mr_ra_ptrdes_t * ptrs)
{
  return (xdr_float (xdrs, ptrs->ra[idx].data.ptr) ? MR_SUCCESS : MR_FAILURE);
}

#define XDR_COMPLEX(NAME_SUFFIX, TYPE_NAME)					\
  static mr_status_t xdr_complex_ ## NAME_SUFFIX (XDR * xdrs, int idx, mr_ra_ptrdes_t * ptrs) { \
    TYPE_NAME real = 0;							\
    TYPE_NAME imag = 0;							\
    complex TYPE_NAME x = 0;						\
    if (XDR_ENCODE == xdrs->x_op)					\
      {									\
	x = *(complex TYPE_NAME*)ptrs->ra[idx].data.ptr;		\
	real = __real__ x;						\
	imag = __imag__ x;						\
      }									\
    if (!xdr_ ## NAME_SUFFIX (xdrs, &real))				\
      return (MR_FAILURE);						\
    if (!xdr_ ## NAME_SUFFIX (xdrs, &imag))				\
      return (MR_FAILURE);						\
    if (XDR_DECODE == xdrs->x_op)					\
      {									\
	__real__ x = real;						\
	__imag__ x = imag;						\
	*(complex TYPE_NAME*)ptrs->ra[idx].data.ptr = x;		\
      }									\
    return (MR_SUCCESS);						\
  }

XDR_COMPLEX (float, float);

/**
 * Handler for type double.
 * @param xdrs XDR stream descriptor
 * @param idx index of node in ptrs
 * @param ptrs array with descriptor of loaded data
 * @return status
 */
static mr_status_t
xdr_double_ (XDR * xdrs, int idx, mr_ra_ptrdes_t * ptrs)
{
  return (xdr_double (xdrs, ptrs->ra[idx].data.ptr) ? MR_SUCCESS : MR_FAILURE);
}

XDR_COMPLEX (double, double);

/**
 * Handler for type long double. Saves as opaque data binary representation of long double in memeory. Assumes that CPU uses ieee854 standard.
 * @param xdrs XDR stream descriptor
 * @param idx index of node in ptrs
 * @param ptrs array with descriptor of loaded data
 * @return status
 */
static bool_t
xdr_long_double (XDR * xdrs, long double * data)
{
  return (xdr_opaque (xdrs, (void*)data, MR_SIZEOF_LONG_DOUBLE));
}

static mr_status_t
xdr_long_double_ (XDR * xdrs, int idx, mr_ra_ptrdes_t * ptrs)
{
  return (xdr_long_double (xdrs, ptrs->ra[idx].data.ptr) ? MR_SUCCESS : MR_FAILURE);
}

XDR_COMPLEX (long_double, long double);

/**
 * Handler for char arrays.
 * @param xdrs XDR stream descriptor
 * @param idx index of node in ptrs
 * @param ptrs array with descriptor of loaded data
 * @return status
 */
static mr_status_t
xdr_char_array_ (XDR * xdrs, int idx, mr_ra_ptrdes_t * ptrs)
{
  uint32_t str_len, max_size = ptrs->ra[idx].fd.size;
  int parent = ptrs->ra[idx].parent;
  bool is_a_dynamic_string = ((parent >= 0) && (MR_TYPE_POINTER == ptrs->ra[parent].fd.mr_type));

  if (XDR_ENCODE == xdrs->x_op)
    {
      str_len = strlen (ptrs->ra[idx].data.ptr) + 1;
      if (!is_a_dynamic_string && (str_len > max_size))
	{
	  str_len = max_size;
	  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_STRING_TRUNCATED);
	}
    }

  if (!xdr_uint32_t (xdrs, &str_len))
    return (MR_FAILURE);

  if ((XDR_DECODE == xdrs->x_op) && (str_len > max_size) && is_a_dynamic_string)
    {
      void * data = MR_REALLOC (ptrs->ra[idx].data.ptr, str_len);
      if (NULL == data)
	{
	  MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	  return (MR_FAILURE);
	}
      *(void**)ptrs->ra[idx - 1].data.ptr = ptrs->ra[idx].data.ptr = data;
    }
  return (xdr_opaque (xdrs, ptrs->ra[idx].data.ptr, str_len) ? MR_SUCCESS : MR_FAILURE);
}

/**
 * Save handler for char pointers.
 * @param xdrs XDR stream descriptor
 * @param idx index of node in ptrs
 * @param ptrs array with descriptor of loaded data
 * @return status
 */
static mr_status_t
xdr_save_string (XDR * xdrs, int idx, mr_ra_ptrdes_t * ptrs)
{
  if (ptrs->ra[idx].ref_idx >= 0)
    {
      if (!xdr_int32_t (xdrs, &ptrs->ra[ptrs->ra[idx].ref_idx].idx))
	return (MR_FAILURE);
      if (!xdr_uint32_t (xdrs, (void*)&ptrs->ra[idx].flags))
	return (MR_FAILURE);
      return (MR_SUCCESS);
    }
  else
    {
      void ** str = ptrs->ra[idx].data.ptr;
      int32_t size = -1;
      if (!xdr_int32_t (xdrs, &ptrs->ra[idx].ref_idx))
	return (MR_FAILURE);
      if (NULL != *str)
	size = strlen (*str);
      if (!xdr_int32_t (xdrs, &size))
	return (MR_FAILURE);
      if (size < 0)
	return (MR_SUCCESS);
      if (!xdr_opaque (xdrs, *str, size))
	return (MR_FAILURE);
      return (MR_SUCCESS);
    }
}

/**
 * Load handler for char pointers.
 * @param xdrs XDR stream descriptor
 * @param idx index of node in ptrs
 * @param ptrs array with descriptor of loaded data
 * @return status
 */
static mr_status_t
xdr_load_string (XDR * xdrs, int idx, mr_ra_ptrdes_t * ptrs)
{
  if (!xdr_int32_t (xdrs, &ptrs->ra[idx].ref_idx))
    return (MR_FAILURE);
  if (ptrs->ra[idx].ref_idx >= 0)
    {
      if (!xdr_uint32_t (xdrs, (void*)&ptrs->ra[idx].flags))
	return (MR_FAILURE);
      return (MR_SUCCESS);
    }
  else
    {
      char ** str = ptrs->ra[idx].data.ptr;
      int32_t size = -1;
      *str = NULL;
      if (!xdr_int32_t (xdrs, &size))
	return (MR_FAILURE);
      if (size < 0)
	return (MR_SUCCESS);
      *str = MR_MALLOC (size + 1);
      if (NULL == *str)
	{
	  MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	  return (MR_FAILURE);
	}
      memset (*str, 0, size + 1);
      if (!xdr_opaque (xdrs, *str, size))
	return (MR_FAILURE);
      return (MR_SUCCESS);
    }
}

/**
 * Load handler for structures.
 * @param xdrs XDR stream descriptor
 * @param idx index of node in ptrs
 * @param ptrs array with descriptor of loaded data
 * @param tdp type descriptor
 * @return status
 */
static mr_status_t
xdr_load_struct_inner (XDR * xdrs, int idx, mr_ra_ptrdes_t * ptrs, mr_td_t * tdp)
{
  char * data = ptrs->ra[idx].data.ptr;
  int i, count;

  if (NULL == tdp)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_NO_TYPE_DESCRIPTOR, ptrs->ra[idx].fd.type);
      return (MR_FAILURE);
    }

  if (tdp->mr_type != MR_TYPE_STRUCT)
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_TYPE_NOT_STRUCT, tdp->type.str);
      return (MR_FAILURE);
    }

  count = tdp->fields_size / sizeof (tdp->fields[0]);
  for (i = 0; i < count; ++i)
    {
      mr_fd_t * fdp = tdp->fields[i].fdp;
      if (MR_SUCCESS != xdr_load_inner (&data[fdp->offset], fdp, xdrs, ptrs, idx))
	return (MR_FAILURE);
    }
  return (MR_SUCCESS);
}

/**
 * Load handler for structures.
 * @param xdrs XDR stream descriptor
 * @param idx index of node in ptrs
 * @param ptrs array with descriptor of loaded data
 * @return status
 */
static mr_status_t
xdr_load_struct (XDR * xdrs, int idx, mr_ra_ptrdes_t * ptrs)
{
  return (xdr_load_struct_inner (xdrs, idx, ptrs, mr_get_td_by_name (ptrs->ra[idx].fd.type)));
}

/*
  Union mr_ptr_t might be different on source and destination computer. That's why union branch could be identified only by name, not index.
 */
/**
 * Save handler for unions. Saves discriminator as a string.
 * @param xdrs XDR stream descriptor
 * @param idx index of node in ptrs
 * @param ptrs array with descriptor of loaded data
 * @return status
 */
static mr_status_t
xdr_save_union (XDR * xdrs, int idx, mr_ra_ptrdes_t * ptrs)
{
  /* save union branch field name as string */
  char * dummy_str = "";
  mr_ptrdes_t ptrdes = { /* temporary pointer descriptor for this string */
    .data.ptr = &dummy_str,
    .ref_idx = -1,
    .flags = { .is_null = false, .is_referenced = false, .is_content_reference = false, },
  };

  if (ptrs->ra[idx].first_child >= 0)
    ptrdes.data.ptr = &ptrs->ra[ptrs->ra[idx].first_child].fd.name.str;

  mr_ra_ptrdes_t ptrs_ = { .ra = &ptrdes, .size = sizeof (ptrdes), .alloc_size = -1, }; /* temporary resizeable array */
  return (xdr_save_string (xdrs, 0, &ptrs_));
}

/**
 * Load handler for unions.
 * @param xdrs XDR stream descriptor
 * @param idx index of node in ptrs
 * @param ptrs array with descriptor of loaded data
 * @return status
 */
static mr_status_t
xdr_load_union (XDR * xdrs, int idx, mr_ra_ptrdes_t * ptrs)
{
  mr_td_t * tdp = mr_get_td_by_name (ptrs->ra[idx].fd.type); /* look up for type descriptor */
  char * data = ptrs->ra[idx].data.ptr;
  char * discriminator = NULL;
  mr_fd_t * fdp;
  mr_status_t status = MR_FAILURE;
  mr_ptrdes_t ptrdes = { .data.ptr = &discriminator, }; /* temporary pointer descriptor for union discriminator string */
  mr_ra_ptrdes_t ptrs_ = { .ra = &ptrdes, .size = sizeof (ptrdes), .alloc_size = -1, }; /* temporary resizeable array */

  /* get pointer on structure descriptor */
  if (NULL == tdp)
    MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_NO_TYPE_DESCRIPTOR, ptrs->ra[idx].fd.type);
  else if ((tdp->mr_type != MR_TYPE_UNION) && (tdp->mr_type != MR_TYPE_ANON_UNION) && (tdp->mr_type != MR_TYPE_NAMED_ANON_UNION))
    MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_TYPE_NOT_UNION, tdp->type.str);
  else if (MR_SUCCESS != xdr_load_string (xdrs, 0, &ptrs_))
    MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNION_DISCRIMINATOR_ERROR, discriminator);
  else if (NULL == discriminator)
    MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNION_DISCRIMINATOR_ERROR, discriminator);
  else
    {
      if (0 == tdp->fields_size) /* check for an empty union */
	status = MR_SUCCESS;
      else
	{
	  fdp = mr_get_fd_by_name (tdp, discriminator);
	  if (NULL == fdp)
	    MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNION_DISCRIMINATOR_ERROR, discriminator);
	  else
	    status = xdr_load_inner (data + fdp->offset, fdp, xdrs, ptrs, idx);
	}
      MR_FREE (discriminator);
    }

  return (status);
}

/**
 * Saves temporary string into XDR stream.
 * @param xdrs XDR stream descriptor
 * @param str pointer on a string
 * @return status
 */
static mr_status_t
xdr_save_temp_string (XDR * xdrs, char ** str)
{
  mr_status_t status = MR_FAILURE;
  if (NULL != str)
    {
      mr_ptrdes_t ptrdes = { /* temporary pointer descriptor for this string */
	.data.ptr = str,
	.ref_idx = -1,
	.flags = { .is_null = false, .is_referenced = false, .is_content_reference = false, },
      };
      mr_ra_ptrdes_t ptrs = { .ra = &ptrdes, .size = sizeof (ptrdes), .alloc_size = -1, }; /* temporary resizeable array */
      status = xdr_save_string (xdrs, 0, &ptrs);
    }
  return (status);
}

/**
 * Loads temporary string from XDR stream.
 * @param xdrs XDR stream descriptor
 * @param str pointer on a string
 * @return status
 */
static mr_status_t
xdr_load_temp_string (XDR * xdrs, char ** str)
{
  mr_ptrdes_t ptrdes = { .data.ptr = str, }; /* temporary pointer descriptor for union discriminator string */
  mr_ra_ptrdes_t ptrs = { .ra = &ptrdes, .size = sizeof (ptrdes), .alloc_size = -1, }; /* temporary resizeable array */
  return (xdr_load_string (xdrs, 0, &ptrs));
}

/**
 * Saves enum value as a string.
 * @param xdrs XDR stream descriptor
 * @param idx index of node in ptrs
 * @param ptrs array with descriptor of loaded data
 * @return status
 */
static mr_status_t
xdr_save_enum (XDR * xdrs, int idx, mr_ra_ptrdes_t * ptrs)
{
  char * value = mr_stringify_enum (&ptrs->ra[idx]);
  mr_status_t status = xdr_save_temp_string (xdrs, &value);
  if (NULL != value)
    MR_FREE (value);
  return (status);
}

/**
 * Saves bitmask value as a string.
 * @param xdrs XDR stream descriptor
 * @param idx index of node in ptrs
 * @param ptrs array with descriptor of loaded data
 * @return status
 */
static mr_status_t
xdr_save_bitmask (XDR * xdrs, int idx, mr_ra_ptrdes_t * ptrs)
{
  char * value = mr_stringify_bitmask (&ptrs->ra[idx], MR_BITMASK_OR_DELIMITER);
  mr_status_t status = xdr_save_temp_string (xdrs, &value);
  if (NULL != value)
    MR_FREE (value);
  return (status);
}

/**
 * Saves function pointer as a string.
 * @param xdrs XDR stream descriptor
 * @param idx index of node in ptrs
 * @param ptrs array with descriptor of loaded data
 * @return status
 */
static mr_status_t
xdr_save_func (XDR * xdrs, int idx, mr_ra_ptrdes_t * ptrs)
{
  char * value = mr_stringify_func (&ptrs->ra[idx]);
  mr_status_t status = xdr_save_temp_string (xdrs, &value);
  if (NULL != value)
    MR_FREE (value);
  return (status);
}

/**
 * Loads enum or bitmask.
 * @param xdrs XDR stream descriptor
 * @param idx index of node in ptrs
 * @param ptrs array with descriptor of loaded data
 * @return status
 */
static mr_status_t
xdr_load_stringified_type (XDR * xdrs, int idx, mr_ra_ptrdes_t * ptrs)
{
  mr_status_t status = MR_FAILURE;
  mr_ptrdes_t ptrdes = { .fd = ptrs->ra[idx].fd, .data = ptrs->ra[idx].data, };
  mr_load_data_t mr_load_data = { .ptrs = { .ra = &ptrdes, .size = sizeof (ptrdes), .alloc_size = -1, }, };
  char * str = NULL;

  if (MR_SUCCESS != xdr_load_temp_string (xdrs, &str))
    return (MR_FAILURE);
  if (NULL == str)
    return (MR_FAILURE);

  ptrdes.load_params.mr_value.value_type = MR_VT_QUOTED_SUBSTR;
  ptrdes.load_params.mr_value.vt_quoted_substr.substr.str = str;
  ptrdes.load_params.mr_value.vt_quoted_substr.substr.length = strlen (str);
  ptrdes.load_params.mr_value.vt_quoted_substr.unquote = NULL;
  
  switch (ptrs->ra[idx].fd.mr_type)
    {
    case MR_TYPE_ENUM:
    case MR_TYPE_BITMASK:
      status = mr_load_integer (0, &mr_load_data);
      break;
    case MR_TYPE_FUNC:
    case MR_TYPE_FUNC_TYPE:
      status = mr_load_func (0, &mr_load_data);
      break;
    default:
      status = MR_FAILURE;
      break;
    }

  MR_FREE (str);

  return (status);
}

/**
 * Saves/loads bit field value according its type.
 * @param xdrs XDR stream descriptor
 * @param fdp field descriptor
 * @param data pointer on data
 * @return status
 */
static mr_status_t
xdr_bitfield_value (XDR * xdrs, mr_fd_t * fdp, void * data)
{
  mr_ptrdes_t ptrdes = { .fd = *fdp, .data.ptr = data, };
  mr_ra_ptrdes_t ptrs = { .ra = &ptrdes, .size = sizeof (ptrdes), .alloc_size = -1, };
  
  ptrdes.fd.mr_type = ptrdes.fd.mr_type_aux;
  if (XDR_ENCODE == xdrs->x_op)
    return (xdr_save_handler[ptrdes.fd.mr_type] (xdrs, 0, &ptrs));
  else if (XDR_DECODE == xdrs->x_op)
    return (xdr_load_handler[ptrdes.fd.mr_type] (xdrs, 0, &ptrs));
  else
    return (MR_SUCCESS);
}

/**
 * Saves bit field into XDR stream.
 * @param xdrs XDR stream descriptor
 * @param idx index of node in ptrs
 * @param ptrs array with descriptor of loaded data
 * @return status
 */
static mr_status_t
xdr_save_bitfield (XDR * xdrs, int idx, mr_ra_ptrdes_t * ptrs)
{
  uint64_t value;
  if (MR_SUCCESS != mr_save_bitfield_value (&ptrs->ra[idx], &value))
    return (MR_FAILURE);
  return (xdr_bitfield_value (xdrs, &ptrs->ra[idx].fd, &value));
}

/**
 * Loads bit field from XDR stream.
 * @param xdrs XDR stream descriptor
 * @param idx index of node in ptrs
 * @param ptrs array with descriptor of loaded data
 * @return status
 */
static mr_status_t
xdr_load_bitfield (XDR * xdrs, int idx, mr_ra_ptrdes_t * ptrs)
{
  uint64_t value;
  if (MR_SUCCESS != xdr_bitfield_value (xdrs, &ptrs->ra[idx].fd, &value))
    return (MR_FAILURE);
  return (mr_load_bitfield_value (&ptrs->ra[idx], &value));
}

/**
 * Loads char array from XDR stream.
 * @param xdrs XDR stream descriptor
 * @param idx index of node in ptrs
 * @param ptrs array with descriptor of loaded data
 * @return status
 */
static mr_status_t
xdr_load_array (XDR * xdrs, int idx, mr_ra_ptrdes_t * ptrs)
{
  int i;
  char * data = ptrs->ra[idx].data.ptr;
  mr_fd_t fd_ = ptrs->ra[idx].fd;
  int count = fd_.param.array_param.count;
  int row_count = fd_.param.array_param.row_count;

  if (1 == fd_.param.array_param.row_count)
    fd_.mr_type = fd_.mr_type_aux;
  else
    {
      fd_.param.array_param.count = row_count;
      fd_.param.array_param.row_count = 1;
    }

  for (i = 0; i < count; i += row_count)
    if (MR_SUCCESS != xdr_load_inner (&data[i * fd_.size], &fd_, xdrs, ptrs, idx))
      return (MR_FAILURE);
  return (MR_SUCCESS);
}

/**
 * Saves pointer into binary XDR stream. First goes flags of the node
 * and if pointer is not NULL, then ref_idx goes next.
 * @param xdrs XDR stream descriptor
 * @param idx index of the node in nodes collection
 * @param ptrs resizeable array with pointers descriptors
 * @return status of operation. 0 - failure, !0 - success.
 */
static mr_status_t
xdr_save_pointer (XDR * xdrs, int idx, mr_ra_ptrdes_t * ptrs)
{
  if (!xdr_int32_t (xdrs, (void*)&ptrs->ra[idx].flags))
    return (MR_FAILURE);

  if (ptrs->ra[idx].ref_idx >= 0)
    return (xdr_int32_t (xdrs, &ptrs->ra[ptrs->ra[idx].ref_idx].idx) ? MR_SUCCESS : MR_FAILURE);

  if (!xdr_int32_t (xdrs, &ptrs->ra[idx].ref_idx))
    return (MR_FAILURE);
  
  if (true == ptrs->ra[idx].flags.is_null)
    return (MR_SUCCESS);

  if (!xdr_ssize_t (xdrs, &ptrs->ra[idx].MR_SIZE))
    return (MR_FAILURE);
  
  if (ptrs->ra[idx].flags.is_opaque_data && (ptrs->ra[idx].MR_SIZE > 0))
    return (xdr_opaque (xdrs, *(void**)ptrs->ra[idx].data.ptr, ptrs->ra[idx].MR_SIZE) ? MR_SUCCESS : MR_FAILURE);

  return (MR_SUCCESS);
}

/**
 * Loads pointer from binary XDR stream.
 * @param xdrs XDR stream descriptor
 * @param idx index of the node in nodes collection
 * @param ptrs resizeable array with pointers descriptors
 * @return status of operation
 */
static mr_status_t
xdr_load_pointer (XDR * xdrs, int idx, mr_ra_ptrdes_t * ptrs)
{
  char ** data = ptrs->ra[idx].data.ptr;
  mr_fd_t fd_ = ptrs->ra[idx].fd;

  *data = NULL;
  
  if (!xdr_int32_t (xdrs, (void*)&ptrs->ra[idx].flags))
    return (MR_FAILURE);

  if (!xdr_int32_t (xdrs, &ptrs->ra[idx].ref_idx))
    return (MR_FAILURE);
  
  if (true == ptrs->ra[idx].flags.is_null)
    return (MR_SUCCESS);
      
  if (ptrs->ra[idx].ref_idx < 0)
    {
      int i, count = 0;
      
      if (!xdr_ssize_t (xdrs, &ptrs->ra[idx].MR_SIZE))
	return (MR_FAILURE);

      if (!ptrs->ra[idx].flags.is_opaque_data)
	{
	  if (fd_.size > 0) /* types with zero size used for dynamics strings allocation */
	    {
	      count = ptrs->ra[idx].MR_SIZE / fd_.size;
	      ptrs->ra[idx].MR_SIZE = count * fd_.size;
	    }
	  else
	    {
	      count = 1;
	      ptrs->ra[idx].MR_SIZE = sizeof (char);
	    }
	}

      if (ptrs->ra[idx].MR_SIZE <= 0)
	return (MR_FAILURE);

      mr_pointer_set_size (idx, ptrs);
      
      *data = MR_MALLOC (ptrs->ra[idx].MR_SIZE);
      if (NULL == *data)
	{
	  MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	  return (MR_FAILURE);
	}
      memset (*data, 0, ptrs->ra[idx].MR_SIZE);
      
      if (ptrs->ra[idx].flags.is_opaque_data)
	return (xdr_opaque (xdrs, *data, ptrs->ra[idx].MR_SIZE) ? MR_SUCCESS : MR_FAILURE);
      else
	{
	  fd_.mr_type = fd_.mr_type_aux;
	  for (i = 0; i < count; ++i)
	    if (MR_SUCCESS != xdr_load_inner (*data + i * fd_.size, &fd_, xdrs, ptrs, idx))
	      return (MR_FAILURE);
	}
    }
  
  return (MR_SUCCESS);
}

/**
 * Init save handlers Table
 */
static xdr_handler_t xdr_save_handler[] =
  {
    [MR_TYPE_NONE] = xdr_none,
    [MR_TYPE_VOID] = xdr_none,
    [MR_TYPE_ENUM] = xdr_save_enum,
    [MR_TYPE_BITFIELD] = xdr_save_bitfield,
    [MR_TYPE_BITMASK] = xdr_save_bitmask,
    [MR_TYPE_BOOL] = xdr_int_,
    [MR_TYPE_INT8] = xdr_int_,
    [MR_TYPE_UINT8] = xdr_uint_,
    [MR_TYPE_INT16] = xdr_int_,
    [MR_TYPE_UINT16] = xdr_uint_,
    [MR_TYPE_INT32] = xdr_int_,
    [MR_TYPE_UINT32] = xdr_uint_,
    [MR_TYPE_INT64] = xdr_int_,
    [MR_TYPE_UINT64] = xdr_uint_,
    [MR_TYPE_FLOAT] = xdr_float_,
    [MR_TYPE_COMPLEX_FLOAT] = xdr_complex_float,
    [MR_TYPE_DOUBLE] = xdr_double_,
    [MR_TYPE_COMPLEX_DOUBLE] = xdr_complex_double,
    [MR_TYPE_LONG_DOUBLE] = xdr_long_double_,
    [MR_TYPE_COMPLEX_LONG_DOUBLE] = xdr_complex_long_double,
    [MR_TYPE_CHAR] = xdr_int_,
    [MR_TYPE_CHAR_ARRAY] = xdr_char_array_,
    [MR_TYPE_STRING] = xdr_save_string,
    [MR_TYPE_STRUCT] = xdr_none,
    [MR_TYPE_FUNC] = xdr_save_func,
    [MR_TYPE_FUNC_TYPE] = xdr_save_func,
    [MR_TYPE_ARRAY] = xdr_none,
    [MR_TYPE_POINTER] = xdr_save_pointer,
    [MR_TYPE_UNION] = xdr_save_union,
    [MR_TYPE_ANON_UNION] = xdr_save_union,
    [MR_TYPE_NAMED_ANON_UNION] = xdr_save_union,
  };

static mr_status_t
xdr_save_node (mr_ra_ptrdes_t * ptrs, int idx, void * context)
{
  XDR * xdrs = context;
  mr_fd_t * fdp = &ptrs->ra[idx].fd;
  
  if ((fdp->mr_type < MR_TYPE_LAST) && xdr_save_handler[fdp->mr_type])
    {
      if (MR_SUCCESS != xdr_save_handler[fdp->mr_type] (xdrs, idx, ptrs))
	return (MR_FAILURE);
    }
  else
    {
      MR_MESSAGE_UNSUPPORTED_NODE_TYPE_ (fdp);
      return (MR_FAILURE);
    }
  return (MR_SUCCESS);
}

/**
 * Public function. Save scheduler. Save any object into XDR stream.
 * @param xdrs XDR context structure
 * @param ptrs resizeable array with pointers descriptors
 * @return status
 */
mr_status_t
xdr_save (XDR * xdrs, mr_ra_ptrdes_t * ptrs)
{
  return (mr_ptrs_ds (ptrs, xdr_save_node, xdrs));
}

/**
 * Init load handlers Table
 */
static xdr_handler_t xdr_load_handler[] =
  {
    [MR_TYPE_NONE] = xdr_none,
    [MR_TYPE_VOID] = xdr_none,
    [MR_TYPE_ENUM] = xdr_load_stringified_type,
    [MR_TYPE_BITFIELD] = xdr_load_bitfield,
    [MR_TYPE_BITMASK] = xdr_load_stringified_type,
    [MR_TYPE_BOOL] = xdr_int_,
    [MR_TYPE_INT8] = xdr_int_,
    [MR_TYPE_UINT8] = xdr_uint_,
    [MR_TYPE_INT16] = xdr_int_,
    [MR_TYPE_UINT16] = xdr_uint_,
    [MR_TYPE_INT32] = xdr_int_,
    [MR_TYPE_UINT32] = xdr_uint_,
    [MR_TYPE_INT64] = xdr_int_,
    [MR_TYPE_UINT64] = xdr_uint_,
    [MR_TYPE_FLOAT] = xdr_float_,
    [MR_TYPE_COMPLEX_FLOAT] = xdr_complex_float,
    [MR_TYPE_DOUBLE] = xdr_double_,
    [MR_TYPE_COMPLEX_DOUBLE] = xdr_complex_double,
    [MR_TYPE_LONG_DOUBLE] = xdr_long_double_,
    [MR_TYPE_COMPLEX_LONG_DOUBLE] = xdr_complex_long_double,
    [MR_TYPE_CHAR] = xdr_int_,
    [MR_TYPE_CHAR_ARRAY] = xdr_char_array_,
    [MR_TYPE_STRING] = xdr_load_string,
    [MR_TYPE_STRUCT] = xdr_load_struct,
    [MR_TYPE_FUNC] = xdr_load_stringified_type,
    [MR_TYPE_FUNC_TYPE] = xdr_load_stringified_type,
    [MR_TYPE_ARRAY] = xdr_load_array,
    [MR_TYPE_POINTER] = xdr_load_pointer,
    [MR_TYPE_UNION] = xdr_load_union,
    [MR_TYPE_ANON_UNION] = xdr_load_union,
    [MR_TYPE_NAMED_ANON_UNION] = xdr_load_union,
  };
