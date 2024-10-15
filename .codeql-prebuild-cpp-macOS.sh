# install dependencies for C++ analysis
set -e

# install dependencies
dependencies=(
  "cmake"
  "coreutils"
  "lld"
  "llvm"
)
brew install "${dependencies[@]}"

# build
eval "$(./third-party/nxdk/bin/activate -s)"
make

# skip autobuild
echo "skip_autobuild=true" >> "$GITHUB_OUTPUT"
