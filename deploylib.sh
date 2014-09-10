#!/bin/bash
set -e
mvn deploy
if [ -f 'crowbar.jar' ]
then
	mvn -q deploy:deploy-file -Durl='sftp://sftp.dc2.gpaas.net/vhosts/crowbar.io/htdocs/maven' -DrepositoryId='crowbar-io' -Dfile=crowbar.jar -DgroupId=io.crowbar.libcrowbar  -DartifactId=libcrowbar-jar -Dversion=1.0.0-SNAPSHOT -Dpackaging=jar
fi
