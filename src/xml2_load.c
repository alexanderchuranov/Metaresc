/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <metaresc.h>

int
xml2_load (xmlNodePtr node, mr_ra_ptrdes_t * ptrs)
{
  int idx = mr_add_ptr_to_list (ptrs);
  xmlNodePtr node_;
  char * content = NULL;
  char * property = NULL;

  if (idx < 0)
    return (idx);

  /* handle REF_IDX property */
  property = (char*)xmlGetProp (node, (unsigned char*)MR_REF_IDX);
  if (property)
    {
      if (1 != sscanf (property, "%" SCNd32, &ptrs->ra[idx].idx))
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_READ_REF, property);
      ptrs->ra[idx].flags.is_referenced = true;
      xmlFree (property);
    }
  /* handle REF property */
  property = (char*)xmlGetProp (node, (unsigned char*)MR_REF);
  if (property)
    {
      if (1 != sscanf (property, "%" SCNd32, &ptrs->ra[idx].ref_idx))
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_READ_REF, property);
      xmlFree (property);
    }
  /* handle REF_CONTENT property */
  property = (char*)xmlGetProp (node, (unsigned char*)MR_REF_CONTENT);
  if (property)
    {
      if (1 != sscanf (property, "%" SCNd32, &ptrs->ra[idx].ref_idx))
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
  
  ptrs->ra[idx].name_ss.str = (char*)node->name;
  ptrs->ra[idx].name_ss.length = strlen (ptrs->ra[idx].name_ss.str);
  
  ptrs->ra[idx].mr_value.value_type = MR_VT_UNKNOWN;
  ptrs->ra[idx].mr_value.vt_string = mr_strdup (content);
  if (NULL == ptrs->ra[idx].mr_value.vt_string)
    return (-1);

  /* loop on subnodes */
  for (node_ = node->xmlChildrenNode; node_; node_ = node_->next)
    if (XML_ELEMENT_NODE == node_->type)
      {
	int child = xml2_load (node_, ptrs);
	if (child < 0)
	  return (child);
	mr_add_child (idx, child, ptrs->ra);
      }
  
  return (idx);
}
