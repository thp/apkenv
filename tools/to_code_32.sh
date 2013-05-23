#!/bin/sh
awk '{ print "((int32_t*)wrapper_addr)[helper++] = 0x"$2"; // "$3" "$4" "$5" "$6" "$7" "$8" "$9" "$10" "$11}' -
