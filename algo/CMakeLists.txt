add_library(algo euclidean_center.cpp sampling.h)
# set(CGAL_DIR /usr/local/opt/cgal/lib/cmake/CGAL)
set(CGAL_DIR  C:/dev/vcpkg/packages/cgal_x64-windows)
find_package(CGAL)
target_link_libraries(algo CGAL::CGAL)

