cmake_minimum_required (VERSION 3.11)
project(bioInfo)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -pedantic -std=c++2a")

set(BW_BUILD
    "src/bw-build.cc"
    )

set(BW_SEARCH
    "src/bw-search.cc"
    )

add_executable(bw-build ${BW_BUILD})
add_executable(bw-search ${BW_SEARCH})
