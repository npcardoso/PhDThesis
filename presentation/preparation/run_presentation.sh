#!/bin/bash

set -e

cd `dirname $0`

PRESENTATION=../main.pdf

echo "Setting Screen"
./set_screen.sh
xbacklight -set 100

echo "Launching presentation"
exec pdfpc -g -s -n right $PRESENTATION $@
