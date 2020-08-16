#!/bin/bash

pushd ./SimpleServ

echo "Building tests"
scons -Q test=1

echo "Running tests"
./build/tests && scons -Q test=1 -c

popd
