GENERATE=$(dirname $(readlink -e $0))"/spectra.py"
MIN_COMP=1
MAX_COMP=100
MIN_TRAN=1
MAX_TRAN=100
ITER=1

case $# in
  6)
	MIN_COMP=$2
	MAX_COMP=$3
	MIN_TRAN=$4
	MAX_TRAN=$5
	ITER=$6
	;;
  5)
	MIN_COMP=$2
	MAX_COMP=$3
	MIN_TRAN=$4
	MAX_TRAN=$5
	;;
  3)
	MAX_COMP=$2
	MAX_TRAN=$3
	;;
  1)
	;;
  *)
	echo -e "Usage:\t$0 <EXEC> \n\t$0 <EXEC> <MAX_COMP> <MAX_TRAN>\n\t$0 <EXEC> <MIN_COMP> <MAX_COMP> <MIN_TRAN> <MAX_TRAN> [<N_ITER>]"
	exit 1
esac

if [[ -x $1 ]]; then
  EXEC=$(readlink $1)
else
  EXEC=$1
fi

function min(){
  if [[ $1 < $2 ]]; then
	echo $1
  else
	echo $2
  fi
}

function max(){
  if [[ $1 > $2 ]]; then
	echo $1
  else
	echo $2
  fi
}

function benchmark(){
	POSTFIX=$2"x"$3".txt"
	IN="in.$POSTFIX"
	OUT="out.$POSTFIX"
	ERR="err.$POSTFIX"

	if [[ ! -f $IN ]]; then
	  $GENERATE $2 $3 $4 > $IN
	fi
	echo "------------------------------------------------------" >> $OUT
	echo "------------------------------------------------------" >> $ERR
	$1 < $IN >> $OUT 2>> $ERR
}


MIN=`min $MIN_COMP $MIN_TRAN`
MAX=`max $MAX_COMP $MAX_TRAN`

for ((it = 1; it <= $ITER; it++)); do
  for ((i = MIN; i <= MAX; i++)); do
	for ((j = MIN; j <= i; j++)); do
	  echo "Iteration $it ($i $j)"
	  [[ $j -ge $MIN_COMP && $j -le $MAX_COMP && $i -ge $MIN_TRAN && $i -le $MAX_TRAN ]] && benchmark $EXEC $j $i $gen_opts
	  if [[ $i != $j ]]; then
		[[ $i -ge $MIN_COMP && $i -le $MAX_COMP && $j -ge $MIN_TRAN && $j -le $MAX_TRAN ]] && benchmark $EXEC $i $j $gen_opts
	  fi
	done
  done
done
