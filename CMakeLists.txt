cmake_minimum_required(VERSION 3.10)
project(grader)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O2")

include_directories(include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# find library
find_package(Threads REQUIRED)

# set marco defintions
if (NOT DEFINED BASE_DIR)
    set(BASE_DIR "/")
endif()

# build main source files
add_subdirectory(libs/yaml-cpp EXCLUDE_FROM_ALL)

file(GLOB main-source src/*.cpp)
add_library(main ${main-source})
add_definitions(-DBASE_DIR=${BASE_DIR})
target_link_libraries(main PUBLIC yaml-cpp)

# build setup script
add_executable(setup src/setup/setup.cpp)
target_link_libraries(setup PRIVATE main)

# build sanbox
file(GLOB sandbox-source "src/sandbox/*.[ch]pp")
add_executable(sandbox ${sandbox-source})
target_link_libraries(sandbox PRIVATE main)

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

# build test files
if (NOT DEFINED GRADER_BUILD_TESTING)
    option(GRADER_BUILD_TESTING "enable build testing program" OFF)
endif()
MESSAGE("Build test progam: " ${GRADER_BUILD_TESTING})
if (GRADER_BUILD_TESTING)
    file(GLOB test-source "test/*.[ch]pp" "test/*/*.[ch]pp")
        list(FILTER test-source EXCLUDE REGEX "test/sample-.*/.*")
    add_executable(tests ${test-source})
    target_include_directories(tests PRIVATE test)
    target_link_libraries(tests PRIVATE main Threads::Threads)
endif()
