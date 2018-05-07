cmake_minimum_required(VERSION 2.8)

project(MutablePriorityQueue)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-O2 -std=c++11")

find_package(PkgConfig)
pkg_check_modules(GTEST REQUIRED gtest_main)
find_package(benchmark)

include_directories(.)

add_executable(tst_mpq "test/tst_mpq.cpp")
target_link_libraries(tst_mpq ${GTEST_LIBRARIES} pthread)

add_executable(bm_mpq "benchmark/bm_mpq.cpp")
target_link_libraries(bm_mpq benchmark pthread)
