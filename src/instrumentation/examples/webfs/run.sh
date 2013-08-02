#!/bin/bash

HTTPD_ROOT=".hroot"

BIG_FILE="$HTTPD_ROOT/big"
SMALL_FILE="$HTTPD_ROOT/small"

HTTPD_EXEC="webfsd"
HTTPD_ARGS="-p 8090 -Fr$HTTPD_ROOT -y10"


[[ ! -e $HTTPD_ROOT ]] && mkdir -p $HTTPD_ROOT
[[ ! -e $BIG_FILE ]] && head -c 200M /dev/zero > $BIG_FILE
[[ ! -e $SMALL_FILE ]] && head -c 200K /dev/zero > $SMALL_FILE

$* $HTTPD_EXEC $HTTPD_ARGS
#gdb ./$HTTPD_DIR/$HTTPD_EXEC $HTTPD_ARGS
#valgrind -v --leak-check=full ./$HTTPD_DIR/$HTTPD_EXEC $HTTPD_ARGS
