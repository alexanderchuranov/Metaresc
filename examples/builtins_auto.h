
#include <reslib.h>
                                             // equivalent to the following:
TYPEDEF_STRUCT (builtins_auto_t,             // typedef struct { // some meta-data magic here
		(char, _char),               //   char _char;
		(signed char, _schar),       //   signed char _schar;
		(unsigned char, _uchar),     //   unsigned char _uchar;
		(int, _int),                 //   int _int;
		(unsigned, _uint),           //   unsigned _uint;
		(short, _short),             //   short _ushort;
		(unsigned short, _ushort),   //   unsigned short _ushort;
		(long, _long),               //   long _long;
		(unsigned long int, _ulong), //   unsigned long int _ulong;
		(long long int, _llint),     //   long long int _llint;
		(unsigned long long, _ull),  //   unsigned long long _ull;
		(float, _float),             //   float _float;
		(double, _double),           //   double _double;
		(long double, _ldouble),     //   long double _ldouble;
		)                            // } builtins_auto_t;
