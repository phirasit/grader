cmake_minimum_required(VERSION 3.10)
project(grader)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O2")

include_directories(include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)

# find library
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# set marco defintions
if (NOT DEFINED BASE_DIR)
    set(BASE_DIR "/")
endif()

# add yaml-cpp
add_subdirectory(libs/yaml-cpp EXCLUDE_FROM_ALL)

# add pistache
set(CMAKE_BUILD_TYPE "Release")
add_subdirectory(libs/pistache EXCLUDE_FROM_ALL)
unset(CMAKE_BUILD_TYPE)

# add libseccomp
add_library(libseccomp STATIC IMPORTED)
set_target_properties(
        libseccomp PROPERTIES
        IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/libseccomp/src/.libs/libseccomp.a"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/libs/libseccomp/include"
)

file(GLOB main-lib-source src/*.cpp)
add_library(main-lib ${main-lib-source})
add_definitions(-DBASE_DIR=${BASE_DIR})
target_link_libraries(main-lib PUBLIC yaml-cpp)

# build setup script
add_executable(setup src/setup/setup.cpp)
target_link_libraries(setup PRIVATE main-lib)

# build sandbox
file(GLOB sandbox-source "src/sandbox/*.[ch]pp" "src/sandbox/*/*.[ch]pp")
add_executable(sandbox ${sandbox-source})
target_include_directories(sandbox PRIVATE libs/libseccomp/include)
target_link_libraries(sandbox PRIVATE main-lib libseccomp)

# build task-manager manager
file(GLOB task-manager-source "src/task-manager/*.[ch]pp")
add_executable(task ${task-manager-source})
target_link_libraries(task PRIVATE main-lib)

# build grader
file(GLOB grader-source "src/grader/*.[ch]pp")
add_library(grader-lib ${grader-source})
target_link_libraries(grader-lib PRIVATE main-lib)
file(GLOB grader-server-source "src/rest/*.[ch]pp")
add_executable(grader-server ${grader-server-source})
target_link_libraries(grader-server PRIVATE grader-lib Threads::Threads pistache)

# build test files
if (NOT DEFINED GRADER_BUILD_TESTING)
    option(GRADER_BUILD_TESTING "enable build testing program" OFF)
endif()
MESSAGE("Build test program: " ${GRADER_BUILD_TESTING})
if (GRADER_BUILD_TESTING)
    file(GLOB test-source "test/*.[ch]pp" "test/*/*.[ch]pp")
    list(FILTER test-source EXCLUDE REGEX "test/sample-.*/.*")
    add_executable(tests ${test-source})
    target_include_directories(tests PRIVATE test)
    target_link_libraries(tests PRIVATE main-lib grader-lib Threads::Threads)
endif()
