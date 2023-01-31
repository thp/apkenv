#!/bin/bash
# Preprocess list of header files with Scratchbox preprocessor
# Thomas Perl <m@thp.io>; 2012-10-19

source environment.sh
for file in $*; do
    cat default_includes.h $file | $ANDROID_CPP -I$ANDROID_HEADERS >$file.android
done

