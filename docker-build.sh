#!/bin/bash

VERSION=$(cat ./version)

docker build -t danielbytes/resume .
docker tag danielbytes/resume danielbytes/resume:latest
docker tag danielbytes/resume danielbytes/resume:$VERSION
