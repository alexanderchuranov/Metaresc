/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

#define RL_MODE PROTO /* explicitly set type of inclusion */
#include <reslib.h>

/**
 * Public function. Save scheduler. Save any object as XML node.
 * @param node a pointer on XML node
 * @param idx an index of node in ptrs
 * @param ptrs resizeable array with pointers descriptors 
 */
xmlNodePtr
xml2_save (rl_ra_rl_ptrdes_t * ptrs)
{
  long idx = 0;
  while (idx >= 0)
    {
      int parent = ptrs->ra.data[idx].parent;
      rl_fd_t * fdp = &ptrs->ra.data[idx].fd;
      char * content = NULL;
      
      /* route saving handler */
      if ((fdp->rl_type_ext >= 0) && (fdp->rl_type_ext < RL_MAX_TYPES)
	  && rl_conf.io_ext_handlers[fdp->rl_type_ext].save.xml)
	content = rl_conf.io_ext_handlers[fdp->rl_type_ext].save.xml (idx, ptrs);
      else if ((fdp->rl_type >= 0) && (fdp->rl_type < RL_MAX_TYPES)
	       && rl_conf.io_handlers[fdp->rl_type].save.xml)
	content = rl_conf.io_handlers[fdp->rl_type].save.xml (idx, ptrs);
      else
	RL_MESSAGE_UNSUPPORTED_NODE_TYPE_ (fdp);    	  

      if (content)
	{
	  xmlNodePtr node = xmlNewNode (NULL, (unsigned char*)fdp->name);
  
	  if (NULL == node)
	    {
	      RL_FREE (content);
	      RL_MESSAGE (RL_LL_FATAL, RL_MESSAGE_OUT_OF_MEMORY);
	    }
	  else
	    {
	      char number[RL_INT_TO_STRING_BUF_SIZE];
	      node->_private = (void*)idx;
	      ptrs->ra.data[idx].ext = node;

	      if (content[0])
		xmlNodeSetContent (node, (unsigned char*)content);
	      RL_FREE (content);

	      if (ptrs->ra.data[idx].ref_idx >= 0)
		{
		  /* set REF_IDX property */
		  sprintf (number, "%" SCNd32, ptrs->ra.data[ptrs->ra.data[idx].ref_idx].idx);
		  xmlSetProp (node, (unsigned char*)RL_REF, (unsigned char*)number);
		}
	      if (ptrs->ra.data[idx].flags & RL_PDF_IS_REFERENCED)
		{
		  /* set IDX property */
		  sprintf (number, "%" SCNd32, ptrs->ra.data[idx].idx);
		  xmlSetProp (node, (unsigned char*)RL_REF_IDX, (unsigned char*)number);
		}
	      if (ptrs->ra.data[idx].flags & RL_PDF_IS_NULL)
		xmlSetProp (node, (unsigned char*)RL_ISNULL, (unsigned char*)"true");      

	      if (parent >= 0)
		xmlAddChild (ptrs->ra.data[parent].ext, node);
	    }
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
  
  return (ptrs->ra.data[0].ext);
}

extern void rl_init_save_xml1 (void);
static void __attribute__((constructor)) rl_init_save_xml2 (void) { rl_init_save_xml1 (); }
