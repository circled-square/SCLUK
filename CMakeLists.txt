cmake_minimum_required(VERSION 3.25)
project(scluk LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SCLUK_SOURCE_FILES "src/*.cpp")

add_library(scluk STATIC ${SCLUK_SOURCE_FILES})
target_include_directories(scluk PUBLIC "include/")

