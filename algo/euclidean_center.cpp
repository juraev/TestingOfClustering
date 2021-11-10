//
// Created by gitarist on 31/10/21.
//
#include "euclidean_center.h"
#include <stdexcept>

double radius(const std::vector<Cgal_Point>& points, const int D){

    switch (D) {
        case 1:
            return radiusnD<1>(points);

        case 2:
            return radiusnD<2>(points);

        case 3:
            return radiusnD<3>(points);

        case 4:
            return radiusnD<4>(points);

        case 5:
            return radiusnD<5>(points);

        default:
            throw std::runtime_error("Euclidean 1 center : Not impemented for D < 1 or D > 5.");
    }
    return 0.0;
}

bool euclidean_k_center(const vector<Point> &sample, int k, double b, const Dist &dist, int d) {
    int n = static_cast<int>(sample.size());

    auto check = [&](vector<int> &part) {
        vector<vector<Point>> partitions(k);

        for (int i = 0; i < n; i++) {
            partitions[part[i]].push_back(sample[i]);
        }
        for (auto P : partitions) {
            if (P.empty()) continue;

            int c = static_cast<int>(P.size());

            for (int i = 0; i < c; i++) {
                for (int j = i + 1; j < c; j++) {
                    if (dist(P[i], P[j], d) > b) return false;
                }
            }
        }
        return true;
    };


    std::function<bool(int, int, vector<int> &)> rec = [&](int N, int K, vector<int> &part) {
        if (N == n) {
            return check(part);
        }

        for (int i = 0; i < K; i++) {
            part[N] = i;
            if (rec(N + 1, K, part)) {
                return true;
            }
        }
        return false;
    };

    auto part = vector<int>(n, 0);
    return rec(0, k, part);
}