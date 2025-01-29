#
# CMakeLists.txt - CMake configuration file for tests/outfile
#
# Date Created: 9/17/2019
#
# Author: Michael E. Tryby
#         US EPA - ORD/NRMRL
#


add_executable(test_output
    test_output.cpp
    )

target_compile_features(test_output
    PUBLIC
        cxx_generalized_initializers
    )

target_link_libraries(test_output
    ${Boost_LIBRARIES}
    epanet-output
    )

set_target_properties(test_output
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
