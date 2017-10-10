cmake_minimum_required(VERSION 3.1)
project(sec_random)

set(CMAKE_CXX_STANDARD 11)

enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/src)

add_subdirectory(src)
add_subdirectory(tests)
