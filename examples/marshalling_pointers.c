
#include <stdio.h>
#include <stdlib.h>

#include "marshalling_pointers.h"

int main ()
{
  back_end_server_t server1 = {
    .id = 3,
    .address = 0x12341234,
    .port = 80
  };
  back_end_server_t server2 = {
    .id = 11,
    .address = 0x43214321,
    .port = 443
  };

  filter_t filter1 = { 123 };
  filter_t __attribute__ ((unused)) filter2 = { 321 };

  policy_t policy = {
    .server_a = &server1,
    .server_b = &server2,
    .f = &filter1,
    .fallback = &server2
  };

  MR_PRINT ((policy_t, &policy, XML));
  return (EXIT_SUCCESS);
}
