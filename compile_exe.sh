#usr/bin/env bash

set -e

#ZIG=$(pwd)/../bin/zig
ZIG=zig
CC="zig cc"
CXX="zig c++"
AR="zig ar"

BUILDDIR=./zig-out
OPT=-O2

mkdir -p ${BUILDDIR}

export MACOSX_DEPLOYMENT_TARGET=10.13

export ZIG_GLOBAL_CACHE_DIR=${BUILDDIR}
export ZIG_LOCAL_CACHE_DIR=${BUILDDIR}

echo "Build libutil.a"

${CXX} ${OPT} -Iinclude -c -o ${BUILDDIR}/util.cxx.o ./src/util.cpp
${CXX} ${OPT} -Iinclude -c -o ${BUILDDIR}/plugin.cxx.o ./src/plugin.cpp
${AR} rcs ${BUILDDIR}/libutil.a ${BUILDDIR}/*.o

rm ${BUILDDIR}/*.o

echo "Build libplugin_cpp.a"
${CXX} ${OPT} -Iinclude -c -o ${BUILDDIR}/plugin_cpp.cxx.o ./src/plugin_cpp.cpp
${AR} rcs ${BUILDDIR}/libplugin_cpp.a ${BUILDDIR}/plugin_cpp*.o

echo "Build libplugin_zig.a"
${ZIG} build-obj -fPIC -O ReleaseFast -l c -I. -femit-bin=${BUILDDIR}/plugin_zig.o src/plugin.zig
${AR} rcs ${BUILDDIR}/libplugin_zig.a ${BUILDDIR}/plugin_zig*.o

echo "Link exe"
mkdir -p ${BUILDDIR}/bin
clang++ ${OPT} -mmacosx-version-min=13.1 -g -o ${BUILDDIR}/bin/testzig -Iinclude -Llib -L${BUILDDIR} -L./zig-out/lib ./src/main.cpp -lutil -lplugin_cpp -lplugin_zig

echo "Compile done!"
echo ""
