#!/bin/bash
set -e

./mvnc package

java -javaagent:target/javassist-1.0-SNAPSHOT-jar-with-dependencies.jar -cp target/classes/ io.crowbar.instrumentation.App 2>&1
