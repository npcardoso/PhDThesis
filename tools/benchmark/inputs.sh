#!/bin/bash
GENERATE=$(dirname $(readlink -e $0))/../generate.py


OUT=
SIZES=
PARAMS=
ITERATIONS=1

[[ $# > 4 || $# == 0 ]] && echo -e "Usage:\t$0 <OUT_DIR> <SIZES> [<ITERATIONS> <PARAMS>]" && exit 1

[[ $# > 3 ]] && PARAMS=$4
[[ $# > 2 ]] && ITERATIONS=$3
[[ $# > 1 ]] && SIZES=$2
[[ $# > 0 ]] && OUT=$1

[[ ! -d $OUT ]] && mkdir $OUT

IFS="
"
for size in $SIZES; do
  size=($(echo $size | tr " " "\n"))
  c=${size[0]} 
  t=${size[1]}

  for ((i=1; i<=ITERATIONS; i++)); do
	FNAME="in."$c"x"$t"."$i".txt"
	$GENERATE $c $t $PARAMS > $OUT/${FNAME//[[:space:]]}
  done
done
