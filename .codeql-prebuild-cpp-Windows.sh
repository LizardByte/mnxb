# install dependencies for C++ analysis
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
  "mingw-w64-x86_64-gcc"
  "mingw-w64-x86_64-llvm"
  "mingw-w64-x86_64-clang"
  "mingw-w64-x86_64-lld"
)
pacman -S --noconfirm "${dependencies[@]}"

# build
nxdk_dir="$(pwd)/third-party/nxdk"
eval "$(${nxdk_dir}/bin/activate -s)"
cd "${nxdk_dir}"
make NXDK_ONLY=y
make tools

cd "${GITHUB_WORKSPACE}"
mkdir -p build
cmake -DCMAKE_TOOLCHAIN_FILE="${nxdk_dir}/share/toolchain-nxdk.cmake" -B build -S .
cmake --build build

# skip autobuild
echo "skip_autobuild=true" >> "$GITHUB_OUTPUT"
