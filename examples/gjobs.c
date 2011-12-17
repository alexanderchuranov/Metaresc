/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <reslib.h>

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
		RARRAY (person_t, Developers),
		(person_t, Contact),
		string_t Requirements,
		string_t Skills,
		string_t Details,
		)

TYPEDEF_STRUCT (helping_t, RARRAY (job_t, Jobs))

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
  developers = job->Developers.size / sizeof (job->Developers.data[0]);
  printf ("%d developers\n", developers);

  for (i = 0; i < developers; ++i)
    print_person (&job->Developers.data[i]);
  printf ("======= \n");
}

static void
print_helping (helping_t * helping)
{
  int i;
  int jobs = helping->Jobs.size / sizeof (helping->Jobs.data[0]);
  printf ("%d Jobs registered\n", jobs);
  for (i = 0; i < jobs; ++i)
    print_job (&helping->Jobs.data[i]);
}

int
main (int argc, char * argv[])
{
  helping_t helping;

#ifdef HAVE_LIBXML2
  LIBXML_TEST_VERSION;
  xmlKeepBlanksDefault (0);
#endif /* HAVE_LIBXML2 */

  char * str = rl_read_xml_doc (stdin);
  if (!str)
    printf ("rl_read_xml_doc failed\n");
  else
    {
      memset (&helping, 0, sizeof (helping));
      if (0 == RL_LOAD_XML (helping_t, str, &helping))
	printf ("Load failed\n");
      else
	print_helping (&helping);
      RL_FREE_RECURSIVELY (helping_t, &helping);
      RL_FREE (str);
    }

  /* Clean up everything else before quitting. */
#ifdef HAVE_LIBXML2
  xmlCleanupParser();
#endif /* HAVE_LIBXML2 */
  return (EXIT_SUCCESS);
}
