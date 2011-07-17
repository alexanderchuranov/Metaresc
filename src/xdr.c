/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

#include <stdio.h>
#include <string.h>
#include <rpc/types.h>
#include <rpc/xdr.h>
#include <netinet/in.h>

#include <rlconfig.h>
#define RL_MODE PROTO /* explicitly set type of inclusion */
#include <reslib.h>

static bool_t
xdrra_getlong (XDR * xdrs, long * lp)
{
  rl_rarray_t * ra = (rl_rarray_t*)xdrs->x_private;
  char * base = ra->data;
  if (NULL == ra)
    return (FALSE);
  if (ra->size - xdrs->x_handy < sizeof (int32_t))
    return (FALSE);
  *(int32_t*)lp = (int32_t) ntohl ((*((int32_t *)(&base[xdrs->x_handy]))));
  xdrs->x_handy += sizeof (int32_t);
  return (TRUE);
}

static bool_t
xdrra_putlong (XDR * xdrs, const long * lp)
{
  rl_rarray_t * ra = (rl_rarray_t*)xdrs->x_private;
  int32_t * data = rl_rarray_append (ra, sizeof (int32_t));
  if (NULL == ra)
    return (FALSE);
  if (NULL == data)
    return (FALSE);
  *data = htonl (*(int32_t*)lp);
  xdrs->x_handy += sizeof (int32_t);
  return (TRUE);
}

static bool_t
xdrra_getbytes (XDR * xdrs, caddr_t addr, u_int len)
{
  rl_rarray_t * ra = (rl_rarray_t*)xdrs->x_private;
  char * base = ra->data;
  if (NULL == ra)
    return (FALSE);
  if (ra->size - xdrs->x_handy < len)
    return (FALSE);
  memcpy (addr, &base[xdrs->x_handy], len);
  xdrs->x_handy += len;
  return (TRUE);
}

static bool_t
xdrra_putbytes (XDR * xdrs, const char * addr, u_int len)
{
  rl_rarray_t * ra = (rl_rarray_t*)xdrs->x_private;
  int32_t * data;
  if (NULL == ra)
    return (FALSE);
  data = rl_rarray_append (ra, len);
  if (NULL == data)
    return (FALSE);
  memcpy (data, addr, len);
  xdrs->x_handy += len;
  return (TRUE);
}

static u_int
xdrra_getpostn (__const XDR * xdrs)
{
  return (xdrs->x_handy);
}

static bool_t
xdrra_setpostn (XDR * xdrs, u_int pos)
{
  xdrs->x_handy = pos;
  return (TRUE);
}

static int32_t *
xdrra_inline (XDR * xdrs, u_int len)
{
  rl_rarray_t * ra = (rl_rarray_t*)xdrs->x_private;
  char * base = ra->data;
  if (NULL == ra)
    return (FALSE);
  if (ra->size - xdrs->x_handy < len)
    return (NULL);
  return ((int32_t*)&base[xdrs->x_handy]);
}

static void
xdrra_destroy (XDR * xdrs)
{
  rl_rarray_t * ra = (rl_rarray_t*)xdrs->x_private;
  if (NULL == ra)
    return;
  if (ra->data)
    RL_FREE (ra->data);
  ra->data = NULL;
  ra->size = ra->alloc_size = 0;
}

#ifdef HAVE_STRUCT_XDR_OPS_X_GETINT32
static bool_t
xdrra_getint32 (XDR * xdrs, int32_t * ip)
{
  return (xdrra_getlong (xdrs, (long*)ip));
}
#endif /* HAVE_STRUCT_XDR_OPS_X_GETINT32 */

#ifdef HAVE_STRUCT_XDR_OPS_X_PUTINT32
static bool_t
xdrra_putint32 (XDR * xdrs, const int32_t * ip)
{
  return (xdrra_putlong (xdrs, (long*)ip));
}
#endif /* HAVE_STRUCT_XDR_OPS_X_PUTINT32 */

