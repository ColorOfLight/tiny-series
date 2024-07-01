# Tiny Series

Implementation for [ssoly's tiny series](https://github.com/ssloy)

## Pre-requisite

- [Install VC Package Manager](https://learn.microsoft.com/vcpkg/get_started/get-started)
- Install CMake
- Set up your `.bashrc` or `.zshrc`:

```txt
# rbenv
export CURL_CA_BUNDLE=$(brew --prefix)/etc/ca-certificates/cert.pem

# vcpkg
export VCPKG_ROOT=/Users/seongho/Documents/vcpkg
export PATH=$VCPKG_ROOT:$PATH
export CMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
```

## How to run

- Build executables with CMake

```zsh
cmake .
make
```

- Run executables in `/bin` folder (ex. `bin/TinyRenderer`)

## Profiling

### Mac

- You can use Xcode Instruments for various profiling.
- You can profile the execution file.
- If you need the further information, please refer to [this blog post](https://www.jviotti.com/2024/01/29/using-xcode-instruments-for-cpp-cpu-profiling.html).
