#!/bin/bash

cd build
cmake ..
make clean
make -j 32
make $1
cd ..
