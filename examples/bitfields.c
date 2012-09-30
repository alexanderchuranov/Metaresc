
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bitfields.h"

int main ()
{
  date_t today;
  const time_t time_stamp = time (NULL);
  struct tm * break_down_time = localtime (&time_stamp);

  memset (&today, 0, sizeof(today));
  today.year = break_down_time->tm_year + 1900;
  today.month = break_down_time->tm_mon;
  today.day = break_down_time->tm_mday;
  today.hour = break_down_time->tm_hour;
  today.minute = break_down_time->tm_min;

  char * xml = MR_SAVE_XML (date_t, &today);

  printf ("sizeof (date_t) = %zd\n", sizeof (date_t));
  if (NULL == xml)
    printf ("Serialization failed\n");
  else
    {
      printf ("%s", xml);
      MR_FREE (xml);
    }
  
  return (EXIT_SUCCESS);
}
