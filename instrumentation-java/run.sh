#!/bin/bash
java -Djava.library.path=lib-native -cp target/*-jar-with-dependencies.jar $*
