#!/bin/bash
# Preprocess list of header files with Scratchbox preprocessor
# Thomas Perl <m@thp.io>; 2012-10-19

for file in $*; do
    cat default_includes.h $file | cpp >$file.harmattan
done

