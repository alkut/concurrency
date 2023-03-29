cmake_minimum_required(VERSION 3.24)
project(splitWork)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)

add_executable(splitWork main.cpp)
target_link_libraries(splitWork PUBLIC pthread)

option(ENABLE_TESTS "boost unit test required" OFF)
if (ENABLE_TESTS)
    add_subdirectory(tests)
endif()