void
xdrra_create (XDR * xdrs, rl_rarray_t * ra, enum xdr_op op)
{
  static struct xdr_ops xdrra_ops =
    {
      .x_getlong = xdrra_getlong,
      .x_putlong = xdrra_putlong,
      .x_getbytes = xdrra_getbytes,
      .x_putbytes = xdrra_putbytes,
      .x_getpostn = xdrra_getpostn,
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

static int
rl_set_crossrefs (rl_ra_rl_ptrdes_t * ptrs)
{
  int count = ptrs->ra.size / sizeof (ptrs->ra.data[0]);
  int i;

  /* set all cross refernces */
  for (i = 0; i < count; ++i)
    if ((ptrs->ra.data[i].ref_idx >= 0) && (ptrs->ra.data[i].ref_idx <= count))
      switch (ptrs->ra.data[i].fd.rl_type_ext)
	{
	case RL_TYPE_EXT_POINTER:
	  *(void**)ptrs->ra.data[i].data = ptrs->ra.data[ptrs->ra.data[i].ref_idx].data;
	  break;
	case RL_TYPE_EXT_RARRAY:
	  ((rl_rarray_t*)(ptrs->ra.data[i].data))->data = ptrs->ra.data[ptrs->ra.data[i].ref_idx].data;
	  break;
	default:
	  break;
	}
  return (!0);
}

static int
xdr_none (XDR * xdrs, int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  return (xdr_void ());
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
    return (FALSE);
  }
  *cp = i;
  return (TRUE);
}

  Here is workaround.
*/

static int __attribute__((unused))
xdr_char_ (XDR * xdrs, char * cp)
{
  int x = 0;
  if (XDR_ENCODE == xdrs->x_op)
    x = *cp;
  if (!xdr_int (xdrs, &x))
    return (0);
  if (XDR_DECODE == xdrs->x_op)
    *cp = x;
  return (!0);
}

#ifndef HAVE_XDR_UINT8_T
#define xdr_uint8_t xdr_char_
#endif /* HAVE_XDR_UINT8_T */
#ifndef HAVE_XDR_INT8_T
#define xdr_int8_t xdr_char_
#endif /* HAVE_XDR_INT8_T */

static int
xdr_uint_ (XDR * xdrs, int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  switch (ptrs->ra.data[idx].fd.size)
    {
    case sizeof (uint8_t): return (xdr_uint8_t (xdrs, ptrs->ra.data[idx].data));
    case sizeof (uint16_t): return (xdr_uint16_t (xdrs, ptrs->ra.data[idx].data));
    case sizeof (uint32_t): return (xdr_uint32_t (xdrs, ptrs->ra.data[idx].data));
    case sizeof (uint64_t): return (xdr_uint64_t (xdrs, ptrs->ra.data[idx].data));
    default: return (0);
    }
}

static int
xdr_int_ (XDR * xdrs, int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  switch (ptrs->ra.data[idx].fd.size)
    {
    case sizeof (int8_t): return (xdr_int8_t (xdrs, ptrs->ra.data[idx].data));
    case sizeof (int16_t): return (xdr_int16_t (xdrs, ptrs->ra.data[idx].data));
    case sizeof (int32_t): return (xdr_int32_t (xdrs, ptrs->ra.data[idx].data));
    case sizeof (int64_t): return (xdr_int64_t (xdrs, ptrs->ra.data[idx].data));
    default: return (0);
    }
}

static int
xdr_float_ (XDR * xdrs, int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  return (xdr_float (xdrs, ptrs->ra.data[idx].data));
}

static int
xdr_double_ (XDR * xdrs, int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  return (xdr_double (xdrs, ptrs->ra.data[idx].data));
}

static int
xdr_long_double (XDR * xdrs, int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  long double * ldp = ptrs->ra.data[idx].data;
  double x = 0;
  if (XDR_ENCODE == xdrs->x_op)
    x = *ldp;
  if (!xdr_double (xdrs, &x))
    return (0);
  if (XDR_DECODE == xdrs->x_op)
    *ldp = x;
  return (!0);
}

static int
xdr_char_array_ (XDR * xdrs, int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  unsigned int size = ptrs->ra.data[idx].fd.size * ptrs->ra.data[idx].fd.param.array_param.count;
  void ** char_array = &ptrs->ra.data[idx].data;
  return (xdr_bytes (xdrs, (char**)char_array, &size, RL_MAX_STRING_LENGTH));
}

static int
xdr_rl_typed_int (XDR * xdrs, rl_type_t rl_type, int size, void * value)
{
  switch (rl_type)
    {
    case RL_TYPE_INT8: return (xdr_int8_t (xdrs, value));
    case RL_TYPE_UINT8: return (xdr_uint8_t (xdrs, value));
    case RL_TYPE_INT16: return (xdr_int16_t (xdrs, value));
    case RL_TYPE_UINT16: return (xdr_uint16_t (xdrs, value));
    case RL_TYPE_INT32: return (xdr_int32_t (xdrs, value));
    case RL_TYPE_UINT32: return (xdr_uint32_t (xdrs, value));
    case RL_TYPE_INT64: return (xdr_int64_t (xdrs, value));
    case RL_TYPE_UINT64: return (xdr_uint64_t (xdrs, value));
    case RL_TYPE_ENUM:
      switch (size)
	{
	case sizeof (uint8_t): return (xdr_uint8_t (xdrs, value));
	case sizeof (uint16_t): return (xdr_uint16_t (xdrs, value));
	case sizeof (uint32_t): return (xdr_uint32_t (xdrs, value));
	case sizeof (uint64_t): return (xdr_uint64_t (xdrs, value));
	default: return (xdr_uint64_t (xdrs, value));
	}
    default: return (xdr_uint64_t (xdrs, value));
    }  
}

static int
xdr_save_bitfield (XDR * xdrs, int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  uint64_t value;
  if (EXIT_SUCCESS != rl_save_bitfield_value (&ptrs->ra.data[idx], &value))
    return (0);
  return (xdr_rl_typed_int (xdrs, ptrs->ra.data[idx].fd.rl_type_aux, ptrs->ra.data[idx].fd.size, &value));
}

static int
xdr_load_bitfield (XDR * xdrs, int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  uint64_t value;
  if (!xdr_rl_typed_int (xdrs, ptrs->ra.data[idx].fd.rl_type_aux, ptrs->ra.data[idx].fd.size, &value))
    return (0);
  return (EXIT_SUCCESS == rl_load_bitfield_value (&ptrs->ra.data[idx], &value));
}

static int
xdr_save_string (XDR * xdrs, int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  void ** str = ptrs->ra.data[idx].data;
  int size = -1;
  if (NULL != *str)
    size = strlen (*str);
  if (!xdr_int (xdrs, &size))
    return (0);
  if (size < 0)
    return (!0);
  return (xdr_bytes (xdrs, (char**)str, (unsigned int*)&size, RL_MAX_STRING_LENGTH));
}

static int
xdr_load_string (XDR * xdrs, int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  void ** str = ptrs->ra.data[idx].data;
  int size = -1;
  *str = NULL;
  if (!xdr_int (xdrs, &size))
    return (0);
  if (size < 0)
    return (!0);
  *str = RL_MALLOC (size + 1);
  if (NULL == *str)
    {
      RL_MESSAGE (RL_LL_FATAL, RL_MESSAGE_OUT_OF_MEMORY);
      return (0);
    }    
  memset (*str, 0, size + 1);
  return (xdr_bytes (xdrs, (char**)str, (unsigned int*)&size, RL_MAX_STRING_LENGTH));
}

static int
xdr_load_struct (XDR * xdrs, int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  int i;
  int count;
  rl_td_t * tdp = rl_get_td_by_name (ptrs->ra.data[idx].fd.type); /* look up for type descriptor */
  char * data = ptrs->ra.data[idx].data;

  /* get pointer on structure descriptor */
  if (NULL == tdp)
    {
      RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_NO_TYPE_DESCRIPTOR, ptrs->ra.data[idx].fd.type);
      return (0);
    }
  count = tdp->fields.size / sizeof (tdp->fields.data[0]);
  for (i = 0; i < count; ++i)
    if (!xdr_load (data + tdp->fields.data[i].offset, &tdp->fields.data[i], xdrs, ptrs))
      return (0);
  
  return (!0);
}

