if [[ $# < 3 ]]; then
  echo "Usage: $0 <input_dir> <first> <second>"
  exit 1
fi

OUTPUT_DIR=`mktemp -d`
INPUT=$1
if [[ ! -d $1 ]]; then
  INPUT_DIR=`dirname $1`
else
  INPUT_DIR=$1
fi


FIRST=$2
SECOND=$3
IFS="
"
for TEST_CASE in `command ls -1 $INPUT`; do
  echo "-----$TEST_CASE-----"
  FIRST_OUT=$OUTPUT_DIR"/first"
  SECOND_OUT=$OUTPUT_DIR"/second"

  $FIRST < $INPUT_DIR"/"$TEST_CASE > $FIRST_OUT
  $SECOND < $INPUT_DIR"/"$TEST_CASE > $SECOND_OUT
  
  diff $FIRST_OUT $SECOND_OUT > /dev/null
  
  if [[ $? != 0  ]]; then
	echo -n "Entering inspection shell"
	cd $OUTPUT_DIR
	$SHELL
	rm -rf $OUTPUT_DIR
	break
  fi
  rm -f $FIRST_OUT $SECOND_OUT
done
