#!/bin/bash

set -e
mvn install
cp -v instrumentation-java/target/*-jar-with-dependencies.jar crowbar.jar
mvn -q install:install-file -Dfile=crowbar.jar -DgroupId=io.crowbar.libcrowbar -DartifactId=libcrowbar-jar -Dversion=1.0.0-SNAPSHOT -Dpackaging=jar