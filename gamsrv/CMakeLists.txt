cmake_minimum_required(VERSION 3.3)
project(gamsrv)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        gamsrv.c)

add_executable(gamsrv ${SOURCE_FILES})
