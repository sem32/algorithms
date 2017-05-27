cmake_minimum_required(VERSION 3.3)
project(ijones)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        ijones.c)

add_executable(ijones ${SOURCE_FILES})