static int
xdr_save_union (XDR * xdrs, int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  return (xdr_int (xdrs, &ptrs->ra.data[idx].union_field_idx));
}

static int
xdr_load_union (XDR * xdrs, int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  rl_td_t * tdp = rl_get_td_by_name (ptrs->ra.data[idx].fd.type); /* look up for type descriptor */
  char * data = ptrs->ra.data[idx].data;
  int field_idx = -1;

  /* get pointer on structure descriptor */
  if (NULL == tdp)
    {
      RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_NO_TYPE_DESCRIPTOR, ptrs->ra.data[idx].fd.type);
      return (0);
    }
  if ((tdp->rl_type != RL_TYPE_UNION) && (tdp->rl_type != RL_TYPE_ANON_UNION))
    {
      RL_MESSAGE (RL_LL_WARN, RL_MESSAGE_NOT_A_UNION, tdp->type);
      return (0);
    }    
  if (!xdr_int (xdrs, &field_idx))
    return (0);
  if ((field_idx >= 0) && (field_idx < tdp->fields.size / sizeof (tdp->fields.data[0])))
    return (xdr_load (data + tdp->fields.data[field_idx].offset, &tdp->fields.data[field_idx], xdrs, ptrs));
  
  return (!0);
}

static int
xdr_load_array (XDR * xdrs, int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  int i;
  char * data = ptrs->ra.data[idx].data;
  rl_fd_t fd_ = ptrs->ra.data[idx].fd;
  int count = fd_.param.array_param.count;
  int row_count = fd_.param.array_param.row_count;
  
  if (1 == fd_.param.array_param.row_count)
    fd_.rl_type_ext = RL_TYPE_EXT_NONE; /* set extended type property to RL_NONE in copy of field descriptor */
  else
    {
      fd_.param.array_param.count = row_count;
      fd_.param.array_param.row_count = 1;
    }

  for (i = 0; i < count; i += row_count)
    if (!xdr_load (&data[i * fd_.size], &fd_, xdrs, ptrs))
      return (0);
  return (!0);
}

