/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <reslib.h>

TYPEDEF_STRUCT (project_t,
		(int, ID)
		)

TYPEDEF_ENUM (status_t,
	      Open,
	      Close,
	      )

TYPEDEF_STRUCT (update_t,
	       (status_t, Status),
	       (char *, Modified),
	       (char *, Salary),
	       )
     
TYPEDEF_STRUCT (person_t,
		(char *, Person),
		(char *, Email),
		(char *, Company),
		(char *, Organisation),
		(char *, Webpage),
		(char *, Snailmail),
		(char *, Phone),
		)

TYPEDEF_STRUCT (job_t,
		(project_t, Project),
		(char *, Application),
		(char *, Category),
		(update_t, Update),
		RARRAY (person_t, Developers),
		(person_t, Contact),
		(char *, Requirements),
		(char *, Skills),
		(char *, Details),
		)

TYPEDEF_STRUCT (helping_t,
		RARRAY (job_t, Jobs),
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
  printf ("projectID: %" SCNd32 "\n", job->Project.ID);
  if (job->Application != NULL) printf ("application: %s\n", job->Application);
  if (job->Category != NULL) printf ("category: %s\n", job->Category);
  print_person (&job->Contact);
  developers = job->Developers.size / sizeof (job->Developers.data[0]);
  printf ("%" SCNd32 " developers\n", developers);

  for (i = 0; i < developers; ++i)
    print_person (&job->Developers.data[i]);
  printf ("======= \n");
}

static void
print_helping (helping_t * helping)
{
  int i;
  int jobs = helping->Jobs.size / sizeof (helping->Jobs.data[0]);
  printf ("%" SCNd32 " Jobs registered\n", jobs);
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
