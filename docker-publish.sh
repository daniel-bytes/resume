#!/bin/bash

VERSION=$(cat ./version)

# prune old images, otherwise publish takes forever
echo "Pruning old image tags"
docker image ls | grep resume | awk '{print $2}' | grep -v none | grep -v $VERSION | grep -v latest | xargs -I {} docker image rmi danielbytes/resume:{}

echo "Publishing version [$VERSION]"
docker push danielbytes/resume
