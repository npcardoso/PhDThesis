#!/bin/bash
java -Djava.library.path=lib -cp lib:target/javassist-1.0-SNAPSHOT-jar-with-dependencies.jar $*
