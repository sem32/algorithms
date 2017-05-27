cmake_minimum_required(VERSION 3.3)
project(bugtrk)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        bugtrk.c)

add_executable(bugtrk ${SOURCE_FILES})
