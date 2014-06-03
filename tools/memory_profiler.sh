#!/bin/zsh

if [[ $# != 2 ]]; then

    exit 1
fi

SLEEP=$1
TARGET=$2

TMP=`mktemp`
echo $TMP

${=TARGET} &
TARGET=$!

TRAPINT() {
    kill -9 $TARGET
}


while true; do
    OUT=`grep Vm -i /proc/$TARGET/status`
    if [[ $? != 0 ]]; then
        break
    fi
    echo $OUT | sed 's/^[^0-9]*\([0-9]*\).*$/\1/' | tr "\\n" "," >> $TMP
    echo 0 >> $TMP
    sleep $1
done

TITLES=`cat /proc/1/status | grep vm -i | sed 's/:.*$//'`

PLOT_EXPR="set autoscale; set datafile separator ','; f(x)=x/1024; plot "
i=1
for t in ${=TITLES}; do
    PLOT_EXPR="$PLOT_EXPR '$TMP' using (f(\$$i)) smooth bezier title '$t',"
    i=$((i+1))
done
PLOT_EXPR="$PLOT_EXPR '$TMP' using (f(\$$i)) smooth bezier title 'foo'"
gnuplot -e "$PLOT_EXPR"
