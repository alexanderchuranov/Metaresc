#include <yaml.h>
#include <metaresc.h>

static void
add_missed_crossrefs (mr_ra_ptrdes_t * ptrs)
{
  int i;
  int count = ptrs->size / sizeof (ptrs->ra[0]);
  int max_idx = 0;
  int * table;

  for (i = 0; i < count; ++i)
    if (ptrs->ra[i].idx > max_idx)
      max_idx = ptrs->ra[i].idx;

  table = MR_CALLOC (max_idx + 1, sizeof (table[0]));
  if (NULL == table)
    return;

  for (i = 0; i <= max_idx; ++i)
    table[i] = -1;
  for (i = 0; i < count; ++i)
    if (ptrs->ra[i].idx >= 0)
      table[ptrs->ra[i].idx] = i;

  for (i = 0; i < count; ++i)
    {
      int ref_idx = ptrs->ra[i].ref_idx;
      if (ref_idx >= 0)
	if ((ref_idx > max_idx) || (table[ref_idx] < 0))
	  if ((ref_idx < count) && (ptrs->ra[ref_idx].idx < 0))
	    {
	      ptrs->ra[ref_idx].idx = ref_idx;
	      ptrs->ra[ref_idx].flags.is_referenced = true;
	      if (ref_idx <= max_idx)
		table[ref_idx] = ref_idx;
	    }
    }
  
  MR_FREE (table);
}

static mr_status_t
yaml_add_node (mr_ra_ptrdes_t * ptrs, int * parent, char * name, char * value, char * anchor, char * alias, bool unnamed)
{
  int idx = mr_add_ptr_to_list (ptrs);
  if (idx < 0)
    return (MR_FAILURE);
  mr_add_child (*parent, idx, ptrs->ra);

  ptrs->ra[idx].unnamed = unnamed;
  ptrs->ra[idx].name = name;
  
  if (value)
    {
      ptrs->ra[idx].load_params.mr_value.value_type = MR_VT_STRING;
      ptrs->ra[idx].load_params.mr_value.vt_string = mr_strdup (value);
      if (NULL == ptrs->ra[idx].load_params.mr_value.vt_string)
	return (MR_FAILURE);
    }

  if (alias)
    {
      if (1 == sscanf (alias, MR_YAML_REF_ANCHOR_CONTENT_TMPLT, &ptrs->ra[idx].ref_idx))
	ptrs->ra[idx].flags.is_content_reference = true;
      else if (1 != sscanf (alias, MR_YAML_REF_ANCHOR_TMPLT, &ptrs->ra[idx].ref_idx))
	{
	  if (strcmp (alias, MR_ISNULL) == 0)
	    ptrs->ra[idx].flags.is_null = true;
	  else
	    MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_READ_REF, alias);
	}
    }

  if (anchor)
    {
      if (1 == sscanf (anchor, MR_YAML_ANCHOR_TMPLT, &ptrs->ra[idx].idx))
	ptrs->ra[idx].flags.is_referenced = true;
      else
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_READ_REF, anchor);
    }

  if ((NULL == value) && (NULL == alias))
    *parent = idx;
  return (MR_SUCCESS);
}

