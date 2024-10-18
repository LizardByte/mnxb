# Moonlight-XboxOG
Port of Moonlight for the Original Xbox. Unlikely to ever actually work. Do NOT use.


## Build

### Pre Build

1. Install nxdk prerequisites. Then run the following from mingw64 or bash shell:

```bash
nxdk_dir="$(pwd)/third-party/nxdk"
eval "$(${nxdk_dir}/bin/activate -s)"
cd "${nxdk_dir}"
make NXDK_ONLY=y
make tools
```

### Configure

1. Create build directory

   ```bash
   mkdir -p build
   ```

2. Configure the project

   ```bash
   cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="${nxdk_dir}/share/toolchain-nxdk.cmake"
   ```

### Build

```bash
cmake --build build
```
