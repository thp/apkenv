#!/bin/bash
# Extract function prototypes from header files
# Thomas Perl <m@thp.io>; 2012-10-19

for file in $*; do
    echo "*** $file ***"
    rm -f $file.h $file.missing
    cproto -x <$file >tmp
    for symbol in $(cat $(dirname $file)/$(basename $file .includes.android)); do
        grep '\<'$symbol'\>(' <tmp >>$file.h || (echo $symbol >>$file.missing)
    done
    rm -f tmp
done

