cmake_minimum_required(VERSION 3.17)
project(TestingOfClustering)

set(CMAKE_CXX_STANDARD 17)
set(CGAL_DIR /usr/local/opt/cgal/lib/cmake/CGAL)
set(CMAKE_BUILD_TYPE Release)
find_package(CGAL)

add_executable(TestingOfClustering main.cpp)
target_link_libraries(TestingOfClustering CGAL::CGAL)
