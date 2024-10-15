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
eval "$(./third-party/nxdk/bin/activate -s)"
make

# skip autobuild
echo "skip_autobuild=true" >> "$GITHUB_OUTPUT"
