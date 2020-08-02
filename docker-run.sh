#!/bin/bash

if $(docker ps -a | grep -q resume); then
    docker rm -vf resume
fi

docker run \
    -d --name resume \
    -p 3000:3000 \
    -e SHOW_ADDRESS=true \
    -e TRACE=true \
    -e CACHE_FILES=true \
    -e CACHE_TEMPLATES=true \
    --privileged \
    danielbytes/resume

## production:
# VERSION=xxx
# docker pull danielbytes/resume:$VERSION
# docker rm -vf resume && docker run -d --name resume -p 80:3000 -e CACHE_FILES=true -e CACHE_TEMPLATES=true danielbytes/resume:$VERSION
