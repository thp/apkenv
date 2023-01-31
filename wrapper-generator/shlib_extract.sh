#!/bin/bash
# Extract required and provided symbols from a JNI shared library
# Thomas Perl <m@thp.io>; 2012-10-19

source environment.sh

mkdir -p output

for file in $*; do
    BASENAME=$(basename $file .so)
    REQUIRED=output/$BASENAME.required
    PROVIDED=output/$BASENAME.provided
    JNI_INTF=output/$BASENAME.jni
    $ANDROID_OBJDUMP -T $file | grep '\*UND\*' | sort -u | awk '{ print $5 }' >$REQUIRED
    $ANDROID_OBJDUMP -T $file | grep 'DF .text' | grep -v '\*UND\*' | awk '{ print $6 }' | sort -u  >$PROVIDED
    egrep '^(JNI_Init|Java_)' $PROVIDED >$JNI_INTF
done

