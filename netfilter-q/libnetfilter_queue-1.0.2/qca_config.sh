#!/bin/sh

ConfigOpt="--host=${ARCH}-linux --target=${ARCH}-linux --build=i686-pc-linux"

CC=${ToolChainPrefix}gcc
CXX=${ToolChainPrefix}c++
CPP=${ToolChainPrefix}cpp
LD=${ToolChainPrefix}ld
AR=${ToolChainPrefix}ar
STRIP=${ToolChainPrefix}strip
RANLIB=${ToolChainPrefix}ranlib
CFLAGS="-fomit-frame-pointer -pipe -Os"

rm -f configure
tar xf configure.tar

env CC=$CC \
env CFLAGS="$CFLAGS" \
env LIBNFNETLINK_CFLAGS="-I${USERAPPS_ROOT}/netfilter-q/libnfnetlink-1.0.1/include" \
env LIBNFNETLINK_LIBS="-L${USERAPPS_ROOT}/netfilter-q/libnfnetlink-1.0.1/src/.libs" \
env LIBMNL_CFLAGS="-I${USERAPPS_ROOT}/netfilter-q/libmnl-1.0.4/include" \
env LIBMNL_LIBS="-L${USERAPPS_ROOT}/netfilter-q/libmnl-1.0.4/src/.libs" \
./configure  --host=arm-linux --target=${ARCH}-linux
