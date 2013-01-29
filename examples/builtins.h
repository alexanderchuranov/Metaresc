
#include <metaresc.h>

TYPEDEF_STRUCT (builtins_t,

                (char, _char),              // The type char,

                (signed char, _schar),      // the signed
                (short int, _short),
                (int, _int),
                (long int, _long),
                (long long int, _llint),

                (_Bool, _bool),             // and unsigned integer types,
                (unsigned char, _uchar),
                (unsigned int, _uint),
                (unsigned short int, _ushort),
                (unsigned long int, _ulong),
                (unsigned long long int, _ull),

                (float, _float),            // and the floating types
                (double, _double),
                (long double, _ldouble),
                (float _Complex, _fcompl),
                (double _Complex, _dcompl),
                (long double _Complex, _ldcompl)
                )                           // are collectively called
                                            // the basic types.
                                            //
                                            // ISO/IEC 9899:1999, §6.2.5 (14)
