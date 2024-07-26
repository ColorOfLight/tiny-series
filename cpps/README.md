# Tiny Series C++

- Include projects: [TinyRenderer](./TinyRenderer/README.md)

## Pre-requisite

### CMake

- [CMake Download & Install](https://cmake.org/download/)

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

## Profiling

### Mac

- You can use Xcode Instruments for various profiling.
- You can profile the execution file.
- If you need the further information, please refer to [this blog post](https://www.jviotti.com/2024/01/29/using-xcode-instruments-for-cpp-cpu-profiling.html).

## Other Platforms

- TBD

## Lint

- Install [cpplint](https://github.com/cpplint/cpplint)
- Set commands executable: `chmod +x commands/*.sh`
- Run `lint-all.sh`: `commands/lint-all.sh`
