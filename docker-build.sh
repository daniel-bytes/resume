#!/bin/bash
set -e

VERSION=$(cat ./version)
echo "Version $VERSION"

docker build -t danielbytes/resume --build-arg ARG_VERSION=$VERSION .
docker tag danielbytes/resume danielbytes/resume:latest
docker tag danielbytes/resume danielbytes/resume:$VERSION
