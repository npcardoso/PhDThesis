MIN_COMP=1
MAX_COMP=100
MIN_TRAN=1
MAX_TRAN=100

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
  *)
	echo -e "Usage:\t$0 <MAX_COMP> <MAX_TRAN>\n\t$0 <MIN_COMP> <MAX_COMP> <MIN_TRAN> <MAX_TRAN>"
	exit 1
esac
 
function collect() {
	POSTFIX=$1"x"$2".txt"
	ERR="err.$POSTFIX"
	
	grep "^Run Time" $ERR | sed 's/.*: \([0-9]*\.[0-9]*\).*/\1/' | sed ':a;N;$!ba;s/\n/;/g'
}

for ((i = MIN_COMP; i <= MAX_COMP; i++)); do
  for ((j = MIN_TRAN; j <= MAX_TRAN; j++)); do
	echo -n $i"x"$j";"
  	collect $i $j
  done
done
