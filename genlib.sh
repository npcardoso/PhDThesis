#!/bin/bash

set -e
mvn install
cp -v glue-java/target/*-jar-with-dependencies.jar crowbar.jar
