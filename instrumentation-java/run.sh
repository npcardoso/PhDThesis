#!/bin/bash
set -e
mvn package -q


JUNITPATH=~/.m2/repository/junit/junit/4.8.1/junit-4.8.1.jar
AGENT=`echo ./target/*jar-with-dependencies.jar`
TEST=$1
echo "Using $AGENT"

java -cp $JUNITPATH:target/test-classes \
    -XX:+AlwaysLockClassLoader\
    -javaagent:$AGENT=1234 \
    org.junit.runner.JUnitCore \
    io.crowbar.instrumentation.$1
