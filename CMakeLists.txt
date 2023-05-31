cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_C_COMPILER "clang-15")
set(CMAKE_CXX_COMPILER "clang++-15")
set(CMAKE_C_STANDARD 20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS ON)

project(metal VERSION 0.1.0 LANGUAGES CXX C)

find_package(Catch2 3 REQUIRED)
find_package(fmt REQUIRED)

include_directories(.)

add_compile_options(-fsanitize=address -fexperimental-library)
add_link_options(-fsanitize=address)

add_library(dual metal/Parameter.cpp)

add_executable(test_expression tests/ExpressionTest.cpp)
target_link_libraries(test_expression PRIVATE dual Catch2::Catch2WithMain fmt)

add_executable(test_dual tests/DualTest.cpp)
target_link_libraries(test_dual PRIVATE dual Catch2::Catch2WithMain)

add_executable(test_parameter tests/ParameterTest.cpp)
target_link_libraries(test_parameter PRIVATE dual Catch2::Catch2WithMain)

add_executable(test_small_vector tests/SmallVectorTest.cpp)
target_link_libraries(test_small_vector PRIVATE dual Catch2::Catch2WithMain)

add_executable(test_scalar_gradient tests/ScalarGradientTest.cpp)
target_link_libraries(test_scalar_gradient PRIVATE dual Catch2::Catch2WithMain fmt)

include(CTest)
include(Catch)
catch_discover_tests(test_expression)
catch_discover_tests(test_dual)
catch_discover_tests(test_parameter)
catch_discover_tests(test_small_vector)
catch_discover_tests(test_scalar_gradient)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
