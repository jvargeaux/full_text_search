#! /bin/bash

./autogen.sh &&
./configure --prefix=`realpath $PWD/../build` --with-lz4 --disable-apache-arrow --enable-optimize --with-mecab-config=`realpath $PWD/../build/bin/mecab-config` &&
make -j `nproc` &&
make check &&
make install