static int
xdr_save_rarray (XDR * xdrs, int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  rl_rarray_t * ra = ptrs->ra.data[idx].data;
  if (!xdr_int (xdrs, &ra->size))
    return (0);
  
  if (ptrs->ra.data[idx].ref_idx >= 0)
    return (xdr_int (xdrs, &ptrs->ra.data[ptrs->ra.data[idx].ref_idx].idx));
  else
    return (xdr_int (xdrs, &ptrs->ra.data[idx].ref_idx));
}

static int
xdr_load_rarray (XDR * xdrs, int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  int i;
  rl_rarray_t * ra = ptrs->ra.data[idx].data;
  rl_fd_t fd_ = ptrs->ra.data[idx].fd;

  /* prepare copy of filed descriptor for array elements loading */
  fd_.rl_type_ext = RL_TYPE_EXT_NONE;
  ra->data = NULL;
  
  if (!xdr_int (xdrs, &ra->size))
    return (0);
  if (!xdr_int (xdrs, &ptrs->ra.data[idx].ref_idx))
    return (0);
  if (ptrs->ra.data[idx].ref_idx >= 0)
    ra->alloc_size = -1;
  else
    {
      ra->alloc_size = ra->size;
      if (ra->size > 0)
	{
	  ra->data = RL_MALLOC (ra->size);
	  if (NULL == ra->data)
	    {
	      RL_MESSAGE (RL_LL_FATAL, RL_MESSAGE_OUT_OF_MEMORY);
	      return (0);
	    }
	  for (i = 0; i < ra->size / fd_.size; ++i)
	    if (!xdr_load (&(((char*)ra->data)[i * fd_.size]), &fd_, xdrs, ptrs))
	      return (0);
	}
    }
  return (!0);
}

