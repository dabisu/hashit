#!/bin/sh

TIME="/usr/bin/time -f %U"

# Do the actual benchmarks. This will run the benchmark
# 14 times. First, three times to have the cache hot and nice,
# then 11 times to get reliable numbers.
do_bmark()
{
    # Discard the three first to have the cache hot.
    echo -n "Running bench_$1 -$2 three times to have the cache hot... "
    for i in 1 2 3 4; do
	$TIME ./bench_$1 -$2 $3 $4 2> tmp/bench_$1_$2 1> /dev/null
	echo -n "$i "
    done

    # Do the benchmark
    echo "Done."
    echo -n "Now running bench_$1 -$2 10 times... "
    for i in 1 2 3 4 5 6 7 8 9 10 ; do
	$TIME ./bench_$1 -$2 $3 $4 2>> tmp/bench_$1_$2 1> /dev/null
	echo -n "$i " 
    done
    echo "Done."
    echo "The mean is `./mean tmp/bench_$1_$2`"
    echo ""
    rm -f tmp/bench_$1_$2
}


# The script begins here

# FIXME! Check the number of arguments

if test $# -ne 2 ; then
    echo "Usage: ./do_benchmark.sh dict_file text_file"
    echo ""
    echo "Both dict_file and text_file should be normal ASCII text files,"
    echo "like e-books. See www.gutenberg.org for suitable e-books."
    echo ""
    echo "The benchmark generates files in tmp/"
    exit 0
fi

install -d tmp
# The STL and glibc benchmarks don't take any special arguments
do_bmark stl 0 $1 $2
#do_bmark glibc 0 $1 $2
do_bmark hashit 0 $1 $2
# Run all the ght benchmarks (different options, see the bench_ght
# usage for descriptions of the different options).
do_bmark ght 1 $1 $2
do_bmark ght 2 $1 $2
#do_bmark ght 3 $1 $2
#do_bmark ght 4 $1 $2
#do_bmark ght 5 $1 $2
#do_bmark ght 6 $1 $2
