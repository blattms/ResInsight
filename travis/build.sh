#!/usr/bin/env bash
set -e

pushd . > /dev/null
mkdir build
cd build
cmake  ../
make
popd > /dev/null
