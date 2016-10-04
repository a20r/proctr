cmake_minimum_required (VERSION 2.6)
project (proctr CXX)

# Definitions
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_definitions(-std=c++11 -Wall -Wextra -Ofast -march=native
    -flto -fwhole-program -fopenmp)

# Setting up Boost
find_package(
    Boost REQUIRED
    COMPONENTS date_time)

include_directories(${Boost_INCLUDE_DIRS} include)

add_subdirectory(src)
add_subdirectory(test)
