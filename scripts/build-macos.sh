#!/bin/bash

set -e

ARCHITECTURES="x86_64;arm64"

SRC_DIR=".."
BUILD_DIR="../build"

cmake \
    -DCMAKE_INSTALL_PREFIX="_OUT" \
    -DCMAKE_OSX_DEPLOYMENT_TARGET='10.10.1' \
    -DCMAKE_OSX_ARCHITECTURES=${ARCHITECTURES} \
    --log-level=WARNING \
    -B"${BUILD_DIR}"  \
    "${SRC_DIR}"

make -C"${BUILD_DIR}" -j32
