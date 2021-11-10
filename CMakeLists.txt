cmake_minimum_required(VERSION 3.14)
project(TestingOfClustering)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

# add the libraries
add_subdirectory(utils)

# add the libraries
add_subdirectory(algo)

# add the executable
set(CGAL_DIR  C:/dev/vcpkg/packages/cgal_x64-windows)
find_package(CGAL)
add_executable(TestingOfClustering main.cpp)

# add the link
target_link_libraries(TestingOfClustering PUBLIC utils algo)

## add the binary tree to the search path for include files
## so that we will find TutorialConfig.h
# target_include_directories(TestingOfClustering
#                          ${PROJECT_BINARY_DIR}
#                          ${PROJECT_SOURCE_DIR}/utils)
