#!/bin/bash
set -e

mvn -q $* \
    | grep 'ERROR.*:\[' \
    | sed -e "s'\[ERROR\] ''" \
    -e's/://g' \
    -e's/\[/,/' \
    -e's/]/,/g' \
    -e"s'$PWD/src/\([a-zA-Z]*\)/java/io/crowbar'[\1]'" \
    -e "s' error''" \
    | sort -t',' -k1,1dr -k2,2nr -k 3,3n \
    | sed -e 's/,/\t[/' -e 's/,/:/' -e 's/,/]\t/' \
    | uniq
