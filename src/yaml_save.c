#include <yaml.h>
#include <metaresc.h>
#include <mr_stringify.h>
#include <mr_save.h>

TYPEDEF_STRUCT (mr_yaml_context_t,
		VOID (yaml_emitter_t, emitter),
		(mr_rarray_t, buffer));

TYPEDEF_FUNC (mr_status_t, mr_yaml_save_handler_t, (mr_yaml_context_t * /* context */, mr_ra_ptrdes_t * /* ptrs */, int /* idx */));

static int
yaml_ra_string_writer (void * data, unsigned char * buffer, size_t size)
{
  mr_rarray_t * mr_ra_str = data;
  char * tail = mr_rarray_append (mr_ra_str, size);

  if (NULL != tail)
    {
      memcpy (tail, buffer, size);
      return (!0);
    }
  
  if (mr_ra_str->data.ptr)
    MR_FREE (mr_ra_str->data.ptr);
  mr_ra_str->data.ptr = NULL;
  mr_ra_str->MR_SIZE = mr_ra_str->alloc_size = 0;
  return (0);
}

#define ANCHOR_SIZE (sizeof (MR_YAML_ANCHOR_TMPLT) + (sizeof (((mr_ptrdes_t*)0)->idx) * 12 + 4) / 5)

static inline yaml_char_t *
yaml_get_anchor (char * anchor, mr_ptrdes_t * ptrdes)
{
  if (!ptrdes->flags.is_referenced)
    return (NULL);
  
  sprintf (anchor, MR_YAML_ANCHOR_TMPLT, ptrdes->idx);
  return ((yaml_char_t*)anchor);
}

static inline mr_status_t
yaml_emit_field (yaml_emitter_t * emitter, yaml_event_t * event, mr_ra_ptrdes_t * ptrs, int idx)
{
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  
  if (!ptrdes->unnamed)
    {
      yaml_event_t name_event;
      if (!yaml_scalar_event_initialize (&name_event, NULL, (yaml_char_t *)YAML_STR_TAG,
					 (yaml_char_t *)ptrdes->name, strlen (ptrdes->name),
					 1, 0, YAML_PLAIN_SCALAR_STYLE))
	{
	  yaml_event_delete (event);
	  return (MR_FAILURE);
	}
  
      if (!yaml_emitter_emit (emitter, &name_event))
	{
	  yaml_event_delete (event);
	  return (MR_FAILURE);
	}
    }

  if ((0 <= ptrdes->ref_idx) && (ptrdes->ref_idx < ptrs->size / sizeof (ptrs->ra[0])))
    {
      char anchor[MR_MAX (sizeof (MR_YAML_REF_ANCHOR_TMPLT), sizeof (MR_YAML_REF_ANCHOR_CONTENT_TMPLT)) + (sizeof (ptrdes->idx) * 12 + 4) / 5];
      if (ptrdes->flags.is_content_reference)
	sprintf (anchor, MR_YAML_REF_ANCHOR_CONTENT_TMPLT, ptrs->ra[ptrdes->ref_idx].idx);
      else
	sprintf (anchor, MR_YAML_REF_ANCHOR_TMPLT, ptrs->ra[ptrdes->ref_idx].idx);

      yaml_event_delete (event);
      if (!yaml_alias_event_initialize (event, (yaml_char_t *)anchor))
	return (MR_FAILURE);
    }
  else if (ptrdes->flags.is_null)
    {
      yaml_event_delete (event);
      if (!yaml_alias_event_initialize (event, (yaml_char_t *)MR_ISNULL))
	return (MR_FAILURE);
    }
  
  if (!yaml_emitter_emit (emitter, event))
    return (MR_FAILURE);

  return (MR_SUCCESS);
}

static inline mr_status_t
yaml_emit_string (yaml_emitter_t * emitter, const char * value, mr_ra_ptrdes_t * ptrs, int idx)
{
  yaml_event_t event;
  char anchor[ANCHOR_SIZE];

  if (NULL == value)
    value = "";
  
  if (!yaml_scalar_event_initialize (&event, yaml_get_anchor (anchor, &ptrs->ra[idx]),
				     (yaml_char_t *)YAML_STR_TAG,
				     (yaml_char_t *)value, strlen (value),
				     1, 0, YAML_PLAIN_SCALAR_STYLE))
    return (MR_FAILURE);

  return (yaml_emit_field (emitter, &event, ptrs, idx));
}

static mr_status_t
yaml_save_string (mr_yaml_context_t * mr_yaml_context, mr_ra_ptrdes_t * ptrs, int idx)
{
  char * value = *(char **)ptrs->ra[idx].data.ptr;
  return (yaml_emit_string (&mr_yaml_context->emitter, value, ptrs, idx));
}

