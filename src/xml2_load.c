/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

#define RL_MODE PROTO /* explicitly set type of inclusion */
#include <reslib.h>

int
xml2_load (xmlNodePtr node, rl_ra_rl_ptrdes_t * ptrs)
{
  int idx = rl_add_ptr_to_list (ptrs);
  xmlNodePtr node_;
  char * content = NULL;
  char * property = NULL;

  if (idx < 0)
    return (idx);

  /* handle REF_IDX property */
  property = (char*)xmlGetProp (node, (unsigned char*)RL_REF_IDX);
  if (property)
    {
      if (1 != sscanf (property, "%" SCNd32, &ptrs->ra.data[idx].idx))
	RL_MESSAGE (RL_LL_WARN, RL_MESSAGE_READ_REF, property);
      ptrs->ra.data[idx].flags |= RL_PDF_IS_REFERENCED;
      xmlFree (property);
    }
  /* handle REF property */
  property = (char*)xmlGetProp (node, (unsigned char*)RL_REF);
  if (property)
    {
      if (1 != sscanf (property, "%" SCNd32, &ptrs->ra.data[idx].ref_idx))
	RL_MESSAGE (RL_LL_WARN, RL_MESSAGE_READ_REF, property);
      xmlFree (property);
    }
  /* handle REF_CONTENT property */
  property = (char*)xmlGetProp (node, (unsigned char*)RL_REF_CONTENT);
  if (property)
    {
      if (1 != sscanf (property, "%" SCNd32, &ptrs->ra.data[idx].ref_idx))
	RL_MESSAGE (RL_LL_WARN, RL_MESSAGE_READ_REF, property);
      else
	ptrs->ra.data[idx].flags |= RL_PDF_CONTENT_REFERENCE;
      xmlFree (property);
    }
  property = (char*)xmlGetProp (node, (unsigned char*)RL_RARRAY_SIZE);
  if (property)
    {
      if (1 != sscanf (property, "%" SCNd32, &ptrs->ra.data[idx].rarray_size))
	RL_MESSAGE (RL_LL_WARN, RL_MESSAGE_READ_RARRAY_SIZE, property);
      xmlFree (property);
    }
  property = (char*)xmlGetProp (node, (unsigned char*)RL_ISNULL);
  if (property)
    {
      ptrs->ra.data[idx].flags |= RL_PDF_IS_NULL;
      xmlFree (property);
    }

  for (node_ = node->xmlChildrenNode; node_ && !content; node_ = node_->next)
    content = (char*)XML_GET_CONTENT (node_);

  ptrs->ra.data[idx].value = content ? RL_STRDUP (content) : RL_STRDUP ("");
  ptrs->ra.data[idx].fd.name = RL_STRDUP ((char*)node->name);

  /* loop on subnodes */
  for (node_ = node->xmlChildrenNode; node_; node_ = node_->next)
    if (XML_ELEMENT_NODE == node_->type)
      rl_add_child (idx, xml2_load (node_, ptrs), ptrs);

  return (idx);
}
