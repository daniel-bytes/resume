#!/bin/bash

docker build -f ./Dockerfile-Test -t danielbytes/resume-test .

if $(docker ps -a | grep -q resume-test); then
    docker rm -vf resume-test
fi

docker run --privileged --name resume-test danielbytes/resume-test
