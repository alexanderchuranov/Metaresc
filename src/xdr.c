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
    return (NULL);
  
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

  /* in XDR a reference holds the index of the target node */
  for (i = 1; (MR_SUCCESS == status) && (i < count); ++i)
    if (ptrs->ra[i].flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE))
      if (MR_SUCCESS != mr_set_crossref (ptrs, i, ptrs->ra[i].first_child))
        status = MR_FAILURE;
  return (status);
}

/**
 * Adds a node for a value, links it to its parent and loads the value with
 * the XDR handler of its type.
 * @param data pointer on the value
 * @param fdp field descriptor of the value
 * @param xdrs XDR stream
 * @param ptrs array with descriptors of loaded data
 * @param parent index of the parent node
 * @return status
 */
static mr_status_t
xdr_load_inner (void * data, mr_fd_t * fdp, XDR * xdrs, mr_ra_ptrdes_t * ptrs, mr_idx_t parent)
{
  mr_status_t status = MR_FAILURE;
  mr_idx_t idx = mr_add_ptr_to_list (ptrs);
  if (MR_NULL_IDX == idx)
    return (MR_FAILURE);
  
  ptrs->ra[idx].data.ptr = data;

  ptrs->ra[idx].fdp = fdp;
  ptrs->ra[idx].MR_SIZE = fdp->stype.size;
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

  /* non persistent descriptors are local variables of the callers, so they can't be referenced after return */
  if (fdp->non_persistent)
    ptrs->ra[idx].fdp = fdp->stype.tdp ? &fdp->stype.tdp->mr_ptr_fd : NULL;

  return (status);
}

/**
 * Loads a value from an XDR stream, resolves references and validates the
 * loaded data. A failed load frees everything it allocated.
 * @param data pointer on the value to load into
 * @param fdp field descriptor of the value
 * @param xdrs XDR stream
 * @return status
 */
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

  if (MR_SUCCESS == status)
    status = mr_set_crossrefs (&ptrs);

  if (MR_SUCCESS == status)
    status = mr_validate_load (&ptrs);

  /* a failed load must not leave allocated memory behind */
  if (MR_SUCCESS != status)
    mr_free_loaded_pointers (&ptrs);

  if (ptrs.ra != NULL)
    MR_FREE (ptrs.ra);
  return (status);
}

/**
 * Handler for MR_TYPE_NONE.
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

/**
 * Encodes or decodes an unsigned byte. Not provided by all XDR libraries.
 * @param xdrs XDR stream
 * @param cp pointer on the value
 * @return true on success
 */
bool_t
xdr_uint8_t (XDR * xdrs, uint8_t * cp)
{
  return (xdr_int8_t (xdrs, (int8_t*)cp));
}

/**
 * Encodes or decodes a 64 bit integer as two 32 bit words, high word first.
 * @param xdrs XDR stream
 * @param int64 pointer on the value
 * @return true on success
 */
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
      *int64 = ((uint64_t) hi) << 32 | (uint32_t) low;
      return (true);

    case XDR_FREE:
      return (true);

    default:
      return (false);
    }
}

/**
 * Encodes or decodes an unsigned 64 bit integer.
 * @param xdrs XDR stream
 * @param uint64 pointer on the value
 * @return true on success
 */
bool_t
xdr_uint64_t (XDR * xdrs, uint64_t * uint64)
{
  return (xdr_int64_t (xdrs, (int64_t*) uint64));
}

/*
  Defines static mr_status_t _xdr_<TYPE> (XDR * xdrs, mr_idx_t idx, mr_ra_ptrdes_t * ptrs),
  an XDR handler that encodes or decodes the integer node idx with xdr_<TYPE>.
  It returns MR_SUCCESS or MR_FAILURE.
*/
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

/**
 * XDR handler for a 128 bit integer, encoded as two 64 bit words.
 * @param xdrs XDR stream
 * @param idx index of the node
 * @param ptrs array with node descriptors
 * @return status
 */
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

