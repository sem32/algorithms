cmake_minimum_required(VERSION 3.3)
project(lngpok2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    lngpok.c)

add_executable(lngpok2 ${SOURCE_FILES})