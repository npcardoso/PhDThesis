#!/bin/bash
set -e


JUNITPATH=~/.m2/repository/junit/junit/4.8.1/junit-4.8.1.jar
JAR=`echo ./target/*jar-with-dependencies.jar`
if [[ $# == 0 ]]; then
    CLASS=rest.DiagnosticServer
else
    CLASS=$1
fi

echo "Using $JAR:$CLASS"

java -cp $JUNITPATH:target/test-classes:$JAR \
    io.crowbar.$CLASS
