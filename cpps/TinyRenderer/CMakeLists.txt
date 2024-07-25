project(TinyRenderer)

# Include directories
include_directories(include third-party)

# Add source files
file(GLOB_RECURSE TINY_RENDERER_SOURCES src/*.cpp third-party/*.cpp include/*.h third-parth/*.h)

if (WASM)
  add_executable(TinyRenderer ${TINY_RENDERER_SOURCES} main/wasm.cpp)
  set_target_properties(TinyRenderer PROPERTIES LINK_FLAGS "-O1 -s WASM=1 -s EXPORTED_RUNTIME_METHODS=cwrap,ccall,FS -s MODULARIZE=1 -s EXPORT_ES6=1 -s ENVIRONMENT=web -s ALLOW_MEMORY_GROWTH=1 --bind")
else ()
  add_executable(TinyRenderer ${TINY_RENDERER_SOURCES} main/default.cpp)
endif()
