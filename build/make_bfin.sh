#!/bin/sh

make clean
# prebuilt  pjproject
SYSROOT=/home/amneiht/app/toolchain/uClinux/pj

export PKG_CONFIG_PATH=
export PKG_CONFIG_LIBDIR=${SYSROOT}/usr/lib/pkgconfig:${SYSROOT}/usr/local/lib/pkgconfig
export PKG_CONFIG_SYSROOT_DIR=${SYSROOT}

./configure --host=bfin-linux-uclibc --with-name=bfin-linux-uclibc

make
make gen_lib