/*
  Defines static mr_status_t xdr_complex_<NAME_SUFFIX> (XDR * xdrs, mr_idx_t idx, mr_ra_ptrdes_t * ptrs),
  an XDR handler that encodes or decodes a complex TYPE_NAME node as its real and imaginary parts.
  It returns MR_SUCCESS or MR_FAILURE.
*/
#define XDR_COMPLEX(NAME_SUFFIX, TYPE_NAME)				\
  static mr_status_t xdr_complex_ ## NAME_SUFFIX (XDR * xdrs, mr_idx_t idx, mr_ra_ptrdes_t * ptrs) { \
    TYPE_NAME real = 0;							\
    TYPE_NAME imag = 0;							\
    complex TYPE_NAME x = 0;						\
    if (XDR_ENCODE == xdrs->x_op)					\
      {									\
        x = *(complex TYPE_NAME*)ptrs->ra[idx].data.ptr;                \
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
        *(complex TYPE_NAME*)ptrs->ra[idx].data.ptr = x;                \
      }									\
    return (MR_SUCCESS);                                                \
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

/**
 * XDR handler for a long double node.
 * @param xdrs XDR stream
 * @param idx index of the node
 * @param ptrs array with node descriptors
 * @return status
 */
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
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  /*
    A pointer on a char array type without a descriptor has elements of unknown size.
    xdr_load_pointer allocates a single byte for it, and the string grows to the length
    from the stream. Elements of a known size are fixed char arrays in the block of the pointer.
  */
  bool is_a_dynamic_string = ((parent != MR_NULL_IDX) && (MR_TYPE_POINTER == ptrs->ra[parent].mr_type)
                              && ((NULL == ptrdes->fdp) || (0 == ptrdes->fdp->stype.size)));
  uint32_t str_len;

  if (XDR_ENCODE == xdrs->x_op)
    {
      /* save sets MR_SIZE of a char array to its size, also for the elements of pointers */
      typeof (ptrdes->MR_SIZE) size = ptrdes->MR_SIZE;
      char * string = ptrs->ra[idx].data.string;
      for (str_len = 0; (str_len < size) && (*string != 0); ++str_len)
        ++string;

      if (str_len < size)
        str_len += 1;
    }

  if (!xdr_uint32_t (xdrs, &str_len))
    return (MR_FAILURE);

  if (XDR_DECODE == xdrs->x_op)
    {
      if (is_a_dynamic_string)
        {
          /* dynamic string: grow the allocation to the length from the stream */
          if (str_len > sizeof (char))
            {
              void * data = MR_REALLOC (ptrs->ra[idx].data.ptr, str_len);
              if (NULL == data)
                {
                  MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
                  return (MR_FAILURE);
                }
              *(void**)ptrs->ra[parent].data.ptr = ptrs->ra[idx].data.ptr = data;
            }
        }
      else
        {
          /* fixed-size char array: a length from the stream that exceeds the
             declared field size would overflow the inline buffer */
          uint32_t max_size = ptrdes->fdp ? ptrdes->fdp->stype.size : 0;
          if (str_len > max_size)
            {
              MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_RANGE_CHECK, ptrdes->fdp ? ptrdes->fdp->name.str : "");
              return (MR_FAILURE);
            }
        }
    }
  return (xdr_opaque (xdrs, ptrs->ra[idx].data.ptr, str_len) ? MR_SUCCESS : MR_FAILURE);
}

/* flags that XDR save writes for pointers and strings, MR_IS_TYPED is set only by the CINIT loader */
#define XDR_PTRDES_FLAGS (MR_IS_NULL | MR_IS_REFERENCE | MR_IS_REFERENCED | MR_IS_CONTENT_REFERENCE | MR_IS_UNNAMED)

/**
 * Loads flags of a pointer or a string node from XDR stream and rejects
 * values that XDR save never writes.
 * @param xdrs XDR stream descriptor
 * @param idx index of node in ptrs
 * @param ptrs array with descriptor of loaded data
 * @return status
 */
static mr_status_t
xdr_load_flags (XDR * xdrs, mr_idx_t idx, mr_ra_ptrdes_t * ptrs)
{
  uint8_t flags = 0;
  if (!xdr_uint8_t (xdrs, &flags))
    return (MR_FAILURE);

  /* unknown bits, or a node that is both a reference and a content reference */
  if ((flags & ~XDR_PTRDES_FLAGS) ||
      ((flags & MR_IS_REFERENCE) && (flags & MR_IS_CONTENT_REFERENCE)))
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_XDR_WRONG_FLAGS, flags);
      return (MR_FAILURE);
    }

  ptrs->ra[idx].flags = flags;
  return (MR_SUCCESS);
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
  /* the pointer must be initialized before any failure, otherwise rollback would free garbage */
  *(char**)ptrs->ra[idx].data.ptr = NULL;

  if (MR_SUCCESS != xdr_load_flags (xdrs, idx, ptrs))
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

