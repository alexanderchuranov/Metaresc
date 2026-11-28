#include <yaml.h>
#include <metaresc.h>
#include <mr_stringify.h>
#include <mr_save.h>

TYPEDEF_STRUCT (mr_yaml_context_t,
                VOID (yaml_emitter_t, emitter),
                (mr_rarray_t, buffer)
                );

TYPEDEF_FUNC (mr_status_t, mr_yaml_save_handler_t, (mr_yaml_context_t * /* context */, mr_ptrdes_t * /* ptrs */, mr_idx_t /* idx */));

/**
 * Output handler of the YAML emitter that appends to a resizable string.
 * On error the string is freed.
 * @param data output string
 * @param buffer emitted text
 * @param size size of the text
 * @return non-zero on success, 0 on error
 */
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

/**
 * Formats the anchor of a referenced node.
 * @param anchor buffer of ANCHOR_SIZE bytes
 * @param ptrdes node descriptor
 * @return anchor or NULL if the node is not referenced
 */
static inline yaml_char_t *
yaml_get_anchor (char * anchor, mr_ptrdes_t * ptrdes)
{
  if (!(ptrdes->flags & MR_IS_REFERENCED))
    return (NULL);
  
  snprintf (anchor, ANCHOR_SIZE, MR_YAML_ANCHOR_TMPLT, (uint32_t)ptrdes->idx);
  return ((yaml_char_t*)anchor);
}

/**
 * Emits the key of a named node and the event of its value. A reference is
 * emitted as an alias to the anchor of its target, a NULL pointer as an
 * alias to the NULL marker. The event is deleted on error.
 * @param emitter YAML emitter
 * @param event event of the value
 * @param ptrs array with node descriptors
 * @param idx index of the node
 * @return status
 */
static inline mr_status_t
yaml_emit_field (yaml_emitter_t * emitter, yaml_event_t * event, mr_ptrdes_t * ptrs, mr_idx_t idx)
{
  mr_ptrdes_t * ptrdes = &ptrs[idx];
  
  if (!(ptrdes->flags & MR_IS_UNNAMED))
    {
      yaml_event_t name_event;
      char * name = ptrdes->fdp ? ptrdes->fdp->name.str : MR_DEFAULT_NODE_NAME;
      if (!yaml_scalar_event_initialize (&name_event, NULL, (yaml_char_t *)YAML_STR_TAG,
                                         (yaml_char_t*)name, strlen (name),
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

  if (ptrdes->flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE))
    {
#define MR_ANCHOR_SIZE (MR_MAX (sizeof (MR_YAML_REF_ANCHOR_TMPLT), sizeof (MR_YAML_REF_ANCHOR_CONTENT_TMPLT)) + (sizeof (ptrdes->idx) * 12 + 4) / 5)
      char anchor[MR_ANCHOR_SIZE];
      if (ptrdes->flags & MR_IS_CONTENT_REFERENCE)
        snprintf (anchor, MR_ANCHOR_SIZE, MR_YAML_REF_ANCHOR_CONTENT_TMPLT, (uint32_t)ptrs[ptrdes->first_child].idx);
      else
        snprintf (anchor, MR_ANCHOR_SIZE, MR_YAML_REF_ANCHOR_TMPLT, (uint32_t)ptrs[ptrdes->first_child].idx);

      yaml_event_delete (event);
      if (!yaml_alias_event_initialize (event, (yaml_char_t *)anchor))
        return (MR_FAILURE);
    }
  else if (ptrdes->flags & MR_IS_NULL)
    {
      yaml_event_delete (event);
      if (!yaml_alias_event_initialize (event, (yaml_char_t *)MR_ISNULL))
        return (MR_FAILURE);
    }
  
  if (!yaml_emitter_emit (emitter, event))
    return (MR_FAILURE);

  return (MR_SUCCESS);
}

/**
 * Emits a node with a scalar value of a given length.
 * @param emitter YAML emitter
 * @param value characters of the value, not necessarily zero terminated
 * @param length number of characters
 * @param ptrs array with node descriptors
 * @param idx index of the node
 * @return status
 */
static inline mr_status_t
yaml_emit_chars (yaml_emitter_t * emitter, const char * value, size_t length, mr_ptrdes_t * ptrs, mr_idx_t idx)
{
  yaml_event_t event;
  char anchor[ANCHOR_SIZE];
  char * utf8 = NULL;
  size_t i;

  /*
    libyaml accepts only valid UTF-8. As in JSON and XML, a byte that is not a
    part of a valid sequence is saved as the code point of the same value, so
    the copy needs at most two bytes for each byte of the value.
  */
  for (i = 0; i < length; ++i)
    if ((unsigned char)value[i] >= 0x80)
      {
        size_t sequence = mr_utf8_sequence_length (&value[i], length - i);
        if (0 == sequence)
          break;
        i += sequence - 1;
      }

  if (i < length)
    {
      utf8 = MR_CALLOC (2 * length + sizeof (char), sizeof (char));
      if (NULL == utf8)
        {
          MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
          return (MR_FAILURE);
        }
      size_t utf8_length = i;
      memcpy (utf8, value, i);
      while (i < length)
        {
          size_t sequence = ((unsigned char)value[i] < 0x80) ? 1 : mr_utf8_sequence_length (&value[i], length - i);
          if (0 == sequence)
            utf8_length += mr_utf8_encode ((unsigned char)value[i++], &utf8[utf8_length]);
          else
            {
              memcpy (&utf8[utf8_length], &value[i], sequence);
              utf8_length += sequence;
              i += sequence;
            }
        }
      value = utf8;
      length = utf8_length;
    }

  int initialized = yaml_scalar_event_initialize (&event, yaml_get_anchor (anchor, &ptrs[idx]),
                                                  (yaml_char_t *)YAML_STR_TAG,
                                                  (yaml_char_t *)value, length,
                                                  1, 0, YAML_PLAIN_SCALAR_STYLE);
  /* the event keeps its own copy of the value */
  if (utf8)
    MR_FREE (utf8);
  if (!initialized)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_YAML_ERROR, "can't create an event of a scalar");
      return (MR_FAILURE);
    }

  return (yaml_emit_field (emitter, &event, ptrs, idx));
}

