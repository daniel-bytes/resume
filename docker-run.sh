#!/bin/bash

if $(docker ps -a | grep -q resume); then
    docker rm -vf resume
fi

# Run container with no caching and /app/public mapped to local public files
docker run \
    -d --name resume \
    -p 3000:3000 \
    -e TRACE=true \
    -e SHOW_ADDRESS=true \
    -e SHOW_PROJECTS=true \
    -e CACHE_FILES=false \
    -e CACHE_TEMPLATES=false \
    --privileged \
    --volume=`pwd`/SimpleServ/public:/app/public \
    danielbytes/resume
