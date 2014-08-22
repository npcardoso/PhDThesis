#!/bin/bash

set -e
mvn install
cp -v glue-java/target/*-jar-with-dependencies.jar crowbar.jar
mvn -q install:install-file -Dfile=crowbar.jar -DgroupId=io.crowbar.libcrowbar -DartifactId=libcrowbar-jar -Dversion=1.0.0 -Dpackaging=jar