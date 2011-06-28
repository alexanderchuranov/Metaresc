
#include <stdio.h>
#include "marshalling_enumerations.h"

int main()
{
  screen_t cons25 = {
    .width = 80,
    .height = 25,
    .bg = BLACK,
    .fg = WHITE
  };
  
  char const* xml = RL_SAVE_XML( screen_t, &cons25 );
  puts(xml);
  
  // TODO: loading unmatched enum value
  return 0;
}
