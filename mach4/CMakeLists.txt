cmake_minimum_required(VERSION 2.8)

# compiled program name
project (main)

# compileflags
set(CMAKE_XSS_FLAGS "${CMAKE_XSS_FLAGS} - Wall -Wextra -Wpedantic -std=c++11")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add files
include_directories(../lib-mpi)

file (GLOB_RECURSE PROJECT_HEADERS src/*.hpp
                                   src/*.h
                                   ../lib-mpi/*.h)
file (GLOB_RECURSE PROJECT_SOURCES src/*.cpp
                                   src/*.cxx
                                   ../lib-mpi/*.cpp)

# Organizing files
source_group ("headers" FILES ${PROJECT_HEADERS})
source_group ("sources" FILES ${PROJECT_SOURCES})

# set main as output executable
add_executable (${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})

# add and link MPI
find_package(MPI REQUIRED)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})
target_link_libraries(${PROJECT_NAME} ${MPI_C_LIBRARIES})

# add and link OpenMP
find_package(OpenMP REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenMP_C_FLAGS})

# test run program with make run
add_custom_target(run
    mpirun --use-hwthread-cpus -np 4  ./${PROJECT_NAME} 100000000
)
