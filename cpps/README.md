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

## How to build

### Executable

```zsh
mkdir build && cd build
cmake .. --preset=default
make
bin/TinyRenderer
```

### WASM

```zsh
mkdir build-wasm && cd build-wasm
cmake .. --preset=wasm
make
````
