//
// Created by gitarist on 27/10/21.
//

#ifndef TESTINGOFCLUSTERING_EUCLIDEAN_CENTER_H
#define TESTINGOFCLUSTERING_EUCLIDEAN_CENTER_H

#include <vector>
#include <functional>

#include <CGAL/Cartesian_d.h>
#include <CGAL/Random.h>
#include <CGAL/Exact_rational.h>
#include <CGAL/Min_sphere_of_spheres_d.h>

typedef CGAL::Exact_rational              FT;
typedef CGAL::Cartesian_d<FT>             Space;
typedef Space::Point_d                    Cgal_Point;

namespace {

    typedef CGAL::Min_sphere_of_spheres_d_traits_d<Space,FT,1>  Traits1D;
    typedef CGAL::Min_sphere_of_spheres_d<Traits1D>             Min_sphere1D;
    typedef Traits1D::Sphere                                    Sphere1D;

    typedef CGAL::Min_sphere_of_spheres_d_traits_d<Space,FT,2>  Traits2D;
    typedef CGAL::Min_sphere_of_spheres_d<Traits2D>             Min_sphere2D;
    typedef Traits2D::Sphere                                    Sphere2D;


    typedef CGAL::Min_sphere_of_spheres_d_traits_d<Space,FT,3>  Traits3D;
    typedef CGAL::Min_sphere_of_spheres_d<Traits3D>             Min_sphere3D;
    typedef Traits3D::Sphere                                    Sphere3D;


    typedef CGAL::Min_sphere_of_spheres_d_traits_d<Space,FT,4>  Traits4D;
    typedef CGAL::Min_sphere_of_spheres_d<Traits4D>             Min_sphere4D;
    typedef Traits4D::Sphere                                    Sphere4D;


    typedef CGAL::Min_sphere_of_spheres_d_traits_d<Space,FT,5>  Traits5D;
    typedef CGAL::Min_sphere_of_spheres_d<Traits5D>             Min_sphere5D;
    typedef Traits5D::Sphere                                    Sphere5D;

    template<int D>
    double radiusnD(const std::vector<Cgal_Point> &points) {

    }

    template<>
    double radiusnD<1>(const std::vector<Cgal_Point> &points) {
        int D = 1;
        std::vector<Sphere1D> S;                  // n spheres
        int N = static_cast<int>(points.size());

        for (int i = 0; i < N; ++i) {
            S.emplace_back(points[i], FT(0));
        }

        Min_sphere1D ms(S.begin(), S.end());       // check in the spheres
        CGAL_assertion(ms.is_valid());

        double t = CGAL::to_double(ms.discriminant());
        double a = CGAL::to_double(ms.radius().first);
        double b = CGAL::to_double(ms.radius().second);

        return a + sqrt(t) * b;
    }

    template<>
    double radiusnD<2>(const std::vector<Cgal_Point> &points) {
        int D = 2;
        std::vector<Sphere2D> S;                  // n spheres
        int N = static_cast<int>(points.size());

        for (int i = 0; i < N; ++i) {
            S.emplace_back(points[i], FT(0));
        }

        Min_sphere2D ms(S.begin(), S.end());       // check in the spheres
        CGAL_assertion(ms.is_valid());

        double t = CGAL::to_double(ms.discriminant());
        double a = CGAL::to_double(ms.radius().first);
        double b = CGAL::to_double(ms.radius().second);

        return a + sqrt(t) * b;
    }

    template<>
    double radiusnD<3>(const std::vector<Cgal_Point> &points) {
        int D = 1;
        std::vector<Sphere3D> S;                  // n spheres
        int N = static_cast<int>(points.size());

        for (int i = 0; i < N; ++i) {
            S.emplace_back(points[i], FT(0));
        }

        Min_sphere3D ms(S.begin(), S.end());       // check in the spheres
        CGAL_assertion(ms.is_valid());

        double t = CGAL::to_double(ms.discriminant());
        double a = CGAL::to_double(ms.radius().first);
        double b = CGAL::to_double(ms.radius().second);

        return a + sqrt(t) * b;
    }

    template<>
    double radiusnD<4>(const std::vector<Cgal_Point> &points) {
        int D = 1;
        std::vector<Sphere4D> S;                  // n spheres
        int N = static_cast<int>(points.size());

        for (int i = 0; i < N; ++i) {
            S.emplace_back(points[i], FT(0));
        }

        Min_sphere4D ms(S.begin(), S.end());       // check in the spheres
        CGAL_assertion(ms.is_valid());

        double t = CGAL::to_double(ms.discriminant());
        double a = CGAL::to_double(ms.radius().first);
        double b = CGAL::to_double(ms.radius().second);

        return a + sqrt(t) * b;
    }

    template<>
    double radiusnD<5>(const std::vector<Cgal_Point> &points) {
        int D = 1;
        std::vector<Sphere5D> S;                  // n spheres
        int N = static_cast<int>(points.size());

        for (int i = 0; i < N; ++i) {
            S.emplace_back(points[i], FT(0));
        }

        Min_sphere5D ms(S.begin(), S.end());       // check in the spheres
        CGAL_assertion(ms.is_valid());

        double t = CGAL::to_double(ms.discriminant());
        double a = CGAL::to_double(ms.radius().first);
        double b = CGAL::to_double(ms.radius().second);

        return a + sqrt(t) * b;
    }
}

double radius(const std::vector<Cgal_Point>& points, int D);

using std::vector;
using Point = vector<double>;
using Dist = std::function<double(const Point&, const Point&, int)>;

bool euclidean_k_center(const vector<Point>& sample, int k, double b, const Dist& dist, int d);



#endif //TESTINGOFCLUSTERING_EUCLIDEAN_CENTER_H
