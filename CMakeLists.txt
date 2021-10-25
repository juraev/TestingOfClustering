cmake_minimum_required(VERSION 3.14)
project(TestingOfClustering)

set(CMAKE_CXX_STANDARD 17)

# add the libraries
add_subdirectory(utils)

# add the executable
add_executable(TestingOfClustering main.cpp)

# add the link
target_link_libraries(TestingOfClustering PUBLIC utils)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(TestingOfClustering PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/utils")