/**
 * Saves/loads pointer into/from binary XDR stream. First goes flag that pointer is not NULL,
 * and if pointer is not NULL, strucure goes after flag.
 * @param xdrs XDR stream descriptor
 * @param idx index of the node in nodes collection
 * @param ptrs collection of nodes
 * @return status of operation. 0 - failure, !0 - success.
 */
static int
xdr_save_pointer (XDR * xdrs, int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  int32_t not_null = (*(void**)ptrs->ra.data[idx].data != NULL);
  if (!xdr_int32_t (xdrs, &not_null))
    return (0);
  if (!not_null)
    return (!0);
  if (ptrs->ra.data[idx].ref_idx >= 0)
    return (xdr_int (xdrs, &ptrs->ra.data[ptrs->ra.data[idx].ref_idx].idx));
  else
    return (xdr_int (xdrs, &ptrs->ra.data[idx].ref_idx));
}

static int
xdr_load_pointer (XDR * xdrs, int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  void ** data = ptrs->ra.data[idx].data;
  rl_fd_t fd_ = ptrs->ra.data[idx].fd;
  int32_t not_null = 0;

  if (!xdr_int32_t (xdrs, &not_null))
    return (0);
  if (not_null)
    {
      if (!xdr_int (xdrs, &ptrs->ra.data[idx].ref_idx))
	return (0);
      if (ptrs->ra.data[idx].ref_idx >= 0)
	return (!0);
      *data = RL_MALLOC (fd_.size);
      if (NULL == *data)
	{
	  RL_MESSAGE (RL_LL_FATAL, RL_MESSAGE_OUT_OF_MEMORY);
	  return (0);
	}
      memset (*data, 0, fd_.size);
      fd_.rl_type_ext = RL_TYPE_EXT_NONE;
      return (xdr_load (*data, &fd_, xdrs, ptrs));
    }
  *data = NULL;
  return (!0);
}

/**
 * Public function. Save scheduler. Save any object as XML node.
 * @param data a pointer on data
 * @param fdp a ponter of field descriptor
 * @param ptrs resizeable array with pointers descriptors 
 * @return A pointer on new XML node
 */
int
xdr_save (XDR * xdrs, rl_ra_rl_ptrdes_t * ptrs)
{
  int idx = 0;
  while (idx >= 0)
    {
      rl_fd_t * fdp = &ptrs->ra.data[idx].fd;
      if ((fdp->rl_type_ext >= 0) && (fdp->rl_type_ext < RL_MAX_TYPES)
	  && rl_conf.io_ext_handlers[fdp->rl_type_ext].save.xdr)
	{
	  if (!rl_conf.io_ext_handlers[fdp->rl_type_ext].save.xdr (xdrs, idx, ptrs))
	    return (0);
	}
      else if ((fdp->rl_type >= 0) && (fdp->rl_type < RL_MAX_TYPES)
	       && rl_conf.io_handlers[fdp->rl_type].save.xdr)
	{
	  if (!rl_conf.io_handlers[fdp->rl_type].save.xdr (xdrs, idx, ptrs))
	    return (0);
	}
      else
	{
	  RL_MESSAGE_UNSUPPORTED_NODE_TYPE_ (fdp);    	  
	  return (0);
	}
      
      if (ptrs->ra.data[idx].first_child >= 0)
	idx = ptrs->ra.data[idx].first_child;
      else
	{
	  while ((ptrs->ra.data[idx].next < 0) && (ptrs->ra.data[idx].parent >= 0))
	    idx = ptrs->ra.data[idx].parent;
	  idx = ptrs->ra.data[idx].next;
	}
    }
  return (!0);
}

