#!/bin/sh

numfiles=$1

generate_header()
{
    local fileno=$1
    local name="header_${fileno}.h"
    
    : > ${name}
    
    (
        echo
        echo "#ifndef _METARESC_HUGE_ENTERPRISE_APP_TEST_HEADER_${fileno}_INCLUDED_"
        echo "#define _METARESC_HUGE_ENTERPRISE_APP_TEST_HEADER_${fileno}_INCLUDED_"
        echo
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
    local name="file_${fileno}.c"
    
    : > ${name}
    
    (
        echo
        echo "#include \"header_${fileno}.h\""
        echo '#include "main.h"'
        echo
        echo "void f${fileno}(void)"
        echo '{'
        echo "  struct_${fileno}_t s;"
        echo '  s.a = 3;'
        echo '}'
        echo
    ) >> ${name}
}

generate_main()
{
    (
        echo
        echo '#include <stdio.h>'
        echo '#include "main.h"'
        echo
        echo
        echo 'int main()'
        echo '{'
        echo '  puts("It has started!");'
        echo '  return (0);'
        echo '}'
        echo
    ) > main.c
}

generate_prefix_of_main_h()
{
    (
        echo
        echo "#ifndef _METARESC_HUGE_ENTERPRISE_APP_TEST_HEADER_MAIN_INCLUDED_"
        echo "#define _METARESC_HUGE_ENTERPRISE_APP_TEST_HEADER_MAIN_INCLUDED_"
        echo
    ) > main.h
}

generate_include_for_main_h()
{
    local fileno=$1
    
    echo "#include \"header_${fileno}.h\"" >> main.h
}

generate_suffix_of_main_h()
{
    (
        echo
        echo '#endif'
    ) >> main.h
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
