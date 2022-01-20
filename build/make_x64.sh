#!/bin/sh

make clean
./configure --with-name=x64
make
make gen_lib