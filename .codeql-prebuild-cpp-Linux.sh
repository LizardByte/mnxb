# install dependencies for C++ analysis
set -e

# install dependencies
dependencies=(
  "bison"
  "build-essential"
  "clang"
  "cmake"
  "flex"
  "git"
  "lld"
  "llvm"
)
sudo apt-get update
sudo apt-get install --no-install-recommends -y "${dependencies[@]}"

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
