#!/bin/bash

if $(docker ps | grep -q resume); then
    docker rm -vf resume
fi

docker run -d --name resume -p 3000:3000 danielbytes/resume
