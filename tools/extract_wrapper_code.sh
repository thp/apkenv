awk '/   0:/ {do_print=1} NF==0 {do_print=0} do_print==1 {print}'
