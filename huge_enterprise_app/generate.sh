#!/bin/sh

numfiles=$1
optimized=${2:-NO}

generate_header()
{
    local fileno=$1
    local optimized=$2
    local name="header_${fileno}.h"
    
    : > ${name}
    
    (
        echo
        echo "#ifndef _METARESC_HUGE_ENTERPRISE_APP_TEST_HEADER_${fileno}_INCLUDED_"
        echo "#define _METARESC_HUGE_ENTERPRISE_APP_TEST_HEADER_${fileno}_INCLUDED_"
        echo
        
        if [ "$optimized" = "YES" ]
        then
            echo '#ifndef RL_MODE'
            echo '# define RL_MODE PROTO'
            echo '#endif'
            echo
        fi
        
        echo '#include <reslib.h>'
        echo
        echo "TYPEDEF_STRUCT (struct_${fileno}_t,"
	echo '    (int, a),'
        echo '    (long, b),'
        echo '    (void*, c),'
        echo '    )'
        echo
        echo "void f${fileno}(void);"
        echo
        echo '#endif // guardian'
    ) >> ${name}
}

generate_source()
{
    local fileno=$1
    local optimized=$2
    local name="file_${fileno}.c"
    
    : > ${name}
    
    (
        echo
        echo "#include \"header_${fileno}.h\""
        echo
        
        if [ "$optimized" = "YES" ]
        then
            echo '#undef RL_MODE'
            echo '#define RL_MODE DESC'
            echo "#include \"header_${fileno}.h\""
            echo
        fi
        
        echo "void f${fileno}(void)"
        echo '{'
        echo "  struct_${fileno}_t s;"
        echo '  s.a = 3;'
        echo '}'
        echo
    ) >> ${name}
}

generate_prefix_of_main()
{
    (
        echo
        echo '#include <stdio.h>'
        echo
    ) > main.c
}

generate_suffix_of_main()
{
    (
        echo
        echo 'int main()'
        echo '{'
        echo '  puts("It has started!");'
        echo '}'
        echo
    ) >> main.c
}

generate_include_for_main()
{
    local fileno=$1
    
    echo "#include \"header_${fileno}.h\"" >> main.c
}

generate_prefix_of_main

fileno=0;
while [ "$fileno" -lt "$numfiles" ]
do
    printno=$(printf "%05u" $fileno)
    generate_header $printno $optimized
    generate_source $printno $optimized
    generate_include_for_main $printno
    fileno=$(( $fileno + 1 ))
done;

generate_suffix_of_main
