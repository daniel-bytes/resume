#!/bin/bash

if $(docker ps | grep -q resume); then
    docker rm -vf resume
fi

docker run -it --name resume -p 3000:3000 danielbytes/resume


## production:
# docker rm -vf resume && docker run -d --name resume -p 80:3000 danielbytes/resume:$VERSION
