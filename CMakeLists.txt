cmake_minimum_required(VERSION 3.31)
project(coolgame)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# CPM
file(
        DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.41.0/CPM.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
        EXPECTED_HASH SHA256=e570f03806b9aae2082ca5b950a9e6b3b41ad56972a78a876aedcaad16653116
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

set(LIBS)
set(SDL_TEST OFF)

## Dependencies
# SDL
CPMAddPackage("gh:libsdl-org/SDL#release-3.2.10")
list(APPEND LIBS SDL3::SDL3)

# SDL_image
CPMAddPackage("gh:libsdl-org/SDL_image#release-3.2.4")
list(APPEND LIBS SDL3_image::SDL3_image)

# SDL_ttf
CPMAddPackage("gh:libsdl-org/SDL_ttf#release-3.2.2")
list(APPEND LIBS SDL3_ttf::SDL3_ttf)

include_directories(src/include)

## Executables
add_executable(coolgame src/main.cpp src/button.cpp
        src/util.cpp
        src/player.cpp
        src/vector.cpp
        src/timer.cpp)
target_compile_features(coolgame PRIVATE cxx_std_23)
target_link_libraries(coolgame PRIVATE ${LIBS})
