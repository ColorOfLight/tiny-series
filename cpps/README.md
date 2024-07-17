# Tiny Series C++

## Pre-requisite

### Emscripten

- Download & Install

```zsh
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
```

### VCpkg

- Download & Install

```zsh
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
```

## How to build

### Executable

```zsh
mkdir build && cd build
cmake ..
make
bin/TinyRenderer
```

### WASM

```zsh
mkdir build-wasm && cd build-wasm
cmake -DWASM=ON -DCMAKE_TOOLCHAIN_FILE=../emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake ..
make
````
