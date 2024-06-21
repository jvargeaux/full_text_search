#! /bin/bash

locate libiconv.h > /dev/null &&
mkdir $PWD/../../build;
./configure --prefix `realpath $PWD/../../build` &&
make -j `nproc` &&
make check &&
make install