/**
 * Emits a node with a scalar value.
 * @param emitter YAML emitter
 * @param value scalar value, NULL is emitted as an empty string
 * @param ptrs array with node descriptors
 * @param idx index of the node
 * @return status
 */
static inline mr_status_t
yaml_emit_string (yaml_emitter_t * emitter, const char * value, mr_ptrdes_t * ptrs, mr_idx_t idx)
{
  if (NULL == value)
    value = "";
  return (yaml_emit_chars (emitter, value, strlen (value), ptrs, idx));
}

/**
 * Emits a string node.
 * @param mr_yaml_context YAML emitter and buffer
 * @param ptrs array with node descriptors
 * @param idx index of the node
 * @return status
 */
static mr_status_t
yaml_save_string (mr_yaml_context_t * mr_yaml_context, mr_ptrdes_t * ptrs, mr_idx_t idx)
{
  char * value = *(char **)ptrs[idx].data.ptr;
  return (yaml_emit_string (&mr_yaml_context->emitter, value, ptrs, idx));
}

/**
 * Emits a char node. Characters outside of printable ASCII are emitted as
 * hexadecimal codes.
 * @param mr_yaml_context YAML emitter and buffer
 * @param ptrs array with node descriptors
 * @param idx index of the node
 * @return status
 */
static mr_status_t
yaml_save_char (mr_yaml_context_t * mr_yaml_context, mr_ptrdes_t * ptrs, mr_idx_t idx)
{
  char buffer[] = "0x00";
  unsigned char c = *(unsigned char*)ptrs[idx].data.ptr;
  if ((0x20 <= c) && (c < 0x80))
    {
      buffer[0] = c;
      buffer[1] = 0;
    }
  else
    snprintf (buffer, sizeof (buffer), "0x%02x", c);
    
  return (yaml_emit_string (&mr_yaml_context->emitter, buffer, ptrs, idx));
}

/**
 * Emits a char array node. The array may be not zero terminated.
 * @param mr_yaml_context YAML emitter and buffer
 * @param ptrs array with node descriptors
 * @param idx index of the node
 * @return status
 */
static mr_status_t
yaml_save_char_array (mr_yaml_context_t * mr_yaml_context, mr_ptrdes_t * ptrs, mr_idx_t idx)
{
  mr_ptrdes_t * ptrdes = &ptrs[idx];
  /* a char array may be filled up to its size without a terminating zero */
  return (yaml_emit_chars (&mr_yaml_context->emitter, ptrdes->data.ptr, strnlen (ptrdes->data.ptr, ptrdes->MR_SIZE), ptrs, idx));
}

/**
 * Emits nothing for a void node.
 * @param mr_yaml_context YAML emitter and buffer
 * @param ptrs array with node descriptors
 * @param idx index of the node
 * @return status
 */
static mr_status_t
yaml_save_void (mr_yaml_context_t * mr_yaml_context, mr_ptrdes_t * ptrs, mr_idx_t idx)
{
  return (MR_SUCCESS);
}

/**
 * Emits a bool node as 'true' or 'false'.
 * @param mr_yaml_context YAML emitter and buffer
 * @param ptrs array with node descriptors
 * @param idx index of the node
 * @return status
 */
