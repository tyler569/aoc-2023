cmake_minimum_required(VERSION 3.26)
project(aoc_2023)

find_package(CURL REQUIRED)

set(CMAKE_CXX_STANDARD 23)

add_library(aoc aoc.cpp)
target_link_libraries(aoc CURL)
include_directories(.)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)
add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
add_link_options(-fsanitize=address)

foreach (day RANGE 1 7)
    if (day LESS 10)
        set(day "0${day}")
    endif ()
    add_executable(day${day} days/${day}.cpp)
    target_link_libraries(day${day} aoc)
endforeach ()