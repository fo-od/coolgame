# from https://bolu.dev/programming/2020/07/18/Cmake-icons.html
function(AddIconToBinary appsources)
    set(options)
    set(oneValueArgs OUTFILE_BASENAME)
    set(multiValueArgs ICONS)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    foreach (icon ${ARG_ICONS})
        get_filename_component(icon_full ${icon} ABSOLUTE)
        get_filename_component(icon_type ${icon_full} EXT)
        get_filename_component(icon_name ${icon_full} NAME_WE)

        if (APPLE)
            set(icon_full_output ${CMAKE_CURRENT_BINARY_DIR}/${icon_name}.icns)
            configure_file(${icon_full} ${icon_full_output} COPYONLY)
            set(MACOSX_BUNDLE_ICON_FILE ${icon_name}.icns PARENT_SCOPE)
            set(${appsources} "${${appsources}};${icon_full_output}" PARENT_SCOPE)
            set_source_files_properties(${icon_full_output} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
            return()
        endif ()
        if (WIN32)
            set(icon_full_output ${CMAKE_CURRENT_BINARY_DIR}/${icon_name}.ico)
            configure_file(${icon_full} ${icon_full_output} COPYONLY)
            file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/${icon_name}.rc.in" "IDI_ICON1        ICON        DISCARDABLE    \"${icon_name}.ico\"\n")
            add_custom_command(
                    OUTPUT "${icon_name}.rc"
                    COMMAND ${CMAKE_COMMAND}
                    ARGS -E copy "${icon_name}.rc.in" "${icon_name}.rc"
                    DEPENDS "${icon_name}.ico"
                    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
            set(${appsources} "${${appsources}};${icon_name}.rc" PARENT_SCOPE)
            return()
        endif ()

    endforeach ()

    return()
endfunction()