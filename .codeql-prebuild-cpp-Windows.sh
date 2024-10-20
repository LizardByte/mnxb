#!/bin/bash
set -e

# update pacman
pacman --noconfirm -Syu

# install dependencies
dependencies=(
  "make"
  "cmake"
  "git"
  "bison"
  "flex"
  "mingw-w64-ucrt-x86_64-gcc"
  "mingw-w64-ucrt-x86_64-llvm"
  "mingw-w64-ucrt-x86_64-clang"
  "mingw-w64-ucrt-x86_64-lld"
)
pacman -S --noconfirm "${dependencies[@]}"

# build
export NXDK_DIR="$(pwd)/third-party/nxdk"
eval "$(${NXDK_DIR}/bin/activate -s)"
cd "${NXDK_DIR}"
make NXDK_ONLY=y
make tools

cd "${GITHUB_WORKSPACE}"
mkdir -p build
cmake -DCMAKE_TOOLCHAIN_FILE="${NXDK_DIR}/share/toolchain-nxdk.cmake" -B build -S .
cmake --build build

# skip autobuild
echo "skip_autobuild=true" >> "$GITHUB_OUTPUT"
