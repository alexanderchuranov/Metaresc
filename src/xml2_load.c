/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

#include <metaresc.h>

mr_idx_t
mr_xml2_load (xmlNodePtr node, mr_ra_ptrdes_t * ptrs)
{
  xmlNodePtr node_;
  char * content = NULL;
  char * property = NULL;
  uint32_t prop_value = 0;
  mr_idx_t idx = mr_add_ptr_to_list (ptrs);
  if (0 == idx)
    return (0);

  /* handle REF_IDX property */
  property = (char*)xmlGetProp (node, (unsigned char*)MR_REF_IDX);
  if (property)
    {
      if (1 == sscanf (property, "%" SCNu32, &prop_value))
	ptrs->ra[idx].idx = prop_value;
      else
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_READ_REF, property);
      ptrs->ra[idx].flags.is_referenced = true;
      xmlFree (property);
    }
  /* handle REF property */
  property = (char*)xmlGetProp (node, (unsigned char*)MR_REF);
  if (property)
    {
      if (1 == sscanf (property, "%" SCNu32, &prop_value))
	ptrs->ra[idx].ref_idx = prop_value;
      else
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_READ_REF, property);
      xmlFree (property);
    }
  /* handle REF_CONTENT property */
  property = (char*)xmlGetProp (node, (unsigned char*)MR_REF_CONTENT);
  if (property)
    {
      if (1 == sscanf (property, "%" SCNu32, &prop_value))
	ptrs->ra[idx].ref_idx = prop_value;
      else
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_READ_REF, property);
      ptrs->ra[idx].flags.is_content_reference = true;
      xmlFree (property);
    }
  property = (char*)xmlGetProp (node, (unsigned char*)MR_ISNULL);
  if (property)
    {
      ptrs->ra[idx].flags.is_null = true;
      xmlFree (property);
    }

  for (node_ = node->xmlChildrenNode; node_ && !content; node_ = node_->next)
    content = (char*)XML_GET_CONTENT (node_);

  if (NULL == content)
    content = "";

  ptrs->ra[idx].fdp = mr_get_any_fd_by_name ((char*)node->name, NULL);
  if (NULL == ptrs->ra[idx].fdp)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNKNOWN_FIELD_NAME, (char*)node->name);
      return (-1);
    }
  
  ptrs->ra[idx].value_type = MR_VT_SUBSTR;
  ptrs->ra[idx].load_params.vt_substr.str = content;
  ptrs->ra[idx].load_params.vt_substr.length = strlen (content);

  /* loop on subnodes */
  for (node_ = node->xmlChildrenNode; node_; node_ = node_->next)
    if (XML_ELEMENT_NODE == node_->type)
      {
	mr_idx_t child = mr_xml2_load (node_, ptrs);
	if (child == 0)
	  return (0);
	mr_add_child (idx, child, ptrs->ra);
      }
  
  return (idx);
}
