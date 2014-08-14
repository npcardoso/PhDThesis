#!/bin/bash




RESOURCES_DIR=target/volatile-resources/linux-x86-64
LIBDIAG_FILE=../diagnostic-native/install/lib/libdiag.so

set -e

cd diagnostic-native
scons

cd ../instrumentation-java

mvn clean -q

mkdir -vp $RESOURCES_DIR
cp -v $LIBDIAG_FILE $RESOURCES_DIR

mvn install -q

cp target/*-jar-with-dependencies.jar ../crowbar.jar
