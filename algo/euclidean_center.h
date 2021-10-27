//
// Created by gitarist on 27/10/21.
//

#ifndef TESTINGOFCLUSTERING_EUCLIDEAN_CENTER_H
#define TESTINGOFCLUSTERING_EUCLIDEAN_CENTER_H

#include <CGAL/Cartesian_d.h>
#include <CGAL/Random.h>
#include <CGAL/Exact_rational.h>
#include <CGAL/Min_sphere_of_spheres_d.h>
#include <vector>

typedef CGAL::Exact_rational              FT;
typedef CGAL::Cartesian_d<FT>             Space;
typedef Space::Point_d                    Cgal_Point;

namespace hidden{

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
        int N = points.size();

        for (int i = 0; i < N; ++i) {
            Cgal_Point p(D, points[i].cartesian_begin(), points[i].cartesian_end());
            S.push_back(Sphere1D(p, FT(0)));
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
        int N = points.size();

        for (int i = 0; i < N; ++i) {
            Cgal_Point p(D, points[i].cartesian_begin(), points[i].cartesian_end());
            S.push_back(Sphere2D(p, FT(0)));
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
        int N = points.size();

        for (int i = 0; i < N; ++i) {
            Cgal_Point p(D, points[i].cartesian_begin(), points[i].cartesian_end());
            S.push_back(Sphere3D(p, FT(0)));
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
        int N = points.size();

        for (int i = 0; i < N; ++i) {
            Cgal_Point p(D, points[i].cartesian_begin(), points[i].cartesian_end());
            S.push_back(Sphere4D(p, FT(0)));
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
        int N = points.size();

        for (int i = 0; i < N; ++i) {
            Cgal_Point p(D, points[i].cartesian_begin(), points[i].cartesian_end());
            S.push_back(Sphere5D(p, FT(0)));
        }

        Min_sphere5D ms(S.begin(), S.end());       // check in the spheres
        CGAL_assertion(ms.is_valid());

        double t = CGAL::to_double(ms.discriminant());
        double a = CGAL::to_double(ms.radius().first);
        double b = CGAL::to_double(ms.radius().second);

        return a + sqrt(t) * b;
    }
}

double radius(const std::vector<Cgal_Point>& points, const int D){

    switch (D) {
        case 1:
            return hidden::radiusnD<1>(points);

        case 2:
            return hidden::radiusnD<2>(points);

        case 3:
            return hidden::radiusnD<3>(points);

        case 4:
            return hidden::radiusnD<4>(points);

        case 5:
            return hidden::radiusnD<5>(points);

        default:
            std::runtime_error("Euclidean 1 center : Not impemented for D < 1 or D > 5.");
    }
    return 0.0;
}




#endif //TESTINGOFCLUSTERING_EUCLIDEAN_CENTER_H
