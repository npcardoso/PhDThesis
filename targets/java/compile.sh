#!/bin/bash
set -e

mvn -q $* | grep 'ERROR.*:\[' | sed '1!G;h;$!d' | sort |uniq
