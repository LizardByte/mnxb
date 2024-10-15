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
eval "$(./third-party/nxdk/bin/activate -s)"
make

# skip autobuild
echo "skip_autobuild=true" >> "$GITHUB_OUTPUT"