static mr_status_t
yaml_save_char (mr_yaml_context_t * mr_yaml_context, mr_ra_ptrdes_t * ptrs, int idx)
{
  char buffer[] = "0x00";
  unsigned char c = *(unsigned char*)ptrs->ra[idx].data.ptr;
  if ((0x20 <= c) && (c < 0x80))
    {
      buffer[0] = c;
      buffer[1] = 0;
    }
  else
    sprintf (buffer, "0x%02x", c);
    
  return (yaml_emit_string (&mr_yaml_context->emitter, buffer, ptrs, idx));
}

static mr_status_t
yaml_save_char_array (mr_yaml_context_t * mr_yaml_context, mr_ra_ptrdes_t * ptrs, int idx)
{
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  typeof (ptrdes->fdp->stype.size) size = ptrdes->fdp ? ptrdes->fdp->stype.size : 0;
  char buffer[size + 1];
  strncpy (buffer, ptrdes->data.ptr, size);
  buffer[size] = 0;
  return (yaml_emit_string (&mr_yaml_context->emitter, buffer, ptrs, idx));
}

static mr_status_t
yaml_save_void (mr_yaml_context_t * mr_yaml_context, mr_ra_ptrdes_t * ptrs, int idx)
{
  return (MR_SUCCESS);
}

static mr_status_t
yaml_save_bool (mr_yaml_context_t * mr_yaml_context, mr_ra_ptrdes_t * ptrs, int idx)
{
  char * value = *(bool*)ptrs->ra[idx].data.ptr ? "true" : "false";
  return (yaml_emit_string (&mr_yaml_context->emitter, value, ptrs, idx));
}

static mr_status_t
yaml_save_func (mr_yaml_context_t * mr_yaml_context, mr_ra_ptrdes_t * ptrs, int idx)
{
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  const char * func_str = mr_serialize_func (*(void**)ptrdes->data.ptr);
  if (func_str)
    return (yaml_emit_string (&mr_yaml_context->emitter, func_str, ptrs, idx));
  #define YAML_PTR_TMPLT "0x%llx"
  char buffer[sizeof (YAML_PTR_TMPLT) + sizeof (void*) * __CHAR_BIT__ / 4];
  sprintf (buffer, YAML_PTR_TMPLT, (unsigned long long int)*(uintptr_t*)ptrdes->data.ptr);
  return (yaml_emit_string (&mr_yaml_context->emitter, buffer, ptrs, idx));
}

static mr_status_t
yaml_pre_save_struct (mr_yaml_context_t * mr_yaml_context, mr_ra_ptrdes_t * ptrs, int idx)
{
  yaml_event_t event;
  char anchor[ANCHOR_SIZE];
  
  if (!yaml_mapping_start_event_initialize (&event, yaml_get_anchor (anchor, &ptrs->ra[idx]),
					    (yaml_char_t *)YAML_MAP_TAG,
					    1, YAML_ANY_MAPPING_STYLE))
    return (MR_FAILURE);
  
  return (yaml_emit_field (&mr_yaml_context->emitter, &event, ptrs, idx));
}

static mr_status_t
yaml_post_save_struct (mr_yaml_context_t * mr_yaml_context, mr_ra_ptrdes_t * ptrs, int idx)
{
  yaml_event_t event;

  if (!yaml_mapping_end_event_initialize (&event))
    return (MR_FAILURE);
  
  if (!yaml_emitter_emit (&mr_yaml_context->emitter, &event))
    return (MR_FAILURE);

  return (MR_SUCCESS);
}

static mr_status_t
yaml_pre_save_array (mr_yaml_context_t * mr_yaml_context, mr_ra_ptrdes_t * ptrs, int idx)
{
  yaml_event_t event;
  char anchor[ANCHOR_SIZE];
  yaml_sequence_style_t sequence_style = YAML_ANY_SEQUENCE_STYLE;
  int first_child = ptrs->ra[idx].first_child;
  
#define NON_SCALAR_TYPE (0 MR_FOREACH (MR_ONE_SHIFT, MR_TYPE_STRUCT, MR_TYPE_UNION, MR_TYPE_ANON_UNION, MR_TYPE_NAMED_ANON_UNION, MR_TYPE_POINTER, MR_TYPE_ARRAY))
  if ((first_child >= 0) && !((NON_SCALAR_TYPE >> ptrs->ra[first_child].mr_type) & 1))
    sequence_style = YAML_FLOW_SEQUENCE_STYLE;

  if (!yaml_sequence_start_event_initialize (&event, yaml_get_anchor (anchor, &ptrs->ra[idx]),
					     (yaml_char_t *)YAML_SEQ_TAG,
					     1, sequence_style))
    return (MR_FAILURE);
  
  return (yaml_emit_field (&mr_yaml_context->emitter, &event, ptrs, idx));
}

