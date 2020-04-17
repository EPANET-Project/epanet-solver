#
# CMakeLists.txt - CMake configuration file for epanet/src/outfile
#
# Created: 9/17/2019
# Updated: 12/12/2019
#
# Author: Michael E. Tryby
#         US EPA - ORD/NRMRL
#


# configure file groups
set(EPANET_OUT_PUBLIC_HEADERS
    include/epanet_output.h
    include/epanet_output_enums.h
    include/epanet_output_export.h
)


# the binary output file API
add_library(epanet-output
    SHARED
        epanet_output.c
        $<TARGET_OBJECTS:shared_objs>
)

target_include_directories(epanet-output
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${INCLUDE_DIST}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

set_target_properties(epanet-output
    PROPERTIES
        MACOSX_RPATH TRUE
        SKIP_BUILD_RPATH FALSE
        BUILD_WITH_INSTALL_RPATH FALSE
        INSTALL_RPATH "@loader_path"
        INSTALL_RPATH_USE_LINK_PATH FALSE
)

include(GenerateExportHeader)
generate_export_header(epanet-output
    BASE_NAME
        epanet_output
    EXPORT_MACRO_NAME
        EXPORT_OUT_API
    EXPORT_FILE_NAME
        epanet_output_export.h
    STATIC_DEFINE
        SHARED_EXPORTS_BUILT_AS_STATIC
)

file(COPY ${CMAKE_CURRENT_BINARY_DIR}/epanet_output_export.h
    DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/include
)


install(TARGETS epanet-output
    EXPORT
        epanet-outputTargets
    RUNTIME
        DESTINATION "${TOOL_DIST}"
    LIBRARY
        DESTINATION "${LIBRARY_DIST}"
    ARCHIVE
        DESTINATION "${LIBRARY_DIST}"
)

# Install target import scripts so other cmake projects can use epanet libraries
install(EXPORT epanet-outputTargets
    DESTINATION
        "${CONFIG_DIST}"
    FILE
        epanet-output-config.cmake
)

install(FILES ${EPANET_OUT_PUBLIC_HEADERS}
    DESTINATION
        "${INCLUDE_DIST}"
)


# copy epanet-output to build tree for testing
if(BUILD_TESTS)
    add_custom_command(TARGET epanet-output POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:epanet-output>
            ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/$<TARGET_FILE_NAME:epanet-output>
    )
endif()
