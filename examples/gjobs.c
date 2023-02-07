/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <metaresc.h>

TYPEDEF_STRUCT (project_t, int ID) // typedef struct project_t { int ID } project_t;

TYPEDEF_ENUM (status_t,
	      Open,
	      Close,
	      )

TYPEDEF_STRUCT (update_t,
		(status_t, Status),
		string_t Modified,
		string_t Salary,
		)

TYPEDEF_STRUCT (person_t,
		string_t Person,
		string_t Email,
		string_t Company,
		string_t Organisation,
		string_t Webpage,
		string_t Snailmail,
		string_t Phone,
		)

TYPEDEF_STRUCT (job_t,
		(project_t, Project),
		string_t Application,
		string_t Category,
		(update_t, Update),
		(person_t *, Developers, /* suffix */, /* meta */, { .offset = offsetof (job_t, Developers_size) }, "offset"),
		VOID (ssize_t, Developers_size),
		(person_t, Contact),
		string_t Requirements,
		string_t Skills,
		string_t Details,
		)

TYPEDEF_STRUCT (helping_t,
		(job_t *, Jobs, /* suffix */ , /* meta */, { .offset = offsetof (helping_t, Jobs_size) }, "offset" ),
		VOID (ssize_t, Jobs_size),
		)

static void
print_person (person_t * person)
{
  if (person == NULL) return;
  printf ("------ Person\n");
  if (person->Person) printf ("	name: %s\n", person->Person);
  if (person->Email) printf ("	email: %s\n", person->Email);
  if (person->Company) printf ("	company: %s\n", person->Company);
  if (person->Organisation) printf ("	organisation: %s\n", person->Organisation);
  if (person->Snailmail) printf ("	smail: %s\n", person->Snailmail);
  if (person->Webpage) printf ("	Web: %s\n", person->Webpage);
  if (person->Phone) printf ("	phone: %s\n", person->Phone);
  printf ("------\n");
}

static void
print_job (job_t * job)
{
  int i;
  int developers;

  if (job == NULL) return;
  printf ("=======  Job\n");
  printf ("projectID: %d\n", job->Project.ID);
  if (job->Application != NULL) printf ("application: %s\n", job->Application);
  if (job->Category != NULL) printf ("category: %s\n", job->Category);
  print_person (&job->Contact);
  developers = job->Developers_size / sizeof (job->Developers[0]);
  printf ("%d developers\n", developers);

  for (i = 0; i < developers; ++i)
    print_person (&job->Developers[i]);
  printf ("======= \n");
}

static void
print_helping (helping_t * helping)
{
  int i;
  int jobs = helping->Jobs_size / sizeof (helping->Jobs[0]);
  printf ("%d Jobs registered\n", jobs);
  for (i = 0; i < jobs; ++i)
    print_job (&helping->Jobs[i]);
}

int
main (int argc, char * argv[])
{
  helping_t helping;

#ifdef HAVE_LIBXML2
  LIBXML_TEST_VERSION;
  xmlKeepBlanksDefault (0);
#endif /* HAVE_LIBXML2 */

  char * xml = mr_read_xml_doc (stdin);
  if (NULL == xml)
    printf ("mr_read_xml_doc failed\n");
  else
    {
      memset (&helping, 0, sizeof (helping));
      if (MR_SUCCESS != MR_LOAD_XML (helping_t, xml, &helping))
	printf ("Load failed\n");
      else
	print_helping (&helping);
      MR_FREE_RECURSIVELY (helping_t, &helping);
      MR_FREE (xml);
    }

  /* Clean up everything else before quitting. */
#ifdef HAVE_LIBXML2
  xmlCleanupParser();
#endif /* HAVE_LIBXML2 */
  return (EXIT_SUCCESS);
}
