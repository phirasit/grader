cmake_minimum_required(VERSION 3.10)
project(grader)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(src)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/libs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# set marco defintions
if (NOT DEFINED BASE_DIR)
    set(BASE_DIR "/")
endif()

# build main source files
file(GLOB main_source src/*.cpp)
add_library(main ${main_source})
target_link_libraries(main PRIVATE yaml-cpp stdc++fs)
add_definitions(-DBASE_DIR=${BASE_DIR})

# build setup script
add_executable(setup src/setup/setup.cpp)
target_link_libraries(setup PRIVATE main)

# build sanbox
add_executable(sandbox src/sandbox/sandbox.cpp)

# build task-manager manager
file(GLOB task-manager-source "src/task-manager/*.[ch]pp")
add_executable(task ${task-manager-source})
target_link_libraries(task PRIVATE main)

# build grader
file(GLOB grader-source "src/grader/*.[ch]pp")
add_executable(grader ${grader-source})

    # add pthread
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)

target_link_libraries(grader PRIVATE main Threads::Threads)

