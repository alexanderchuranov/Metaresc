/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

#include <reslib.h>

/**
 * Public function. Save scheduler. Save any object as XML node.
 * @param node a pointer on XML node
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors 
 */
xmlDocPtr
xml2_save (rl_ra_rl_ptrdes_t * ptrs)
{
  long idx = 0;
  xmlDocPtr doc = xmlNewDoc (BAD_CAST "1.0");
  
  if (NULL == doc)
    {
      RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_XML_SAVE_FAILED);
      return (NULL);
    }

  ptrs->ra.ext.ptr = doc;
  while (idx >= 0)
    {
      int parent = ptrs->ra.data[idx].parent;
      rl_fd_t * fdp = &ptrs->ra.data[idx].fd;
      char * content = NULL;
      
      /* route saving handler */
      if ((fdp->rl_type_ext >= 0) && (fdp->rl_type_ext < RL_MAX_TYPES)
	  && rl_conf.io_ext_handlers[fdp->rl_type_ext].save.xml2)
	content = rl_conf.io_ext_handlers[fdp->rl_type_ext].save.xml2 (idx, ptrs);
      else if ((fdp->rl_type >= 0) && (fdp->rl_type < RL_MAX_TYPES)
	       && rl_conf.io_handlers[fdp->rl_type].save.xml2)
	content = rl_conf.io_handlers[fdp->rl_type].save.xml2 (idx, ptrs);
      else
	RL_MESSAGE_UNSUPPORTED_NODE_TYPE_ (fdp);    	  

      if (content)
	{
	  char number[RL_INT_TO_STRING_BUF_SIZE];
	  xmlNodePtr node = xmlNewNode (NULL, BAD_CAST fdp->name);
	  ptrs->ra.data[idx].ext.ptr = node;

	  if (NULL == node)
	    RL_MESSAGE (RL_LL_FATAL, RL_MESSAGE_OUT_OF_MEMORY);
	  else
	    {
	      node->_private = (void*)idx;

	      if (content[0])
		xmlNodeSetContent (node, BAD_CAST content);

	      if (ptrs->ra.data[idx].ref_idx >= 0)
		{
		  /* set REF_IDX property */
		  sprintf (number, "%" SCNd32, ptrs->ra.data[ptrs->ra.data[idx].ref_idx].idx);
		  xmlSetProp (node,
			      BAD_CAST ((ptrs->ra.data[idx].flags & RL_PDF_CONTENT_REFERENCE) ? RL_REF_CONTENT : RL_REF),
			      BAD_CAST number);
		}
	      if (ptrs->ra.data[idx].flags & RL_PDF_IS_REFERENCED)
		{
		  /* set IDX property */
		  sprintf (number, "%" SCNd32, ptrs->ra.data[idx].idx);
		  xmlSetProp (node, BAD_CAST RL_REF_IDX, BAD_CAST number);
		}
	      if (ptrs->ra.data[idx].flags & RL_PDF_IS_NULL)
		xmlSetProp (node, BAD_CAST RL_ISNULL, BAD_CAST "true");      

	      if (parent >= 0)
		xmlAddChild (ptrs->ra.data[parent].ext.ptr, node);
	    }
	  RL_FREE (content);
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
  
  if (NULL == ptrs->ra.data[0].ext.ptr)
    RL_MESSAGE (RL_LL_ERROR, RL_MESSAGE_XML_SAVE_FAILED);
  else
    xmlDocSetRootElement (doc, ptrs->ra.data[0].ext.ptr);
  
  return (doc);
}

static char *
xml2_save_string (int idx, rl_ra_rl_ptrdes_t * ptrs)
{
  xmlDocPtr doc = ptrs->ra.ext.ptr;
  xmlChar * encoded_content;
  char * content = *(char**)ptrs->ra.data[idx].data;
  
  if ((NULL == content) || (ptrs->ra.data[idx].ref_idx >= 0))
    content = "";
  encoded_content = xmlEncodeEntitiesReentrant (doc, BAD_CAST content);
  if (NULL == encoded_content)
    {
      content = RL_STRDUP ("");
      RL_MESSAGE (RL_LL_FATAL, RL_MESSAGE_XML_STRING_ENCODING_FAILED, content);
    }
  else
    {
      content = RL_STRDUP ((char*)encoded_content);
      xmlFree (encoded_content);
    }
  return (content);
}

static void __attribute__((constructor))
rl_init_save_xml2 (void)
{
  int i;
  
  rl_init_save_xml ();
  for (i = 0; i < RL_MAX_TYPES; ++i)
    {
      rl_conf.io_handlers[i].save.xml2 = rl_conf.io_handlers[i].save.xml;
      rl_conf.io_ext_handlers[i].save.xml2 = rl_conf.io_ext_handlers[i].save.xml;
    }
    
  rl_conf.io_handlers[RL_TYPE_STRING].save.xml2 = xml2_save_string;
}