static mr_status_t
yaml_post_save_array (mr_yaml_context_t * mr_yaml_context, mr_ra_ptrdes_t * ptrs, int idx)
{
  yaml_event_t event;

  if (!yaml_sequence_end_event_initialize (&event))
    return (MR_FAILURE);
  
  if (!yaml_emitter_emit (&mr_yaml_context->emitter, &event))
    return (MR_FAILURE);

  return (MR_SUCCESS);
}

static mr_status_t
yaml_pre_save_pointer (mr_yaml_context_t * mr_yaml_context, mr_ra_ptrdes_t * ptrs, int idx)
{
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  
  if (ptrdes->fdp)
    if (ptrdes->fdp->meta)
      if (0 == strcmp (ptrdes->fdp->meta, MR_PTR_META))
	ptrdes->unnamed = false;
  
  if (ptrdes->flags.is_null || (ptrdes->ref_idx >= 0))
    return (yaml_emit_string (&mr_yaml_context->emitter, "", ptrs, idx));

  return (yaml_pre_save_array (mr_yaml_context, ptrs, idx));
}

static mr_status_t
yaml_post_save_pointer (mr_yaml_context_t * mr_yaml_context, mr_ra_ptrdes_t * ptrs, int idx)
{
  mr_ptrdes_t * ptrdes = &ptrs->ra[idx];
  if (ptrdes->flags.is_null || (ptrdes->ref_idx >= 0))
    return (MR_SUCCESS);
  
  return (yaml_post_save_array (mr_yaml_context, ptrs, idx));
}

static mr_status_t
yaml_pre_save_anon_union (mr_yaml_context_t * mr_yaml_context, mr_ra_ptrdes_t * ptrs, int idx)
{
  ptrs->ra[idx].unnamed = false;
  return (yaml_pre_save_struct (mr_yaml_context, ptrs, idx));
}

static inline mr_status_t
yaml_allocate_ra_str (mr_rarray_t * mr_ra_str)
{
  if (NULL == mr_ra_str->data.ptr)
    mr_ra_str->data.ptr = MR_CALLOC (1, mr_ra_str->alloc_size);

  if (NULL == mr_ra_str->data.ptr)
    return (MR_FAILURE);
  
  mr_ra_str->data.string[0] = 0;
  mr_ra_str->MR_SIZE = 1;

  return (MR_SUCCESS);
}

#define YAML_SAVE_HANDLER(TYPE, ...)					\
  static mr_status_t							\
  yaml_save_ ## TYPE (mr_yaml_context_t * mr_yaml_context, mr_ra_ptrdes_t * ptrs, int idx) \
  {									\
    if (MR_SUCCESS != yaml_allocate_ra_str (&mr_yaml_context->buffer))	\
      return (MR_FAILURE);						\
    int count = mr_ra_printf_ ## TYPE					\
      (&mr_yaml_context->buffer, &ptrs->ra[idx]				\
       MR_IF_ELSE (MR_IS_EMPTY (__VA_ARGS__)) () ( , __VA_ARGS__));	\
    if (count <= 0)							\
      return (MR_FAILURE);						\
    return (yaml_emit_string (&mr_yaml_context->emitter,		\
			      mr_yaml_context->buffer.data.string,	\
			      ptrs, idx));				\
  }

MR_FOREACH (YAML_SAVE_HANDLER, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, int128_t, uint128_t, float, double, long_double_t);

#define COMPLEX_REAL_IMAG_DELIMITER " + "

YAML_SAVE_HANDLER (complex_float, COMPLEX_REAL_IMAG_DELIMITER);
YAML_SAVE_HANDLER (complex_double, COMPLEX_REAL_IMAG_DELIMITER);
YAML_SAVE_HANDLER (complex_long_double_t, COMPLEX_REAL_IMAG_DELIMITER);

#define MR_YAML_BITMASK_DELIMITER " | "

YAML_SAVE_HANDLER (bitmask, MR_YAML_BITMASK_DELIMITER);
YAML_SAVE_HANDLER (bitfield, MR_YAML_BITMASK_DELIMITER);

