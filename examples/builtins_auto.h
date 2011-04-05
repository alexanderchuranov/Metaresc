
#include <reslib.h>
                                        // equivalent to the following:
#undef RL_TYPE_NAME                     //
#define RL_TYPE_NAME builtins_auto_t    // typedef struct
RL_TYPEDEF_STRUCT ()                    // {
  RL_AUTO (char, _char)                 //   char _char;
  RL_AUTO (signed char, _schar)         //   signed char _schar;
  RL_AUTO (unsigned char, _uchar)       //   unsigned char _uchar;
  RL_AUTO (int, _int)                   //   int _int;
  RL_AUTO (unsigned, _uint)             //   unsigned _uint;
  RL_AUTO (short, _short)               //   short _ushort;
  RL_AUTO (unsigned short, _ushort)     //   unsigned short _ushort;
  RL_AUTO (long, _long)                 //   long _long;
  RL_AUTO (unsigned long int, _ulong)   //   unsigned long int _ulong;
  RL_AUTO (long long int, _llint)       //   long long int _llint;
  RL_AUTO (unsigned long long, _ull)    //   unsigned long long _ull;
  RL_AUTO (float, _float)               //   float _float;
  RL_AUTO (double, _double)             //   double _double;
  RL_AUTO (long double, _ldouble)       //   long double _ldouble;
RL_END_STRUCT ()                        // } builtins_auto_t;
                                        //
                                        // as always, some meta-data magic here
#undef RL_MODE
