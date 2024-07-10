project(TinyRenderer)

# Include directories
include_directories(include)

# Add source files
file(GLOB_RECURSE TINY_RENDERER_SOURCES src/*.cpp include/*.h)

if (WASM)
  # add_executable(test_exe wasm/test.cpp)
  set_target_properties(test_exe PROPERTIES LINK_FLAGS "-O1 -s EXPORTED_RUNTIME_METHODS=cwrap,ccall -s MODULARIZE=1 -s -s EXPORT_ES6=1 -s ENVIRONMENT=web --bind")
else ()
  add_executable(TinyRenderer ${TINY_RENDERER_SOURCES})

  # Link libraries (if any, using vcpkg or others)
  # target_link_libraries(Project1 PRIVATE ...)
endif()
