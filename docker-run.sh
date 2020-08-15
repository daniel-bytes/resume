#!/bin/bash

if $(docker ps -a | grep -q resume); then
    docker rm -vf resume
fi

docker run \
    -d --name resume \
    -p 3000:3000 \
    -e TRACE=true \
    -e SHOW_ADDRESS=true \
    -e SHOW_PROJECTS=false \
    -e CACHE_FILES=true \
    -e CACHE_TEMPLATES=true \
    --privileged \
    danielbytes/resume
