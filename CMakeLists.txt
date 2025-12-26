cmake_minimum_required(VERSION 3.31)
project(coolgame)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

include_directories("src/include")
file(GLOB SOURCE_FILES "src/*.cpp")

# Executable
if (MSVC)
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
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3_ttf::SDL3_ttf SDL3::SDL3)

# Assets
if (APPLE)
    # on Apple targets, the application bundle has a "resources" subfolder where we can place our assets.
    # SDL_GetBasePath() gives us easy access to that location.
    macro(add_resource FILE DIR)
        target_sources(${PROJECT_NAME} PRIVATE ${FILE})
        set_property(SOURCE ${FILE} PROPERTY MACOSX_PACKAGE_LOCATION "Resources/${DIR}")
    endmacro()
    add_resource("${CMAKE_CURRENT_LIST_DIR}/src/assets/cozette.fnt" "assets")
    add_resource("${CMAKE_CURRENT_LIST_DIR}/src/assets/icon.icns" "")
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
    copy_helper("icon.ico")
endif ()

# App icon
if (MSVC)
    # i honestly dont know if this works because i dont have a windows machine and dont care enough to set up an environment to build this game for windwos
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/cool.rc.in" "IDI_ICON1        ICON        DISCARDABLE    \"icon.ico\"\n")
    add_custom_command(
            OUTPUT "cool.rc"
            COMMAND ${CMAKE_COMMAND}
            ARGS -E copy "cool.rc.in" "cool.rc"
            DEPENDS "icon.ico"
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
    target_sources(${PROJECT_NAME} PRIVATE "cool.rc")
elseif (APPLE)
    set(MACOSX_BUNDLE_ICON_FILE "icon")
endif ()
