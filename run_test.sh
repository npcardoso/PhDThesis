#!/bin/bash -e

BIN_DIR="obj/install/bin"
EXAMPLES_DIR="examples"
[[ -z $RUNNER_NAME ]] && RUNNER_NAME="run.sh"

if [[ $# == 0 ]]; then
	echo "Missing example name" >&2
	exit 1
fi

EXAMPLE_NAME=$1

shift

EXAMPLE_RUNNER=$PWD/$EXAMPLES_DIR/$EXAMPLE_NAME/$RUNNER_NAME

if [[ ! -x $EXAMPLE_RUNNER ]]; then
	echo "No example called $EXAMPLE_NAME" >&2
	exit 1
fi

cd $BIN_DIR
export INSTR_TCP_PORT=12345
export LD_LIBRARY_PATH=../lib

echo "Starting $EXAMPLE_RUNNER" >&2
$EXAMPLE_RUNNER $*
