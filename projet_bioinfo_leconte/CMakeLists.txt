cmake_minimum_required (VERSION 3.11)
project(bioInfo)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++2a")

set(BW_BUILD
    "src/bw-build.cc"
    )

add_executable(bw_build ${BW_BUILD})
