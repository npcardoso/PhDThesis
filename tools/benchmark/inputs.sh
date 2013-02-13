#!/bin/bash
GENERATE=$(dirname $(readlink -e $0))/../generate.py


OUT=
SIZES=
PARAMS=
ITERATIONS=1

[[ $# > 4 || $# == 0 ]] && echo -e "Usage:\t$0 <OUT_DIR> <SIZES> [<ITERATIONS> <PARAMS>]" && exit 1

[[ $# > 3 ]] && PARAMS=`echo -e $4 | tr " " "\n"`
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
	FNAME=$OUT/${FNAME//[[:space:]]}
	echo $FNAME
	$GENERATE $c $t $PARAMS > $FNAME
  done
done