static mr_yaml_save_handler_t yaml_pre_save_tbl[MR_TYPE_LAST] =
  {
    [MR_TYPE_STRING] = yaml_save_string,
    [MR_TYPE_NONE] = yaml_save_void,
    [MR_TYPE_VOID] = yaml_save_void,
    [MR_TYPE_ENUM] = yaml_save_bitmask,
    [MR_TYPE_BITFIELD] = yaml_save_bitfield,
    [MR_TYPE_BOOL] = yaml_save_bool,
    [MR_TYPE_INT8] = yaml_save_int8_t,
    [MR_TYPE_UINT8] = yaml_save_uint8_t,
    [MR_TYPE_INT16] = yaml_save_int16_t,
    [MR_TYPE_UINT16] = yaml_save_uint16_t,
    [MR_TYPE_INT32] = yaml_save_int32_t,
    [MR_TYPE_UINT32] = yaml_save_uint32_t,
    [MR_TYPE_INT64] = yaml_save_int64_t,
    [MR_TYPE_UINT64] = yaml_save_uint64_t,
    [MR_TYPE_INT128] = yaml_save_int128_t,
    [MR_TYPE_UINT128] = yaml_save_uint128_t,
    [MR_TYPE_FLOAT] = yaml_save_float,
    [MR_TYPE_COMPLEX_FLOAT] = yaml_save_complex_float,
    [MR_TYPE_DOUBLE] = yaml_save_double,
    [MR_TYPE_COMPLEX_DOUBLE] = yaml_save_complex_double,
    [MR_TYPE_LONG_DOUBLE] = yaml_save_long_double_t,
    [MR_TYPE_COMPLEX_LONG_DOUBLE] = yaml_save_complex_long_double_t,
    [MR_TYPE_CHAR] = yaml_save_char,
    [MR_TYPE_CHAR_ARRAY] = yaml_save_char_array,
    [MR_TYPE_STRUCT] = yaml_pre_save_struct,
    [MR_TYPE_FUNC] = yaml_save_func,
    [MR_TYPE_FUNC_TYPE] = yaml_save_func,
    [MR_TYPE_ARRAY] = yaml_pre_save_array,
    [MR_TYPE_POINTER] = yaml_pre_save_pointer,
    [MR_TYPE_UNION] = yaml_pre_save_struct,
    [MR_TYPE_ANON_UNION] = yaml_pre_save_anon_union,
    [MR_TYPE_NAMED_ANON_UNION] = yaml_pre_save_struct,
  };

static mr_yaml_save_handler_t yaml_post_save_tbl[MR_TYPE_LAST] =
  {
    [MR_TYPE_STRING] = yaml_save_void,
    [MR_TYPE_NONE] = yaml_save_void,
    [MR_TYPE_VOID] = yaml_save_void,
    [MR_TYPE_ENUM] = yaml_save_void,
    [MR_TYPE_BITFIELD] = yaml_save_void,
    [MR_TYPE_BOOL] = yaml_save_void,
    [MR_TYPE_INT8] = yaml_save_void,
    [MR_TYPE_UINT8] = yaml_save_void,
    [MR_TYPE_INT16] = yaml_save_void,
    [MR_TYPE_UINT16] = yaml_save_void,
    [MR_TYPE_INT32] = yaml_save_void,
    [MR_TYPE_UINT32] = yaml_save_void,
    [MR_TYPE_INT64] = yaml_save_void,
    [MR_TYPE_UINT64] = yaml_save_void,
    [MR_TYPE_INT128] = yaml_save_void,
    [MR_TYPE_UINT128] = yaml_save_void,
    [MR_TYPE_FLOAT] = yaml_save_void,
    [MR_TYPE_COMPLEX_FLOAT] = yaml_save_void,
    [MR_TYPE_DOUBLE] = yaml_save_void,
    [MR_TYPE_COMPLEX_DOUBLE] = yaml_save_void,
    [MR_TYPE_LONG_DOUBLE] = yaml_save_void,
    [MR_TYPE_COMPLEX_LONG_DOUBLE] = yaml_save_void,
    [MR_TYPE_CHAR] = yaml_save_void,
    [MR_TYPE_CHAR_ARRAY] = yaml_save_void,
    [MR_TYPE_STRUCT] = yaml_post_save_struct,
    [MR_TYPE_FUNC] = yaml_save_void,
    [MR_TYPE_FUNC_TYPE] = yaml_save_void,
    [MR_TYPE_ARRAY] = yaml_post_save_array,
    [MR_TYPE_POINTER] = yaml_post_save_pointer,
    [MR_TYPE_UNION] = yaml_post_save_struct,
    [MR_TYPE_ANON_UNION] = yaml_post_save_struct,
    [MR_TYPE_NAMED_ANON_UNION] = yaml_post_save_struct,
  };

