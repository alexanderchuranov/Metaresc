/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

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

