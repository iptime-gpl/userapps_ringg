#!/bin/sh

ToolChainPrefix="mips-openwrt-linux-uclibc-"

ConfigOpt="--host=mips-linux --target=mips-linux"

CC=${ToolChainPrefix}gcc
CXX=${ToolChainPrefix}c++
CPP=${ToolChainPrefix}cpp
LD=${ToolChainPrefix}ld
AR=${ToolChainPrefix}ar
STRIP=${ToolChainPrefix}strip
RANLIB=${ToolChainPrefix}ranlib
CFLAGS="-fomit-frame-pointer -pipe -Os"

env CC=$CC \
env CFLAGS="$CFLAGS" \
./configure  --host=mips-linux --target=mips-linux

