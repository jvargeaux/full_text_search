#! /bin/bash

root_dir=$(realpath `dirname $0`/..)
CC=gcc python3.11 $root_dir/pymodule/setup/setup.py build_ext
python3.11 $root_dir/pymodule/setup/setup.py install