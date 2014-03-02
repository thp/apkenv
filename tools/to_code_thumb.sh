#!/bin/sh

while read line
do
    opstart=2;
    opend=2;
    line=( $line );
    if [[ "${line[2]}" =~ "^[0-9a-f]+$" ]]; then
        opend=3;
    fi
    echo -ne "((int16_t*)wrapper_addr)[helper++] = 0x"${line[1]}";"
    echo -ne " // "
    for i in `seq $opend ${#line[@]}`
    do
        echo -ne "${line[$i]} "
    done
    if [[ $opend -eq 2 ]]; then
        echo -ne "\n"
    else
        echo -ne "\n((int16_t*)wrapper_addr)[helper++] = 0x"${line[2]}"; // above command continued\n"
    fi
done