static mr_status_t
yaml_print_node (mr_ra_ptrdes_t * ptrs, int idx, int level, mr_dfs_order_t order, void * context)
{
  mr_yaml_context_t * mr_yaml_context = context;
  mr_yaml_save_handler_t save_handler = NULL;
  mr_type_t mr_type = ptrs->ra[idx].mr_type;
  
  if ((MR_TYPE_NONE <= mr_type) && (mr_type < MR_TYPE_LAST))
    switch (order)
      {
      case MR_DFS_PRE_ORDER:
	save_handler = yaml_pre_save_tbl[mr_type];
	break;
      case MR_DFS_POST_ORDER:
	save_handler = yaml_post_save_tbl[mr_type];
	break;
      default:
	break;
      }
  
  if (NULL == save_handler)
    {
      MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_UNSUPPORTED_NODE_TYPE, mr_type);
      return (MR_FAILURE);
    }

  return (save_handler (mr_yaml_context, ptrs, idx));
}

/**
 * Public function. Save scheduler. Save any object as a string.
 * @param ptrs resizeable array with pointers descriptors
 * @return stringified representation of object
 */
char *
mr_yaml_save (mr_ra_ptrdes_t * ptrs)
{
  yaml_event_t event;
  mr_rarray_t mr_ra_str = {
    .data = { NULL },
    .MR_SIZE = 0,
    .type = "string",
    .alloc_size = 0,
  };
  mr_yaml_context_t mr_yaml_context = {
    .buffer = {
      .data = { NULL },
      .MR_SIZE = 0,
      .type = "string",
      .alloc_size = 64 - sizeof (void*),
    },
  };

  mr_status_t status = yaml_emitter_initialize (&mr_yaml_context.emitter) ? MR_SUCCESS : MR_FAILURE;
  if (MR_SUCCESS != status)
    goto cleanup;

  yaml_emitter_set_output (&mr_yaml_context.emitter, yaml_ra_string_writer, &mr_ra_str);

  status = yaml_stream_start_event_initialize (&event, YAML_ANY_ENCODING) ? MR_SUCCESS : MR_FAILURE;
  if (MR_SUCCESS != status)
    goto cleanup;
  
  status = yaml_emitter_emit (&mr_yaml_context.emitter, &event) ? MR_SUCCESS : MR_FAILURE;
  if (MR_SUCCESS != status)
    goto cleanup;

  status = yaml_document_start_event_initialize (&event, NULL, NULL, NULL, 0) ? MR_SUCCESS : MR_FAILURE;
  if (MR_SUCCESS != status)
    goto cleanup;

  status = yaml_emitter_emit (&mr_yaml_context.emitter, &event) ? MR_SUCCESS : MR_FAILURE;
  if (MR_SUCCESS != status)
    goto cleanup;
  
  status = mr_ptrs_dfs (ptrs, yaml_print_node, &mr_yaml_context);
  if (MR_SUCCESS != status)
    goto cleanup;

  status = yaml_document_end_event_initialize (&event, 0) ? MR_SUCCESS : MR_FAILURE;
  if (MR_SUCCESS != status)
    goto cleanup;
  
  status = yaml_emitter_emit (&mr_yaml_context.emitter, &event) ? MR_SUCCESS : MR_FAILURE;
  if (MR_SUCCESS != status)
    goto cleanup;

  status = yaml_stream_end_event_initialize (&event) ? MR_SUCCESS : MR_FAILURE;
  if (MR_SUCCESS != status)
    goto cleanup;

  status = yaml_emitter_emit (&mr_yaml_context.emitter, &event) ? MR_SUCCESS : MR_FAILURE;

 cleanup:

  if (mr_yaml_context.emitter.error != YAML_NO_ERROR)
    MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_YAML_ERROR, mr_yaml_context.emitter.problem);
  
  yaml_emitter_delete (&mr_yaml_context.emitter);

  if (mr_yaml_context.buffer.data.ptr)
    MR_FREE (mr_yaml_context.buffer.data.ptr);

  if (MR_SUCCESS != status)
    {
      if (mr_ra_str.data.ptr)
	MR_FREE (mr_ra_str.data.ptr);
      return (NULL);
    }

  if (mr_ra_str.data.string)
    if (mr_ra_append_char (&mr_ra_str, mr_ra_str.data.string[mr_ra_str.mr_size - 1]) != sizeof (char))
      return (NULL);
  
  return (mr_ra_str.data.string);
}
