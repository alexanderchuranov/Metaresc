#!/bin/sh

numpasses=3

perform_test()
{
    local numfiles=$1
    local variant=$2
    local optimize=
    
    local buildtime='N/A'
    local filesize='N/A'
    local starttime='N/A'
    
    [ "$variant" = "optimized" ] && CFLAGS="-DMR_MODE=PROTO" || CFLAGS=""
    
    make distclean
    buildtime=$( (time -p CFLAGS=${CFLAGS} make -j NUMFILES=${numfiles} > /dev/null) 2>&1 | grep real | awk '{print $NF;}')
    filesize=$( stat --format '%s' huge_enterprise_app )
    starttime=$( (time -p ./huge_enterprise_app > /dev/null) 2>&1 | grep real | awk '{print $NF;}' )
    
    echo "$numfiles files $variant build $buildtime start $starttime size $filesize"
}

test_all_sizes()
{
    for numfiles in 1 10 50 100
    do
        echo -n "${numfiles}..." > /dev/stderr
        perform_test $numfiles simple
        perform_test $numfiles optimized
    done
    
    echo > /dev/stderr
}

calc_average()
{
    local number=$1
    local variant=$2
    local fieldno=$3
    local numpasses=$4
    
    local formula=$(
        echo -n '(';
        grep -h "${number} files ${variant}" pass* | awk '{print $'"${fieldno}"';}' | tr '\n' '+';
        echo "0)/${numpasses}";
    )
    
    echo $formula | bc -l
}

print_averages()
{
    local number=$1
    local variant=$2
    local numpasses=$3
    
    local avg_build=$(calc_average $number $variant 5 $numpasses)
    local avg_start=$(calc_average $number $variant 7 $numpasses)
    local avg_size=$(calc_average $number $variant 9 $numpasses)
    
    local print_build=$(printf "%.2f" $avg_build)
    local print_start=$(printf "%.2f" $avg_start)
    local print_size=$(printf "%.1f" $( echo "$avg_size / 1024" | bc -l ) )
    
    echo "$number files, $variant, build $print_build, start $print_start, size $print_size k"
}

export LANG=C

for (( pass = 0; $pass < $numpasses; pass = $pass + 1 ));
do
    echo "pass #${pass}" > /dev/stderr
    test_all_sizes > pass${pass}
done

make distclean

tested_numbers=$(cat pass* | cut -d' ' -f1 | sort -n | uniq)

for number in $tested_numbers
do
    print_averages $number simple $numpasses
    print_averages $number optimized $numpasses
done

rm pass*
