#!/bin/bash -e

BIN_DIR="obj/bin"
TESTS_DIR="tests"
RUNNER_NAME="run.sh"

if [[ $# == 0 ]]; then
	echo "Missing test name" >&2
	exit 1
fi

TEST_NAME=$1

shift

TEST_RUNNER=$PWD/$TESTS_DIR/$TEST_NAME/$RUNNER_NAME

if [[ ! -x $TEST_RUNNER ]]; then
	echo "No test called $TEST_NAME" >&2
	exit 1
fi

cd $BIN_DIR
export INSTR_TCP_PORT=12345
export LD_LIBRARY_PATH=../lib

echo "Starting $TEST_RUNNER" >&2
$TEST_RUNNER $*
