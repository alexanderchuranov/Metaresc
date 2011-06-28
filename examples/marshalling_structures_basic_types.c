
#include "marshalling_structures_basic_types.h"

int main()
{
  basics_t b = {
    .c = 'Z',
    .uli = 123,
    .fl = {
      .f = 0.3f,
      .ld = 3.14,
      .dc = 7.42
    }
  };
  
  char const* xml = RL_SAVE_XML( basics_t, &b);
  puts(xml);
  return 0;
}
