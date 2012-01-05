
#include <stdio.h>
#include "marshalling_arrays.h"

int main()
{
  with_array_t wa = { .a = { 11, 17, 23 } };
  
  char * xml = RL_SAVE_XML( with_array_t, &wa );
  puts(xml);
  RL_FREE(xml);
  
  return 0;
}
