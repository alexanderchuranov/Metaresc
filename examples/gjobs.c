/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gjobs.h>
#include <gjobs.h>

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

static void
free_person (person_t * person)
{
  if (person == NULL) return;
  if (person->Person) RL_FREE (person->Person);
  if (person->Email) RL_FREE (person->Email);
  if (person->Company) RL_FREE (person->Company);
  if (person->Organisation) RL_FREE (person->Organisation);
  if (person->Snailmail) RL_FREE (person->Snailmail);
  if (person->Webpage) RL_FREE (person->Webpage);
  if (person->Phone) RL_FREE (person->Phone);
}

static void
free_job (job_t * job)
{
  int i;
  int developers;

  if (job == NULL) return;
  if (job->Application != NULL) RL_FREE (job->Application);
  if (job->Category != NULL) RL_FREE (job->Category);
  if (job->Requirements != NULL) RL_FREE (job->Requirements);
  if (job->Skills != NULL) RL_FREE (job->Skills);
  if (job->Details != NULL) RL_FREE (job->Details);

  if (job->Update.Modified != NULL) RL_FREE (job->Update.Modified);
  if (job->Update.Salary != NULL) RL_FREE (job->Update.Salary);
    
  free_person (&job->Contact);
  developers = job->Developers.size / sizeof (job->Developers.data[0]);
  for (i = 0; i < developers; ++i)
    free_person (&job->Developers.data[i]);
  RL_FREE (job->Developers.data);
}

static void
free_helping (helping_t * helping)
{
  int i;
  int jobs = helping->Jobs.size / sizeof (helping->Jobs.data[0]);
  for (i = 0; i < jobs; ++i)
    free_job (&helping->Jobs.data[i]);
  RL_FREE (helping->Jobs.data);
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
      free_helping (&helping);
      RL_FREE (str);
    }

  /* Clean up everything else before quitting. */
#ifdef HAVE_LIBXML2
  xmlCleanupParser();
#endif /* HAVE_LIBXML2 */
  return (EXIT_SUCCESS);
}
