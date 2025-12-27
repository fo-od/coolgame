cmake_minimum_required(VERSION 3.31)
project(coolgame)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

include_directories("src/include")
file(GLOB SOURCE_FILES "src/*.cpp")

# App icon
include(${CMAKE_SOURCE_DIR}/cmake/AddIconToBinary.cmake)
AddIconToBinary(SOURCE_FILES ICONS ${CMAKE_CURRENT_LIST_DIR}/src/assets/icon.ico ${CMAKE_CURRENT_LIST_DIR}/src/assets/icon.icns)

# Executable
if (WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES})
elseif (APPLE)
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCE_FILES})
else ()
    add_executable(${PROJECT_NAME} ${SOURCE_FILES})
endif ()

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

## Dependencies
find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
find_package(SDL3_ttf REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL3_ttf::SDL3_ttf SDL3::SDL3)

# Assets (thanks https://github.com/Ravbug/sdl3-sample)
if (APPLE)
    # on Apple targets, the application bundle has a "resources" subfolder where we can place our assets.
    # SDL_GetBasePath() gives us easy access to that location.
    macro(add_resource FILE)
        target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/src/assets/${FILE}")
        set_property(SOURCE "${CMAKE_CURRENT_LIST_DIR}/src/assets/${FILE}" PROPERTY MACOSX_PACKAGE_LOCATION "Resources/assets")
    endmacro()
    add_resource("cozette.fnt")
else ()
    macro(copy_helper filename)
        # for platforms that do not have a good packaging format, all we can do is copy the assets to the process working directory.
        set(outname "${CMAKE_BINARY_DIR}/$<CONFIGURATION>/assets/${filename}")
        add_custom_command(POST_BUILD
                TARGET "${PROJECT_NAME}"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_LIST_DIR}/src/assets/${filename}" "${outname}"
        )
    endmacro()
    copy_helper("cozette.fnt")
endif ()
