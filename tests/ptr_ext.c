/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

#include <stdio.h>
#include <stdlib.h>

#include <ptr_ext.h>

int main ()
{
  char * str;
  x_t x;
  y_t y;

  main_data_t main_data;
  static_data_t static_data = { .data = {1, 2, 3, 4} };

  x.y = &y;
  y.z.x = &x;

  main_data.static_data = &static_data;
  main_data.shifted_data1 = (void*)&static_data.data[1];
  main_data.shifted_data2 = (void*)&static_data.data[2];
  main_data.shifted_data3 = (void*)&static_data.data[1];
  main_data.shifted_data4 = (void*)&static_data.data[2];
  
  str = RL_SAVE_XML (x_t, &x);
  if (str)
    {
      printf ("%s\n", str);
      RL_FREE (str);
    }

  str = RL_SAVE_XML (main_data_t, &main_data);
  if (str)
    {
      printf ("%s\n", str);
      RL_FREE (str);
    }

#ifdef HAVE_LIBXML2 
  /* Clean up everything else before quitting. */
  xmlCleanupParser();
#endif /* HAVE_LIBXML2 */
  return (EXIT_SUCCESS);
}
