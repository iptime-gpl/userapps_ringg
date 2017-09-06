#!/bin/sh

ToolChainPrefix="/home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/msdk-linux-"
ToolChainBin="/home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827"
ToolChainInclude="/home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/include"
ToolChainLib="/home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/lib"

ConfigOpt="--host=mips-linux --target=mips-linux --build=i686-pc-linux"

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
env LIBNFNETLINK_CFLAGS="-I${USERAPPS_ROOT}/netfilter-q/libnfnetlink-1.0.1/include" \
env LIBNFNETLINK_LIBS="-L${USERAPPS_ROOT}/netfilter-q/libnfnetlink-1.0.1/src/.libs" \
env LIBMNL_CFLAGS="-I${USERAPPS_ROOT}/netfilter-q/libmnl-1.0.4/include" \
env LIBMNL_LIBS="-L${USERAPPS_ROOT}/netfilter-q/libmnl-1.0.4/src/.libs" \
./configure  --host=mips-linux --target=mips-linux

