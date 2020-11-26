#!/bin/bash
set -e

if [ $# -eq 0 ]; then
  echo "No arguments supplied, first argument should be the verion number"
  exit 1
fi

VERSION=$1

docker pull danielbytes/resume:$VERSION

if $(docker ps -a | grep -q resume); then
  echo "Removing old container"
  docker rm -vf resume
fi

echo "Running new container at version $VERSION"

# Run container with caching enabled
# and certificate and .well-known volumes mapped for lets-encrypt support
docker run \
  -d \
  --name resume \
  -p 80:3080 \
  -p 443:3443 \
  -v /root/app/public/.well-known:/app/public/.well-known \
  -v /root/app/certs:/app/private/certs \
  -v /usr/local/share/ca-certificates:/usr/local/share/ca-certificates \
  -e TRACE=$TRACE \
  -e CACHE_FILES=true \
  -e CACHE_TEMPLATES=true \
  -e SSL_ENABLED=true \
  -e SSL_CERT_PATH=/app/private/certs/etc/letsencrypt/live/daniel-battaglia.com/cert.pem \
  -e SSL_KEY_PATH=/app/private/certs/etc/letsencrypt/live/daniel-battaglia.com/privkey.pem \
  danielbytes/resume:$VERSION

echo "Pruning old image tags"
docker image ls | grep resume | awk '{print $2}' | grep -v none | grep -v $VERSION | grep -v latest | xargs -I {} docker image rmi danielbytes/resume:{}
