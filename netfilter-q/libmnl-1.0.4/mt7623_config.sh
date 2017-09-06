#!/bin/sh

ToolChainPrefix="arm-linux-"

CC=${ToolChainPrefix}gcc
CXX=${ToolChainPrefix}c++
CPP=${ToolChainPrefix}cpp
LD=${ToolChainPrefix}ld
AR=${ToolChainPrefix}ar
STRIP=${ToolChainPrefix}strip
RANLIB=${ToolChainPrefix}ranlib
CFLAGS="-mcpu=cortex-a7 -O2 -fomit-frame-pointer -pipe -Os"

env CC=$CC \
env CFLAGS="$CFLAGS" \
./configure  --host=arm-linux --target=arm-linux --build=i686-pc-linux --enable-shared --enable-static

