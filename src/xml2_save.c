/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of Metaresc project */

#include <metaresc.h>

/**
 * Public function. Save scheduler. Save any object as XML node.
 * @param ptrs resizeable array with pointers descriptors 
 * @return XML document
 */
xmlDocPtr
xml2_save (mr_ra_mr_ptrdes_t * ptrs)
{
  long idx = 0;
  xmlDocPtr doc = xmlNewDoc (BAD_CAST "1.0");
  
  if (NULL == doc)
    {
      MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_XML_SAVE_FAILED);
      return (NULL);
    }

  ptrs->ra.ext.ptr = doc;
  while (idx >= 0)
    {
      mr_fd_t * fdp = &ptrs->ra.data[idx].fd;
      xmlNodePtr node = xmlNewNode (NULL, BAD_CAST fdp->name);
      
      ptrs->ra.data[idx].ext.ptr = node;

      if (NULL == node)
	MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_OUT_OF_MEMORY);
      else
	{
	  int parent = ptrs->ra.data[idx].parent;
	  char number[MR_INT_TO_STRING_BUF_SIZE];
	  char * content = NULL;
	  
	  node->_private = (void*)idx;

	  /* route saving handler */
	  if ((fdp->mr_type_ext >= 0) && (fdp->mr_type_ext < MR_MAX_TYPES)
	      && mr_conf.io_ext_handlers[fdp->mr_type_ext].save.xml2)
	    content = mr_conf.io_ext_handlers[fdp->mr_type_ext].save.xml2 (idx, ptrs);
	  else if ((fdp->mr_type >= 0) && (fdp->mr_type < MR_MAX_TYPES)
		   && mr_conf.io_handlers[fdp->mr_type].save.xml2)
	    content = mr_conf.io_handlers[fdp->mr_type].save.xml2 (idx, ptrs);
	  else
	    MR_MESSAGE_UNSUPPORTED_NODE_TYPE_ (fdp);    	  

	  if (content)
	    {
	      if (content[0])
		xmlNodeSetContent (node, BAD_CAST content);
	      MR_FREE (content);
	    }

	  if (ptrs->ra.data[idx].ref_idx >= 0)
	    {
	      /* set REF_IDX property */
	      sprintf (number, "%" SCNd32, ptrs->ra.data[ptrs->ra.data[idx].ref_idx].idx);
	      xmlSetProp (node,
			  BAD_CAST ((ptrs->ra.data[idx].flags.is_content_reference) ? MR_REF_CONTENT : MR_REF),
			  BAD_CAST number);
	    }
	  if (ptrs->ra.data[idx].flags.is_referenced)
	    {
	      /* set IDX property */
	      sprintf (number, "%" SCNd32, ptrs->ra.data[idx].idx);
	      xmlSetProp (node, BAD_CAST MR_REF_IDX, BAD_CAST number);
	    }
	  if (ptrs->ra.data[idx].flags.is_null)
	    xmlSetProp (node, BAD_CAST MR_ISNULL, BAD_CAST MR_ISNULL_VALUE);

	  if (parent >= 0)
	    xmlAddChild (ptrs->ra.data[parent].ext.ptr, node);
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
    MR_MESSAGE (MR_LL_ERROR, MR_MESSAGE_XML_SAVE_FAILED);
  else
    xmlDocSetRootElement (doc, ptrs->ra.data[0].ext.ptr);
  
  return (doc);
}

/**
 * XML string quote handler. Uses libxml encoding function.
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors 
 * @return XML escaped string content
 */
static char *
xml2_save_string (int idx, mr_ra_mr_ptrdes_t * ptrs)
{
  xmlDocPtr doc = ptrs->ra.ext.ptr;
  xmlChar * encoded_content;
  char * content = *(char**)ptrs->ra.data[idx].data;
  
  if ((NULL == content) || (ptrs->ra.data[idx].ref_idx >= 0))
    content = "";
  encoded_content = xmlEncodeEntitiesReentrant (doc, BAD_CAST content);
  if (NULL == encoded_content)
    {
      content = MR_STRDUP ("");
      MR_MESSAGE (MR_LL_FATAL, MR_MESSAGE_XML_STRING_ENCODING_FAILED, content);
    }
  else
    {
      content = MR_STRDUP ((char*)encoded_content);
      xmlFree (encoded_content);
    }
  return (content);
}

/**
 * libxml initialize handlers. Copy handlers common with builtin implementation and replace string handler.
 */
static void __attribute__((constructor))
mr_init_save_xml2 (void)
{
  int i;
  
  mr_init_save_xml ();
  for (i = 0; i < MR_MAX_TYPES; ++i)
    {
      mr_conf.io_handlers[i].save.xml2 = mr_conf.io_handlers[i].save.xml;
      mr_conf.io_ext_handlers[i].save.xml2 = mr_conf.io_ext_handlers[i].save.xml;
    }
    
  mr_conf.io_handlers[MR_TYPE_STRING].save.xml2 = xml2_save_string;
}
