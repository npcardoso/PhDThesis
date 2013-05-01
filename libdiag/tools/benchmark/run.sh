#!/bin/bash
OUT=
EXEC=
INPUTS=

[[ $# != 3 ]] && echo -e "Usage:\t$0 <OUT> <EXEC> <INPUTS>" && exit 1

OUT=$1
EXEC=$2
INPUTS=$3

[[ ! -d $OUT ]] && mkdir $OUT

IFS="
"
for input in $INPUTS; do
  basename=`basename $input`
  output=$OUT/out.$basename
  $EXEC $input $output #>/dev/null 2>/dev/null
done
