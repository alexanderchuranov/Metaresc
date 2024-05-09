/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

#include <metaresc.h>

static mr_status_t
mr_load_xml_property (xmlNodePtr node, char * property_name, mr_idx_t * idx, mr_ptrdes_flags_t * flags, mr_ptrdes_flags_t flag)
{
  char * property = (char*)xmlGetProp (node, (unsigned char*)property_name);
  if (property)
    {
      char * tail = NULL;
      *idx = mr_strtouintmax (property, &tail, 0);
      if (tail)
	while (isspace (*tail))
	  ++tail;
      bool fail = (NULL == tail) || *tail;
      if (fail)
	MR_MESSAGE (MR_LL_WARN, MR_MESSAGE_READ_REF, property);
      xmlFree (property);
      if (fail)
	return (MR_FAILURE);
      *flags |= flag;
    }
  return (MR_SUCCESS);
}

mr_idx_t
mr_xml2_load (xmlNodePtr node, mr_ra_ptrdes_t * ptrs)
{
  xmlNodePtr node_;
  char * content = NULL;
  char * property = NULL;
  mr_idx_t idx = mr_add_ptr_to_list (ptrs);
  if (MR_NULL_IDX == idx)
    goto failure;

  /* handle REF_IDX property */
  if (mr_load_xml_property (node, MR_REF_IDX, &ptrs->ra[idx].idx, &ptrs->ra[idx].flags, MR_IS_REFERENCED) != MR_SUCCESS)
    goto failure;
  /* handle REF property */
  if (mr_load_xml_property (node, MR_REF, &ptrs->ra[idx].first_child, &ptrs->ra[idx].flags, MR_IS_REFERENCE) != MR_SUCCESS)
    goto failure;
  /* handle REF_CONTENT property */
  if (mr_load_xml_property (node, MR_REF_CONTENT, &ptrs->ra[idx].first_child, &ptrs->ra[idx].flags, MR_IS_CONTENT_REFERENCE) != MR_SUCCESS)
    goto failure;

  property = (char*)xmlGetProp (node, (unsigned char*)MR_ISNULL);
  if (property)
    {
      ptrs->ra[idx].flags |= MR_IS_NULL;
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
      goto failure;
    }
  
  ptrs->ra[idx].value_type = MR_VT_SUBSTR;
  ptrs->ra[idx].load_params.vt_substr.str = content;
  ptrs->ra[idx].load_params.vt_substr.length = strlen (content);

  /* loop on subnodes */
  for (node_ = node->xmlChildrenNode; node_; node_ = node_->next)
    if (XML_ELEMENT_NODE == node_->type)
      {
	if (ptrs->ra[idx].flags & (MR_IS_REFERENCE | MR_IS_CONTENT_REFERENCE))
	  {
	    MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_UNEXPECTED_SUBNODES);
	    goto failure;
	  }
	mr_idx_t child = mr_xml2_load (node_, ptrs);
	if (MR_NULL_IDX == child)
	  goto failure;
	mr_add_child (ptrs, idx, child);
      }
  
  return (idx);

 failure:
  if (ptrs->ra)
    MR_FREE (ptrs->ra);
  ptrs->ra = NULL;
  return (0);
}