int
xdr_load (void * data, rl_fd_t * fdp, XDR * xdrs, rl_ra_rl_ptrdes_t * ptrs)
{
  rl_ra_rl_ptrdes_t ptrs_ = { .ra = { .size = 0, .alloc_size = 0, .data = NULL } };
  int status = !0;
  int idx;

  if (NULL == ptrs)
    ptrs = &ptrs_;
  
  idx = rl_add_ptr_to_list (ptrs);
  if (idx < 0)
    return (idx);
  ptrs->ra.data[idx].data = data;
  ptrs->ra.data[idx].fd = *fdp;

  if ((fdp->rl_type_ext >= 0) && (fdp->rl_type_ext < RL_MAX_TYPES)
      && rl_conf.io_ext_handlers[fdp->rl_type_ext].load.xdr)
    status = rl_conf.io_ext_handlers[fdp->rl_type_ext].load.xdr (xdrs, idx, ptrs);
  else if ((fdp->rl_type >= 0) && (fdp->rl_type < RL_MAX_TYPES)
	   && rl_conf.io_handlers[fdp->rl_type].load.xdr)
    status = rl_conf.io_handlers[fdp->rl_type].load.xdr (xdrs, idx, ptrs);
  else
    RL_MESSAGE_UNSUPPORTED_NODE_TYPE_ (fdp);    	  

  if (status && (0 == idx))
    status = rl_set_crossrefs (ptrs);

  if (ptrs_.ra.data)
    RL_FREE (ptrs_.ra.data);
  return (status);
}

/**
 * Init save handlers Table
 */
static void __attribute__((constructor)) rl_init_save_xdr (void)
{
  rl_conf.io_handlers[RL_TYPE_NONE].save.xdr = xdr_none; 
  rl_conf.io_handlers[RL_TYPE_VOID].save.xdr = xdr_none; 
  rl_conf.io_handlers[RL_TYPE_ENUM].save.xdr = xdr_uint_;
  rl_conf.io_handlers[RL_TYPE_BITFIELD].save.xdr = xdr_save_bitfield;
  rl_conf.io_handlers[RL_TYPE_BITMASK].save.xdr = xdr_uint_;
  rl_conf.io_handlers[RL_TYPE_INT8].save.xdr = xdr_int_;
  rl_conf.io_handlers[RL_TYPE_UINT8].save.xdr = xdr_uint_;
  rl_conf.io_handlers[RL_TYPE_INT16].save.xdr = xdr_int_;
  rl_conf.io_handlers[RL_TYPE_UINT16].save.xdr = xdr_uint_;
  rl_conf.io_handlers[RL_TYPE_INT32].save.xdr = xdr_int_;
  rl_conf.io_handlers[RL_TYPE_UINT32].save.xdr = xdr_uint_;
  rl_conf.io_handlers[RL_TYPE_INT64].save.xdr = xdr_int_;
  rl_conf.io_handlers[RL_TYPE_UINT64].save.xdr = xdr_uint_;
  rl_conf.io_handlers[RL_TYPE_FLOAT].save.xdr = xdr_float_; 
  rl_conf.io_handlers[RL_TYPE_DOUBLE].save.xdr = xdr_double_; 
  rl_conf.io_handlers[RL_TYPE_LONG_DOUBLE].save.xdr = xdr_long_double;
  rl_conf.io_handlers[RL_TYPE_CHAR].save.xdr = xdr_int_; 
  rl_conf.io_handlers[RL_TYPE_CHAR_ARRAY].save.xdr = xdr_char_array_; 
  rl_conf.io_handlers[RL_TYPE_STRING].save.xdr = xdr_save_string;
  rl_conf.io_handlers[RL_TYPE_STRUCT].save.xdr = xdr_none; 
  rl_conf.io_handlers[RL_TYPE_FUNC].save.xdr = xdr_none; 
  rl_conf.io_handlers[RL_TYPE_FUNC_TYPE].save.xdr = xdr_none; 
  rl_conf.io_handlers[RL_TYPE_UNION].save.xdr = xdr_save_union; 
  rl_conf.io_handlers[RL_TYPE_ANON_UNION].save.xdr = xdr_save_union; 
     
  rl_conf.io_ext_handlers[RL_TYPE_EXT_ARRAY].save.xdr = xdr_none;
  rl_conf.io_ext_handlers[RL_TYPE_EXT_RARRAY].save.xdr = xdr_save_rarray; 
  rl_conf.io_ext_handlers[RL_TYPE_EXT_POINTER].save.xdr = xdr_save_pointer; 
}

