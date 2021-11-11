add_library(algo euclidean_center.cpp sampling.h)
set(CGAL_DIR /usr/local/opt/cgal/lib/cmake/CGAL)
find_package(CGAL)
target_link_libraries(algo PUBLIC CGAL::CGAL)

