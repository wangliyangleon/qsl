#!/bin/bash -x
# just for convenience
autoreconf -i
./configure
make clean
make -j4
make check
