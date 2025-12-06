/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

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

TYPEDEF_FUNC (mr_status_t, xdr_handler_t, (XDR *, mr_idx_t, mr_ra_ptrdes_t *));

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

  if (NULL == ra)
    return (false);
  
  if (ra->MR_SIZE - xdrs->x_handy < sizeof (int32_t))
    return (false);
  
  char * base = ra->data.ptr;
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

  if (NULL == ra)
    return (false);

  if (ra->MR_SIZE - xdrs->x_handy < len)
    return (false);

  char * base = ra->data.ptr;
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

  if (NULL == ra)
    return (false);
  
  if (ra->MR_SIZE - xdrs->x_handy < len)
    return (NULL);

  char * base = ra->data.ptr;
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
mr_xdrra_create (XDR * xdrs, mr_rarray_t * ra, enum xdr_op op)
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
  mr_idx_t i, count = ptrs->size / sizeof (ptrs->ra[0]);
  mr_status_t status = MR_SUCCESS;

  /* set all cross refernces */
  for (i = 1; i < count; ++i)
    if (ptrs->ra[i].flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE))
      {
	mr_idx_t ref_idx = ptrs->ra[i].first_child;
	if (ref_idx >= count)
	  {
	    MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNDEFINED_REF_IDX, "ref_idx", ref_idx);
	    status = MR_FAILURE;
	  }
	else
	  {
	    void * data = ptrs->ra[ref_idx].data.ptr;

	    if ((MR_TYPE_POINTER != ptrs->ra[i].mr_type) &&
		(MR_TYPE_STRING != ptrs->ra[i].mr_type))
	      {
		MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNEXPECTED_TARGET_TYPE, ptrs->ra[i].mr_type);
		status = MR_FAILURE;
		continue;
	      }

	    if (ptrs->ra[i].flags & MR_IS_CONTENT_REFERENCE)
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
xdr_load_inner (void * data, mr_fd_t * fdp, XDR * xdrs, mr_ra_ptrdes_t * ptrs, mr_idx_t parent)
{
  mr_status_t status = MR_FAILURE;
  mr_idx_t idx = mr_add_ptr_to_list (ptrs);
  if (MR_NULL_IDX == idx)
    return (MR_FAILURE);
  
  ptrs->ra[idx].data.ptr = data;

  ptrs->ra[idx].fdp = fdp;
  ptrs->ra[idx].mr_type = fdp->stype.mr_type;
  ptrs->ra[idx].mr_type_aux = fdp->stype.mr_type_aux;

  mr_add_child (ptrs, parent, idx);

  xdr_handler_t load_handler = NULL;
  if ((fdp->stype.mr_type >= 0) && (fdp->stype.mr_type < MR_TYPE_LAST))
    load_handler = xdr_load_handler[fdp->stype.mr_type];
  
  if (load_handler)
    status = load_handler (xdrs, idx, ptrs);
  else
    MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNSUPPORTED_NODE_TYPE, fdp->stype.mr_type);

  return (status);
}

mr_status_t
mr_xdr_load (void * data, mr_fd_t * fdp, XDR * xdrs)
{
  if (NULL == data)
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_NULL_POINTER);
      return (MR_FAILURE);
    }

  mr_ra_ptrdes_t ptrs = { .ra = NULL, .size = 0, .alloc_size = 0, };
  mr_add_ptr_to_list (&ptrs);
  if (NULL == ptrs.ra)
    return (MR_FAILURE);

  mr_status_t status = xdr_load_inner (data, fdp, xdrs, &ptrs, 0);
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
xdr_none (XDR * xdrs, mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  return (MR_SUCCESS);
}

/**
 * Handler for type char.
 * @param xdrs XDR stream descriptor
 * @param cp pointer on char
 * @return status
 */
bool_t
xdr_int8_t (XDR * xdrs, int8_t * cp)
{
  int x = 0;
  if (XDR_ENCODE == xdrs->x_op)
    x = *cp;
  if (!xdr_int (xdrs, &x))
    return (false);
  if (XDR_DECODE == xdrs->x_op)
    *cp = x;
  return (true);
}

bool_t
xdr_uint8_t (XDR * xdrs, uint8_t * cp)
{
  return (xdr_int8_t (xdrs, (int8_t*)cp));
}

