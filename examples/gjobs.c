/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <metaresc.h>

TYPEDEF_STRUCT (project_t, int ID); // typedef struct project_t { int ID } project_t;

#define MR_IS_BUILTIN_project_t project_t,

TYPEDEF_ENUM (status_t,
	      Open,
	      Close,
	      );

#define MR_IS_BUILTIN_status_t status_t,

TYPEDEF_STRUCT (update_t,
		status_t Status,
		string_t Modified,
		string_t Salary,
		);

#define MR_IS_BUILTIN_update_t update_t,

TYPEDEF_STRUCT (person_t,
		string_t Person,
		string_t Email,
		string_t Company,
		string_t Organisation,
		string_t Webpage,
		string_t Snailmail,
		string_t Phone,
		);

#define MR_IS_BUILTIN_person_t person_t,

TYPEDEF_STRUCT (job_t,
		project_t Project,
		string_t Application,
		string_t Category,
		update_t Update,
		(person_t *, Developers),
		_ size_t Developers_count,
		person_t Contact,
		string_t Requirements,
		string_t Skills,
		string_t Details,
		);

TYPEDEF_STRUCT (helping_t,
		(job_t *, Jobs),
		_ size_t Jobs_count,
		);

int
main (int argc, char * argv[])
{
#ifdef HAVE_LIBXML2
  LIBXML_TEST_VERSION;
#endif /* HAVE_LIBXML2 */

  char * xml = mr_read_xml_doc (stdin);
  if (NULL == xml)
    printf ("mr_read_xml_doc failed\n");
  else
    {
      helping_t helping = MR_LOAD_XML (helping_t, xml);
      MR_PRINT ((helping_t, &helping, YAML));
      MR_FREE_RECURSIVELY (helping_t, &helping);
      MR_FREE (xml);
    }

  /* Clean up everything else before quitting. */
#ifdef HAVE_LIBXML2
  xmlCleanupParser();
#endif /* HAVE_LIBXML2 */
  return (EXIT_SUCCESS);
}
