#!/bin/bash
set -e
java -version
java -javaagent:target/javassist-1.0-SNAPSHOT-jar-with-dependencies.jar=1234 \
    -cp target/javassist-1.0-SNAPSHOT.jar \
    io.crowbar.sandbox.App 2>&1
