#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include <metaresc.h>
#include <mem_failures.h>

static mr_status_t
xml2_load_method (void * arg)
{
  mr_conf_t _mr_conf;
  mr_status_t status;
  xmlNodePtr node = arg;
  memset (&_mr_conf, 0, sizeof (_mr_conf));
  status = MR_LOAD_XML2_NODE_ARG3 (mr_conf_t, node, &_mr_conf);
  mr_conf.mr_mem = mr_mem;
  mr_ra_ptrdes_t ptrs = MR_SAVE (mr_conf_t, &_mr_conf);
  mr_conf.mr_mem = _mr_mem;
  if (ptrs.ra != NULL)
    {
      mr_free_recursively (&ptrs);
      mr_mem.free (NULL, NULL, 0, ptrs.ra);
    }
  return (status);
}

MR_START_TEST (mem_load_failures_xml2, "test load memory operations failures for XML2")
{
  mr_detect_type (NULL); /* explicitly init library */
  char * xml2 = MR_SAVE_XML2 (mr_conf_t, &mr_conf);
  ck_assert_msg (xml2 != NULL, "Failed to save mr_conf for tests of load methods");
  xmlDocPtr doc = xmlParseMemory (xml2, strlen (xml2));
  MR_FREE (xml2);
  ck_assert_msg (doc != NULL, "Failed to parse xml");
  xmlNodePtr node = xmlDocGetRootElement (doc);
  ck_assert_msg (node != NULL, "Failed to get root element from xml document");
  mem_failures_method (xml2_load_method, node);
  xmlFreeDoc (doc);
} END_TEST
