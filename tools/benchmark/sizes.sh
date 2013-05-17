BASE_DIR=$(dirname $(readlink -e $0))

. $BASE_DIR/utils.sh

MIN_COMP=1
MAX_COMP=40
MIN_TRAN=1
MAX_TRAN=40

case $# in
  4)
	MIN_COMP=$1
	MAX_COMP=$2
	MIN_TRAN=$3
	MAX_TRAN=$4
	;;
  2)
	MAX_COMP=$1
	MAX_TRAN=$2
	;;
  0)
	;;
  *)
	echo -e "Usage:\t$0 \n\t$0 <MAX_COMP> <MAX_TRAN>\n\t$0 <MIN_COMP> <MAX_COMP> <MIN_TRAN> <MAX_TRAN>"
	exit 1
esac

MIN=`min $MIN_COMP $MIN_TRAN`
MAX=`max $MAX_COMP $MAX_TRAN`

#echo $MIN $MAX $MIN_COMP  $MAX_COMP $MIN_TRAN $MAX_TRAN

for ((i = MIN; i <= MAX; i++)); do
  for ((j = MIN; j <= i; j++)); do
	[[ $j -ge $MIN_COMP && $j -le $MAX_COMP && $i -ge $MIN_TRAN && $i -le $MAX_TRAN ]] && echo $j $i
	if [[ $i != $j ]]; then
	  [[ $i -ge $MIN_COMP && $i -le $MAX_COMP && $j -ge $MIN_TRAN && $j -le $MAX_TRAN ]] && echo $i $j
	fi
  done
done

