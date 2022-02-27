#!/bin/bash

if [ ! -d "build" ]; then
  mkdir build
fi

cd build
cmake ..
make clean
make -j 32
make $1
cd ..