bool_t
xdr_int64_t (XDR * xdrs, int64_t * int64)
{
  int32_t hi, low;

  switch (xdrs->x_op)
    {
    case XDR_ENCODE:
      hi = *int64 >> 32;
      low = *int64;
      if (!XDR_PUTINT32 (xdrs, &hi))
	return (false);
      return (XDR_PUTINT32 (xdrs, &low));

    case XDR_DECODE:
      if (!XDR_GETINT32 (xdrs, &hi))
	return (false);
      if (!XDR_GETINT32 (xdrs, &low))
	return (false);
      *int64 = ((int64_t) hi) << 32 | (uint32_t) low;
      return (true);

    case XDR_FREE:
      return (true);

    default:
      return (false);
    }
}

bool_t
xdr_uint64_t (XDR * xdrs, uint64_t * uint64)
{
  return (xdr_int64_t (xdrs, (int64_t*) uint64));
}

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

#define XDR_INT_TYPE(TYPE)						\
  static mr_status_t _xdr_ ## TYPE (XDR * xdrs, mr_idx_t idx, mr_ra_ptrdes_t * ptrs) \
  { return (xdr_ ## TYPE (xdrs, ptrs->ra[idx].data.ptr) ? MR_SUCCESS : MR_FAILURE); }

XDR_INT_TYPE (uint8_t)
XDR_INT_TYPE (int8_t)
XDR_INT_TYPE (uint16_t)
XDR_INT_TYPE (int16_t)
XDR_INT_TYPE (uint32_t)
XDR_INT_TYPE (int32_t)
XDR_INT_TYPE (uint64_t)
XDR_INT_TYPE (int64_t)

static mr_status_t _xdr_uint128_t (XDR * xdrs, mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  uint64_t * u64 = ptrs->ra[idx].data.ptr;
  if (!xdr_uint64_t (xdrs, &u64[0]))
    return (MR_FAILURE);
  return (xdr_uint64_t (xdrs, &u64[1]) ? MR_SUCCESS : MR_FAILURE);
}

/**
 * Handler for type float.
 * @param xdrs XDR stream descriptor
 * @param idx index of node in ptrs
 * @param ptrs array with descriptor of loaded data
 * @return status
 */
static mr_status_t
xdr_float_ (XDR * xdrs, mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  return (xdr_float (xdrs, ptrs->ra[idx].data.ptr) ? MR_SUCCESS : MR_FAILURE);
}

#define XDR_COMPLEX(NAME_SUFFIX, TYPE_NAME)					\
  static mr_status_t xdr_complex_ ## NAME_SUFFIX (XDR * xdrs, mr_idx_t idx, mr_ra_ptrdes_t * ptrs) { \
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
xdr_double_ (XDR * xdrs, mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
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
xdr_long_double_ (XDR * xdrs, mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
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
xdr_char_array_ (XDR * xdrs, mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  mr_idx_t parent = ptrs->ra[idx].parent;
  bool is_a_dynamic_string = ((parent != MR_NULL_IDX) && (MR_TYPE_POINTER == ptrs->ra[parent].mr_type));
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  typeof (ptrdes->MR_SIZE) str_len;

  if (XDR_ENCODE == xdrs->x_op)
    {
      typeof (ptrdes->MR_SIZE) size = ptrdes->MR_SIZE;
      char * string = ptrs->ra[idx].data.string;
      if (is_a_dynamic_string)
	str_len = strlen (string) + 1;
      else
	{
	  for (str_len = 0; (str_len < size) && (*string != 0); ++str_len)
	    ++string;

	  if (str_len < size)
	    str_len += 1;
	}
    }

  if (!xdr_uint32_t (xdrs, &str_len))
    return (MR_FAILURE);

  if ((XDR_DECODE == xdrs->x_op) && (str_len > sizeof (char)) && is_a_dynamic_string)
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
xdr_save_string (XDR * xdrs, mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  if (!xdr_uint8_t (xdrs, (uint8_t*)&ptrs->ra[idx].flags))
    return (MR_FAILURE);

  if (ptrs->ra[idx].flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE))
    {
      uint32_t ref_idx = ptrs->ra[ptrs->ra[idx].first_child].idx;
      if (!xdr_uint32_t (xdrs, &ref_idx))
	return (MR_FAILURE);
    }
  else if (!(ptrs->ra[idx].flags & MR_IS_NULL))
    {
      void ** str = ptrs->ra[idx].data.ptr;
      uint32_t size = strlen (*str);

      if (!xdr_uint32_t (xdrs, &size))
	return (MR_FAILURE);
      if (!xdr_opaque (xdrs, *str, size))
	return (MR_FAILURE);
    }
  return (MR_SUCCESS);
}

/**
 * Load handler for char pointers.
 * @param xdrs XDR stream descriptor
 * @param idx index of node in ptrs
 * @param ptrs array with descriptor of loaded data
 * @return status
 */
static mr_status_t
xdr_load_string (XDR * xdrs, mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  if (!xdr_uint8_t (xdrs, (uint8_t*)&ptrs->ra[idx].flags))
    return (MR_FAILURE);

  if (ptrs->ra[idx].flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE))
    {
      uint32_t ref_idx = MR_NULL_IDX;
      if (!xdr_uint32_t (xdrs, &ref_idx))
	return (MR_FAILURE);
      ptrs->ra[idx].first_child = ref_idx;
      return (MR_SUCCESS);
    }
  else if (ptrs->ra[idx].flags & MR_IS_NULL)
    {
      char ** str = ptrs->ra[idx].data.ptr;
      *str = NULL;
      return (MR_SUCCESS);
    }
  else
    {
      char ** str = ptrs->ra[idx].data.ptr;
      uint32_t size = 0;
      *str = NULL;
      if (!xdr_uint32_t (xdrs, &size))
	return (MR_FAILURE);
      *str = MR_CALLOC (size + 1, sizeof (**str));
      if (NULL == *str)
	{
	  MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
	  return (MR_FAILURE);
	}
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
 * @return status
 */
static mr_status_t
xdr_load_struct (XDR * xdrs, mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  mr_td_t * tdp = ptrs->ra[idx].fdp ? ptrs->ra[idx].fdp->stype.tdp : NULL;
  char * data = ptrs->ra[idx].data.ptr;

  if (NULL == tdp)
    return (MR_FAILURE);

  mr_idx_t i;
  for (i = 0; i < tdp->param.struct_param.fields_count; ++i)
    {
      mr_fd_t * fdp = tdp->param.struct_param.fields[i];
      if (MR_SUCCESS != xdr_load_inner (&data[fdp->offset], fdp, xdrs, ptrs, idx))
	return (MR_FAILURE);
    }
  return (MR_SUCCESS);
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
xdr_save_union (XDR * xdrs, mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  /* save union branch field name as string */
  char * dummy_str = "";
  mr_ptrdes_t ptrdes = { /* temporary pointer descriptor for this string */
    .data.ptr = &dummy_str,
    .flags = MR_NO_FLAGS,
  };

  if (ptrs->ra[idx].first_child != MR_NULL_IDX)
    {
      mr_idx_t first_child = ptrs->ra[idx].first_child;
      if (ptrs->ra[first_child].fdp)
	ptrdes.data.ptr = &ptrs->ra[first_child].fdp->name.str;
    }
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
xdr_load_union (XDR * xdrs, mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  mr_td_t * tdp = ptrs->ra[idx].fdp ? ptrs->ra[idx].fdp->stype.tdp : NULL;
  char * data = ptrs->ra[idx].data.ptr;
  char * discriminator = NULL;
  mr_status_t status = MR_FAILURE;
  mr_ptrdes_t ptrdes = { .data.ptr = &discriminator, }; /* temporary pointer descriptor for union discriminator string */
  mr_ra_ptrdes_t ptrs_ = { .ra = &ptrdes, .size = sizeof (ptrdes), .alloc_size = -1, }; /* temporary resizeable array */

  if (NULL == tdp)
    return (MR_FAILURE);

  if (MR_SUCCESS != xdr_load_string (xdrs, 0, &ptrs_))
    MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNION_DISCRIMINATOR_ERROR, discriminator);
  else if (NULL == discriminator)
    MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNION_DISCRIMINATOR_ERROR, discriminator);
  else
    {
      if (0 == tdp->param.struct_param.fields_count) /* check for an empty union */
	status = MR_SUCCESS;
      else
	{
	  mr_fd_t * fdp = mr_get_fd_by_name (tdp, discriminator);
	  if (NULL == fdp)
	    MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNION_DISCRIMINATOR_ERROR, discriminator);
	  else
	    status = xdr_load_inner (data + fdp->offset, fdp, xdrs, ptrs, idx);
	}
      MR_FREE (discriminator);
    }

  return (status);
}

/**
 * Saves enum value as a string.
 * @param xdrs XDR stream descriptor
 * @param idx index of node in ptrs
 * @param ptrs array with descriptor of loaded data
 * @return status
 */
static mr_status_t
_xdr_enum (XDR * xdrs, mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  mr_td_t * tdp = ptrs->ra[idx].fdp ? ptrs->ra[idx].fdp->stype.tdp : NULL;

  if (NULL == tdp)
    return (MR_FAILURE);
  
  switch (tdp->param.enum_param.mr_type_effective)
    {
    case MR_TYPE_UINT8: return (_xdr_uint8_t (xdrs, idx, ptrs));
    case MR_TYPE_INT8: return (_xdr_int8_t (xdrs, idx, ptrs));
    case MR_TYPE_UINT16: return (_xdr_uint16_t (xdrs, idx, ptrs));
    case MR_TYPE_INT16: return (_xdr_int16_t (xdrs, idx, ptrs));
    case MR_TYPE_UINT32: return (_xdr_uint32_t (xdrs, idx, ptrs));
    case MR_TYPE_INT32: return (_xdr_int32_t (xdrs, idx, ptrs));
    case MR_TYPE_UINT64: return (_xdr_uint64_t (xdrs, idx, ptrs));
    case MR_TYPE_INT64: return (_xdr_int64_t (xdrs, idx, ptrs));
    default: return (MR_FAILURE);
    }
  return (MR_FAILURE);
}

static mr_status_t
mr_xdr_pointer (XDR * xdrs, mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  switch (sizeof (void *))
    {
    case sizeof (uint8_t): return (_xdr_uint8_t (xdrs, idx, ptrs));
    case sizeof (uint16_t): return (_xdr_uint16_t (xdrs, idx, ptrs));
    case sizeof (uint32_t): return (_xdr_uint32_t (xdrs, idx, ptrs));
    case sizeof (uint64_t): return (_xdr_uint64_t (xdrs, idx, ptrs));
    case sizeof (mr_uint128_t): return (_xdr_uint128_t (xdrs, idx, ptrs));
    }
  return (MR_FAILURE);
}

static mr_status_t
mr_xdr_bool (XDR * xdrs, mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  switch (sizeof (bool))
    {
    case sizeof (uint8_t): return (_xdr_uint8_t (xdrs, idx, ptrs));
    case sizeof (uint16_t): return (_xdr_uint16_t (xdrs, idx, ptrs));
    case sizeof (uint32_t): return (_xdr_uint32_t (xdrs, idx, ptrs));
    case sizeof (uint64_t): return (_xdr_uint64_t (xdrs, idx, ptrs));
    case sizeof (mr_uint128_t): return (_xdr_uint128_t (xdrs, idx, ptrs));
    }
  return (MR_FAILURE);
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
  mr_ptrdes_t ptrdes = { .fdp = fdp, .data.ptr = data, };
  mr_ra_ptrdes_t ptrs = { .ra = &ptrdes, .size = sizeof (ptrdes), .alloc_size = -1, };
  
  if (XDR_ENCODE == xdrs->x_op)
    return (xdr_save_handler[fdp->stype.mr_type_aux] (xdrs, 0, &ptrs));
  else if (XDR_DECODE == xdrs->x_op)
    return (xdr_load_handler[fdp->stype.mr_type_aux] (xdrs, 0, &ptrs));
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
xdr_save_bitfield (XDR * xdrs, mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  mr_uintmax_t value;
  if (MR_SUCCESS != mr_save_bitfield_value (&ptrs->ra[idx], &value))
    return (MR_FAILURE);
  return (xdr_bitfield_value (xdrs, ptrs->ra[idx].fdp, &value));
}

/**
 * Loads bit field from XDR stream.
 * @param xdrs XDR stream descriptor
 * @param idx index of node in ptrs
 * @param ptrs array with descriptor of loaded data
 * @return status
 */
static mr_status_t
xdr_load_bitfield (XDR * xdrs, mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  mr_uintmax_t value;
  if (MR_SUCCESS != xdr_bitfield_value (xdrs, ptrs->ra[idx].fdp, &value))
    return (MR_FAILURE);
  return (mr_load_bitfield_value (&ptrs->ra[idx], &value));
}

static mr_status_t
xdr_save_array (XDR * xdrs, mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  /* only for single dimensional arrays store actual size */
  if (!ptrs->ra[idx].fdp->non_persistent)
    if ((ptrs->ra[idx].fdp->stype.dim.dim_count == 1) &&
	(ptrs->ra[idx].fdp->stype.dim.dim[0] != 0))
      if (!xdr_uint32_t (xdrs, &ptrs->ra[idx].MR_SIZE))
	return (MR_FAILURE);
  return (MR_SUCCESS);
}

/**
 * Loads char array from XDR stream.
 * @param xdrs XDR stream descriptor
 * @param idx index of node in ptrs
 * @param ptrs array with descriptor of loaded data
 * @return status
 */
static mr_status_t
xdr_load_array (XDR * xdrs, mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  char * data = ptrs->ra[idx].data.ptr;
  mr_fd_t fd_ = *ptrs->ra[idx].fdp;
  mr_idx_t i, count = fd_.stype.dim.dim[0];

  if (0 == count)
    return (MR_SUCCESS);

  fd_.non_persistent = true;
  fd_.stype.size /= count;

  if (fd_.stype.dim.dim_count == 1)
    {
      fd_.stype.mr_type = fd_.stype.mr_type_aux;
      fd_.stype.mr_type_aux = fd_.stype.tdp ? fd_.stype.tdp->mr_type : MR_TYPE_VOID;

      if (!ptrs->ra[idx].fdp->non_persistent)
	{
	  /* only for single dimensional array dynamic sizing is supported */
	  if (!xdr_uint32_t (xdrs, &ptrs->ra[idx].MR_SIZE))
	    return (MR_FAILURE);

	  if (fd_.stype.size == 0)
	    return (MR_SUCCESS);

	  count = ptrs->ra[idx].MR_SIZE / fd_.stype.size;
	  ptrs->ra[idx].MR_SIZE = count * fd_.stype.size;

	  mr_pointer_set_size (idx, ptrs);
	}
    }
  else
    {
      --fd_.stype.dim.dim_count;
      memmove (&fd_.stype.dim.dim[0], &fd_.stype.dim.dim[1], fd_.stype.dim.dim_count * sizeof (fd_.stype.dim.dim[0]));
    }

  for (i = 0; i < count; ++i)
    if (MR_SUCCESS != xdr_load_inner (&data[i * fd_.stype.size], &fd_, xdrs, ptrs, idx))
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
xdr_save_pointer (XDR * xdrs, mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  if (!xdr_uint8_t (xdrs, (uint8_t*)&ptrs->ra[idx].flags))
    return (MR_FAILURE);

  if (ptrs->ra[idx].flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE))
    {
      uint32_t ref_idx = ptrs->ra[ptrs->ra[idx].first_child].idx;
      return (xdr_uint32_t (xdrs, &ref_idx) ? MR_SUCCESS : MR_FAILURE);
    }

  if (ptrs->ra[idx].flags & MR_IS_NULL)
    return (MR_SUCCESS);

  if (!xdr_uint32_t (xdrs, &ptrs->ra[idx].MR_SIZE))
    return (MR_FAILURE);
  
  if ((ptrs->ra[idx].flags & MR_IS_OPAQUE_DATA) && (ptrs->ra[idx].MR_SIZE > 0))
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
xdr_load_pointer (XDR * xdrs, mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  char ** data = ptrs->ra[idx].data.ptr;
  mr_td_t * tdp = ptrs->ra[idx].fdp ? ptrs->ra[idx].fdp->stype.tdp : NULL;
  mr_fd_t fd_;
  
  memset (&fd_, 0, sizeof (fd_));
  fd_.non_persistent = true;
  fd_.unnamed = true;
  fd_.stype.mr_type = ptrs->ra[idx].mr_type_aux;
  fd_.stype.mr_type_aux = tdp ? tdp->mr_type : MR_TYPE_VOID;
  fd_.stype.tdp = tdp;
  fd_.stype.size = mr_type_size (fd_.stype.mr_type);
  if (fd_.stype.size == 0)
    fd_.stype.size = fd_.stype.tdp ? fd_.stype.tdp->size : 0;
  
  *data = NULL;

  if (!xdr_uint8_t (xdrs, (uint8_t*)&ptrs->ra[idx].flags))
    return (MR_FAILURE);

  if (ptrs->ra[idx].flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE))
    {
      uint32_t ref_idx = MR_NULL_IDX;
      if (!xdr_uint32_t (xdrs, &ref_idx))
	return (MR_FAILURE);
      ptrs->ra[idx].first_child = ref_idx;
      return (MR_SUCCESS);
    }

  if (ptrs->ra[idx].flags & MR_IS_NULL)
    return (MR_SUCCESS);

  if (!xdr_uint32_t (xdrs, &ptrs->ra[idx].MR_SIZE))
    return (MR_FAILURE);

  mr_idx_t count = 0;
      
  if (!(ptrs->ra[idx].flags & MR_IS_OPAQUE_DATA))
    {
      if (fd_.stype.size > 0) /* types with zero size used for dynamics strings allocation */
	{
	  count = ptrs->ra[idx].MR_SIZE / fd_.stype.size;
	  ptrs->ra[idx].MR_SIZE = count * fd_.stype.size;
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

  *data = MR_CALLOC (1, ptrs->ra[idx].MR_SIZE);
  if (NULL == *data)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (MR_FAILURE);
    }

  if (ptrs->ra[idx].flags & MR_IS_OPAQUE_DATA)
    return (xdr_opaque (xdrs, *data, ptrs->ra[idx].MR_SIZE) ? MR_SUCCESS : MR_FAILURE);
  else
    {
      mr_idx_t i;
      for (i = 0; i < count; ++i)
	if (MR_SUCCESS != xdr_load_inner (*data + i * fd_.stype.size, &fd_, xdrs, ptrs, idx))
	  return (MR_FAILURE);
    }
  
  return (MR_SUCCESS);
}

/**
 * Init save handlers Table
 */
static xdr_handler_t xdr_save_handler[MR_TYPE_LAST] =
  {
    [MR_TYPE_NONE] = xdr_none,
    [MR_TYPE_VOID] = xdr_none,
    [MR_TYPE_ENUM] = _xdr_enum,
    [MR_TYPE_BITFIELD] = xdr_save_bitfield,
    [MR_TYPE_BOOL] = mr_xdr_bool,
    [MR_TYPE_INT8] = _xdr_int8_t,
    [MR_TYPE_UINT8] = _xdr_uint8_t,
    [MR_TYPE_INT16] = _xdr_int16_t,
    [MR_TYPE_UINT16] = _xdr_uint16_t,
    [MR_TYPE_INT32] = _xdr_int32_t,
    [MR_TYPE_UINT32] = _xdr_uint32_t,
    [MR_TYPE_INT64] = _xdr_int64_t,
    [MR_TYPE_UINT64] = _xdr_uint64_t,
    [MR_TYPE_INT128] = _xdr_uint128_t,
    [MR_TYPE_UINT128] = _xdr_uint128_t,
    [MR_TYPE_FLOAT] = xdr_float_,
    [MR_TYPE_COMPLEX_FLOAT] = xdr_complex_float,
    [MR_TYPE_DOUBLE] = xdr_double_,
    [MR_TYPE_COMPLEX_DOUBLE] = xdr_complex_double,
    [MR_TYPE_LONG_DOUBLE] = xdr_long_double_,
    [MR_TYPE_COMPLEX_LONG_DOUBLE] = xdr_complex_long_double,
    [MR_TYPE_CHAR] = _xdr_int8_t,
    [MR_TYPE_CHAR_ARRAY] = xdr_char_array_,
    [MR_TYPE_STRING] = xdr_save_string,
    [MR_TYPE_STRUCT] = xdr_none,
    [MR_TYPE_FUNC] = mr_xdr_pointer,
    [MR_TYPE_FUNC_TYPE] = mr_xdr_pointer,
    [MR_TYPE_ARRAY] = xdr_save_array,
    [MR_TYPE_POINTER] = xdr_save_pointer,
    [MR_TYPE_UNION] = xdr_save_union,
    [MR_TYPE_ANON_UNION] = xdr_save_union,
    [MR_TYPE_NAMED_ANON_UNION] = xdr_save_union,
  };

static mr_status_t
xdr_save_node (mr_ra_ptrdes_t * ptrs, mr_idx_t idx, int level, mr_dfs_order_t order, void * context)
{
  if (MR_DFS_PRE_ORDER != order)
    return (MR_SUCCESS);

  XDR * xdrs = context;

  xdr_handler_t save_handler = NULL;
  if ((ptrs->ra[idx].mr_type >= 0) && (ptrs->ra[idx].mr_type < MR_TYPE_LAST))
    save_handler = xdr_save_handler[ptrs->ra[idx].mr_type];
  
  if (save_handler)
    return (save_handler (xdrs, idx, ptrs));

  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNSUPPORTED_NODE_TYPE, ptrs->ra[idx].mr_type);
  return (MR_FAILURE);
}

/**
 * Public function. Save scheduler. Save any object into XDR stream.
 * @param xdrs XDR context structure
 * @param ptrs resizeable array with pointers descriptors
 * @return status
 */
mr_status_t
mr_xdr_save (XDR * xdrs, mr_ra_ptrdes_t * ptrs)
{
  return (mr_ptrs_dfs (ptrs, xdr_save_node, xdrs));
}

/**
 * Init load handlers Table
 */
static xdr_handler_t xdr_load_handler[MR_TYPE_LAST] =
  {
    [MR_TYPE_NONE] = xdr_none,
    [MR_TYPE_VOID] = xdr_none,
    [MR_TYPE_ENUM] = _xdr_enum,
    [MR_TYPE_BITFIELD] = xdr_load_bitfield,
    [MR_TYPE_BOOL] = mr_xdr_bool,
    [MR_TYPE_INT8] = _xdr_int8_t,
    [MR_TYPE_UINT8] = _xdr_uint8_t,
    [MR_TYPE_INT16] = _xdr_int16_t,
    [MR_TYPE_UINT16] = _xdr_uint16_t,
    [MR_TYPE_INT32] = _xdr_int32_t,
    [MR_TYPE_UINT32] = _xdr_uint32_t,
    [MR_TYPE_INT64] = _xdr_int64_t,
    [MR_TYPE_UINT64] = _xdr_uint64_t,
    [MR_TYPE_INT128] = _xdr_uint128_t,
    [MR_TYPE_UINT128] = _xdr_uint128_t,
    [MR_TYPE_FLOAT] = xdr_float_,
    [MR_TYPE_COMPLEX_FLOAT] = xdr_complex_float,
    [MR_TYPE_DOUBLE] = xdr_double_,
    [MR_TYPE_COMPLEX_DOUBLE] = xdr_complex_double,
    [MR_TYPE_LONG_DOUBLE] = xdr_long_double_,
    [MR_TYPE_COMPLEX_LONG_DOUBLE] = xdr_complex_long_double,
    [MR_TYPE_CHAR] = _xdr_int8_t,
    [MR_TYPE_CHAR_ARRAY] = xdr_char_array_,
    [MR_TYPE_STRING] = xdr_load_string,
    [MR_TYPE_STRUCT] = xdr_load_struct,
    [MR_TYPE_FUNC] = mr_xdr_pointer,
    [MR_TYPE_FUNC_TYPE] = mr_xdr_pointer,
    [MR_TYPE_ARRAY] = xdr_load_array,
    [MR_TYPE_POINTER] = xdr_load_pointer,
    [MR_TYPE_UNION] = xdr_load_union,
    [MR_TYPE_ANON_UNION] = xdr_load_union,
    [MR_TYPE_NAMED_ANON_UNION] = xdr_load_union,
  };
