
#include <stdio.h>
#include <reslib.h>

TYPEDEF_STRUCT (with_f_pointers_t,
                (char*, pfunc, (int))
                )

char* even_odd(int x)
{
  if (x % 2)
  {
    return "odd";
  }
  else {
    return "even";
  }
}

int main()
{
  with_f_pointers_t s = { .pfunc = even_odd };
  int numbers[] = { 1, 4, 7 };
  
  for (int i = 0; i < sizeof(numbers)/sizeof(numbers[0]); ++i)
    printf("%i is %s\n", numbers[i], s.pfunc(numbers[i]));
  
  return 0;
}
