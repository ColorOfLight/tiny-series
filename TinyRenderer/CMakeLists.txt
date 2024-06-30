project(TinyRenderer)

# Include directories
include_directories(include)

# Add source files
file(GLOB_RECURSE TINY_RENDERER_SOURCES src/*.cpp include/*.h)

# Create executable
add_executable(TinyRenderer ${TINY_RENDERER_SOURCES})

# Link libraries (if any, using vcpkg or others)
# target_link_libraries(Project1 PRIVATE ...)
