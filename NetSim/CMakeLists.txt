cmake_minimum_required(VERSION 3.19)
project(sieci)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -Wpedantic -pedantic-errors)

include_directories(include)

set(SOURCE_FILES
        src/factory.cpp
        src/helpers.cpp
        src/nodes.cpp
        src/package.cpp
        src/reports.cpp
        src/storage_types.cpp
        src/simulation.cpp
        )

add_executable(sieci main.cpp ${SOURCE_FILES})

set(SOURCE_FILES_TESTS
        test/test_Factory.cpp
        test/test_factory_io.cpp
        test/test_nodes.cpp
        test/test_package.cpp
        test/test_reports.cpp
        test/test_simulate.cpp
        test/test_storage_types.cpp
        )

add_executable(sieci_test ${SOURCE_FILES} ${SOURCE_FILES_TESTS} test/main_gtest.cpp)

target_compile_definitions(sieci_test PUBLIC EXERCISE_ID=5)

target_include_directories(sieci_test PUBLIC
        googletest-master/googlemock/include
        googletest-master/googletest/include
        mocks
        )

add_subdirectory(googletest-master)

target_link_libraries(sieci_test gmock)