/**
 * Encodes or decodes the value of a pointer as an unsigned integer of the
 * pointer size.
 * @param xdrs XDR stream
 * @param idx index of the node
 * @param ptrs array with node descriptors
 * @return status
 */
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

/**
 * XDR handler for a bool node, encoded as an unsigned integer of the size of
 * bool.
 * @param xdrs XDR stream
 * @param idx index of the node
 * @param ptrs array with node descriptors
 * @return status
 */
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

/**
 * Saves the number of loaded bytes of a single dimension array field. Other
 * arrays have a fixed size and save nothing.
 * @param xdrs XDR stream
 * @param idx index of the array node
 * @param ptrs array with node descriptors
 * @return status
 */
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
 * Loads an array from XDR stream. A single dimensional array of a structure
 * is preceded by the number of bytes of its loaded elements.
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
  mr_idx_t max_count = count; /* declared capacity of the inline array */

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

          if (ptrs->ra[idx].MR_SIZE % fd_.stype.size != 0)
            {
              MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_WRONG_ARRAY_SIZE, ptrs->ra[idx].MR_SIZE);
              return (MR_FAILURE);
            }

          count = ptrs->ra[idx].MR_SIZE / fd_.stype.size;
          /* a count from the stream larger than the declared array capacity
             would write past the inline buffer */
          if (count > max_count)
            {
              MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_RANGE_CHECK, fd_.name.str);
              return (MR_FAILURE);
            }
          ptrs->ra[idx].MR_SIZE = count * fd_.stype.size;
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
 * Saves pointer into binary XDR stream. First go the flags of the node.
 * A reference is followed by the index of the target node. A pointer that
 * is not NULL is followed by the size of its block in bytes, and opaque
 * data by the bytes of the block. Elements of other pointers are saved
 * as child nodes.
 * @param xdrs XDR stream descriptor
 * @param idx index of the node in nodes collection
 * @param ptrs resizeable array with pointers descriptors
 * @return MR_SUCCESS or MR_FAILURE
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
  
  if (mr_is_opaque_data (&ptrs->ra[idx]) && (ptrs->ra[idx].MR_SIZE > 0))
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

  if (MR_SUCCESS != xdr_load_flags (xdrs, idx, ptrs))
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
  bool opaque = mr_is_opaque_data (&ptrs->ra[idx]);

  if (!opaque)
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

  *data = MR_CALLOC (1, ptrs->ra[idx].MR_SIZE);
  if (NULL == *data)
    {
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      return (MR_FAILURE);
    }

  if (opaque)
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

/**
 * DFS visitor that saves a node with the XDR handler of its type.
 * @param ptrs array with node descriptors
 * @param idx index of the node
 * @param level depth of the node
 * @param order DFS order, only pre-order is handled
 * @param context XDR stream
 * @return status
 */
static mr_status_t
xdr_save_node (mr_ptrdes_t * ptrs, mr_idx_t idx, int level, mr_dfs_order_t order, void * context)
{
  if (MR_DFS_PRE_ORDER != order)
    return (MR_SUCCESS);

  XDR * xdrs = context;

  xdr_handler_t save_handler = NULL;
  if ((ptrs[idx].mr_type >= 0) && (ptrs[idx].mr_type < MR_TYPE_LAST))
    save_handler = xdr_save_handler[ptrs[idx].mr_type];

  mr_ra_ptrdes_t mr_ra_ptrdes = { .ra = ptrs };
  if (save_handler)
    return (save_handler (xdrs, idx, &mr_ra_ptrdes));

  MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNSUPPORTED_NODE_TYPE, ptrs[idx].mr_type);
  return (MR_FAILURE);
}

/**
 * Public function. Save scheduler. Save any object into XDR stream.
 * @param xdrs XDR context structure
 * @param ptrs resizeable array with pointers descriptors
 * @return status
 */
mr_status_t
mr_xdr_save (XDR * xdrs, mr_ptrdes_t * ptrs)
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