/**
 * Init load handlers Table
 */
static void __attribute__((constructor)) rl_init_load_xdr (void)
{
  rl_conf.io_handlers[RL_TYPE_NONE].load.xdr = xdr_none; 
  rl_conf.io_handlers[RL_TYPE_VOID].load.xdr = xdr_none; 
  rl_conf.io_handlers[RL_TYPE_ENUM].load.xdr = xdr_uint_;
  rl_conf.io_handlers[RL_TYPE_BITFIELD].load.xdr = xdr_load_bitfield;
  rl_conf.io_handlers[RL_TYPE_BITMASK].load.xdr = xdr_uint_;
  rl_conf.io_handlers[RL_TYPE_INT8].load.xdr = xdr_int_;
  rl_conf.io_handlers[RL_TYPE_UINT8].load.xdr = xdr_uint_;
  rl_conf.io_handlers[RL_TYPE_INT16].load.xdr = xdr_int_;
  rl_conf.io_handlers[RL_TYPE_UINT16].load.xdr = xdr_uint_;
  rl_conf.io_handlers[RL_TYPE_INT32].load.xdr = xdr_int_;
  rl_conf.io_handlers[RL_TYPE_UINT32].load.xdr = xdr_uint_;
  rl_conf.io_handlers[RL_TYPE_INT64].load.xdr = xdr_int_;
  rl_conf.io_handlers[RL_TYPE_UINT64].load.xdr = xdr_uint_;
  rl_conf.io_handlers[RL_TYPE_FLOAT].load.xdr = xdr_float_; 
  rl_conf.io_handlers[RL_TYPE_DOUBLE].load.xdr = xdr_double_; 
  rl_conf.io_handlers[RL_TYPE_LONG_DOUBLE].load.xdr = xdr_long_double; 
  rl_conf.io_handlers[RL_TYPE_CHAR].load.xdr = xdr_int_; 
  rl_conf.io_handlers[RL_TYPE_CHAR_ARRAY].load.xdr = xdr_char_array_; 
  rl_conf.io_handlers[RL_TYPE_STRING].load.xdr = xdr_load_string;
  rl_conf.io_handlers[RL_TYPE_STRUCT].load.xdr = xdr_load_struct; 
  rl_conf.io_handlers[RL_TYPE_FUNC].load.xdr = xdr_none; 
  rl_conf.io_handlers[RL_TYPE_FUNC_TYPE].load.xdr = xdr_none; 
  rl_conf.io_handlers[RL_TYPE_UNION].load.xdr = xdr_load_union; 
  rl_conf.io_handlers[RL_TYPE_ANON_UNION].load.xdr = xdr_load_union; 
     
  rl_conf.io_ext_handlers[RL_TYPE_EXT_ARRAY].load.xdr = xdr_load_array;
  rl_conf.io_ext_handlers[RL_TYPE_EXT_RARRAY].load.xdr = xdr_load_rarray; 
  rl_conf.io_ext_handlers[RL_TYPE_EXT_POINTER].load.xdr = xdr_load_pointer; 
}