static mr_status_t
yaml_save_bool (mr_yaml_context_t * mr_yaml_context, mr_ptrdes_t * ptrs, mr_idx_t idx)
{
  char * value = *(bool*)ptrs[idx].data.ptr ? "true" : "false";
  return (yaml_emit_string (&mr_yaml_context->emitter, value, ptrs, idx));
}

/**
 * Emits a function pointer by name or as an address.
 * @param mr_yaml_context YAML emitter and buffer
 * @param ptrs array with node descriptors
 * @param idx index of the node
 * @return status
 */
static mr_status_t
yaml_save_func (mr_yaml_context_t * mr_yaml_context, mr_ptrdes_t * ptrs, mr_idx_t idx)
{
  mr_ptrdes_t * ptrdes = &ptrs[idx];
  const char * func_str = mr_serialize_func (*(void**)ptrdes->data.ptr);
  if (func_str)
    return (yaml_emit_string (&mr_yaml_context->emitter, func_str, ptrs, idx));
#define YAML_PTR_TMPLT "0x%llx"
  char buffer[sizeof (YAML_PTR_TMPLT) + sizeof (void*) * __CHAR_BIT__ / 4];
  snprintf (buffer, sizeof (buffer), YAML_PTR_TMPLT, (unsigned long long int)*(uintptr_t*)ptrdes->data.ptr);
  return (yaml_emit_string (&mr_yaml_context->emitter, buffer, ptrs, idx));
}

/**
 * Starts a mapping for a structure or union.
 * @param mr_yaml_context YAML emitter and buffer
 * @param ptrs array with node descriptors
 * @param idx index of the node
 * @return status
 */
static mr_status_t
yaml_pre_save_struct (mr_yaml_context_t * mr_yaml_context, mr_ptrdes_t * ptrs, mr_idx_t idx)
{
  yaml_event_t event;
  char anchor[ANCHOR_SIZE];
  
  if (!yaml_mapping_start_event_initialize (&event, yaml_get_anchor (anchor, &ptrs[idx]),
                                            (yaml_char_t *)YAML_MAP_TAG,
                                            1, YAML_ANY_MAPPING_STYLE))
    return (MR_FAILURE);
  
  return (yaml_emit_field (&mr_yaml_context->emitter, &event, ptrs, idx));
}

/**
 * Ends the mapping of a structure or union.
 * @param mr_yaml_context YAML emitter and buffer
 * @param ptrs array with node descriptors
 * @param idx index of the node
 * @return status
 */
static mr_status_t
yaml_post_save_struct (mr_yaml_context_t * mr_yaml_context, mr_ptrdes_t * ptrs, mr_idx_t idx)
{
  yaml_event_t event;

  if (!yaml_mapping_end_event_initialize (&event))
    return (MR_FAILURE);
  
  if (!yaml_emitter_emit (&mr_yaml_context->emitter, &event))
    return (MR_FAILURE);

  return (MR_SUCCESS);
}

/**
 * Starts a sequence for an array or pointer. Sequences of scalars use the
 * flow style.
 * @param mr_yaml_context YAML emitter and buffer
 * @param ptrs array with node descriptors
 * @param idx index of the node
 * @return status
 */
static mr_status_t
yaml_pre_save_array (mr_yaml_context_t * mr_yaml_context, mr_ptrdes_t * ptrs, mr_idx_t idx)
{
  yaml_event_t event;
  char anchor[ANCHOR_SIZE];
  yaml_sequence_style_t sequence_style = YAML_ANY_SEQUENCE_STYLE;
  mr_idx_t first_child = ptrs[idx].first_child;
  
#define NON_SCALAR_TYPE (0 MR_FOREACH (MR_ONE_SHIFT, MR_TYPE_STRUCT, MR_TYPE_UNION, MR_TYPE_ANON_UNION, MR_TYPE_NAMED_ANON_UNION, MR_TYPE_POINTER, MR_TYPE_ARRAY))
  if ((first_child != MR_NULL_IDX) && !((NON_SCALAR_TYPE >> ptrs[first_child].mr_type) & 1))
    sequence_style = YAML_FLOW_SEQUENCE_STYLE;

  if (!yaml_sequence_start_event_initialize (&event, yaml_get_anchor (anchor, &ptrs[idx]),
                                             (yaml_char_t *)YAML_SEQ_TAG,
                                             1, sequence_style))
    return (MR_FAILURE);
  
  return (yaml_emit_field (&mr_yaml_context->emitter, &event, ptrs, idx));
}

/**
 * Ends the sequence of an array or pointer.
 * @param mr_yaml_context YAML emitter and buffer
 * @param ptrs array with node descriptors
 * @param idx index of the node
 * @return status
 */
static mr_status_t
yaml_post_save_array (mr_yaml_context_t * mr_yaml_context, mr_ptrdes_t * ptrs, mr_idx_t idx)
{
  yaml_event_t event;

  if (!yaml_sequence_end_event_initialize (&event))
    return (MR_FAILURE);
  
  if (!yaml_emitter_emit (&mr_yaml_context->emitter, &event))
    return (MR_FAILURE);

  return (MR_SUCCESS);
}

