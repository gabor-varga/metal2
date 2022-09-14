cmake_minimum_required(VERSION 3.0.0)
project(metal VERSION 0.1.0 LANGUAGES CXX C)

find_package(Catch2 3 REQUIRED)

set(CMAKE_C_STANDARD 20)
set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(dual tests/DualTest.cpp)
target_link_libraries(dual PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(dual)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
