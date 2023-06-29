#usr/bin/env bash

set -e

ZIG=$(pwd)/../bin/zig
CC="zig cc"
CXX="zig c++"
AR="zig ar"

BUILDDIR=./zig-out
OPT=-O2

mkdir -p ${BUILDDIR}

export MACOSX_DEPLOYMENT_TARGET=10.13

echo "Build libutil.a"

${CXX} ${OPT} -Iinclude -c -o ${BUILDDIR}/util.cxx.o ./src/util.cpp
${CXX} ${OPT} -Iinclude -c -o ${BUILDDIR}/plugin.cxx.o ./src/plugin.cpp
${AR} rcs ${BUILDDIR}/libutil.a ${BUILDDIR}/*.o

rm ${BUILDDIR}/*.o

echo "Build libplugin_cpp.a"
${CXX} ${OPT} -Iinclude -c -o ${BUILDDIR}/plugin_cpp.cxx.o ./src/plugin_cpp.cpp
${AR} rcs ${BUILDDIR}/libplugin_cpp.a ${BUILDDIR}/plugin_cpp*.o

echo "Build libplugin_zig.a"
${ZIG} build
#${ZIG} build-lib --name plugin_zig
#${ZIG} build-lib --cache-dir ${BUILDDIR} --name plugin_zigfoo -fPIC
#${ZIG} build install --prefix ${BUILDDIR} --prefix-lib-dir ${BUILDDIR}
#--cache-dir

echo "Link exe"
mkdir -p ${BUILDDIR}/bin
# -flto isn't supported yet
#clang++ ${OPT} -flto -o ${BUILDDIR}/bin/testzig ./src/main.cpp ${BUILDDIR}/libutil.a ./zig-out/lib/libzigtest.a
#clang++ ${OPT} -g -o ${BUILDDIR}/bin/testzig -Iinclude -Llib -L${BUILDDIR} -L./zig-out/lib ./src/main.cpp -llua -lutil -lplugin_cpp -lzigtest
clang++ ${OPT} -mmacosx-version-min=13.1 -g -o ${BUILDDIR}/bin/testzig -Iinclude -Llib -L${BUILDDIR} -L./zig-out/lib ./src/main.cpp -llua -lutil -lplugin_cpp -lplugin_zig

echo "Compile done!"
echo ""
