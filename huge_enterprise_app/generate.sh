#!/bin/sh

numfiles=$1

generate_header()
{
    local fileno=$1
    cat > header_${fileno}.h <<EOF
#ifndef _METARESC_HUGE_ENTERPRISE_APP_TEST_HEADER_${fileno}_INCLUDED_
#define _METARESC_HUGE_ENTERPRISE_APP_TEST_HEADER_${fileno}_INCLUDED_
#include <reslib.h>

TYPEDEF_STRUCT (struct_${fileno}_t,
		(int, a),
		(long, b),
		(void*, c),
		)
void f${fileno}(void);
#endif // guardian
EOF
}

generate_source()
{
    local fileno=$1
    cat > file_${fileno}.c <<EOF
#include "header_${fileno}.h"
#include "main.h"
        
void f${fileno}(void)
{
  struct_${fileno}_t s;
  s.a = 0;
}
EOF
}

generate_main()
{
    cat > main.c <<EOF
#include <stdio.h>
#include "main.h"
        
        
int main()
{
  puts("It has started!");
  return (0);
}
EOF
}

generate_prefix_of_main_h()
{
    cat > main.h <<EOF
#ifndef _METARESC_HUGE_ENTERPRISE_APP_TEST_HEADER_MAIN_INCLUDED_
#define _METARESC_HUGE_ENTERPRISE_APP_TEST_HEADER_MAIN_INCLUDED_

EOF
}

generate_include_for_main_h()
{
    local fileno=$1
    
    echo "#include \"header_${fileno}.h\"" >> main.h
}

generate_suffix_of_main_h()
{
    cat >> main.h <<EOF

#endif
EOF
}

generate_prefix_of_main_h

for (( fileno = 1; $fileno <= $numfiles; fileno = $fileno + 1 ));
do
    printno=$(printf "%05u" $fileno)
    generate_header $printno
    generate_source $printno
    generate_include_for_main_h $printno
done;

generate_suffix_of_main_h

generate_main
