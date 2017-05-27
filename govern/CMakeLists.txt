cmake_minimum_required(VERSION 3.3)
project(govern)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        govern.c)

add_executable(govern ${SOURCE_FILES})
