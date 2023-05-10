cmake_minimum_required(VERSION 3.0.0)
project(metal VERSION 0.1.0 LANGUAGES CXX C)

find_package(Catch2 3 REQUIRED)

set(CMAKE_C_STANDARD 20)
set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

add_library(dual metal/Parameter.cpp)

add_executable(test_dual tests/DualTest.cpp)
target_link_libraries(test_dual PRIVATE dual Catch2::Catch2WithMain)

add_executable(test_parameter tests/ParameterTest.cpp)
target_link_libraries(test_parameter PRIVATE dual Catch2::Catch2WithMain)

add_executable(test_small_vector tests/SmallVectorTest.cpp)
target_link_libraries(test_small_vector PRIVATE dual Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(test_dual)
catch_discover_tests(test_parameter)
catch_discover_tests(test_small_vector)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
