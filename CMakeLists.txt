cmake_minimum_required(VERSION 3.31)
project(coolgame)

add_executable(${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3_ttf::SDL3_ttf SDL3::SDL3)

## Dependencies
find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
find_package(SDL3_ttf REQUIRED)

include_directories(src/include)
file(GLOB SOURCES "src/*.cpp")

# Assets
if (APPLE)
    set(RESOURCES "${CMAKE_SOURCE_DIR}/src/assets")

    target_sources(${PROJECT_NAME} PRIVATE ${SOURCES} ${RESOURCES})

    set_target_properties(${PROJECT_NAME} PROPERTIES
            MACOSX_BUNDLE TRUE
            RESOURCE "${RESOURCES}")
    #    target_link_libraries(${PROJECT_NAME} PRIVATE objc stdc++)
else ()
    macro(copy_helper filename)
        # for platforms that do not have a good packaging format, all we can do is copy the assets to the process working directory.
        set(outname "${CMAKE_BINARY_DIR}/$<CONFIGURATION>/${filename}")
        add_custom_command(POST_BUILD
                TARGET "${PROJECT_NAME}"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_LIST_DIR}/src/assets/${filename}" "${outname}"
        )
    endmacro()
    copy_helper("cozette.fon")
endif ()
