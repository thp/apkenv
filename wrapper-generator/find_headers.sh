#!/bin/bash
# Find headers for functions using manpages
# Thomas Perl <m@thp.io>; 2012-10-19

for file in $*; do
    rm -f $file.includes
    for symbol in $(cat $file); do
        man -S 2,3 $symbol | grep -o '#include <.*>' >>$file.includes.tmp
    done
    cat $file.includes.tmp | sort -u >$file.includes
    rm -f $file.includes.tmp
done