/**
 * Starts a sequence for a pointer. NULL pointers and references are emitted
 * as scalars (aliases).
 * @param mr_yaml_context YAML emitter and buffer
 * @param ptrs array with node descriptors
 * @param idx index of the node
 * @return status
 */
static mr_status_t
yaml_pre_save_pointer (mr_yaml_context_t * mr_yaml_context, mr_ptrdes_t * ptrs, mr_idx_t idx)
{
  mr_ptrdes_t * ptrdes = &ptrs[idx];
  mr_idx_t parent = ptrs[idx].parent;

  if ((parent != MR_NULL_IDX) && ptrs[parent].fdp)
    if ((MR_TYPE_UNION == ptrs[parent].mr_type) && ptrs[parent].fdp->stype.tdp)
      if (0 == strcmp (ptrs[parent].fdp->stype.tdp->type.str, "mr_ptr_t"))
        ptrdes->flags &= ~MR_IS_UNNAMED;
  
  if (ptrdes->flags & (MR_IS_NULL | MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE))
    return (yaml_emit_string (&mr_yaml_context->emitter, "", ptrs, idx));

  return (yaml_pre_save_array (mr_yaml_context, ptrs, idx));
}

/**
 * Ends the sequence of a pointer unless it was emitted as a scalar.
 * @param mr_yaml_context YAML emitter and buffer
 * @param ptrs array with node descriptors
 * @param idx index of the node
 * @return status
 */
static mr_status_t
yaml_post_save_pointer (mr_yaml_context_t * mr_yaml_context, mr_ptrdes_t * ptrs, mr_idx_t idx)
{
  mr_ptrdes_t * ptrdes = &ptrs[idx];
  if (ptrdes->flags & (MR_IS_NULL | MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE))
    return (MR_SUCCESS);
  
  return (yaml_post_save_array (mr_yaml_context, ptrs, idx));
}

/**
 * Starts a mapping for an anonymous union. The union is always named.
 * @param mr_yaml_context YAML emitter and buffer
 * @param ptrs array with node descriptors
 * @param idx index of the node
 * @return status
 */
static mr_status_t
yaml_pre_save_anon_union (mr_yaml_context_t * mr_yaml_context, mr_ptrdes_t * ptrs, mr_idx_t idx)
{
  ptrs[idx].flags &= ~MR_IS_UNNAMED;
  return (yaml_pre_save_struct (mr_yaml_context, ptrs, idx));
}

/**
 * Allocates the buffer for formatted values on the first use and makes it
 * an empty string.
 * @param mr_ra_str buffer
 * @return status
 */
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

/*
  Defines static mr_status_t yaml_save_<TYPE> (mr_yaml_context_t * mr_yaml_context, mr_ptrdes_t * ptrs, mr_idx_t idx)
  that formats the node idx with mr_ra_printf_<TYPE> and emits it as a scalar.
  Extra arguments are passed to mr_ra_printf_<TYPE>.
  It returns MR_SUCCESS or MR_FAILURE.
*/
#define YAML_SAVE_HANDLER(TYPE, ...)					\
  static mr_status_t							\
  yaml_save_ ## TYPE (mr_yaml_context_t * mr_yaml_context, mr_ptrdes_t * ptrs, mr_idx_t idx) \
  {									\
    if (MR_SUCCESS != yaml_allocate_ra_str (&mr_yaml_context->buffer))	\
      return (MR_FAILURE);						\
    int count = mr_ra_printf_ ## TYPE					\
      (&mr_yaml_context->buffer, &ptrs[idx]				\
       MR_IF_ELSE (MR_IS_EMPTY (__VA_ARGS__)) () ( , __VA_ARGS__));	\
    if (count <= 0)							\
      return (MR_FAILURE);						\
    return (yaml_emit_string (&mr_yaml_context->emitter,                \
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

/**
 * DFS visitor that emits YAML events for a node with the pre-order or
 * post-order handler of its type.
 * @param ptrs array with node descriptors
 * @param idx index of the node
 * @param level depth of the node
 * @param order DFS order
 * @param context YAML emitter and buffer
 * @return status
 */
static mr_status_t
yaml_print_node (mr_ptrdes_t * ptrs, mr_idx_t idx, int level, mr_dfs_order_t order, void * context)
{
  mr_yaml_context_t * mr_yaml_context = context;
  mr_yaml_save_handler_t save_handler = NULL;
  mr_type_t mr_type = ptrs[idx].mr_type;
  
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
mr_yaml_save (mr_ptrdes_t * ptrs)
{
  if (NULL == ptrs)
    return (NULL);

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
