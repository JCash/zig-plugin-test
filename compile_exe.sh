#usr/bin/env bash

set -e

CC="zig cc"
CXX="zig c++"
AR="zig ar"

BUILDDIR=./zig-out
OPT=-O2

mkdir -p ${BUILDDIR}

echo "Build libutil.a"
${CXX} ${OPT} -c -o ${BUILDDIR}/util.o ./src/util.cpp
${AR} rcs ${BUILDDIR}/libutil.a ${BUILDDIR}/*.o

echo "Build libzigtest.a"
zig build --verbose

echo "Link exe"
mkdir -p ${BUILDDIR}/bin
# -flto isn't supported yet
#clang++ ${OPT} -flto -o ${BUILDDIR}/bin/testzig ./src/main.cpp ${BUILDDIR}/libutil.a ./zig-out/lib/libzigtest.a
clang++ ${OPT} -g -o ${BUILDDIR}/bin/testzig ./src/main.cpp ${BUILDDIR}/libutil.a ./zig-out/lib/libzigtest.a
