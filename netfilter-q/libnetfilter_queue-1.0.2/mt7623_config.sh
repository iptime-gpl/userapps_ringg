#!/bin/sh

ToolChainPrefix="arm-linux-"
ToolChainBin="/opt/buildroot-gcc483_arm/usr/bin"
ToolChainInclude="/opt/buildroot-gcc483_arm/usr/bin/include"
ToolChainLib="/opt/buildroot-gcc483_arm/usr/arm-buildroot-linux-uclibcgnueabi/sysroot/lib"

ConfigOpt="--host=arm-linux --target=arm-linux --build=i686-pc-linux"

CC=${ToolChainPrefix}gcc
CXX=${ToolChainPrefix}c++
CPP=${ToolChainPrefix}cpp
LD=${ToolChainPrefix}ld
AR=${ToolChainPrefix}ar
STRIP=${ToolChainPrefix}strip
RANLIB=${ToolChainPrefix}ranlib
CFLAGS="-mcpu=cortex-a7 -O2 -fomit-frame-pointer -pipe -Os"

rm -f configure
tar xf configure.tar

env CC=$CC \
env CFLAGS="$CFLAGS" \
env LIBNFNETLINK_CFLAGS="-I${USERAPPS_ROOT}/netfilter-q/libnfnetlink-1.0.1/include" \
env LIBNFNETLINK_LIBS="-L${USERAPPS_ROOT}/netfilter-q/libnfnetlink-1.0.1/src/.libs" \
env LIBMNL_CFLAGS="-I${USERAPPS_ROOT}/netfilter-q/libmnl-1.0.4/include" \
env LIBMNL_LIBS="-L${USERAPPS_ROOT}/netfilter-q/libmnl-1.0.4/src/.libs" \
./configure  --host=arm-linux --target=arm-linux --build=i686-pc-linux

