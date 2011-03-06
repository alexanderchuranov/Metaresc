/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

#include <reslib.h>

#undef RL_TYPE_NAME
#define RL_TYPE_NAME project_t
RL_TYPEDEF_STRUCT ()
  RL_INT32 (ID)
RL_END_STRUCT ()

#undef RL_TYPE_NAME
#define RL_TYPE_NAME status_t
RL_TYPEDEF_ENUM ()
  RL_ENUM_DEF (Open)
  RL_ENUM_DEF (Close)
RL_END_ENUM ()

#undef RL_TYPE_NAME
#define RL_TYPE_NAME update_t
RL_TYPEDEF_STRUCT ()
  RL_ENUM (status_t, Status)
  RL_STRING (Modified)
  RL_STRING (Salary)
RL_END_STRUCT ()
     
#undef RL_TYPE_NAME
#define RL_TYPE_NAME person_t
RL_TYPEDEF_STRUCT ()
  RL_STRING (Person)
  RL_STRING (Email)
  RL_STRING (Company)
  RL_STRING (Organisation)
  RL_STRING (Webpage)
  RL_STRING (Snailmail)
  RL_STRING (Phone)
RL_END_STRUCT ()

#undef RL_TYPE_NAME
#define RL_TYPE_NAME job_t
RL_TYPEDEF_STRUCT ()
  RL_STRUCT (project_t, Project)
  RL_STRING (Application)
  RL_STRING (Category)
  RL_STRUCT (update_t, Update)
  RL_RARRAY (person_t, Developers)
  RL_STRUCT (person_t, Contact)
  RL_STRING (Requirements)
  RL_STRING (Skills)
  RL_STRING (Details)
RL_END_STRUCT ()

#undef RL_TYPE_NAME
#define RL_TYPE_NAME helping_t
RL_TYPEDEF_STRUCT ()
  RL_RARRAY (job_t, Jobs)
RL_END_STRUCT ()

#undef RL_MODE
