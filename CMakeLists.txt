cmake_minimum_required(VERSION 3.31)
project(coolgame)
set(EXECUTABLE_NAME ${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

## Dependencies
find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
find_package(SDL3_ttf REQUIRED)

include_directories(src/include)
file(GLOB PROJECT_SOURCES "src/*.cpp")

add_executable(${EXECUTABLE_NAME} ${PROJECT_SOURCES})
target_compile_features(${EXECUTABLE_NAME} PRIVATE cxx_std_23)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE SDL3_ttf::SDL3_ttf SDL3::SDL3)
