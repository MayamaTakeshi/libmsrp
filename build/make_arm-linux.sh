#!/bin/sh
mdir=$(dirname "$0")
cd $mdir
make distclean
./configure --host=arm-linux --with-name=arm-linux 
make