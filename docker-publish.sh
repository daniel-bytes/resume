#!/bin/bash

echo "Publishing version [$(cat ./version)]"

docker push danielbytes/resume
