#!/bin/bash

HTTPD_ROOT=".hroot"

BIG_FILE="$HTTPD_ROOT/big"
SMALL_FILE="$HTTPD_ROOT/small"

HTTPD_EXEC="webfsd"
HTTPD_ARGS="-p 8090 -Fr$HTTPD_ROOT -y10"


[[ ! -e $HTTPD_ROOT ]] && mkdir -p $HTTPD_ROOT
[[ ! -e $BIG_FILE ]] && head -c 200M /dev/zero > $BIG_FILE
[[ ! -e $SMALL_FILE ]] && head -c 200K /dev/zero > $SMALL_FILE

for ((obs=0; obs<20; obs++)); do
  $* $HTTPD_EXEC $HTTPD_ARGS & # 2>/dev/null &
  pid=$!
  sleep 1
  for ((transactions = 0; transactions < 20; transactions++)); do
	curl 127.0.0.1:8090 > /dev/null 2>/dev/null
  done
  echo -n "state" | nc 127.0.0.1 12345 > $obs.log
  kill -9 $pid
  sleep 1
done
#gdb ./$HTTPD_DIR/$HTTPD_EXEC $HTTPD_ARGS
#valgrind -v --leak-check=full ./$HTTPD_DIR/$HTTPD_EXEC $HTTPD_ARGS
