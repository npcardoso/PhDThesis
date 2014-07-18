#!/bin/bash
set -e

java -javaagent:target/javassist-1.0-SNAPSHOT-jar-with-dependencies.jar \
    -cp target/javassist-1.0-SNAPSHOT.jar \
    io.crowbar.sandbox.App 2>&1