mr_status_t
mr_yaml_load (char * str, mr_ra_ptrdes_t * ptrs)
{
  yaml_parser_t parser;
  mr_status_t status;
  int parent = -1;

  if (NULL == str)
    return (MR_FAILURE);
  
  status = yaml_parser_initialize (&parser) ? MR_SUCCESS : MR_FAILURE;
  if (MR_SUCCESS == status)
    {
      bool done = false;
      yaml_parser_set_input_string (&parser, (unsigned char *)str, strlen (str));

      while (!done && (MR_SUCCESS == status))
	{
	  yaml_event_t event;
	  status = yaml_parser_parse (&parser, &event) ? MR_SUCCESS : MR_FAILURE;
	  if (MR_SUCCESS != status)
	    break;
      
	  switch (event.type)
	    {
	    case YAML_SCALAR_EVENT:
	      if (parent >= 0)
		if (!ptrs->ra[parent].unnamed)
		  {
		    char * name = mr_get_static_field_name_from_string ((char*)event.data.scalar.value);
		    if (NULL == name)
		      {
			MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNKNOWN_FIELD_NAME, (char*)event.data.scalar.value);
			status = MR_FAILURE;
			break;
		      }
		    yaml_event_delete (&event);
		    
		    status = yaml_parser_parse (&parser, &event) ? MR_SUCCESS : MR_FAILURE;
		    if (MR_SUCCESS != status)
		      break;

		    switch (event.type)
		      {
		      case YAML_SCALAR_EVENT:
			status = yaml_add_node (ptrs, &parent, name, (char*)event.data.scalar.value, (char*)event.data.scalar.anchor, NULL, false);
			break;
		      case YAML_MAPPING_START_EVENT:
			status = yaml_add_node (ptrs, &parent, name, NULL, (char*)event.data.scalar.anchor, NULL, false);
			break;
		      case YAML_SEQUENCE_START_EVENT:
			status = yaml_add_node (ptrs, &parent, name, NULL, (char*)event.data.scalar.anchor, NULL, true);
		      break;
		      case YAML_ALIAS_EVENT:
			status = yaml_add_node (ptrs, &parent, name, NULL, NULL, (char*)event.data.alias.anchor, false);
			break;
		      default:
			status = MR_FAILURE;
			break;
		      }
		    break;
		  }

	      status = yaml_add_node (ptrs, &parent, NULL, (char*)event.data.scalar.value, (char*)event.data.scalar.anchor, NULL, true);
	      break;

	    case YAML_ALIAS_EVENT:
	      status = yaml_add_node (ptrs, &parent, NULL, NULL, NULL, (char*)event.data.alias.anchor, true);
	      break;

	    case YAML_MAPPING_START_EVENT:
	      status = yaml_add_node (ptrs, &parent, NULL, NULL, (char*)event.data.mapping_start.anchor, NULL, false);
	      break;
	      
	    case YAML_MAPPING_END_EVENT:
	      if (parent < 0)
		status = MR_FAILURE;
	      else
		{
		  status = ptrs->ra[parent].unnamed ? MR_FAILURE : MR_SUCCESS;
		  parent = ptrs->ra[parent].parent;
		}
	      break;
	      
	    case YAML_SEQUENCE_START_EVENT:
	      status = yaml_add_node (ptrs, &parent, NULL, NULL, (char*)event.data.sequence_start.anchor, NULL, true);
	      break;
	      
	    case YAML_SEQUENCE_END_EVENT:
	      if (parent < 0)
		status = MR_FAILURE;
	      else
		{
		  status = !ptrs->ra[parent].unnamed ? MR_FAILURE : MR_SUCCESS;
		  parent = ptrs->ra[parent].parent;
		}
	      break;

	    case YAML_STREAM_END_EVENT:
	      done = true;
	      __attribute__ ((fallthrough));
	    case YAML_STREAM_START_EVENT:
	    case YAML_DOCUMENT_START_EVENT:
	    case YAML_DOCUMENT_END_EVENT:
	      status = (-1 == parent) ? MR_SUCCESS : MR_FAILURE;
	      break;
	      
	    case YAML_NO_EVENT:
	      break;
	    }

	  yaml_event_delete (&event);
	}

      if (parser.error != YAML_NO_ERROR)
	{
	  MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_YAML_ERROR, parser.problem);
	  status = MR_FAILURE;
	}
      
      yaml_parser_delete (&parser);
    }

  if (MR_SUCCESS == status)
    add_missed_crossrefs (ptrs);
  else
    {
      if (ptrs->ra)
	{
	  int i;
	  for (i = ptrs->size / sizeof (ptrs->ra[0]) - 1; i >= 0; --i)
	    if (MR_VT_STRING == ptrs->ra[i].load_params.mr_value.value_type)
	      if (ptrs->ra[i].load_params.mr_value.vt_string)
		MR_FREE (ptrs->ra[i].load_params.mr_value.vt_string);
	  MR_FREE (ptrs->ra);
	}
      memset (ptrs, 0, sizeof (*ptrs));
    }

  return (status);
}
