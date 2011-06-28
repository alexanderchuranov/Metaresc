
#include <stdio.h>
#include "marshalling_pointers.h"

int main()
{
  back_end_server server1 = {
    .id = 3,
    .address = 0x12341234,
    .port = 80
  };
  
  back_end_server server2 = {
    .id = 11,
    .address = 0x43214321,
    .port = 443
  };
  
  filter filter1 = { 123 };
  filter filter2 = { 321 };
  
  policy p = {
    .server_a = &server1,
    .server_b = &server2,
    .f = &filter1,
    .fallback = &server2
  };
  
  char const* xml = RL_SAVE_XML( policy, &p );
  puts(xml);
  RL_FREE(xml);
  
  (void)filter2;
  
  return 0;
}
