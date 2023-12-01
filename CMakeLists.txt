cmake_minimum_required(VERSION 3.26)
project(aoc_2023)

find_package(CURL REQUIRED)

set(CMAKE_CXX_STANDARD 23)

add_library(aoc aoc.cpp)
target_link_libraries(aoc CURL)

include_directories(${CMAKE_SOURCE_DIR})
add_executable(aoc_2023_1 days/01.cpp)
target_link_libraries(aoc_2023_1 aoc)