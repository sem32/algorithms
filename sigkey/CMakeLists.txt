cmake_minimum_required(VERSION 3.3)
project(sigkey)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        sigkey.c)

add_executable(sigkey ${SOURCE_FILES})
