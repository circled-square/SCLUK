if(DEFINED scluk_GUARD)
    if(NOT scluk_GUARD STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
        return() # Project has been already included by someone else
    endif()
else()
    set(scluk_GUARD ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "scluk guard")
endif()


cmake_minimum_required(VERSION 3.25)
project(scluk LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

add_library(scluk STATIC src/exception.cpp src/log.cpp src/math.cpp src/range.cpp)
target_include_directories(scluk PUBLIC "include/")

