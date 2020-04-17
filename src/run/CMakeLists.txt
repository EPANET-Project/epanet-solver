#
# CMakeLists.txt - CMake configuration file for epanet/src/run
#
# Date Created: 9/17/2019
#
# Author: Michael E. Tryby
#         US EPA - ORD/NRMRL
#

# Set up file groups for exe target
source_group("CLI" FILES main.c)


# Creates the EPANET command line executable
add_executable(runepanet
    main.c
)

target_include_directories(runepanet
    PUBLIC
        include
)

target_link_libraries(runepanet
    PUBLIC
        epanet2
)

set_target_properties(runepanet
    PROPERTIES
        MACOSX_RPATH TRUE
        SKIP_BUILD_RPATH FALSE
        BUILD_WITH_INSTALL_RPATH FALSE
        INSTALL_RPATH "@executable_path/../lib"
        INSTALL_RPATH_USE_LINK_PATH TRUE
)

install(TARGETS runepanet
    DESTINATION  "${TOOL_DIST}"
)


# copy runepanet to build tree for testing
add_custom_command(TARGET runepanet
    POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:runepanet>
            ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/$<TARGET_FILE_NAME:runepanet>
)
