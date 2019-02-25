cmake_minimum_required(VERSION 3.0)

# compiled program name
project (main)

# compileflags
set(CMAKE_XSS_FLAGS "${CMAKE_XSS_FLAGS} - Wall -Wextra -Wpedantic -std=c++11")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add files
include_directories(../lib0)

file (GLOB_RECURSE PROJECT_HEADERS src/*.hpp
                                   src/*.h
                                   ../lib0/*.h)
file (GLOB_RECURSE PROJECT_SOURCES src/*.cpp
                                   src/*.cxx
                                   ../lib0/*.cpp)

# Organizing files
source_group ("headers" FILES ${PROJECT_HEADERS})
source_group ("sources" FILES ${PROJECT_SOURCES})

# set main as output executable
add_executable (${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})

# test run program with make run
add_custom_target(run
    ./${PROJECT_NAME} 100000
)

# unit tests
add_custom_target(utest
    ./${PROJECT_NAME} -u
)

# verification tests
add_custom_target(vtest
    ./${PROJECT_NAME} -v
)
